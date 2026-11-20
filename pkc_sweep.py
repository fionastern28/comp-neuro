#!/usr/bin/env python3
"""
Ab-fiber input rate  ->  PKC neuron firing rate.   [1-comp / _cn model]

Builds the network from your existing cfg / netParams files, then, WITHOUT
editing those files, strips the network down to:
    populations : Ab input pop (default Ab) + target pop (default PKC)
    connections : only  Ab pop -> target pop   (AMPA + NMDA for this model)
Every other population (PV, VGLUT3, ...) and connection (incl. PV -> PKC
inhibition) is removed for these runs, so this is the DIRECT Ab -> PKC
input/output curve.

The number of Ab fibers comes from cfg.numAb (via netParams).

For each Ab rate, runs N trials (default 4). Each trial uses its own seed
for connectivity and for the Poisson Ab spike trains. Trial k uses the
same connectivity seed at every rate, so the curves are paired across rates.

Each single run writes one small CSV row file:
    <out>/<model>_rate<R>_trial<T>.csv
with columns: model, input_pop, ab_rate_hz, trial, target_pop, target_rate_hz,
              n_target_cells, n_target_spikes, win_start_ms, win_end_ms, duration_ms

Run from inside the model folder (the one with cfg_mechanical_cn.py,
netParams_mechanical_cn.py, spkt_gen_cn.py and the compiled mods):

  # whole sweep, one core
  python3 ab_pkc_sweep.py

  # custom rates / trials
  python3 ab_pkc_sweep.py --rates 0 1 2 5 10 20 40 --trials 4

  # one run (for running in parallel, see below)
  python3 ab_pkc_sweep.py --rate 10 --trial 2

  # other cfg/netParams files or pop names
  python3 ab_pkc_sweep.py --model 1-comp --cfg cfg_mechanical_cn.py \
      --netparams netParams_mechanical_cn.py --inpop Ab --target PKC

  PARALLEL (20 cores), default rates x 4 trials:
  mkdir -p logs
  for r in $(python3 -c "import pkc_sweep as s; print(*s.DEFAULT_RATES)"); do for t in 0 1 2 3; do echo "$r $t"; done; done | \
  xargs -P 20 -n 2 sh -c 'python3 pkc_sweep.py --rate $0 --trial $1 --duration 6000 --win 2000 6000 > logs/log_pkc_r$0_t$1.txt 2>&1'
"""
import argparse
import csv
import importlib
import os
import sys

DEFAULT_RATES = [0, 1, 2, 5, 7, 10, 12, 15, 17, 20, 25, 30, 35, 40, 45, 50, 60, 70, 80, 90]   # Hz

# make the model folder (cwd) importable
sys.path.insert(0, os.getcwd())


def parse_args():
    ap = argparse.ArgumentParser(description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--model", default="1-comp", help="label stored in the CSV (used for overlaying)")
    ap.add_argument("--cfg", default="cfg_mechanical_cn.py")
    ap.add_argument("--netparams", default="netParams_mechanical_cn.py")
    ap.add_argument("--inpop", default="Ab", help="Ab-fiber input population label")
    ap.add_argument("--target", default="PKC", help="population whose firing rate is measured")
    ap.add_argument("--rates", type=float, nargs="+", default=DEFAULT_RATES,
                    help="Ab Poisson rates (Hz) for the full sweep")
    ap.add_argument("--trials", type=int, default=4, help="trials per rate")
    ap.add_argument("--rate", type=float, help="run only this rate (use with --trial)")
    ap.add_argument("--trial", type=int, help="run only this trial index (0-based)")
    ap.add_argument("--duration", type=float, default=10000.0, help="sim length (ms)")
    ap.add_argument("--win", type=float, nargs=2, default=[2000.0, 10000.0],
                    metavar=("START", "END"),
                    help="window (ms) for measuring target firing rate; skips the start-up transient")
    ap.add_argument("--out", default="data/ab_pkc_sweep", help="folder for the per-run CSVs")
    ap.add_argument("--save-json", action="store_true",
                    help="also save the full NetPyNE JSON for every run (large)")
    a, _ = ap.parse_known_args()
    if (a.rate is None) != (a.trial is None):
        ap.error("--rate and --trial must be given together")
    if a.win[1] > a.duration:
        ap.error("--win END cannot be later than --duration")
    return a


def module_name(path):
    """'cfg_mechanical_cn.py' -> 'cfg_mechanical_cn'"""
    return os.path.splitext(os.path.basename(path))[0]


def poisson_trains(rate, n_cells, t_stop, seed_base):
    """One independent Poisson train per cell (seed = seed_base + i).
    rate 0 -> one spike parked past the end of the sim (same trick netParams
    uses, since VecStim wants a non-empty vector), so the fiber is silent."""
    from spkt_gen_cn import poisson_generator
    if rate <= 0:   # poisson_generator divides by rate
        return [[t_stop * 10.0] for _ in range(n_cells)]
    return [poisson_generator(rate=rate, t_start=0, t_stop=t_stop,
                              seed=seed_base + i).tolist()
            for i in range(n_cells)]


def pop_of(conds):
    return conds.get("popLabel", conds.get("pop"))


def run_one(a, rate, trial):
    from netpyne import sim

    # netParams does `from cfg_mechanical_cn import cfg`, so reload cfg first
    # (fresh object each run), set duration + Ab rate, then reload netParams
    # so both share the same cfg.
    cfgmod = importlib.reload(importlib.import_module(module_name(a.cfg)))
    cfg = cfgmod.cfg
    cfg.duration = a.duration
    cfg.inputRate = rate          # keeps cfg consistent; trains are overwritten below anyway
    npmod = importlib.reload(importlib.import_module(module_name(a.netparams)))
    netParams = npmod.netParams

    # ---- keep only the Ab pop and the target pop ----
    for p in list(netParams.popParams.keys()):
        if p not in (a.inpop, a.target):
            del netParams.popParams[p]
    if a.inpop not in netParams.popParams or a.target not in netParams.popParams:
        sys.exit(f"Need populations '{a.inpop}' and '{a.target}' in {a.netparams} "
                 f"(is the PKC popParams line uncommented?)")

    # ---- keep only  Ab -> target  connections ----
    kept = []
    for c in list(netParams.connParams.keys()):
        cp = netParams.connParams[c]
        if pop_of(cp["preConds"]) == a.inpop and pop_of(cp["postConds"]) == a.target:
            kept.append(c)
        else:
            del netParams.connParams[c]
    for k in list(netParams.stimSourceParams.keys()):
        del netParams.stimSourceParams[k]
    for k in list(netParams.stimTargetParams.keys()):
        del netParams.stimTargetParams[k]
    if not kept:
        sys.exit(f"No {a.inpop} -> {a.target} connections found in {a.netparams}")
    print(f"Connections kept: {kept}")

    # ---- Ab input at the requested rate ----
    n_in = netParams.popParams[a.inpop]["numCells"]
    seed_base = 1_000_000 * (trial + 1) + int(round(rate * 1000)) * 100
    netParams.popParams[a.inpop]["spkTimes"] = poisson_trains(rate, n_in, a.duration, seed_base)

    # ---- sim settings for a lean sweep ----
    cfg.simLabel = f"{a.model}_{a.target}_rate{rate:g}_trial{trial}"
    cfg.saveFolder = a.out
    cfg.seeds = {"conn": 4311 + trial, "stim": 1214 + trial, "loc": 4121 + trial}
    cfg.analysis = {}          # no raster/trace plots popping up
    cfg.recordTraces = {}      # no voltage traces (saves memory)
    cfg.saveJson = bool(a.save_json)
    cfg.savePickle = False
    cfg.printPopAvgRates = list(a.win)
    cfg.verbose = False

    sim.create(netParams=netParams, simConfig=cfg)
    sim.simulate()
    sim.analyze()

    # ---- target firing rate in the analysis window ----
    gids = set(sim.net.allPops[a.target]["cellGids"])
    spkt = list(sim.allSimData["spkt"])
    spkid = list(sim.allSimData["spkid"])
    t0, t1 = a.win
    n_spk = sum(1 for i, t in zip(spkid, spkt) if int(i) in gids and t0 <= t < t1)
    target_rate = n_spk / len(gids) / ((t1 - t0) / 1000.0)

    os.makedirs(a.out, exist_ok=True)
    path = os.path.join(a.out, f"{a.model}_{a.target}_rate{rate:g}_trial{trial}.csv")
    with open(path, "w", newline="") as f:
        w = csv.writer(f)
        w.writerow(["model", "input_pop", "ab_rate_hz", "trial", "target_pop", "target_rate_hz",
                    "n_target_cells", "n_target_spikes", "win_start_ms", "win_end_ms",
                    "duration_ms"])
        w.writerow([a.model, a.inpop, rate, trial, a.target, target_rate, len(gids), n_spk,
                    t0, t1, a.duration])
    print(f"[{a.model}] Ab={rate:g} Hz trial {trial}: {a.target} = {target_rate:.3f} Hz -> {path}")

    sim.clearAll()
    return target_rate


def main():
    a = parse_args()
    sys.argv = [sys.argv[0]]   # stop netpyne seeing this script's own args
    if a.rate is not None:
        run_one(a, a.rate, a.trial)
        return
    for rate in a.rates:
        for trial in range(a.trials):
            print(f"\n=== {a.model}: Ab {rate:g} Hz -> {a.target}, trial {trial + 1}/{a.trials} ===")
            run_one(a, rate, trial)


if __name__ == "__main__":
    main()
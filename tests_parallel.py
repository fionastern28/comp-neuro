'''
Parallel version of tests_cn.py: runs nRuns independent simulations on
several cores at once, then averages each cell's firing rate across runs.

Each run is its own Python process (fresh NEURON), so runs can't interfere.
Seeds are the same as tests_cn.py (spktSeed = 1703 + 10*run), so the Ab input
matches the serial version. (C-fiber trains in netParams use seed=None, so
those are random every run, in both versions.)

Run from inside the model folder (the one with the compiled mods):
    python3 tests_cn_parallel.py               # uses (cores - 1) workers
    python3 tests_cn_parallel.py --workers 4   # choose the number of cores
'''
import os
os.environ.setdefault('MPLBACKEND', 'Agg')   # no plot windows from worker processes

import argparse
import multiprocessing as mp
import sys
from collections import defaultdict

import numpy as np

nRuns = 20
SIM_DURATION = 3000  # ms
SIM_DURATION_sec = SIM_DURATION / 1000.0
INPUT_RATE = 10


def run_one(run):
    '''One simulation (same steps as the loop body in tests_cn.py).
    Returns (run, {gid: rate_Hz}, {gid: pop}).'''
    import importlib
    sys.path.insert(0, os.getcwd())
    sys.argv = [sys.argv[0]]          # keep netpyne from reading our args
    from netpyne import sim
    from neuron import h

    cfgmod = importlib.import_module('cfg_mechanical_cn')
    cfg = cfgmod.cfg

    cfg.inputRate = float(INPUT_RATE)
    cfg.spktSeed = 1703 + 10 * run
    cfg.duration = SIM_DURATION
    cfg.simLabel = f"Circuit_Trial_run{run}"

    npmod = importlib.import_module('netParams_mechanical_cn')
    importlib.reload(npmod)

    sim.create(netParams=npmod.netParams, simConfig=cfg)

    # --- wire ExcCap pointers on PV/VGLUT3/PKC/DYN cells (same as tests_cn.py) ---
    for cell in sim.net.cells:
        if cell.tags.get('pop') not in ('PV', 'VGLUT3', 'PKC', 'DYN'):
            continue
        excCap = cell.secs['soma']['pointps']['ExcCap']['hObj']
        for sm in cell.secs['soma']['synMechs']:
            if sm['label'] == 'AMPA':
                h.setpointer(excCap._ref_iacc_ampa, 'iacc', sm['hObj'])
            elif sm['label'] == 'NMDA':
                h.setpointer(excCap._ref_iacc_nmda, 'iacc', sm['hObj'])
    # --- end wiring ---

    sim.simulate()
    sim.analyze()

    all_gids = [c.gid for c in sim.net.cells]
    pop_of = {c.gid: c.tags['pop'] for c in sim.net.cells}

    spike_counts = {gid: 0 for gid in all_gids}
    for gid in sim.allSimData['spkid']:
        spike_counts[int(gid)] += 1

    rates = {gid: spike_counts[gid] / SIM_DURATION_sec for gid in all_gids}

    sim.clearAll()
    return run, rates, pop_of


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--workers', type=int,
                    default=max(1, (os.cpu_count() or 2) - 1),
                    help='number of runs at once (default: cores - 1)')
    a = ap.parse_args()
    n_workers = min(a.workers, nRuns)
    print(f"{nRuns} runs on {n_workers} cores")

    firing_rates = defaultdict(list)
    pop_of = {}

    # 'spawn' = each worker starts a clean Python; maxtasksperchild=1 = a brand-new
    # process (and NEURON) for every run, so nothing carries over between runs.
    ctx = mp.get_context('spawn')
    with ctx.Pool(processes=n_workers, maxtasksperchild=1) as pool:
        for done, (run, rates, pops) in enumerate(pool.imap_unordered(run_one, range(nRuns)), 1):
            print(f"Run {run} finished ({done}/{nRuns})")
            for gid, r in rates.items():
                firing_rates[gid].append(r)
            pop_of.update(pops)

    # Average firing rate across runs
    avg_firing_rate = {gid: np.mean(r) for gid, r in firing_rates.items()}

    print("Average firing rates (Hz):")
    for gid in sorted(avg_firing_rate):
        print(f"Cell {gid} ({pop_of[gid]}): {avg_firing_rate[gid]:.2f} Hz")


if __name__ == '__main__':
    main()
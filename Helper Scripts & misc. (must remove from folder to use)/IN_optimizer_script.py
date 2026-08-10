"""
NetPyNE version: optimize a single-compartment INcellRule to match BOTH:
  (1) the voltage trace shape of the 3-compartment INcellRule at TRACE_AMP, and
  (2) the F-I (firing rate vs. current) curve across a range of amplitudes.

This is the INHIBITORY / fast-spiking variant (the "SG cell" of the prior
model). Unlike the PRO cell, the IN cell has NO calcium machinery: it uses the
Borg-Graham style Na/K channels (B_A, B_DR, B_Na) plus KDR / KDRI delayed
rectifiers and a leak (pas). There are no ca/can ions.

The single compartment is derived from the soma, and its searchable parameters
are constrained to lie BETWEEN the soma and AIS (hillock) values:
    B_Na.gnabar : soma 0.008  <-> AIS 3.45
    KDRI.gkbar  : soma 0.0043 <-> AIS 0.076
    pas.g       : 1.1e-05 in both      -> widened (see BOUNDS)
    KDR.gkbar   : fixed at 0.0 (0 throughout the baseline; NOT searchable)
(SS appears only in the dendrite, so it is intentionally left out of the
soma-based 1-compartment model.)

Requires: netpyne, NEURON (with B_A, B_DR, B_Na, KDR, KDRI, SS, pas mechanisms
compiled via nrnivmodl in this directory), numpy, scipy, matplotlib.
(SS is only needed to build the 3-comp target; the 1-comp candidate does not
use it, but the .mod must still be compiled for the target to load.)

Setup:
    nrnivmodl mods/            # compile mechanisms once, if not already done

HOW TO RUN (recommended -- in VSCode / Jupyter, cell by cell):
    This file is split into `# %%` cells. Run them top to bottom:
      Cell 1  -- setup: imports + all definitions (fast, just registers funcs)
      Cell 2  -- build 3-comp target + its F-I curve
      Cell 3  -- BENCHMARK: times one fast sim, prints projected full-run time
      Cell 4  -- run the optimizer (THE SLOW PART -- only run when ready)
      Cell 5  -- results + plots
    Running cells 1-3 first lets you see the time estimate BEFORE committing
    to the long optimization in cell 4.

    (Running the whole file as `python3 optimize_1comp_IN_netpyne.py` will
    execute everything including the full optimization -- can take a long time.)
"""

# %%
# =========================================================================
# CELL 1 -- SETUP: imports + all definitions (fast; defines run_1comp_fast etc.)
# =========================================================================
import numpy as np
from scipy.optimize import differential_evolution
import matplotlib.pyplot as plt
from netpyne import specs, sim
import contextlib
import io
import time


@contextlib.contextmanager
def suppress_output():
    """Silence NetPyNE/NEURON's verbose stdout AND stderr during repeated sim
    calls (NetPyNE's tqdm progress bars go to stderr)."""
    buf_out, buf_err = io.StringIO(), io.StringIO()
    with contextlib.redirect_stdout(buf_out), contextlib.redirect_stderr(buf_err):
        yield


# =========================================================================
# CURRENT-INJECTION CONFIG  (fast-spiking IN: higher / wider range than PRO)
# -- These are the main knobs to tune to your recorded F-I data. --
# =========================================================================
TRACE_AMP = 0.15                       # nA, suprathreshold amp for shape matching
FI_AMPS   = np.arange(0.0, 0.31, 0.02)  # nA, F-I sweep (0 -> 0.3 nA, matches data)
V_INIT    = -70.0                      # mV, matches pas.e (leak reversal)

# Onset/rheobase matching: the objective naturally lets large-Hz points at
# high current dominate the sum of squared errors, sacrificing accuracy near
# threshold. Upweight the first few (low-current, near-rheobase) F-I points
# so the optimizer can't trade away onset accuracy for a better high-end fit.
FI_LOW_AMP_N      = 4     # number of low-current points to upweight
FI_LOW_AMP_WEIGHT = 6.0   # weight multiplier applied to those points

# Depolarization-block penalty: pushing B_Na_gnabar up to hit the target's
# high firing rates can inactivate Na and cause the candidate to collapse to
# near-zero firing at high current where the target is still firing robustly.
# The base squared-error term already penalizes this, but not enough to stop
# it -- so any F-I point where the target is clearly firing but the candidate
# has collapsed gets an extra multiplier on top of its squared error.
BLOCK_TARGET_MIN_HZ    = 10.0   # target rate (Hz) at/above which it counts as "robustly firing"
BLOCK_CANDIDATE_MAX_HZ = 5.0    # candidate rate (Hz) at/below which it counts as "blocked"
BLOCK_PENALTY_WEIGHT   = 15.0   # extra multiplier applied to those points' squared error

# =========================================================================
# 1) Reference 3-compartment cellRule (target) -- the inhibitory IN cell
# =========================================================================
INcellRule_3comp = {  # was termed SG cell in prior model
    'conds': {},
    'globals': {},
    'secLists': {},
    'secs': {
        'dend': {
            'geom': {'L': 1371.0, 'nseg': 50, 'diam': 1.4, 'Ra': 80.0, 'cm': 1.0},
            'ions': {
                'k': {'e': -84.0, 'i': 54.4, 'o': 2.5},
                'na': {'e': 60.0, 'i': 10.0, 'o': 140.0}},
            'mechs': {
                'B_DR': {},
                'KDR': {'gkbar': 0.0},
                'KDRI': {'gkbar': 0.034},
                'SS': {'gnabar': 0.0},
                'pas': {'g': 1.1e-05, 'e': -70.0}},
            'topol': {'parentSec': 'soma', 'parentX': 0.0, 'childX': 0.0}},
        'hillock': {
            'geom': {'L': 30.0, 'Ra': 80.0, 'cm': 1.0, 'diam': 0.742, 'nseg': 30},
            'ions': {
                'k': {'e': -84.0, 'i': 54.4, 'o': 2.5},
                'na': {'e': 60.0, 'i': 10.0, 'o': 140.0}},
            'mechs': {
                'B_A': {},
                'B_DR': {},
                'B_Na': {'gnabar': 3.45, 'alpha_shift': 0.0, 'beta_shift': 0.0},
                'KDR': {'gkbar': 0.0},
                'KDRI': {'gkbar': 0.076},
                'pas': {'g': 1.1e-05, 'e': -70.0}},
            'topol': {'parentSec': 'soma', 'parentX': 1.0, 'childX': 0.0}},
        'soma': {
            'geom': {'L': 10.0, 'nseg': 10, 'diam': 10.0, 'Ra': 80.0, 'cm': 1.0},
            'ions': {
                'k': {'e': -84.0, 'i': 54.4, 'o': 2.5},
                'na': {'e': 60.0, 'i': 10.0, 'o': 140.0}},
            'mechs': {
                'B_A': {},
                'B_DR': {},
                'B_Na': {'gnabar': 0.008, 'alpha_shift': 0.0, 'beta_shift': 0.0},
                'KDR': {'gkbar': 0.0},
                'KDRI': {'gkbar': 0.0043},
                'pas': {'g': 1.1e-05, 'e': -70.0}},
            'topol': {}}}
}

# =========================================================================
# 2) Parameter names / bounds for the 1-compartment candidate, constrained
#    to lie between the 3-compartment soma and AIS (hillock) values.
#    pas.g is identical in soma & AIS, so (like pas in the PRO script) it is
#    given a widened searchable range. KDR.gkbar is 0 throughout the baseline
#    and is held fixed at 0.0 (not searchable).
# =========================================================================
PARAM_NAMES = [
    'B_Na_gnabar',   # soma 0.008  <-> AIS 3.45
    'KDRI_gkbar',    # soma 0.0043 <-> AIS 0.076
    'pas_g',         # 1.1e-05 in both -> widened
]

BOUNDS = [
    (0.008,   3.45),     # B_Na_gnabar (soma=0.008, AIS=3.45)
    (0.0043,  0.076),    # KDRI_gkbar  (soma=0.0043, AIS=0.076)
    (5e-06,   4e-04),    # pas_g       (widened well past baseline 1.1e-05 --
                          # 3-comp total membrane area is ~20x the bare soma,
                          # so matching input resistance needs leak density
                          # around 1.1e-05 * 20 =~ 2.2e-04; range covers that
                          # with margin on both sides)
]


def make_1comp_rule(params):
    """Build an INcellRule-style dict for the 1-compartment candidate."""
    p = dict(zip(PARAM_NAMES, params))
    return {
        'conds': {},
        'globals': {},
        'secLists': {},
        'secs': {
            'soma': {
                'geom': {'L': 10.0, 'nseg': 1, 'diam': 10.0, 'Ra': 80.0, 'cm': 1.0},
                'ions': {
                    'k': {'e': -84.0, 'i': 54.4, 'o': 2.5},
                    'na': {'e': 60.0, 'i': 10.0, 'o': 140.0}},
                'mechs': {
                    'B_A': {},
                    'B_DR': {},
                    'B_Na': {'gnabar': p['B_Na_gnabar'], 'alpha_shift': 0.0,
                             'beta_shift': 0.0},
                    'KDR': {'gkbar': 0.0},
                    'KDRI': {'gkbar': p['KDRI_gkbar']},
                    'pas': {'g': p['pas_g'], 'e': -70.0}},
                'topol': {}}}
    }


# =========================================================================
# 3a) SLOW path: full NetPyNE rebuild (used only for the 3-comp target,
#     which runs a handful of times, so per-call overhead doesn't matter)
# =========================================================================
def run_netpyne_sim(cell_rule, amp=TRACE_AMP, delay=100, dur=400, tstop=500):
    netParams = specs.NetParams()
    netParams.cellParams['INcellRule'] = cell_rule

    netParams.popParams['testPop'] = {
        'cellType': 'INcellRule',
        'numCells': 1,
        'cellModel': 'HH'
    }
    netParams.cellParams['INcellRule']['conds'] = {'cellType': 'INcellRule'}

    netParams.stimSourceParams['IClamp1'] = {
        'type': 'IClamp', 'delay': delay, 'dur': dur, 'amp': amp
    }
    netParams.stimTargetParams['IClamp1->testPop'] = {
        'source': 'IClamp1',
        'conds': {'pop': 'testPop'},
        'sec': 'soma', 'loc': 0.5
    }

    simConfig = specs.SimConfig()
    simConfig.duration = tstop
    simConfig.dt = 0.025
    simConfig.verbose = False
    simConfig.recordStep = 0.025
    simConfig.saveJson = False
    simConfig.printPopAvgRates = False
    simConfig.progressBar = False
    simConfig.createNEURONObj = True
    simConfig.createPyStruct = True
    simConfig.analysis = {}
    simConfig.hParams['v_init'] = V_INIT   # init at leak reversal (-70)

    with suppress_output():
        sim.create(netParams=netParams, simConfig=simConfig)
        from neuron import h
        cell = sim.net.cells[0]
        soma_sec = cell.secs['soma']['hObj']
        v_vec = h.Vector().record(soma_sec(0.5)._ref_v)
        t_vec = h.Vector().record(h._ref_t)
        sim.simulate()

    t = np.array(t_vec.to_python())
    v = np.array(v_vec.to_python())
    return t, v


# =========================================================================
# 3b) FAST path: build the 1-compartment candidate ONCE via NetPyNE, then
#     for every subsequent evaluation just mutate its mechanism parameters
#     and re-run. This avoids the expensive sim.create() teardown/rebuild on
#     every one of the tens of thousands of optimizer evaluations.
# =========================================================================
_CANDIDATE = {'built': False}


def _build_candidate_once():
    """Build the single-compartment candidate cell one time and cache handles
    to its NEURON section, IClamp, and recording vectors."""
    from neuron import h
    h.load_file('stdrun.hoc')  # provides finitialize/continuerun run-control

    # Build via NetPyNE (no stim -- we attach our own IClamp for full control)
    placeholder = make_1comp_rule([b[0] for b in BOUNDS])  # any in-bounds values
    placeholder['conds'] = {'cellType': 'INcand'}

    netParams = specs.NetParams()
    netParams.cellParams['INcand'] = placeholder
    netParams.popParams['candPop'] = {
        'cellType': 'INcand', 'numCells': 1, 'cellModel': 'HH'
    }

    simConfig = specs.SimConfig()
    simConfig.duration = 500
    simConfig.dt = 0.025
    simConfig.verbose = False
    simConfig.saveJson = False
    simConfig.printPopAvgRates = False
    simConfig.progressBar = False
    simConfig.createNEURONObj = True
    simConfig.createPyStruct = True
    simConfig.analysis = {}
    simConfig.hParams['v_init'] = V_INIT

    with suppress_output():
        sim.create(netParams=netParams, simConfig=simConfig)
        soma_sec = sim.net.cells[0].secs['soma']['hObj']

        # our own IClamp on the section -- amp/delay/dur set per call
        iclamp = h.IClamp(soma_sec(0.5))
        iclamp.delay = 100
        iclamp.dur = 400
        iclamp.amp = 0.0

        v_vec = h.Vector().record(soma_sec(0.5)._ref_v)
        t_vec = h.Vector().record(h._ref_t)

    _CANDIDATE.update({
        'built': True,
        'h': h,
        'soma_sec': soma_sec,
        'seg': soma_sec(0.5),
        'iclamp': iclamp,
        'v_vec': v_vec,
        't_vec': t_vec,
    })


def run_1comp_fast(params, amp=TRACE_AMP, delay=100, dur=400, tstop=500):
    """Fast candidate evaluation: update mechanism params on the persistent
    section, set stim, run, return (t, v)."""
    if not _CANDIDATE['built']:
        _build_candidate_once()

    h = _CANDIDATE['h']
    seg = _CANDIDATE['seg']
    iclamp = _CANDIDATE['iclamp']

    p = dict(zip(PARAM_NAMES, params))

    # update mechanism parameters in place
    seg.B_Na.gnabar = p['B_Na_gnabar']
    seg.KDRI.gkbar = p['KDRI_gkbar']
    seg.pas.g = p['pas_g']
    # (KDR.gkbar stays fixed at 0.0; B_A and B_DR carry no free scalar params)

    # set stimulus
    iclamp.delay = delay
    iclamp.dur = dur
    iclamp.amp = amp

    # run
    h.finitialize(V_INIT)
    h.continuerun(tstop)

    t = np.array(_CANDIDATE['t_vec'].to_python())
    v = np.array(_CANDIDATE['v_vec'].to_python())
    return t, v


# =========================================================================
# 4) Feature extraction (shape descriptors, not raw point-by-point voltage,
#    so exact spike-timing alignment doesn't dominate the error)
# =========================================================================
def extract_features(t, v, spike_thresh=0.0, stim_start=100):
    features = {}
    pre_mask = t < stim_start - 10
    features['v_rest'] = np.mean(v[pre_mask]) if pre_mask.any() else v[0]

    above = v > spike_thresh
    spike_idxs = []
    i = 0
    while i < len(above):
        if above[i]:
            j = i
            while j < len(above) and above[j]:
                j += 1
            peak_idx = i + np.argmax(v[i:j])
            spike_idxs.append(peak_idx)
            i = j
        else:
            i += 1

    if len(spike_idxs) < 2:
        features['n_spikes'] = len(spike_idxs)
        features['mean_isi'] = 1e6
        features['peak_v'] = np.max(v) if len(v) else 0
        features['trough_v'] = np.min(v) if len(v) else 0
        features['spike_width'] = 0
        features['firing_rate'] = 0.0
        return features

    spike_times = t[spike_idxs]
    isis = np.diff(spike_times)

    features['n_spikes'] = len(spike_idxs)
    features['mean_isi'] = np.mean(isis)
    features['firing_rate'] = 1000.0 / features['mean_isi']  # Hz
    features['peak_v'] = np.mean(v[spike_idxs])

    troughs = []
    for k in range(len(spike_idxs) - 1):
        seg = v[spike_idxs[k]:spike_idxs[k + 1]]
        troughs.append(np.min(seg))
    features['trough_v'] = np.mean(troughs)

    widths = []
    for idx in spike_idxs:
        half = (v[idx] + features['trough_v']) / 2
        left = idx
        while left > 0 and v[left] > half:
            left -= 1
        right = idx
        while right < len(v) - 1 and v[right] > half:
            right += 1
        widths.append(t[right] - t[left])
    features['spike_width'] = np.mean(widths)

    return features


def feature_vector(features):
    return np.array([
        features['v_rest'],
        features['mean_isi'],
        features['peak_v'],
        features['trough_v'],
        features['spike_width'],
        features['n_spikes'],
    ])


# =========================================================================
# 5) F-I curve computation
# =========================================================================
def compute_fi_curve_target(cell_rule, amps=FI_AMPS,
                            delay=100, dur=400, tstop=500):
    """F-I curve for the 3-comp target using the slow full-rebuild path
    (only runs once, so overhead is fine)."""
    rates = []
    for amp in amps:
        t, v = run_netpyne_sim(cell_rule, amp=amp, delay=delay, dur=dur, tstop=tstop)
        feats = extract_features(t, v, stim_start=delay)
        rates.append(feats['firing_rate'])
    return np.array(rates)


def compute_fi_curve_fast(params, amps=FI_AMPS,
                          delay=100, dur=400, tstop=500):
    """F-I curve for a candidate using the fast persistent-section path."""
    rates = []
    for amp in amps:
        t, v = run_1comp_fast(params, amp=amp, delay=delay, dur=dur, tstop=tstop)
        feats = extract_features(t, v, stim_start=delay)
        rates.append(feats['firing_rate'])
    return np.array(rates)


# =========================================================================
# 6) Objective function combining shape match (TRACE_AMP) + F-I curve match
# =========================================================================
def make_objective(target_features_trace, target_fi_curve):
    target_vec = feature_vector(target_features_trace)
    shape_weights = np.array([1.0, 0.05, 1.0, 1.0, 2.0, 5.0])

    fi_weights = np.ones_like(FI_AMPS)
    fi_weights[:FI_LOW_AMP_N] = FI_LOW_AMP_WEIGHT

    def objective(params):
        try:
            t, v = run_1comp_fast(params, amp=TRACE_AMP)
            feats = extract_features(t, v)
            vec = feature_vector(feats)
            shape_cost = np.sum(((vec - target_vec) * shape_weights) ** 2)

            fi_curve = compute_fi_curve_fast(params, amps=FI_AMPS)
            diffs_sq = (fi_curve - target_fi_curve) ** 2

            point_weights = fi_weights.copy()
            block_mask = (target_fi_curve >= BLOCK_TARGET_MIN_HZ) & \
                         (fi_curve <= BLOCK_CANDIDATE_MAX_HZ)
            point_weights[block_mask] *= BLOCK_PENALTY_WEIGHT

            fi_cost = np.sum(point_weights * diffs_sq)

            return float(shape_cost + 0.5 * fi_cost)
        except Exception as e:
            print("  [objective failed]:", e)
            return 1e8

    return objective


# %%
# =========================================================================
# CELL 2 -- BUILD TARGET: run 3-comp reference at TRACE_AMP + compute its F-I
#           curve. Run once.
# =========================================================================
print(f"Running 3-compartment IN reference model (NetPyNE) at {TRACE_AMP} nA...")
t_target, v_target = run_netpyne_sim(INcellRule_3comp, amp=TRACE_AMP)
target_features_trace = extract_features(t_target, v_target)
print(f"Target shape features ({TRACE_AMP} nA):", target_features_trace)

print(f"Computing 3-compartment F-I curve across {len(FI_AMPS)} amplitudes...")
target_fi_curve = compute_fi_curve_target(INcellRule_3comp, amps=FI_AMPS)
print("Target F-I curve (Hz):", target_fi_curve)


# %%
# =========================================================================
# CELL 3 -- BENCHMARK: time a single fast candidate sim and project how long
#           the full optimization will take. Run this BEFORE the optimizer so
#           you know what you're committing to.
# =========================================================================
# warm-up (first call builds the persistent candidate cell)
run_1comp_fast([b[0] for b in BOUNDS], amp=TRACE_AMP)

N_BENCH = 10
t0 = time.time()
for _ in range(N_BENCH):
    run_1comp_fast([b[0] for b in BOUNDS], amp=TRACE_AMP)
per_sim = (time.time() - t0) / N_BENCH

# each objective() call = 1 shape sim + len(FI_AMPS) F-I sims
sims_per_obj = 1 + len(FI_AMPS)
# differential_evolution: ~popsize*ndim candidates/gen, ~maxiter gens
POPSIZE, MAXITER, NDIM = 12, 40, len(BOUNDS)
est_objcalls = POPSIZE * NDIM * MAXITER
est_total_sims = est_objcalls * sims_per_obj
est_seconds = est_total_sims * per_sim

print(f"Per-sim time:            {per_sim*1000:.1f} ms")
print(f"Sims per objective call: {sims_per_obj}")
print(f"Est. objective calls:    ~{est_objcalls:,}")
print(f"Est. total simulations:  ~{est_total_sims:,}")
print(f"Est. full-run time:      ~{est_seconds/60:.1f} min "
      f"(~{est_seconds/3600:.1f} hours)")
print("\nIf that's too long: reduce FI_AMPS points, POPSIZE, and/or MAXITER "
      "in the optimizer cell below.")


# %%
# =========================================================================
# CELL 4 -- OPTIMIZE (THE SLOW PART). Only run when you're happy with the
#           time estimate from CELL 3. Adjust maxiter/popsize/FI_AMPS to
#           trade speed vs. fit quality.
# =========================================================================
objective = make_objective(target_features_trace, target_fi_curve)

progress_state = {'gen': 0}

def progress_callback(xk, convergence):
    # Throttled progress: prints every few generations instead of the
    # per-iteration firehose. convergence is already computed (free to print).
    progress_state['gen'] += 1
    gen = progress_state['gen']
    if gen == 1 or gen % 5 == 0:
        print(f"  [gen {gen}] convergence: {convergence:.4f}")
    return False  # returning True would stop optimization early

# NOTE: workers=1 -- NEURON keeps global hoc state, so parallel workers are
# NOT safe here without per-worker NEURON isolation (MPI). Keep at 1.
result = differential_evolution(
    objective,
    BOUNDS,
    maxiter=40,      # lower for a faster/rougher first pass
    popsize=12,      # lower for a faster/rougher first pass
    tol=1e-6,
    seed=42,
    workers=1,
    polish=True,
    disp=False,
    callback=progress_callback,
)

print("\nOptimization complete.")
print("Best cost:", result.fun)
best_params = dict(zip(PARAM_NAMES, result.x))
print("Best parameters:")
for k, val in best_params.items():
    print(f"  {k}: {val:.6g}")


# %%
# =========================================================================
# CELL 5 -- RESULTS: compare optimized 1-comp vs. 3-comp target, plot, and
#           print the ready-to-paste INcellRule dict.
# =========================================================================
# Final comparison: trace shape (fast path -- same model that was optimized)
t_opt, v_opt = run_1comp_fast(result.x, amp=TRACE_AMP)
opt_features = extract_features(t_opt, v_opt)
print("Optimized 1-comp shape features:", opt_features)
print("Target 3-comp shape features:   ", target_features_trace)

# Final comparison: F-I curve
opt_fi_curve = compute_fi_curve_fast(result.x, amps=FI_AMPS)
print("\nOptimized F-I curve (Hz):", opt_fi_curve)
print("Target F-I curve (Hz):   ", target_fi_curve)

# Plots
fig, axes = plt.subplots(1, 2, figsize=(14, 5))

axes[0].plot(t_target, v_target, label='3 compartment (target)', linewidth=1.5)
axes[0].plot(t_opt, v_opt, '--', label='1 compartment (optimized)', linewidth=1.5)
axes[0].set_xlabel('Time (ms)')
axes[0].set_ylabel('Voltage (mV)')
axes[0].set_title(f'Voltage Trace ({TRACE_AMP} nA)')
axes[0].legend()
axes[0].grid(True)

axes[1].plot(FI_AMPS, target_fi_curve, 'o-', label='3 compartment (target)')
axes[1].plot(FI_AMPS, opt_fi_curve, 's--', label='1 compartment (optimized)')
axes[1].set_xlabel('Current Amplitude (nA)')
axes[1].set_ylabel('Firing Frequency (Hz)')
axes[1].set_title('F-I Curve Comparison')
axes[1].legend()
axes[1].grid(True)

plt.tight_layout()
plt.savefig('optimized_comparison_IN_netpyne.png', dpi=150)
plt.show()

# Print final cellRule dict, NetPyNE-style, ready to paste in
print("\n--- Ready-to-use INcellRule (1-compartment, NetPyNE format) ---")
print(f"""
INcellRule_1comp = {{
 'conds': {{}},
 'globals': {{}},
 'secLists': {{}},
 'secs': {{'soma': {{'geom': {{'L': 10.0, 'nseg': 1, 'diam': 10.0, 'Ra': 80.0, 'cm': 1.0}},
 'ions': {{'k': {{'e': -84.0, 'i': 54.4, 'o': 2.5}},
 'na': {{'e': 60.0, 'i': 10.0, 'o': 140.0}}}},
 'mechs': {{'B_A': {{}},
 'B_DR': {{}},
 'B_Na': {{'gnabar': {best_params['B_Na_gnabar']:.6g}, 'alpha_shift': 0.0, 'beta_shift': 0.0}},
 'KDR': {{'gkbar': 0.0}},
 'KDRI': {{'gkbar': {best_params['KDRI_gkbar']:.6g}}},
 'pas': {{'g': {best_params['pas_g']:.6g}, 'e': -70.0}}}},
 'topol': {{}}}}}}
}}
""")
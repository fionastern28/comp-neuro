'''
Overlay: Medlock population average (mean +/- SD) with individual comp-neuro runs.
Run from the comp-neuro folder:  python3 overlay_cn_medlock.py
'''
import os
import json
import numpy as np
import matplotlib.pyplot as plt

# ---- settings -------------------------------------------------------------
CN_RUNS   = [1]#list(range(20))     # which comp-neuro runs to use (Circuit_Trial_run<N>_data.json)
CN_MODE   = 'average'  # 'individual' = one trace per run
                          # 'average'    = mean +/- SD across the runs in CN_RUNS
                          # 'both'       = faint individual runs + mean +/- SD
CN_DIR    = 'data'
ML_FILE   = os.path.expanduser(
    '~/Desktop/medlock-comparison-model/data/Circuit_Trial_Medlock_run0_data.json')
OUT_PNG   = 'data/overlay_cn_medlock.png'
# ---------------------------------------------------------------------------

POPS = ['PKC', 'PV']


def load(path):
    d = json.load(open(path))
    sd = d['simData']
    t = np.array(sd['t'])
    traces = {}
    for pop in POPS:
        gids = d['net']['pops'][pop]['cellGids']
        traces[pop] = [np.array(sd['v_soma'][f'cell_{g}']) for g in gids
                       if f'cell_{g}' in sd['v_soma']]
    return t, traces


# Medlock: average across cells
t_ml, ml = load(ML_FILE)
for pop in POPS:
    print(f'Medlock {pop}: {len(ml[pop])} cells averaged')

# comp-neuro: individual runs
cn = []
for r in CN_RUNS:
    path = os.path.join(CN_DIR, f'Circuit_Trial_run{r}_data.json')
    t, tr = load(path)
    cn.append((r, t, tr))
    print(f'comp-neuro run {r}: {t[-1] + (t[1] - t[0]):.0f} ms')

if CN_MODE in ('average', 'both') and len({len(t) for _, t, _ in cn}) > 1:
    raise SystemExit('comp-neuro runs have different durations; average only runs with the same cfg.duration')

if CN_MODE not in ('individual', 'average', 'both'):
    raise SystemExit("CN_MODE must be 'individual', 'average' or 'both'")

# plot only the time range both models cover
t_end = min(t_ml[-1], min(t[-1] for _, t, _ in cn))
if t_end < max(t_ml[-1], max(t[-1] for _, t, _ in cn)):
    print(f'NOTE: durations differ; plotting the shared range 0-{t_end:.0f} ms')

fig, axes = plt.subplots(2, 1, figsize=(12, 7), sharex=True)
cn_colors = plt.cm.Blues(np.linspace(0.45, 0.95, len(cn)))

for ax, pop in zip(axes, POPS):
    keep = t_ml <= t_end
    if ml[pop]:
        arr = np.array(ml[pop])[:, keep]
        m, s = arr.mean(0), arr.std(0)
        ax.fill_between(t_ml[keep], m - s, m + s, color='#d1495b', alpha=0.2, lw=0, zorder=3)
        ax.plot(t_ml[keep], m, color='#d1495b', lw=1.2, zorder=4,
                label=f'Medlock mean ± SD (n={len(arr)} cells)')
    if CN_MODE in ('individual', 'both'):
        faint = CN_MODE == 'both'
        for (r, t, tr), c in zip(cn, cn_colors):
            for v in tr[pop]:
                k = t <= t_end
                ax.plot(t[k], v[k], color=c, lw=0.5 if faint else 0.6,
                        alpha=0.35 if faint else 1.0,
                        label=None if faint else f'comp-neuro run {r}')
    if CN_MODE in ('average', 'both'):
        t0 = cn[0][1]
        k = t0 <= t_end
        runs = [v[k] for (_, t, tr) in cn for v in tr[pop]]
        arr = np.array(runs)
        cm, cs = arr.mean(0), arr.std(0)
        ax.fill_between(t0[k], cm - cs, cm + cs, color='#2a6fdb', alpha=0.2, lw=0, zorder=5)
        ax.plot(t0[k], cm, color='#2a6fdb', lw=1.2, zorder=6,
                label=f'comp-neuro mean ± SD (n={len(arr)} runs)')
    ax.set_title(f'{pop} soma voltage')
    ax.set_ylabel('V (mV)')
    ax.legend(loc='upper right', fontsize=8, frameon=False)

axes[-1].set_xlabel('Time (ms)')
axes[-1].set_xlim(0, t_end)
fig.tight_layout()
fig.savefig(OUT_PNG, dpi=200)
print(f'Saved {OUT_PNG}')
#plt.show()
'''
Overlay F-I curves from two or more models. No NEURON, no netpyne -- just reads
the CSVs written by fi_sweep_cn.py.

Put this in a fresh folder together with the CSVs and run:

    python plot_fi_overlay.py

With no arguments it picks up every fi_*.csv in the folder, sorted by filename.
To control curve order (legend order, and the sign of the difference panel),
pass the paths explicitly:

    python plot_fi_overlay.py fi_mechanical.csv fi_model_B.csv

Outputs fi_overlay.png and fi_summary.csv.
'''

import os
import sys
import csv
import glob
from collections import OrderedDict, defaultdict

import numpy as np
import matplotlib
matplotlib.use('Agg')          # comment out for interactive figures
import matplotlib.pyplot as plt


# ---------------------------------------------------------------------------
COLORS = ['#1f77b4', '#d62728', '#2ca02c', '#9467bd', '#ff7f0e', '#8c564b']
PLOT_DIFFERENCE = True      # lower panel, only when exactly 2 models and seeds match
X_AS_TOTAL = False          # True -> x-axis is total afferent rate (rate * num_ab)
DODGE = 0.006               # fractional x-offset so error bars don't overlap
FIGNAME = 'fi_overlay.png'
SUMNAME = 'fi_summary.csv'


def load(path):
    '''Return (label, meta, {input_rate: {seed: output_rate}}).'''
    data = defaultdict(dict)
    meta = {}
    fallback = os.path.splitext(os.path.basename(path))[0]
    if fallback.startswith('fi_'):
        fallback = fallback[3:]

    with open(path, newline='') as f:
        for row in csv.DictReader(f):
            meta.setdefault('label', row.get('model') or fallback)
            meta.setdefault('pop', row.get('pop', '?'))
            meta.setdefault('num_ab', float(row.get('num_ab', 'nan')))
            meta.setdefault('duration_ms', row.get('duration_ms', '?'))
            meta.setdefault('transient_ms', row.get('transient_ms', '?'))

            key = ('total_input_rate_Hz' if X_AS_TOTAL
                   else 'input_rate_per_afferent_Hz')
            data[float(row[key])][int(row['seed'])] = float(row['output_rate_Hz'])

    if not data:
        sys.exit('{} contains no data rows'.format(path))
    return meta['label'], meta, data


def check_comparable(metas, curves):
    '''Print warnings for anything that makes the comparison unfair.'''
    labels = list(curves)

    n_abs = {lab: metas[lab]['num_ab'] for lab in labels}
    paired = True
    if len(set(n_abs.values())) > 1:
        print('WARNING: differing num_ab {} -- identical seeds do NOT give '
              'identical input trains, so this is not paired.'.format(n_abs))
        paired = False

    for field, name in (('duration_ms', 'duration'), ('transient_ms', 'transient')):
        vals = {lab: metas[lab][field] for lab in labels}
        if len(set(vals.values())) > 1:
            print('WARNING: differing {} {} -- rate estimates use different '
                  'measurement windows.'.format(name, vals))

    rate_sets = {lab: set(curves[lab]) for lab in labels}
    if len(set(map(frozenset, rate_sets.values()))) > 1:
        only = {lab: sorted(rate_sets[lab] - set.intersection(*rate_sets.values()))
                for lab in labels}
        print('note: input rates differ between models; extras = {}'.format(
            {k: v for k, v in only.items() if v}))

    seed_sets = {lab: set().union(*(set(curves[lab][r]) for r in curves[lab]))
                 for lab in labels}
    if len(set(map(frozenset, seed_sets.values()))) > 1:
        print('WARNING: differing seeds {} -- falling back to unpaired '
              'comparison.'.format({k: sorted(v) for k, v in seed_sets.items()}))
        paired = False

    return paired


def main(paths):
    if not paths:
        paths = sorted(p for p in glob.glob('fi_*.csv')
                       if os.path.basename(p) != SUMNAME)
    if not paths:
        sys.exit('no fi_*.csv found here -- copy the sweep outputs into this folder')

    curves, metas = OrderedDict(), {}
    for p in paths:
        label, meta, data = load(p)
        if label in curves:
            label = '{} ({})'.format(label, os.path.basename(p))
            meta['label'] = label
        curves[label] = data
        metas[label] = meta
        print('loaded {:<28s} {:>3d} points, {} rates'.format(
            os.path.basename(p),
            sum(len(v) for v in data.values()), len(data)))

    paired = check_comparable(metas, curves)

    # per-curve stats over that curve's own rates
    stats = {}
    for label, data in curves.items():
        xs = np.array(sorted(data), dtype=float)
        vals = [list(data[r].values()) for r in sorted(data)]
        stats[label] = {
            'x': xs,
            'mean': np.array([np.mean(v) for v in vals]),
            'sd': np.array([np.std(v, ddof=1) if len(v) > 1 else 0.0 for v in vals]),
            'n': int(np.median([len(v) for v in vals])),
        }

    all_x = np.concatenate([s['x'] for s in stats.values()])
    span = max(all_x.max(), 1.0)

    can_diff = PLOT_DIFFERENCE and len(curves) == 2 and paired
    if can_diff:
        fig, (ax, axd) = plt.subplots(2, 1, figsize=(5.8, 6.4), sharex=True,
                                      gridspec_kw={'height_ratios': [2.4, 1]})
    else:
        fig, ax = plt.subplots(figsize=(5.8, 4.4))
        axd = None

    for i, (label, s) in enumerate(stats.items()):
        offset = (i - (len(stats) - 1) / 2.0) * DODGE * span
        ax.errorbar(s['x'] + offset, s['mean'], yerr=s['sd'], fmt='o-',
                    color=COLORS[i % len(COLORS)], markersize=5, capsize=3,
                    linewidth=1.5, elinewidth=1,
                    label='{} (n={})'.format(label, s['n']))

    pops = {metas[l]['pop'] for l in curves}
    ax.set_ylabel('{} firing rate (Hz)'.format(pops.pop() if len(pops) == 1
                                               else 'output'))
    ax.set_title('F-I curves' + ('  (paired seeds)' if paired else ''))
    ax.grid(axis='y', linestyle='--', linewidth=0.6, alpha=0.45)
    ax.legend(frameon=False)
    ax.spines['top'].set_visible(False)
    ax.spines['right'].set_visible(False)
    ax.set_xlim(left=0)
    ax.set_ylim(bottom=0)

    if can_diff:
        a, b = list(curves)
        shared_x = sorted(set(curves[a]) & set(curves[b]))
        dx, dmean, dsd = [], [], []
        for r in shared_x:
            seeds = sorted(set(curves[a][r]) & set(curves[b][r]))
            if not seeds:
                continue
            d = np.array([(curves[b][r][s] - curves[a][r][s]) for s in seeds])
            dx.append(r)
            dmean.append(d.mean())
            dsd.append(d.std(ddof=1) if len(d) > 1 else 0.0)

        axd.axhline(0, color='0.6', linewidth=0.8, zorder=0)
        axd.grid(axis='y', linestyle='--', linewidth=0.6, alpha=0.45)
        axd.errorbar(dx, dmean, yerr=dsd, fmt='s-', color='0.25', markersize=4,
                     capsize=3, linewidth=1.2, elinewidth=1)
        axd.set_ylabel('Difference (Hz)'.format(b, a), fontsize=9)
        axd.spines['top'].set_visible(False)
        axd.spines['right'].set_visible(False)
        target = axd
    else:
        target = ax

    target.set_xlabel('total Ab input rate (Hz)' if X_AS_TOTAL
                      else 'Ab input rate per afferent (Hz)')
    fig.tight_layout()
    fig.savefig(FIGNAME, dpi=300)

    with open(SUMNAME, 'w', newline='') as f:
        w = csv.writer(f)
        w.writerow(['model', 'input_rate_Hz', 'mean_Hz', 'sd_Hz', 'n'])
        for label, s in stats.items():
            for xi, m, sd in zip(s['x'], s['mean'], s['sd']):
                w.writerow([label, xi, m, sd, s['n']])

    print('\nsaved {} and {}'.format(FIGNAME, SUMNAME))


if __name__ == '__main__':
    main(sys.argv[1:])
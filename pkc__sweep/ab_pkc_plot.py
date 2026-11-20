#!/usr/bin/env python3
"""
Overlay PKC-rate vs Ab-fiber-rate curves from one or more models.

Reads every *_rate*_trial*.csv with an 'ab_rate_hz' column (written by
ab_pkc_sweep.py [1-comp] and ab_pkc_sweep_sdh.py [SDH]) in the folders you give,
groups by the 'model' column, and plots mean +/- SD (sample, n-1) for each model
with individual trials as faint dots. Files without 'ab_rate_hz' (e.g. C-fiber
sweep CSVs) are skipped.

Usage:
    python3 plot_ab_pkc_curves.py data/ab_pkc_sweep
    python3 plot_ab_pkc_curves.py /path/SDHmodel/data/ab_pkc_sweep \
        /path/comp-neuro/data/ab_pkc_sweep --out comparison_ab
Outputs (in --out, default = first folder):
    ab_pkc_curves.png
    ab_pkc_curves_summary.csv     (model, ab_rate_hz, mean, sd, n, n_input_fibers)
    ab_pkc_curves_all_trials.csv
"""
import argparse
import csv
import glob
import os
import statistics
from collections import defaultdict

import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt

COLORS = ["#1f4e79", "#c0504d", "#4f8f3a", "#8064a2", "#e08a1e", "#2a9d8f"]


def main():
    ap = argparse.ArgumentParser(description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("folders", nargs="+")
    ap.add_argument("--out")
    a = ap.parse_args()
    out = a.out or a.folders[0]
    os.makedirs(out, exist_ok=True)

    rows, skipped = [], 0
    for folder in a.folders:
        for p in sorted(glob.glob(os.path.join(folder, "*_rate*_trial*.csv"))):
            with open(p) as f:
                for r in csv.DictReader(f):
                    if "ab_rate_hz" in r:
                        rows.append(r)
                    else:
                        skipped += 1
    if skipped:
        print(f"[skip] {skipped} rows without an 'ab_rate_hz' column")
    if not rows:
        raise SystemExit("No Ab-sweep CSVs found.")

    data = defaultdict(lambda: defaultdict(list))   # model -> rate -> [target rates]
    fibers = {}                                     # model -> n_input_fibers (if recorded)
    targets = sorted({r["target_pop"] for r in rows})
    target = "/".join(targets)
    for r in rows:
        data[r["model"]][float(r["ab_rate_hz"])].append(float(r["target_rate_hz"]))
        if r.get("n_input_fibers"):
            fibers[r["model"]] = r["n_input_fibers"]

    # all-trials table (union of columns, since the 1-comp CSVs lack n_input_fibers)
    fields = []
    for r in rows:
        for k in r:
            if k not in fields:
                fields.append(k)
    with open(os.path.join(out, "ab_pkc_curves_all_trials.csv"), "w", newline="") as f:
        w = csv.DictWriter(f, fieldnames=fields)
        w.writeheader()
        w.writerows(rows)

    fig, ax = plt.subplots(figsize=(6.5, 4.5))
    summary = []
    for k, model in enumerate(sorted(data)):
        col = COLORS[k % len(COLORS)]
        rates = sorted(data[model])
        means, sds = [], []
        for x in rates:
            v = data[model][x]
            m = statistics.mean(v)
            s = statistics.stdev(v) if len(v) > 1 else float("nan")
            means.append(m); sds.append(s)
            summary.append([model, x, m, s, len(v), fibers.get(model, "")])
            ax.scatter([x] * len(v), v, s=14, color=col, alpha=0.35, zorder=2)
        label = f"{model} (mean ± SD)"
        if model in fibers:
            label = f"{model}, {fibers[model]} Ab fibers (mean ± SD)"
        ax.errorbar(rates, means, yerr=sds, fmt="o-", color=col, capsize=4,
                    lw=1.8, ms=6, zorder=3, label=label)
        print(f"\n{model}")
        for x, m, s in zip(rates, means, sds):
            print(f"  Ab {x:>6g} Hz -> {target} {m:8.3f} ± {s:6.3f} Hz (n={len(data[model][x])})")

    with open(os.path.join(out, "ab_pkc_curves_summary.csv"), "w", newline="") as f:
        w = csv.writer(f)
        w.writerow(["model", "ab_rate_hz", f"mean_{target}_rate_hz", "sd", "n", "n_input_fibers"])
        w.writerows(summary)

    ax.set_xlabel("Ab-fiber input rate (Hz)")
    ax.set_ylabel(f"{target} firing rate (Hz)")
    ax.set_title(f"{target} output vs. Ab-fiber input")
    ax.legend(frameon=False)
    ax.spines[["top", "right"]].set_visible(False)
    fig.tight_layout()
    png = os.path.join(out, "ab_pkc_curves.png")
    fig.savefig(png, dpi=200)
    print(f"\nSaved {png}")


if __name__ == "__main__":
    main()
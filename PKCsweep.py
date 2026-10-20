'''
Sweep the Ab (A-beta) input firing rate and record the PKC firing-rate response.

For each Ab input rate we set cfg.inputRate (and cfg.numAb / cfg.spktSeed) and
reload netParams so it redraws the Ab Poisson spike trains internally (see
make_poisson_pop() in netParams_mechanical_cn.py), then (re)build and run the
network for a number of trials, measure the PKC population firing rate, and
plot PKC rate vs Ab input rate.
'''

import numpy as np
import matplotlib.pyplot as plt

from netpyne import sim
from neuron import h
import importlib


# ----------------------------- sweep settings -----------------------------
# Ab input rates to test, in Hz. NOTE: avoid 0 Hz -- poisson_generator divides
# by the rate (1.0/rate), so a 0 will blow up. Edit range/step as you like.
ab_rates = [5, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80, 85, 90]

trials_per_rate = 3      # runs averaged per Ab rate (Ab spikes are random each run)
num_ab_cells    = 4      # must match numCells for the 'Ab' pop in netParams
SIM_DURATION    = 2000   # ms  (matches cfg.duration)
seed            = None   # None -> a different Poisson realization every run

SIM_DURATION_sec = SIM_DURATION / 1000.0

NETPARAMS_MODULE  = 'netParams_mechanical_cn'    # module basename, no .py
CFG_MODULE        = 'cfg_mechanical_cn'
# MAX_SLOTS removed 2026-08-07 by Claude (Cowork): ExcCap no longer uses a
# fixed-size pointer array (see SynCurrentCap.mod), so there's no synapse
# count limit to configure here.
# --------------------------------------------------------------------------


def pkc_rate_from_sim():
    """PKC population firing rate (Hz) from the just-completed simulation."""
    pkc_gids = set(sim.net.pops['PKC'].cellGids)
    n_pkc = max(len(pkc_gids), 1)
    spkids = list(sim.allSimData.get('spkid', []))
    n_spikes = sum(1 for gid in spkids if gid in pkc_gids)
    return n_spikes / (n_pkc * SIM_DURATION_sec)


mean_pkc_rate = []
sem_pkc_rate  = []

for ab_rate in ab_rates:
    trial_rates = []
    for trial in range(trials_per_rate):
        print(f"Ab = {ab_rate} Hz   trial {trial + 1}/{trials_per_rate}")

        cfgmod = importlib.import_module(CFG_MODULE)
        cfg = cfgmod.cfg

        # netParams builds the Ab population directly from cfg.inputRate /
        # cfg.numAb / cfg.spktSeed (make_poisson_pop in netParams_mechanical_cn.py),
        # so set those here BEFORE reloading netParams to draw fresh trains.
        cfg.inputRate = ab_rate
        cfg.numAb = num_ab_cells
        cfg.spktSeed = seed

        npmod = importlib.import_module(NETPARAMS_MODULE)
        importlib.reload(npmod)

        # Skip per-run plotting/saving during the sweep (faster, no popups).
        cfg.analysis = {}
        cfg.saveJson = False
        cfg.savePickle = False

        sim.create(netParams=npmod.netParams, simConfig=cfg)

        # --- wire ExcCap pointers on PV/VGLUT3/PKC cells ---
        # EDITED by Claude (Cowork) on 2026-08-07 21:31 EDT
        # PURPOSE: ExcCap now uses a push-accumulator design (see SynCurrentCap.mod)
        # instead of a fixed 20-slot pointer array, so there's no MAX_SLOTS limit,
        # no zero-padding of unused slots, and no assert needed -- just one
        # setpointer call per synapse, pointing it at ExcCap's iacc_ampa/iacc_nmda.
        for cell in sim.net.cells:
            if cell.tags.get('pop') not in ('PV', 'VGLUT3', 'PKC'):
                continue
            excCap = cell.secs['soma']['pointps']['ExcCap']['hObj']

            ampaSyns = [sm for sm in cell.secs['soma']['synMechs'] if sm['label'] == 'AMPA']
            nmdaSyns = [sm for sm in cell.secs['soma']['synMechs'] if sm['label'] == 'NMDA']

            for syn in ampaSyns:
                h.setpointer(excCap._ref_iacc_ampa, 'iacc', syn['hObj'])
            for syn in nmdaSyns:
                h.setpointer(excCap._ref_iacc_nmda, 'iacc', syn['hObj'])
        # --- end wiring ---

        sim.simulate()    
        trial_rates.append(pkc_rate_from_sim())

    trial_rates = np.array(trial_rates)
    mean_pkc_rate.append(trial_rates.mean())
    # standard error of the mean across trials (0 if only one trial)
    sem_pkc_rate.append(
        trial_rates.std(ddof=1) / np.sqrt(len(trial_rates)) if len(trial_rates) > 1 else 0.0
    )
    print(f"  -> PKC mean rate = {mean_pkc_rate[-1]:.2f} Hz")


# ------------------------------- plot -------------------------------------
ab_rates      = np.array(ab_rates)
mean_pkc_rate = np.array(mean_pkc_rate)
sem_pkc_rate  = np.array(sem_pkc_rate)

fig, ax = plt.subplots(figsize=(6, 4.5))
ax.errorbar(ab_rates, mean_pkc_rate, yerr=sem_pkc_rate,
            marker='o', capsize=3, linewidth=1.5)
ax.set_xlabel('Ab input firing rate (Hz)')
ax.set_ylabel('PKC firing rate (Hz)')
ax.set_title(f'PKC response vs Ab input  ({trials_per_rate} trials/point)')
ax.grid(True, alpha=0.3)
fig.tight_layout()
fig.savefig('pkc_vs_ab_rate.png', dpi=150)
print("saved figure -> pkc_vs_ab_rate.png")
plt.show()
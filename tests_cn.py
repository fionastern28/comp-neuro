'''
Script for testing various parts of the model
come back to the meanings of what i have here...
eventually this will likely be the init
'''
'''
from netpyne import sim
from neuron import h
from spkt_gen_cn import generate_new_SPKT

for i in range(5):
    generate_new_SPKT(4, 10, 0, 5000, None)
                        
    simConfig, netParams = sim.readCmdLineArgs(simConfigDefault='cfg_mechanical_cn.py', netParamsDefault='netParams_mechanical_cn.py')

    # Create, Simulate, and Analyze the Network:
    sim.createSimulateAnalyze(netParams = netParams, simConfig = simConfig)

'''

from collections import defaultdict
import numpy as np
from netpyne import sim
from spkt_gen_cn import generate_new_SPKT
import importlib


nRuns = 20
SIM_DURATION = 5000  # ms
SIM_DURATION_sec = SIM_DURATION / 1000.0
INPUT_RATE = 10

# Store firing rate for each celjl in each run
firing_rates = defaultdict(list)

for run in range(nRuns):
    print(f"Run {run+1}/{nRuns}")

    cfgmod = importlib.import_module('cfg_mechanical_cn')
    cfg = cfgmod.cfg

    cfg.inputRate = float(INPUT_RATE)
    cfg.spktSeed = run * 230
    cfg.duration = SIM_DURATION
    cfg.simLabel = f"Circuit_Trial_run{run}"

    npmod = importlib.import_module('netParams_mechanical_cn')
    importlib.reload(npmod)

    #sim.createSimulateAnalyze(netParams=npmod.netParams, simConfig=cfg)

    ### BELOW REPLACES ABOVE LINE TO APPLY SYNAPTIC LIMIT ON PV CELLS

    from neuron import h

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
    sim.analyze()

    all_gids = [c.gid for c in sim.net.cells]
    pop_of = {c.gid: c.tags['pop'] for c in sim.net.cells}

    spike_counts = {gid: 0 for gid in all_gids}
    for gid in sim.allSimData['spkid']:
        spike_counts[int(gid)] += 1

    for gid in all_gids:
        firing_rates[gid].append(spike_counts[gid] / SIM_DURATION_sec)

    sim.clearAll()

# Average firing rate across runs
avg_firing_rate = {
    gid: np.mean(rates)
    for gid, rates in firing_rates.items()
}

print("Average firing rates (Hz):")
for gid in sorted(avg_firing_rate):
    print(f"Cell {gid} ({pop_of[gid]}): {avg_firing_rate[gid]:.2f} Hz")

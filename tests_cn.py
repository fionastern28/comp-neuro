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
from syn_caps import apply_syn_caps



nRuns = 1
SIM_DURATION = 1000  # ms
SIM_DURATION_sec = SIM_DURATION / 1000.0
INPUT_RATE = 10

# Store firing rate for each celjl in each run
firing_rates = defaultdict(list)

for run in range(nRuns):
    print(f"Run {run+1}/{nRuns}")

    cfgmod = importlib.import_module('cfg_mechanical_cn')
    cfg = cfgmod.cfg

    cfg.inputRate = float(INPUT_RATE)
    cfg.spktSeed = 1703 + 10 * run
    cfg.duration = SIM_DURATION
    cfg.simLabel = f"run{run}"

    npmod = importlib.import_module('netParams_mechanical_cn')
    importlib.reload(npmod)

    #sim.createSimulateAnalyze(netParams=npmod.netParams, simConfig=cfg)

    ### BELOW REPLACES ABOVE LINE TO APPLY SYNAPTIC LIMIT ON PV CELLS

    from neuron import h

    sim.create(netParams=npmod.netParams, simConfig=cfg)
    apply_syn_caps(sim, cfg)

    pv = next(c for c in sim.net.cells if c.tags.get('pop') == 'PV')
    amp_vecs  = [h.Vector().record(sm['hObj']._ref_i) for sm in pv.secs['soma']['synMechs'] if sm['label'] == 'AMPA']
    nmda_vecs = [h.Vector().record(sm['hObj']._ref_i) for sm in pv.secs['soma']['synMechs'] if sm['label'] == 'NMDA']

    sim.simulate()

    i_ampa = np.sum([np.array(v) for v in amp_vecs], axis=0)
    i_nmda = np.sum([np.array(v) for v in nmda_vecs], axis=0)
    print("mean AMPA / NMDA current (nA):", i_ampa.mean(), i_nmda.mean())


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

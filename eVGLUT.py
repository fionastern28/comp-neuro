from neuron import h, load_mechanisms
from netpyne import specs, sim
import numpy as np
import matplotlib.pyplot as plt

class VGLUT3_Modeling:

    def vglut_create_graph(rule, window = [0,1000], amp = 0.1, dur = 2000, sec_input = 'soma', 
                           title = 'title', show = True, gating = False, calcium = False):
        netParams = specs.NetParams() # for easy of typing
        rule['conds'] = {'cellType': 'EXdl'}
        netParams.cellParams['VGLUT3Rule'] = rule #sets the parameters to above dictionary
        netParams.popParams['VGLUT3'] = {'cellType': 'EXdl', 'numCells': 1} # VGLUT3+ neurons (excitatory) -> creates one ceel of VLUT3

        # settings for the input
        netParams.stimSourceParams['input'] = {
            'type': 'IClamp',
            'del': 0,     # ms: rest first, so you can see baseline
            'dur': dur,     # ms: then hold the current on for the rest of the run
            'amp': amp}     # nA: the constant input level  <-- the knob to change

        netParams.stimTargetParams['input->VGLUT3'] = {
            'source': 'input', 'conds': {'pop': 'VGLUT3'},
            'sec': sec_input, 'loc': 0.5}

        # ---- run settings ----
        cfg = specs.SimConfig() #creates a configuration object used to define and store simulation options
        cfg.hParams    = {'celsius': 36, 'v_init': -60}
        cfg.duration   = 2000      # ms
        cfg.dt         = 0.025
        cfg.recordStep = 0.1
        cfg.includeParamsLabel = True
        cfg.saveFolder = 'data'     # where output goes
        cfg.saveJson   = True       # save results (also clears the "won't be saved" warning)
        cfg.recordTraces['V_soma'] = {'sec': 'soma', 'loc': 0.5, 'var': 'v'}

        if gating:
            cfg.recordTraces['HH2_m'] = {'sec':'soma','loc':0.5,'mech':'HH2','var':'m'}
            cfg.recordTraces['HH2_h'] = {'sec':'soma','loc':0.5,'mech':'HH2','var':'h'}
            cfg.recordTraces['HH2_n'] = {'sec':'soma','loc':0.5,'mech':'HH2','var':'n'}
            cfg.recordTraces['BNa_m'] = {'sec':'soma','loc':0.5,'mech':'B_Na','var':'m'}
            cfg.recordTraces['BNa_h'] = {'sec':'soma','loc':0.5,'mech':'B_Na','var':'h'}

        if calcium:
            cfg.recordTraces['cai_soma'] = {'sec': 'soma', 'loc': 0.5, 'var': 'cai'}

        print_title(title)

        # note measures loc. 0.5 on each, but unclear what actual model does should check
        cfg.analysis['plotTraces'] = {
            'include': [0],
            'timeRange': window,     # zoom the x-axis to 100-140 ms
            'saveFig': True,
            'showFig': show}

        # Suppress output from sim.createSimulateAnalyze
        sim.createSimulateAnalyze(netParams=netParams, simConfig=cfg)

        if gating:
            hh2_m = np.array(sim.simData['HH2_m']['cell_0'])
            hh2_h = np.array(sim.simData['HH2_h']['cell_0'])
            hh2_n = np.array(sim.simData['HH2_n']['cell_0'])
            bna_m = np.array(sim.simData['BNa_m']['cell_0'])
            bna_h = np.array(sim.simData['BNa_h']['cell_0'])

            plt.figure(figsize=(8, 4))
            plt.plot(sim.simData['t'], hh2_m, label='HH2 m')
            plt.plot(sim.simData['t'], hh2_h, label='HH2 h')
            plt.plot(sim.simData['t'], hh2_n, label='HH2 n')
            plt.plot(sim.simData['t'], bna_m, label='B_Na m')
            plt.plot(sim.simData['t'], bna_h, label='B_Na h')
            plt.axis([68, 76, 0, 1.5])
            plt.legend()
            
        if calcium:
            cai = np.array(sim.simData['cai_soma']['cell_0'])
            plt.figure(figsize=(8, 4))
            plt.plot(sim.simData['t'], cai, label='cai')

        t = np.array(sim.simData['t'])
        v = np.array(sim.simData['V_soma']['cell_0'])

        return t, v

    def print_title(title):
        print()
        print()
        print("*" * (len(title)+34))
        print("*" * (len(title)+34))
        print('The following graph depicts the', title)
        print("*" * (len(title)+34))
        print("*" * (len(title)+34))
        print()
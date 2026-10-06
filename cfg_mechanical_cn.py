'''
Contains duration, dt, recording settings, tunable parameters
such as synaptic weights
'''

from netpyne import specs

cfg = specs.SimConfig()

cfg.hParams = {'celsius': 36, 'v_init': -60}
cfg.vrest = cfg.hParams['v_init']
cfg.duration = 12000

cfg.recordStep = 0.025#25#0.025
cfg.dt = 0.025

# ---------------------------------------------------------------------------
# PRIMARY AFFERENT (Ab) POISSON DRIVE
# These are swept by fi_curve_cn.py to build the F-I curve.
# ---------------------------------------------------------------------------
cfg.inputRate = 10.0    # Hz, mean rate of EACH Ab afferent
cfg.numAb     = 4      # number of independent Ab afferents
cfg.spktSeed  = 1703    # base RNG seed (cell i uses spktSeed + i); None = random
cfg.abInputMode = 'physiological'  # 'physiological' = SAI/SAII rate curves (default, normal runs);
                                    # 'poisson' = homogeneous rate-controlled drive (set by poisson_sweep.py)
 

EX_SCALE = 0.23 #0.23       #  Ab -> Ex synaptic weights
IN_SCALE = 0.35        #  Ab -> IN synaptic weights
PV_PKC_SCALE = 0.018 #0.024  #  PV -> PKC synaptic weights

SYN_CAP_EX = 0.29       # nA, max allowed inward (excitatory) AMPA+NMDA current on PV cells
SYN_CAP_IN = 0.037      # nA, max allowed inward (excitatory) AMPA+NMDA current on PV cells

# SYNAPTIC WEIGHT

## new c -> PKC connection weights
cfg.C_PKC_AMPA = 0.00067115 * 3 # 0.00067115 * 3 = 0.00201345
cfg.C_PKC_NMDA = 0.000407345 * 3 # 0.000407345 * 3 = 0.001222035

cfg.Ab_EX_AMPA = 0.0221559 * EX_SCALE        #0.005871 # = 0.0221559 *.265
cfg.Ab_EX_NMDA = 0.015 * EX_SCALE            #0.003975 # = 0.015 * .265
cfg.Ab_IN_AMPA = 0.00208312 * IN_SCALE       # 0.00208312 * .5 = 0.00104156
cfg.Ab_IN_NMDA = 0.0098189  * IN_SCALE       # 0.0098189  * .5 = 0.00490945

cfg.PV_GABA = 0.29416 * PV_PKC_SCALE # 0.29416 * 0.024 = 0.00705984
cfg.PV_GLY =  0.011521 * PV_PKC_SCALE  # 0.011521 * 0.024 = 0.000276504  

cfg.recordTraces['v_soma'] = {'sec': 'soma', 'loc': 0.5, 'var': 'v'}

# IMPOSES LIMIT ON SYNAPTIC CURRENT
cfg.excCapEX = SYN_CAP_EX  # nA, max allowed inward (excitatory) AMPA+NMDA current on PV cells
cfg.excCapIN = SYN_CAP_IN  # nA, max allowed inward (excitatory) AMPA+NMDA current on PV cells


#cfg.recordTraces['exc_itotal'] = {'sec':'soma', 'loc':0.5, 'pointp':'ExcCap', 'var':'itotal'}
#cfg.recordTraces['exc_icorrect'] = {'sec':'soma', 'loc':0.5, 'pointp':'ExcCap', 'var':'icorrect'}
#cfg.recordTraces['i_NMDA'] = {'sec':'soma', 'loc':0.5, 'synMech':'NMDA', 'var':'i'}



cfg.simLabel = f"Circuit_Trial"
cfg.saveFolder = 'data'
cfg.savePickle = False
cfg.saveJson = True
cfg.saveDataInclude = ['simData', 'simConfig', 'netParams', 'net']


# Analysis and Plotting 
cells = [x for x in range(0, 11, 1)]
cfg.analysis['plotRaster'] = {'include': ['all'], 'timeRange': [0, cfg.duration],'orderInverse': True, 'saveFig': True, 'showFig': True} 
#cfg.analysis['plotConn'] = {'includePre': ['all'], 'includePost': ['all'], 'feature': 'weight','logPlot': True, 'saveFig': True, 'showFig': True}
# cfg.analysis['plotSpikeHist'] = {'include': ['eachPop'], 'timeRange': [0,cfg.duration], 'spikeHistBin': 5, 'saveFig': True, 'showFig': False}
#cfg.analysis['plotSpikeStats'] = {'include': ['eachPop'], 'timeRange': [0,cfg.duration], 'saveFig': True, 'showFig': False}
cfg.analysis['plotTraces'] = {'include': [11], 'timeRange': [0, cfg.duration], 'saveFig': True, 'showFig': False}
#cfg.analysis['plot2Dnet'] = True 
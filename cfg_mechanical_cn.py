'''
Contains duration, dt, recording settings, tunable parameters
such as synaptic weights
'''

from netpyne import specs

cfg = specs.SimConfig()

cfg.hParams = {'celsius': 36, 'v_init': -60}
cfg.vrest = cfg.hParams['v_init']
cfg.duration = 5000

cfg.recordStep = 0.025

# SYNAPTIC WEIGHT
cfg.Ab_EX_AMPA = 0.0221559 *.31 #tbd
cfg.Ab_EX_NMDA = 0.015 *.31 #tbd
cfg.Ab_IN_AMPA = 0
cfg.Ab_IN_NMDA = 0

cfg.recordTraces['vs'] = {'sec':'soma', 'loc':0.5,'var':'v'}

cfg.simLabel = 'Test Trial 1'
cfg.saveFolder = 'data'
cfg.savePickle = False
cfg.saveJson = True
cfg.saveDataInclude = ['simData', 'simConfig', 'netParams']


# Analysis and Plotting -> will have to adjust stats and things but might be useful!


cells = [x for x in range(0, 4, 1)]
cfg.analysis['plotRaster'] = {'include': ['all'], 'timeRange': [0, cfg.duration],'orderInverse': True, 'saveFig': True, 'showFig': True} 
cfg.analysis['plotConn'] = {'includePre': ['all'], 'includePost': ['all'], 'feature': 'weight','logPlot': True, 'saveFig': True, 'showFig': True}
# cfg.analysis['plotSpikeHist'] = {'include': ['eachPop'], 'timeRange': [0,cfg.duration], 'spikeHistBin': 5, 'saveFig': True, 'showFig': False}
cfg.analysis['plotSpikeStats'] = {'include': ['eachPop'], 'timeRange': [0,cfg.duration], 'saveFig': True, 'showFig': False}
cfg.analysis['plotTraces'] = {'include': [4], 'timeRange': [0, cfg.duration], 'saveFig': True, 'showFig': False}
# cfg.analysis['plot2Dnet'] = False 
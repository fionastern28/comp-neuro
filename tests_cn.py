'''
Script for testing various parts of the model
come back to the meanings of what i have here...
eventually this will likely be the init
'''

from netpyne import sim
from neuron import h
					
simConfig, netParams = sim.readCmdLineArgs(simConfigDefault='cfg_mechanical_cn.py', netParamsDefault='netParams_mechanical_cn.py')

# Create, Simulate, and Analyze the Network:
sim.createSimulateAnalyze(netParams = netParams, simConfig = simConfig)

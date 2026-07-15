'''
Network Parameters script
'''

from netpyne import specs
from neuron import h
import cells_cn
from spkt_gen_cn import poisson_generator
import json
import sys
# not sure wht this does
from cfg_mechanical_cn import cfg

netParams = specs.NetParams()
# spkt_10Hz_4cells
 
# Primary Afferent info
with open('spkt/spkt_10Hz_4cells.json', 'rb') as spkt_Ab: spkt_Ab = json.load(spkt_Ab)
netParams.popParams['Ab'] = {'cellModel': 'VecStim', 'numCells': 4, 'spkTimes': spkt_Ab}  # input from Ab_slow adapting type I

# moves dictionary from independent script into netParams
netParams.cellParams['EXdelayedRule'] = cells_cn.EXdelayedRule
netParams.cellParams['INcellRule'] = cells_cn.INcellRule

netParams.popParams['VGLUT3'] = {'cellType': 'EXdl', 'numCells': 1} # VGLUT3+ neurons (excitatory)
netParams.popParams['PKC'] = {'cellType': 'EXdl', 'numCells': 1}
netParams.popParams['PV'] = {'cellType': 'IN', 'numCells': 1} # PV+ neurons (inhibitory)

netParams.synMechParams['AMPA'] = {'mod': 'AMPA_DynSyn'   , 'tau_rise': 0.1, 'tau_decay': 5            }
netParams.synMechParams['NMDA'] = {'mod': 'NMDA_DynSyn'   , 'tau_rise': 2  , 'tau_decay': 100          }
netParams.synMechParams['GABA'] = {'mod': 'GABAa_DynSyn'  , 'tau_rise': 0.1, 'tau_decay': 20, 'e': -70 }
netParams.synMechParams['GLY']  = {'mod': 'Glycine_DynSyn', 'tau_rise': 0.1, 'tau_decay': 10, 'e': -70 }

netParams.connParams['Ab_AMPA->VGLUT'] = {
    'oneSynPerNetcon': True,
    'preConds': {'popLabel': 'Ab'}, 
    'postConds': {'popLabel': 'VGLUT3'},  
    'weight': cfg.Ab_EX_AMPA,           
    'sec': 'soma',
    'probability': 1,
    'delay': 1.0,
    'loc': 0.5,
    'synMech': 'AMPA'} 

netParams.connParams['Ab_NMDA->VGLUT3'] = {
    'oneSynPerNetcon': True,
    'preConds': {'popLabel': 'Ab'}, 
    'postConds': {'popLabel': 'VGLUT3'},  
    'weight': cfg.Ab_EX_NMDA,           
    'sec': 'soma',
    'probability': 1,
    'delay': 1.0,
    'loc': 0.5,
    'synMech': 'NMDA'} 
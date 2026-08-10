'''
Network Parameters script
'''

from netpyne import specs
from neuron import h
import cells_cn
from spkt_gen_cn import poisson_generator, rate_SAI, rate_SAII, inh_poisson_generator
import json
import sys
# not sure wht this does
from cfg_mechanical_cn import cfg

netParams = specs.NetParams()


PROB = 1 # probability of connection between ab fibers and other
PROB_AB = 1
MEDLOCK_INPUT = False
NUM_AB = 20 # number of ab fibers to use in the model
RATE_INCREASE = False # if true, use the 1.5x rate increase for medlock input

# for pain inhibition
NUM_C_FIBERS = 10
NUM_AB_PULSE = 4
AB_FIBER_RATE = 30
C_FIBER_RATE = 10
A_START = 2000
A_END = 2030

 
# ---------------------------------------------------------------------------
# Primary afferent (Ab) input -- inhomogeneous Poisson (thinning) drive using
# the SAI/SAII cubic rate time-course from spkt_gen_cn.py, split evenly across
# cfg.numAb fibers. Each afferent gets an independent realisation
# (seed = spktSeed + cell index).
# ---------------------------------------------------------------------------
_rate  = getattr(cfg, 'inputRate', 10.0)
_nAb   = getattr(cfg, 'numAb', NUM_AB)
_seed  = getattr(cfg, 'spktSeed', 1000)
_ab_mode = getattr(cfg, 'abInputMode', 'physiological')

# Primary Afferent info - RETURN TO THIS WHEN DONE WITH FRH FIGURE
#with open('spkt/spkt.json', 'rb') as spkt_Ab: spkt_Ab = json.load(spkt_Ab)

if MEDLOCK_INPUT:
    if _nAb != 20:
        print("Warning: Medlock input is hardcoded for 20 Ab fibers. Adjusting _nAb to 20.")
        _nAb = 20
    if RATE_INCREASE:
        with open('spkt/spkt_SAI_100mN_Ab10x.json', 'rb') as f: spkt_SAI  = json.load(f)
        with open('spkt/spkt_SAII_100mN_Ab10x.json', 'rb') as f: spkt_SAII = json.load(f)
    else:
        with open('spkt/spkt_SAI_100mN.json', 'rb') as f: spkt_SAI  = json.load(f)
        with open('spkt/spkt_SAII_100mN.json', 'rb') as f: spkt_SAII = json.load(f)
    _nSAI, _nSAII = 10, 10
    spkt_Ab = spkt_SAI + spkt_SAII  # 10 + 10 = 20 fibres
elif _ab_mode == 'poisson':
    # Homogeneous Poisson drive, rate-controlled by cfg.inputRate -- used by
    # poisson_sweep.py (and tests_cn.py) to build F-I curves. Each fiber gets
    # an independent realisation (seed = spktSeed + fiber index).
    def make_poisson_pop(rate, n_cells, t_stop, seed_base):
        trains = []
        for i in range(n_cells):
            seed = None if seed_base is None else seed_base + i
            trains.append(poisson_generator(rate=rate, t_start=0, t_stop=t_stop,
                                             seed=seed).tolist())
        return trains

    spkt_Ab = make_poisson_pop(_rate, _nAb, cfg.duration, _seed)
else:
    rate_sai_vals,  t_sai  = rate_SAI()
    rate_saii_vals, t_saii = rate_SAII()

    _nSAI  = _nAb // 2
    _nSAII = _nAb - _nSAI

    def make_inhomog_pop(rate_vals, t_vals, n_cells, t_stop, seed_base):
        trains = []
        for i in range(n_cells):
            seed = None if seed_base is None else seed_base + i
            trains.append(inh_poisson_generator(rate=rate_vals, t=t_vals,
                                                 t_stop=t_stop, seed=seed))
        return trains

    spkt_SAI  = make_inhomog_pop(rate_sai_vals,  t_sai,  _nSAI,  cfg.duration, _seed)
    _seed_saii = None if _seed is None else _seed + _nSAI
    spkt_SAII = make_inhomog_pop(rate_saii_vals, t_saii, _nSAII, cfg.duration, _seed_saii)

    spkt_Ab = spkt_SAI + spkt_SAII  # combine SAI and SAII draws into one Ab population

# moves dictionary from independent script into netParams
netParams.cellParams['EXdelayedRule'] = cells_cn.EXdelayedRule
netParams.cellParams['EXdelayedRule']['secs']['soma'].setdefault('pointps', {})
netParams.cellParams['EXdelayedRule']['secs']['soma']['pointps']['ExcCap'] = {
    'mod': 'ExcCap',
    'cap': cfg.excCapEX
}
netParams.cellParams['INcellRule'] = cells_cn.INcellRule
netParams.cellParams['INcellRule']['secs']['soma'].setdefault('pointps', {})
netParams.cellParams['INcellRule']['secs']['soma']['pointps']['ExcCap'] = {
    'mod': 'ExcCap',
    'cap': cfg.excCapIN
}

spkt_C = []
for i in range(NUM_C_FIBERS):
   spkt_C.append(poisson_generator(C_FIBER_RATE, t_start = 0, t_stop=cfg.duration, seed=None).tolist())  # 10 Hz, 1000 ms, seed = i


spkt_Ab_pulse = []
for i in range(NUM_AB_PULSE):
    spkt_Ab_pulse.append(poisson_generator(rate=AB_FIBER_RATE, t_start=A_START, t_stop=A_END, seed=None).tolist())
netParams.popParams['Ab'] = {'cellModel': 'VecStim', 'numCells': _nAb, 'spkTimes': spkt_Ab_pulse}

#netParams.popParams['Ab'] = {'cellModel': 'VecStim', 'numCells': _nAb, 'spkTimes': spkt_Ab}
netParams.popParams['C_PEP'] = {'cellModel': 'VecStim', 'numCells': NUM_C_FIBERS, 'spkTimes': spkt_C}
netParams.popParams['PV'] = {'cellType': 'IN', 'numCells': 1} # PV+ neurons (inhibitory)
netParams.popParams['PKC'] = {'cellType': 'EXdl', 'numCells': 1}


netParams.defaultThreshold = -30

netParams.synMechParams['AMPA'] = {'mod': 'AMPA_DynSyn'   , 'tau_rise': 0.1, 'tau_decay': 5            }
netParams.synMechParams['NMDA'] = {'mod': 'NMDA_DynSyn'   , 'tau_rise': 2  , 'tau_decay': 100          }
netParams.synMechParams['GABA'] = {'mod': 'GABAa_DynSyn'  , 'tau_rise': 0.1, 'tau_decay': 20, 'e': -70 }
netParams.synMechParams['GLY']  = {'mod': 'Glycine_DynSyn', 'tau_rise': 0.1, 'tau_decay': 10, 'e': -70 }


netParams.connParams['Ab_AMPA->PV'] = {
    'oneSynPerNetcon': True,
    'preConds': {'popLabel': 'Ab'},
    'postConds': {'popLabel': 'PV'},
    'weight': cfg.Ab_IN_AMPA,
    'sec': 'soma',
    'probability': PROB_AB,
    'delay': 1.0,
    'loc': 0.5,
    'synMech': 'AMPA'}

netParams.connParams['Ab_NMDA->PV'] = {
    'oneSynPerNetcon': True,
    'preConds': {'popLabel': 'Ab'},
    'postConds': {'popLabel': 'PV'},
    'weight': cfg.Ab_IN_NMDA,
    'sec': 'soma',
    'probability': PROB_AB,
    'delay': 1.0,
    'loc': 0.5,
    'synMech': 'NMDA'}

netParams.connParams['Ab_AMPA->PKC'] = {
    'oneSynPerNetcon': True,
    'preConds': {'popLabel': 'Ab'},
    'postConds': {'popLabel': 'PKC'},
    'weight': cfg.Ab_EX_AMPA,
    'sec': 'soma',
    'probability': PROB_AB,
    'delay': 1.0,
    'loc': 0.5,
    'synMech': 'AMPA'}

netParams.connParams['Ab_NMDA->PKC'] = {
    'oneSynPerNetcon': True,
    'preConds': {'popLabel': 'Ab'},
    'postConds': {'popLabel': 'PKC'},
    'weight': cfg.Ab_EX_NMDA,
    'sec': 'soma',
    'probability': PROB_AB,
    'delay': 1.0,
    'loc': 0.5,
    'synMech': 'NMDA'}

### new connections: PV -> PKC ###

netParams.connParams['PV_GABA->PKC'] = {
    'oneSynPerNetcon': True,
    'preConds': {'popLabel': 'PV'}, 
    'postConds': {'popLabel':'PKC'},  
    'weight': cfg.PV_GABA,          
    'probability': PROB,
    'sec': 'soma',
    'delay': 0.5, 
    'loc': 0.5,
    'synMech': 'GABA'}

netParams.connParams['PV_GLY->PKC'] = {
    'oneSynPerNetcon': True,
    'preConds': {'popLabel': 'PV'}, 
    'postConds': {'popLabel':'PKC'},  
    'weight': cfg.PV_GLY,        
    'probability': PROB,
    'sec': 'soma',
    'delay': 0.5, 
    'loc': 0.5,
    'synMech': 'GLY'}

#### NEW FOR C FIBERS ###
# From Peptidergic C (C,TRPV1) to Spinal Interneurons
netParams.connParams['C_PEP_AMPA->PKC'] = {
    'oneSynPerNetcon': True,
    'preConds': {'popLabel': 'C_PEP'}, 
    'postConds': {'popLabel': 'PKC'},  
    'weight': cfg.C_PKC_AMPA,
    'probability': PROB,
    'sec': 'soma',
    'delay': 10.0,
    'loc': 0.5,
    'synMech': 'AMPA'}

netParams.connParams['C_PEP_NMDA->PKC'] = {
    'oneSynPerNetcon': True,
    'preConds': {'popLabel': 'C_PEP'}, 
    'postConds': {'popLabel': 'PKC'},  
    'weight': cfg.C_PKC_NMDA ,
    'probability': PROB,
    'sec': 'soma',
    'delay': 10.0,
    'loc': 0.5,
    'synMech': 'NMDA'}
'''
Network Parameters script
'''

from netpyne import specs
from neuron import h
import cells_cn
from spkt_gen_cn import make_volley_pop, poisson_generator, rate_SAI, rate_SAII, inh_poisson_generator
import json
import sys
# not sure wht this does
from cfg_mechanical_cn import cfg

netParams = specs.NetParams()


PROB = 1 # probability of connection between ab fibers and other
PROB_AB = 1
MEDLOCK_INPUT = False
NUM_AB = 4 # number of ab fibers to use in the model
RATE_INCREASE = False # if true, use the 1.5x rate increase for medlock input

# for pain inhibition
NUM_C_FIBERS = 16
NUM_AB_PULSE = 4
AB_FIBER_RATE = 30
C_FIBER_RATE = 10
A_START = 1000
A_END = 1050

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

def make_poisson_pop(rate, n_cells, t_start, t_stop, seed_base):
        trains = []
        for i in range(n_cells):
            seed = None if seed_base is None else seed_base + i
            trains.append(poisson_generator(rate=rate, t_start=t_start, t_stop=t_stop,
                                             seed=seed).tolist())
        return trains

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
    spkt_Ab = make_poisson_pop(_rate, _nAb, 0, cfg.duration, _seed)
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

# below used for testing different values of Ab input to make synapse strength

# moves dictionary from independent script into netParams
netParams.cellParams['EXdelayedRule'] = cells_cn.EXdelayedRule
netParams.cellParams['INcellRule'] = cells_cn.INcellRule

cells_cn.PROcellRule['conds'] = {'cellType': 'PRO'}
netParams.cellParams['NK1Rule'] = cells_cn.PROcellRule

spkt_C = make_poisson_pop(rate=7, n_cells=NUM_C_FIBERS, t_start=0, t_stop=cfg.duration, seed_base= None)


spkt_C_pulse = []
spkt_C_pulse = make_volley_pop(n_cells=NUM_C_FIBERS, freq_hz=C_FIBER_RATE, n_stim=5, jitter_ms=1.0, seed=None)

spkt_Ab_pulse_PV = []
spkt_Ab_pulse_PKC = []
for i in range(NUM_AB_PULSE):
    spkt_Ab_pulse_PV.append(poisson_generator(rate=AB_FIBER_RATE, t_start=A_START, t_stop=A_END, seed=None).tolist())
    spkt_Ab_pulse_PKC.append(poisson_generator(rate=AB_FIBER_RATE, t_start=A_START, t_stop=A_END, seed=None).tolist())

spkt_Ab_30 = make_poisson_pop(rate=AB_FIBER_RATE, n_cells=_nAb, t_start=1000, t_stop=1050, seed_base=1)

#netParams.popParams['Ab_PV'] = {'cellModel': 'VecStim', 'numCells': _nAb, 'spkTimes': spkt_Ab_pulse_PV}
#netParams.popParams['Ab'] = {'cellModel': 'VecStim', 'numCells': _nAb, 'spkTimes': spkt_Ab_pulse_PKC}


spkt_Ab = make_poisson_pop(rate=500, n_cells=10, t_start=0, t_stop=cfg.duration, seed_base=_seed*30)
spkt_Ab_PV = make_poisson_pop(rate=500, n_cells=10, t_start=0, t_stop=cfg.duration, seed_base=_seed*31)


netParams.popParams['Ab'] = {'cellModel': 'VecStim', 'numCells': 4, 'spkTimes': spkt_Ab}
netParams.popParams['Ab_PV'] = {'cellModel': 'VecStim', 'numCells':4, 'spkTimes': spkt_Ab_PV}
netParams.popParams['C_PEP'] = {'cellModel': 'VecStim', 'numCells': NUM_C_FIBERS, 'spkTimes': spkt_C}
#netParams.popParams['C_PEP'] = {'cellModel': 'VecStim', 'numCells': NUM_C_FIBERS, 'spkTimes': spkt_C_pulse}
#netParams.popParams['PV'] = {'cellType': 'IN', 'numCells': 1} # PV+ neurons (inhibitory)
#netParams.popParams['PKC'] = {'cellType': 'EXdl', 'numCells': 1}
#netParams.popParams['NK1'] = {'cellType': 'PRO', 'numCells': 1} # NK1+ neurons (projection)
netParams.popParams['DYN'] = {'cellType': 'IN', 'numCells': 1} # Central/DYN+ neurons (inhibitory)



netParams.defaultThreshold = -30

netParams.synMechParams['AMPA'] = {'mod': 'AMPA_DynSyn'   , 'tau_rise': 0.1, 'tau_decay': 5            }
#netParams.synMechParams['AMPA_IN'] = {'mod': 'AMPA_DynSyn_IN'   , 'tau_rise': 0.1, 'tau_decay': 5, 'gmax': cfg.gmaxAMPA_IN}
#netParams.synMechParams['AMPA_EX'] = {'mod': 'AMPA_DynSyn_EX'   , 'tau_rise': 0.1, 'tau_decay': 5,  'gmax': cfg.gmaxAMPA_EX}
netParams.synMechParams['NMDA'] = {'mod': 'NMDA_DynSyn'   , 'tau_rise': 2  , 'tau_decay': 100          }
#netParams.synMechParams['NMDA_IN'] = {'mod': 'NMDA_DynSyn'   , 'tau_rise': 2  , 'tau_decay': 100,  'gmax': cfg.gmaxAMPA_IN}
#netParams.synMechParams['NMDA_EX'] = {'mod': 'NMDA_DynSyn'   , 'tau_rise': 2  , 'tau_decay': 100,  'gmax': cfg.gmaxNMDA_EX}

netParams.synMechParams['GABA'] = {'mod': 'GABAa_DynSyn'  , 'tau_rise': 0.1, 'tau_decay': 20, 'e': -70 }
netParams.synMechParams['GLY']  = {'mod': 'Glycine_DynSyn', 'tau_rise': 0.1, 'tau_decay': 10, 'e': -70 }
netParams.synMechParams['NK13'] = {'mod': 'NK1_DynSyn'    , 'tau_rise': 100, 'tau_decay': 1000         }
netParams.synMechParams['NK23'] = {'mod': 'NK1_DynSyn'    , 'tau_rise': 200, 'tau_decay': 3000         }


netParams.connParams['Ab_AMPA->PV'] = {
    'oneSynPerNetcon': True,
    'preConds': {'popLabel': 'Ab_PV'},
    'postConds': {'popLabel': 'PV'},
    'weight': cfg.Ab_IN_AMPA,
    'sec': 'soma',
    'probability': PROB_AB,
    'delay': 1.0,
    'loc': 0.5,
    'synMech': 'AMPA'}

netParams.connParams['Ab_NMDA->PV'] = {
    'oneSynPerNetcon': True,
    'preConds': {'popLabel': 'Ab_PV'},
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


netParams.connParams['C_PEP_AMPA->NK1'] = {
    'oneSynPerNetcon': True,
    'preConds': {'popLabel': 'C_PEP'}, 
    'postConds': {'popLabel': 'NK1'},  
    'weight': cfg.C_NK1_AMPA,
    'probability': 1,
    'sec': 'soma',
    'delay': 10.0,
    'loc': 0.5,
    'synMech': 'AMPA'}

netParams.connParams['C_PEP_NMDA->NK1'] = {
    'oneSynPerNetcon': True,
    'preConds': {'popLabel': 'C_PEP'}, 
    'postConds': {'popLabel': 'NK1'},  
    'weight': cfg.C_NK1_NMDA,
    'probability': 1,
    'sec': 'soma',
    'delay': 10.0,
    'loc': 0.5,
    'synMech': 'NMDA'}

netParams.connParams['C_PEP_NK1->NK1'] = {
    'oneSynPerNetcon': True,
    'preConds': {'popLabel': 'C_PEP'}, 
    'postConds': {'popLabel': 'NK1'},  
    'weight': cfg.C_NK1_NK1,
    'probability': 1,
    'sec': 'soma',
    'delay': 10.0,
    'loc': 0.5,
    'synMech': 'NK13'}

netParams.connParams['Ab_AMPA->DYN'] = {
    'oneSynPerNetcon': True,
    'preConds': {'popLabel': 'Ab'}, 
    'postConds': {'popLabel': 'DYN'},  
    'weight': cfg.Ab_DYN_AMPA,           
    'sec': 'soma',
    'probability': 1,
    'delay': 1.0,
    'loc': 0.5,
    'synMech': 'AMPA'} 

netParams.connParams['C_PEP_AMPA->DYN'] = {
    'oneSynPerNetcon': True,
    'preConds': {'popLabel': 'C_PEP'}, 
    'postConds': {'popLabel': 'DYN'},  
    'weight': cfg.C_DYN_AMPA,
    'probability': 1,
    'sec': 'soma',
    'delay': 10.0,
    'loc': 0.5,
    'synMech': 'AMPA'}

'''
holds cell templates
'''

from neuron import h


EXdelayedRule  = {
 'conds': {'cellType': 'EXdl'},
 'globals': {},
 'secLists': {},
 'secs': {'soma': {
     'geom': {'L': 20.0, 'nseg': 1, 'diam': 20.0, 'Ra': 150.0, 'cm': 1.0},
     'ions': {'ca': {'e': 132.4579341637009, 'i': 5e-05, 'o': 2.0},
              'k':  {'e': -70.0, 'i': 54.4, 'o': 2.5},
              'na': {'e': 42.0,  'i': 10.0, 'o': 140.0}},
     'mechs': {
         'B_Na':   {'gnabar': 0.0001652, 'alpha_shift': 0.0, 'beta_shift': 0.0},   
         'CaIntraCellDyn': {'cai_inf': 5e-05, 'cai_tau': 1.0, 'depth': 0.1},        # unchanged, inert (no Ca channel)
         'HH2':    {'gkbar': 0.0043, 'gnabar': 0.08548, 'vtraub': -50.2},           #
         'borgka': {'gkabar': 0.013},                                             
         'KDRI':   {'gkbar': 0.0019},                                              
         'iKCa':   {'gbar': 0.0, 'gk': 0.0},                                        
         'pas':    {'g': 0.0001, 'e': -65.0},                                       
     }
 }}
}


INcellRule  = {
 'conds': {'cellType': 'IN'},
 'globals': {},
 'secLists': {},
 'secs': {'soma': {'geom': {'L': 10.0, 'nseg': 1, 'diam': 10.0, 'Ra': 80.0, 'cm': 1.0},
                   'ions': {'k': {'e': -78.0, 'i': 54.4, 'o': 2.5},
                            'na': {'e': 47.0, 'i': 10.0, 'o': 140.0}},
                   'mechs': {'B_A': {},
                             'B_DR': {},
                             'B_Na': {'gnabar': 0.6, 'alpha_shift': 0.0, 'beta_shift': 0.0},  #*.3
                             'KDR': {'gkbar': 0.0},
                             'KDRI': {'gkbar': .1}, #.15
                             'pas': {'g': 0.00065, 'e': -70.0}}, # 0.0006
                    }
                   }}

PROcellRuleNEW = {
 'conds': {},
 'globals': {},
 'secLists': {},
 'secs': {'soma': {'geom': {'L': 20.0, 'nseg': 1, 'diam': 20.0, 'Ra': 150.0, 'cm': 1.0},
                   'ions': {'ca': {'e': 132.4579341637009, 'i': 5e-05, 'o': 2.0},
                            'can': {'e': 0.0, 'i': 1.0, 'o': 1.0},
                            'k': {'e': -70.0, 'i': 54.4, 'o': 2.5},
                            'na': {'e': 75.0, 'i': 10.0, 'o': 140.0}}, # changed from 50
                   'mechs': {'CaIntraCellDyn': {'cai_inf': 5e-05,
                                                'cai_tau': 1.5,#1.66374
                                                'depth': 0.1},
                             'iCaL': {'pcabar': 3.0e-05}, # need this to feed calcium into the cell, without this there is no source and the other calcium related channels are useless
                             'HH2': {'gkbar': 0.0037,
                                     'gnabar': 0.015,
                                     'vtraub': -55.0},
                            'iKCa': {'gbar': 0.0008, 'gk': 0.0},
                             'pas': {'g':  1e-04, 'e': -65.0}},
                   'topol': {}}}
}
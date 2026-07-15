'''
holds cell templates
'''

from neuron import h

EXdelayedRule  = {
 'conds': {'cellType': 'EXdl'},
 'globals': {},
 'secLists': {},
 'secs': {'soma': {'geom': {'L': 20.0, 'nseg': 1, 'diam': 20.0, 'Ra': 150.0, 'cm': 1.0},
                   'ions': {'ca': {'e': 132.4579341637009, 'i': 5e-05, 'o': 2.0},
                            'k': {'e': -70.0, 'i': 54.4, 'o': 2.5},
                            'na': {'e': 50.0, 'i': 10.0, 'o': 140.0}},
                   'mechs': {'B_Na': {'gnabar': 0.0001652, 'alpha_shift': 0.0, 'beta_shift': 0.0},
                             'CaIntraCellDyn': {'cai_inf': 5e-05, 'cai_tau': 1.0, 'depth': 0.1},
                             'HH2': {'gkbar': 0.0043, 'gnabar': 0.08548, 'vtraub': -50.2},
                             'borgka': {'gkabar': 0.03 * 1.0 },
                             'KDRI': {'gkbar': 0.00045},
                             'iKCa': {'gbar': 0.000, 'gk': 0.0},
                             'pas': {'g': 0.0001, 'e': -65.0}}
                   }}
}

INcellRule  = {
 'conds': {'cellType': 'IN'},
 'globals': {},
 'secLists': {},
 'secs': {'soma': {'geom': {'L': 10.0, 'nseg': 1, 'diam': 10.0, 'Ra': 80.0, 'cm': 1.0},
                   'ions': {'k': {'e': -84.0, 'i': 54.4, 'o': 2.5},
                            'na': {'e': 60.0, 'i': 10.0, 'o': 140.0}},
                   'mechs': {'B_A': {},
                             'B_DR': {},
                             'B_Na': {'gnabar': 1.1 * 1, 'alpha_shift': 0.0, 'beta_shift': 0.0},  #*.3
                             'KDR': {'gkbar': 0.0},
                             'KDRI': {'gkbar': 0.008 * 5.05}, #*20
                             'pas': {'g': 0.000638, 'e': -70.0}}
                   }}
}

def apply_syn_caps(sim, cfg):
    '''
    Per-cell conductance budget for each population listed in cfg.synCaps,
    split across that cell's synapses of each receptor type (all sources).
    Call after sim.create(), before sim.simulate().
    '''
    caps  = getattr(cfg, 'synCaps', {})
    split = getattr(cfg, 'synCapSplit', 'equal')
    record = {}

    for cell in sim.net.cells:
        pop = cell.tags.get('pop')
        if pop not in caps:
            continue
        budgets = caps[pop]

        # group this cell's synapses by mod type, from every presynaptic source
        groups = {mod: [] for mod in budgets}
        for conn in cell.conns:
            syn = conn['hObj'].syn()
            mod = syn.hname().split('[')[0]
            if mod in groups:
                groups[mod].append((syn, conn['weight']))

        record[cell.gid] = {'pop': pop}
        for mod, syns in groups.items():
            N = len(syns)
            if N == 0:
                continue
            if not hasattr(syns[0][0], 'gmax'):
                raise AttributeError(f"{mod} has no 'gmax' -- add it to {mod}.mod and recompile")
            wsum = sum(w for _, w in syns)
            for syn, w in syns:
                share = 1.0 / N if split == 'equal' else w / wsum
                syn.gmax = budgets[mod] * share
            record[cell.gid][mod] = {'N': N, 'gmax': [s.gmax for s, _ in syns]}

    sim.simData['synCaps'] = record
    return record
import numpy as np
import json
import os


def poisson_generator(rate, t_start=0.0, t_stop=1000.0, seed=None):
    rng = np.random.RandomState(seed)
    n = (t_stop-t_start) / 1000.0 * rate
    number = np.ceil(n + 3*np.sqrt(n))
    if number < 100:
        number = min(5 + np.ceil(2*n), 100)
    if number > 0:
        isi = rng.exponential(1.0/rate, int(number)) * 1000.0
        if number > 1:
            spikes = np.add.accumulate(isi)
        else:
            spikes = isi
    else:
        spikes = np.array([])
    spikes += t_start
    i = np.searchsorted(spikes, t_stop)
    extra_spikes = []
    if i == len(spikes):
        # ISI buf overrun
        
        t_last = spikes[-1] + rng.exponential(1.0/rate, 1)[0] * 1000.0
        while (t_last < t_stop):
            extra_spikes.append(t_last)
            t_last += rng.exponential(1.0/rate, 1)[0] * 1000.0
        
        spikes = np.concatenate((spikes, extra_spikes))
    else:
        spikes = np.resize(spikes, (i,))
        
    return spikes

if __name__ == '__main__':
    # --- generation parameters ---
    NUM_CELLS = 4
    RATE_HZ   = 10.0
    T_START   = 0.0
    T_STOP    = 5000.0   # ms, matches cfg.duration in cfg_mechanical.py
    SEED_BASE = None        # set to None for non-reproducible runs
 
    # each cell gets its own independent Poisson realization (different seed)
    spkt = []
    for cell_id in range(NUM_CELLS):
        seed = SEED_BASE + cell_id if SEED_BASE is not None else None
        spikes = poisson_generator(rate=RATE_HZ, t_start=T_START, t_stop=T_STOP, seed=seed)
        spkt.append(spikes.tolist())
 
    # save next to this script, in a "spkt" subfolder (matches project convention)
    script_dir = os.path.dirname(os.path.abspath(__file__))
    out_dir = os.path.join(script_dir, 'spkt')
    os.makedirs(out_dir, exist_ok=True)
    out_path = os.path.join(out_dir, 'spkt_10Hz_4cells.json')
 
    with open(out_path, 'w') as f:
        json.dump(spkt, f)
 
    # quick sanity check
    for i, s in enumerate(spkt):
        print(f'cell {i}: {len(s)} spikes, mean rate = {len(s)/(T_STOP/1000.0):.2f} Hz')
 
    print(f'saved to {out_path}')
 
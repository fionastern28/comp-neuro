TITLE Hard refractory period / firing rate cap

COMMENT
POINT_PROCESS that enforces an absolute refractory period after each
somatic spike, capping the maximum possible firing rate.

Mechanism: uses NMODL's WATCH statement to monitor local v directly (no
external NetCon needed - WATCH works on a POINT_PROCESS's own v). When v
crosses vthresh (should match whatever threshold NetPyNE/NEURON uses to
detect this cell's output spikes - NetPyNE's default is 10 mV if not set
explicitly in cellParams/connParams), a strong shunt conductance (gmax,
reversal e) turns on for refrac_dur ms, then turns back off. While the shunt
is on, it counteracts depolarizing drive and prevents the cell from crossing
threshold again.

  refrac_dur = 20 ms  ->  max firing rate = 1000/20 = 50 Hz

NOTE: must be added as a POINT_PROCESS (NetPyNE 'pointps' key), not a
density mechanism ('mechs' key) - this NEURON version requires NET_RECEIVE
blocks to live in a POINT_PROCESS.

Caveats:
- This is a phenomenological cap, not a biophysical Na-channel-inactivation
  refractory period. It works by brute-force shunting, so gmax must be large
  enough to override whatever depolarizing drive/conductances your cell has.
  Your soma is small (L=diam=10um), so a few uS is already a large shunt
  relative to total channel conductance there - but verify empirically (see
  validation step) and raise gmax if spikes still sneak through.
- vthresh MUST match the threshold used elsewhere to detect this cell's
  spikes, or the shunt will arm at the wrong voltage.
ENDCOMMENT

NEURON {
	POINT_PROCESS Refrac
	RANGE vthresh, refrac_dur, gmax, e, g
	NONSPECIFIC_CURRENT i
}

PARAMETER {
	vthresh    = 10   (mV)   : spike-detection threshold - MATCH your NetPyNE/NetCon threshold
	refrac_dur = 20   (ms)   : minimum inter-spike interval -> 1000/refrac_dur Hz cap
	gmax       = 5.0  (umho) : shunt conductance while "refractory" - increase if spikes still occur under strong drive
	e          = -75  (mV)   : shunt reversal (near/below rest, hyperpolarizing pull)
}

ASSIGNED {
	v (mV)
	i (nA)
	g (umho)
}

INITIAL {
	g = 0
	net_send(0, 3)   : arm the watch at t=0
}

BREAKPOINT {
	i = g*(v - e)
}

NET_RECEIVE(w) {
	if (flag == 3) {
		WATCH (v > vthresh) 1
	} else if (flag == 1) {
		: spike detected -> engage shunt, schedule release
		g = gmax
		net_send(refrac_dur, 2)
	} else if (flag == 2) {
		: refractory period over -> release shunt, re-arm watch
		g = 0
		WATCH (v > vthresh) 1
	}
}

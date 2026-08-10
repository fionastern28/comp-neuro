TITLE AMPA receptor with pre-synaptic short-term plasticity 


COMMENT
AMPA receptor conductance using a dual-exponential profile
Pre-synaptic short-term plasticity based on Fuhrmann et al, 2002

Written by Paulo Aguiar and Mafalda Sousa, IBMC, May 2008
pauloaguiar@fc.up.pt ; mafsousa@ibmc.up.pt
ENDCOMMENT



: ===========================================================================
: EDITED by Claude (Cowork) on 2026-08-07 21:31 EDT (revised 21:52 EDT)
: PURPOSE: Added POINTER iacc so this synapse can push its own current into
: the shared ExcCap accumulator (iacc_ampa) each timestep, replacing the old
: scheme where ExcCap pulled from a fixed 20-slot array of pointers. See
: SynCurrentCap.mod for the accumulator side of this change.
: REVISION NOTE: the push (iacc = iacc + i) originally lived inside
: BREAKPOINT, but testing showed NEURON evaluates BREAKPOINT twice per
: timestep for mechanisms with a NONSPECIFIC_CURRENT and no declared
: analytic conductance (once at v, once at a perturbed v, to build the
: implicit integration Jacobian) -- which silently double-counted every
: push. Moved to AFTER SOLVE, which NEURON guarantees runs exactly once
: per accepted timestep, after v is finalized, using the same 'i' that's
: normally trusted for recording/plotting synaptic current traces.
: ===========================================================================

NEURON {
	POINT_PROCESS AMPA_DynSyn
	RANGE tau_rise, tau_decay
	RANGE U1, tau_rec, tau_fac
	RANGE i, g, e
	POINTER iacc
	NONSPECIFIC_CURRENT i
}

PARAMETER {
	tau_rise  = 0.1   (ms)  : dual-exponential conductance profile
	tau_decay = 5.0   (ms)  : IMPORTANT: tau_rise < tau_decay
	U1        = 1.0   (1)   : The parameter U1, tau_rec and tau_fac define _
	tau_rec   = 0.1   (ms)  : the pre-synaptic short-term plasticity _
	tau_fac   = 0.1   (ms)  : mechanism (see Fuhrmann et al, 2002)
	e         = 0.0   (mV)  : AMPA synapse reversal potential
}
     

ASSIGNED {
	v (mV)
	i (nA)
	g (umho)
	factor
	iacc (nA)  : ExcCap's iacc_ampa accumulator, wired via h.setpointer (added 2026-08-07)
}

STATE {
	A	: state variable to construct the dual-exponential profile
	B	: 
}

INITIAL{
	LOCAL tp
	A = 0
	B = 0
	tp = (tau_rise*tau_decay)/(tau_decay-tau_rise)*log(tau_decay/tau_rise)
	factor = -exp(-tp/tau_rise)+exp(-tp/tau_decay)
	factor = 1/factor
}

BREAKPOINT {
	SOLVE state METHOD cnexp
	g = B-A
	i = g*(v-e)
}

: runs exactly once per accepted timestep (see revision note above) -- safe
: place to push into the shared accumulator without double-counting
AFTER SOLVE {
	iacc = iacc + i
}

DERIVATIVE state{
	A' = -A/tau_rise
	B' = -B/tau_decay
}

NET_RECEIVE (weight, Pv, P, Use, t0 (ms)){
	INITIAL{
		P=1
		Use=0
		t0=t
	    }	
	Use = Use * exp(-(t-t0)/tau_fac)
	Use = Use + U1*(1-Use) 
	P   = 1-(1- P) * exp(-(t-t0)/tau_rec)
	Pv  = Use * P
	P   = P - Use * P
	
	t0 = t
	
	A = A + weight*factor*Pv: BME503--weight may not be necessarily what you think it is because a scaling factor has been left out.  Weighting factor is 1/(td*tr/(td-tr)).
	B = B + weight*factor*Pv	
}

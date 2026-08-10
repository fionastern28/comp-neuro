TITLE  NMDA receptor with Ca influx and pre-synaptic short-term plasticity


COMMENT
Dynamic presynaptic activity based on Fuhrmann et al, 2002: "Coding of temporal information by activity-dependent synapses" 

Written by Paulo Aguiar and Mafalda Sousa, IBMC, May 2008
pauloaguiar@fc.up.pt ; mafsousa@ibmc.up.pt
ENDCOMMENT


: ===========================================================================
: EDITED by Claude (Cowork) on 2026-08-07 21:31 EDT (revised 21:52 EDT)
: PURPOSE: Added POINTER iacc so this synapse can push its own current into
: the shared ExcCap accumulator (iacc_nmda) each timestep, replacing the old
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
	POINT_PROCESS NMDA_DynSyn
	USEION ca WRITE ica
	USEION mg READ mgo VALENCE 2
	RANGE tau_rise, tau_decay
	RANGE U1, tau_rec, tau_fac
	RANGE i, g, e, mg, inon, ica, ca_ratio
	POINTER iacc
	NONSPECIFIC_CURRENT inon
    }
    
UNITS {
	(nA) = (nanoamp)
	(mV) = (millivolt)
	(molar) = (1/liter)
	(mM) = (millimolar)
    }    
    
    PARAMETER {
  	tau_rise  = 5.0   (ms)  : dual-exponential conductance profile
	tau_decay = 70.0  (ms)  : IMPORTANT: tau_rise < tau_decay
	U1        = 1.0   (1)   : The parameter U1, tau_rec and tau_fac define
	tau_rec   = 0.1   (ms)  : the pre-synaptic SP short-term plasticity
	tau_fac   = 0.1   (ms)  : mechanism (see Fuhrmann et al, 2002)
	e         = 0.0   (mV)  : synapse reversal potential
	mgo		  = 1.0   (mM)  : external magnesium concentration
	ca_ratio  = 0.1   (1)   : ratio of calcium current to total current( Burnashev/Sakmann J Phys 1995 485 403-418)
    }
    
    
ASSIGNED {
	v		(mV)
	i		(nA)
	g		(umho)
	factor	(1)
	ica		(nA)
	inon	(nA)
	iacc	(nA)  : ExcCap's iacc_nmda accumulator, wired via h.setpointer (added 2026-08-07)
}

STATE {
	A
	B
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
	i = g*mgblock(v)*(v-e)
	ica = ca_ratio*i
	inon = (1-ca_ratio)*i
	:printf("\nt=%f\tinon=%f\tica=%f\ti=%f\tmgb=%f",t, inon, ica, i, mgblock(v))
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

FUNCTION mgblock(v(mV)) {
	: from Jahr & Stevens 1990
	mgblock = 1 / (1 + exp(0.062 (/mV) * -v) * (mgo / 3.57 (mM)))
}

NET_RECEIVE (weight, Pv, P, Use, t0 (ms)){
	INITIAL{
		P=1
		Use=0
		t0=t
	}	

	Use = Use * exp(-(t-t0)/tau_fac)
	Use = Use + U1*(1-Use) 
	P = 1-(1- P) * exp(-(t-t0)/tau_rec)
	Pv= Use * P
	P = P - Use * P
	
	t0=t
	
	A=A + weight*factor*Pv
	B=B + weight*factor*Pv
}


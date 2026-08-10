TITLE Bilinear synaptic-integration correction current (Li et al. 2019)

COMMENT
ADDED BY CLAUDEEEEEEE
Phenomenological point process implementing the single-excitatory-class
reduction of the bilinear dendritic-integration term from

  Li, Xiao, Cai (2019) "Dendritic computations captured by an effective
  point neuron model", PNAS 116(30):15244-15252.

For one excitatory input class the bilinear correction collapses to a single
term (the paper's notation, "current into the cell"):

  i_into_cell = alpha * gE^2 * (eE - v)

with gE = g_AMPA + g_NMDA the TOTAL excitatory conductance. Implemented here
in NEURON's outward-positive (NONSPECIFIC_CURRENT) convention as

  i = alpha * gE^2 * (v - e)                                 [nA]

SIGN: the two forms carry the SAME alpha. NEURON flips the driving-force sign
(v - e here vs. eE - v in the paper), so with e = eE the sign of alpha is
preserved. alpha < 0 therefore gives an OUTWARD (suppressive) current whose
magnitude grows with gE^2 -- i.e. dendritic saturation clamping the firing
rate. alpha = -20 in your synMechParams is already the suppressive sign.

SILENT UNDER CURRENT CLAMP: with no synaptic drive g_AMPA = g_NMDA = 0, so
gE = 0 and i = 0 for any v and any alpha. The term exists only when built
from live conductances.

SQUARING: gE is squared as (g_AMPA + g_NMDA)^2, so the cross term
2*g_AMPA*g_NMDA is included. Do NOT sum g_AMPA^2 + g_NMDA^2 separately.

NMDA CONDUCTANCE: the pointer reads the RAW NMDA conductance g = B - A from
NMDA_DynSyn, NOT the Mg-gated effective conductance g*mgblock(v). Since your
plateau is NMDA/Mg saturation, if you want gE to track the gated drive use the
commented line in BREAKPOINT (requires [Mg]o and the Jahr-Stevens factor).

POINTERS must be attached with setpointer AFTER the network is built and
BEFORE finitialize() (i.e. between sim.create() and sim.simulate()).
ENDCOMMENT

NEURON {
    POINT_PROCESS SynInteg
    POINTER g_ampa
    POINTER g_nmda
    RANGE alpha, e, i, gE, gE2
    NONSPECIFIC_CURRENT i
}

UNITS {
    (nA) = (nanoamp)
    (mV) = (millivolt)
    (uS) = (microsiemens)
}

PARAMETER {
    alpha = -20   (1/uS)   : fit knob; < 0 => outward / suppressive
    e     = 0     (mV)     : excitatory reversal (eE); match AMPA/NMDA e
}

ASSIGNED {
    v        (mV)
    i        (nA)
    g_ampa   (uS)
    g_nmda   (uS)
    gE       (uS)
    gE2
}

BREAKPOINT {
    LOCAL geff, vv
    gE = g_ampa + g_nmda
    geff = alpha*gE*gE/(1 - alpha*gE)   : bounded effective conductance coeff
    vv = v
    if (vv >  20) { vv =  20 }          : clamp driving force to a physiological window
    if (vv < -90) { vv = -90 }
    i = geff*(vv - e)
}

: --- optional, only needed if you enable the Mg-gated line above ---
: FUNCTION mgblock(vm (mV)) {
:     : Jahr & Stevens 1990, [Mg]o = 1 mM (match NMDA_DynSyn mgo)
:     mgblock = 1 / (1 + exp(0.062 (/mV) * -vm) * (1 (mM) / 3.57 (mM)))
: }

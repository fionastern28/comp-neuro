: ===========================================================================
: EDITED by Claude (Cowork) on 2026-08-07 21:31 EDT (revised 21:58 EDT)
: PURPOSE: Replaced the fixed 80-slot POINTER "pull" design (20 AMPA + 20 NMDA
: slots for i, plus 20 AMPA + 20 NMDA slots for g) with a 2-variable "push"
: accumulator design. AMPA_DynSyn / NMDA_DynSyn now add their own current
: into iacc_ampa / iacc_nmda directly, once per accepted timestep via an
: AFTER SOLVE block (see matching edits in AMPA_DynSyn.mod and
: NMDA_DynSyn.mod). This removes the old 20-synapse-per-type limit entirely
: (works for any number of synapses) and removes the need for the
: MAX_SLOTS padding/assert loop that used to live in every simulation script.
:
: Also removed at the user's request: all conductance-cap tracking
: (gtotal, gtotal_amp, gtotal_nmda, and the g_ampaN/g_nmdaN pointer slots).
: This mechanism now only tracks and caps CURRENT, not conductance.
:
: REVISION NOTE (design history, kept for anyone reading this later):
: An earlier version of this file reset iacc_ampa/iacc_nmda to 0 in a
: BEFORE BREAKPOINT block. That turned out to be wrong: BEFORE BREAKPOINT
: (all mechanisms) runs immediately before BREAKPOINT/nrn_cur (all
: mechanisms) in the SAME timestep, and the synapse push happens later,
: in AFTER SOLVE of that same timestep -- so the reset always wiped the
: accumulator before ExcCap ever got to read a nonzero value. Verified by
: testing: itotal was stuck at 0 the whole run.
:
: The fix: don't reset in BEFORE BREAKPOINT at all. Instead, read AND
: reset atomically inside this mechanism's own BREAKPOINT. That's only
: safe because of the CONDUCTANCE declaration below -- without it, NEURON
: would call BREAKPOINT twice per timestep (once at v, once at a perturbed
: v, to numerically build the implicit-integration Jacobian), and the
: reset would fire twice, corrupting the value used for that Jacobian
: estimate. icorrect never actually depends on v (it's a function of the
: synaptic accumulators only), so its true analytic conductance is exactly
: 0 -- declaring that lets NEURON skip the extra perturbed-v evaluation
: entirely, so BREAKPOINT below runs exactly once per timestep.
:
: End-to-end timing with this fix: iacc_ampa/iacc_nmda pushed by synapses
: during timestep N (in their AFTER SOLVE) are read by ExcCap at the start
: of timestep N+1 (in BREAKPOINT/nrn_cur), then immediately zeroed for the
: next cycle. So itotal/icorrect lag the actual synaptic currents by
: exactly one timestep (dt, typically 0.025-0.1 ms) -- negligible next to
: synaptic time constants, and no worse than the ambiguous same-step-or-
: one-step-lag behavior of the original 20-slot pointer design.
: ===========================================================================

NEURON {
    POINT_PROCESS ExcCap
    : iacc_ampa/iacc_nmda are REAL storage owned by ExcCap (not pointers) --
    : the AMPA/NMDA synapses each hold a POINTER back to these two variables
    : and add their own current into them. See AMPA_DynSyn.mod / NMDA_DynSyn.mod.
    RANGE cap, itotal, icorrect, iacc_ampa, iacc_nmda
    NONSPECIFIC_CURRENT icorrect
}

PARAMETER {
    cap = 0.12 (nA)
}

ASSIGNED {
    iacc_ampa (nA)  : running sum of i across all AMPA synapses on this cell (one timestep's worth)
    iacc_nmda (nA)  : running sum of i across all NMDA synapses on this cell (one timestep's worth)
    itotal (nA)
    icorrect (nA)
    gcorrect (umho) : always 0 -- icorrect does not depend on v; see CONDUCTANCE note above
}

BREAKPOINT {
    : icorrect has zero analytic dependence on v -- see REVISION NOTE above.
    CONDUCTANCE gcorrect
    gcorrect = 0

    itotal = iacc_ampa + iacc_nmda

    if (itotal < -cap) {
        icorrect = -(itotal + cap)
    } else {
        icorrect = 0
    }

    : reset for the next cycle -- safe here (read-then-reset, atomic) only
    : because CONDUCTANCE above guarantees this block runs once per timestep
    iacc_ampa = 0
    iacc_nmda = 0
}

#include <stdio.h>
#include "hocdec.h"
extern int nrnmpi_myid;
extern int nrn_nobanner_;

extern "C" void _AMPA_DynSyn_reg(void);
extern "C" void _B_A_reg(void);
extern "C" void _B_DR_reg(void);
extern "C" void _B_NA_reg(void);
extern "C" void _CaIntraCellDyn_reg(void);
extern "C" void _GABAa_DynSyn_reg(void);
extern "C" void _GABAb_DynSyn_reg(void);
extern "C" void _Glycine_DynSyn_reg(void);
extern "C" void _HH2_reg(void);
extern "C" void _HH2new_reg(void);
extern "C" void _KDR_reg(void);
extern "C" void _KDRI_reg(void);
extern "C" void _NK1_DynSyn_reg(void);
extern "C" void _NMDA_DynSyn_reg(void);
extern "C" void _SS_reg(void);
extern "C" void _borgka_reg(void);
extern "C" void _iCaAN_reg(void);
extern "C" void _iCaL_reg(void);
extern "C" void _iKCa_reg(void);
extern "C" void _iNaP_reg(void);
extern "C" void _vecevent_reg(void);
extern "C" void _vsource_reg(void);

extern "C" void modl_reg() {
  if (!nrn_nobanner_) if (nrnmpi_myid < 1) {
    fprintf(stderr, "Additional mechanisms from files\n");
    fprintf(stderr, " \"AMPA_DynSyn.mod\"");
    fprintf(stderr, " \"B_A.mod\"");
    fprintf(stderr, " \"B_DR.mod\"");
    fprintf(stderr, " \"B_NA.mod\"");
    fprintf(stderr, " \"CaIntraCellDyn.mod\"");
    fprintf(stderr, " \"GABAa_DynSyn.mod\"");
    fprintf(stderr, " \"GABAb_DynSyn.mod\"");
    fprintf(stderr, " \"Glycine_DynSyn.mod\"");
    fprintf(stderr, " \"HH2.mod\"");
    fprintf(stderr, " \"HH2new.mod\"");
    fprintf(stderr, " \"KDR.mod\"");
    fprintf(stderr, " \"KDRI.mod\"");
    fprintf(stderr, " \"NK1_DynSyn.mod\"");
    fprintf(stderr, " \"NMDA_DynSyn.mod\"");
    fprintf(stderr, " \"SS.mod\"");
    fprintf(stderr, " \"borgka.mod\"");
    fprintf(stderr, " \"iCaAN.mod\"");
    fprintf(stderr, " \"iCaL.mod\"");
    fprintf(stderr, " \"iKCa.mod\"");
    fprintf(stderr, " \"iNaP.mod\"");
    fprintf(stderr, " \"vecevent.mod\"");
    fprintf(stderr, " \"vsource.mod\"");
    fprintf(stderr, "\n");
  }
  _AMPA_DynSyn_reg();
  _B_A_reg();
  _B_DR_reg();
  _B_NA_reg();
  _CaIntraCellDyn_reg();
  _GABAa_DynSyn_reg();
  _GABAb_DynSyn_reg();
  _Glycine_DynSyn_reg();
  _HH2_reg();
  _HH2new_reg();
  _KDR_reg();
  _KDRI_reg();
  _NK1_DynSyn_reg();
  _NMDA_DynSyn_reg();
  _SS_reg();
  _borgka_reg();
  _iCaAN_reg();
  _iCaL_reg();
  _iKCa_reg();
  _iNaP_reg();
  _vecevent_reg();
  _vsource_reg();
}

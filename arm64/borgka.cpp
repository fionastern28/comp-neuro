/* Created by Language version: 7.7.0 */
/* NOT VECTORIZED */
#define NRN_VECTORIZED 0
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "mech_api.h"
#undef PI
#define nil 0
#define _pval pval
// clang-format off
#include "md1redef.h"
#include "section_fwd.hpp"
#include "nrniv_mf.h"
#include "md2redef.h"
#include "nrnconf.h"
// clang-format on
#include "neuron/cache/mechanism_range.hpp"
#include <vector>
using std::size_t;
static auto& std_cerr_stream = std::cerr;
static constexpr auto number_of_datum_variables = 3;
static constexpr auto number_of_floating_point_variables = 21;
namespace {
template <typename T>
using _nrn_mechanism_std_vector = std::vector<T>;
using _nrn_model_sorted_token = neuron::model_sorted_token;
using _nrn_mechanism_cache_range = neuron::cache::MechanismRange<number_of_floating_point_variables, number_of_datum_variables>;
using _nrn_mechanism_cache_instance = neuron::cache::MechanismInstance<number_of_floating_point_variables, number_of_datum_variables>;
using _nrn_non_owning_id_without_container = neuron::container::non_owning_identifier_without_container;
template <typename T>
using _nrn_mechanism_field = neuron::mechanism::field<T>;
template <typename... Args>
void _nrn_mechanism_register_data_fields(Args&&... args) {
  neuron::mechanism::register_data_fields(std::forward<Args>(args)...);
}
}
 
#if !NRNGPU
#undef exp
#define exp hoc_Exp
#if NRN_ENABLE_ARCH_INDEP_EXP_POW
#undef pow
#define pow hoc_pow
#endif
#endif
 
#define nrn_init _nrn_init__borgka
#define _nrn_initial _nrn_initial__borgka
#define nrn_cur _nrn_cur__borgka
#define _nrn_current _nrn_current__borgka
#define nrn_jacob _nrn_jacob__borgka
#define nrn_state _nrn_state__borgka
#define _net_receive _net_receive__borgka 
#define rates rates__borgka 
#define states states__borgka 
 
#define _threadargscomma_ /**/
#define _threadargsprotocomma_ /**/
#define _internalthreadargsprotocomma_ /**/
#define _threadargs_ /**/
#define _threadargsproto_ /**/
#define _internalthreadargsproto_ /**/
 	/*SUPPRESS 761*/
	/*SUPPRESS 762*/
	/*SUPPRESS 763*/
	/*SUPPRESS 765*/
	 extern double *hoc_getarg(int);
 
#define t nrn_threads->_t
#define dt nrn_threads->_dt
#define gkabar _ml->template fpfield<0>(_iml)
#define gkabar_columnindex 0
#define vhalfn _ml->template fpfield<1>(_iml)
#define vhalfn_columnindex 1
#define vhalfl _ml->template fpfield<2>(_iml)
#define vhalfl_columnindex 2
#define vhalfm _ml->template fpfield<3>(_iml)
#define vhalfm_columnindex 3
#define vhalfk _ml->template fpfield<4>(_iml)
#define vhalfk_columnindex 4
#define a0l _ml->template fpfield<5>(_iml)
#define a0l_columnindex 5
#define a0n _ml->template fpfield<6>(_iml)
#define a0n_columnindex 6
#define zetan _ml->template fpfield<7>(_iml)
#define zetan_columnindex 7
#define zetal _ml->template fpfield<8>(_iml)
#define zetal_columnindex 8
#define gmn _ml->template fpfield<9>(_iml)
#define gmn_columnindex 9
#define gml _ml->template fpfield<10>(_iml)
#define gml_columnindex 10
#define zetam _ml->template fpfield<11>(_iml)
#define zetam_columnindex 11
#define zetak _ml->template fpfield<12>(_iml)
#define zetak_columnindex 12
#define gka _ml->template fpfield<13>(_iml)
#define gka_columnindex 13
#define n _ml->template fpfield<14>(_iml)
#define n_columnindex 14
#define l _ml->template fpfield<15>(_iml)
#define l_columnindex 15
#define ek _ml->template fpfield<16>(_iml)
#define ek_columnindex 16
#define Dn _ml->template fpfield<17>(_iml)
#define Dn_columnindex 17
#define Dl _ml->template fpfield<18>(_iml)
#define Dl_columnindex 18
#define ik _ml->template fpfield<19>(_iml)
#define ik_columnindex 19
#define _g _ml->template fpfield<20>(_iml)
#define _g_columnindex 20
#define _ion_ek *(_ml->dptr_field<0>(_iml))
#define _p_ion_ek static_cast<neuron::container::data_handle<double>>(_ppvar[0])
#define _ion_ik *(_ml->dptr_field<1>(_iml))
#define _p_ion_ik static_cast<neuron::container::data_handle<double>>(_ppvar[1])
#define _ion_dikdv *(_ml->dptr_field<2>(_iml))
 static _nrn_mechanism_cache_instance _ml_real{nullptr};
static _nrn_mechanism_cache_range *_ml{&_ml_real};
static size_t _iml{0};
static Datum *_ppvar;
 static int hoc_nrnpointerindex =  -1;
 static Prop* _extcall_prop;
 /* _prop_id kind of shadows _extcall_prop to allow validity checking. */
 static _nrn_non_owning_id_without_container _prop_id{};
 /* external NEURON variables */
 extern double celsius;
 /* declaration of user functions */
 static void _hoc_alpm(void);
 static void _hoc_alpl(void);
 static void _hoc_alpk(void);
 static void _hoc_alpn(void);
 static void _hoc_betl(void);
 static void _hoc_betn(void);
 static void _hoc_rates(void);
 static int _mechtype;
extern void _nrn_cacheloop_reg(int, int);
extern void hoc_register_limits(int, HocParmLimits*);
extern void hoc_register_units(int, HocParmUnits*);
extern void nrn_promote(Prop*, int, int);
 
#define NMODL_TEXT 1
#if NMODL_TEXT
static void register_nmodl_text_and_filename(int mechtype);
#endif
 static void _hoc_setdata();
 /* connect user functions to hoc names */
 static VoidFunc hoc_intfunc[] = {
 {"setdata_borgka", _hoc_setdata},
 {"alpm_borgka", _hoc_alpm},
 {"alpl_borgka", _hoc_alpl},
 {"alpk_borgka", _hoc_alpk},
 {"alpn_borgka", _hoc_alpn},
 {"betl_borgka", _hoc_betl},
 {"betn_borgka", _hoc_betn},
 {"rates_borgka", _hoc_rates},
 {0, 0}
};
 
/* Direct Python call wrappers to density mechanism functions.*/
 static double _npy_alpm(Prop*);
 static double _npy_alpl(Prop*);
 static double _npy_alpk(Prop*);
 static double _npy_alpn(Prop*);
 static double _npy_betl(Prop*);
 static double _npy_betn(Prop*);
 static double _npy_rates(Prop*);
 
static NPyDirectMechFunc npy_direct_func_proc[] = {
 {"alpm", _npy_alpm},
 {"alpl", _npy_alpl},
 {"alpk", _npy_alpk},
 {"alpn", _npy_alpn},
 {"betl", _npy_betl},
 {"betn", _npy_betn},
 {"rates", _npy_rates},
 {0, 0}
};
#define alpm alpm_borgka
#define alpl alpl_borgka
#define alpk alpk_borgka
#define alpn alpn_borgka
#define betl betl_borgka
#define betn betn_borgka
 extern double alpm( double );
 extern double alpl( double );
 extern double alpk( double );
 extern double alpn( double );
 extern double betl( double );
 extern double betn( double );
 /* declare global and static user variables */
 #define gind 0
 #define _gth 0
#define linf linf_borgka
 double linf = 0;
#define ninf ninf_borgka
 double ninf = 0;
#define taun taun_borgka
 double taun = 0;
#define taul taul_borgka
 double taul = 0;
 /* some parameters have upper and lower limits */
 static HocParmLimits _hoc_parm_limits[] = {
 {0, 0, 0}
};
 static HocParmUnits _hoc_parm_units[] = {
 {"gkabar_borgka", "mho/cm2"},
 {"vhalfn_borgka", "mV"},
 {"vhalfl_borgka", "mV"},
 {"vhalfm_borgka", "mV"},
 {"vhalfk_borgka", "mV"},
 {"a0l_borgka", "/ms"},
 {"a0n_borgka", "/ms"},
 {"zetan_borgka", "1"},
 {"zetal_borgka", "1"},
 {"gmn_borgka", "1"},
 {"gml_borgka", "1"},
 {"zetam_borgka", "1"},
 {"zetak_borgka", "1"},
 {0, 0}
};
 static double delta_t = 0.01;
 static double l0 = 0;
 static double n0 = 0;
 static double v = 0;
 /* connect global user variables to hoc */
 static DoubScal hoc_scdoub[] = {
 {"ninf_borgka", &ninf_borgka},
 {"linf_borgka", &linf_borgka},
 {"taul_borgka", &taul_borgka},
 {"taun_borgka", &taun_borgka},
 {0, 0}
};
 static DoubVec hoc_vdoub[] = {
 {0, 0, 0}
};
 static double _sav_indep;
 extern void _nrn_setdata_reg(int, void(*)(Prop*));
 static void _setdata(Prop* _prop) {
 _extcall_prop = _prop;
 _prop_id = _nrn_get_prop_id(_prop);
 neuron::legacy::set_globals_from_prop(_prop, _ml_real, _ml, _iml);
_ppvar = _nrn_mechanism_access_dparam(_prop);
 Node * _node = _nrn_mechanism_access_node(_prop);
v = _nrn_mechanism_access_voltage(_node);
 }
 static void _hoc_setdata() {
 Prop *_prop, *hoc_getdata_range(int);
 _prop = hoc_getdata_range(_mechtype);
   _setdata(_prop);
 hoc_retpushx(1.);
}
 static void nrn_alloc(Prop*);
static void nrn_init(_nrn_model_sorted_token const&, NrnThread*, Memb_list*, int);
static void nrn_state(_nrn_model_sorted_token const&, NrnThread*, Memb_list*, int);
 static void nrn_cur(_nrn_model_sorted_token const&, NrnThread*, Memb_list*, int);
static void nrn_jacob(_nrn_model_sorted_token const&, NrnThread*, Memb_list*, int);
 
static int _ode_count(int);
static void _ode_map(Prop*, int, neuron::container::data_handle<double>*, neuron::container::data_handle<double>*, double*, int);
static void _ode_spec(_nrn_model_sorted_token const&, NrnThread*, Memb_list*, int);
static void _ode_matsol(_nrn_model_sorted_token const&, NrnThread*, Memb_list*, int);
 
#define _cvode_ieq _ppvar[3].literal_value<int>()
 static void _ode_matsol_instance1(_internalthreadargsproto_);
 /* connect range variables in _p that hoc is supposed to know about */
 static const char *_mechanism[] = {
 "7.7.0",
"borgka",
 "gkabar_borgka",
 "vhalfn_borgka",
 "vhalfl_borgka",
 "vhalfm_borgka",
 "vhalfk_borgka",
 "a0l_borgka",
 "a0n_borgka",
 "zetan_borgka",
 "zetal_borgka",
 "gmn_borgka",
 "gml_borgka",
 "zetam_borgka",
 "zetak_borgka",
 0,
 "gka_borgka",
 0,
 "n_borgka",
 "l_borgka",
 0,
 0};
 static Symbol* _k_sym;
 
 /* Used by NrnProperty */
 static _nrn_mechanism_std_vector<double> _parm_default{
     0.012, /* gkabar */
     -45, /* vhalfn */
     -67, /* vhalfl */
     -67, /* vhalfm */
     -45, /* vhalfk */
     0.023, /* a0l */
     0.04, /* a0n */
     -4, /* zetan */
     2, /* zetal */
     0.45, /* gmn */
     1, /* gml */
     4, /* zetam */
     -5, /* zetak */
 }; 
 
 
extern Prop* need_memb(Symbol*);
static void nrn_alloc(Prop* _prop) {
  Prop *prop_ion{};
  Datum *_ppvar{};
   _ppvar = nrn_prop_datum_alloc(_mechtype, 4, _prop);
    _nrn_mechanism_access_dparam(_prop) = _ppvar;
     _nrn_mechanism_cache_instance _ml_real{_prop};
    auto* const _ml = &_ml_real;
    size_t const _iml{};
    assert(_nrn_mechanism_get_num_vars(_prop) == 21);
 	/*initialize range parameters*/
 	gkabar = _parm_default[0]; /* 0.012 */
 	vhalfn = _parm_default[1]; /* -45 */
 	vhalfl = _parm_default[2]; /* -67 */
 	vhalfm = _parm_default[3]; /* -67 */
 	vhalfk = _parm_default[4]; /* -45 */
 	a0l = _parm_default[5]; /* 0.023 */
 	a0n = _parm_default[6]; /* 0.04 */
 	zetan = _parm_default[7]; /* -4 */
 	zetal = _parm_default[8]; /* 2 */
 	gmn = _parm_default[9]; /* 0.45 */
 	gml = _parm_default[10]; /* 1 */
 	zetam = _parm_default[11]; /* 4 */
 	zetak = _parm_default[12]; /* -5 */
 	 assert(_nrn_mechanism_get_num_vars(_prop) == 21);
 	_nrn_mechanism_access_dparam(_prop) = _ppvar;
 	/*connect ionic variables to this model*/
 prop_ion = need_memb(_k_sym);
 nrn_promote(prop_ion, 0, 1);
 	_ppvar[0] = _nrn_mechanism_get_param_handle(prop_ion, 0); /* ek */
 	_ppvar[1] = _nrn_mechanism_get_param_handle(prop_ion, 3); /* ik */
 	_ppvar[2] = _nrn_mechanism_get_param_handle(prop_ion, 4); /* _ion_dikdv */
 
}
 static void _initlists();
  /* some states have an absolute tolerance */
 static Symbol** _atollist;
 static HocStateTolerance _hoc_state_tol[] = {
 {0, 0}
};
 extern Symbol* hoc_lookup(const char*);
extern void _nrn_thread_reg(int, int, void(*)(Datum*));
void _nrn_thread_table_reg(int, nrn_thread_table_check_t);
extern void hoc_register_tolerance(int, HocStateTolerance*, Symbol***);
extern void _cvode_abstol( Symbol**, double*, int);

 extern "C" void _borgka_reg() {
	int _vectorized = 0;
  _initlists();
 	ion_reg("k", -10000.);
 	_k_sym = hoc_lookup("k_ion");
 	register_mech(_mechanism, nrn_alloc,nrn_cur, nrn_jacob, nrn_state, nrn_init, hoc_nrnpointerindex, 0);
 _mechtype = nrn_get_mechtype(_mechanism[1]);
 hoc_register_parm_default(_mechtype, &_parm_default);
         hoc_register_npy_direct(_mechtype, npy_direct_func_proc);
     _nrn_setdata_reg(_mechtype, _setdata);
 #if NMODL_TEXT
  register_nmodl_text_and_filename(_mechtype);
#endif
   _nrn_mechanism_register_data_fields(_mechtype,
                                       _nrn_mechanism_field<double>{"gkabar"} /* 0 */,
                                       _nrn_mechanism_field<double>{"vhalfn"} /* 1 */,
                                       _nrn_mechanism_field<double>{"vhalfl"} /* 2 */,
                                       _nrn_mechanism_field<double>{"vhalfm"} /* 3 */,
                                       _nrn_mechanism_field<double>{"vhalfk"} /* 4 */,
                                       _nrn_mechanism_field<double>{"a0l"} /* 5 */,
                                       _nrn_mechanism_field<double>{"a0n"} /* 6 */,
                                       _nrn_mechanism_field<double>{"zetan"} /* 7 */,
                                       _nrn_mechanism_field<double>{"zetal"} /* 8 */,
                                       _nrn_mechanism_field<double>{"gmn"} /* 9 */,
                                       _nrn_mechanism_field<double>{"gml"} /* 10 */,
                                       _nrn_mechanism_field<double>{"zetam"} /* 11 */,
                                       _nrn_mechanism_field<double>{"zetak"} /* 12 */,
                                       _nrn_mechanism_field<double>{"gka"} /* 13 */,
                                       _nrn_mechanism_field<double>{"n"} /* 14 */,
                                       _nrn_mechanism_field<double>{"l"} /* 15 */,
                                       _nrn_mechanism_field<double>{"ek"} /* 16 */,
                                       _nrn_mechanism_field<double>{"Dn"} /* 17 */,
                                       _nrn_mechanism_field<double>{"Dl"} /* 18 */,
                                       _nrn_mechanism_field<double>{"ik"} /* 19 */,
                                       _nrn_mechanism_field<double>{"_g"} /* 20 */,
                                       _nrn_mechanism_field<double*>{"_ion_ek", "k_ion"} /* 0 */,
                                       _nrn_mechanism_field<double*>{"_ion_ik", "k_ion"} /* 1 */,
                                       _nrn_mechanism_field<double*>{"_ion_dikdv", "k_ion"} /* 2 */,
                                       _nrn_mechanism_field<int>{"_cvode_ieq", "cvodeieq"} /* 3 */);
  hoc_register_prop_size(_mechtype, 21, 4);
  hoc_register_dparam_semantics(_mechtype, 0, "k_ion");
  hoc_register_dparam_semantics(_mechtype, 1, "k_ion");
  hoc_register_dparam_semantics(_mechtype, 2, "k_ion");
  hoc_register_dparam_semantics(_mechtype, 3, "cvodeieq");
 	hoc_register_cvode(_mechtype, _ode_count, _ode_map, _ode_spec, _ode_matsol);
 	hoc_register_tolerance(_mechtype, _hoc_state_tol, &_atollist);
 
    hoc_register_var(hoc_scdoub, hoc_vdoub, hoc_intfunc);
 	ivoc_help("help ?1 borgka /Users/fionastern/Desktop/comp-neuro/mods/borgka.mod\n");
 hoc_register_limits(_mechtype, _hoc_parm_limits);
 hoc_register_units(_mechtype, _hoc_parm_units);
 }
static int _reset;
static const char *modelname = "Borg-Graham type generic K-A channel for a Sympathetic Preganglionic Neuron";

static int error;
static int _ninits = 0;
static int _match_recurse=1;
static void _modl_cleanup(){ _match_recurse=1;}
static int rates(double);
 
static int _ode_spec1(_internalthreadargsproto_);
/*static int _ode_matsol1(_internalthreadargsproto_);*/
 static neuron::container::field_index _slist1[2], _dlist1[2];
 static int states(_internalthreadargsproto_);
 
double alpn (  double _lv ) {
   double _lalpn;
 _lalpn = exp ( 1.e-3 * zetan * ( _lv - vhalfn ) * 9.648e4 / ( 8.315 * ( 273.16 + celsius ) ) ) ;
   
return _lalpn;
 }
 
static void _hoc_alpn(void) {
  double _r;
  
  if(!_prop_id) {
    hoc_execerror("No data for alpn_borgka. Requires prior call to setdata_borgka and that the specified mechanism instance still be in existence.", NULL);
  } else {
    _setdata(_extcall_prop);
  }
   _r =  alpn (  *getarg(1) );
 hoc_retpushx(_r);
}
 
static double _npy_alpn(Prop* _prop) {
    double _r{0.0};
    neuron::legacy::set_globals_from_prop(_prop, _ml_real, _ml, _iml);
  _ppvar = _nrn_mechanism_access_dparam(_prop);
 _r =  alpn (  *getarg(1) );
 return(_r);
}
 
double alpk (  double _lv ) {
   double _lalpk;
 _lalpk = exp ( 1.e-3 * zetak * ( _lv - vhalfk ) * 9.648e4 / ( 8.315 * ( 273.16 + celsius ) ) ) ;
   
return _lalpk;
 }
 
static void _hoc_alpk(void) {
  double _r;
  
  if(!_prop_id) {
    hoc_execerror("No data for alpk_borgka. Requires prior call to setdata_borgka and that the specified mechanism instance still be in existence.", NULL);
  } else {
    _setdata(_extcall_prop);
  }
   _r =  alpk (  *getarg(1) );
 hoc_retpushx(_r);
}
 
static double _npy_alpk(Prop* _prop) {
    double _r{0.0};
    neuron::legacy::set_globals_from_prop(_prop, _ml_real, _ml, _iml);
  _ppvar = _nrn_mechanism_access_dparam(_prop);
 _r =  alpk (  *getarg(1) );
 return(_r);
}
 
double betn (  double _lv ) {
   double _lbetn;
 _lbetn = exp ( 1.e-3 * zetan * gmn * ( _lv - vhalfn ) * 9.648e4 / ( 8.315 * ( 273.16 + celsius ) ) ) ;
   
return _lbetn;
 }
 
static void _hoc_betn(void) {
  double _r;
  
  if(!_prop_id) {
    hoc_execerror("No data for betn_borgka. Requires prior call to setdata_borgka and that the specified mechanism instance still be in existence.", NULL);
  } else {
    _setdata(_extcall_prop);
  }
   _r =  betn (  *getarg(1) );
 hoc_retpushx(_r);
}
 
static double _npy_betn(Prop* _prop) {
    double _r{0.0};
    neuron::legacy::set_globals_from_prop(_prop, _ml_real, _ml, _iml);
  _ppvar = _nrn_mechanism_access_dparam(_prop);
 _r =  betn (  *getarg(1) );
 return(_r);
}
 
double alpl (  double _lv ) {
   double _lalpl;
 _lalpl = exp ( 1.e-3 * zetal * ( _lv - vhalfl ) * 9.648e4 / ( 8.315 * ( 273.16 + celsius ) ) ) ;
   
return _lalpl;
 }
 
static void _hoc_alpl(void) {
  double _r;
  
  if(!_prop_id) {
    hoc_execerror("No data for alpl_borgka. Requires prior call to setdata_borgka and that the specified mechanism instance still be in existence.", NULL);
  } else {
    _setdata(_extcall_prop);
  }
   _r =  alpl (  *getarg(1) );
 hoc_retpushx(_r);
}
 
static double _npy_alpl(Prop* _prop) {
    double _r{0.0};
    neuron::legacy::set_globals_from_prop(_prop, _ml_real, _ml, _iml);
  _ppvar = _nrn_mechanism_access_dparam(_prop);
 _r =  alpl (  *getarg(1) );
 return(_r);
}
 
double alpm (  double _lv ) {
   double _lalpm;
 _lalpm = exp ( 1.e-3 * zetam * ( _lv - vhalfm ) * 9.648e4 / ( 8.315 * ( 273.16 + celsius ) ) ) ;
   
return _lalpm;
 }
 
static void _hoc_alpm(void) {
  double _r;
  
  if(!_prop_id) {
    hoc_execerror("No data for alpm_borgka. Requires prior call to setdata_borgka and that the specified mechanism instance still be in existence.", NULL);
  } else {
    _setdata(_extcall_prop);
  }
   _r =  alpm (  *getarg(1) );
 hoc_retpushx(_r);
}
 
static double _npy_alpm(Prop* _prop) {
    double _r{0.0};
    neuron::legacy::set_globals_from_prop(_prop, _ml_real, _ml, _iml);
  _ppvar = _nrn_mechanism_access_dparam(_prop);
 _r =  alpm (  *getarg(1) );
 return(_r);
}
 
double betl (  double _lv ) {
   double _lbetl;
 _lbetl = exp ( 1.e-3 * zetal * gml * ( _lv - vhalfl ) * 9.648e4 / ( 8.315 * ( 273.16 + celsius ) ) ) ;
   
return _lbetl;
 }
 
static void _hoc_betl(void) {
  double _r;
  
  if(!_prop_id) {
    hoc_execerror("No data for betl_borgka. Requires prior call to setdata_borgka and that the specified mechanism instance still be in existence.", NULL);
  } else {
    _setdata(_extcall_prop);
  }
   _r =  betl (  *getarg(1) );
 hoc_retpushx(_r);
}
 
static double _npy_betl(Prop* _prop) {
    double _r{0.0};
    neuron::legacy::set_globals_from_prop(_prop, _ml_real, _ml, _iml);
  _ppvar = _nrn_mechanism_access_dparam(_prop);
 _r =  betl (  *getarg(1) );
 return(_r);
}
 
/*CVODE*/
 static int _ode_spec1 () {_reset=0;
 {
   rates ( _threadargscomma_ v ) ;
   Dn = ( ninf - n ) / taun ;
   Dl = ( linf - l ) / taul ;
   }
 return _reset;
}
 static int _ode_matsol1 () {
 rates ( _threadargscomma_ v ) ;
 Dn = Dn  / (1. - dt*( ( ( ( - 1.0 ) ) ) / taun )) ;
 Dl = Dl  / (1. - dt*( ( ( ( - 1.0 ) ) ) / taul )) ;
  return 0;
}
 /*END CVODE*/
 static int states () {_reset=0;
 {
   rates ( _threadargscomma_ v ) ;
    n = n + (1. - exp(dt*(( ( ( - 1.0 ) ) ) / taun)))*(- ( ( ( ninf ) ) / taun ) / ( ( ( ( - 1.0 ) ) ) / taun ) - n) ;
    l = l + (1. - exp(dt*(( ( ( - 1.0 ) ) ) / taul)))*(- ( ( ( linf ) ) / taul ) / ( ( ( ( - 1.0 ) ) ) / taul ) - l) ;
   }
  return 0;
}
 
static int  rates (  double _lv ) {
   double _la , _lq10 , _lb ;
 _lq10 = pow( 3.0 , ( ( celsius - 30.0 ) / 10.0 ) ) ;
   _la = alpn ( _threadargscomma_ _lv ) ;
   _lb = alpk ( _threadargscomma_ _lv ) ;
   ninf = 1.0 / ( 1.0 + _lb ) ;
   taun = betn ( _threadargscomma_ _lv ) / ( _lq10 * a0n * ( 1.0 + _la ) ) ;
   _la = alpl ( _threadargscomma_ _lv ) ;
   _lb = alpm ( _threadargscomma_ _lv ) ;
   linf = 1.0 / ( 1.0 + _lb ) ;
   taul = betl ( _threadargscomma_ _lv ) / ( _lq10 * a0l * ( 1.0 + _la ) ) ;
    return 0; }
 
static void _hoc_rates(void) {
  double _r;
  
  if(!_prop_id) {
    hoc_execerror("No data for rates_borgka. Requires prior call to setdata_borgka and that the specified mechanism instance still be in existence.", NULL);
  } else {
    _setdata(_extcall_prop);
  }
   _r = 1.;
 rates (  *getarg(1) );
 hoc_retpushx(_r);
}
 
static double _npy_rates(Prop* _prop) {
    double _r{0.0};
    neuron::legacy::set_globals_from_prop(_prop, _ml_real, _ml, _iml);
  _ppvar = _nrn_mechanism_access_dparam(_prop);
 _r = 1.;
 rates (  *getarg(1) );
 return(_r);
}
 
static int _ode_count(int _type){ return 2;}
 
static void _ode_spec(_nrn_model_sorted_token const& _sorted_token, NrnThread* _nt, Memb_list* _ml_arg, int _type) {
      Node* _nd{};
  double _v{};
  int _cntml;
  _nrn_mechanism_cache_range _lmr{_sorted_token, *_nt, *_ml_arg, _type};
  _ml = &_lmr;
  _cntml = _ml_arg->_nodecount;
  Datum *_thread{_ml_arg->_thread};
  double* _globals = nullptr;
  if (gind != 0 && _thread != nullptr) { _globals = _thread[_gth].get<double*>(); }
  for (_iml = 0; _iml < _cntml; ++_iml) {
    _ppvar = _ml_arg->_pdata[_iml];
    _nd = _ml_arg->_nodelist[_iml];
    v = NODEV(_nd);
  ek = _ion_ek;
     _ode_spec1 ();
  }}
 
static void _ode_map(Prop* _prop, int _ieq, neuron::container::data_handle<double>* _pv, neuron::container::data_handle<double>* _pvdot, double* _atol, int _type) { 
  _ppvar = _nrn_mechanism_access_dparam(_prop);
  _cvode_ieq = _ieq;
  for (int _i=0; _i < 2; ++_i) {
    _pv[_i] = _nrn_mechanism_get_param_handle(_prop, _slist1[_i]);
    _pvdot[_i] = _nrn_mechanism_get_param_handle(_prop, _dlist1[_i]);
    _cvode_abstol(_atollist, _atol, _i);
  }
 }
 
static void _ode_matsol_instance1(_internalthreadargsproto_) {
 _ode_matsol1 ();
 }
 
static void _ode_matsol(_nrn_model_sorted_token const& _sorted_token, NrnThread* _nt, Memb_list* _ml_arg, int _type) {
      Node* _nd{};
  double _v{};
  int _cntml;
  _nrn_mechanism_cache_range _lmr{_sorted_token, *_nt, *_ml_arg, _type};
  _ml = &_lmr;
  _cntml = _ml_arg->_nodecount;
  Datum *_thread{_ml_arg->_thread};
  double* _globals = nullptr;
  if (gind != 0 && _thread != nullptr) { _globals = _thread[_gth].get<double*>(); }
  for (_iml = 0; _iml < _cntml; ++_iml) {
    _ppvar = _ml_arg->_pdata[_iml];
    _nd = _ml_arg->_nodelist[_iml];
    v = NODEV(_nd);
  ek = _ion_ek;
 _ode_matsol_instance1(_threadargs_);
 }}

static void initmodel() {
  int _i; double _save;_ninits++;
 _save = t;
 t = 0.0;
{
  l = l0;
  n = n0;
 {
   rates ( _threadargscomma_ v ) ;
   n = ninf ;
   l = linf ;
   }
  _sav_indep = t; t = _save;

}
}

static void nrn_init(_nrn_model_sorted_token const& _sorted_token, NrnThread* _nt, Memb_list* _ml_arg, int _type){
Node *_nd; double _v; int* _ni; int _cntml;
_nrn_mechanism_cache_range _lmr{_sorted_token, *_nt, *_ml_arg, _type};
auto* const _vec_v = _nt->node_voltage_storage();
_ml = &_lmr;
_ni = _ml_arg->_nodeindices;
_cntml = _ml_arg->_nodecount;
for (_iml = 0; _iml < _cntml; ++_iml) {
 _ppvar = _ml_arg->_pdata[_iml];
   _v = _vec_v[_ni[_iml]];
 v = _v;
  ek = _ion_ek;
 initmodel();
 }}

static double _nrn_current(double _v){double _current=0.;v=_v;{ {
   gka = gkabar * n * l ;
   ik = gka * ( v - ek ) ;
   }
 _current += ik;

} return _current;
}

static void nrn_cur(_nrn_model_sorted_token const& _sorted_token, NrnThread* _nt, Memb_list* _ml_arg, int _type){
_nrn_mechanism_cache_range _lmr{_sorted_token, *_nt, *_ml_arg, _type};
auto const _vec_rhs = _nt->node_rhs_storage();
auto const _vec_sav_rhs = _nt->node_sav_rhs_storage();
auto const _vec_v = _nt->node_voltage_storage();
Node *_nd; int* _ni; double _rhs, _v; int _cntml;
_ml = &_lmr;
_ni = _ml_arg->_nodeindices;
_cntml = _ml_arg->_nodecount;
for (_iml = 0; _iml < _cntml; ++_iml) {
 _ppvar = _ml_arg->_pdata[_iml];
   _v = _vec_v[_ni[_iml]];
  ek = _ion_ek;
 auto const _g_local = _nrn_current(_v + .001);
 	{ double _dik;
  _dik = ik;
 _rhs = _nrn_current(_v);
  _ion_dikdv += (_dik - ik)/.001 ;
 	}
 _g = (_g_local - _rhs)/.001;
  _ion_ik += ik ;
	 _vec_rhs[_ni[_iml]] -= _rhs;
 
}}

static void nrn_jacob(_nrn_model_sorted_token const& _sorted_token, NrnThread* _nt, Memb_list* _ml_arg, int _type) {
_nrn_mechanism_cache_range _lmr{_sorted_token, *_nt, *_ml_arg, _type};
auto const _vec_d = _nt->node_d_storage();
auto const _vec_sav_d = _nt->node_sav_d_storage();
auto* const _ml = &_lmr;
Node *_nd; int* _ni; int _iml, _cntml;
_ni = _ml_arg->_nodeindices;
_cntml = _ml_arg->_nodecount;
for (_iml = 0; _iml < _cntml; ++_iml) {
  _vec_d[_ni[_iml]] += _g;
 
}}

static void nrn_state(_nrn_model_sorted_token const& _sorted_token, NrnThread* _nt, Memb_list* _ml_arg, int _type){
Node *_nd; double _v = 0.0; int* _ni; int _cntml;
_nrn_mechanism_cache_range _lmr{_sorted_token, *_nt, *_ml_arg, _type};
auto* const _vec_v = _nt->node_voltage_storage();
_ml = &_lmr;
_ni = _ml_arg->_nodeindices;
_cntml = _ml_arg->_nodecount;
for (_iml = 0; _iml < _cntml; ++_iml) {
 _ppvar = _ml_arg->_pdata[_iml];
 _nd = _ml_arg->_nodelist[_iml];
   _v = _vec_v[_ni[_iml]];
 v=_v;
{
  ek = _ion_ek;
 { error =  states();
 if(error){
  std_cerr_stream << "at line 73 in file borgka.mod:\nBREAKPOINT {\n";
  std_cerr_stream << _ml << ' ' << _iml << '\n';
  abort_run(error);
}
 } }}

}

static void terminal(){}

static void _initlists() {
 int _i; static int _first = 1;
  if (!_first) return;
 _slist1[0] = {n_columnindex, 0};  _dlist1[0] = {Dn_columnindex, 0};
 _slist1[1] = {l_columnindex, 0};  _dlist1[1] = {Dl_columnindex, 0};
_first = 0;
}

#if NMODL_TEXT
static void register_nmodl_text_and_filename(int mech_type) {
    const char* nmodl_filename = "/Users/fionastern/Desktop/comp-neuro/mods/borgka.mod";
    const char* nmodl_file_text = 
  "TITLE Borg-Graham type generic K-A channel for a Sympathetic Preganglionic Neuron\n"
  "\n"
  "COMMENT\n"
  "	Description: A-type transient K current for a Sympathetic Preganglionic Neuron.	\n"
  "	Author: Linford Briant\n"
  "	          \n"
  "	A-type transient K current = \"IA\"\n"
  "	\n"
  "	Sympathetic Preganglionic Neurones = \"SPNs\"\n"
  "	\n"
  "	Note that this is a modified version of IA found widely on SenseLab. This version has had steady-state kinetics \n"
  "	that have been fit to data for the IA in SPN according to Whyment et al. (2011).\n"
  "	\n"
  "	Whyment et al. (2011), PMID: 21211550\n"
  "\n"
  "ENDCOMMENT\n"
  "\n"
  "\n"
  "\n"
  "UNITS {\n"
  "	(mA) = (milliamp)\n"
  "	(mV) = (millivolt)\n"
  "\n"
  "}\n"
  "\n"
  "PARAMETER {\n"
  "	v 		(mV)\n"
  "	ek 		(mV)\n"
  "	celsius 	(degC)\n"
  "	gkabar=0.012 	(mho/cm2)\n"
  "	vhalfn=-45	(mV)\n"
  "	vhalfl=-67	(mV)\n"
  "	vhalfm=-67	(mV)\n"
  "	vhalfk=-45	(mV)\n"
  "	a0l=0.023	(/ms)\n"
  "	a0n=0.04	(/ms)\n"
  "	zetan=-4	(1)\n"
  "	zetal=2    	(1)\n"
  "	gmn=0.45   	(1)\n"
  "	gml=1 	  	(1)\n"
  "	zetam=4		(1)\n"
  "	zetak=-5	(1)\n"
  "}\n"
  "\n"
  "\n"
  "NEURON {\n"
  "	SUFFIX borgka\n"
  "	USEION k READ ek WRITE ik\n"
  "        RANGE gkabar,gka,vhalfn,vhalfl,a0l,a0n,zetan,zetal,gmn,gml,zetam,zetak,vhalfm,vhalfk\n"
  "        GLOBAL ninf,linf,taul,taun\n"
  "}\n"
  "\n"
  "STATE {\n"
  "	n\n"
  "	l\n"
  "}\n"
  "\n"
  "INITIAL {\n"
  "        rates(v)\n"
  "        n=ninf\n"
  "        l=linf\n"
  "}\n"
  "\n"
  "ASSIGNED {\n"
  "	ik (mA/cm2)\n"
  "        ninf\n"
  "        linf      \n"
  "        taul\n"
  "        taun\n"
  "        gka\n"
  "}\n"
  "\n"
  "BREAKPOINT {\n"
  "	SOLVE states METHOD cnexp\n"
  "	gka = gkabar*n*l\n"
  "	ik = gka*(v-ek)\n"
  "}\n"
  "\n"
  "\n"
  "FUNCTION alpn(v(mV)) {\n"
  "  alpn = exp(1.e-3*zetan*(v-vhalfn)*9.648e4/(8.315*(273.16+celsius))) \n"
  "}\n"
  "\n"
  "FUNCTION alpk(v(mV)) {\n"
  "  alpk = exp(1.e-3*zetak*(v-vhalfk)*9.648e4/(8.315*(273.16+celsius))) \n"
  "}\n"
  "\n"
  "FUNCTION betn(v(mV)) {\n"
  "  betn = exp(1.e-3*zetan*gmn*(v-vhalfn)*9.648e4/(8.315*(273.16+celsius))) \n"
  "}\n"
  "\n"
  "FUNCTION alpl(v(mV)) {\n"
  "  alpl = exp(1.e-3*zetal*(v-vhalfl)*9.648e4/(8.315*(273.16+celsius))) \n"
  "}\n"
  "\n"
  "FUNCTION alpm(v(mV)) {\n"
  "  alpm = exp(1.e-3*zetam*(v-vhalfm)*9.648e4/(8.315*(273.16+celsius))) \n"
  "}\n"
  "\n"
  "FUNCTION betl(v(mV)) {\n"
  "  betl = exp(1.e-3*zetal*gml*(v-vhalfl)*9.648e4/(8.315*(273.16+celsius))) \n"
  "}\n"
  "\n"
  "DERIVATIVE states { \n"
  "        rates(v)\n"
  "        n' = (ninf - n)/taun\n"
  "        l' = (linf - l)/taul\n"
  "}\n"
  "\n"
  "PROCEDURE rates(v (mV)) { :callable from hoc\n"
  "        LOCAL a,q10,b\n"
  "        q10=3^((celsius-30)/10)\n"
  "        a = alpn(v)\n"
  "	b = alpk(v)\n"
  "        ninf = 1/(1 + b)\n"
  "        taun = betn(v)/(q10*a0n*(1 + a))\n"
  "        a = alpl(v)\n"
  "	b = alpm(v)\n"
  "        linf = 1/(1 + b)\n"
  "        taul = betl(v)/(q10*a0l*(1 + a))\n"
  "}\n"
  ;
    hoc_reg_nmodl_filename(mech_type, nmodl_filename);
    hoc_reg_nmodl_text(mech_type, nmodl_file_text);
}
#endif

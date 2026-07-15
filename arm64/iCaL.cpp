/* Created by Language version: 7.7.0 */
/* VECTORIZED */
#define NRN_VECTORIZED 1
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
static constexpr auto number_of_datum_variables = 4;
static constexpr auto number_of_floating_point_variables = 11;
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
 
#define nrn_init _nrn_init__iCaL
#define _nrn_initial _nrn_initial__iCaL
#define nrn_cur _nrn_cur__iCaL
#define _nrn_current _nrn_current__iCaL
#define nrn_jacob _nrn_jacob__iCaL
#define nrn_state _nrn_state__iCaL
#define _net_receive _net_receive__iCaL 
#define rates rates__iCaL 
#define states states__iCaL 
 
#define _threadargscomma_ _ml, _iml, _ppvar, _thread, _globals, _nt,
#define _threadargsprotocomma_ Memb_list* _ml, size_t _iml, Datum* _ppvar, Datum* _thread, double* _globals, NrnThread* _nt,
#define _internalthreadargsprotocomma_ _nrn_mechanism_cache_range* _ml, size_t _iml, Datum* _ppvar, Datum* _thread, double* _globals, NrnThread* _nt,
#define _threadargs_ _ml, _iml, _ppvar, _thread, _globals, _nt
#define _threadargsproto_ Memb_list* _ml, size_t _iml, Datum* _ppvar, Datum* _thread, double* _globals, NrnThread* _nt
#define _internalthreadargsproto_ _nrn_mechanism_cache_range* _ml, size_t _iml, Datum* _ppvar, Datum* _thread, double* _globals, NrnThread* _nt
 	/*SUPPRESS 761*/
	/*SUPPRESS 762*/
	/*SUPPRESS 763*/
	/*SUPPRESS 765*/
	 extern double *hoc_getarg(int);
 
#define t _nt->_t
#define dt _nt->_dt
#define pcabar _ml->template fpfield<0>(_iml)
#define pcabar_columnindex 0
#define ica _ml->template fpfield<1>(_iml)
#define ica_columnindex 1
#define tau_m _ml->template fpfield<2>(_iml)
#define tau_m_columnindex 2
#define m_inf _ml->template fpfield<3>(_iml)
#define m_inf_columnindex 3
#define m _ml->template fpfield<4>(_iml)
#define m_columnindex 4
#define cai _ml->template fpfield<5>(_iml)
#define cai_columnindex 5
#define cao _ml->template fpfield<6>(_iml)
#define cao_columnindex 6
#define Dm _ml->template fpfield<7>(_iml)
#define Dm_columnindex 7
#define tadj _ml->template fpfield<8>(_iml)
#define tadj_columnindex 8
#define v _ml->template fpfield<9>(_iml)
#define v_columnindex 9
#define _g _ml->template fpfield<10>(_iml)
#define _g_columnindex 10
#define _ion_cai *(_ml->dptr_field<0>(_iml))
#define _p_ion_cai static_cast<neuron::container::data_handle<double>>(_ppvar[0])
#define _ion_cao *(_ml->dptr_field<1>(_iml))
#define _p_ion_cao static_cast<neuron::container::data_handle<double>>(_ppvar[1])
#define _ion_ica *(_ml->dptr_field<2>(_iml))
#define _p_ion_ica static_cast<neuron::container::data_handle<double>>(_ppvar[2])
#define _ion_dicadv *(_ml->dptr_field<3>(_iml))
 /* Thread safe. No static _ml, _iml or _ppvar. */
 static int hoc_nrnpointerindex =  -1;
 static _nrn_mechanism_std_vector<Datum> _extcall_thread;
 static Prop* _extcall_prop;
 /* _prop_id kind of shadows _extcall_prop to allow validity checking. */
 static _nrn_non_owning_id_without_container _prop_id{};
 /* external NEURON variables */
 extern double celsius;
 /* declaration of user functions */
 static void _hoc_ghk(void);
 static void _hoc_rates(void);
 static void _hoc_states(void);
 static void _hoc_vtrap(void);
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
 {"setdata_iCaL", _hoc_setdata},
 {"ghk_iCaL", _hoc_ghk},
 {"rates_iCaL", _hoc_rates},
 {"states_iCaL", _hoc_states},
 {"vtrap_iCaL", _hoc_vtrap},
 {0, 0}
};
 
/* Direct Python call wrappers to density mechanism functions.*/
 static double _npy_ghk(Prop*);
 static double _npy_rates(Prop*);
 static double _npy_states(Prop*);
 static double _npy_vtrap(Prop*);
 
static NPyDirectMechFunc npy_direct_func_proc[] = {
 {"ghk", _npy_ghk},
 {"rates", _npy_rates},
 {"states", _npy_states},
 {"vtrap", _npy_vtrap},
 {0, 0}
};
#define ghk ghk_iCaL
#define vtrap vtrap_iCaL
 extern double ghk( _internalthreadargsprotocomma_ double , double , double , double );
 extern double vtrap( _internalthreadargsprotocomma_ double , double );
 /* declare global and static user variables */
 #define gind 0
 #define _gth 0
 /* some parameters have upper and lower limits */
 static HocParmLimits _hoc_parm_limits[] = {
 {0, 0, 0}
};
 static HocParmUnits _hoc_parm_units[] = {
 {"pcabar_iCaL", "cm/s"},
 {"ica_iCaL", "mA/cm2"},
 {"tau_m_iCaL", "ms"},
 {0, 0}
};
 static double delta_t = 0.01;
 static double m0 = 0;
 /* connect global user variables to hoc */
 static DoubScal hoc_scdoub[] = {
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
 /* connect range variables in _p that hoc is supposed to know about */
 static const char *_mechanism[] = {
 "7.7.0",
"iCaL",
 "pcabar_iCaL",
 0,
 "ica_iCaL",
 "tau_m_iCaL",
 "m_inf_iCaL",
 0,
 "m_iCaL",
 0,
 0};
 static Symbol* _ca_sym;
 
 /* Used by NrnProperty */
 static _nrn_mechanism_std_vector<double> _parm_default{
     0.000276, /* pcabar */
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
    assert(_nrn_mechanism_get_num_vars(_prop) == 11);
 	/*initialize range parameters*/
 	pcabar = _parm_default[0]; /* 0.000276 */
 	 assert(_nrn_mechanism_get_num_vars(_prop) == 11);
 	_nrn_mechanism_access_dparam(_prop) = _ppvar;
 	/*connect ionic variables to this model*/
 prop_ion = need_memb(_ca_sym);
 nrn_promote(prop_ion, 1, 0);
 	_ppvar[0] = _nrn_mechanism_get_param_handle(prop_ion, 1); /* cai */
 	_ppvar[1] = _nrn_mechanism_get_param_handle(prop_ion, 2); /* cao */
 	_ppvar[2] = _nrn_mechanism_get_param_handle(prop_ion, 3); /* ica */
 	_ppvar[3] = _nrn_mechanism_get_param_handle(prop_ion, 4); /* _ion_dicadv */
 
}
 static void _initlists();
 extern Symbol* hoc_lookup(const char*);
extern void _nrn_thread_reg(int, int, void(*)(Datum*));
void _nrn_thread_table_reg(int, nrn_thread_table_check_t);
extern void hoc_register_tolerance(int, HocStateTolerance*, Symbol***);
extern void _cvode_abstol( Symbol**, double*, int);

 extern "C" void _iCaL_reg() {
	int _vectorized = 1;
  _initlists();
 	ion_reg("ca", -10000.);
 	_ca_sym = hoc_lookup("ca_ion");
 	register_mech(_mechanism, nrn_alloc,nrn_cur, nrn_jacob, nrn_state, nrn_init, hoc_nrnpointerindex, 1);
 _mechtype = nrn_get_mechtype(_mechanism[1]);
 hoc_register_parm_default(_mechtype, &_parm_default);
         hoc_register_npy_direct(_mechtype, npy_direct_func_proc);
     _nrn_setdata_reg(_mechtype, _setdata);
 #if NMODL_TEXT
  register_nmodl_text_and_filename(_mechtype);
#endif
   _nrn_mechanism_register_data_fields(_mechtype,
                                       _nrn_mechanism_field<double>{"pcabar"} /* 0 */,
                                       _nrn_mechanism_field<double>{"ica"} /* 1 */,
                                       _nrn_mechanism_field<double>{"tau_m"} /* 2 */,
                                       _nrn_mechanism_field<double>{"m_inf"} /* 3 */,
                                       _nrn_mechanism_field<double>{"m"} /* 4 */,
                                       _nrn_mechanism_field<double>{"cai"} /* 5 */,
                                       _nrn_mechanism_field<double>{"cao"} /* 6 */,
                                       _nrn_mechanism_field<double>{"Dm"} /* 7 */,
                                       _nrn_mechanism_field<double>{"tadj"} /* 8 */,
                                       _nrn_mechanism_field<double>{"v"} /* 9 */,
                                       _nrn_mechanism_field<double>{"_g"} /* 10 */,
                                       _nrn_mechanism_field<double*>{"_ion_cai", "ca_ion"} /* 0 */,
                                       _nrn_mechanism_field<double*>{"_ion_cao", "ca_ion"} /* 1 */,
                                       _nrn_mechanism_field<double*>{"_ion_ica", "ca_ion"} /* 2 */,
                                       _nrn_mechanism_field<double*>{"_ion_dicadv", "ca_ion"} /* 3 */);
  hoc_register_prop_size(_mechtype, 11, 4);
  hoc_register_dparam_semantics(_mechtype, 0, "ca_ion");
  hoc_register_dparam_semantics(_mechtype, 1, "ca_ion");
  hoc_register_dparam_semantics(_mechtype, 2, "ca_ion");
  hoc_register_dparam_semantics(_mechtype, 3, "ca_ion");
 	hoc_register_cvode(_mechtype, _ode_count, 0, 0, 0);
 
    hoc_register_var(hoc_scdoub, hoc_vdoub, hoc_intfunc);
 	ivoc_help("help ?1 iCaL /Users/fionastern/Desktop/comp-neuro/mods/iCaL.mod\n");
 hoc_register_limits(_mechtype, _hoc_parm_limits);
 hoc_register_units(_mechtype, _hoc_parm_units);
 }
 static double FARADAY = 96480.0;
 static double R = 8.314;
static int _reset;
static const char *modelname = "high threshold calcium current (L-current)";

static int error;
static int _ninits = 0;
static int _match_recurse=1;
static void _modl_cleanup(){ _match_recurse=1;}
static int rates(_internalthreadargsprotocomma_ double);
static int states(_internalthreadargsproto_);
 
static int  states ( _internalthreadargsproto_ ) {
   rates ( _threadargscomma_ v ) ;
   m = m + ( 1.0 - exp ( - dt / tau_m ) ) * ( m_inf - m ) ;
    return 0; }
 
static void _hoc_states(void) {
  double _r;
 Datum* _ppvar; Datum* _thread; NrnThread* _nt;
 
  if(!_prop_id) {
    hoc_execerror("No data for states_iCaL. Requires prior call to setdata_iCaL and that the specified mechanism instance still be in existence.", NULL);
  }
  Prop* _local_prop = _extcall_prop;
  _nrn_mechanism_cache_instance _ml_real{_local_prop};
auto* const _ml = &_ml_real;
size_t const _iml{};
_ppvar = _local_prop ? _nrn_mechanism_access_dparam(_local_prop) : nullptr;
_thread = _extcall_thread.data();
double* _globals = nullptr;
if (gind != 0 && _thread != nullptr) { _globals = _thread[_gth].get<double*>(); }
_nt = nrn_threads;
 _r = 1.;
 states ( _threadargs_ );
 hoc_retpushx(_r);
}
 
static double _npy_states(Prop* _prop) {
    double _r{0.0};
 Datum* _ppvar; Datum* _thread; NrnThread* _nt;
 _nrn_mechanism_cache_instance _ml_real{_prop};
auto* const _ml = &_ml_real;
size_t const _iml{};
_ppvar = _nrn_mechanism_access_dparam(_prop);
_thread = _extcall_thread.data();
double* _globals = nullptr;
if (gind != 0 && _thread != nullptr) { _globals = _thread[_gth].get<double*>(); }
_nt = nrn_threads;
 _r = 1.;
 states ( _threadargs_ );
 return(_r);
}
 
double ghk ( _internalthreadargsprotocomma_ double _lv , double _lci , double _lco , double _lz ) {
   double _lghk;
 double _le , _lw ;
 _lw = _lv * ( .001 ) * _lz * FARADAY / ( R * ( celsius + 273.16 ) ) ;
   if ( fabs ( _lw ) > 1e-4 ) {
     _le = _lw / ( exp ( _lw ) - 1.0 ) ;
     }
   else {
     _le = 1.0 - _lw / 2.0 ;
     }
   _lghk = - ( .001 ) * _lz * FARADAY * ( _lco - _lci * exp ( _lw ) ) * _le ;
   
return _lghk;
 }
 
static void _hoc_ghk(void) {
  double _r;
 Datum* _ppvar; Datum* _thread; NrnThread* _nt;
 
  Prop* _local_prop = _prop_id ? _extcall_prop : nullptr;
  _nrn_mechanism_cache_instance _ml_real{_local_prop};
auto* const _ml = &_ml_real;
size_t const _iml{};
_ppvar = _local_prop ? _nrn_mechanism_access_dparam(_local_prop) : nullptr;
_thread = _extcall_thread.data();
double* _globals = nullptr;
if (gind != 0 && _thread != nullptr) { _globals = _thread[_gth].get<double*>(); }
_nt = nrn_threads;
 _r =  ghk ( _threadargscomma_ *getarg(1) , *getarg(2) , *getarg(3) , *getarg(4) );
 hoc_retpushx(_r);
}
 
static double _npy_ghk(Prop* _prop) {
    double _r{0.0};
 Datum* _ppvar; Datum* _thread; NrnThread* _nt;
 _nrn_mechanism_cache_instance _ml_real{_prop};
auto* const _ml = &_ml_real;
size_t const _iml{};
_ppvar = _nrn_mechanism_access_dparam(_prop);
_thread = _extcall_thread.data();
double* _globals = nullptr;
if (gind != 0 && _thread != nullptr) { _globals = _thread[_gth].get<double*>(); }
_nt = nrn_threads;
 _r =  ghk ( _threadargscomma_ *getarg(1) , *getarg(2) , *getarg(3) , *getarg(4) );
 return(_r);
}
 
static int  rates ( _internalthreadargsprotocomma_ double _lv ) {
   double _la , _lb ;
 _la = 1.6 / ( 1.0 + exp ( - 0.072 * ( _lv - 5.0 ) ) ) ;
   _lb = 0.02 * vtrap ( _threadargscomma_ - ( _lv - 1.31 ) , 5.36 ) ;
   tau_m = 1.0 / ( _la + _lb ) / tadj ;
   m_inf = 1.0 / ( 1.0 + exp ( ( _lv + 10.0 ) / - 10.0 ) ) ;
    return 0; }
 
static void _hoc_rates(void) {
  double _r;
 Datum* _ppvar; Datum* _thread; NrnThread* _nt;
 
  if(!_prop_id) {
    hoc_execerror("No data for rates_iCaL. Requires prior call to setdata_iCaL and that the specified mechanism instance still be in existence.", NULL);
  }
  Prop* _local_prop = _extcall_prop;
  _nrn_mechanism_cache_instance _ml_real{_local_prop};
auto* const _ml = &_ml_real;
size_t const _iml{};
_ppvar = _local_prop ? _nrn_mechanism_access_dparam(_local_prop) : nullptr;
_thread = _extcall_thread.data();
double* _globals = nullptr;
if (gind != 0 && _thread != nullptr) { _globals = _thread[_gth].get<double*>(); }
_nt = nrn_threads;
 _r = 1.;
 rates ( _threadargscomma_ *getarg(1) );
 hoc_retpushx(_r);
}
 
static double _npy_rates(Prop* _prop) {
    double _r{0.0};
 Datum* _ppvar; Datum* _thread; NrnThread* _nt;
 _nrn_mechanism_cache_instance _ml_real{_prop};
auto* const _ml = &_ml_real;
size_t const _iml{};
_ppvar = _nrn_mechanism_access_dparam(_prop);
_thread = _extcall_thread.data();
double* _globals = nullptr;
if (gind != 0 && _thread != nullptr) { _globals = _thread[_gth].get<double*>(); }
_nt = nrn_threads;
 _r = 1.;
 rates ( _threadargscomma_ *getarg(1) );
 return(_r);
}
 
double vtrap ( _internalthreadargsprotocomma_ double _lx , double _lc ) {
   double _lvtrap;
 if ( fabs ( _lx / _lc ) < 1e-6 ) {
     _lvtrap = _lc + _lx / 2.0 ;
     }
   else {
     _lvtrap = _lx / ( 1.0 - exp ( - _lx / _lc ) ) ;
     }
   
return _lvtrap;
 }
 
static void _hoc_vtrap(void) {
  double _r;
 Datum* _ppvar; Datum* _thread; NrnThread* _nt;
 
  Prop* _local_prop = _prop_id ? _extcall_prop : nullptr;
  _nrn_mechanism_cache_instance _ml_real{_local_prop};
auto* const _ml = &_ml_real;
size_t const _iml{};
_ppvar = _local_prop ? _nrn_mechanism_access_dparam(_local_prop) : nullptr;
_thread = _extcall_thread.data();
double* _globals = nullptr;
if (gind != 0 && _thread != nullptr) { _globals = _thread[_gth].get<double*>(); }
_nt = nrn_threads;
 _r =  vtrap ( _threadargscomma_ *getarg(1) , *getarg(2) );
 hoc_retpushx(_r);
}
 
static double _npy_vtrap(Prop* _prop) {
    double _r{0.0};
 Datum* _ppvar; Datum* _thread; NrnThread* _nt;
 _nrn_mechanism_cache_instance _ml_real{_prop};
auto* const _ml = &_ml_real;
size_t const _iml{};
_ppvar = _nrn_mechanism_access_dparam(_prop);
_thread = _extcall_thread.data();
double* _globals = nullptr;
if (gind != 0 && _thread != nullptr) { _globals = _thread[_gth].get<double*>(); }
_nt = nrn_threads;
 _r =  vtrap ( _threadargscomma_ *getarg(1) , *getarg(2) );
 return(_r);
}
 
static int _ode_count(int _type){ hoc_execerror("iCaL", "cannot be used with CVODE"); return 0;}

static void initmodel(_internalthreadargsproto_) {
  int _i; double _save;{
  m = m0;
 {
   tadj = pow( 3.0 , ( ( celsius - 23.5 ) / 10.0 ) ) ;
   rates ( _threadargscomma_ v ) ;
   m = m_inf ;
   }
 
}
}

static void nrn_init(_nrn_model_sorted_token const& _sorted_token, NrnThread* _nt, Memb_list* _ml_arg, int _type){
_nrn_mechanism_cache_range _lmr{_sorted_token, *_nt, *_ml_arg, _type};
auto* const _vec_v = _nt->node_voltage_storage();
auto* const _ml = &_lmr;
Datum* _ppvar; Datum* _thread;
Node *_nd; double _v; int* _ni; int _iml, _cntml;
_ni = _ml_arg->_nodeindices;
_cntml = _ml_arg->_nodecount;
_thread = _ml_arg->_thread;
double* _globals = nullptr;
if (gind != 0 && _thread != nullptr) { _globals = _thread[_gth].get<double*>(); }
for (_iml = 0; _iml < _cntml; ++_iml) {
 _ppvar = _ml_arg->_pdata[_iml];
   _v = _vec_v[_ni[_iml]];
 v = _v;
  cai = _ion_cai;
  cao = _ion_cao;
 initmodel(_threadargs_);
 }
}

static double _nrn_current(_internalthreadargsprotocomma_ double _v) {
double _current=0.; v=_v;
{ {
   ica = pcabar * m * m * ghk ( _threadargscomma_ v , cai , cao , 2.0 ) ;
   }
 _current += ica;

} return _current;
}

static void nrn_cur(_nrn_model_sorted_token const& _sorted_token, NrnThread* _nt, Memb_list* _ml_arg, int _type) {
_nrn_mechanism_cache_range _lmr{_sorted_token, *_nt, *_ml_arg, _type};
auto const _vec_rhs = _nt->node_rhs_storage();
auto const _vec_sav_rhs = _nt->node_sav_rhs_storage();
auto const _vec_v = _nt->node_voltage_storage();
auto* const _ml = &_lmr;
Datum* _ppvar; Datum* _thread;
Node *_nd; int* _ni; double _rhs, _v; int _iml, _cntml;
_ni = _ml_arg->_nodeindices;
_cntml = _ml_arg->_nodecount;
_thread = _ml_arg->_thread;
double* _globals = nullptr;
if (gind != 0 && _thread != nullptr) { _globals = _thread[_gth].get<double*>(); }
for (_iml = 0; _iml < _cntml; ++_iml) {
 _ppvar = _ml_arg->_pdata[_iml];
   _v = _vec_v[_ni[_iml]];
  cai = _ion_cai;
  cao = _ion_cao;
 auto const _g_local = _nrn_current(_threadargscomma_ _v + .001);
 	{ double _dica;
  _dica = ica;
 _rhs = _nrn_current(_threadargscomma_ _v);
  _ion_dicadv += (_dica - ica)/.001 ;
 	}
 _g = (_g_local - _rhs)/.001;
  _ion_ica += ica ;
	 _vec_rhs[_ni[_iml]] -= _rhs;
 
}
 
}

static void nrn_jacob(_nrn_model_sorted_token const& _sorted_token, NrnThread* _nt, Memb_list* _ml_arg, int _type) {
_nrn_mechanism_cache_range _lmr{_sorted_token, *_nt, *_ml_arg, _type};
auto const _vec_d = _nt->node_d_storage();
auto const _vec_sav_d = _nt->node_sav_d_storage();
auto* const _ml = &_lmr;
Datum* _ppvar; Datum* _thread;
Node *_nd; int* _ni; int _iml, _cntml;
_ni = _ml_arg->_nodeindices;
_cntml = _ml_arg->_nodecount;
_thread = _ml_arg->_thread;
double* _globals = nullptr;
if (gind != 0 && _thread != nullptr) { _globals = _thread[_gth].get<double*>(); }
for (_iml = 0; _iml < _cntml; ++_iml) {
  _vec_d[_ni[_iml]] += _g;
 
}
 
}

static void nrn_state(_nrn_model_sorted_token const& _sorted_token, NrnThread* _nt, Memb_list* _ml_arg, int _type) {
_nrn_mechanism_cache_range _lmr{_sorted_token, *_nt, *_ml_arg, _type};
auto* const _vec_v = _nt->node_voltage_storage();
auto* const _ml = &_lmr;
Datum* _ppvar; Datum* _thread;
Node *_nd; double _v = 0.0; int* _ni;
_ni = _ml_arg->_nodeindices;
size_t _cntml = _ml_arg->_nodecount;
_thread = _ml_arg->_thread;
double* _globals = nullptr;
if (gind != 0 && _thread != nullptr) { _globals = _thread[_gth].get<double*>(); }
for (size_t _iml = 0; _iml < _cntml; ++_iml) {
 _ppvar = _ml_arg->_pdata[_iml];
 _nd = _ml_arg->_nodelist[_iml];
   _v = _vec_v[_ni[_iml]];
 v=_v;
{
  cai = _ion_cai;
  cao = _ion_cao;
 {  { states(_threadargs_); }
  } }}

}

static void terminal(){}

static void _initlists(){
 int _i; static int _first = 1;
  if (!_first) return;
_first = 0;
}

#if NMODL_TEXT
static void register_nmodl_text_and_filename(int mech_type) {
    const char* nmodl_filename = "/Users/fionastern/Desktop/comp-neuro/mods/iCaL.mod";
    const char* nmodl_file_text = 
  "TITLE high threshold calcium current (L-current)\n"
  "\n"
  "COMMENT\n"
  "        *********************************************\n"
  "        reference:      McCormick & Huguenard (1992) \n"
  "			J.Neurophysiology 68(4), 1384-1400\n"
  "        found in:       hippocampal pyramidal cells\n"
  "        *********************************************\n"
  "	Assembled for MyFirstNEURON by Arthur Houweling\n"
  "	\n"
  "ENDCOMMENT\n"
  "\n"
  "\n"
  "\n"
  "\n"
  "INDEPENDENT {t FROM 0 TO 1 WITH 1 (ms)}\n"
  "\n"
  "\n"
  "NEURON {\n"
  "	SUFFIX iCaL\n"
  "	USEION ca READ cai,cao WRITE ica\n"
  "        RANGE pcabar, m_inf, tau_m, ica\n"
  "}\n"
  "\n"
  "UNITS {\n"
  "	(mA)	= (milliamp)\n"
  "	(mV)	= (millivolt)\n"
  "	(mM)	= (milli/liter)\n"
  "        FARADAY = 96480 (coul)\n"
  "        R       = 8.314 (volt-coul/degC)\n"
  "}\n"
  "\n"
  "PARAMETER {\n"
  "	v			(mV)\n"
  "	celsius			(degC)\n"
  "        dt              	(ms)\n"
  "	cai			(mM)\n"
  "	cao			(mM)\n"
  "	pcabar= 0.000276	(cm/s)		\n"
  "}\n"
  "\n"
  "STATE {\n"
  "	m\n"
  "}\n"
  "\n"
  "ASSIGNED {\n"
  "	ica		(mA/cm2)\n"
  "	tau_m		(ms)\n"
  "	m_inf \n"
  "	tadj\n"
  "}\n"
  "\n"
  "BREAKPOINT { \n"
  "	SOLVE states :METHOD euler\n"
  "	ica = pcabar * m*m * ghk(v,cai,cao,2)\n"
  "}\n"
  "\n"
  ":DERIVATIVE states {\n"
  ":       rates(v)\n"
  ":\n"
  ":       m'= (m_inf-m) / tau_m \n"
  ":}\n"
  "  \n"
  "PROCEDURE states() {\n"
  "        rates(v)\n"
  "	\n"
  "        m= m + (1-exp(-dt/tau_m))*(m_inf-m)\n"
  "	:printf(\"\\n iCaL tau_m=%g\", tau_m)\n"
  "}\n"
  "\n"
  "UNITSOFF\n"
  "\n"
  "INITIAL {\n"
  "	tadj = 3.0 ^ ((celsius-23.5)/10)\n"
  "	rates(v)\n"
  "	m = m_inf\n"
  "}\n"
  "\n"
  "FUNCTION ghk( v(mV), ci(mM), co(mM), z)  (millicoul/cm3) {\n"
  "        LOCAL e, w\n"
  "        w = v * (.001) * z*FARADAY / (R*(celsius+273.16))\n"
  "        \n"
  "	if (fabs(w)>1e-4) \n"
  "          { e = w / (exp(w)-1) }\n"
  "        else\n"
  "	: denominator is small -> Taylor series\n"
  "        { e = 1-w/2 }\n"
  "	\n"
  "        ghk = - (.001) * z*FARADAY * (co-ci*exp(w)) * e\n"
  "}\n"
  "UNITSOFF\n"
  "\n"
  "PROCEDURE rates(v(mV)) { LOCAL a,b\n"
  "	a = 1.6 / (1+ exp(-0.072*(v-5)))\n"
  "	b = 0.02 * vtrap( -(v-1.31), 5.36)\n"
  "\n"
  "	tau_m = 1/(a+b) / tadj\n"
  "	m_inf = 1/(1+exp((v+10)/-10))\n"
  "}\n"
  "\n"
  "FUNCTION vtrap(x,c) { \n"
  "	: Traps for 0 in denominator of rate equations\n"
  "        if (fabs(x/c) < 1e-6) {\n"
  "          vtrap = c + x/2 }\n"
  "        else {\n"
  "          vtrap = x / (1-exp(-x/c)) }\n"
  "}\n"
  "UNITSON\n"
  ;
    hoc_reg_nmodl_filename(mech_type, nmodl_filename);
    hoc_reg_nmodl_text(mech_type, nmodl_file_text);
}
#endif

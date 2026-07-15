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
static constexpr auto number_of_floating_point_variables = 8;
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
 
#define nrn_init _nrn_init__KDRI
#define _nrn_initial _nrn_initial__KDRI
#define nrn_cur _nrn_cur__KDRI
#define _nrn_current _nrn_current__KDRI
#define nrn_jacob _nrn_jacob__KDRI
#define nrn_state _nrn_state__KDRI
#define _net_receive _net_receive__KDRI 
#define _f_rate _f_rate__KDRI 
#define rate rate__KDRI 
#define states states__KDRI 
 
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
#define gkbar _ml->template fpfield<0>(_iml)
#define gkbar_columnindex 0
#define ik _ml->template fpfield<1>(_iml)
#define ik_columnindex 1
#define n _ml->template fpfield<2>(_iml)
#define n_columnindex 2
#define h _ml->template fpfield<3>(_iml)
#define h_columnindex 3
#define ek _ml->template fpfield<4>(_iml)
#define ek_columnindex 4
#define Dn _ml->template fpfield<5>(_iml)
#define Dn_columnindex 5
#define Dh _ml->template fpfield<6>(_iml)
#define Dh_columnindex 6
#define _g _ml->template fpfield<7>(_iml)
#define _g_columnindex 7
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
 static void _hoc_alp(void);
 static void _hoc_bet(void);
 static void _hoc_expM1(void);
 static void _hoc_rate(void);
 static void _hoc_states(void);
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
 {"setdata_KDRI", _hoc_setdata},
 {"alp_KDRI", _hoc_alp},
 {"bet_KDRI", _hoc_bet},
 {"expM1_KDRI", _hoc_expM1},
 {"rate_KDRI", _hoc_rate},
 {"states_KDRI", _hoc_states},
 {0, 0}
};
 
/* Direct Python call wrappers to density mechanism functions.*/
 static double _npy_alp(Prop*);
 static double _npy_bet(Prop*);
 static double _npy_expM1(Prop*);
 static double _npy_rate(Prop*);
 static double _npy_states(Prop*);
 
static NPyDirectMechFunc npy_direct_func_proc[] = {
 {"alp", _npy_alp},
 {"bet", _npy_bet},
 {"expM1", _npy_expM1},
 {"rate", _npy_rate},
 {"states", _npy_states},
 {0, 0}
};
#define alp alp_KDRI
#define bet bet_KDRI
#define expM1 expM1_KDRI
 extern double alp( double , double );
 extern double bet( double , double );
 extern double expM1( double , double );
 /* declare global and static user variables */
 #define gind 0
 #define _gth 0
#define inf inf_KDRI
 double inf[2];
#define usetable usetable_KDRI
 double usetable = 1;
 /* some parameters have upper and lower limits */
 static HocParmLimits _hoc_parm_limits[] = {
 {"gkbar_KDRI", 0, 1e+09},
 {"usetable_KDRI", 0, 1},
 {0, 0, 0}
};
 static HocParmUnits _hoc_parm_units[] = {
 {"gkbar_KDRI", "mho/cm2"},
 {"ik_KDRI", "mA/cm2"},
 {0, 0}
};
 static double delta_t = 0.01;
 static double h0 = 0;
 static double n0 = 0;
 static double v = 0;
 /* connect global user variables to hoc */
 static DoubScal hoc_scdoub[] = {
 {"usetable_KDRI", &usetable_KDRI},
 {0, 0}
};
 static DoubVec hoc_vdoub[] = {
 {"inf_KDRI", inf_KDRI, 2},
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
 /* connect range variables in _p that hoc is supposed to know about */
 static const char *_mechanism[] = {
 "7.7.0",
"KDRI",
 "gkbar_KDRI",
 0,
 "ik_KDRI",
 0,
 "n_KDRI",
 "h_KDRI",
 0,
 0};
 static Symbol* _k_sym;
 
 /* Used by NrnProperty */
 static _nrn_mechanism_std_vector<double> _parm_default{
     0, /* gkbar */
 }; 
 
 
extern Prop* need_memb(Symbol*);
static void nrn_alloc(Prop* _prop) {
  Prop *prop_ion{};
  Datum *_ppvar{};
   _ppvar = nrn_prop_datum_alloc(_mechtype, 3, _prop);
    _nrn_mechanism_access_dparam(_prop) = _ppvar;
     _nrn_mechanism_cache_instance _ml_real{_prop};
    auto* const _ml = &_ml_real;
    size_t const _iml{};
    assert(_nrn_mechanism_get_num_vars(_prop) == 8);
 	/*initialize range parameters*/
 	gkbar = _parm_default[0]; /* 0 */
 	 assert(_nrn_mechanism_get_num_vars(_prop) == 8);
 	_nrn_mechanism_access_dparam(_prop) = _ppvar;
 	/*connect ionic variables to this model*/
 prop_ion = need_memb(_k_sym);
 nrn_promote(prop_ion, 0, 1);
 	_ppvar[0] = _nrn_mechanism_get_param_handle(prop_ion, 0); /* ek */
 	_ppvar[1] = _nrn_mechanism_get_param_handle(prop_ion, 3); /* ik */
 	_ppvar[2] = _nrn_mechanism_get_param_handle(prop_ion, 4); /* _ion_dikdv */
 
}
 static void _initlists();
 extern Symbol* hoc_lookup(const char*);
extern void _nrn_thread_reg(int, int, void(*)(Datum*));
void _nrn_thread_table_reg(int, nrn_thread_table_check_t);
extern void hoc_register_tolerance(int, HocStateTolerance*, Symbol***);
extern void _cvode_abstol( Symbol**, double*, int);

 extern "C" void _KDRI_reg() {
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
                                       _nrn_mechanism_field<double>{"gkbar"} /* 0 */,
                                       _nrn_mechanism_field<double>{"ik"} /* 1 */,
                                       _nrn_mechanism_field<double>{"n"} /* 2 */,
                                       _nrn_mechanism_field<double>{"h"} /* 3 */,
                                       _nrn_mechanism_field<double>{"ek"} /* 4 */,
                                       _nrn_mechanism_field<double>{"Dn"} /* 5 */,
                                       _nrn_mechanism_field<double>{"Dh"} /* 6 */,
                                       _nrn_mechanism_field<double>{"_g"} /* 7 */,
                                       _nrn_mechanism_field<double*>{"_ion_ek", "k_ion"} /* 0 */,
                                       _nrn_mechanism_field<double*>{"_ion_ik", "k_ion"} /* 1 */,
                                       _nrn_mechanism_field<double*>{"_ion_dikdv", "k_ion"} /* 2 */);
  hoc_register_prop_size(_mechtype, 8, 3);
  hoc_register_dparam_semantics(_mechtype, 0, "k_ion");
  hoc_register_dparam_semantics(_mechtype, 1, "k_ion");
  hoc_register_dparam_semantics(_mechtype, 2, "k_ion");
 	hoc_register_cvode(_mechtype, _ode_count, 0, 0, 0);
 
    hoc_register_var(hoc_scdoub, hoc_vdoub, hoc_intfunc);
 	ivoc_help("help ?1 KDRI /Users/fionastern/Desktop/comp-neuro/mods/KDRI.mod\n");
 hoc_register_limits(_mechtype, _hoc_parm_limits);
 hoc_register_units(_mechtype, _hoc_parm_units);
 }
 static double _zfac [ 2 ] ;
 static double *_t_inf[2];
 static double *_t__zfac[2];
static int _reset;
static const char *modelname = "HH k channel channel";

static int error;
static int _ninits = 0;
static int _match_recurse=1;
static void _modl_cleanup(){ _match_recurse=1;}
static int _f_rate(double);
static int rate(double);
static int states();
 static void _n_rate(double);
 
static int  states (  ) {
   rate ( _threadargscomma_ v * 1.0 ) ;
   n = n + _zfac [ 0 ] * ( inf [ 0 ] - n ) ;
   h = h + _zfac [ 1 ] * ( inf [ 1 ] - h ) ;
   
/*VERBATIM*/
	return 0;
  return 0; }
 
static void _hoc_states(void) {
  double _r;
  
  if(!_prop_id) {
    hoc_execerror("No data for states_KDRI. Requires prior call to setdata_KDRI and that the specified mechanism instance still be in existence.", NULL);
  } else {
    _setdata(_extcall_prop);
  }
   _r = 1.;
 states (  );
 hoc_retpushx(_r);
}
 
static double _npy_states(Prop* _prop) {
    double _r{0.0};
    neuron::legacy::set_globals_from_prop(_prop, _ml_real, _ml, _iml);
  _ppvar = _nrn_mechanism_access_dparam(_prop);
 _r = 1.;
 states (  );
 return(_r);
}
 
double alp (  double _lv , double _li ) {
   double _lalp;
 double _la , _lb , _lc , _ltau_factor ;
 _lv = _lv ;
   _ltau_factor = 1.0 ;
   if ( _li  == 0.0 ) {
     _lalp = _ltau_factor * 1.0 * .035 * expM1 ( _threadargscomma_ - _lv - 15.0 , 9.0 ) ;
     }
   else if ( _li  == 1.0 ) {
     _lalp = _ltau_factor * 0.0083 * ( 1.0 / ( exp ( ( _lv + 20.0 ) / 10.0 ) + 1.0 ) + 1.0 ) ;
     }
   
return _lalp;
 }
 
static void _hoc_alp(void) {
  double _r;
    _r =  alp (  *getarg(1) , *getarg(2) );
 hoc_retpushx(_r);
}
 
static double _npy_alp(Prop* _prop) {
    double _r{0.0};
    neuron::legacy::set_globals_from_prop(_prop, _ml_real, _ml, _iml);
  _ppvar = _nrn_mechanism_access_dparam(_prop);
 _r =  alp (  *getarg(1) , *getarg(2) );
 return(_r);
}
 
double bet (  double _lv , double _li ) {
   double _lbet;
 double _la , _lb , _lc , _ltau_factor ;
 _lv = _lv ;
   _ltau_factor = 1.0 ;
   if ( _li  == 0.0 ) {
     _lbet = _ltau_factor * 1.0 * .014 * exp ( ( - _lv + 12.0 ) / 46.0 ) ;
     }
   else if ( _li  == 1.0 ) {
     _lbet = _ltau_factor * 0.0083 / ( exp ( ( - _lv - 20.0 ) / 10.0 ) + 1.0 ) ;
     }
   
return _lbet;
 }
 
static void _hoc_bet(void) {
  double _r;
    _r =  bet (  *getarg(1) , *getarg(2) );
 hoc_retpushx(_r);
}
 
static double _npy_bet(Prop* _prop) {
    double _r{0.0};
    neuron::legacy::set_globals_from_prop(_prop, _ml_real, _ml, _iml);
  _ppvar = _nrn_mechanism_access_dparam(_prop);
 _r =  bet (  *getarg(1) , *getarg(2) );
 return(_r);
}
 
double expM1 (  double _lx , double _ly ) {
   double _lexpM1;
 if ( fabs ( _lx / _ly ) < 1e-6 ) {
     _lexpM1 = _ly * ( 1.0 - _lx / _ly / 2.0 ) ;
     }
   else {
     _lexpM1 = _lx / ( exp ( _lx / _ly ) - 1.0 ) ;
     }
   
return _lexpM1;
 }
 
static void _hoc_expM1(void) {
  double _r;
    _r =  expM1 (  *getarg(1) , *getarg(2) );
 hoc_retpushx(_r);
}
 
static double _npy_expM1(Prop* _prop) {
    double _r{0.0};
    neuron::legacy::set_globals_from_prop(_prop, _ml_real, _ml, _iml);
  _ppvar = _nrn_mechanism_access_dparam(_prop);
 _r =  expM1 (  *getarg(1) , *getarg(2) );
 return(_r);
}
 static double _mfac_rate, _tmin_rate;
 static void _check_rate();
 static void _check_rate() {
  static int _maktable=1; int _i, _j, _ix = 0;
  double _xi, _tmax;
  static double _sav_dt;
  static double _sav_celsius;
  if (!usetable) {return;}
  if (_sav_dt != dt) { _maktable = 1;}
  if (_sav_celsius != celsius) { _maktable = 1;}
  if (_maktable) { double _x, _dx; _maktable=0;
   _tmin_rate =  - 160.0 ;
   _tmax =  100.0 ;
   _dx = (_tmax - _tmin_rate)/200.; _mfac_rate = 1./_dx;
   for (_i=0, _x=_tmin_rate; _i < 201; _x += _dx, _i++) {
    _f_rate(_x);
    for (_j = 0; _j < 2; _j++) { _t_inf[_j][_i] = inf[_j];
}    for (_j = 0; _j < 2; _j++) { _t__zfac[_j][_i] = _zfac[_j];
}   }
   _sav_dt = dt;
   _sav_celsius = celsius;
  }
 }

 static int rate(double _lv){ _check_rate();
 _n_rate(_lv);
 return 0;
 }

 static void _n_rate(double _lv){ int _i, _j;
 double _xi, _theta;
 if (!usetable) {
 _f_rate(_lv); return; 
}
 _xi = _mfac_rate * (_lv - _tmin_rate);
 if (std::isnan(_xi)) {
  for (_j = 0; _j < 2; _j++) { inf[_j] = _xi;
}  for (_j = 0; _j < 2; _j++) { _zfac[_j] = _xi;
}  return;
 }
 if (_xi <= 0.) {
 for (_j = 0; _j < 2; _j++) { inf[_j] = _t_inf[_j][0];
} for (_j = 0; _j < 2; _j++) { _zfac[_j] = _t__zfac[_j][0];
} return; }
 if (_xi >= 200.) {
 for (_j = 0; _j < 2; _j++) { inf[_j] = _t_inf[_j][200];
} for (_j = 0; _j < 2; _j++) { _zfac[_j] = _t__zfac[_j][200];
} return; }
 _i = (int) _xi;
 _theta = _xi - (double)_i;
 for (_j = 0; _j < 2; _j++) {double *_t = _t_inf[_j]; inf[_j] = _t[_i] + _theta*(_t[_i+1] - _t[_i]);}
 for (_j = 0; _j < 2; _j++) {double *_t = _t__zfac[_j]; _zfac[_j] = _t[_i] + _theta*(_t[_i+1] - _t[_i]);}
 }

 
static int  _f_rate (  double _lv ) {
   double _la , _lb , _ltau ;
 {int  _li ;for ( _li = 0 ; _li <= 1 ; _li ++ ) {
     _la = alp ( _threadargscomma_ _lv , ((double) _li ) ) ;
     _lb = bet ( _threadargscomma_ _lv , ((double) _li ) ) ;
     _ltau = 1.0 / ( _la + _lb ) ;
     inf [ _li ] = _la / ( _la + _lb ) ;
     _zfac [ _li ] = ( 1.0 - exp ( - dt / _ltau ) ) ;
     } }
    return 0; }
 
static void _hoc_rate(void) {
  double _r;
     _r = 1.;
 rate (  *getarg(1) );
 hoc_retpushx(_r);
}
 
static double _npy_rate(Prop* _prop) {
    double _r{0.0};
    neuron::legacy::set_globals_from_prop(_prop, _ml_real, _ml, _iml);
  _ppvar = _nrn_mechanism_access_dparam(_prop);
  _r = 1.;
 rate (  *getarg(1) );
 return(_r);
}
 
static int _ode_count(int _type){ hoc_execerror("KDRI", "cannot be used with CVODE"); return 0;}

static void initmodel() {
  int _i; double _save;_ninits++;
 _save = t;
 t = 0.0;
{
  h = h0;
  n = n0;
 {
   rate ( _threadargscomma_ v * 1.0 ) ;
   n = inf [ 0 ] ;
   h = inf [ 1 ] ;
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
   ik = gkbar * n * n * n * n * h * ( v - ek ) ;
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
  std_cerr_stream << "at line 44 in file KDRI.mod:\n	SOLVE states\n";
  std_cerr_stream << _ml << ' ' << _iml << '\n';
  abort_run(error);
}
 } }}

}

static void terminal(){}

static void _initlists() {
 int _i; static int _first = 1;
  if (!_first) return;
  for (_i=0; _i < 2; _i++) {  _t_inf[_i] = makevector(201*sizeof(double)); }
  for (_i=0; _i < 2; _i++) {  _t__zfac[_i] = makevector(201*sizeof(double)); }
_first = 0;
}

#if NMODL_TEXT
static void register_nmodl_text_and_filename(int mech_type) {
    const char* nmodl_filename = "/Users/fionastern/Desktop/comp-neuro/mods/KDRI.mod";
    const char* nmodl_file_text = 
  "TITLE HH k channel channel\n"
  ": Hodgkin - Huxley k channel\n"
  "\n"
  ": The model used in Melnick et al. 2003 \n"
  ": implemented tau_factor as local variable in FUNCTION and used it insted of Q10 (15thMay20, Kazutaka)\n"
  "\n"
  "NEURON {\n"
  "	SUFFIX KDRI\n"
  "	USEION k READ ek WRITE ik\n"
  "	RANGE gkbar, ik\n"
  "	GLOBAL inf\n"
  "}\n"
  "\n"
  "UNITS {\n"
  "	(mA) = (milliamp)\n"
  "	(mV) = (millivolt)\n"
  "}\n"
  "\n"
  "INDEPENDENT {t FROM 0 TO 1 WITH 1 (ms)}\n"
  "PARAMETER {\n"
  "	v (mV)\n"
  "	dt (ms)\n"
  "	gkbar=0 (mho/cm2) <0,1e9>\n"
  "	ek = -84 (mV)\n"
  "	celsius = 37 (degC)\n"
  "}\n"
  "STATE {\n"
  "	n h\n"
  "}\n"
  "ASSIGNED {\n"
  "	ik (mA/cm2)\n"
  "	inf[2]\n"
  "	\n"
  "}\n"
  "LOCAL	fac[2]\n"
  "\n"
  "INITIAL {\n"
  "	rate(v*1(/mV))\n"
  "	n = inf[0]\n"
  "	h = inf[1]\n"
  "}\n"
  "\n"
  "BREAKPOINT {\n"
  "	SOLVE states\n"
  "	ik = gkbar*n*n*n*n*h*(v - ek)\n"
  "}\n"
  "\n"
  "PROCEDURE states() {	: exact when v held constant\n"
  "	rate(v*1(/mV))\n"
  "	n = n + fac[0]*(inf[0] - n)\n"
  "	h = h + fac[1]*(inf[1] - h)\n"
  "	VERBATIM\n"
  "	return 0;\n"
  "	ENDVERBATIM\n"
  "}\n"
  "\n"
  "UNITSOFF\n"
  "FUNCTION alp(v(mV),i) { LOCAL a,b,c,tau_factor :rest=-70 order n,h\n"
  "	v = v\n"
  ":	q10 = 3^((celsius - 23)/10)\n"
  "	tau_factor = 1\n"
  "	if (i==0) {\n"
  "		alp = tau_factor * 1 * .035*expM1(-v - 15, 9)\n"
  "	}else if (i==1){\n"
  "		alp = tau_factor * 0.0083*(1/(exp((v + 20)/10)+1) + 1)\n"
  "	}\n"
  "}\n"
  "\n"
  "FUNCTION bet(v,i) { LOCAL a,b,c,tau_factor :rest=-70 order n,h\n"
  "	v = v\n"
  ":	q10 = 3^((celsius - 23)/10)\n"
  "	tau_factor = 1\n"
  "	if (i==0) {\n"
  "		bet = tau_factor * 1 * .014*exp((-v + 12)/46)\n"
  "	}else if (i==1){\n"
  "		bet = tau_factor * 0.0083 /(exp((-v - 20)/10)+1)\n"
  "	}\n"
  "}\n"
  "\n"
  "FUNCTION expM1(x,y) {\n"
  "        if (fabs(x/y) < 1e-6) {\n"
  "                expM1 = y*(1 - x/y/2)\n"
  "        }else{\n"
  "                expM1 = x/(exp(x/y) - 1)\n"
  "        }\n"
  "}\n"
  "\n"
  "\n"
  "PROCEDURE rate(v) {LOCAL a, b, tau :rest = -70\n"
  "	TABLE inf, fac DEPEND dt, celsius FROM -160 TO 100 WITH 200\n"
  "	FROM i=0 TO 1 {\n"
  "		a=alp(v,i)  b=bet(v,i)\n"
  "		tau = 1/(a+b)\n"
  "		inf[i] = a/(a+b)\n"
  "		fac[i] = (1 - exp(-dt/tau))\n"
  "	}\n"
  "}\n"
  "UNITSON\n"
  ;
    hoc_reg_nmodl_filename(mech_type, nmodl_filename);
    hoc_reg_nmodl_text(mech_type, nmodl_file_text);
}
#endif

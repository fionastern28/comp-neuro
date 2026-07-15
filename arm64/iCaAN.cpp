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
static constexpr auto number_of_datum_variables = 5;
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
 
#define nrn_init _nrn_init__iCaAN
#define _nrn_initial _nrn_initial__iCaAN
#define nrn_cur _nrn_cur__iCaAN
#define _nrn_current _nrn_current__iCaAN
#define nrn_jacob _nrn_jacob__iCaAN
#define nrn_state _nrn_state__iCaAN
#define _net_receive _net_receive__iCaAN 
#define evaluate_fct evaluate_fct__iCaAN 
#define states states__iCaAN 
 
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
#define gbar _ml->template fpfield<0>(_iml)
#define gbar_columnindex 0
#define ican _ml->template fpfield<1>(_iml)
#define ican_columnindex 1
#define m_inf _ml->template fpfield<2>(_iml)
#define m_inf_columnindex 2
#define tau_m _ml->template fpfield<3>(_iml)
#define tau_m_columnindex 3
#define m _ml->template fpfield<4>(_iml)
#define m_columnindex 4
#define ecan _ml->template fpfield<5>(_iml)
#define ecan_columnindex 5
#define cai _ml->template fpfield<6>(_iml)
#define cai_columnindex 6
#define Dm _ml->template fpfield<7>(_iml)
#define Dm_columnindex 7
#define tadj _ml->template fpfield<8>(_iml)
#define tadj_columnindex 8
#define v _ml->template fpfield<9>(_iml)
#define v_columnindex 9
#define _g _ml->template fpfield<10>(_iml)
#define _g_columnindex 10
#define _ion_ecan *(_ml->dptr_field<0>(_iml))
#define _p_ion_ecan static_cast<neuron::container::data_handle<double>>(_ppvar[0])
#define _ion_ican *(_ml->dptr_field<1>(_iml))
#define _p_ion_ican static_cast<neuron::container::data_handle<double>>(_ppvar[1])
#define _ion_dicandv *(_ml->dptr_field<2>(_iml))
#define _ion_cai *(_ml->dptr_field<3>(_iml))
#define _p_ion_cai static_cast<neuron::container::data_handle<double>>(_ppvar[3])
#define _ion_cao *(_ml->dptr_field<4>(_iml))
#define _p_ion_cao static_cast<neuron::container::data_handle<double>>(_ppvar[4])
 /* Thread safe. No static _ml, _iml or _ppvar. */
 static int hoc_nrnpointerindex =  -1;
 static _nrn_mechanism_std_vector<Datum> _extcall_thread;
 static Prop* _extcall_prop;
 /* _prop_id kind of shadows _extcall_prop to allow validity checking. */
 static _nrn_non_owning_id_without_container _prop_id{};
 /* external NEURON variables */
 extern double celsius;
 /* declaration of user functions */
 static void _hoc_evaluate_fct(void);
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
 {"setdata_iCaAN", _hoc_setdata},
 {"evaluate_fct_iCaAN", _hoc_evaluate_fct},
 {"states_iCaAN", _hoc_states},
 {0, 0}
};
 
/* Direct Python call wrappers to density mechanism functions.*/
 static double _npy_evaluate_fct(Prop*);
 static double _npy_states(Prop*);
 
static NPyDirectMechFunc npy_direct_func_proc[] = {
 {"evaluate_fct", _npy_evaluate_fct},
 {"states", _npy_states},
 {0, 0}
};
 /* declare global and static user variables */
 #define gind 0
 #define _gth 0
#define beta beta_iCaAN
 double beta = 0.002;
#define cac cac_iCaAN
 double cac = 0.0005;
#define tau_factor tau_factor_iCaAN
 double tau_factor = 40;
#define taumin taumin_iCaAN
 double taumin = 0.1;
 /* some parameters have upper and lower limits */
 static HocParmLimits _hoc_parm_limits[] = {
 {0, 0, 0}
};
 static HocParmUnits _hoc_parm_units[] = {
 {"beta_iCaAN", "1/ms"},
 {"cac_iCaAN", "mM"},
 {"taumin_iCaAN", "ms"},
 {"gbar_iCaAN", "mho/cm2"},
 {"ican_iCaAN", "mA/cm2"},
 {"tau_m_iCaAN", "ms"},
 {0, 0}
};
 static double delta_t = 0.01;
 static double m0 = 0;
 /* connect global user variables to hoc */
 static DoubScal hoc_scdoub[] = {
 {"beta_iCaAN", &beta_iCaAN},
 {"tau_factor_iCaAN", &tau_factor_iCaAN},
 {"cac_iCaAN", &cac_iCaAN},
 {"taumin_iCaAN", &taumin_iCaAN},
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
"iCaAN",
 "gbar_iCaAN",
 0,
 "ican_iCaAN",
 "m_inf_iCaAN",
 "tau_m_iCaAN",
 0,
 "m_iCaAN",
 0,
 0};
 static Symbol* _can_sym;
 static Symbol* _ca_sym;
 
 /* Used by NrnProperty */
 static _nrn_mechanism_std_vector<double> _parm_default{
     0.00025, /* gbar */
 }; 
 
 
extern Prop* need_memb(Symbol*);
static void nrn_alloc(Prop* _prop) {
  Prop *prop_ion{};
  Datum *_ppvar{};
   _ppvar = nrn_prop_datum_alloc(_mechtype, 5, _prop);
    _nrn_mechanism_access_dparam(_prop) = _ppvar;
     _nrn_mechanism_cache_instance _ml_real{_prop};
    auto* const _ml = &_ml_real;
    size_t const _iml{};
    assert(_nrn_mechanism_get_num_vars(_prop) == 11);
 	/*initialize range parameters*/
 	gbar = _parm_default[0]; /* 0.00025 */
 	 assert(_nrn_mechanism_get_num_vars(_prop) == 11);
 	_nrn_mechanism_access_dparam(_prop) = _ppvar;
 	/*connect ionic variables to this model*/
 prop_ion = need_memb(_can_sym);
 nrn_promote(prop_ion, 0, 1);
 	_ppvar[0] = _nrn_mechanism_get_param_handle(prop_ion, 0); /* ecan */
 	_ppvar[1] = _nrn_mechanism_get_param_handle(prop_ion, 3); /* ican */
 	_ppvar[2] = _nrn_mechanism_get_param_handle(prop_ion, 4); /* _ion_dicandv */
 prop_ion = need_memb(_ca_sym);
 nrn_promote(prop_ion, 1, 0);
 	_ppvar[3] = _nrn_mechanism_get_param_handle(prop_ion, 1); /* cai */
 	_ppvar[4] = _nrn_mechanism_get_param_handle(prop_ion, 2); /* cao */
 
}
 static void _initlists();
 extern Symbol* hoc_lookup(const char*);
extern void _nrn_thread_reg(int, int, void(*)(Datum*));
void _nrn_thread_table_reg(int, nrn_thread_table_check_t);
extern void hoc_register_tolerance(int, HocStateTolerance*, Symbol***);
extern void _cvode_abstol( Symbol**, double*, int);

 extern "C" void _iCaAN_reg() {
	int _vectorized = 1;
  _initlists();
 	ion_reg("can", 1.0);
 	ion_reg("ca", -10000.);
 	_can_sym = hoc_lookup("can_ion");
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
                                       _nrn_mechanism_field<double>{"gbar"} /* 0 */,
                                       _nrn_mechanism_field<double>{"ican"} /* 1 */,
                                       _nrn_mechanism_field<double>{"m_inf"} /* 2 */,
                                       _nrn_mechanism_field<double>{"tau_m"} /* 3 */,
                                       _nrn_mechanism_field<double>{"m"} /* 4 */,
                                       _nrn_mechanism_field<double>{"ecan"} /* 5 */,
                                       _nrn_mechanism_field<double>{"cai"} /* 6 */,
                                       _nrn_mechanism_field<double>{"Dm"} /* 7 */,
                                       _nrn_mechanism_field<double>{"tadj"} /* 8 */,
                                       _nrn_mechanism_field<double>{"v"} /* 9 */,
                                       _nrn_mechanism_field<double>{"_g"} /* 10 */,
                                       _nrn_mechanism_field<double*>{"_ion_ecan", "can_ion"} /* 0 */,
                                       _nrn_mechanism_field<double*>{"_ion_ican", "can_ion"} /* 1 */,
                                       _nrn_mechanism_field<double*>{"_ion_dicandv", "can_ion"} /* 2 */,
                                       _nrn_mechanism_field<double*>{"_ion_cai", "ca_ion"} /* 3 */,
                                       _nrn_mechanism_field<double*>{"_ion_cao", "ca_ion"} /* 4 */);
  hoc_register_prop_size(_mechtype, 11, 5);
  hoc_register_dparam_semantics(_mechtype, 0, "can_ion");
  hoc_register_dparam_semantics(_mechtype, 1, "can_ion");
  hoc_register_dparam_semantics(_mechtype, 2, "can_ion");
  hoc_register_dparam_semantics(_mechtype, 3, "ca_ion");
  hoc_register_dparam_semantics(_mechtype, 4, "ca_ion");
 	hoc_register_cvode(_mechtype, _ode_count, 0, 0, 0);
 
    hoc_register_var(hoc_scdoub, hoc_vdoub, hoc_intfunc);
 	ivoc_help("help ?1 iCaAN /Users/fionastern/Desktop/comp-neuro/mods/iCaAN.mod\n");
 hoc_register_limits(_mechtype, _hoc_parm_limits);
 hoc_register_units(_mechtype, _hoc_parm_units);
 }
static int _reset;
static const char *modelname = "Slow Ca-dependent cation current";

static int error;
static int _ninits = 0;
static int _match_recurse=1;
static void _modl_cleanup(){ _match_recurse=1;}
static int evaluate_fct(_internalthreadargsprotocomma_ double, double);
static int states(_internalthreadargsproto_);
 
static int  states ( _internalthreadargsproto_ ) {
   evaluate_fct ( _threadargscomma_ v , cai ) ;
   m = m + ( 1.0 - exp ( - dt / tau_m ) ) * ( m_inf - m ) ;
    return 0; }
 
static void _hoc_states(void) {
  double _r;
 Datum* _ppvar; Datum* _thread; NrnThread* _nt;
 
  if(!_prop_id) {
    hoc_execerror("No data for states_iCaAN. Requires prior call to setdata_iCaAN and that the specified mechanism instance still be in existence.", NULL);
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
 
static int  evaluate_fct ( _internalthreadargsprotocomma_ double _lv , double _lcai ) {
   double _lalpha2 ;
 _lalpha2 = beta * pow( ( _lcai / cac ) , 2.0 ) ;
   tau_m = tau_factor / ( _lalpha2 + beta ) / tadj ;
   m_inf = _lalpha2 / ( _lalpha2 + beta ) ;
   if ( tau_m < taumin ) {
     tau_m = taumin ;
     }
    return 0; }
 
static void _hoc_evaluate_fct(void) {
  double _r;
 Datum* _ppvar; Datum* _thread; NrnThread* _nt;
 
  if(!_prop_id) {
    hoc_execerror("No data for evaluate_fct_iCaAN. Requires prior call to setdata_iCaAN and that the specified mechanism instance still be in existence.", NULL);
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
 evaluate_fct ( _threadargscomma_ *getarg(1) , *getarg(2) );
 hoc_retpushx(_r);
}
 
static double _npy_evaluate_fct(Prop* _prop) {
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
 evaluate_fct ( _threadargscomma_ *getarg(1) , *getarg(2) );
 return(_r);
}
 
static int _ode_count(int _type){ hoc_execerror("iCaAN", "cannot be used with CVODE"); return 0;}

static void initmodel(_internalthreadargsproto_) {
  int _i; double _save;{
  m = m0;
 {
   tadj = pow( 3.0 , ( ( celsius - 22.0 ) / 10.0 ) ) ;
   evaluate_fct ( _threadargscomma_ v , cai ) ;
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
  ecan = _ion_ecan;
  cai = _ion_cai;
 initmodel(_threadargs_);
 }
}

static double _nrn_current(_internalthreadargsprotocomma_ double _v) {
double _current=0.; v=_v;
{ {
   ican = gbar * m * m * ( v - ecan ) ;
   }
 _current += ican;

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
  ecan = _ion_ecan;
  cai = _ion_cai;
 auto const _g_local = _nrn_current(_threadargscomma_ _v + .001);
 	{ double _dican;
  _dican = ican;
 _rhs = _nrn_current(_threadargscomma_ _v);
  _ion_dicandv += (_dican - ican)/.001 ;
 	}
 _g = (_g_local - _rhs)/.001;
  _ion_ican += ican ;
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
  ecan = _ion_ecan;
  cai = _ion_cai;
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
    const char* nmodl_filename = "/Users/fionastern/Desktop/comp-neuro/mods/iCaAN.mod";
    const char* nmodl_file_text = 
  "TITLE Slow Ca-dependent cation current\n"
  ":\n"
  ":   Ca++ dependent nonspecific cation current ICAN\n"
  ":   Differential equations\n"
  ":\n"
  ":   Model based on a first order kinetic scheme\n"
  ":\n"
  ":      <closed> + n cai <-> <open>	(alpha,beta)\n"
  ":\n"
  ":   Following this model, the activation fct will be half-activated at \n"
  ":   a concentration of Cai = (beta/alpha)^(1/n) = cac (parameter)\n"
  ":\n"
  ":   The mod file is here written for the case n=2 (2 binding sites)\n"
  ":   ---------------------------------------------\n"
  ":\n"
  ":   Kinetics based on: Partridge & Swandulla, TINS 11: 69-72, 1988.\n"
  ":\n"
  ":   This current has the following properties:\n"
  ":      - inward current (non specific for cations Na, K, Ca, ...)\n"
  ":      - activated by intracellular calcium\n"
  ":      - NOT voltage dependent\n"
  ":\n"
  ":   A minimal value for the time constant has been added\n"
  ":\n"
  ":   Ref: Destexhe et al., J. Neurophysiology 72: 803-818, 1994.\n"
  ":\n"
  ":   Modifications by Arthur Houweling for use in MyFirstNEURON\n"
  ":\n"
  ":   Some parameter changes by Paulo Aguiar (pauloaguiar@fc.up.pt):\n"
  ":   tau_factor = 40 => parameter beta	changes from 2.0e-3 to 5.0e-5\n"
  ":		cac = 0.5e-3 (before was 1.0e-3)\n"
  "\n"
  "INDEPENDENT {t FROM 0 TO 1 WITH 1 (ms)}\n"
  "\n"
  "NEURON {\n"
  "	SUFFIX iCaAN\n"
  "	USEION can READ ecan WRITE ican VALENCE 1\n"
  "	USEION ca READ cai\n"
  "        RANGE gbar, m_inf, tau_m\n"
  "	RANGE ican\n"
  "	GLOBAL beta, cac, taumin\n"
  "}\n"
  "\n"
  "\n"
  "UNITS {\n"
  "	(mA) = (milliamp)\n"
  "	(mV) = (millivolt)\n"
  "	(molar) = (1/liter)\n"
  "	(mM) = (millimolar)\n"
  "}\n"
  "\n"
  "\n"
  "PARAMETER {\n"
  "	v		  (mV)\n"
  "	celsius		  (degC)\n"
  "        dt                (ms)\n"
  "	ecan	= -20	  (mV)		: reversal potential\n"
  "	cai		  (mM)\n"
  "	gbar	= 0.00025 (mho/cm2)\n"
  "	beta	= 2.0e-3  (1/ms)	: backward rate constant (original value)\n"
  "\n"
  "	tau_factor = 40         : scaling factor allowing tuning\n"
  "\n"
  "	:cac	= 1.0e-3  (mM)		: middle point of activation fct  (original value)\n"
  "	cac		= 5e-4	  (mM)		: middle point of activation fct\n"
  "\n"
  "	taumin	= 0.1	  (ms)		: minimal value of time constant\n"
  "	\n"
  "	:parameter tau_factor and cac were set to produce tau_m ~ 2000(ms) at cai=cac and celsius=36;\n"
  "	:implications of parameters change when cai=cac:\n"
  "	: -> BEFORE (beta=2.0e-3;tadj=4.66) => tau_m ~ 50   ms\n"
  "	: -> AFTER  (beta=5.0e-5;tadj=4.66) => tau_m ~ 2000 ms\n"
  "	:\n"
  "	:also cac was reduced to half, from 1.0 uM to 0.5 uM\n"
  "}\n"
  "\n"
  "\n"
  "STATE {\n"
  "	m\n"
  "}\n"
  "\n"
  "ASSIGNED {\n"
  "	ican	(mA/cm2)\n"
  "	m_inf\n"
  "	tau_m	(ms)\n"
  "	tadj\n"
  "}\n"
  "\n"
  "BREAKPOINT { \n"
  "	SOLVE states :METHOD euler\n"
  "	ican = gbar * m*m * (v - ecan)\n"
  "}\n"
  "\n"
  ":DERIVATIVE states {\n"
  ":       evaluate_fct(v,cai)\n"
  ":\n"
  ":       m'= (m_inf-m) / tau_m \n"
  ":}\n"
  "  \n"
  "PROCEDURE states() {\n"
  "        evaluate_fct(v,cai)\n"
  "	\n"
  "        m = m + ( 1-exp(-dt/tau_m) )*(m_inf-m)\n"
  "	:printf(\"\\n iCAN tau_m=%g\", tau_m)\n"
  "\n"
  "}\n"
  "\n"
  "UNITSOFF\n"
  "INITIAL {\n"
  ":\n"
  ":  activation kinetics are assumed to be at 22 deg. C\n"
  ":  Q10 is assumed to be 3\n"
  ":\n"
  "	tadj = 3.0 ^ ((celsius-22.0)/10)\n"
  "\n"
  "	evaluate_fct(v,cai)\n"
  "	m = m_inf\n"
  "}\n"
  "\n"
  "\n"
  "PROCEDURE evaluate_fct(v(mV),cai(mM)) {  LOCAL alpha2\n"
  "\n"
  "	alpha2 = beta * (cai/cac)^2\n"
  "	\n"
  "	tau_m = tau_factor / (alpha2 + beta) / tadj		: tau_m = tau_factor / ( beta * (1 + (cai/cac)^2) ) / tadj\n"
  "	\n"
  "	m_inf = alpha2 / (alpha2 + beta)							: m_inf = (cai/cac)^2 / ( 1 + (cai/cac)^2 )\n"
  "\n"
  "	if(tau_m < taumin) { tau_m = taumin }					: min value of time cst\n"
  "\n"
  "}\n"
  "UNITSON\n"
  ;
    hoc_reg_nmodl_filename(mech_type, nmodl_filename);
    hoc_reg_nmodl_text(mech_type, nmodl_file_text);
}
#endif

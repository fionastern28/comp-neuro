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
static constexpr auto number_of_datum_variables = 6;
static constexpr auto number_of_floating_point_variables = 20;
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
 
#define nrn_init _nrn_init__NMDA_DynSyn
#define _nrn_initial _nrn_initial__NMDA_DynSyn
#define nrn_cur _nrn_cur__NMDA_DynSyn
#define _nrn_current _nrn_current__NMDA_DynSyn
#define nrn_jacob _nrn_jacob__NMDA_DynSyn
#define nrn_state _nrn_state__NMDA_DynSyn
#define _net_receive _net_receive__NMDA_DynSyn 
#define state state__NMDA_DynSyn 
 
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
#define tau_rise _ml->template fpfield<0>(_iml)
#define tau_rise_columnindex 0
#define tau_decay _ml->template fpfield<1>(_iml)
#define tau_decay_columnindex 1
#define U1 _ml->template fpfield<2>(_iml)
#define U1_columnindex 2
#define tau_rec _ml->template fpfield<3>(_iml)
#define tau_rec_columnindex 3
#define tau_fac _ml->template fpfield<4>(_iml)
#define tau_fac_columnindex 4
#define e _ml->template fpfield<5>(_iml)
#define e_columnindex 5
#define ca_ratio _ml->template fpfield<6>(_iml)
#define ca_ratio_columnindex 6
#define i _ml->template fpfield<7>(_iml)
#define i_columnindex 7
#define g _ml->template fpfield<8>(_iml)
#define g_columnindex 8
#define ica _ml->template fpfield<9>(_iml)
#define ica_columnindex 9
#define inon _ml->template fpfield<10>(_iml)
#define inon_columnindex 10
#define A _ml->template fpfield<11>(_iml)
#define A_columnindex 11
#define B _ml->template fpfield<12>(_iml)
#define B_columnindex 12
#define mgo _ml->template fpfield<13>(_iml)
#define mgo_columnindex 13
#define factor _ml->template fpfield<14>(_iml)
#define factor_columnindex 14
#define DA _ml->template fpfield<15>(_iml)
#define DA_columnindex 15
#define DB _ml->template fpfield<16>(_iml)
#define DB_columnindex 16
#define v _ml->template fpfield<17>(_iml)
#define v_columnindex 17
#define _g _ml->template fpfield<18>(_iml)
#define _g_columnindex 18
#define _tsav _ml->template fpfield<19>(_iml)
#define _tsav_columnindex 19
#define _nd_area *_ml->dptr_field<0>(_iml)
#define _ion_ica *(_ml->dptr_field<2>(_iml))
#define _p_ion_ica static_cast<neuron::container::data_handle<double>>(_ppvar[2])
#define _ion_dicadv *(_ml->dptr_field<3>(_iml))
#define _ion_mgo *(_ml->dptr_field<4>(_iml))
#define _p_ion_mgo static_cast<neuron::container::data_handle<double>>(_ppvar[4])
#define _ion_mgi *(_ml->dptr_field<5>(_iml))
#define _p_ion_mgi static_cast<neuron::container::data_handle<double>>(_ppvar[5])
 /* Thread safe. No static _ml, _iml or _ppvar. */
 static int hoc_nrnpointerindex =  -1;
 static _nrn_mechanism_std_vector<Datum> _extcall_thread;
 /* external NEURON variables */
 /* declaration of user functions */
 static double _hoc_mgblock(void*);
 static int _mechtype;
extern void _nrn_cacheloop_reg(int, int);
extern void hoc_register_limits(int, HocParmLimits*);
extern void hoc_register_units(int, HocParmUnits*);
extern void nrn_promote(Prop*, int, int);
 
#define NMODL_TEXT 1
#if NMODL_TEXT
static void register_nmodl_text_and_filename(int mechtype);
#endif
 extern Prop* nrn_point_prop_;
 static int _pointtype;
 static void* _hoc_create_pnt(Object* _ho) { void* create_point_process(int, Object*);
 return create_point_process(_pointtype, _ho);
}
 static void _hoc_destroy_pnt(void*);
 static double _hoc_loc_pnt(void* _vptr) {double loc_point_process(int, void*);
 return loc_point_process(_pointtype, _vptr);
}
 static double _hoc_has_loc(void* _vptr) {double has_loc_point(void*);
 return has_loc_point(_vptr);
}
 static double _hoc_get_loc_pnt(void* _vptr) {
 double get_loc_point_process(void*); return (get_loc_point_process(_vptr));
}
 static void _hoc_setdata(void*);
 /* connect user functions to hoc names */
 static VoidFunc hoc_intfunc[] = {
 {0, 0}
};
 static Member_func _member_func[] = {
 {"loc", _hoc_loc_pnt},
 {"has_loc", _hoc_has_loc},
 {"get_loc", _hoc_get_loc_pnt},
 {"mgblock", _hoc_mgblock},
 {0, 0}
};
#define mgblock mgblock_NMDA_DynSyn
 extern double mgblock( _internalthreadargsprotocomma_ double );
 /* declare global and static user variables */
 #define gind 0
 #define _gth 0
 /* some parameters have upper and lower limits */
 static HocParmLimits _hoc_parm_limits[] = {
 {0, 0, 0}
};
 static HocParmUnits _hoc_parm_units[] = {
 {"tau_rise", "ms"},
 {"tau_decay", "ms"},
 {"U1", "1"},
 {"tau_rec", "ms"},
 {"tau_fac", "ms"},
 {"e", "mV"},
 {"ca_ratio", "1"},
 {"i", "nA"},
 {"g", "umho"},
 {"ica", "nA"},
 {"inon", "nA"},
 {0, 0}
};
 static double A0 = 0;
 static double B0 = 0;
 static double delta_t = 0.01;
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
 }
 static void _hoc_setdata(void* _vptr) { Prop* _prop;
 _prop = ((Point_process*)_vptr)->_prop;
   _setdata(_prop);
 }
 static void nrn_alloc(Prop*);
static void nrn_init(_nrn_model_sorted_token const&, NrnThread*, Memb_list*, int);
static void nrn_state(_nrn_model_sorted_token const&, NrnThread*, Memb_list*, int);
 static void nrn_cur(_nrn_model_sorted_token const&, NrnThread*, Memb_list*, int);
static void nrn_jacob(_nrn_model_sorted_token const&, NrnThread*, Memb_list*, int);
 static void _hoc_destroy_pnt(void* _vptr) {
   destroy_point_process(_vptr);
}
 
static int _ode_count(int);
static void _ode_map(Prop*, int, neuron::container::data_handle<double>*, neuron::container::data_handle<double>*, double*, int);
static void _ode_spec(_nrn_model_sorted_token const&, NrnThread*, Memb_list*, int);
static void _ode_matsol(_nrn_model_sorted_token const&, NrnThread*, Memb_list*, int);
 
#define _cvode_ieq _ppvar[6].literal_value<int>()
 static void _ode_matsol_instance1(_internalthreadargsproto_);
 /* connect range variables in _p that hoc is supposed to know about */
 static const char *_mechanism[] = {
 "7.7.0",
"NMDA_DynSyn",
 "tau_rise",
 "tau_decay",
 "U1",
 "tau_rec",
 "tau_fac",
 "e",
 "ca_ratio",
 0,
 "i",
 "g",
 "ica",
 "inon",
 0,
 "A",
 "B",
 0,
 0};
 static Symbol* _ca_sym;
 static Symbol* _mg_sym;
 
 /* Used by NrnProperty */
 static _nrn_mechanism_std_vector<double> _parm_default{
     5, /* tau_rise */
     70, /* tau_decay */
     1, /* U1 */
     0.1, /* tau_rec */
     0.1, /* tau_fac */
     0, /* e */
     0.1, /* ca_ratio */
 }; 
 
 
extern Prop* need_memb(Symbol*);
static void nrn_alloc(Prop* _prop) {
  Prop *prop_ion{};
  Datum *_ppvar{};
  if (nrn_point_prop_) {
    _nrn_mechanism_access_alloc_seq(_prop) = _nrn_mechanism_access_alloc_seq(nrn_point_prop_);
    _ppvar = _nrn_mechanism_access_dparam(nrn_point_prop_);
  } else {
   _ppvar = nrn_prop_datum_alloc(_mechtype, 7, _prop);
    _nrn_mechanism_access_dparam(_prop) = _ppvar;
     _nrn_mechanism_cache_instance _ml_real{_prop};
    auto* const _ml = &_ml_real;
    size_t const _iml{};
    assert(_nrn_mechanism_get_num_vars(_prop) == 20);
 	/*initialize range parameters*/
 	tau_rise = _parm_default[0]; /* 5 */
 	tau_decay = _parm_default[1]; /* 70 */
 	U1 = _parm_default[2]; /* 1 */
 	tau_rec = _parm_default[3]; /* 0.1 */
 	tau_fac = _parm_default[4]; /* 0.1 */
 	e = _parm_default[5]; /* 0 */
 	ca_ratio = _parm_default[6]; /* 0.1 */
  }
 	 assert(_nrn_mechanism_get_num_vars(_prop) == 20);
 	_nrn_mechanism_access_dparam(_prop) = _ppvar;
 	/*connect ionic variables to this model*/
 prop_ion = need_memb(_ca_sym);
 	_ppvar[2] = _nrn_mechanism_get_param_handle(prop_ion, 3); /* ica */
 	_ppvar[3] = _nrn_mechanism_get_param_handle(prop_ion, 4); /* _ion_dicadv */
 prop_ion = need_memb(_mg_sym);
 nrn_promote(prop_ion, 1, 0);
 	_ppvar[4] = _nrn_mechanism_get_param_handle(prop_ion, 2); /* mgo */
 	_ppvar[5] = _nrn_mechanism_get_param_handle(prop_ion, 1); /* mgi */
 
}
 static void _initlists();
  /* some states have an absolute tolerance */
 static Symbol** _atollist;
 static HocStateTolerance _hoc_state_tol[] = {
 {0, 0}
};
 static void _net_receive(Point_process*, double*, double);
 static void _net_init(Point_process*, double*, double);
 extern Symbol* hoc_lookup(const char*);
extern void _nrn_thread_reg(int, int, void(*)(Datum*));
void _nrn_thread_table_reg(int, nrn_thread_table_check_t);
extern void hoc_register_tolerance(int, HocStateTolerance*, Symbol***);
extern void _cvode_abstol( Symbol**, double*, int);

 extern "C" void _NMDA_DynSyn_reg() {
	int _vectorized = 1;
  _initlists();
 	ion_reg("ca", -10000.);
 	ion_reg("mg", 2.0);
 	_ca_sym = hoc_lookup("ca_ion");
 	_mg_sym = hoc_lookup("mg_ion");
 	_pointtype = point_register_mech(_mechanism,
	 nrn_alloc,nrn_cur, nrn_jacob, nrn_state, nrn_init,
	 hoc_nrnpointerindex, 1,
	 _hoc_create_pnt, _hoc_destroy_pnt, _member_func);
 _mechtype = nrn_get_mechtype(_mechanism[1]);
 hoc_register_parm_default(_mechtype, &_parm_default);
     _nrn_setdata_reg(_mechtype, _setdata);
 #if NMODL_TEXT
  register_nmodl_text_and_filename(_mechtype);
#endif
   _nrn_mechanism_register_data_fields(_mechtype,
                                       _nrn_mechanism_field<double>{"tau_rise"} /* 0 */,
                                       _nrn_mechanism_field<double>{"tau_decay"} /* 1 */,
                                       _nrn_mechanism_field<double>{"U1"} /* 2 */,
                                       _nrn_mechanism_field<double>{"tau_rec"} /* 3 */,
                                       _nrn_mechanism_field<double>{"tau_fac"} /* 4 */,
                                       _nrn_mechanism_field<double>{"e"} /* 5 */,
                                       _nrn_mechanism_field<double>{"ca_ratio"} /* 6 */,
                                       _nrn_mechanism_field<double>{"i"} /* 7 */,
                                       _nrn_mechanism_field<double>{"g"} /* 8 */,
                                       _nrn_mechanism_field<double>{"ica"} /* 9 */,
                                       _nrn_mechanism_field<double>{"inon"} /* 10 */,
                                       _nrn_mechanism_field<double>{"A"} /* 11 */,
                                       _nrn_mechanism_field<double>{"B"} /* 12 */,
                                       _nrn_mechanism_field<double>{"mgo"} /* 13 */,
                                       _nrn_mechanism_field<double>{"factor"} /* 14 */,
                                       _nrn_mechanism_field<double>{"DA"} /* 15 */,
                                       _nrn_mechanism_field<double>{"DB"} /* 16 */,
                                       _nrn_mechanism_field<double>{"v"} /* 17 */,
                                       _nrn_mechanism_field<double>{"_g"} /* 18 */,
                                       _nrn_mechanism_field<double>{"_tsav"} /* 19 */,
                                       _nrn_mechanism_field<double*>{"_nd_area", "area"} /* 0 */,
                                       _nrn_mechanism_field<Point_process*>{"_pntproc", "pntproc"} /* 1 */,
                                       _nrn_mechanism_field<double*>{"_ion_ica", "ca_ion"} /* 2 */,
                                       _nrn_mechanism_field<double*>{"_ion_dicadv", "ca_ion"} /* 3 */,
                                       _nrn_mechanism_field<double*>{"_ion_mgo", "mg_ion"} /* 4 */,
                                       _nrn_mechanism_field<double*>{"_ion_mgi", "mg_ion"} /* 5 */,
                                       _nrn_mechanism_field<int>{"_cvode_ieq", "cvodeieq"} /* 6 */);
  hoc_register_prop_size(_mechtype, 20, 7);
  hoc_register_dparam_semantics(_mechtype, 0, "area");
  hoc_register_dparam_semantics(_mechtype, 1, "pntproc");
  hoc_register_dparam_semantics(_mechtype, 2, "ca_ion");
  hoc_register_dparam_semantics(_mechtype, 3, "ca_ion");
  hoc_register_dparam_semantics(_mechtype, 4, "mg_ion");
  hoc_register_dparam_semantics(_mechtype, 5, "mg_ion");
  hoc_register_dparam_semantics(_mechtype, 6, "cvodeieq");
 	hoc_register_cvode(_mechtype, _ode_count, _ode_map, _ode_spec, _ode_matsol);
 	hoc_register_tolerance(_mechtype, _hoc_state_tol, &_atollist);
 pnt_receive[_mechtype] = _net_receive;
 pnt_receive_init[_mechtype] = _net_init;
 pnt_receive_size[_mechtype] = 5;
 
    hoc_register_var(hoc_scdoub, hoc_vdoub, hoc_intfunc);
 	ivoc_help("help ?1 NMDA_DynSyn /Users/fionastern/Desktop/comp-neuro/mods/NMDA_DynSyn.mod\n");
 hoc_register_limits(_mechtype, _hoc_parm_limits);
 hoc_register_units(_mechtype, _hoc_parm_units);
 }
static int _reset;
static const char *modelname = "NMDA receptor with Ca influx and pre-synaptic short-term plasticity";

static int error;
static int _ninits = 0;
static int _match_recurse=1;
static void _modl_cleanup(){ _match_recurse=1;}
 
static int _ode_spec1(_internalthreadargsproto_);
/*static int _ode_matsol1(_internalthreadargsproto_);*/
 static neuron::container::field_index _slist1[2], _dlist1[2];
 static int state(_internalthreadargsproto_);
 
/*CVODE*/
 static int _ode_spec1 (_internalthreadargsproto_) {int _reset = 0; {
   DA = - A / tau_rise ;
   DB = - B / tau_decay ;
   }
 return _reset;
}
 static int _ode_matsol1 (_internalthreadargsproto_) {
 DA = DA  / (1. - dt*( ( - 1.0 ) / tau_rise )) ;
 DB = DB  / (1. - dt*( ( - 1.0 ) / tau_decay )) ;
  return 0;
}
 /*END CVODE*/
 static int state (_internalthreadargsproto_) { {
    A = A + (1. - exp(dt*(( - 1.0 ) / tau_rise)))*(- ( 0.0 ) / ( ( - 1.0 ) / tau_rise ) - A) ;
    B = B + (1. - exp(dt*(( - 1.0 ) / tau_decay)))*(- ( 0.0 ) / ( ( - 1.0 ) / tau_decay ) - B) ;
   }
  return 0;
}
 
double mgblock ( _internalthreadargsprotocomma_ double _lv ) {
   double _lmgblock;
 _lmgblock = 1.0 / ( 1.0 + exp ( 0.062 * - _lv ) * ( mgo / 3.57 ) ) ;
   
return _lmgblock;
 }
 
static double _hoc_mgblock(void* _vptr) {
 double _r;
 Datum* _ppvar; Datum* _thread; NrnThread* _nt;
   auto* const _pnt = static_cast<Point_process*>(_vptr);
  auto* const _p = _pnt->_prop;
  if (!_p) {
    hoc_execerror("POINT_PROCESS data instance not valid", NULL);
  }
   _nrn_mechanism_cache_instance _ml_real{_p};
  auto* const _ml = &_ml_real;
  size_t const _iml{};
  _ppvar = _nrn_mechanism_access_dparam(_p);
  _thread = _extcall_thread.data();
  double* _globals = nullptr;
  if (gind != 0 && _thread != nullptr) { _globals = _thread[_gth].get<double*>(); }
  _nt = static_cast<NrnThread*>(_pnt->_vnt);
 _r =  mgblock ( _threadargscomma_ *getarg(1) );
 return(_r);
}
 
static void _net_receive (Point_process* _pnt, double* _args, double _lflag) 
{  Prop* _p; Datum* _ppvar; Datum* _thread; NrnThread* _nt;
   _nrn_mechanism_cache_instance _ml_real{_pnt->_prop};
  auto* const _ml = &_ml_real;
  size_t const _iml{};
   _thread = nullptr; double* _globals = nullptr; _nt = (NrnThread*)_pnt->_vnt;   _ppvar = _nrn_mechanism_access_dparam(_pnt->_prop);
  if (_tsav > t){ hoc_execerror(hoc_object_name(_pnt->ob), ":Event arrived out of order. Must call ParallelContext.set_maxstep AFTER assigning minimum NetCon.delay");}
 _tsav = t; {
   _args[3] = _args[3] * exp ( - ( t - _args[4] ) / tau_fac ) ;
   _args[3] = _args[3] + U1 * ( 1.0 - _args[3] ) ;
   _args[2] = 1.0 - ( 1.0 - _args[2] ) * exp ( - ( t - _args[4] ) / tau_rec ) ;
   _args[1] = _args[3] * _args[2] ;
   _args[2] = _args[2] - _args[3] * _args[2] ;
   _args[4] = t ;
     if (nrn_netrec_state_adjust && !cvode_active_){
    /* discon state adjustment for cnexp case (rate uses no local variable) */
    double __state = A;
    double __primary = (A + _args[0] * factor * _args[1]) - __state;
     __primary += ( 1. - exp( 0.5*dt*( ( - 1.0 ) / tau_rise ) ) )*( - ( 0.0 ) / ( ( - 1.0 ) / tau_rise ) - __primary );
    A += __primary;
  } else {
 A = A + _args[0] * factor * _args[1] ;
     }
   if (nrn_netrec_state_adjust && !cvode_active_){
    /* discon state adjustment for cnexp case (rate uses no local variable) */
    double __state = B;
    double __primary = (B + _args[0] * factor * _args[1]) - __state;
     __primary += ( 1. - exp( 0.5*dt*( ( - 1.0 ) / tau_decay ) ) )*( - ( 0.0 ) / ( ( - 1.0 ) / tau_decay ) - __primary );
    B += __primary;
  } else {
 B = B + _args[0] * factor * _args[1] ;
     }
 } }
 
static void _net_init(Point_process* _pnt, double* _args, double _lflag) {
     _nrn_mechanism_cache_instance _ml_real{_pnt->_prop};
  auto* const _ml = &_ml_real;
  size_t const _iml{};
  Datum* _ppvar = _nrn_mechanism_access_dparam(_pnt->_prop);
  Datum* _thread = nullptr;
  double* _globals = nullptr;
  NrnThread* _nt = (NrnThread*)_pnt->_vnt;
 _args[2] = 1.0 ;
   _args[3] = 0.0 ;
   _args[4] = t ;
   }
 
static int _ode_count(int _type){ return 2;}
 
static void _ode_spec(_nrn_model_sorted_token const& _sorted_token, NrnThread* _nt, Memb_list* _ml_arg, int _type) {
   Datum* _ppvar;
   size_t _iml;   _nrn_mechanism_cache_range* _ml;   Node* _nd{};
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
  mgo = _ion_mgo;
     _ode_spec1 (_threadargs_);
  }}
 
static void _ode_map(Prop* _prop, int _ieq, neuron::container::data_handle<double>* _pv, neuron::container::data_handle<double>* _pvdot, double* _atol, int _type) { 
  Datum* _ppvar;
  _ppvar = _nrn_mechanism_access_dparam(_prop);
  _cvode_ieq = _ieq;
  for (int _i=0; _i < 2; ++_i) {
    _pv[_i] = _nrn_mechanism_get_param_handle(_prop, _slist1[_i]);
    _pvdot[_i] = _nrn_mechanism_get_param_handle(_prop, _dlist1[_i]);
    _cvode_abstol(_atollist, _atol, _i);
  }
 }
 
static void _ode_matsol_instance1(_internalthreadargsproto_) {
 _ode_matsol1 (_threadargs_);
 }
 
static void _ode_matsol(_nrn_model_sorted_token const& _sorted_token, NrnThread* _nt, Memb_list* _ml_arg, int _type) {
   Datum* _ppvar;
   size_t _iml;   _nrn_mechanism_cache_range* _ml;   Node* _nd{};
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
  mgo = _ion_mgo;
 _ode_matsol_instance1(_threadargs_);
 }}

static void initmodel(_internalthreadargsproto_) {
  int _i; double _save;{
  A = A0;
  B = B0;
 {
   double _ltp ;
 A = 0.0 ;
   B = 0.0 ;
   _ltp = ( tau_rise * tau_decay ) / ( tau_decay - tau_rise ) * log ( tau_decay / tau_rise ) ;
   factor = - exp ( - _ltp / tau_rise ) + exp ( - _ltp / tau_decay ) ;
   factor = 1.0 / factor ;
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
 _tsav = -1e20;
   _v = _vec_v[_ni[_iml]];
 v = _v;
  mgo = _ion_mgo;
 initmodel(_threadargs_);
 }
}

static double _nrn_current(_internalthreadargsprotocomma_ double _v) {
double _current=0.; v=_v;
{ {
   g = B - A ;
   i = g * mgblock ( _threadargscomma_ v ) * ( v - e ) ;
   ica = ca_ratio * i ;
   inon = ( 1.0 - ca_ratio ) * i ;
   }
 _current += ica;
 _current += inon;

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
  mgo = _ion_mgo;
 auto const _g_local = _nrn_current(_threadargscomma_ _v + .001);
 	{ double _dica;
  _dica = ica;
 _rhs = _nrn_current(_threadargscomma_ _v);
  _ion_dicadv += (_dica - ica)/.001 * 1.e2/ (_nd_area);
 	}
 _g = (_g_local - _rhs)/.001;
  _ion_ica += ica * 1.e2/ (_nd_area);
 _g *=  1.e2/(_nd_area);
 _rhs *= 1.e2/(_nd_area);
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
  mgo = _ion_mgo;
 {   state(_threadargs_);
  } }}

}

static void terminal(){}

static void _initlists(){
 int _i; static int _first = 1;
  if (!_first) return;
 _slist1[0] = {A_columnindex, 0};  _dlist1[0] = {DA_columnindex, 0};
 _slist1[1] = {B_columnindex, 0};  _dlist1[1] = {DB_columnindex, 0};
_first = 0;
}

#if NMODL_TEXT
static void register_nmodl_text_and_filename(int mech_type) {
    const char* nmodl_filename = "/Users/fionastern/Desktop/comp-neuro/mods/NMDA_DynSyn.mod";
    const char* nmodl_file_text = 
  "TITLE  NMDA receptor with Ca influx and pre-synaptic short-term plasticity\n"
  "\n"
  "\n"
  "COMMENT\n"
  "Dynamic presynaptic activity based on Fuhrmann et al, 2002: \"Coding of temporal information by activity-dependent synapses\" \n"
  "\n"
  "Written by Paulo Aguiar and Mafalda Sousa, IBMC, May 2008\n"
  "pauloaguiar@fc.up.pt ; mafsousa@ibmc.up.pt\n"
  "ENDCOMMENT\n"
  "\n"
  "\n"
  "NEURON {\n"
  "	POINT_PROCESS NMDA_DynSyn\n"
  "	USEION ca WRITE ica	\n"
  "	USEION mg READ mgo VALENCE 2\n"
  "	RANGE tau_rise, tau_decay\n"
  "	RANGE U1, tau_rec, tau_fac\n"
  "	RANGE i, g, e, mg, inon, ica, ca_ratio\n"
  "	NONSPECIFIC_CURRENT inon\n"
  "    }\n"
  "    \n"
  "UNITS {\n"
  "	(nA) = (nanoamp)\n"
  "	(mV) = (millivolt)\n"
  "	(molar) = (1/liter)\n"
  "	(mM) = (millimolar)\n"
  "    }    \n"
  "    \n"
  "    PARAMETER {\n"
  "  	tau_rise  = 5.0   (ms)  : dual-exponential conductance profile\n"
  "	tau_decay = 70.0  (ms)  : IMPORTANT: tau_rise < tau_decay\n"
  "	U1        = 1.0   (1)   : The parameter U1, tau_rec and tau_fac define\n"
  "	tau_rec   = 0.1   (ms)  : the pre-synaptic SP short-term plasticity\n"
  "	tau_fac   = 0.1   (ms)  : mechanism (see Fuhrmann et al, 2002)\n"
  "	e         = 0.0   (mV)  : synapse reversal potential\n"
  "	mgo		  = 1.0   (mM)  : external magnesium concentration\n"
  "	ca_ratio  = 0.1   (1)   : ratio of calcium current to total current( Burnashev/Sakmann J Phys 1995 485 403-418)\n"
  "    }\n"
  "    \n"
  "    \n"
  "ASSIGNED {\n"
  "	v		(mV)\n"
  "	i		(nA)\n"
  "	g		(umho)\n"
  "	factor	(1)\n"
  "	ica		(nA)\n"
  "	inon	(nA)\n"
  "}\n"
  "\n"
  "STATE {\n"
  "	A\n"
  "	B\n"
  "}\n"
  "\n"
  "INITIAL{\n"
  "	LOCAL tp\n"
  "	A = 0\n"
  "	B = 0\n"
  "	tp = (tau_rise*tau_decay)/(tau_decay-tau_rise)*log(tau_decay/tau_rise)\n"
  "	factor = -exp(-tp/tau_rise)+exp(-tp/tau_decay)\n"
  "	factor = 1/factor\n"
  "}\n"
  "\n"
  "BREAKPOINT {\n"
  "	SOLVE state METHOD cnexp\n"
  "	g = B-A\n"
  "	i = g*mgblock(v)*(v-e)\n"
  "	ica = ca_ratio*i\n"
  "	inon = (1-ca_ratio)*i\n"
  "	:printf(\"\\nt=%f\\tinon=%f\\tica=%f\\ti=%f\\tmgb=%f\",t, inon, ica, i, mgblock(v))\n"
  "}\n"
  "\n"
  "DERIVATIVE state{\n"
  "	A' = -A/tau_rise\n"
  "	B' = -B/tau_decay\n"
  "}\n"
  "\n"
  "FUNCTION mgblock(v(mV)) {\n"
  "	: from Jahr & Stevens 1990\n"
  "	mgblock = 1 / (1 + exp(0.062 (/mV) * -v) * (mgo / 3.57 (mM)))\n"
  "}\n"
  "\n"
  "NET_RECEIVE (weight, Pv, P, Use, t0 (ms)){\n"
  "	INITIAL{\n"
  "		P=1\n"
  "		Use=0\n"
  "		t0=t\n"
  "	}	\n"
  "\n"
  "	Use = Use * exp(-(t-t0)/tau_fac)\n"
  "	Use = Use + U1*(1-Use) \n"
  "	P = 1-(1- P) * exp(-(t-t0)/tau_rec)\n"
  "	Pv= Use * P\n"
  "	P = P - Use * P\n"
  "	\n"
  "	t0=t\n"
  "	\n"
  "	A=A + weight*factor*Pv\n"
  "	B=B + weight*factor*Pv\n"
  "}\n"
  "\n"
  ;
    hoc_reg_nmodl_filename(mech_type, nmodl_filename);
    hoc_reg_nmodl_text(mech_type, nmodl_file_text);
}
#endif

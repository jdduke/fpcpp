/////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2012, Jared Duke.
// This code is released under the MIT License.
// www.opensource.org/licenses/mit-license.php
/////////////////////////////////////////////////////////////////////////////

#ifndef _FP_CURRY_H_
#define _FP_CURRY_H_

#include <fp_curry_defines.h>
#include <fp_composition_utils.h>

#include <tuple>

namespace fp {

template<size_t ArgC> struct curry_helper_impl;

  // When all arguments are given, we can uniquely determine the function type (useful for template/overloaded functions)

template<> struct curry_helper_impl<1> {
  template<typename F,typename T>
  static inline auto bind(F f, T t) -> std::function< decltype( f(t) ) (void) > {
    typedef decltype( f(t) ) result_type;
    return std::bind( static_cast< std::function< result_type (T) > >(f), t );
  }
};
template<> struct curry_helper_impl<2> {
  template<typename F,typename T>
  static inline auto bind(F f, T t) FP_RETURNS( FP_CURRY2(FP_PREFIX1) );
  template<typename F, typename T, typename T1>
  static inline auto bind2(F f, T t, T1 t1) -> std::function< decltype( f(t,t1) ) (void) > {
    typedef decltype( f(t,t1) ) result_type;
    return std::bind( static_cast< std::function< result_type (T,T1) > >(f), t, t1 );
  }
};
template<> struct curry_helper_impl<3> {
  template<typename F,typename T>
  static inline auto bind(F f, T t) FP_RETURNS( FP_CURRY3(FP_PREFIX1) );
  template<typename F, typename T, typename T1>
  static inline auto bind2(F f, T t, T1 t1) FP_RETURNS( FP_CURRY2(FP_PREFIX2) );
  template<typename F, typename T, typename T1, typename T2>
  static inline auto bind3(F f, T t, T1 t1, T2 t2) -> std::function< decltype( f(t,t1,t2) ) (void) > {
    typedef decltype( f(t,t1,t2) ) result_type;
    return std::bind( static_cast< std::function< result_type (T,T1,T2) > >(f), t, t1, t2 );
  }
};
template<> struct curry_helper_impl<4> {
  template<typename F,typename T>
  static inline auto bind(F f, T t) FP_RETURNS( FP_CURRY4(FP_PREFIX1) );
  template<typename F, typename T, typename T1>
  static inline auto bind2(F f, T t, T1 t1) FP_RETURNS( FP_CURRY3(FP_PREFIX2) );
  template<typename F, typename T, typename T1, typename T2>
  static inline auto bind3(F f, T t, T1 t1, T2 t2) FP_RETURNS( FP_CURRY2(FP_PREFIX3) );
  template<typename F, typename T, typename T1, typename T2, typename T3>
  static inline auto bind4(F f, T t, T1 t1, T2 t2, T3 t3) -> std::function< decltype( f(t,t1,t2,t3) ) (void) > {
    typedef decltype( f(t,t1,t2,t3) ) result_type;
    return std::bind( static_cast< std::function< result_type (T,T1,T2,T3) > >(f), t, t1, t2, t3 );
  }
};

template <typename F> struct curry_helper : public curry_helper_impl<fp::function_traits<F>::arity> { };

template <typename F, typename T>
inline auto curry(F f, T t) FP_RETURNS( fp::curry_helper<F>::bind(f,t) );

template <typename F, typename T, typename T1>
inline auto curry2(F f, T t, T1 t1) FP_RETURNS( fp::curry_helper<F>::bind2(f,t,t1) );

template <typename F, typename T, typename T1, typename T2>
inline auto curry3(F f, T t, T1 t1, T2 t2) FP_RETURNS( fp::curry_helper<F>::bind3(f,t,t1,t2) );

template<typename F, typename T, typename T1, typename T2, typename T3>
inline auto curry4(F f, T t, T1 t1, T2 t2, T3 t3) FP_RETURNS( fp::curry_helper<F>::bind4(f,t,t1,t2,t3) );

// This is useful for template/overloaded functions... using the arguments, we can uniquely determine the
//     the function template instance, and in turn curry the arguments
template <typename F, typename T>
inline auto curryAll(F f, T t) FP_RETURNS( std::bind(f,t) );

template <typename F, typename T, typename T1>
inline auto curryAll2(F f, T t, T1 t1) FP_RETURNS( std::bind(f,t,t1) );

template <typename F, typename T, typename T1, typename T2>
inline auto curryAll3(F f, T t, T1 t1, T2 t2) FP_RETURNS( std::bind(f,t,t1,t2) );

template<typename F, typename T, typename T1, typename T2, typename T3>
inline auto curryAll4(F f, T t, T1 t1, T2 t2, T3 t3) FP_RETURNS( std::bind(f,t,t1,t2,t3) );


///////////////////////////////////////////////////////////////////////////
// uncurry

template<typename F>
struct uncurryF {

  uncurryF(F f_) : f(f_) { }

  template<typename T0, typename T1>
  inline auto operator()(const std::pair<T0,T1>& p2) -> decltype( declval<F>()( fst(p2), snd(p2) ) ) {
    return f( fst(p2), snd(p2) );
  }
  template<typename T0, typename T1, typename T2>
  inline auto operator()(const std::tuple<T0,T1,T2>& p3) -> decltype( declval<F>()( std::get<0>(p3), std::get<1>(p3), std::get<2>(p3) ) ) {
    return f( std::get<0>(p3), std::get<1>(p3), std::get<2>(p3) );
  }
  template<typename T0, typename T1, typename T2, typename T3>
  inline auto operator()(const std::tuple<T0,T1,T2,T3>& p4) -> decltype( declval<F>()( std::get<0>(p4), std::get<1>(p4), std::get<2>(p4), std::get<3>(p4) ) ) {
    return f( std::get<0>(p4), std::get<1>(p4), std::get<2>(p4), std::get<3>(p4) );
  }

private:
  F f;
};

template<typename F>
inline uncurryF<F> uncurry(F f) {
  return uncurryF<F>(f);
}

} /* namespace fp */

#endif /* _FP_CURRY_H_ */

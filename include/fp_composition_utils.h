/////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2012, Jared Duke.
// This code is released under the MIT License.
// www.opensource.org/licenses/mit-license.php
/////////////////////////////////////////////////////////////////////////////

#ifndef _FP_COMPOSITION_UTILS_H_
#define _FP_COMPOSITION_UTILS_H_

#include "fp_defines.h"

#include <functional>

#if !FP_DECLVAL
namespace std {
  template <typename T>
  typename std::add_rvalue_reference<T>::type declval();
}
#endif

namespace fp {

///////////////////////////////////////////////////////////////////////////

template <typename T>
struct make_function_traits {
  typedef std::function<typename std::enable_if<std::is_function<T>::value, T>::type> type;
};

template<typename T>
auto make_function(T *t) -> typename make_function_traits<T>::type {
  return t;
}

///////////////////////////////////////////////////////////////////////////

template <typename T> struct function_traits;
template <typename T0, typename T1, typename T2> struct function_traits2;
template <typename T0, typename T1, typename T2, typename T3> struct function_traits3;

///////////////////////////////////////////////////////////////////////////

#if FP_VARIADIC

template <typename T>
struct function_traits : public function_traits<decltype(&T::operator())> {};

template <typename C, typename R, typename... Args>
struct function_traits<R(C::*)(Args...) const> {
  static const size_t arity = sizeof...(Args);
  typedef R result_type;
  template <size_t i>
  struct arg {
    typedef typename std::tuple_element<i, std::tuple<Args...>>::type type;
  };
};

template <typename C, typename R, typename... Args>
struct function_traits<R(C::*)(Args...)> {
  static const size_t arity = sizeof...(Args);
  typedef R result_type;
  template <size_t i>
  struct arg {
    typedef typename std::tuple_element<i, std::tuple<Args...>>::type type;
  };
};

template<typename... Args>
struct results;

template<typename F, typename... Args>
struct results<F,Args...> {
  typedef typename std::result_of< F(Args...) >::type type;
};

template<typename F, typename G, typename... Args>
struct compound_result {
  typedef typename results<G,Args...>::type U;
  typedef typename results<F,U>::type type;
};

#else

template <typename T>
struct function_traits : public function_traits< decltype( &T::operator() ) > {};

template <typename T>
struct function_traits<T*> : public function_traits< typename make_function_traits<T>::type > { };

template <typename T0, typename T1, typename T2>
struct function_traits2 {
  static const size_t arity = function_traits<T1>::arity + function_traits<T2>::arity;
  typedef typename function_traits<T0>::result_type result_type;
};

template <typename T0, typename T1, typename T2, typename T3>
struct function_traits3 {
  static const size_t arity = function_traits2<T0,T1,T2>::arity + function_traits<T3>::arity;
  typedef typename function_traits<T0>::result_type result_type;
};

template <typename F, typename G>
struct function_traits< composed<F,G> > : public function_traits<G> {};

template <typename F, typename G0, typename G1>
struct function_traits< composed2<F,G0,G1> > : public function_traits2<F,G0,G1> {};

template <typename C, typename R>
struct function_traits<R(C::*)() const> {
  static const size_t arity = 0;
  typedef R result_type;
};

template <typename C, typename R, typename T0>
struct function_traits<R(C::*)(T0) const> {
  static const size_t arity = 1;
  typedef R result_type;
  typedef T0 t0_type;
};

template <typename C, typename R, typename T0, typename T1>
struct function_traits<R(C::*)(T0,T1) const> {
  static const size_t arity = 2;
  typedef R result_type;
  typedef T0 t0_type;
  typedef T1 t1_type;
};

template <typename C, typename R, typename T0, typename T1, typename T2>
struct function_traits<R(C::*)(T0,T1,T2) const> {
  static const size_t arity = 3;
  typedef R result_type;
  typedef T0 t0_type;
  typedef T1 t1_type;
  typedef T2 t2_type;
};

template <typename C, typename R, typename T0, typename T1, typename T2, typename T3>
struct function_traits<R(C::*)(T0,T1,T2,T3) const> {
  static const size_t arity = 4;
  typedef R result_type;
  typedef T0 t0_type;
  typedef T1 t1_type;
  typedef T2 t2_type;
  typedef T3 t3_type;
};

template <typename C, typename R, typename T0, typename T1, typename T2, typename T3, typename T4>
struct function_traits<R(C::*)(T0,T1,T2,T3,T4) const> {
  static const size_t arity = 5;
  typedef R result_type;
  typedef T0 t0_type;
  typedef T1 t1_type;
  typedef T2 t2_type;
  typedef T3 t3_type;
  typedef T4 t4_type;
};

template <typename C, typename R>
struct function_traits<R(C::*)(void)> {
  static const size_t arity = 0;
  typedef R result_type;
};

template <typename C, typename R, typename T0>
struct function_traits<R(C::*)(T0)> {
  static const size_t arity = 1;
  typedef R result_type;
  typedef T0 t0_type;
};

template <typename C, typename R, typename T0, typename T1>
struct function_traits<R(C::*)(T0,T1)> {
  static const size_t arity = 2;
  typedef R result_type;
  typedef T0 t0_type;
  typedef T1 t1_type;
};

template <typename C, typename R, typename T0, typename T1, typename T2>
struct function_traits<R(C::*)(T0,T1,T2)> {
  static const size_t arity = 3;
  typedef R result_type;
  typedef T0 t0_type;
  typedef T1 t1_type;
  typedef T2 t2_type;
};

template <typename C, typename R, typename T0, typename T1, typename T2, typename T3>
struct function_traits<R(C::*)(T0,T1,T2,T3)> {
  static const size_t arity = 4;
  typedef R result_type;
  typedef T0 t0_type;
  typedef T1 t1_type;
  typedef T2 t2_type;
  typedef T3 t3_type;
};

template <typename C, typename R, typename T0, typename T1, typename T2, typename T3, typename T4>
struct function_traits<R(C::*)(T0,T1,T2,T3,T4)> {
  static const size_t arity = 5;
  typedef R result_type;
  typedef T0 t0_type;
  typedef T1 t1_type;
  typedef T2 t2_type;
  typedef T3 t3_type;
  typedef T4 t4_type;
};

#endif

///////////////////////////////////////////////////////////////////////////

} /* namespace fp */

#endif /* _FP_COMPOSITION_UTILS_H */

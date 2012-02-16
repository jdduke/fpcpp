/////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2012, Jared Duke.
// This code is released under the MIT License.
// www.opensource.org/licenses/mit-license.php
/////////////////////////////////////////////////////////////////////////////

#if FP_COMPOUND

#ifndef _FP_COMPOSITION_COMPOUND_UTILS_H_
#define _FP_COMPOSITION_COMPOUND_UTILS_H_

#include <tuple>

#include "fp_defines.h"
#include "fp_composition_utils.h"

namespace fp {

template<typename F, typename G0, typename G1>               class composed_base2;
template<typename F, typename G0, typename G1, size_t ArgGC> class composed2;

template<typename F, typename G0, typename G1, typename G2>               class composed_base3;
template<typename F, typename G0, typename G1, typename G2, size_t ArgGC> class composed3;

///////////////////////////////////////////////////////////////////////////

template< size_t arg_c >
struct apply_helper {
  template< typename F, typename Args, size_t arg_0 >
  static auto apply( F& f, const Args& args ) FP_RETURNS( f() );
};

template<>
struct apply_helper<1> {
  template< typename F, typename Args, size_t arg_0 >
  static auto apply( F& f, const Args& args ) FP_RETURNS( f( std::get<arg_0>(args) ) );
};

template<>
struct apply_helper<2> {
  template< typename F, typename Args, size_t arg_0 >
  static auto apply( F& f, const Args& args ) FP_RETURNS( f( std::get<arg_0>(args), std::get<arg_0+1>(args) ) );
};

template<>
struct apply_helper<3> {
  template< typename F, typename Args, size_t arg_0 >
  static auto apply( F& f, const Args& args ) FP_RETURNS( f( std::get<arg_0>(args), std::get<arg_0+1>(args), std::get<arg_0+2>(args) ) );
};

template<>
struct apply_helper<4> {
  template< typename F, typename Args, size_t arg_0 >
  static auto apply( F& f, const Args& args ) FP_RETURNS( f( std::get<arg_0>(args), std::get<arg_0+1>(args), std::get<arg_0+2>(args), std::get<arg_0+3>(args) ) );
};

template<>
struct apply_helper<5> {
  template< typename F, typename Args, size_t arg_0 >
  static auto apply( F& f, const Args& args ) FP_RETURNS( f( std::get<arg_0>(args), std::get<arg_0+1>(args), std::get<arg_0+2>(args), std::get<arg_0+3>(args), std::get<arg_0+4>(args) ) );
};

template< typename F, typename Args, size_t arg_0, size_t arg_c >
auto apply_func( F& f, const Args& args) FP_RETURNS( apply_helper<arg_c>::apply<F,Args,arg_0>(f, args) );

template< typename R, typename F, typename Args, size_t arg_0, size_t arg_c >
R apply_func2( F& f, const Args& args) {
  return apply_helper<arg_c>::apply<F,Args,arg_0>(f, args);
}

///////////////////////////////////////////////////////////////////////////

template<typename F, typename G0, typename G1>
struct composed_traits2 {
  typedef composed2<F,G0,G1,function_traits2<F,G0,G1>::arity> type;
};

///////////////////////////////////////////////////////////////////////////

template<typename F, typename G0, typename G1, typename tuple_type>
struct compound_result2_helper {
  typedef typename function_traits<G0>::result_type U;
  typedef typename function_traits<G1>::result_type V;
  typedef typename result2<F,U,V>::type type;
};

///////////////////////////////////////////////////////////////////////////

template<typename F, typename G0, typename G1>
struct compound_result20 {
  typedef void tuple_type;
  typedef typename compound_result2_helper<F,G0,G1,tuple_type>::type type;
};

#if FP_VARIADIC

template<typename F, typename G0, typename G1, typename... Args>
struct compound_result2 {
  typedef std::tuple<Args...> tuple_type;
  typedef typename compound_result2_helper<F,G0,G1,tuple_type>::type type;
};

#else

template<typename F, typename G0, typename G1, typename T0>
struct compound_result21 {
  typedef std::tuple<T0> tuple_type;
  typedef typename compound_result2_helper<F,G0,G1,tuple_type>::type type;
};

template<typename F, typename G0, typename G1, typename T0, typename T1>
struct compound_result22 {
  typedef std::tuple<T0,T1> tuple_type;
  typedef typename compound_result2_helper<F,G0,G1,tuple_type>::type type;
};

template<typename F, typename G0, typename G1, typename T0, typename T1, typename T2>
struct compound_result23 {
  typedef std::tuple<T0,T1,T2> tuple_type;
  typedef typename compound_result2_helper<F,G0,G1,tuple_type>::type type;
};

template<typename F, typename G0, typename G1, typename T0, typename T1, typename T2, typename T3>
struct compound_result24 {
  typedef std::tuple<T0,T1,T2,T3> tuple_type;
  typedef typename compound_result2_helper<F,G0,G1,tuple_type>::type type;
};

#endif

///////////////////////////////////////////////////////////////////////////

template<typename F, typename G0, typename G1, typename G2>
struct composed_traits3 {
  typedef composed3<F,G0,G1,G2,function_traits3<F,G0,G1,G2>::arity> type;
};

///////////////////////////////////////////////////////////////////////////

template<typename F, typename G0, typename G1, typename G2, typename tuple_type>
struct compound_result3_helper {
  typedef typename function_traits<G0>::result_type U;
  typedef typename function_traits<G1>::result_type V;
  typedef typename function_traits<G0>::result_type W;
  typedef typename result3<F,U,V,W>::type type;
};

template<typename F, typename G0, typename G1, typename G2>
struct compound_result30 {
  typedef void tuple_type;
  typedef typename compound_result3_helper<F,G0,G1,G2,tuple_type>::type type;
};

#if FP_VARIADIC

template<typename F, typename G0, typename G1, typename G2, typename... Args>
struct compound_result3 {
  typedef std::tuple<Args...> tuple_type;
  typedef typename compound_result3_helper<F,G0,G1,G2,tuple_type>::type type;
};

#else

template<typename F, typename G0, typename G1, typename G2, typename T0>
struct compound_result31 {
  typedef std::tuple<T0> tuple_type;
  typedef typename compound_result3_helper<F,G0,G1,G2,tuple_type>::type type;
};

template<typename F, typename G0, typename G1, typename G2, typename T0, typename T1>
struct compound_result32 {
  typedef std::tuple<T0,T1> tuple_type;
  typedef typename compound_result3_helper<F,G0,G1,G2,tuple_type>::type type;
};

template<typename F, typename G0, typename G1, typename G2,  typename T0, typename T1, typename T2>
struct compound_result33 {
  typedef std::tuple<T0,T1,T2> tuple_type;
  typedef typename compound_result3_helper<F,G0,G1,G2,tuple_type>::type type;
};

template<typename F, typename G0, typename G1, typename G2, typename T0, typename T1, typename T2, typename T3>
struct compound_result34 {
  typedef std::tuple<T0,T1,T2,T3> tuple_type;
  typedef typename compound_result3_helper<F,G0,G1,G2,tuple_type>::type type;
};

#endif

///////////////////////////////////////////////////////////////////////////

} /* namespace fp */

#endif /* _FP_EXPERIMENTAL_UTILS_H_ */

#endif /* FP_COMPOUND */

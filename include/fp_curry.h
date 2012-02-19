/////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2012, Jared Duke.
// This code is released under the MIT License.
// www.opensource.org/licenses/mit-license.php
/////////////////////////////////////////////////////////////////////////////

#ifndef _FP_CURRY_H_
#define _FP_CURRY_H_

#include <fp_curry_defines.h>
#include <fp_composition_utils.h>

namespace fp {

template<size_t FArgC> struct curry_helper_impl { };

#if 1
  template<> struct curry_helper_impl<1> {
    template<typename F,typename T>
    static inline auto bind(F f, T t) FP_RETURNS( FP_CURRY1(FP_PREFIX1) );
   };
  template<> struct curry_helper_impl<2> {
    template<typename F,typename T>
    static inline auto bind(F f, T t) FP_RETURNS( FP_CURRY2(FP_PREFIX1) );
    template<typename F, typename T, typename T1>
    static inline auto bind2(F f, T t, T1 t1) FP_RETURNS( FP_CURRY1(FP_PREFIX2) );
  };
  template<> struct curry_helper_impl<3> {
    template<typename F,typename T>
    static inline auto bind(F f, T t) FP_RETURNS( FP_CURRY3(FP_PREFIX1) );
    template<typename F, typename T, typename T1>
    static inline auto bind2(F f, T t, T1 t1) FP_RETURNS( FP_CURRY2(FP_PREFIX2) );
    template<typename F, typename T, typename T1, typename T2>
    static inline auto bind3(F f, T t, T1 t1, T2 t2) FP_RETURNS( FP_CURRY1(FP_PREFIX3) );
  };
  template<> struct curry_helper_impl<4> {
    template<typename F,typename T>
    static inline auto bind(F f, T t) FP_RETURNS( FP_CURRY4(FP_PREFIX1) );
    template<typename F, typename T, typename T1>
    static inline auto bind2(F f, T t, T1 t1) FP_RETURNS( FP_CURRY3(FP_PREFIX2) );
    template<typename F, typename T, typename T1, typename T2>
    static inline auto bind3(F f, T t, T1 t1, T2 t2) FP_RETURNS( FP_CURRY2(FP_PREFIX3) );
    template<typename F, typename T, typename T1, typename T2, typename T3>
    static inline auto bind4(F f, T t, T1 t1, T2 t2, T3 t3) FP_RETURNS( FP_CURRY1(FP_PREFIX4) );
  };
#else
FP_CURRY_DEFINE_HELPER(1)
FP_CURRY_DEFINE_HELPER(2)
FP_CURRY_DEFINE_HELPER(3)
FP_CURRY_DEFINE_HELPER(4)
FP_CURRY_DEFINE_HELPER(5)
#endif

template<typename F> struct uncurry_helper {
  typedef function_traits<F> f_traits;
  typedef typename f_traits::result_type result_type;
  typedef std::pair< typename f_traits::t0_type, typename f_traits::t1_type > pair_type;
  typedef std::function< result_type ( pair_type ) > f_type;
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

template<typename F>
inline auto uncurry(F f) -> typename uncurry_helper<F>::f_type {
  return [=]( typename uncurry_helper<F>::pair_type p ) {
    return f( fst(p), snd(p) );
  };
}

} /* namespace fp */

#endif /* _FP_CURRY_H_ */

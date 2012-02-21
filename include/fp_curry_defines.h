/////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2012, Jared Duke.
// This code is released under the MIT License.
// www.opensource.org/licenses/mit-license.php
/////////////////////////////////////////////////////////////////////////////

#ifndef _FP_CURRY_DEFINES_H_
#define _FP_CURRY_DEFINES_H_

#include "fp_defines.h"

#define FP_PREFIX1  f, t
#define FP_PREFIX2 FP_PREFIX1 , t1
#define FP_PREFIX3 FP_PREFIX2 , t2
#define FP_PREFIX4 FP_PREFIX3 , t3

#define FP_TYPE_PREFIX1  F f, T t
#define FP_TYPE_PREFIX2 FP_TYPE_PREFIX1 , T1 t1
#define FP_TYPE_PREFIX3 FP_TYPE_PREFIX2 , T2 t2
#define FP_TYPE_PREFIX4 FP_TYPE_PREFIX3 , T3 t3

#define FP_TTYPE_PREFIX1  typename F, typename T
#define FP_TTYPE_PREFIX2 FP_TYPE_PREFIX1 , typename T1
#define FP_TTYPE_PREFIX3 FP_TYPE_PREFIX2 , typename T2
#define FP_TTYPE_PREFIX4 FP_TYPE_PREFIX3 , typename T3

#define FP_CURRY1(a) std::bind(a)
#define FP_CURRY2(a) std::bind(a, std::placeholders::_1)
#define FP_CURRY3(a) std::bind(a, std::placeholders::_1, std::placeholders::_2)
#define FP_CURRY4(a) std::bind(a, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)
#define FP_CURRY5(a) std::bind(a, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4)

#define FP_CURRY_HELPER(FUNC,PREFIX) FUNC(PREFIX)
#define FP_CURRY(ARG,PREFIX)         FP_CURRY_HELPER(FP_CURRY##ARG, PREFIX)

#define FP_DEFINE_FUNCTION_OBJECT(a,b)                            \
  struct b {                                                      \
    b() { }                                                       \
    template<typename T>                                          \
    auto operator()(T t) FP_RETURNS( a(t) );                      \
    template<typename T>                                          \
    auto operator()(const T& t) FP_RETURNS( a(t) );               \
    template<typename F, typename T>                              \
    auto operator()(F f, T t) FP_RETURNS( a(f,t) );               \
    template<typename F, typename T, typename C>                  \
    auto operator()(F f, T t, const C& c) FP_RETURNS( a(f,t,c) ); \
  };

#if 1

#define FP_DEFINE_CURRIED_HELPER(a,b)   \
  template <typename F>                 \
  inline auto b(F f) FP_RETURNS(a);
#define FP_DEFINE_CURRIED_HELPER2(a,b)  \
  template <typename F, typename T>     \
  inline auto b(F f, T t) FP_RETURNS(a);

#define FP_CURRIED(func0,func1)                                                                                             \
  fp::curry(func0<decltype(func1),                                                                                          \
  /*typename fp::types<typename remove_const_ref<typename fp::function_traits< decltype(func1) >::t0_type>::type> >::list, */   \
  typename std::deque<typename remove_const_ref<typename fp::function_traits< decltype(func1) >::t0_type>::type> >,   \
  func1)
#define FP_CURRIED2(func0,func2,value)                                                                                             \
  fp::curry2(func0<decltype(func2), decltype(value), std::deque<typename remove_const_ref<typename fp::function_traits< decltype(func1) >::t0_type>::type> >, \
  func2, value)

#else

template <typename F>
inline FuncObj<F> makeFuncObj(F f) { return FuncObj<F>(f); }

#define FP_DEFINE_FUNCTION_OBJECT_CURRIED_HELPER(funcName, funcObjName1) \
template <typename F>                                      \
struct funcObjName1 {                                      \
  funcObjName1(F f_) : f(f_) { }                           \
  template<typename C>                                     \
  inline auto operator()(const C& c) FP_RETURNS( funcName(f, c) );
private:                                                   \
  F f;                                                     \
};

#define FP_DEFINE_FUNCTION_OBJECT_CURRIED(func0,func1) \
  FP_DEFINE_FUNCTION_OBJECT_CURRIED_HELPER()

#define FP_CURRIED(func0,func1) \
  return makeFuncObj(func0,func1)
  
  /*
  template<typename T, typename T1>                        \
  inline returnType operator()(const T& t, const T1& t1) { \
  return funcName(f,t,t1);                               \
  }                                                        \
  /*\
  template<typename F, typename T>                           \
  struct funcObjName2 : funcObjName1<F> {                    \
    funcObjName2(F f_) : funcObjName1(f_) { }                \
    inline returnType operator()(const T& t) {               \
      return funcObjName1::operator()(t);                    \
    }                                                        \
    template <typename T1>                                   \
    inline returnType operator()(const T& t, const T1& t1) { \
      return funcObjName1::operator()(t,t1);                 \
    }                                                        \
};*/

#endif

#define FP_DEFINE_CURRIED(funcName, funcName2)            \
  FP_DEFINE_FUNCTION_OBJECT( funcName, funcName ## Func ) \
  FP_DEFINE_CURRIED_HELPER(  FP_CURRIED(funcName, f), funcName2 )
#define FP_DEFINE_CURRIED_T(funcName, funcName2, funcName3)           \
  FP_DEFINE_FUNCTION_OBJECT( funcName, funcName ## Func )             \
  FP_DEFINE_CURRIED_HELPER(  FP_CURRIED( funcName, f),    funcName2 ) \
  FP_DEFINE_CURRIED_HELPER2( FP_CURRIED2(funcName, f, t), funcName3 )

#endif /* _FP_CURRY_DEFINES_H_ */

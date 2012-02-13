/////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2012, Jared Duke.
// This code is released under the MIT License.
// www.opensource.org/licenses/mit-license.php
/////////////////////////////////////////////////////////////////////////////

#ifndef _FP_CURRY_DEFINES_H_
#define _FP_CURRY_DEFINES_H_

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

#define FP_DEFINE_CURRIED_HELPER_IMPL(a,b)    \
  template <typename F>                       \
  inline auto b(F f) -> decltype( a ) {       \
  return a;                                   \
  }

#define FP_CURRIED(func0,func1)                                                                                             \
  fp::curry(func0<decltype(func1),                                                                                          \
  std::vector<typename remove_const_ref<typename fc::function_traits< decltype(func1) >::t0_type>::type> >, \
  func1)

#define FP_DEFINE_CURRIED_HELPER(funcName, funcName2)  FP_DEFINE_CURRIED_HELPER_IMPL( FP_CURRIED(funcName, f), funcName2 )

#endif /* _FP_CURRY_DEFINES_H_ */

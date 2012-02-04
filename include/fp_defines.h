/////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2012, Jared Duke.
// This code is released under the MIT License.
// www.opensource.org/licenses/mit-license.php
/////////////////////////////////////////////////////////////////////////////

#ifndef _FP_DEFINES_H_
#define _FP_DEFINES_H_

#define let auto
#define extent(c) head(c), tail(c)
#define rextent(c) rhead(c), rtail(c)
#define value_type_of(t) typename traits<t>::value_type

#define FP_DEFINE_CURRIED_HELPER_IMPL(a,b)    \
  template <typename F>                       \
    inline auto b(F f) -> decltype( a ) {     \
    return a;                                 \
  }
//#define FP_CURRIED(func0,func1) fp::curry(func0<decltype(func1), typename remove_ref_const<typename fc::function_traits< decltype(func1) >::t0_type>::type>, func1)
#define FP_CURRIED(func0,func1)                                                                                             \
  fp::curry(func0<decltype(func1),                                                                                          \
                  std::vector<typename remove_ref_const<typename fc::function_traits< decltype(func1) >::t0_type>::type> >, \
            func1)

#define FP_DEFINE_CURRIED_HELPER(funcName, funcName2)  FP_DEFINE_CURRIED_HELPER_IMPL( FP_CURRIED(funcName, f), funcName2 )
#define FP_DEFINE_FUNC_OBJ(funcName, funcObjName1, funcObjName2)   FP_DEFINE_CURRIED_HELPER(funcName, funcObjName1)
#define FP_DEFINE_FUNC_OBJ_T(funcName, funcObjName1, funcObjName2) FP_DEFINE_CURRIED_HELPER(funcName, funcObjName1)

#endif /* _FP_DEFINES_H_ */

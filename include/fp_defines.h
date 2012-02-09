/////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2012, Jared Duke.
// This code is released under the MIT License.
// www.opensource.org/licenses/mit-license.php
/////////////////////////////////////////////////////////////////////////////

#ifndef _FP_DEFINES_H_
#define _FP_DEFINES_H_

#define let auto
#define extent(c)  fp::head(c),  fp::tail(c)
#define rextent(c) fp::rhead(c), fp::rtail(c)
#define value_type_of(TYPE)         typename fp::traits<TYPE>::value_type
#define result_type_of(TYPE)        typename fc::function_traits<TYPE>::result_type
#define argument_type_of(TYPE,ARGC) typename fc::function_traits<TYPE>::t ## ARGC ## _type
#define nonconstref_type_of(TYPE)   typename fp::remove_const_ref<TYPE>::type

// auto function(<arguments>) RETURNS(<some-expression>);
#define RETURNS(...) -> decltype(__VA_ARGS__) { return (__VA_ARGS__); } typedef int RETURNS_CAT(RETURNS_, __LINE__)
#define RETURNS_CAT_0(x, y) x ## y
#define RETURNS_CAT(x, y) RETURNS_CAT_0(x,y)

#define FP_DEFINE_CURRIED_HELPER_IMPL(a,b)    \
  template <typename F>                       \
    inline auto b(F f) -> decltype( a ) {     \
    return a;                                 \
  }

#define FP_CURRIED(func0,func1)                                                                                             \
  fp::curry(func0<decltype(func1),                                                                                          \
                  std::vector<typename remove_const_ref<typename fc::function_traits< decltype(func1) >::t0_type>::type> >, \
            func1)

#define FP_DEFINE_CURRIED_HELPER(funcName, funcName2)  FP_DEFINE_CURRIED_HELPER_IMPL( FP_CURRIED(funcName, f), funcName2 )
#define FP_DEFINE_FUNC_OBJ(funcName, funcObjName1, funcObjName2)   FP_DEFINE_CURRIED_HELPER(funcName, funcObjName1)
#define FP_DEFINE_FUNC_OBJ_T(funcName, funcObjName1, funcObjName2) FP_DEFINE_CURRIED_HELPER(funcName, funcObjName1)

#endif /* _FP_DEFINES_H_ */

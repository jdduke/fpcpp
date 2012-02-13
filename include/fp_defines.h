/////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2012, Jared Duke.
// This code is released under the MIT License.
// www.opensource.org/licenses/mit-license.php
/////////////////////////////////////////////////////////////////////////////

#ifndef _FP_DEFINES_H_
#define _FP_DEFINES_H_

#define let auto
#define extent(c)  fp::head((c)),  fp::tail((c))
#define rextent(c) fp::rhead((c)), fp::rtail((c))
#define value_type_of(TYPE)         typename fp::traits<TYPE>::value_type
#define result_type_of(TYPE)        typename fc::function_traits<TYPE>::result_type
#define argument_type_of(TYPE,ARGC) typename fc::function_traits<TYPE>::t ## ARGC ## _type
#define nonconstref_type_of(TYPE)   typename fp::remove_const_ref<TYPE>::type

#define FP_CONCAT_IMPL(x, y) x ## y
#define FP_CONCAT(x, y) FP_CONCAT_IMPL(x,y)

// auto function(<arguments>) RETURNS(<some-expression>);
#define RETURNS(...) -> decltype(__VA_ARGS__) { return (__VA_ARGS__); } typedef int FP_CONCAT(RETURNS_, __LINE__)
#define FP_DEFINE_FUNC_OBJ(funcName, funcObjName1, funcObjName2)   FP_DEFINE_CURRIED_HELPER(funcName, funcObjName1)
#define FP_DEFINE_FUNC_OBJ_T(funcName, funcObjName1, funcObjName2) FP_DEFINE_CURRIED_HELPER(funcName, funcObjName1)

#endif /* _FP_DEFINES_H_ */

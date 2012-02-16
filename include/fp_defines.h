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
#define result_type_of(TYPE)        typename fp::function_traits<TYPE>::result_type
#define argument_type_of(TYPE,ARGC) typename fp::function_traits<TYPE>::t ## ARGC ## _type
#define nonconstref_type_of(TYPE)   typename fp::remove_const_ref<TYPE>::type

#define FP_CONCAT_IMPL(x, y) x ## y
#define FP_CONCAT(x, y) FP_CONCAT_IMPL(x,y)

// auto function(<arguments>) RETURNS(<some-expression>);
#define FP_RETURNS(...) -> decltype(__VA_ARGS__) { return (__VA_ARGS__); } typedef int FP_CONCAT(RETURNS_, __LINE__)
#define FP_DEFINE_FUNC_OBJ(funcName, funcObjName1, funcObjName2)   FP_DEFINE_CURRIED_HELPER(funcName, funcObjName1)
#define FP_DEFINE_FUNC_OBJ_T(funcName, funcObjName1, funcObjName2) FP_DEFINE_CURRIED_HELPER(funcName, funcObjName1)


#if defined(_MSC_VER)
#define FP_DECLVAL  0
#define FP_VARIADIC 0
#define FP_COMPOUND 1
#else
#define FP_DECLVAL  1
#define FP_VARIADIC 0
#define FP_COMPOUND 0
#endif

#if !defined(FP_OPERATORS)
#define FP_OPERATORS 1
#endif

#if FP_OPERATORS

#if !defined(FP_PLUS_OPERATOR)
#define FP_PLUS_OPERATOR 1
#endif

#if !defined(FP_DOT_OPERATOR)
#define FP_DOT_OPERATOR 1
#endif

#else

#if !defined(FP_PLUS_OPERATOR)
#define FP_PLUS_OPERATOR 0
#endif

#if !defined(FP_DOT_OPERATOR)
#define FP_DOT_OPERATOR 0
#endif

#endif


#endif /* _FP_DEFINES_H_ */

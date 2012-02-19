/////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2012, Jared Duke.
// This code is released under the MIT License.
// www.opensource.org/licenses/mit-license.php
/////////////////////////////////////////////////////////////////////////////

#ifndef _FP_DEFINES_H_
#define _FP_DEFINES_H_

// Compiler specific defines
// FP_DECLVAL  - Whether the compiler has a built-in declval function
// FP_VARIADIC - Whether the compiler supports variadic templates
// FP_COMPOUND - Whether the compiler properly compiles compound composition

#if defined(_MSC_VER)
#define FP_DECLVAL  0
#define FP_VARIADIC 0
#define FP_COMPOUND 1
#else
#define FP_DECLVAL  1
#define FP_VARIADIC 0
#define FP_COMPOUND 0
#endif

// Composition operator defines
#if !defined(FP_OPERATORS)
#define FP_OPERATORS 1
#endif
#if FP_OPERATORS
#define FP_PLUS_OPERATOR 1
#define FP_DOT_OPERATOR 1
#else
#define FP_PLUS_OPERATOR 0
#define FP_DOT_OPERATOR 0
#endif

// Keywords
#define let auto
#define extent(c)  fp::head((c)),  fp::tail((c))
#define rextent(c) fp::rhead((c)), fp::rtail((c))

// Template helpers
#define value_type_of(TYPE)            typename fp::traits<TYPE>::value_type
#define result_type_of(TYPE)           typename fp::function_traits<TYPE>::result_type
#define argument_type_of(TYPE,ARGC)    typename fp::function_traits<TYPE>::t ## ARGC ## _type
#define nonconstref_type_of(TYPE)      typename fp::remove_const_ref<TYPE>::type
#define fp_enable_if(CONDITION,RTYPE)  typename std::enable_if<CONDITION,RTYPE>::type
#define fp_enable_if_container(TYPE,RTYPE)     fp_enable_if(fp::is_container<TYPE>::value, RTYPE)
#define fp_enable_if_not_container(TYPE,RTYPE) fp_enable_if(!fp::is_container<TYPE>::value, RTYPE)
#define fp_enable_if_void(TYPE)                fp_enable_if(std::is_same<void,TYPE>::value,  void)
#define fp_enable_if_nonvoid(TYPE,RTYPE)       fp_enable_if(!std::is_same<void,TYPE>::value, RTYPE)

// Join helpers
#define FP_CONCAT_IMPL(x, y) x ## y
#define FP_CONCAT(x, y) FP_CONCAT_IMPL(x,y)

// auto function(<arguments>) RETURNS(<some-expression>);
#define FP_RETURNS(...) -> decltype(__VA_ARGS__) { return (__VA_ARGS__); } typedef int FP_CONCAT(RETURNS_, __LINE__)
#define FP_RETURNS_NONREF(...) -> nonconstref_type_of(decltype(__VA_ARGS__)) { return (__VA_ARGS__); } typedef int FP_CONCAT(RETURNS_, __LINE__)

#endif /* _FP_DEFINES_H_ */

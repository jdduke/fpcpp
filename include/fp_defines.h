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
#define FP_INITIALIZER 0
#define FP_VARIADIC 0
#define FP_THIS_IN_RET 1
#if _MSC_VER >= 1700
#define FP_DECLVAL  1
#define FP_NOEXCEPT noexcept
#else
#define FP_DECLVAL  1
#define FP_NOEXCEPT noexcept
#endif
#elif defined(__INTEL_COMPILER)
#define FP_INITIALIZER 1
#define FP_DECLVAL     0
#define FP_VARIADIC    1
#define FP_THIS_IN_RET 1
#define FP_NOEXCEPT noexcept
else
#define FP_INITIALIZER 1
#define FP_DECLVAL     1
#define FP_VARIADIC    1
#define FP_THIS_IN_RET 0
#define FP_NOEXCEPT noexcept
#endif

#define USE_DEQUE_FOR_LISTS 0
#if USE_DEQUE_FOR_LISTS
#define fp_list    std::deque
#else
#define fp_list    std::vector
//#define fp_list   std::list
#endif

// Composition operator defines
#if !defined(FP_OPERATORS)
#define FP_OPERATORS 1
#endif
#if FP_OPERATORS
#define FP_PLUS_OPERATOR  1
#define FP_DOT_OPERATOR   1
#define FP_SHIFT_OPERATOR 1
#else
#define FP_PLUS_OPERATOR  0
#define FP_DOT_OPERATOR   0
#define FP_SHIFT_OPERATOR 0
#endif

// Keywords
#define let auto
#define extent(c)  fp::begin((c)),  fp::end((c))
#define rextent(c) fp::rbegin((c)), fp::rend((c))

// Template type helpers
#define value_type_of(TYPE)            typename fp::traits<TYPE>::value_type
#define result_type_of(TYPE)           typename fp::function_traits<TYPE>::result_type
#define argument_type_of(TYPE,ARGC)    typename fp::function_traits<TYPE>::t ## ARGC ## _type
#define nonconstref_type_of(TYPE)      typename fp::remove_const_ref<TYPE>::type
#define ref_type_of(TYPE)              typename fp::traits<TYPE>::ref_type

// Template conditional type helpers
#define fp_enable_if(CONDITION,RTYPE)          typename std::enable_if<CONDITION,RTYPE>::type
#define fp_enable_if_container(TYPE,RTYPE)     fp_enable_if(fp::is_container<TYPE>::value, RTYPE)
#define fp_enable_if_not_container(TYPE,RTYPE) fp_enable_if(!fp::is_container<TYPE>::value, RTYPE)
#define fp_enable_if_void(TYPE)                fp_enable_if(std::is_same<void,TYPE>::value,  void)
#define fp_enable_if_nonvoid(TYPE,RTYPE)       fp_enable_if(!std::is_same<void,TYPE>::value, RTYPE)
#define fp_enable_if_arithmetic(TYPE)          fp_enable_if(std::is_arithmetic<TYPE>::value, TYPE)
#define fp_enable_if_integral(TYPE)            fp_enable_if(std::is_integral<TYPE>::value, TYPE)

// Join helpers
#define FP_CONCAT_IMPL(x, y) x ## y
#define FP_CONCAT(x, y) FP_CONCAT_IMPL(x,y)

// auto function(<arguments>) RETURNS(<single-lineexpression>);
#define FP_RETURNS(...)        -> decltype(__VA_ARGS__) { return (__VA_ARGS__); }                      \
  typedef int FP_CONCAT(RETURNS_ON_, FP_CONCAT(__LINE__,__COUNTER__))
#define FP_RETURNS_NONREF(...) -> nonconstref_type_of(decltype(__VA_ARGS__)) { return (__VA_ARGS__); } \
  typedef int FP_CONCAT(RETURNS_ON_, FP_CONCAT(__LINE__,__COUNTER__))

#endif /* _FP_DEFINES_H_ */

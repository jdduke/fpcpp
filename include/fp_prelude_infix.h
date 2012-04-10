/////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2012, Jared Duke.
// This code is released under the MIT License.
// www.opensource.org/licenses/mit-license.php
/////////////////////////////////////////////////////////////////////////////

#ifndef _FP_PRELUDE_INFIX_H_
#define _FP_PRELUDE_INFIX_H_

#include "fpcpp.h"

#define FP_DEFINE_FUNCTION_INFIX_HELPER(func)                        \
  static struct FP_CONCAT(__infixHelper,func) { } FP_CONCAT(func,_i); \
  template <typename T> struct FP_CONCAT(func, __Value) {            \
    FP_CONCAT(func, __Value) (T t) : mValue(t) { }                   \
    T mValue;                                                        \
  };

#define FP_DEFINE_FUNCTION_INFIX_HELPER_OPERATORS(func)                                            \
  template <typename T0, typename T1>                                                              \
  inline auto operator>(FP_CONCAT(func,__Value) <T0> t0, T1 t1) FP_RETURNS( func(t0.mValue, t1) ); \
  template <typename T0>                                                                           \
  inline auto operator<(T0 t0, FP_CONCAT(__infixHelper,func)) FP_RETURNS( FP_CONCAT(func,__Value) <T0>(t0) );


#define FP_DEFINE_FUNCTION_INFIX(func)           \
  FP_DEFINE_FUNCTION_INFIX_HELPER(func)          \
  FP_DEFINE_FUNCTION_INFIX_HELPER_OPERATORS(func)

namespace fp {

///////////////////////////////////////////////////////////////////////////
// common

///////////////////////////////////////////////////////////////////////////
// prelude

///////////////////////////////////////////////////////////////////////////
// math

namespace math {

FP_DEFINE_FUNCTION_INFIX(add);
FP_DEFINE_FUNCTION_INFIX(subtract);
FP_DEFINE_FUNCTION_INFIX(divide);
FP_DEFINE_FUNCTION_INFIX(multiply);
FP_DEFINE_FUNCTION_INFIX(mod);
FP_DEFINE_FUNCTION_INFIX(logicalAnd);
FP_DEFINE_FUNCTION_INFIX(logicalOr);
FP_DEFINE_FUNCTION_INFIX(logicalNot);
FP_DEFINE_FUNCTION_INFIX(equals);
FP_DEFINE_FUNCTION_INFIX(notEquals);
FP_DEFINE_FUNCTION_INFIX(greaterEquals);
FP_DEFINE_FUNCTION_INFIX(greater);
FP_DEFINE_FUNCTION_INFIX(lessEquals);
FP_DEFINE_FUNCTION_INFIX(less);
}

} /* namespace fp */

#undef FP_DEFINE_FUNCTION_INFIX_HELPER_OPERATORS
#undef FP_DEFINE_FUNCTION_INFIX_HELPER
#undef FP_DEFINE_FUNCTION_INFIX

#endif /* _FP_PRELUDE_INFIX_H_ */

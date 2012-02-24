/////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2012, Jared Duke.
// This code is released under the MIT License.
// www.opensource.org/licenses/mit-license.php
/////////////////////////////////////////////////////////////////////////////

#ifndef _FP_PRELUDE_MATH_H_
#define _FP_PRELUDE_MATH_H_

#include "fp_defines.h"

#define _USE_MATH_DEFINES
#include <math.h>
#undef  _USE_MATH_DEFINES

namespace fp {
namespace math {

///////////////////////////////////////////////////////////////////////////
// Arithmetic operations

template<typename T>
inline T add(const T& t0, const T& t1) {
  return t0 + t1;
}

template<typename T>
inline T subtract(const T& t0, const T& t1) {
  return t0 - t1;
}

template<typename T>
inline T multiply(const T& t0, const T& t1) {
  return t0 * t1;
}

template<typename T>
inline T divide(const T& t0, const T& t1) {
  return t0 / t1;
}

template<typename T>
inline T negate(const T& t0) {
  return -t0;
}

template<typename T>
inline T mod(const T& t0, const T& t1) {
  return t0 % t1;
}

///////////////////////////////////////////////////////////////////////////
// Logical operations

template<typename T>
inline T logicalAnd(const T& t0, const T& t1) {
  return t0 && t1;
}

template<typename T>
inline T logicalOr(const T& t0, const T& t1) {
  return t0 || t1;
}

template<typename T>
inline T logicalNot(const T& t0) {
  return !t0;
}


///////////////////////////////////////////////////////////////////////////
// Comparison operations

template<typename T>
inline T equals(const T& t0, const T& t1) {
  return t0 == t1;
}

template<typename T>
inline T notEquals(const T& t0, const T& t1) {
  return !equals(t0,t1);
}

template<typename T>
inline T greaterEquals(const T& t0, const T& t1) {
  return t0 >= t1;
}

template<typename T>
inline T lessEquals(const T& t0, const T& t1) {
  return t0 <= t1;
}

template<typename T>
inline T less(const T& t0, const T& t1) {
  return t0 < t1;
}

template<typename T>
inline T greater(const T& t0, const T& t1) {
  return t0 > t1;
}


} /* namespace math */
} /* namespace fp   */

#endif /* _FP_PRELUDE_MATH_H_ */

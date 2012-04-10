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

#if defined(_MSC_VER)
#include <random>
#else
#include <tr1/random>
#endif

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

template<typename T>
inline fp_enable_if_arithmetic(T) sqrt(const T& t) {
  return std::sqrt(t);
}

template<typename T>
inline fp_enable_if_arithmetic(T) abs(const T& t0) {
  return std::abs(t0);
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


#if defined(_MSC_VER)
using std::uniform_real;
using std::uniform_int;
using std::mt19937;
using std::random_device;
#else
using std::tr1::uniform_real;
using std::tr1::uniform_int;
using std::tr1::mt19937;
using std::tr1::random_device;
#endif

// ///////////////////////////////////////////////////////////////////////////
// uniform

#if !defined(__MINGW32__)

typedef mt19937 uniform_gen;

template<typename T>
inline typename std::enable_if< std::is_floating_point<T>::value, T>::type
  uniform(T t0, T t1) {
    static uniform_gen generator;
    return uniform_real<T>(t0, t1)(generator);
}

template<typename T>
inline typename std::enable_if< !std::is_floating_point<T>::value, T>::type
  uniform(T t0, T t1) {
    static uniform_gen generator;
    return uniform_int<T>(t0, t1)(generator);
}

template<typename T>
struct uniform_ {
  uniform_(T t0, T t1, unsigned long long seed)
    : mUniform(t0, t1), mGenerator(seed) { }
  uniform_(T t0, T t1)
    : mUniform(t0, t1), mGenerator(random_device()()) { }

  T operator()() const { return mUniform(mGenerator); }

  uniform_real<T> mUniform;
  mutable uniform_gen mGenerator;
};

#else

template<typename T>
inline T uniform(T t0, T t1) {
  return (T)(t0 + static_cast<double>(rand())/((unsigned long long)RAND_MAX+1) * (t1 - t0));
}
template<typename T>
struct uniform_ {
  uniform_(T t0_, T t1_) : t0(t0_), t1(t1_) { }
  T operator()() { return uniform<T>(t0,t1); }
  T t0, t1;
};

#endif

} /* namespace math */
} /* namespace fp   */

#endif /* _FP_PRELUDE_MATH_H_ */

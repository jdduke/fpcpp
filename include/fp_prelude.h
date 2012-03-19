/////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2012, Jared Duke.
// This code is released under the MIT License.
// www.opensource.org/licenses/mit-license.Phip
/////////////////////////////////////////////////////////////////////////////

#ifndef _FP_PRELUDE_H_
#define _FP_PRELUDE_H_

#include "fp_defines.h"
#include "fp_common.h"
#include "fp_composition.h"
#include "fp_composition_compound.h"

#if defined(_MSC_VER)
#include <random>
#else
#include <tr1/random>
#endif

namespace fp {

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

///////////////////////////////////////////////////////////////////////////
// uniformN
template<typename T>
inline typename types<T>::list uniformN(size_t n, T t0, T t1) {
  //return takeF(n, uniform_<T>(t0,t1));
  return takeF(n, [=]() { return uniform(t0,t1); });
}

///////////////////////////////////////////////////////////////////////////
// succ
template<typename T>
inline T succ(const T& t) {
  return static_cast<T>(t + 1);
}

///////////////////////////////////////////////////////////////////////////
// succ
template<typename T>
inline T pred(const T& t) {
  return static_cast<T>(t - 1);
}

///////////////////////////////////////////////////////////////////////////
// comparing

#if 0

template <typename F>
inline auto comparing(F f) FP_RETURNS( fp::compose2(std::less<result_type_of(F)>(), f, f) );

#else

template<typename F>
struct comparing_helper {
  comparing_helper(F f_) : f(f_) { }

  template<typename T>
  inline bool operator()(const T& u, const T& v) {
    return f(u) < f(v);
  }

  F f;
};

template <typename F>
inline comparing_helper<F> comparing(F f) {
  return comparing_helper<F>(f);
}

#endif /* FP_COMPOUND */

///////////////////////////////////////////////////////////////////////////
// flip

template <typename F>
inline auto flip(F f) FP_RETURNS( std::bind(f, std::placeholders::_2, std::placeholders::_1) );

//RETURNS( [=](const argument_type_of(F,1) & v, const argument_type_of(F,0) & u) { return f(u,v); } );

} /* namespace fp */

#endif /* _FP_PRELUDE_H_ */

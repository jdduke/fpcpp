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

#include <limits>
#include <random>

namespace fp {

// ///////////////////////////////////////////////////////////////////////////
// uniform

typedef std::mt19937 uniform_gen;

template<typename T>
inline typename std::enable_if< std::is_floating_point<T>::value, T>::type
uniform(T t0, T t1) {
  static uniform_gen generator;
  return std::uniform_real<T>(t0, t1)(generator);
}

template<typename T>
inline typename std::enable_if< !std::is_floating_point<T>::value, T>::type
uniform(T t0, T t1) {
  static uniform_gen generator;
  return std::uniform_int<T>(t0, t1)(generator);
}

template<typename T>
struct uniform_ {
  uniform_(T t0, T t1, unsigned long long seed) 
    : mUniform(t0, t1), mGenerator(seed) { }
  uniform_(T t0, T t1) 
    : mUniform(t0, t1), mGenerator(uniform(uniform_gen::default_seed/4, uniform_gen::default_seed)) { }
  T operator()() { return mUniform(mGenerator); }
  std::uniform_real<T> mUniform;
  uniform_gen mGenerator;
};

///////////////////////////////////////////////////////////////////////////
// uniformN
template<typename T>
inline typename types<T>::list uniformN(size_t n, T t0, T t1) {
  return takeF(n, uniform_<T>(t0,t1));
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

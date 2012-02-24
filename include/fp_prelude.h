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

namespace fp {

// ///////////////////////////////////////////////////////////////////////////
// rand_range

template<typename T>
inline T rand_range(T t0 = std::numeric_limits<T>::min(), T t1 = std::numeric_limits<T>::max()) {
  return (T)(t0 + static_cast<double>(rand())/((unsigned long long)RAND_MAX+1) * (t1 - t0));
}
template<typename T>
struct rand_range_ {
  rand_range_(T t0_ = std::numeric_limits<T>::min(), T t1_ = std::numeric_limits<T>::max()) : t0(t0_), t1(t1_) { }
  T operator()() { return rand_range<T>(t0,t1); }
  T t0, t1;
};

///////////////////////////////////////////////////////////////////////////
// rand_n
template<typename T>
inline typename types<T>::list rand_n(size_t n, T t0 = std::numeric_limits<T>::min(), T t1 = std::numeric_limits<T>::max()) {
  return generate_n(n, rand_range_<T>(t0,t1));
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

#if FP_COMPOUND

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

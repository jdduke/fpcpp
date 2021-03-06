/////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2012, Jared Duke.
// This code is released under the MIT License.
// www.opensource.org/licenses/mit-license.php
/////////////////////////////////////////////////////////////////////////////

#ifndef _FP_COMPOSITION_COMPOUND_H_
#define _FP_COMPOSITION_COMPOUND_H_

#include "fp_defines.h"

namespace fp {

///////////////////////////////////////////////////////////////////////////

template <typename F, typename G0, typename G1>
class composed2 : public composed<F,G0> {
public:
  typedef composed<F,G0>     parent_type;
  typedef composed2<F,G0,G1> this_type;

  explicit composed2(F f_,   G0   g_, G1   g1_)
      : composed<F,G0>(f_,g_), g1(g1_) { }
  explicit composed2(F&& f_, G0&& g_, G1&& g1_)
      : composed<F,G0>(std::forward(f_),std::forward(g_)), g1(std::forward(g1_)) { }

  template <typename T0, typename T1>
  inline auto operator()(const T0& t0, const T1& t1)
      -> decltype(declval<F>()(declval<G0>()(declval<T0>()), declval<G1>()(declval<T1>()))) {
    return this->f(this->g(t0), this->g1(t1));
  }

protected:
  composed2();

  G1 g1;
};


///////////////////////////////////////////////////////////////////////////

template <typename F, typename G0, typename G1>
inline composed2<F,G0,G1> compose2(F f, G0 g0, G1 g1) {
  return composed2<F,G0,G1>(f, g0, g1);
}

///////////////////////////////////////////////////////////////////////////

} /* namespace fp */

#endif /* _FP_COMPOSITION_COMPOUND_H_ */

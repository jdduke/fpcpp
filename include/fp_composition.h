/////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2012, Jared Duke.
// This code is released under the MIT License.
// www.opensource.org/licenses/mit-license.php
/////////////////////////////////////////////////////////////////////////////

#ifndef _FP_COMPOSITION_H_
#define _FP_COMPOSITION_H_

#include "fp_defines.h"
#include "fp_composition_utils.h"

namespace fp {

///////////////////////////////////////////////////////////////////////////

using std::declval;

template<typename F, typename G>
class composed {
public:

  composed(F f_,   G g_)   : f(f_), g(g_) { }
  composed(F&& f_, G&& g_) : f(std::move(f_)), g(std::move(g_)) { }

#if FP_VARIADIC
  template<typename... Args>
  inline auto operator()(const Args&... args) -> decltype(declval<F>()(declval<G>()(args...))){
    return f(g(args...));
  }
#else

  template<typename T0>
  inline auto operator()(const T0& t0) -> decltype( declval<F>()(declval<G>()(t0)) ) {
    return f(this->g(t0));
  }

  template<typename T0, typename T1>
  inline auto operator()(const T0& t0, const T1& t1) -> decltype( declval<F>()(declval<G>()(t0,t1)) ) {
    return f(g(t0,t1));
  }

  template<typename T0, typename T1, typename T2>
  inline auto operator()(const T0& t0, const T1& t1, const T2& t2) -> decltype( declval<F>()(declval<G>()(t0,t1,t2)) ) {
    return f(g(t0,t1,t2));
  }

  template<typename T0, typename T1, typename T2, typename T3>
  inline auto operator()(const T0& t0, const T1& t1, const T2& t2, const T3& t3) -> decltype(  declval<F>()(declval<G>()(t0,t1,t2,t3)) ) {
    return f(g(t0,t1,t2,t3));
  }

#endif

protected:
  composed();

  F f;
  G g;
};

///////////////////////////////////////////////////////////////////////////

template <typename F, typename G>
inline composed<F,G> compose(F f, G g) {
  return composed<F,G>(f,g);
}

///////////////////////////////////////////////////////////////////////////

} /* namespace fp */

namespace fp_operators {
#if FP_PLUS_OPERATOR
  // Example: (f + g)(Args) == (compose(f,g)(Args)
  template<typename F, typename G>
  inline auto operator+(F f, G g) FP_RETURNS( fp::compose(f,g) );
#endif

#if FP_DOT_OPERATOR
  // Example: (f <o> g)(Args) == (compose(f,g)(args))
  static class compose_dot_helper { } o;
  template<typename F, typename G>
  inline auto operator>(F f, G g) FP_RETURNS( fp::compose(f,g) );
  template<typename F>
  inline F operator<(F f, compose_dot_helper) {
    return f;
  }
#endif

#if FP_SHIFT_OPERATOR
  template<typename F, typename G>
  inline auto operator <<(F f, G g) -> typename std::enable_if<fp::is_functor<F>::value&&fp::is_functor<G>::value, decltype( fp::compose(f,g) )>::type {
    return fp::compose(f,g);
  }
  template<typename F, typename G>
  inline auto operator >>(G g, F f) FP_RETURNS( f << g );
#endif

} /* namespace fp_operators */

#endif /* _FP_COMPOSITION_H_ */

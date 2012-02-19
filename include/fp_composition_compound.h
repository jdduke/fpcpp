/////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2012, Jared Duke.
// This code is released under the MIT License.
// www.opensource.org/licenses/mit-license.php
/////////////////////////////////////////////////////////////////////////////

#if FP_COMPOUND

#ifndef _FP_COMPOSITION_COMPOUND_H_
#define _FP_COMPOSITION_COMPOUND_H_

#include "fp_defines.h"
#include "fp_composition_compound_utils.h"

namespace fp {

///////////////////////////////////////////////////////////////////////////

template <typename F, typename G0, typename G1>
inline auto compose2(F f, G0 g0, G1 g1) -> typename composed_traits2<F,G0,G1>::type {
  return typename composed_traits2<F,G0,G1>::type(f, g0, g1);
}

template <typename F, typename G0, typename G1, typename G2>
inline auto compose3(F f, G0 g0, G1 g1, G2 g2) -> typename composed_traits3<F,G0,G1,G2>::type {
  return typename composed_traits3<F,G0,G1,G2>::type(f, g0, g1, g2);
}

///////////////////////////////////////////////////////////////////////////

template <typename F, typename G0, typename G1>
class composed_base2 : public composed_base<F,G0> {
public:
  typedef composed_base<F,G0> parent_type;

  static const size_t arity_0 = function_traits<G0>::arity;
  static const size_t arity_1 = function_traits<G1>::arity;

  template <typename R, typename Args>
  R applyToTuple(const Args& args) {
    return f(apply_func<G0,Args,0,      arity_0>(this->g,args),
             apply_func<G1,Args,arity_0,arity_1>(g1,     args));
  }

#if FP_VARIADIC

  template <typename... Args>
  auto operator()(Args... args) -> typename compound_result2<F,G0,G1,Args...>::type {
    return apply<typename compound_result2<F,G0,G1,Args...>::type>(std::make_tuple(args...));
  }

#else

  template <typename T0>
  auto operator()(const T0& t0) -> typename compound_result21<F,G0,G1,T0>::type {
    typedef typename compound_result21<F,G0,G1,T0>::type result_type;
    return applyToTuple<result_type>(std::make_tuple(t0));
  }

  template <typename T0, typename T1>
  auto operator()(const T0& t0, const T1& t1) -> typename compound_result22<F,G0,G1,T0,T1>::type {
    typedef typename compound_result22<F,G0,G1,T0,T1>::type result_type;
    return applyToTuple<result_type>(std::make_tuple(t0,t1));
  }

  template <typename T0, typename T1, typename T2>
  auto operator()(const T0& t0, const T1& t1, const T2& t2) -> typename compound_result23<F,G0,G1,T0,T1,T2>::type {
    typedef typename compound_result23<F,G0,G1,T0,T1,T2>::type result_type;
    return applyToTuple<result_type>(std::make_tuple(t0,t1,t2));
  }

  template <typename T0, typename T1, typename T2, typename T3>
  auto operator()(const T0& t0, const T1& t1, const T2& t2, const T3& t3) -> typename compound_result24<F,G0,G1,T0,T1,T2,T3>::type {
    typedef typename compound_result24<F,G0,G1,T0,T1,T2,T3>::type result_type;
    return applyToTuple<result_type>(std::make_tuple(t0,t1,t2,t3));
  }

#endif /* FP_VARIADIC */

protected:

  explicit composed_base2(F f_,   G0   g_, G1   g1_) : composed_base<F,G0>(f_,g_), g1(g1_) { }
  explicit composed_base2(F&& f_, G0&& g_, G1&& g1_) : composed_base<F,G0>(f_,g_), g1(std::forward(g1_)) { }
  composed_base2();

  G1 g1;
};


///////////////////////////////////////////////////////////////////////////

template <typename F, typename G0, typename G1, size_t ArgGC>
class composed2 : public composed_base2<F,G0,G1> {
public:
  composed2(F f_,   G0   g_, G1   g1_) : composed_base2<F,G0,G1>(f_,g_,g1_) { }
  composed2(F&& f_, G0&& g_, G1&& g1_) : composed_base2<F,G0,G1>(std::forward(f_),std::forward(g_),std::forward(g1_)) { }
};

template <typename F, typename G0, typename G1>
class composed2<F,G0,G1,0> : public composed_base2<F,G0,G1> {
public:
  composed2(F f_,   G0   g_, G1   g1_) : composed_base2<F,G0,G1>(f_,g_,g1_) { }
  composed2(F&& f_, G0&& g_, G1&& g1_) : composed_base2<F,G0,G1>(std::forward(f_),std::forward(g_),std::forward(g1_)) { }

  inline auto operator()() -> typename compound_result20<F,G0,G1>::type { return f(g(),g1()); }
};

///////////////////////////////////////////////////////////////////////////

template<typename F, typename G0, typename G1, typename G2>
class composed_base3 : public composed_base2<F,G0,G1> {
public:
  typedef composed_base2<F,G0,G1> parent_type;

  static const size_t arity_0 = parent_type::arity_0;
  static const size_t arity_1 = parent_type::arity_1;
  static const size_t arity_2 = function_traits<G2>::arity;

  template<typename R, typename Args>
  R applyToTuple(const Args& args) {
    return f(apply_func<G0,Args,0,              arity_0>(this->g, args),
             apply_func<G1,Args,arity_0,        arity_1>(this->g1,args),
             apply_func<G2,Args,arity_0+arity_1,arity_2>(this->g2,args));
  }


#if FP_VARIADIC

  template<typename... Args>
  auto operator()(Args... args) -> typename compound_result3<F,G0,G1,G2,Args...>::type {
    return apply<typename compound_result3<F,G0,G1,G2,Args...>::type>(std::make_tuple(args...));
  }

#else

  template <typename T0>
  auto operator()(const T0& t0) -> typename compound_result31<F,G0,G1,G2,T0>::type {
    typedef typename compound_result31<F,G0,G1,G2,T0>::type result_type;
    return applyToTuple<result_type>(std::make_tuple(t0));
  }

  template<typename T0, typename T1>
  auto operator()(const T0& t0, const T1& t1) -> typename compound_result32<F,G0,G1,G2,T0,T1>::type {
    typedef typename compound_result32<F,G0,G1,G2,T0,T1>::type result_type;
    return applyToTuple<result_type>(std::make_tuple(t0,t1));
  }

  template<typename T0, typename T1, typename T2>
  auto operator()(const T0& t0, const T1& t1, const T2& t2) -> typename compound_result33<F,G0,G1,G2,T0,T1,T2>::type {
    typedef typename compound_result33<F,G0,G1,G2,T0,T1,T2>::type result_type;
    return applyToTuple<result_type>(std::make_tuple(t0,t1,t2));
  }

  template<typename T0, typename T1, typename T2, typename T3>
  auto operator()(const T0& t0, const T1& t1, const T2& t2, const T3& t3) -> typename compound_result34<F,G0,G1,G2,T0,T1,T2,T3>::type {
    typedef typename compound_result34<F,G0,G1,G2,T0,T1,T2,T3>::type result_type;
    return applyToTuple<result_type>(std::make_tuple(t0,t1,t2,t3));
  }

#endif /* FP_VARIADIC */

protected:

  explicit composed_base3(F f_,   G0   g_, G1   g1_, G2   g2_) : composed_base2<F,G0,G1>(f_,g_,g1_), g2(g2_) { }
  explicit composed_base3(F&& f_, G0&& g_, G1&& g1_, G1&& g2_) : composed_base2<F,G0,G1>(f_,g_,g1_), g2(std::move(g2_)) { }
  composed_base3();

  G2 g2;
};

///////////////////////////////////////////////////////////////////////////

template<typename F, typename G0, typename G1, typename G2, size_t ArgGC>
class composed3 : public composed_base3<F,G0,G1,G2> {
public:
  composed3(F f_,   G0   g_, G1   g1_, G2   g2_) : composed_base3<F,G0,G1,G2>(f_,g_,g1_,g2_) { }
  composed3(F&& f_, G0&& g_, G1&& g1_, G2&& g2_) : composed_base3<F,G0,G1,G2>(std::forward(f_),std::forward(g_), std::forward(g1_), std::forward(g2_)) { }
};

template<typename F, typename G0, typename G1, typename G2>
class composed3<F,G0,G1,G2,0> : public composed_base3<F,G0,G1,G2> {
public:
  composed3(F f_,   G0   g_, G1   g1_, G2   g2_) : composed_base3<F,G0,G1,G2>(f_,g_,g1_,g2_) { }
  composed3(F&& f_, G0&& g_, G1&& g1_, G2&& g2_) : composed_base3<F,G0,G1,G2>(std::forward(f_),std::forward(g_), std::forward(g1_), std::forward(g2_)) { }

  inline auto operator()() -> typename compound_result30<F,G0,G1,G2>::type { return f(g(),g1(),g2()); }
};

} /* namespace fp */

#endif /* _FP_COMPOSITION_COMPOUND_H_ */

#endif /* FP_COMPOUND */

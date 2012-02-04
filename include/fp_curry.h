#ifndef _FP_CURRY_H_
#define _FP_CURRY_H_

#include <fc_utils.h>
#include <fp_curry_defines.h>

namespace fp {

template<size_t FArgC> struct curry_helper_impl { };

FP_CURRY_DEFINE_HELPER(1)
FP_CURRY_DEFINE_HELPER(2)
FP_CURRY_DEFINE_HELPER(3)
FP_CURRY_DEFINE_HELPER(4)
FP_CURRY_DEFINE_HELPER(5)

template<typename F> struct curry_helper : public curry_helper_impl<fc::function_traits<F>::arity> { };

template<typename F, typename T>
inline auto curry(F f, T t) -> decltype( curry_helper<F>::bind(f,t) ) {
  return curry_helper<F>::bind(f, t);
}

template<typename F, typename T, typename T1>
inline auto curry2(F f, T t, T1 t1) -> decltype( curry_helper<F>::bind(f,t,t1) ) {
  return curry_helper<F>::bind2(f, t, t1);
}

template<typename F, typename T, typename T1, typename T2>
inline auto curry3(F f, T t, T1 t1, T2 t2) -> decltype( curry_helper<F>::bind(f,t,t1,t2) ) {
  return curry_helper<F>::bind2(f, t, t1, t2);
}

template<typename F, typename T, typename T1, typename T2, typename T3>
inline auto curry4(F f, T t, T1 t1, T2 t2, T3 t3) -> decltype( curry_helper<F>::bind(f,t,t1,t2,t3) ) {
  return curry_helper<F>::bind2(f, t, t1, t2, t3);
}

} /* namespace fp */

#endif /* _FP_CURRY_H_ */

#ifndef _FP_CURRY_H_
#define _FP_CURRY_H_

#include <fc_utils.h>

namespace fp {

#define FP_PREFIX       f, t
#define FP_PREFIX2      f, t, t1
#define FP_PREFIX3      f, t, t1, t2
#define FP_PREFIX4      f, t, t1, t2, t3
#define FP_PREFIX_DECL std::declval<F>(), std::declval<T>()

#define FP_CURRY1(a) std::bind(a)
#define FP_CURRY2(a) std::bind(a, std::placeholders::_1)
#define FP_CURRY3(a) std::bind(a, std::placeholders::_1, std::placeholders::_2)
#define FP_CURRY4(a) std::bind(a, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)
#define FP_CURRY5(a) std::bind(a, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4)

#define FP_CURRY_HELPER(FUNC,PREFIX) FUNC(PREFIX)
#define FP_CURRY(ARG,PREFIX) FP_CURRY_HELPER(FP_CURRY##ARG, PREFIX)

#define FP_CURRY_DEFINE_HELPER(ARGC) \
  template<typename F, typename T> struct curry_helper_impl<F,T,ARGC> {           \
    typedef decltype( FP_CURRY(ARGC,FP_PREFIX_DECL) ) result_type;           \
    static inline result_type bind(F f, T t) {                               \
      return FP_CURRY(ARGC,FP_PREFIX);                                       \
    }                                                                        \
  };

template<typename F, typename T, size_t FArgC>
struct curry_helper_impl { };
FP_CURRY_DEFINE_HELPER(1)
FP_CURRY_DEFINE_HELPER(2)
FP_CURRY_DEFINE_HELPER(3)
FP_CURRY_DEFINE_HELPER(4)
FP_CURRY_DEFINE_HELPER(5)

template<typename F, typename T> struct curry_helper 
  : public curry_helper_impl<F,T,fc::function_traits<F>::arity> {
  typedef typename curry_helper_impl::result_type result_type;
};

template<typename F, typename T>
inline auto curry(F f, T t) -> typename curry_helper<F,T>::result_type {
  return curry_helper<F,T>::bind(f, t);
}

template<typename F, typename T, typename T2>
inline auto curry2(F f, T t, T2 t2) -> decltype( std::bind(curry(f,t), _1, t2) ) {
  return std::bind(curry(f,t), _1, t2);
}

} /* namespace fp */

#endif /* _FP_CURRY_H_ */

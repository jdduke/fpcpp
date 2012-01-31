#ifndef _FP_CURRY_H_
#define _FP_CURRY_H_

namespace fp {

namespace curry {

template<typename F, typename Arg, typename P>
inline auto curry(F f, Arg arg, P p) -> decltype( std::bind(f,arg,p) ){
  return std::bind(f, arg, p);
}

} /* namespace curry */

} /* namespace fp */

#endif /* _FP_CURRY_H_ */

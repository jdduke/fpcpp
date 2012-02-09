/////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2012, Jared Duke.
// This code is released under the MIT License.
// www.opensource.org/licenses/mit-license.php
/////////////////////////////////////////////////////////////////////////////

#ifndef _FP_CURRY_DEFINES_H_
#define _FP_CURRY_DEFINES_H_

#define FP_PREFIX  f, t
#define FP_PREFIX2 f, t, t1
#define FP_PREFIX3 f, t, t1, t2
#define FP_PREFIX4 f, t, t1, t2, t3

#define FP_ARGCM1 0
#define FP_ARGCM2 1
#define FP_ARGCM3 2
#define FP_ARGCM4 3
#define FP_ARGCM(ARGC) FP_ARGCM##ARGC
#define FP_ARGCMM(ARGC) FP_ARGCM(FP_ARGCM##ARGC)
#define FP_ARGCMMM(ARGC) FP_ARGCMM(FP_ARGCM##ARGC)

#define FP_ARGCP1 2
#define FP_ARGCP2 3
#define FP_ARGCP3 4
#define FP_ARGCP4 5
#define FP_ARGCP(ARGC) FP_ARGCP##ARGC
#define FP_ARGCPP(ARGC) FP_ARGCP(FP_ARGCP##ARGC)

#define FP_PREFIXM_IMPL(PREFIX,ARGC) PREFIX##ARGC
#define FP_PREFIXM(ARGC) FP_PREFIXM_IMPL(FP_PREFIXM##ARGC,ARGC)

#define FP_CURRY1(a) std::bind(a)
#define FP_CURRY2(a) std::bind(a, std::placeholders::_1)
#define FP_CURRY3(a) std::bind(a, std::placeholders::_1, std::placeholders::_2)
#define FP_CURRY4(a) std::bind(a, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)
#define FP_CURRY5(a) std::bind(a, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4)

#define FP_CURRY_HELPER(FUNC,PREFIX) FUNC(PREFIX)
#define FP_CURRY(ARG,PREFIX)         FP_CURRY_HELPER(FP_CURRY##ARG, PREFIX)

#define FP_CURRY_DEFINE_HELPER(ARGC)                                                                                \
  template<> struct curry_helper_impl<ARGC> {                                                                       \
    template<typename F,typename T>                                                                                 \
    static inline auto bind(F f, T t) -> decltype( FP_CURRY(ARGC,FP_PREFIX) ) {                                     \
      return FP_CURRY(ARGC,FP_PREFIX);                                                                              \
    }                                                                                                               \
    template<typename F, typename T, typename T1>                                                                   \
    static inline auto bind2(F f, T t, T1 t1) -> decltype( FP_CURRY(FP_ARGCM(ARGC),FP_PREFIX2) ) {                  \
      return FP_CURRY(FP_ARGCM(ARGC),FP_PREFIX2);                                                                   \
    }                                                                                                               \
    template<typename F, typename T, typename T1, typename T2>                                                      \
    static inline auto bind3(F f, T t, T1 t1, T2 t2) -> decltype( FP_CURRY(FP_ARGCMM(ARGC),FP_PREFIX3) ) {          \
      return FP_CURRY(FP_ARGCMM(ARGC),FP_PREFIX3);                                                                  \
    }                                                                                                               \
    template<typename F, typename T, typename T1, typename T2, typename T3>                                         \
    static inline auto bind4(F f, T t, T1 t1, T2 t2, T3 t3) -> decltype( FP_CURRY(FP_ARGCMMM(ARGC),FP_PREFIX4) ) {  \
      return FP_CURRY(FP_ARGCMMM(ARGC),FP_PREFIX4);                                                                 \
    }                                                                                                               \
  };

#endif /* _FP_CURRY_DEFINES_H_ */

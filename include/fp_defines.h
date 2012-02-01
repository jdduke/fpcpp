/////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2012, Jared Duke.
// This code is released under the MIT License.
// www.opensource.org/licenses/mit-license.php
/////////////////////////////////////////////////////////////////////////////

#ifndef _FP_DEFINES_H_
#define _FP_DEFINES_H_

#define let auto
#define T_VALUE typename traits<T>::value_type

#define FP_DEFINE_FUNC_OBJ_HELPER(funcName, funcObjName1, funcObjName2, returnType) \
  template <typename F>                                      \
  struct funcObjName1 {                                      \
    funcObjName1(F f_) : f(f_) { }                           \
    template<typename T>                                     \
    inline returnType operator()(const std::vector<T>& t) {  \
      return funcName(f,t);                                  \
    }                                                        \
  private:                                                   \
    F f;                                                     \
  };                                                         \
  template <typename F>                                      \
  inline funcObjName1<F> funcName ## _with (F f) {           \
    return funcObjName1<F>(f);                               \
  }                                                          
  
  
  /*
  template<typename T, typename T1>                        \
  inline returnType operator()(const T& t, const T1& t1) { \
  return funcName(f,t,t1);                               \
  }                                                        \
  /*\
  template<typename F, typename T>                           \
  struct funcObjName2 : funcObjName1<F> {                    \
    funcObjName2(F f_) : funcObjName1(f_) { }                \
    inline returnType operator()(const T& t) {               \
      return funcObjName1::operator()(t);                    \
    }                                                        \
    template <typename T1>                                   \
    inline returnType operator()(const T& t, const T1& t1) { \
      return funcObjName1::operator()(t,t1);                 \
    }                                                        \
};*/

#define FP_DEFINE_FUNC_OBJ(funcName, funcObjName1, funcObjName2) FP_DEFINE_FUNC_OBJ_HELPER(funcName, funcObjName1, funcObjName2, std::vector<T>)
#define FP_DEFINE_FUNC_OBJ_T(funcName, funcObjName1, funcObjName2) FP_DEFINE_FUNC_OBJ_HELPER(funcName, funcObjName1, funcObjName2, T)

#endif /* _FP_DEFINES_H_ */

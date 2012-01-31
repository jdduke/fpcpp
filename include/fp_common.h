#ifndef _FP_COMMON_H_
#define _FP_COMMON_H_

#include "fp_defines.h"

namespace fp {

template<typename C>
inline auto head(C& c) -> decltype(std::begin(c)){
  return std::begin(c);
}

template<typename C>
inline auto tail(C& c) -> decltype(std::begin(c)) {
  return std::end(c);
}

template<typename T, size_t S>
inline T* head(T (&A)[S]) {
  return (&A[0]);
}

template<typename T, size_t S>
inline T* tail(T (&A)[S]) {
 return (&A[0] + S);
}

template <typename T>
back_insert_iterator<T> back(T& t) {
  return std::back_inserter(t);
}

template <typename T>
std::front_insert_iterator<T> front(T& t) {
  return types<T>::back(t);
}

template <typename T>
struct traits {
  typedef typeanem T::value_type value;
  typedef typename T::iterator iterator;
  typedef typename T::const_iterator const_iterator;
};

template <typename T>
struct traits<T*> {
  typedef T value;
  typedef T* iterator;
  typedef const T* iterator;
}

template<typename C>
auto valueIn(typename traits<C>::iterator& i, const C& c) -> decltype(*i) {
  return (i != tail(c) ? *i : typename traits<C>::value() )
}

} /* namespace fp */

#endif /* _FP_COMMON_H_ */

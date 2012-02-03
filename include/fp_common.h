#ifndef _FP_COMMON_H_
#define _FP_COMMON_H_

#include "fp_defines.h"

namespace fp {

///////////////////////////////////////////////////////////////////////////

template<typename C>
inline auto head(C& c) -> decltype(std::begin(c)){
  return std::begin(c);
}
template<typename C>
inline auto chead(const C& c) -> decltype(c.begin()) {
  return c.begin();
}
template<typename C>
inline auto rhead(C& c) -> decltype(c.rbegin()) {
  return c.rbegin();
}

template<typename C>
inline auto tail(C& c) -> decltype(std::end(c)) {
  return std::end(c);
}
template<typename C>
inline auto ctail(const C& c) -> decltype(c.end()) {
  return c.end();
}
template<typename C>
inline auto rtail(C& c) -> decltype(c.rend()) {
  return c.rend();
}

/*
template<typename T, size_t S>
inline T* head(T (&A)[S]) {
  return (&A[0]);
}

template<typename T, size_t S>
inline T* tail(T (&A)[S]) {
 return (&A[0] + S);
}
*/

template <typename C>
std::back_insert_iterator<C> back(C& c) {
  return std::back_inserter(c);
}

template <typename C>
std::front_insert_iterator<C> front(C& c) {
  return std::front_inserter(c);
}

///////////////////////////////////////////////////////////////////////////

template <typename T>
struct remove_ref_const {
  typedef typename std::remove_const< typename std::remove_reference<T>::type >::type type;
};

///////////////////////////////////////////////////////////////////////////

template <typename T>
struct traits {
  typedef typename T::value_type value;
  typedef typename T::iterator iterator;
  typedef typename T::const_iterator const_iterator;
};
template <typename T, typename U>
struct tuple_traits {
  typedef typename traits<T>::value t;
  typedef typename traits<U>::value u;
  typedef std::pair<t,u> type;
};

template <typename T>
struct traits<T*> {
  typedef T value;
  typedef T* iterator;
  typedef const T* const_iterator;
};

///////////////////////////////////////////////////////////////////////////

template <typename T>
struct is_container {
  template <typename U, typename it_t = typename U::const_iterator > 
  struct sfinae  {
    template < typename U, typename IT, IT (U::*)() const, IT (U::*)() const >
    struct type_ {};
    typedef type_<U,it_t,static_cast<it_t (U::*)() const>(&U::begin),static_cast<it_t (U::*)() const>(&U::end)> type;
  };

  template <typename U> static char test(typename sfinae<U>::type*);
  template <typename U> static long test(...);

  enum { value = (1 == sizeof test<T>(0)) };
};

///////////////////////////////////////////////////////////////////////////

template <typename C>
auto valueIn(typename traits<C>::iterator& i, const C& c) -> decltype(*i) {
  return (i != tail(c) ? *i : typename traits<C>::value() )
}

} /* namespace fp */

#endif /* _FP_COMMON_H_ */
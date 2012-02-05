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
inline auto head(const C& c) -> decltype(std::begin(c)){
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
inline auto tail(const C& c) -> decltype(std::end(c)) {
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

template<typename T, size_t S>
inline T* head(T (&A)[S]) {
  return (&A[0]);
}
template<typename T, size_t S>
inline const T* head(const T (&A)[S]) {
  return (&A[0]);
}

template<typename T, size_t S>
inline T* tail(T (&A)[S]) {
 return (&A[0] + S);
}
template<typename T, size_t S>
inline const T* tail(const T (&A)[S]) {
  return (&A[0] + S);
}

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
struct remove_const_ref {
  typedef typename std::remove_const< typename std::remove_reference<T>::type >::type type;
};

///////////////////////////////////////////////////////////////////////////

template <typename T>
struct traits {
  typedef decltype( head( std::declval<T>() ) )       iterator;
  typedef decltype( head( std::declval<const T>() ) ) const_iterator;
  typedef typename remove_const_ref< decltype( *head( std::declval<T>() ) ) >::type value_type;
};

template <typename T, typename U>
struct tuple_traits {
  typedef typename traits<T>::value_type t;
  typedef typename traits<U>::value_type u;
  typedef std::pair<t,u> type;
};

template <typename T>
struct traits<T*> {
  typedef T        value_type;
  typedef T*       iterator;
  typedef const T* const_iterator;
};

///////////////////////////////////////////////////////////////////////////

template <typename T>
struct is_container {
  template <typename U, typename it_t = typename traits<U>::iterator > 
  struct sfinae  {
    template < typename U, typename IT, IT (U::*)() const, IT (U::*)() const >
    struct type_ {};
    typedef type_<U,it_t,static_cast<it_t (U::*)() const>(&U::begin),static_cast<it_t (U::*)() const>(&U::end)> type;
  };

  template <typename U> static char test(typename sfinae<U>::type*);
  template <typename U> static long test(...);

  enum { value = (1 == sizeof test<T>(0)) };
};

template <typename T, typename T2>
struct has_find {
  typedef char Yes[1];
  typedef char No[2];
  typedef typename traits<T>::iterator It;

  template <typename U, U> struct Check;

  template <typename C, typename C2> static Yes& Test(Check<It(C::*)(C2), &C::find>*);
  template <typename C, typename C2> static No&  Test(...);

  static const bool value = sizeof(Test<T,T2>(0)) == sizeof(Yes);
};

///////////////////////////////////////////////////////////////////////////

template <typename I, typename C>
auto iter_value(I i, const C& c) -> decltype(*i) {
  typedef decltype(*i) iter_value;
  return (i != tail(c)) ? *i : iter_value();
}

///////////////////////////////////////////////////////////////////////////
template<typename T, size_t S>
inline std::vector<T> make_vector(T (&A)[S]) {
  return std::vector<T>(A, A+S);
}


} /* namespace fp */

#endif /* _FP_COMMON_H_ */

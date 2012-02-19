/////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2012, Jared Duke.
// This code is released under the MIT License.
// www.opensource.org/licenses/mit-license.php
/////////////////////////////////////////////////////////////////////////////

#ifndef _FP_COMMON_H_
#define _FP_COMMON_H_

#include "fp_defines.h"

#include <array>
#include <iterator>
#include <tuple>
#include <vector>

namespace fp {

///////////////////////////////////////////////////////////////////////////

template <typename C>
inline auto head(C& c) -> decltype(std::begin(c)){
  return std::begin(c);
}
template <typename C>
inline auto head(const C& c) -> decltype(std::begin(c)){
  return std::begin(c);
}
template <typename C>
inline auto chead(const C& c) -> decltype(c.begin()) {
  return c.begin();
}
template <typename C>
inline auto rhead(C& c) -> decltype(c.rbegin()) {
  return c.rbegin();
}

///////////////////////////////////////////////////////////////////////////

template <typename C>
inline auto tail(C& c) -> decltype(std::end(c)) {
  return std::end(c);
}
template <typename C>
inline auto tail(const C& c) -> decltype(std::end(c)) {
  return std::end(c);
}
template <typename C>
inline auto ctail(const C& c) -> decltype(c.end()) {
  return c.end();
}
template<typename C>
inline auto rtail(C& c) -> decltype(c.rend()) {
  return c.rend();
}

///////////////////////////////////////////////////////////////////////////

template<typename T, size_t S>
inline T* head(T (&A)[S]) {
  return (&A[0]);
}
template<typename T, size_t S>
inline const T* head(const T (&A)[S]) {
  return (&A[0]);
}

///////////////////////////////////////////////////////////////////////////

template<typename T, size_t S>
inline T* tail(T (&A)[S]) {
 return (&A[0] + S);
}
template<typename T, size_t S>
inline const T* tail(const T (&A)[S]) {
  return (&A[0] + S);
}

///////////////////////////////////////////////////////////////////////////

template <typename C>
std::back_insert_iterator<C> back(C& c) {
  return std::back_inserter(c);
}

template <typename C>
std::front_insert_iterator<C> front(C& c) {
  return std::front_inserter(c);
}

///////////////////////////////////////////////////////////////////////////

template<typename C>
inline size_t length(const C& c) {
  return c.size();
}
template<typename T, size_t S>
inline size_t length(T (&A)[S]) {
  return S;
}

///////////////////////////////////////////////////////////////////////////

template<typename C>
inline size_t null(const C& c) {
  return length(c) == 0;
}
template<typename T, size_t S>
inline size_t null(T (&A)[S]) {
  return S==0;
}

///////////////////////////////////////////////////////////////////////////

template <typename T>
struct remove_const_ref {
  typedef typename std::remove_const< typename std::remove_reference<T>::type >::type type;
};

///////////////////////////////////////////////////////////////////////////

template<typename T, typename U>
inline auto fst(const std::pair<T,U>& p) -> nonconstref_type_of(T) {
  return p.first;
}
template <typename T, typename U>
inline auto snd(const std::pair<T,U>& p) -> nonconstref_type_of(U) {
  return p.second;
}

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

template <typename T, typename U, typename V>
struct triple_traits {
  typedef typename traits<T>::value_type t;
  typedef typename traits<U>::value_type u;
  typedef typename traits<V>::value_type v;
  typedef std::tuple<t,u,v> type;
};

///////////////////////////////////////////////////////////////////////////

template <int v>
struct int_to_type {
  enum { value = v };
};

template <typename T>
struct type_to_type {
  typedef T type;
};

///////////////////////////////////////////////////////////////////////////

template <typename T>
class is_container {
  typedef char true_type;
  struct false_type{ true_type _[2]; };
  template <typename U>
  static true_type has_iterator_checker(typename U::iterator *);
  template <typename U>
  static false_type has_iterator_checker(...);
public:
  static const size_t value = (sizeof(has_iterator_checker<T>(0)) == sizeof(true_type));
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
auto iter_value(I i, const C& c) -> value_type_of(C) {
  typedef value_type_of(C) value_type;
  return (i != tail(c)) ? *i : value_type();
}

///////////////////////////////////////////////////////////////////////////

#if 0
template <typename T, size_t S>
inline std::vector<T> make_vector(T (&A)[S]) {
  return std::vector<T>(A, A+S);
}
#else
template <typename T, size_t S>
inline std::vector<T> make_vector(const std::array<T,S>& a) {
  std::vector<T> result(head(a), tail(a));
  return result;
}
#endif

///////////////////////////////////////////////////////////////////////////

template <class InputIterator1, class InputIterator2, class InputIterator3,
         class OutputIterator, class TernaryOperation>
OutputIterator transform3(InputIterator1 first1,
                          InputIterator1 last1,
                          InputIterator2 first2,
                          InputIterator3 first3,
                          OutputIterator d_first,
                          TernaryOperation ternary_op)
{
    while (first1 != last1) {
        *d_first++ = ternary_op(*first1++, *first2++, *first3++);
    }
    return d_first;
}

template <typename InputIterator, typename OutputIterator, typename UnaryOperation>
inline InputIterator copyWhile(InputIterator first, InputIterator last, OutputIterator result, UnaryOperation unary_op) {
  while ((first != last) && unary_op(*first)) {
    *result++ = *first++;
  }
  return first;
}

/////////////////////////////////////////////////////////////////////////////

inline const char* cstr( const std::string& s ) { return s.c_str(); }

} /* namespace fp */

#endif /* _FP_COMMON_H_ */

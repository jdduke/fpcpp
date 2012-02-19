/////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2012, Jared Duke.
// This code is released under the MIT License.
// www.opensource.org/licenses/mit-license.php
/////////////////////////////////////////////////////////////////////////////

#ifndef _FP_TEMPLATE_UTILS_H_
#define _FP_TEMPLATE_UTILS_H_

#include "fp_defines.h"

#include <array>
#include <iterator>
#include <tuple>
#include <vector>

namespace fp {

///////////////////////////////////////////////////////////////////////////
// is_container

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

///////////////////////////////////////////////////////////////////////////
// remove_const_ref

template <typename T>
struct remove_const_ref {
 typedef typename std::remove_const< typename std::remove_reference<T>::type >::type type;
};

///////////////////////////////////////////////////////////////////////////
// traits

template<typename T, bool IsContainer> struct traits_helper { };

template <typename T >
struct traits_helper<T,true> {
  typedef decltype( begin( std::declval<T>() ) )       iterator;
  typedef decltype( begin( std::declval<const T>() ) ) const_iterator;
  typedef typename remove_const_ref< decltype( head( std::declval<T>() ) ) >::type value_type;
};

template <typename T>
struct traits_helper<T,false> {
  typedef T*       iterator;
  typedef const T* const_iterator;
  typedef typename remove_const_ref< T >::type value_type;
};

template <typename T>
struct traits : public traits_helper<T, is_container<T>::value> { };

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
// int_to_type/type_to_type

template <int v>
struct int_to_type {
  enum { value = v };
};

template <typename T>
struct type_to_type {
  typedef T type;
};


///////////////////////////////////////////////////////////////////////////
// has_find

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
// transform3

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

///////////////////////////////////////////////////////////////////////////
// copyWhile

template <typename InputIterator, typename OutputIterator, typename UnaryOperation>
inline InputIterator copyWhile(InputIterator first, InputIterator last, OutputIterator result, UnaryOperation unary_op) {
  while ((first != last) && unary_op(*first)) {
    *result++ = *first++;
  }
  return first;
}

///////////////////////////////////////////////////////////////////////////
// rbegin/rend

template<typename C>
inline auto rbegin(C& c) -> decltype(c.rbegin()) {
  return c.rbegin();
}

template<typename C>
inline auto rend(C& c) -> decltype(c.rend()) {
  return c.rend();
}

///////////////////////////////////////////////////////////////////////////

template <typename I, typename C>
auto iter_value(I i, const C& c) -> value_type_of(C) {
  typedef value_type_of(C) value_type;
  return (i != end(c)) ? *i : value_type();
}


} /* namespace fp */

#endif /* _FP_TEMPLATE_UTILS_H_ */

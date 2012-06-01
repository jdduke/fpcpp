/////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2012, Jared Duke.
// This code is released under the MIT License.
// www.opensource.org/licenses/mit-license.php
/////////////////////////////////////////////////////////////////////////////

#ifndef _FP_COMMON_H_
#define _FP_COMMON_H_

#include "fp_defines.h"
#include "fp_template_utils.h"
#include "fp_curry_defines.h"

#include <array>
#include <functional>
#include <iterator>
#include <tuple>
#include <string>

#if FP_INITIALIZER
#include <initializer_list>
#endif

namespace fp {

using std::string;

///////////////////////////////////////////////////////////////////////////
// List helpers
///////////////////////////////////////////////////////////////////////////

using std::begin;
using std::end;
using std::pair;
using std::move;
using std::forward;

template<typename T>
struct thunk : std::function<T()> { };

template<typename C>
std::back_insert_iterator<C> back(C& c) {
  return std::back_inserter(c);
}

template<typename C>
std::front_insert_iterator<C> front(C& c) {
  return std::front_inserter(c);
}

///////////////////////////////////////////////////////////////////////////
// head

template<typename C>
inline auto head(const C& c) -> nonconstref_type_of(decltype(*begin(c))) {
  return *begin(c);
}
inline char head(const char* c) {
  return c[0];
}
//template<typename T>
//inline T head(std::function<T(void)> t) { return t(); }

///////////////////////////////////////////////////////////////////////////
// tail

template<typename C>
inline C tail(C&& c) {
  return move(drop(1, c));
}
template<typename C>
inline C tail(const C& c) {
  return move(drop(1, c));
}
template<typename T>
inline thunk<T> tail(thunk<T> t) {
  t(); return t;
}

///////////////////////////////////////////////////////////////////////////
// last

template<typename C>
inline auto last(C&& c)      FP_RETURNS( *(--end(c)) );
template<typename C>
inline auto last(const C& c) FP_RETURNS( *(--end(c)) );

///////////////////////////////////////////////////////////////////////////

template<typename C>
inline size_t length(const C& c) {
  return c.size();
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

template<typename T, size_t S>
inline typename types<T>::list list(const std::array<T,S>& a) {
  typename types<T>::list result(extent(a));
  return move(result);
}
template<typename T>
inline typename types<T>::list list() {
  return typename types<T>::list();
}
inline types<char>::list list(const string& s) {
  return move(types<char>::list(extent(s)));
}
#if FP_INITIALIZER
template<typename T, size_t S>
inline typename types<T>::list list(const std::initializer_list<T>& il) {
  typename types<T>::list result(extent(il));
  return move(result);
}
#endif

///////////////////////////////////////////////////////////////////////////
// index

template<typename Index, typename C>
inline fp_enable_if_container(C,value_type_of(C)) index(Index i, const C& c) {
  return c[i];
}

template<typename Index, typename T>
inline T index(Index i, const std::list<T>& l) {
  let it = begin(l);
  std::advance(it, i);
  return it != end(l) ? *it : T();
}

///////////////////////////////////////////////////////////////////////////

// Thunk from list... careful, no bounds check
/*
template<typename C>
inline std::function<value_type_of(C) ()> fromList(const C& c) {
  size_t i = 0;
  return [=]() mutable { return fp::index(i++, c); };
}*/

template<typename T>
inline fp_enable_if_not_container(T,string) show(const T& t);
template<typename C>
inline fp_enable_if_container(C,string) show(const C& c);
inline string show(const types<char>::list& c);
inline string show(const string& s);

// C-string from string
inline const char* fromString( const string& s ) { return s.c_str(); }

// Float from whatever
template<typename T>
float fromIntegral(T t) { return static_cast<float>(t); }

} /* namespace fp */

#endif /* _FP_COMMON_H_ */

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

namespace fp {

using std::string;

///////////////////////////////////////////////////////////////////////////
// List helpers
///////////////////////////////////////////////////////////////////////////

using std::begin;
using std::end;
using std::pair;

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
inline auto head(C&& c)      FP_RETURNS( *begin(c) );
template<typename C>
inline auto head(const C& c) FP_RETURNS( *begin(c) );
template<typename T>
inline T head(thunk<T> t) { return t(); }

///////////////////////////////////////////////////////////////////////////
// tail

template<typename C>
inline C tail(C&& c) {
  return drop(1, c);
}
template<typename C>
inline C tail(const C& c) {
  return drop(1, c);
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
  return result;
}
template<typename T>
inline typename types<T>::list list() {
  return typename types<T>::list();
}
inline types<char>::list list(const string& s) {
  return types<char>::list(extent(s));
}

///////////////////////////////////////////////////////////////////////////

// Thunk from list... careful, no bounds check
template<typename C>
inline std::function<value_type_of(C) ()> fromList(const C& c) {
  size_t i = 0;
  return [=]() mutable -> value_type_of(C) { return fp::index(i++, c); };
}

// C-string from string
inline const char* fromString( const string& s ) { return s.c_str(); }

// Float from whatever
template<typename T>
float fromIntegral(T t) { return static_cast<float>(t); }

} /* namespace fp */

#endif /* _FP_COMMON_H_ */

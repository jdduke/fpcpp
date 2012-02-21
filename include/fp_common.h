/////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2012, Jared Duke.
// This code is released under the MIT License.
// www.opensource.org/licenses/mit-license.php
/////////////////////////////////////////////////////////////////////////////

#ifndef _FP_COMMON_H_
#define _FP_COMMON_H_

#include "fp_defines.h"
#include "fp_template_utils.h"

#include <array>
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

///////////////////////////////////////////////////////////////////////////
// head

template <typename C>
inline auto head(C& c) -> nonconstref_type_of(decltype(*begin(c))) {
  return *std::begin(c);
}
template <typename C>
inline auto head(const C& c) -> nonconstref_type_of(decltype(*begin(c))) {
  return *std::begin(c);
}

struct headF {
  template<typename C>
  inline auto operator()(const C& c) FP_RETURNS( head(c) );
};
//FP_DEFINE_FUNCTION_OBJECT(head, headF);

///////////////////////////////////////////////////////////////////////////
// tail

template <typename C>
inline C tail(C&& c) {
  return drop(1, c);
}
template <typename C>
inline C tail(const C& c) {
  return drop(1, c);
}

///////////////////////////////////////////////////////////////////////////
// last

template <typename C>
inline C last(C&& c) {
  return drop(l, c);
}
template <typename C>
inline C last(const C& c) {
  return drop(1, c);
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
struct lengthF {
  template<typename C>
  inline auto operator()(const C& c) FP_RETURNS( length(c) );
};

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

template <typename T, size_t S>
inline typename types<T>::list list(const std::array<T,S>& a) {
  typename types<T>::list result(extent(a));
  return result;
}
template <typename T>
inline typename types<T>::list list() {
  return typename types<T>::list();
}
inline types<char>::list list(const string& s) {
  return types<char>::list(extent(s));
}

inline const char* cstr( const string& s ) { return s.c_str(); }

template<typename T>
float fromIntegral(T t) { return (float)t; }

} /* namespace fp */

#endif /* _FP_COMMON_H_ */

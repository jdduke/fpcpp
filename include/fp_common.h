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
#include <vector>

namespace fp {

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

template<typename T, size_t S>
inline T head(T (&A)[S]) {
  return (&A[0]);
}
template<typename T, size_t S>
inline T head(const T (&A)[S]) {
  return (&A[0]);
}

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
  return 
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
// Pair/Tuple

using std::pair;
using std::make_pair;

template<typename T, typename U>
inline nonconstref_type_of(T) fst(const pair<T,U>& p) {
  return p.first;
}
template <typename T, typename U>
inline nonconstref_type_of(U) snd(const pair<T,U>& p) {
  return p.second;
}
template<typename T, typename U>
inline pair<U,T> swap(const pair<T,U>& p) {
  return make_pair(p.first,p.second);
}
template<typename F0, typename F1, typename T, typename U>
inline auto mapPair(F0 f0, F1 f1, const std::pair<T,U>& p) FP_RETURNS( make_pair( f0(p.first), f1(p.second) ) );


///////////////////////////////////////////////////////////////////////////

#if 0
template <typename T, size_t S>
inline std::vector<T> make_vector(T (&A)[S]) {
  return std::vector<T>(A, A+S);
}
#else
template <typename T, size_t S>
inline std::vector<T> make_vector(const std::array<T,S>& a) {
  std::vector<T> result(extent(a));
  return result;
}
#endif

inline const char* cstr( const std::string& s ) { return s.c_str(); }

} /* namespace fp */

#endif /* _FP_COMMON_H_ */

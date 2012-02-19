/////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2012, Jared Duke.
// This code is released under the MIT License.
// www.opensource.org/licenses/mit-license.php
/////////////////////////////////////////////////////////////////////////////

#ifndef _FP_PRELUDE_LISTS_H_
#define _FP_PRELUDE_LISTS_H_

#include "fp_defines.h"
#include "fp_curry.h"
#include "fp_prelude.h"

#include <algorithm>
#include <functional>
#include <numeric>
#include <map>
#include <sstream>
#include <fstream>
#include <tuple>

namespace fp {


///////////////////////////////////////////////////////////////////////////
// List operations
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
// map

template<typename F, typename C, typename R>
inline R& __map__(F f, const C& c, R& r) {
  std::transform(extent(c), back(r), f);
  return r;
}

template<typename F, typename C>
inline auto map(F f, const C& c) -> std::vector< result_type_of(F) > {
  typedef std::vector< result_type_of(F) > result_type;
  result_type result;
  return __map__(f, c, result);
}
template<typename F>
inline std::string map(F f, const std::string& s) {
  std::vector<char> charVec;
   __map__(f, std::vector<char>(extent(s)), charVec);
  return std::string(extent(charVec));
}
template<typename F>
inline std::string map(F f, const char* s) {
  return map(f, std::string(s));
}
FP_DEFINE_FUNC_OBJ(map, map_, _map_);

template<typename F, typename C>
inline F mapM(F f, const C& c) {
  return std::for_each(extent(c), f);
}

///////////////////////////////////////////////////////////////////////////
// filter

template<typename F, typename C>
inline C filter(F f, C c) {
  C result;
  std::copy_if(extent(c), back(result), f);
  return result;
}
FP_DEFINE_FUNC_OBJ(filter, filter_, _filter_);




///////////////////////////////////////////////////////////////////////////
// Reducing lists
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
// fold

template<typename It, typename Op>
inline auto fold(It first, It last, Op op) -> typename std::iterator_traits<It>::value_type {
  if (first != last) {
    auto value = *first;
    return std::accumulate(++first, last, value, op);
  } else {
    return typename std::iterator_traits<It>::value_type();
  }
}

template<typename It, typename T, typename Op>
inline T fold(It first, It last, T t, Op op) {
  return std::accumulate(first, last, t, op);
}

/////////////////////////////////////////////////////////////////////////////
// foldl

template<typename F, typename T, typename C>
inline T foldl(F f, T t, const C& c) {
  return fold(extent(c), t, f);
}
FP_DEFINE_FUNC_OBJ_T(foldl, foldl_, _foldl_);

template<typename F, typename C>
inline auto foldl1(F f, const C& c) -> value_type_of(C) {
  return fold(extent(c), f);
}
FP_DEFINE_FUNC_OBJ_T(foldl1, foldl1_, _foldl1_);

/////////////////////////////////////////////////////////////////////////////
// foldr

template<typename F, typename T, typename C>
inline T foldr(F f, T t, const C& c) {
  return fold(rextent(c), t, f);
}
FP_DEFINE_FUNC_OBJ_T(foldr, foldr_, _foldr_);

template<typename F, typename C>
inline auto foldr1(F f, const C& c) -> value_type_of(C) {
  return fold(rextent(c), f);
}
FP_DEFINE_FUNC_OBJ_T(foldr1, foldr1_, _foldr1_);



///////////////////////////////////////////////////////////////////////////
// Special folds
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
// sum

template<class C>
auto sum(const C& c) -> value_type_of(C) {
  return std::accumulate(extent(c), value_type_of(C)());
}

///////////////////////////////////////////////////////////////////////////
// product

template<class C>
auto product(const C& c) -> value_type_of(C) {
  return fold(extent(c), std::multiplies< value_type_of(C)>());
}

///////////////////////////////////////////////////////////////////////////
// all

template<typename F, typename C>
inline bool all(F f, const C& c) {
  return std::all_of(extent(c), f);
}

///////////////////////////////////////////////////////////////////////////
// any

template<typename F, typename C>
inline bool any(F f, const C& c) {
  return std::any_of(extent(c), f);
}

///////////////////////////////////////////////////////////////////////////
// and

template<typename C>
inline bool andAll(const C& c) {
  return fold(extent(c), std::logical_and< value_type_of(C) >());
}

///////////////////////////////////////////////////////////////////////////
// or

template<typename C>
inline bool orAll(const C& c) {
  return fold(extent(c), std::logical_or< value_type_of(C) >());
}

///////////////////////////////////////////////////////////////////////////
// Infinite lists
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
// iterate
template<typename F, typename T>
inline std::function< T(void) > iterate(F f, T t) {
  return [=]() mutable -> T { T r = t; t = f(t); return r; };
}

///////////////////////////////////////////////////////////////////////////
// repeat
template<typename T>
inline std::function< T(void) > iterate(T t) {
  return [=]() mutable { return t; };
}

///////////////////////////////////////////////////////////////////////////
// replicate
template<typename T>
inline std::vector<T> replicate(size_t n, T t) {
  return std::vector<T>(n, t);
}

///////////////////////////////////////////////////////////////////////////
// cycle
template<typename C>
inline auto cycle(const C& c) -> std::function< value_type_of(C) (size_t)> {
  return [=](size_t i) { return c[i % length(c)]; };
}

///////////////////////////////////////////////////////////////////////////
// until
template <typename P, typename F, typename T>
inline T until(P condition, F op, T value) {
  while ( !condition( value ) ) {
    value = op( value );
  }
  return value;
}

///////////////////////////////////////////////////////////////////////////
// Zipping and unzipping lists
///////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// zipWith

template<typename F, typename T, typename U, typename R>
inline R& __zipWith__(F f, const T& t, const U& u, R& r) {
  std::transform(extent(t), head(u), back(r), f);
  return r;
}
template<typename F, typename T, typename U, typename V, typename R>
inline R& __zipWith3__(F f, const T& t, const U& u, const V& v, R& r) {
  transform3(extent(t), head(u), head(v), back(r), f);
  return r;
}

template<typename F, typename T, typename U>
inline T zipWith(F f, const T& t, const U& u) {
  T result;
  return __zipWith__(f, t, u, result);
}
FP_DEFINE_FUNC_OBJ(zipWith, zipWith_, _zipWith_)

template<typename F, typename T, typename U, typename V>
inline T zipWith3(F f, const T& t, const U& u, const V& v) {
  T result;
  return __zipWith3__(f, t, u, v, result);
}
FP_DEFINE_FUNC_OBJ(zipWith3, zipWith3_, _zipWith3_)

///////////////////////////////////////////////////////////////////////////
// zip

template<typename C0, typename C1>
inline std::vector< typename tuple_traits<C0,C1>::type > zip(const C0& c1, const C1& c2) {
  typedef value_type_of(C0) T;
  typedef value_type_of(C1) U;
  typedef std::vector< std::pair<T,U> > result_type;
  result_type result;
  return __zipWith__([](const T& t, const U& u) { return std::make_pair(t,u); }, c1, c2, result);
}

template<typename C0, typename C1, typename C2>
inline std::vector< typename triple_traits<C0,C1,C2>::type > zip3(const C0& c1, const C1& c2, const C2& c3) {
  typedef value_type_of(C0) T;
  typedef value_type_of(C1) U;
  typedef value_type_of(C2) V;
  typedef std::vector< std::tuple<T,U,V> > result_type;
  result_type result;
  return __zipWith__([](const T& t, const U& u, const V& v) { return std::make_tuple(t,u,v); }, c1, c2, c3, result);
}

///////////////////////////////////////////////////////////////////////////
// unzip

template<typename F, typename T, typename U, typename R>
inline R& __unzip__(F f, const T& t, const U& u, R& r) {
  std::transform(extent(t), head(u), back(r), f);
  return r;
}

template<typename T, typename U>
inline std::pair< std::vector<T>, std::vector<U> > unzip(const std::vector< std::pair<T,U> >& c) {
  std::vector<T> t;
  std::vector<U> u;
  for_each(extent(c), [&](const std::pair<T,U>& p) {
    t.push_back( fst(p) );
    u.push_back( snd(p) );
  });
  return std::make_pair(t,u);
}

template<typename T, typename U, typename V>
inline std::tuple< std::vector<T>, std::vector<U>, std::vector<V> > unzip3(const std::vector< std::tuple<T,U,V> >& c) {
  std::vector<T> t;
  std::vector<U> u;
  std::vector<V> v;
  for_each(extent(c), [&](const std::tuple<T,U,V>& val) {
    t.push_back( std::get<0>(val) );
    u.push_back( std::get<1>(val) );
    v.push_back( std::get<1>(val) );
  });
  return std::make_tuple(t,u,v);
}

///////////////////////////////////////////////////////////////////////////
// maximum

template<typename F, typename C>
inline value_type_of(C) maximumBy(F f, const C& c) {
  return iter_value( std::max_element(extent(c), f), c);
}

template<typename C>
inline value_type_of(C) maximum(const C& c) {
  return iter_value( std::max_element(extent(c)), c);
}

///////////////////////////////////////////////////////////////////////////
// minimum

template<typename F, typename C>
inline value_type_of(C) minimumBy(F f, const C& c) {
  return iter_value( std::min_element(extent(c), f), c);
}

template<typename C>
inline value_type_of(C) minimum(const C& c) {
  return iter_value( std::min_element(extent(c)), c);
}

///////////////////////////////////////////////////////////////////////////
// reverse

template<typename C>
inline C reverse(const C& c_) {
  C c(c_);
  std::reverse(head(c), tail(c));
  return c;
}

///////////////////////////////////////////////////////////////////////////
// sortBy

template<typename F, typename C>
inline C sortBy(F f, C c) {
  std::sort(extent(c), f);
  return c;
}

///////////////////////////////////////////////////////////////////////////
// sort

template<typename C>
inline C sort(C c) {
  std::sort(extent(c));
  return c;
}

///////////////////////////////////////////////////////////////////////////
// groupBy

template<typename F, typename C>
inline std::vector<C> groupBy(F f, const C& c) {
  std::vector<C> result;
  let it = head(c);
  while (it != tail(c)) {
    C newGroup;
    it = copyWhile(it, tail(c), back(newGroup), [&]( const value_type_of(C) & t ) {
      return f(*it,t);
    });
    result.push_back( newGroup );
  }
  return result;
}

///////////////////////////////////////////////////////////////////////////
// group

template<typename C>
inline C group(const C& c) {
  return groupBy(std::equal_to< value_type_of(C) >(), c);
}

///////////////////////////////////////////////////////////////////////////
// Sublists
///////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// takeWhile

template<typename F, typename C>
inline C takeWhile(F f, const C& c) {
  C result;
  //std::copy(head(t), std::find_if(extent(t), std::not1<F>(f)), back(result));
  copyWhile(extent(c), back(result), f);
  return result;
}
FP_DEFINE_FUNC_OBJ(takeWhile, takeWhile_, _takeWhile_);

template<typename F, typename Source>
inline auto takeWhileF(F f, Source source) -> std::vector< decltype(source()) > {
  typedef decltype(source()) t_type;
  std::vector<t_type> result;
  auto   back_iter = back(result);
  for (t_type value = source(); f(value); value=source())
    back_iter = value;
  return result;
}
FP_DEFINE_FUNC_OBJ(takeWhileF, takeWhileF_, _takeWhileF_);

///////////////////////////////////////////////////////////////////////////
// take
template<typename C>
inline C take(size_t n, const C& c) {
  let takeN = [=](const value_type_of(C) &) mutable { return n-- > 0; };
  return takeWhileF(takeN, c);
}
template<typename T>
inline std::vector<T> take(size_t n, const std::vector<T>& v) {
  return n < length(v) ? std::vector<T>(head(v), head(v) + n) : v;
}

template<typename F>
inline auto takeF(size_t n, F f) -> std::vector< decltype(f()) > {
  let takeN = [=]( decltype(f()) ) mutable { return n-- > 0; };
  return takeWhileF(takeN, f);
}

///////////////////////////////////////////////////////////////////////////
// dropWhile

template<typename F, typename C>
inline C dropWhile(F f, const C& c) {
  C result;
  std::copy(std::find_if_not(extent(c), f), tail(c), back(result));
  return result;
}
FP_DEFINE_FUNC_OBJ(dropWhile, dropWhile_, _dropWhile_);

///////////////////////////////////////////////////////////////////////////
// drop

template<typename C>
inline C drop(size_t n, const C& c) {
  let dropN = [=](const typename traits<C>::value_type&) mutable { return n-- > 0; };
  return dropWhile(dropN, c);
}
template<typename T>
inline std::vector<T> drop(size_t n, const std::vector<T>& v) {
  return n < length(v) ? std::vector<T>(head(v) + n, tail(v)) : std::vector<T>();
}

///////////////////////////////////////////////////////////////////////////
// splitAt

template<typename C>
inline std::pair<C,C> splitAt(size_t n, const C& c) {
  return std::make_pair( take(n, c), drop(n, c) );
}

///////////////////////////////////////////////////////////////////////////
// span

template<typename F, typename C>
inline std::pair<C,C> span(F f, const C& c) {
  return std::make_pair( takeWhile(f, c), dropWhile(f, c) );
}

///////////////////////////////////////////////////////////////////////////
// break

template<typename F, typename C>
inline std::pair<C,C> spanNot(F f, const C& c) {
  return span( std::not1(f), c );
}




///////////////////////////////////////////////////////////////////////////
// Searching lists
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
// elem

template<typename T, typename C>
inline bool elem(const T& t, const C& c) {
  return std::find(extent(c), t) != tail(c);
}

///////////////////////////////////////////////////////////////////////////
// notElem

template<typename T, typename C>
inline bool notElem(const T& t, const C& c) {
  return !elem(t, c);
}

///////////////////////////////////////////////////////////////////////////
// lookup

template<typename K, typename C>
inline bool lookup(const K& k, const C& c) {
  return iter_value( c.find(k), c );
}



///////////////////////////////////////////////////////////////////////////
// Generating lists
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
// generate_n
template<typename F>
inline auto generate_n(size_t n, F f) -> std::vector< decltype(f()) > {
  std::vector< decltype(f()) > t(n);
  std::generate_n(head(t), n, f);
  return t;
}

///////////////////////////////////////////////////////////////////////////
// increasing_n
template<typename T>
inline std::vector<T> increasing_n(size_t n, T t0 = (T)0) {
  return generate_n(n, [&]() -> T {
    T r = t0; t0 = succ(t0); return r;
  });
}

///////////////////////////////////////////////////////////////////////////
// decreasing_n
template<typename T>
inline std::vector<T> decreasing_n(size_t n, T t0 = (T)0) {
  return generate_n(n, [&]() -> T {
    T r = t0; t0 = pred(t0); return r;
  });
}

///////////////////////////////////////////////////////////////////////////
// enumFrom
template<typename T>
std::function<T(void)> enumFrom(T t) {
  return iterate(&succ<T>, t);
  //return [=]() mutable -> T { T r = t; t = succ(t); return r; };
}

///////////////////////////////////////////////////////////////////////////
// concat
template<typename T>
inline typename std::enable_if<is_container<T>::value, T>::type
concat(const T& t0, const T& t1) {
  T result(t0);
  std::copy(head(t1), tail(t1), back(result));
  return result;
}

///////////////////////////////////////////////////////////////////////////
// append
template<typename C, typename T>
inline C append(const C& c, const T& t) {
  C result(c);
  result.insert(tail(result), t);
  return result;
}

///////////////////////////////////////////////////////////////////////////
// cons
template<typename T, typename C>
inline C cons(const T& t, const C& c) {
  return concat( append(C(), t), c );
}

} /* namespace fp */

#endif /* _FP_PRELUDE_LISTS_H_ */

/////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2012, Jared Duke.
// This code is released under the MIT License.
// www.opensource.org/licenses/mit-license.php
/////////////////////////////////////////////////////////////////////////////

#ifndef _FP_PRELUDE_LISTS_H_
#define _FP_PRELUDE_LISTS_H_

#include "fp_defines.h"
#include "fp_curry.h"
#include "fp_prelude_math.h"
#include "fp_prelude.h"

#include <algorithm>
#include <functional>
#include <list>
#include <numeric>
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

// This code is abominable...
template<typename F, typename C>
inline auto
map(F f, const C& c) -> typename std::enable_if< !std::is_same<void,decltype(f(declval<typename traits<C>::value_type>()))>::value,
                                                 typename types< nonconstref_type_of(decltype(f(declval<typename traits<C>::value_type>()))) >::list >::type {
  typedef typename types< nonconstref_type_of(decltype(f(head(c)))) >::list result_type;
  result_type result;
  return move(__map__(f, c, result));
}

template<typename F, typename C>
void mapV(F f, const C& c) {
  std::for_each(extent(c), f);
}

template<typename F>
inline string map(F f, const string& s) {
  typedef typename types<char>::list char_list;
  char_list charList;
   __map__(f, char_list(extent(s)), charList);
  return move(show(charList));
}

template<typename F>
inline string map(F f, const char* s) {
  return move(map(f, string(s)));
}
FP_DEFINE_CURRIED(map, map_);

///////////////////////////////////////////////////////////////////////////
// filter

template<typename F, typename C>
inline C filter(F f, const C& c) {
  C result;
  std::copy_if(extent(c), back(result), f);
  return move(result);
}
FP_DEFINE_CURRIED(filter, filter_);

template<typename T, typename F>
typename types<T>::list operator|(const typename types<T>::list& l, F f) {
  return move(filter(f, l));
}

///////////////////////////////////////////////////////////////////////////
// Reducing lists
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
// fold

#if 1
template<typename It, typename Op>
inline auto fold(It first, It last, Op op) -> nonconstref_type_of(decltype(op(*first, *first))) {
  typedef nonconstref_type_of(decltype(op(*first, *first))) T;
  if (first != last) {
    auto value = T(*first);
    return move(std::accumulate(++first, last, value, op));
  } else {
    return T();
  }
}
#else
template<typename It, typename Op>
inline auto fold(It first, It last, Op op) -> typename std::iterator_traits<It>::value_type {
  if (first != last) {
    auto value = *first;
    return std::accumulate(++first, last, value, op);
  } else {
    return typename std::iterator_traits<It>::value_type();
  }
}
#endif

template<typename It, typename T, typename Op>
inline T fold(It first, It last, T t, Op op) {
  return move(std::accumulate(first, last, t, op));
}

/////////////////////////////////////////////////////////////////////////////
// foldl

template <typename F, typename T, typename C>
inline T foldl(F f, T t, const C& c) {
  return move(fold(extent(c), t, f));
}
FP_DEFINE_CURRIED_T(foldl, foldl_, _foldl_);

template <typename F, typename C>
inline auto foldl1(F f, const C& c) -> value_type_of(C) {
  return move(fold(extent(c), f));
}
FP_DEFINE_CURRIED(foldl1, foldl1_);

/////////////////////////////////////////////////////////////////////////////
// foldr

template <typename F, typename T, typename C>
inline T foldr(F f, T t, const C& c) {
  return move(fold(rextent(c), t, f));
}
FP_DEFINE_CURRIED_T(foldr, foldr_, _foldr_);

template <typename F, typename C>
inline auto foldr1(F f, const C& c) -> value_type_of(C) {
  return move(fold(rextent(c), f));
}
FP_DEFINE_CURRIED(foldr1, foldr1_);

///////////////////////////////////////////////////////////////////////////
// Special folds
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
// sum

template <class C>
auto sum(const C& c) -> value_type_of(C) {
  return std::accumulate(extent(c), value_type_of(C)());
}

///////////////////////////////////////////////////////////////////////////
// product

template <class C>
auto product(const C& c) -> value_type_of(C) {
  return fold(extent(c), std::multiplies< value_type_of(C)>());
}

///////////////////////////////////////////////////////////////////////////
// all

template <typename F, typename C>
inline bool all(F f, const C& c) {
  return std::all_of(extent(c), f);
}

///////////////////////////////////////////////////////////////////////////
// any

template <typename F, typename C>
inline bool any(F f, const C& c) {
  return std::any_of(extent(c), f);
}

///////////////////////////////////////////////////////////////////////////
// and

template <typename C>
inline bool andAll(const C& c) {
  return fold(extent(c), std::logical_and< value_type_of(C) >());
}

///////////////////////////////////////////////////////////////////////////
// or

template <typename C>
inline bool orAll(const C& c) {
  return fold(extent(c), std::logical_or< value_type_of(C) >());
}

///////////////////////////////////////////////////////////////////////////
// Building lists
///////////////////////////////////////////////////////////////////////////

template<typename InputIt, typename OutIt, typename T, typename Op>
inline void scan(InputIt first, InputIt last, OutIt out, T t0, Op op) {
  for(*out = t0; first != last; ) {
    *out = t0 = op(t0, *first++);
  }
}

template<typename InputIt, typename OutIt, typename Op>
inline void scan(InputIt first, InputIt last, OutIt out, Op op) {
  if (first != last) {
    typename std::iterator_traits<InputIt>::value_type t0 = *first++;
    scan(first, last, out, t0, op);
  }
}

//////////////////////////////////////////////////////////////////////////
// scanl

template<typename F, typename T, typename C>
typename types<T>::list scanl(F f, T t, const C& c) {
  typename types<T>::list result(1, t);
  scan(extent(c), back(result), t, f);
  return move(result);
}

//////////////////////////////////////////////////////////////////////////
// scanl1

template<typename F, typename C>
C scanl1(F f, const C& c) {
  C result;
  scan(extent(c), back(result), f);
  return move(result);
}

//////////////////////////////////////////////////////////////////////////
// scanr

template<typename F, typename T, typename C>
typename types<T>::list scanr(F f, T t, const C& c) {
  typename types<T>::list result(1, t);
  scan(rextent(c), back(result), t, f);
  return move(result);
}

//////////////////////////////////////////////////////////////////////////
// scanr

template<typename F, typename C>
C scanr1(F f, const C& c) {
  C result;
  scan(rextent(c), back(result), f);
  return move(result);
}

///////////////////////////////////////////////////////////////////////////
// Infinite lists
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
// iterate
template <typename F, typename T>
inline std::function< T(void) > iterate(F f, T t) {
  return [=]() mutable -> T { T r = t; t = f(t); return r; };
}

///////////////////////////////////////////////////////////////////////////
// repeat
template <typename T>
inline thunk< T > iterate(T t) {
  return [=]() mutable { return t; };
}

///////////////////////////////////////////////////////////////////////////
// replicate
template <typename T>
inline auto replicate(size_t n, T t) FP_RETURNS( typename types<T>::list(n, t) );

///////////////////////////////////////////////////////////////////////////
// cycle
template <typename C>
inline auto cycle(const C& c) -> thunk< value_type_of(C) > {
  size_t i = 0;
  return [=]() mutable { return c[i++ % length(c)]; };
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

template <typename F, typename T, typename U, typename R>
inline R& __zipWith__(F f, const T& t, const U& u, R& r) {
  std::transform(extent(t), begin(u), back(r), f);
  return r;
}
template <typename F, typename T, typename U, typename R>
inline R& __zipWith__(F f, T&& t, U&& u, R& r) {
  std::transform(extent(t), begin(u), back(r), f);
  return r;
}
template <typename F, typename T, typename U, typename V, typename R>
inline R& __zipWith3__(F f, const T& t, const U& u, const V& v, R& r) {
  transform3(extent(t), begin(u), begin(v), back(r), f);
  return r;
}
template <typename F, typename T, typename U, typename V, typename R>
inline R& __zipWith3__(F f, T&& t, U&& u, V&& v, R& r) {
  transform3(extent(t), begin(u), begin(v), back(r), f);
  return r;
}

template <typename F, typename T, typename U>
inline auto zipWith(F f, const T& t, const U& u) -> typename types<decltype(f(head(t),head(u)))>::list {
  typedef decltype(f(head(t),head(u))) result_type;
  typename types<result_type>::list result;
  return move(__zipWith__(f, t, u, result));
}
template <typename F, typename T, typename U>
inline auto zipWith(F f, T&& t, U&& u) -> typename types<decltype(f(head(t),head(u)))>::list {
  typedef decltype(f(head(t),head(u))) result_type;
  typename types<result_type>::list result;
  return move(__zipWith__(f, t, u, result));
}
FP_DEFINE_CURRIED_T(zipWith, zipWith_, _zipWith_)

template <typename F, typename T, typename U, typename V>
inline auto zipWith3(F f, const T& t, const U& u, const V& v) -> typename types<decltype(f(head(t),head(u),head(v)))>::list {
  typedef decltype(f(head(t), head(u), head(v))) result_type;
  typename types<result_type>::list result;
  return move(__zipWith3__(f, t, u, v, result));
}
template <typename F, typename T, typename U, typename V>
inline auto zipWith3(F f, T&& t, U&& u, V&& v)  -> typename types<decltype(f(head(t),head(u),head(v)))>::list {
  typedef decltype(f(head(t), head(u), head(v))) result_type;
  typename types<result_type>::list result;
  return move(__zipWith3__(f, move(t), move(u), move(v), result));
}
FP_DEFINE_CURRIED_T(zipWith3, zipWith3_, _zipWith3_)


///////////////////////////////////////////////////////////////////////////
// zip

template<typename C0, typename C1>
inline typename types< typename tuple_traits<C0,C1>::type >::list zip(const C0& c1, const C1& c2) {
  typedef value_type_of(C0) T;
  typedef value_type_of(C1) U;
  typedef typename types< std::pair<T,U> >::list result_type;
  result_type result;
  return move(__zipWith__([](const T& t, const U& u) { return std::make_pair(t,u); }, c1, c2, result));
}

template<typename C0, typename C1, typename C2>
inline typename types< typename triple_traits<C0,C1,C2>::type >::list zip3(const C0& c1, const C1& c2, const C2& c3) {
  typedef value_type_of(C0) T;
  typedef value_type_of(C1) U;
  typedef value_type_of(C2) V;
  typedef typename types< std::tuple<T,U,V> >::list result_type;
  result_type result;
  return move(__zipWith__([](const T& t, const U& u, const V& v) { return std::make_tuple(t,u,v); }, c1, c2, c3, result));
}

///////////////////////////////////////////////////////////////////////////
// unzip

template<typename F, typename T, typename U, typename R>
inline R& __unzip__(F f, const T& t, const U& u, R& r) {
  std::transform(extent(t), begin(u), back(r), f);
  return r;
}

template<typename T, typename U>
inline auto unzip(const typename types< std::pair<T,U> >::list& c) -> std::pair< typename types<T>::list, typename types<U>::list > {
  typename types<T>::list t;
  typename types<U>::list u;
  for_each(extent(c), [&](const std::pair<T,U>& p) {
    t.push_back( fst(p) );
    u.push_back( snd(p) );
  });
  return std::make_pair(t,u);
}

template<typename T, typename U, typename V>
inline std::tuple< typename types<T>::list, typename types<U>::list, typename types<V>::list > unzip3(const typename types< std::tuple<T,U,V> >::list& c) {
  typename types<T>::list t;
  typename types<U>::list u;
  typename types<V>::list v;
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

template <typename C>
inline value_type_of(C) minimum(const C& c) {
  return iter_value( std::min_element(extent(c)), c);
}

///////////////////////////////////////////////////////////////////////////
// sortBy

template <typename F, typename C>
inline C sortBy(F f, C c) {
  std::sort(extent(c), f);
  return move(c);
}

///////////////////////////////////////////////////////////////////////////
// sort

template <typename C>
inline C sort(C c) {
  std::sort(extent(c));
  return move(c);
}

#if !USE_DEQUE_FOR_LISTS
template <typename T>
inline typename std::enable_if<!is_container<T>::value,std::list<T> >::type sort(std::list<T> l) {
  l.sort();
  return move(l);
}
#endif

///////////////////////////////////////////////////////////////////////////
// groupBy

template<typename F, typename C>
inline typename types<C>::list groupBy(F f, const C& c) {
  typename types<C>::list result;
  let it = begin(c);
  while (it != end(c)) {
    result.push_back( C() );
    it = copyWhile(it, end(c), back(result.back()), [&]( const value_type_of(C) & t ) {
      return f(*it,t);
    });
  }
  return move(result);
}

///////////////////////////////////////////////////////////////////////////
// group

template<typename C>
inline typename types<C>::list group(const C& c) {
  return groupBy(std::equal_to< value_type_of(C) >(), c);
}

///////////////////////////////////////////////////////////////////////////
// insertBy

template<typename F, typename T, typename C>
inline C insertBy(F f, T t, C c) {
  let compare = [&](const T& t1) { return f(t, t1); };
  c.insert( std::find_if( extent(c), compare ), t );
  return move(c);
}

///////////////////////////////////////////////////////////////////////////
// insert

template<typename T, typename C>
inline C insert(T t, C c) {
  return insertBy( math::less<T>, t, c );
}

///////////////////////////////////////////////////////////////////////////
// Sublists
///////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// takeWhile

template <typename F, typename C>
inline C takeWhile(F f, const C& c) {
  C result;
  //std::copy(begin(t), std::find_if(extent(t), std::not1<F>(f)), back(result));
  copyWhile(extent(c), back(result), f);
  return move(result);
}
FP_DEFINE_CURRIED(takeWhile, takeWhile_);

template <typename F, typename T>
inline auto takeWhileT(F f, T t) -> typename types< decltype(t()) >::list {
  typedef decltype(t()) t_type;
  typename types<t_type>::list result;
  auto back_iter = back(result);
  for (let value = t(); f(value); value=t())
    back_iter = value;
  return move(result);
}
FP_DEFINE_CURRIED(takeWhileT, takeWhileT_);

///////////////////////////////////////////////////////////////////////////
// take
template <typename C>
inline C take(size_t n, const C& c) {
  let takeN = [=](...) mutable { return n-- > 0; };
  return takeWhile(takeN, c);
}
template <typename T>
inline typename types<T>::list take(size_t n, const typename types<T>::list& v) {
  return n < length(v) ? typename types<T>::list(begin(v), begin(v) + n) : v;
}
template <typename F>
inline auto takeF(size_t n, F f) -> typename types< decltype(f()) >::list {
  typename types< decltype(f()) >::list result(n);
  std::generate_n(begin(result), n, f);
  return move(result);
}

///////////////////////////////////////////////////////////////////////////
// dropWhile

template <typename F, typename C>
inline C dropWhile(F f, const C& c) {
  C result;
  std::copy(std::find_if_not(extent(c), f), end(c), back(result));
  return move(result);
}
FP_DEFINE_CURRIED(dropWhile, dropWhile_);

///////////////////////////////////////////////////////////////////////////
// drop

template <typename C>
inline C drop(size_t n, const C& c) {
#if 1
  return n < length(c) ? C( begin(c) + n, end(c) ) : C();
#else
  let dropN = [=](const typename traits<C>::value_type&) mutable { return n-- > 0; };
  return dropWhile(dropN, c);
#endif
}

///////////////////////////////////////////////////////////////////////////
// splitAt

template <typename C>
inline pair<C,C> splitAt(size_t n, const C& c) {
  return make_pair( take(n, c), drop(n, c) );
}

///////////////////////////////////////////////////////////////////////////
// span

template <typename F, typename C>
inline pair<C,C> span(F f, const C& c) {
  return make_pair( takeWhile(f, c), dropWhile(f, c) );
}

///////////////////////////////////////////////////////////////////////////
// break

template <typename F, typename C>
inline pair<C,C> spanNot(F f, const C& c) {
  return span( std::not1(f), c );
}

///////////////////////////////////////////////////////////////////////////
// slice

template <typename C>
inline C slice(size_t i, size_t n, const C& c) {
  return C(begin(c)+i, begin(c)+i+n);
}


///////////////////////////////////////////////////////////////////////////
// Searching lists
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
// elem

template <typename T, typename C>
inline bool elem(const T& t, const C& c) {
  return std::find(extent(c), t) != end(c);
}

///////////////////////////////////////////////////////////////////////////
// notElem

template <typename T, typename C>
inline bool notElem(const T& t, const C& c) {
  return !elem(t, c);
}

///////////////////////////////////////////////////////////////////////////
// lookup

template <typename K, typename C>
inline bool lookup(const K& k, const C& c) {
  return iter_value( c.find(k), c );
}

///////////////////////////////////////////////////////////////////////////
// Generating lists
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
// increasing
template <typename T>
inline auto increasing(T t0 = (T)0) FP_RETURNS( iterate(&succ<T>, t0) );
template <typename T>
inline auto increasingN(size_t n, T t0 = (T)0) FP_RETURNS( takeF(n, increasing(t0)) );

///////////////////////////////////////////////////////////////////////////
// decreasing
template <typename T>
inline auto decreasing(T t0 = (T)0) FP_RETURNS( iterate(&pred<T>, t0) );
template <typename T>
inline auto decreasingN(size_t n, T t0 = (T)0) FP_RETURNS( takeF(n, decreasing(t0)) );

///////////////////////////////////////////////////////////////////////////
// enumFrom
template <typename T>
inline auto enumFrom(T t0) FP_RETURNS( iterate(&succ<T>, t0) );

///////////////////////////////////////////////////////////////////////////
// uniform
template<typename T>
inline typename types<T>::list uniformN(size_t n, T t0, T t1) {
  return takeF(n, [=]() { return math::uniform(t0,t1); });
}

///////////////////////////////////////////////////////////////////////////
// auto lists

// Example: (0 <to> 9) == increasingN(0, 10)
static struct list_to_helper { } to;

template <typename T>
struct list_to_helper_val {
  list_to_helper_val(T value_) : value(value_) { }
  T value;
};

template<typename T0, typename T1>
inline auto operator>(list_to_helper_val<T0> t0, T1 t1) FP_RETURNS( increasingN((T0)(t1-t0.value+1), t0.value) );
template<typename T>
inline list_to_helper_val<T> operator<(T t, list_to_helper) {
  ((void)to);
  return list_to_helper_val<T>(t);
}

///////////////////////////////////////////////////////////////////////////
// concat
template <typename T>
inline fp_enable_if_container(T,T)
concat(const T& t0, const T& t1) {
  T result(t0);
  std::copy(extent(t1), back(result));
  return move(result);
}
template <typename T>
inline fp_enable_if_not_container(T,typename types<T>::list)
concat(const T& t0, const T& t1) {
  typename types<T>::list result(1, t0);
  return move(result.append(t1));
}

///////////////////////////////////////////////////////////////////////////
// append
template <typename C, typename T>
inline C append(C c, const T& t) {
  c.insert(end(c), t);
  return move(c);
}

///////////////////////////////////////////////////////////////////////////
// cons
template <typename T, typename C>
inline C cons(const T& t, const C& c) {
  return concat( append(C(), t), c );
}
template <typename T>
inline std::deque<T> cons(const T& t, std::deque<T> c) {
  c.push_front(t);
  return move(c);
}
inline string cons(char t, string s) {
  return move(s.insert(0, 1, t));
}
inline string cons(string s0, const string& s1) {
  return move(s0.append(s1));
}

///////////////////////////////////////////////////////////////////////////
// reverse

#if 0
template <typename C>
inline C reverse( const C& c ) {
  C result( c );
  std::reverse( extent(c) );
  return c;
}
#else
template <typename C>
inline C reverse(const C& c) {
  return foldl( flip( cons<value_type_of(C),C> ), C(), c );
}
#endif

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

#if 0
template<typename T, typename U>
inline nonconstref_type_of(T) fst(pair<T,U>&& p) {
  return move(p.first);
}
template <typename T, typename U>
inline nonconstref_type_of(U) snd(pair<T,U>&& p) {
  return move(p.second);
}
#endif

template<typename T, typename U>
inline pair<U,T> swap(const pair<T,U>& p) {
  return make_pair(p.first,p.second);
}

template<typename F0, typename F1, typename T, typename U>
inline auto mapPair(F0 f0, F1 f1, const std::pair<T,U>& p) FP_RETURNS( make_pair( f0(p.first), f1(p.second) ) );

template<typename F, typename T, typename U>
inline auto mapFst(F f, const std::pair<T,U>& p) FP_RETURNS( make_pair( f(p.first), p.second ) );
FP_DEFINE_CURRIED(mapFst, mapFst_);

///////////////////////////////////////////////////////////////////////////
// mapSnd

template<typename F, typename T, typename U>
inline auto mapSnd(F f, const std::pair<T,U>& p) FP_RETURNS( make_pair( p.first, f(p.second) ) );
template<typename F0>
struct mapSndF {
  mapSndF(F0 f0_) : f0(f0_) { }
  template<typename T, typename U>
  inline auto operator()(const pair<T,U>& t) -> std::pair<T, decltype( declval<F0>()(snd(t)) )> {
    return make_pair( fst(t), f0(snd(t)) );
  }

  F0 f0;
};
template<typename F0>
mapSndF<F0> mapSndF_(F0 f0) { return mapSndF<F0>(f0); }

///////////////////////////////////////////////////////////////////////////
// mapArrow

template<typename F0, typename F1, typename T>
inline auto mapArrow(F0 f0, F1 f1, const T& t) FP_RETURNS( make_pair( f0(t), f1(t) ) );

// TODO: Factor this out into some nice common code for re-use
template<typename F0, typename F1>
struct mapArrowF {
  mapArrowF(F0 f0_, F1 f1_) : f0(f0_), f1(f1_) { }
  template<typename T>
  inline auto operator()(const T& t) -> std::pair<decltype( declval<F0>()(t) ), decltype( declval<F1>()(t) )> {
    return make_pair( f0(t), f1(t) );
  }

  F0 f0;
  F1 f1;
};
template<typename F0, typename F1>
mapArrowF<F0,F1> mapArrowF_(F0 f0, F1 f1) { return mapArrowF<F0,F1>(f0,f1); }

} /* namespace fp */

#endif /* _FP_PRELUDE_LISTS_H_ */

/////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2012, Jared Duke.
// This code is released under the MIT License.
// www.opensource.org/licenses/mit-license.php
/////////////////////////////////////////////////////////////////////////////

#ifndef _FP_PRELUDE_LAZY_H_
#define _FP_PRELUDE_LAZY_H_

#include "fp_common.h"
#include "fp_prelude_lists.h"

namespace fp {

///////////////////////////////////////////////////////////////////////////
// map

template<typename F, typename T>
inline auto map(F f, thunk<T> t) -> thunk< decltype(f(t())) > {
  return [=]() mutable { return f(t()); };
}

///////////////////////////////////////////////////////////////////////////
// filter

template<typename F, typename T>
inline thunk<T> filter(F f, thunk<T> t) {
  return [=]() -> T {
    let value = t();
    while ( !f(value) ) {
      value = t();
    }
    return value;
  };
}

//////////////////////////////////////////////////////////////////////////
// scanl

template<typename F, typename T, typename T1>
thunk<T> scanl(F f, T t0, thunk<T1> t) {
  return [=]() mutable -> T {
    let result = t0;
    t0 = f(result, t());
    return result;
  };
}

//////////////////////////////////////////////////////////////////////////
// scanl1

template<typename F,  typename T>
thunk<T> scanl1(F f, thunk<T> t) {
  let t0 = t();
  return scanl(f, t0, t);
}

/////////////////////////////////////////////////////////////////////////////
// zipWith

template <typename F, typename T, typename U>
inline auto zipWith(F f, thunk<T> t, thunk<U> u) -> thunk< decltype( f(t(),u()) ) > {
  return [=]() mutable -> decltype( f(t(), u()) ) {
    return f(t(), u());
  };
}

template <typename F, typename T, typename U, typename V>
inline auto zipWith3(F f, thunk<T> t, thunk<U> u, thunk<V> v) -> thunk< decltype( f(t(),u(),v()) ) > {
  return [=]() mutable -> decltype( f(t(),u(),v()) ) {
    return f(t(), u());
  };
}

///////////////////////////////////////////////////////////////////////////
// zip

#if 0
template<typename T, typename U>
inline thunk< std::pair<T,U> > zip(thunk<T> t, thunk<U> u) {
  return zipWith( &make_pair<T,U>, t, u );
}

template<typename T, typename U, typename V>
inline thunk< std::pair<T,U> > zip(thunk<T> t, thunk<U> u, thunk<V> v) {
  return zipWith( &make_tuple<T,U,V>, t, u, v );
}
#endif

///////////////////////////////////////////////////////////////////////////
// dropWhile

template <typename F, typename T>
inline thunk<T> dropWhile(F f, thunk<T> t) {
  bool dropped = false;
  return [=]() mutable -> T {
    // If no values have been dropped, drop while f holds
    if (!dropped) {
      for ( const let& result = t(); !f(result); result = t() ) ;
      dropped = true;
      return result;
    }
    return t();
  };
}

///////////////////////////////////////////////////////////////////////////
// drop

template <typename T>
inline thunk<T> drop(size_t n, thunk<T> t) {
  let dropN = [=](...) mutable {
    return n-- > 0;
  };
  return dropWhile(dropN, t);
}

///////////////////////////////////////////////////////////////////////////
// splitAt

// Does this even make sense?
template <typename T>
inline pair< thunk<T>, thunk<T> > splitAt(size_t n, thunk<T> t) {
  return make_pair( t, drop(n, t) );
}

///////////////////////////////////////////////////////////////////////////
// span

template <typename F, typename T>
inline pair< thunk<T>, thunk<T> > span(F f, thunk<T> t) {
  return std::make_pair( t,  dropWhile(f, t) );
}

///////////////////////////////////////////////////////////////////////////
// break

template <typename F, typename T>
inline std::pair< thunk<T>, thunk<T> > spanNot(F f, thunk<T> t) {
  return span( std::not1(f), t );
}

///////////////////////////////////////////////////////////////////////////
// index

template<typename Index, typename T>
inline T index(Index i, thunk<T> t) {
  while( i-- > 0 ) t();
  return t();
}

#if defined(TODO_IMPLEMENT_LAZY)

///////////////////////////////////////////////////////////////////////////
// concat
template <typename T>
inline fp_enable_if_container(T,T)
concat(const T& t0, const T& t1) {
  T result(t0);
  std::copy(extent(t1), back(result));
  return result;
}
template <typename T>
inline fp_enable_if_not_container(T,typename types<T>::list)
concat(const T& t0, const T& t1) {
  typename types<T>::list result(1, t0);
  return result.append(t1);
}
FP_DEFINE_FUNCTION_OBJECT(concat, concatF);

///////////////////////////////////////////////////////////////////////////
// append
template <typename C, typename T>
inline C append(C c, const T& t) {
  c.insert(end(c), t);
  return c;
}
FP_DEFINE_FUNCTION_OBJECT(append, appendF);

///////////////////////////////////////////////////////////////////////////
// cons
template <typename T, typename C>
inline C cons(const T& t, const C& c) {
  return concat( append(C(), t), c );
}
template <typename T>
inline std::deque<T> cons(const T& t, std::deque<T> c) {
  c.push_front(t);
  return c;
}
inline string cons(char t, string s) {
  return s.insert(0, 1, t);
}
inline string cons(string s0, const string& s1) {
  return s0.append(s1);
}
FP_DEFINE_FUNCTION_OBJECT(cons, consF);

///////////////////////////////////////////////////////////////////////////
// Pair/Tuple

using std::pair;
using std::make_pair;

template<typename T, typename U>
inline nonconstref_type_of(T) fst(const pair<T,U>& p) {
  return p.first;
}
FP_DEFINE_FUNCTION_OBJECT(fst, fstF);

template <typename T, typename U>
inline nonconstref_type_of(U) snd(const pair<T,U>& p) {
  return p.second;
}
FP_DEFINE_FUNCTION_OBJECT(snd, sndF);

template<typename T, typename U>
inline pair<U,T> swap(const pair<T,U>& p) {
  return make_pair(p.first,p.second);
}

template<typename F0, typename F1, typename T, typename U>
inline auto mapPair(F0 f0, F1 f1, const std::pair<T,U>& p) FP_RETURNS( make_pair( f0(p.first), f1(p.second) ) );

template<typename F, typename T, typename U>
inline auto mapFst(F f, const std::pair<T,U>& p) FP_RETURNS( make_pair( f(p.first), p.second ) );

template<typename F0, typename F1, typename T>
inline auto mapArrow(F0 f0, F1 f1, const T& T) FP_RETURNS( make_pair( f0(t), f1(t) ) );

// TODO: Factor this out into some nice common code for re-use
template<typename F0, typename F1>
struct mapArrowF {
  mapArrowF(F0 f0_, F1 f1_) : f0(f0_), f1(f1_) { }
  template<typename T>
  inline auto operator()(const T& t) FP_RETURNS( make_pair( f0(t), f1(t) ) );

  F0 f0;
  F1 f1;
};
template<typename F0, typename F1>
mapArrowF<F0,F1> mapArrowF_(F0 f0, F1 f1) { return mapArrowF<F0,F1>(f0,f1); }

#endif /* defined(TODO_IMPLEMENT_LAZY) */

} /* namespace fp */

#endif /* _FP_PRELUDE_LAZY_H_ */

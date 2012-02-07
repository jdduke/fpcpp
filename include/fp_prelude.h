#ifndef _FP_PRELUDE_H_
#define _FP_PRELUDE_H_

#include "fp_defines.h"
#include "fp_curry.h"

#include <algorithm>
#include <numeric>
#include <map>
#include <sstream>
#include <fstream>
#include <tuple>

namespace fp {

///////////////////////////////////////////////////////////////////////////
// map

template<typename F, typename C, typename R>
inline R& __map__(F f, const C& c, R& r) {
  std::transform(extent(c), back(r), f);
  return r;
}

template<typename F, typename C>
inline auto map(F f, const C& c) -> std::vector< typename fc::function_traits<F>::result_type > {
  typedef std::vector< typename fc::function_traits<F>::result_type > result_type;
  return __map__(f, c, result_type());
}
template<typename F>
inline std::string map(F f, const std::string& s) {
  let charVec = __map__(f, std::vector<char>(extent(s)), std::string());
  return std::string(extent(charVec));
}
template<typename F>
inline std::string map(F f, const char* s) {
  return map(f, std::string(s));
}
//FP_DEFINE_FUNC_OBJ(map, map_, _map_);

///////////////////////////////////////////////////////////////////////////
// fold

template<class It, class Op>
auto fold(It first, It last, Op op) -> typename std::iterator_traits<It>::value_type {
  if (first != last) {
    auto value = *first;
    return std::accumulate(++first, last, value, op);
  } else {
    return typename std::iterator_traits<It>::value_type();
  }
}

template<class C>
auto fold(const C& c) -> value_type_of(C) {
  return std::accumulate(extent(c), value_type_of(C)());
}

template<typename C, typename T, typename Op>
T fold_with(const C& c, T t, Op op) {
  return std::accumulate(extent(c), t, op);
}

/////////////////////////////////////////////////////////////////////////////
// foldl

template<typename F, typename C>
inline auto foldl(F f, const C& c) -> value_type_of(C) {
  return fold(extent(c), f);
}
FP_DEFINE_FUNC_OBJ_T(foldl, foldl_, _foldl_);

/////////////////////////////////////////////////////////////////////////////
// foldr

template<typename F, typename C>
inline auto foldr(F f, const C& c) -> value_type_of(C) {
  return fold(rextent(c), f);
}
FP_DEFINE_FUNC_OBJ_T(foldr, foldr_, _foldr_);

///////////////////////////////////////////////////////////////////////////
// filter

template<typename F, typename T>
inline T filter(F f, T t) {
  T result;
  std::copy_if(head(t),
               tail(t),
               back(result),
               f);
  return result;
}
FP_DEFINE_FUNC_OBJ(filter, filter_, _filter_);

/////////////////////////////////////////////////////////////////////////////
// zipWith

template<typename F, typename T, typename U, typename R>
inline R& __zipWith__(F f, const T& t, const U& u, R& r) {
  std::transform(extent(t), head(u), back(r), f);
  return r;
}

template<typename F, typename T, typename U>
inline T zipWith(F f, const T& t, const U& u) {
  return __zipWith__(f, t, u, T());
}
FP_DEFINE_FUNC_OBJ(zipWith, zipWith_, _zipWith_)

///////////////////////////////////////////////////////////////////////////
// zip

template<typename C1, typename C2>
inline std::vector< typename tuple_traits<C1,C2>::type > zip(const C1& c1, const C2& c2) {
  typedef typename traits<C1>::value_type T;
  typedef typename traits<C2>::value_type U;
  typedef typename tuple_traits<C1,C2>::type pair_type;
  typedef std::vector< pair_type > result_type;
  return __zipWith__([](const T& t, const U& u) -> pair_type { return std::make_pair(t,u); }, c1, c2, result_type());
}

///////////////////////////////////////////////////////////////////////////
// all

template<typename F, typename C>
inline bool all(F f, const C& c) {
  return std::all_of(extent(c), f);
}

///////////////////////////////////////////////////////////////////////////
// maximum

template<typename C>
inline value_type_of(C) maximum(const C& c) {
  return iter_value( std::max_element(extent(c)), c);
}

///////////////////////////////////////////////////////////////////////////
// minimum

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
// elem

#if 1

template<typename T, typename C>
inline bool elem(const T& t, const C& c) {
  return std::find(extent(c), t) != tail(c);
}

#else

template<typename T, typename C>
inline typename std::enable_if< !has_find<C,T>::value, bool >::value elem(const T& t, const C& c) {
  return std::find(extent(c), t) != tail(c);
}

template<typename T, typename C>
inline typename std::enable_if< has_find<C,T>::value, bool>::value elem(const T& t, const C& c) {
  return c.find(t) != tail(c);
}

#endif

///////////////////////////////////////////////////////////////////////////
// notElem

template<typename T, typename C>
inline bool notElem(const T& t, const C& c) {
  return !elem(t, c);
}

template<typename InputIt, typename ResultIt, typename F>
inline void copyWhile(InputIt first, InputIt last, ResultIt result, F& f) {
  while ((first != last) && f(*first)) {
    *result++ = *first++;
  }
}

///////////////////////////////////////////////////////////////////////////
// takeWhile

template<typename F, typename T>
inline T takeWhile(F f, const T& t) {
  T result;
  //std::copy(head(t), std::find_if(extent(t), std::not1<F>(f)), back(result));
  copyWhile(extent(t), back(result), f);
  return result;
}
FP_DEFINE_FUNC_OBJ(takeWhile, takeWhile_, _takeWhile_);

template<typename F, typename F2>
inline auto takeWhileF(F f, F2 f2) -> std::vector< decltype(f2()) > {
  typedef decltype(f2()) t_type;
  std::vector<t_type> result;
  auto   backR = back(result);
  for (t_type value = f2(); f(value); value=f2())
    backR = value;
  return result;
}
FP_DEFINE_FUNC_OBJ(takeWhileF, takeWhileF_, _takeWhileF_);

///////////////////////////////////////////////////////////////////////////
// take
template<typename C>
inline C take(size_t n, const C& c) {
  let takeN = [=](const traits<C>::value_type&) { return n-- > 0; }
  return takeWhile(takeN, c);
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

///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////

template<typename T>
inline std::vector<T>& split_helper(const T& s, char delim, std::vector<T>& elems) {
  std::stringstream ss(s);
  T item;
  while(std::getline(ss, item, delim)) {
    elems.push_back(item);
  }
  return elems;
}

template<typename T>
inline std::vector<T> split(const T& s, char delim) {
  std::vector<T> elems;
  return split_helper(s, delim, elems);
};

template<typename T>
typename T concat(const std::vector<T>& elems, char delim = ' ') {
  std::stringstream ss;
  std::for_each(head(elems), tail(elems), [=,&ss](const T& s) {
    ss << s << delim;
  });
  let str = ss.str();
  return str.length() > 0 ? str.substr(0, str.length()-1) : "";
}

inline bool istrue(bool b) { return b; }

///////////////////////////////////////////////////////////////////////////
// lines

template<typename T>
inline std::vector<T> lines(const T& s) {
  return split(s, '\n');
}
inline std::vector<std::string> lines(std::ifstream ifs) {
  std::vector<std::string> ifsLines;
  std::string line;
  while (getline(ifs, line)) 
    ifsLines.push_back(line);
  return ifsLines;
}


///////////////////////////////////////////////////////////////////////////
// unlines

template<typename C>
auto unlines(const C& elems) -> value_type_of(C) {
  return concat(elems, '\n');
}

///////////////////////////////////////////////////////////////////////////
// words
template<typename T>
inline std::vector<T> words(const T& s) {
  return split(s, ' ');
}

///////////////////////////////////////////////////////////////////////////
// unwords

template<typename T>
auto unwords(const T& elems) -> decltype(concat(elems, ' ')) {
  return concat(elems, ' ');
}

///////////////////////////////////////////////////////////////////////////
// generate_n
template<typename F>
inline auto generate_n(size_t n, F f) -> std::vector< decltype(f()) > {
  std::vector< decltype(f()) > t(n);
  std::generate_n(head(t), n, f);
  return t;
}

// ///////////////////////////////////////////////////////////////////////////
// rand_range

template<typename T>
inline T rand_range(T t0 = std::numeric_limits<T>::min(), T t1 = std::numeric_limits<T>::max()) {
  return t0 + static_cast<T>(static_cast<double>(rand())/(RAND_MAX+1)) * (t1 - t0);
}
template<typename T>
struct rand_range_ {
  rand_range_(T t0_ = std::numeric_limits<T>::min(), T t1_ = std::numeric_limits<T>::max()) : t0(t0_), t1(t1_) { }
  T operator()() { return rand_range<T>(t0,t1); }
  T t0, t1;
};

///////////////////////////////////////////////////////////////////////////
// rand_n
template<typename T>
inline std::vector<T> rand_n(size_t n, T t0 = std::numeric_limits<T>::min(), T t1 = std::numeric_limits<T>::max()) {
  return generate_n(n, rand_range_<T>(t0,t1));
}

///////////////////////////////////////////////////////////////////////////
// iterate
template<typename F, typename T>
inline std::function< T(void) > iterate(F f, T t) {
  return [=]() mutable -> T { T r = t; t = f(t); return r; };
}

///////////////////////////////////////////////////////////////////////////
// succ
template<typename T>
inline T succ(const T& t) {
  return static_cast<T>(t + 1);
}

///////////////////////////////////////////////////////////////////////////
// succ
template<typename T>
inline T pred(const T& t) {
  return static_cast<T>(t - 1);
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
inline typename std::enable_if<is_container<C>::value && !is_container<T>::value, C>::type
append(const C& c, const T& t) {
  C result(c);
  result.insert(tail(result), t);
  return result;
}

///////////////////////////////////////////////////////////////////////////
// show
template<typename T>
inline typename std::enable_if<!is_container<T>::value, std::string>::type show(const T& t) {
  std::stringstream ss;
  ss << t;
  return ss.str();
}

template<typename C>
inline typename std::enable_if<is_container<C>::value, std::string>::type show(const C& c) {
  return concat(c, ' ');
}

std::string show(const std::vector<char>& c) {
  return std::string(head(c), tail(c));
}

///////////////////////////////////////////////////////////////////////////
// cons
template<typename T, typename C>
inline typename std::enable_if<!is_container<T>::value && is_container<C>::value, C>::type
cons(const T& t, const C& c) {
  return concat( append(C(), t), c );
}

} /* namespace fp */

#endif /* _FP_PRELUDE_H_ */

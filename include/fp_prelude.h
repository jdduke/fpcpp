#ifndef _FP_PRELUDE_H_
#define _FP_PRELUDE_H_

#include "fp_defines.h"

#include <algorithm>
#include <numeric>

namespace fp {

namespace prelude {

///////////////////////////////////////////////////////////////////////////
// map

template<typename F, typename T>
inline T map(F& f, const T& t) {
  T result;
  std::transform(head(t), tail(t), back(result), f);
  return result;
}
FP_DEFINE_FUNC_OBJ(map, map_, _map_);

/////////////////////////////////////////////////////////////////////////////
// foldl

template<typename F, typename T>
inline typename traits<T>::value foldl(F& f, const T& t, typename traits<T>::value t0 = typename traits<T>::value()) {
  return std::accumulate(head(t), tail(t), t0, f);
}
FP_DEFINE_FUNC_OBJ_T(foldl, foldl_, _foldl_);

/////////////////////////////////////////////////////////////////////////////
// foldr

template<typename F, typename T>
inline typename traits<T>::value foldr(F& f, const T& t, typename traits<T>::value t0 = typename traits<T>::value()) {
  return std::accumulate(rhead(t), rtail(t), t0, f);
}
FP_DEFINE_FUNC_OBJ_T(foldr, foldr_, _foldr_);

///////////////////////////////////////////////////////////////////////////
// filter

template<typename F, typename T>
inline T filter(F& f, const T& t) {
  T result;
  return std::copy_if(head(t), tail(t), back(result), f);
  return result;
}
FP_DEFINE_FUNC_OBJ(filter, filter_, _filter_);

/////////////////////////////////////////////////////////////////////////////
// zipWith

template<typename F, typename T, typename U, typename R>
inline void __zipWith__(F& f, const T& t, const U& u, R& r) {
  std::transform(head(t), tail(t), head(u), back(r), f);
}

template<typename F, typename T, typename U>
inline T zipWith(F& f, const T& t, const U& u) {
  T result;
  __zipWith__(f, t, u, result);
  return result;
}
FP_DEFINE_FUNC_OBJ(zipWith, zipWith_, _zipWith_)

///////////////////////////////////////////////////////////////////////////
// zip

template<typename T, typename U>
inline T zip(const T& t, const U& u) {
  typedef typename traits<T>::value t_val;
  typedef typename traits<U>::value u_val;
  typedef std::vector< std::tuple<t_val,u_val> > result_type;
  result_type r;
  return zipWith(&std::make_tuple<t_val,u_val>,t,u,r);
  return r;
}

///////////////////////////////////////////////////////////////////////////
// maximum

template<typename T>
inline typename traits<T>::value maximum(const T& t) {
  return valueIn( std::max_element(head(t), tail(t)), t);
}

///////////////////////////////////////////////////////////////////////////
// reverse

template<typename T>
inline T reverse(const T& t_) {
  T t(t_);
  std::reverse(head(t), tail(t));
  return t;
}

///////////////////////////////////////////////////////////////////////////
// elem

template<typename T, typename C>
inline bool elem(const T& t, const C& c) {
  return std::find(head(c), tail(c), t) != tail(c);
}

///////////////////////////////////////////////////////////////////////////
// notElem

template<typename T, typename C>
inline bool notElem(const T& t, const C& c) {
  return !elem(t, c);
}

///////////////////////////////////////////////////////////////////////////
// dropWhile

template<typename F, typename T>
inline T dropWhile(F& f, const T& t) {
  T result;
  std::copy_if(head(t), tail(t), back(result), std::not1<F>(f));
  return result;
}
FP_DEFINE_FUNC_OBJ(dropWhile, dropWhile_, _dropWhile_);

///////////////////////////////////////////////////////////////////////////
// drop

template<typename T>
inline T drop(size_t n, const T& t) {
  let dropN = [&](const traits<T>::value&) { return n-- > 0; }
  return dropWhile(dropN, t);
}

///////////////////////////////////////////////////////////////////////////
// takeWhile

template<typename F, typename T>
inline T takeWhile(F& f, const T& t) {
  T result;
  std::copy_if(head(t), tail(t), back(result), f);
  return result;
}
FP_DEFINE_FUNC_OBJ(takeWhile, takeWhile_, _takeWhile_); 

///////////////////////////////////////////////////////////////////////////
// take
template<typename T>
inline T take(size_t n, const T& t) {
  let takeN = [&](const traits<T>::value&) { return n-- > 0; }
  return takeWhile(takeN, t);
}

///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////

template<typename T, typename Ts>
inline Ts& split_helper(const T& s, char delim, Ts& elems) {
  stringstream ss(s);
  Ts item;
  while(std::getline(ss, item, delim)) {
    elems.push_back(item);
  }
  return elems;
}

template<typename T, typename Ts>
inline Ts split(const T& s, char delim) {
  Ts elems;
  return split_helper(s, delim, elems);
};

template<typename Ts>
typename traits<Ts>::value_type concat(const Ts& elems, char delim=' ') {
  std::stringstream ss;
  std::for_each(head(elems), tail(elems), [&ss](const traits<Ts>::value_type& s) {
    ss << s << delim;
  });
  return ss.str();
}

///////////////////////////////////////////////////////////////////////////
// lines

template<typename T> 
inline std::vector<T> lines(const T& s) {
  return split(s, '\n');
}

///////////////////////////////////////////////////////////////////////////
// unlines

template<typename Ts>
typename traits<Ts>::value_type unlines(const Ts& elems) {
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

template<typename Ts>
typename traits<Ts>::value_type unwords(const Ts& elems) {
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
  return t0 + static_cast<T>(static_cast<double>(rand())/(RAND_MAX+1)) * (t1 - t0 + (T)1);
}
template<typename T>
struct rand_range_ {
  rand_range_(T t0 = std::numeric_limits<T>::min(), T t1 = std::numeric_limits<T>::max()) : t0(t0_), t1(t1_) { }
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

} /* namespace prelude */

} /* namespace fp */

#endif /* _FP_PRELUDE_H_ */

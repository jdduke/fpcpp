#ifndef _FP_PRELUDE_H_
#define _FP_PRELUDE_H_

namespace fp {

namespace prelude {

template<typename F, typename T>
inline T _map(F& f, const T& t) {
  T result;
  std::transform(head(t), tail(t), back(result), f);
  return result;
}

template<typename F>
struct map {
  map(F f_) : f(f_) { }
  template<typename T>
  T operator()(const T& t) {
    return _map(f,t);
  }
};
template<typename F, typename T>
struct map_ : map<F> {
  map_(F f_) : map(f_) { }
  T operator()(const T& t) {
    return map::operator()(t);
  }
};

/////////////////////////////////////////////////////////////////////////////

template<typename F, typename T>
  inline T _fold(F& f, const T& t, typename traits<T>::value t0 = typename traits<T>::value()) {
  return std::accumulate(head(t), tail(t), t0, f);
}

template<typename F>
struct fold {
  fold(F f_) : f(f_) { }
  template<typename T>
  T operator()(const T& t, typename traits<T>::value t0 = typename traits<T>::value()) {
    return _fold(f, t, t0);
  }
};

template<typename F, typename T>
struct fold_ : fold<F> {
  typedef typename traits<T>::value value_type;
  fold_(F f_) : fold(f_) { }
  T operator()(const T& t, value_type t0 = value_type()) {
    return fold::operator ()(t, t0);
  }
};

///////////////////////////////////////////////////////////////////////////

template<typename F, typename T>
inline T _filter(F& f, const T& t) {
  T result;
  return std::copy_if(head(t), tail(t), back(result), f);
  return result;
}
template<typename F>
struct filter {
  filter(F f_) : f(f_) { }
  template<typename T>
  T operator()(const T& t) {
    return _filter(f,t);
  }
};
template<typename F, typename T>
struct filter_ : filter<F> {
  filter_(F f_) : filter(f_) { }
  T operator()(const T& t) {
    return filter::operator()(t);
  }
};

/////////////////////////////////////////////////////////////////////////////

template<typename F, typename T, typename U, typename R>
inline void __zipWith(F& f, const T& t, const U& u, R& r) {
  std::transform(head(t), tail(t), head(u), back(r), f);
}

template<typename F, typename T, typename U>
inline T _zipWith(F& f, const T& t, const U& u) {
  T result;
  __zipWith(f, t, u, result);
  return result;
}

template<typename F>
struct zipWith {
  map(F f_) : f(f_) { }
  template<typename T>
  T operator()(const T& t, const U& u) {
    return _zipWith(f,t,u);
  }
};
template<typename F, typename T>
struct zipWith_ : zipWith<F> {
  zipWith_(F f_) : zipWith(f_) { }
  T operator()(const T& t) {
    return zipWith::operator()(t);
  }
};

template<typename T, typename U>
inline T _zip(const T& t, const U& u) {
  typedef typename traits<T>::value t_val;
  typedef typename traits<U>::value u_val;
  typedef std::vector< std::tuple<t_val,u_val> > result_type;
  result_type r;
  return __zipWith(&std::make_tuple<t_val,u_val>,t,u,r);
  return r;
}

/////////////////////////////////////////////////////////////////////////////

template<typename T>
inline typename traits<T>::value _maximum(const T& t) {
  return valueIn( std::max_element(head(t), tail(t)), t);
}

} /* namespace prelude */

} /* namespace fp */

#endif /* _FP_PRELUDE_H_ */

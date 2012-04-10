#ifndef _FP_MAYBE_H_
#define _FP_MAYBE_H_

namespace fp {

class Nothing { };

template <typename T>
class Maybe : public std::pair<bool, T> {
public:
  typedef std::pair<bool, T> pair;

  /////////////////////////////////////////////////////////////////////////
  // Construction

  Maybe()                       : pair(false, T())              { }
  Maybe(const Maybe& other)     : pair(other)                   { }
  Maybe(Maybe&& other)          : pair(std::move(other))        { }
  Maybe(Nothing)                : pair(false, T())              { }
  Maybe(const T& value)         : pair(true,  value)            { }
  Maybe(T&& value)              : pair(true,  std::move(value)) { }
  Maybe(bool valid, const T& t) : pair(valid, t)                { }


  /////////////////////////////////////////////////////////////////////////
  // Assignment

  Maybe& operator=(const Maybe& o) { *static_cast<pair*>(this) = o;            return *this; }
  Maybe& operator=(Maybe&& o)      { *static_cast<pair*>(this) = std::move(o); return *this; }

  /////////////////////////////////////////////////////////////////////////
  // Accessors

  inline bool     valid() const { return pair::first; }
  inline operator bool()  const { return pair::first; }

  inline const T& get()       const  { check(); return pair::second; }
  inline       T& get()              { check(); return pair::second; }
  inline const T& operator*() const  { check(); return pair::second; }
  inline       T& operator*()        { check(); return pair::second; }

private:
  inline void check() const { /* TODO: throw; */ }
};


template <typename T>
bool operator==(const Maybe<T>& opt, Nothing) { return !opt.valid(); }

/////////////////////////////////////////////////////////////////////////
// just

template <typename T>
Maybe<T> just(const T& t) { return Maybe<T>(t); }

template <typename T>
Maybe<T> just(T&& t)      { return Maybe<T>(std::move(t)); }


/////////////////////////////////////////////////////////////////////////
// isNothing

template <typename T>
bool isNothing(const Maybe<T>& t) { return !t.valid(); }

template <typename T>
bool isNothing(Maybe<T>&& t)      { return !t.valid(); }


/////////////////////////////////////////////////////////////////////////
// isJust

template <typename T>
bool isJust(const Maybe<T>& t) { return t.valid(); }

template <typename T>
bool isJust(Maybe<T>&& t)      { return t.valid(); }


/////////////////////////////////////////////////////////////////////////
// fromJust

template <typename T>
T fromJust(const Maybe<T>& opt) { return *opt; }

template <typename T>
T fromJust(Maybe<T>&& opt)      { return *opt; }


/////////////////////////////////////////////////////////////////////////
// fromMaybe

template <typename T>
T fromMaybe(const Maybe<T>& opt) { return isJust(opt) ? *opt : T(); }

template <typename T>
T fromMaybe(Maybe<T>&& opt)      { return isJust(opt) ? *opt : T(); }

} // namespace fp

#endif // _FP_MAYBE_H_

#include <functional>
#include <iostream>
#include <cmath>

void simple();

int main(int argc, char** argv) {

  simple();

  return 0;

}


#if defined(_MSC_VER)
template <typename T>
typename std::add_rvalue_reference<T>::type declval();
#endif

template<typename F, typename G>
struct Wrapper {
  Wrapper(F f, G g): mF(f), mG(g) { }

#if !defined(_MSC_VER)

  template< typename... Args >
  inline auto operator()(Args... args) -> decltype(std::declval<F>()(std::declval<G>()(args...))) {
    return mF(mG(args...));
  }

#else

  template< typename Arg0 >
  inline auto operator()(Arg0 arg0) -> decltype(declval<F>()(declval<G>()(arg0))) {
    return mF(mG(arg0));
  }

  template< typename Arg0, typename Arg1 >
  inline auto operator()(Arg0 arg0, Arg1 arg1) -> decltype(declval<F>()(declval<G>()(arg0,arg1))) {
    return mF(mG(arg0, arg1));
  }

  template< typename Arg0, typename Arg1, typename Arg2 >
  inline auto operator()(Arg0 arg0, Arg1 arg1, Arg2 arg2) -> decltype(declval<F>()(declval<G>()(arg0, arg1, arg2))) {
    return mF(mG(arg0, arg1, arg2));
  }

#endif

  F mF;
  G mG;
};

template< typename F, typename G >
Wrapper<F,G> compose(F f, G g) {
  return Wrapper<F,G>(f, g);
}

void simple() {

  std::cout << compose(sinf, asinf)(0.5f) << std::endl;

}

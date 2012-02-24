/////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2012, Jared Duke.
// This code is released under the MIT License.
// www.opensource.org/licenses/mit-license.php
/////////////////////////////////////////////////////////////////////////////

#if defined(_MSC_VER)
#pragma warning( disable : 4244 )
#endif

#include <algorithm>
#include <numeric>
#include <iostream>
#include <string>
#include <sstream>

#include <fpcpp.h>

#include "timer.h"

///////////////////////////////////////////////////////////////////////////

static const size_t RAND_VEC_SIZE  = 1000;
static const float  RAND_VEC_RANGE = 10.0f;
#if defined(_DEBUG)
static const size_t  VEC_ITERS     = 1000;
#else
static const size_t  VEC_ITERS     = 100000;
#endif

///////////////////////////////////////////////////////////////////////////

template < class InputIterator1, class InputIterator2, class InputIterator3, class InputIterator4, class OutputIterator, class Op>
OutputIterator transform_4(InputIterator1 first1, InputIterator1 last1,
                          InputIterator2 first2,
                          InputIterator3 first3,
                          InputIterator4 first4,
                          OutputIterator result, Op op ) {
  while (first1 != last1)
    *result++ = op(*first1++, *first2++, *first3++, *first4++);
  return result;
}

///////////////////////////////////////////////////////////////////////////

float test_func1(float x, float y, float z, float w) {
  return (x * y) + (z * w);
}

float test_func2(float x, float y, float z, float w) {
  return (x + y + z) * (2.0f * w);
}

float test_func3() {
  return logf(sqrtf(fp::randRange(0.f, RAND_VEC_RANGE) * fp::randRange(0.f, RAND_VEC_RANGE)));
}

float test_func4(float x) {
  return ((((((((((((((((x + 2.f) - 3.f) * 4.f) / 5.f) + 2.f) - 3.f) * 4.f) / 5.f) + 2.f) - 3.f) * 4.f) / 5.f) + 2.f) - 3.f) * 4.f) / 5.f);
}

template<typename T, typename F>
void benchmark(T& data, T& result, F f, const char* desc, size_t iters = VEC_ITERS) {
  timed_run timed(desc);
  for (size_t i = 0; i < iters; ++i)
    transform_4(
      std::begin(data),
      std::end(  data),
      std::begin(data),
      std::begin(data),
      std::begin(data),
      std::begin(result),
      f);
}

template<typename F>
void benchmark2(F f, const char* desc, size_t iters = VEC_ITERS) {
  timed_run timed(desc);
  for (size_t i = 0; i < iters; ++i)
    f();
}

template<typename T, typename F>
void benchmark3(T& data, T& result, F f, const char* desc, size_t iters = VEC_ITERS) {
  timed_run timed(desc);
  for (size_t i = 0; i < iters; ++i)
    transform(
      std::begin(data),
      std::end(  data),
      std::begin(result),
      f);
}

void test() {

#if FP_COMPOUND

  using namespace fp;
  using namespace fp_operators;
  let rand_vec = fp::randN(RAND_VEC_SIZE, (float)0, (float)RAND_VEC_RANGE);

  {
    auto add            = [](float x, float y) -> float { return x + y; };
    auto mult           = [](float x, float y) -> float { return x * y; };
    auto add_mult_mult  = compose2(add, mult, mult);
    auto test_lambda1   = [](float x, float y, float z, float w) -> float {
      return (x * y) + (z * w);
    };

    fp::types<float>::list result(rand_vec.size());
    fp::types<float>::list test_result(rand_vec.size());

    // warmup
    benchmark(rand_vec, result,      test_lambda1, "Warmup - ");

    benchmark(rand_vec, result,      add_mult_mult, "COMPOSED: (x * y) + (z * w) - ");
    benchmark(rand_vec, result,      test_lambda1,  "LAMBDA:   (x * y) + (z * w) - ");
    benchmark(rand_vec, test_result, test_func1,    "FUNC:     (x * y) + (z * w) - ");

    std::cout << "Success = " << std::equal( result.begin(), result.end(), test_result.begin() ) << std::endl;
  }

  {
    //auto length_squared = [](float x, float y, float z) -> float { return x*x + y*y + z*z; };
    auto added           = [](float x, float y, float z) -> float { return x + y + z; };
    auto doubled         = [](float x) -> float { return x * 2.0f; };
    auto multiplied      = [](float x, float y) -> float { return x * y; };
    auto mult_add_double = compose2(multiplied, added, doubled);
    auto test_lambda2    = [](float x, float y, float z, float w) -> float {
      return (x + y + z) * (2.0f * w);
    };

    fp::types<float>::list result(rand_vec.size());
    fp::types<float>::list test_result(rand_vec.size());

    benchmark(rand_vec, result,      mult_add_double, "COMPOSED: (x + y + z) * (2.0f * w) - ");
    benchmark(rand_vec, result,      test_lambda2,    "LAMBDA:   (x + y + z) * (2.0f * w) - ");
    benchmark(rand_vec, test_result, test_func2,      "FUNC:     (x + y + z) * (2.0f * w) - ");

    std::cout << "Success = " << std::equal( result.begin(), result.end(), test_result.begin() ) << std::endl;
  }

  let rand_gen  = fp::curry2(randRange<float>, 0.f, RAND_VEC_RANGE);
  //let rand_gen2 = fp::curry2(randRangeF(),     0.f, RAND_VEC_RANGE);

  {
    //     static inline auto bind2(F f, T t, T1 t1) -> std::function< decltype(f(t,t1)) (T,T1) > {
    //       //FP_RETURNS( std::bind( static_cast< decltype(F) (T,T1) >(f), t, t1) );
    //       typedef decltype(f(t,t1)) result_type;
    // 
    //       return std::bind( static_cast< decltype(F) (T,T1) >(f), t, t1 );
     //let func = randRangeF();
     //typedef std::function< decltype(func(0.f, RAND_VEC_RANGE)) ) result_type;
// 
     let rand_gen2 = fp::curry_helper_impl<2>::bind2(randRangeF(), 0.f, RAND_VEC_RANGE);
     rand_gen2();

  }

  {
    //auto length_squared = [](float x, float y, float z) -> float { return x*x + y*y + z*z; };
    auto multiplied      = [](float x, float y) -> float { return x * y; };
    auto mult_rand_rand  = compose2(multiplied, rand_range_<float>(0.f, RAND_VEC_RANGE), rand_range_<float>(0.f, RAND_VEC_RANGE));
    auto log_sqrt        = [](float x) -> float { return logf(sqrtf(x)); };
    auto test_lambda3    = [&]() -> float { return logf(sqrtf(rand_gen() * rand_gen())); };
    auto sqrt_mult_rand_rand  = log_sqrt + mult_rand_rand;
    auto sqrt_mult_rand_rand2 = [](float x) { return logf(x); }
                              + [](float x) { return sqrtf(x); }
                              + [](float x) { return x*x; }
                              + [&]()       { return rand_gen() * rand_gen(); };

    fp::types<float>::list result(rand_vec.size());
    fp::types<float>::list test_result(rand_vec.size());

    benchmark2(sqrt_mult_rand_rand,  "COMPOSED:  log(sqrt(rand()*rand())) - ", VEC_ITERS * 25);
    benchmark2(sqrt_mult_rand_rand2, "COMPOSED2: log(sqrt(rand()*rand())) - ", VEC_ITERS * 25);
    benchmark2(test_lambda3,         "LAMBDA:    log(sqrt(rand()*rand())) - ", VEC_ITERS * 25);
    benchmark2(test_func3,           "FUNC:      log(Sqrt(rand()*rand())) - ", VEC_ITERS * 25);

    std::cout << "Success = " << std::equal( result.begin(), result.end(), test_result.begin() ) << std::endl;
  }

  {
    auto add_2          = [](float x) -> float { return x + 2.f; };
    auto sub_2          = [](float x) -> float { return x - 3.f; };
    auto mult_2         = [](float x) -> float { return x * 4.f; };
    auto div_2          = [](float x) -> float { return x / 5.f; };
    auto chain          = div_2 + mult_2 + sub_2 + add_2;
    auto chain4         = chain + chain + chain + chain;
    auto test_lambda4   = [](float x) -> float {
      return ((((((((((((((((x + 2.f) - 3.f) * 4.f) / 5.f) + 2.f) - 3.f) * 4.f) / 5.f) + 2.f) - 3.f) * 4.f) / 5.f) + 2.f) - 3.f) * 4.f) / 5.f);
    };

    fp::types<float>::list result(rand_vec.size());
    fp::types<float>::list test_result(rand_vec.size());

    benchmark3(rand_vec, result,      chain4,        "COMPOSED: (f(f(f(f(x)))) with f(x) = ((((x+2)-2)*2)/2) - ");
    benchmark3(rand_vec, result,      test_lambda4,  "LAMBDA:   (f(f(f(f(x)))) with f(x) = ((((x+2)-2)*2)/2) - ");
    benchmark3(rand_vec, test_result, test_func4,    "FUNC:     (f(f(f(f(x)))) with f(x) = ((((x+2)-2)*2)/2) - ");

    std::cout << "Success = " << std::equal( result.begin(), result.end(), test_result.begin() ) << std::endl;
  }

#endif /* FP_COMPOUND */

}

int main(int argc, char** argv) {

  test();

  return 0;
}

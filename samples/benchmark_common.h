/////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2012, Jared Duke.
// This code is released under the MIT License.
// www.opensource.org/licenses/mit-license.php
/////////////////////////////////////////////////////////////////////////////

#ifndef _BENCHMARK_COMMON_H_
#define _BENCHMARK_COMMON_H_

#include "timer.h"

#if defined(_DEBUG)
#define ITER_MULT 5
#else
#define ITER_MULT 5
#endif

#if !defined(ENABLE_BENCHMARK)
#define ENABLE_BENCHMARK 1
#endif

#if ENABLE_BENCHMARK
#define BENCHMARK(desc,func,iters) {     \
    timed_run timed(desc);               \
    for (size_t i = 0; i < iters; ++i) { \
      func;                              \
    }                                    \
  }

#define run_impl(func,iters) BENCHMARK(#func,func,iters)
#define run(func,iters) run_impl(func,iters)
#else
#define run(func,iters) { fp::print(#func); let result = func; fp::print(result); fp::print("");} typedef int FP_CONCAT(RETURNS_,__LINE__)
#endif

#endif /* _BENCHMARK_COMMON_H_ */

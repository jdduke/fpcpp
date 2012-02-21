/////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2012, Jared Duke.
// This code is released under the MIT License.
// www.opensource.org/licenses/mit-license.php
/////////////////////////////////////////////////////////////////////////////

#ifndef _TIMER_H_
#define _TIMER_H_

#include <time.h>
#if defined(_WIN32)
#include <windows.h>
#else
#include <sys/time.h>
#include <unistd.h>
#endif

class sample_timer {
public:

  sample_timer();
  ~sample_timer();
  void restart();
  double elapsed();

private:

#if defined(_WIN32)
  long long int startTime;
  long long int frequency;
#else
  unsigned long startTime[2];
#endif
};

///////////////////////////////////////////////////////////////////////////

#if defined(_WIN32)

sample_timer::sample_timer() {
  DWORD_PTR oldmask = SetThreadAffinityMask(GetCurrentThread(), (DWORD_PTR)1);
  QueryPerformanceFrequency((LARGE_INTEGER *)&this->frequency);
  SetThreadAffinityMask(GetCurrentThread(), oldmask);
  restart();
}

sample_timer::~sample_timer() {

}

void sample_timer::restart() {
  DWORD_PTR oldmask = SetThreadAffinityMask(GetCurrentThread(), (DWORD_PTR)1);
  QueryPerformanceCounter((LARGE_INTEGER *)&this->startTime);
  SetThreadAffinityMask(GetCurrentThread(), oldmask);
}

double sample_timer::elapsed() {
  long long int tempTime;
  DWORD_PTR oldmask = SetThreadAffinityMask(GetCurrentThread(), (DWORD_PTR)1);
  QueryPerformanceCounter((LARGE_INTEGER *)&tempTime);
  SetThreadAffinityMask(GetCurrentThread(), oldmask);
  if (tempTime<startTime)
    return 0;
  return (tempTime - startTime) / (double)this->frequency;
}

#else

static inline void get_clock_count(unsigned long cc[2]) {
  struct timeval tv;
  gettimeofday (&tv,0);
  cc[0] = tv.tv_usec;
  cc[1] = tv.tv_sec;
}

static inline double load_clock_count (unsigned long a[2]) {
  return a[1]*1.0e6 + a[0];
}

double ticks_per_second() {
  return 1000000;
}

sample_timer::sample_timer() {
  restart();
}

sample_timer::~sample_timer() {

}

void sample_timer::restart() {
  get_clock_count(startTime);
}

double sample_timer::elapsed() {
  unsigned long tempTime[2];
  get_clock_count(tempTime);

  double t1 = load_clock_count(startTime);
  double t2 = load_clock_count(tempTime);
  if (t2<t1) {
    return 0;
  }

  return (t2-t1) / ticks_per_second();
}

#endif

struct timed_run {
  timed_run( const char* desc ) : desc(desc) { }
  ~timed_run( ) { std::cout << desc << "\t\t" << timer.elapsed() << " (s)" << std::endl; }
  sample_timer timer;
  const char* desc;
};

#endif

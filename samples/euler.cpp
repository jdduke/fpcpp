/////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2012, Jared Duke.
// This code is released under the MIT License.
// www.opensource.org/licenses/mit-license.php
/////////////////////////////////////////////////////////////////////////////

#include <fpcpp.h>

#include <iostream>

using namespace fp;
using namespace fp::math;

size_t s1();
size_t s2();
size_t s3();
size_t s4();
size_t s5();
size_t s6();
size_t s7();

///////////////////////////////////////////////////////////////////////////

typedef std::function<size_t(void)> solution;

types<solution>::list solutions( ) {
  types<solution>::list ss;
  ss.push_back(s1);
  ss.push_back(s2);
  /*ss.push_back(s3);
  ss.push_back(s4);
  ss.push_back(s5);
  ss.push_back(s6);
  ss.push_back(s7);*/
  return ss;
}

///////////////////////////////////////////////////////////////////////////

int main(int argc, char **argv) {

  size_t x = 0;
  mapV( [=](solution s) mutable { std::cout << x++ << ": " << s() << std::endl; },
        solutions() );

  return 0;
}

///////////////////////////////////////////////////////////////////////////

size_t s1() {
  return sum(filter([](size_t x) { return (x % 3) == 0 || (x % 5) == 0; }, fp::increasingN(999, 1)));
}

///////////////////////////////////////////////////////////////////////////

size_t s2() {
  size_t x0 = 0, x1 = 1;
  let fibs = [=]() mutable -> size_t {
    size_t result = x0;
    std::swap(x0+=x1, x1);
    return result;
  };
  return sum(filter([](size_t x) { return x % 2 == 0; }, 
                    takeWhileT(curry(greater<size_t>, 4000000), fibs)));
}

///////////////////////////////////////////////////////////////////////////


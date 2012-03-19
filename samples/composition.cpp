/////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2012, Jared Duke.
// This code is released under the MIT License.
// www.opensource.org/licenses/mit-license.php
/////////////////////////////////////////////////////////////////////////////

#include <algorithm>
#include <iostream>
#include <string>
#include <sstream>

#include <fpcpp.h>

template <typename T> void print(const char* prefix, const T& result) {
  std::cout << prefix << result << std::endl;
}

void test() {

  using namespace fp;
  using namespace fp_operators;

  ///////////////////////////////////////////////////////////////////////////

  {
    auto g = [](float x, float y, float z) -> float { return x*x + y*y + z*z; };
    auto f = [](float l2) -> float                  { return std::sqrt(l2); };
    auto fg  = compose(f, g);
    auto fg2 = f<<g;
    auto fg3  = [](float l2) -> float                  { return std::sqrt(l2); }
              + [](float x, float y, float z) -> float { return x*x + y*y + z*z; };
    auto fg4  = compose([](float l2) -> float                  { return std::sqrt(l2); },
                        [](float x, float y, float z) -> float { return x*x + y*y + z*z; });

    float v[] = {1.f, 2.f, -1.f};

    ///////////////////////////////////////////////////////////////////////////

    print( "Length (compose(f,g))         = ", fg(   v[0], v[1], v[2]) );
    print( "Length (f+g)                  = ", fg2(  v[0], v[1], v[2]) );
    print( "Length (f_lam + g_lam)        = ", fg3(  v[0], v[1], v[2]) );
    print( "Length (compose(f_lam,g_lam)) = ", fg4(  v[0], v[1], v[2]) );
    print( "Length                        = ", fg(   v[0], v[1], v[2]) );
  }
}

int main(int argc, char** argv) {

  test();

  return 0;
}

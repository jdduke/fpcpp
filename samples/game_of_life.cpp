/////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2012, Jared Duke.
// This code is released under the MIT License.
// www.opensource.org/licenses/mit-license.php
/////////////////////////////////////////////////////////////////////////////

#include <fpcpp.h>

#include <iostream>
#include <time.h>

#if WIN32
#include <Windows.h>
inline void idle(DWORD milliseconds) { Sleep(milliseconds); }
#else
#include <unistd.h>
inline void idle(size_t microseconds) { usleep(microseconds*1000); }
#endif

template <typename C>
C life(const C& grid, size_t x, size_t y) {
  using namespace fp;
  const let size = x * y;
  return map([=,&grid](size_t i) -> value_type_of(C) {
    const let neighbors = grid[(i-x-1)%size] + grid[(i-x)%size] + grid[(i-x+1)%size] +
                          grid[(i  -1)%size]                    + grid[(i  +1)%size] +
                          grid[(i+x-1)%size] + grid[(i+x)%size] + grid[(i+x+1)%size];
    return ((grid[i]==1 && (neighbors==2 || neighbors==3)) || (grid[i]==0 && neighbors == 3)) ? 1 : 0;
  }, (0 <to> x*y-1));
}

int main(int argc, char **argv) {

  srand((unsigned)time((time_t*)NULL));

  enum {
    X = 80,
    Y = 20,
  };

  let grid = fp::map([](float f) { return f < 0.f ? 0 : 1; },
                     fp::uniformN(X*Y, -1.f, 1.f));

  while (true) {
    grid = life(grid, X, Y);
    size_t i = 0;
    std::stringstream ss;
    let showGrid = [=,&ss](int x) mutable {
      if(i++ % X == 0)
        ss << std::endl;
      const char c = (x ? 'X' : ' ');
      ss << c;
    };
    fp::mapV(showGrid, grid);
    std::cout << ss.str();
  }

  return 0;
}

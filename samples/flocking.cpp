/////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2012, Jared Duke.
// This code is released under the MIT License.
// www.opensource.org/licenses/mit-license.php
/////////////////////////////////////////////////////////////////////////////

#include <fc.h>
#include <fpcpp.h>

#include <iostream>
#include <time.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <array>

#if WIN32
#include <Windows.h>
inline void idle(DWORD milliseconds) { Sleep(milliseconds); }
#else
#include <unistd.h>
inline void idle(size_t microseconds) { usleep(microseconds*1000); }
#endif

template <typename C>
C evolve(const C& boids, size_t x, size_t y) {
  return boids;
}

int main(int argc, char **argv) {

  srand((unsigned)time((time_t*)NULL));

  enum {
    BOIDS = 100,
    X     = 80,
    Y     = 20,
  };

  typedef std::pair<int,int>     P;
  typedef std::pair<float,float> V;
  typedef std::pair< P, V >      Boid;

  std::array<char, 8> cardToChar = { '-', '/', '|', '\\', '-', '/', '|', '\\' };
  let vecToCard = [](const V& v) -> int {
    let theta = atan2f(fp::snd(v), fp::fst(v)); 
    theta = theta < 0.f ? theta + 2.f*(float)M_PI : theta;
    return (int)(theta * (4.f/M_PI)) & 0x07;
  };

  let boids = fp::zip(fp::zip(fp::takeF(X*Y, fp::rand_range_<int>(0, X)),
                              fp::takeF(X*Y, fp::rand_range_<int>(0, Y))),
                      fp::zip(fp::takeF(X*Y, fp::rand_range_<float>(-1.f,1.f)),
                              fp::takeF(X*Y, fp::rand_range_<float>(-1.f,1.f))));

  typedef std::array< std::array< V, X >, Y > Grid;
  
  while (true) {
    boids = evolve(boids, X, Y);

    Grid grid;
    for (size_t i = 0; i < fp::length(boids); ++i) {
      let x = boids[i].first.second; 
      let y = boids[i].first.second;
      grid[x][y].first  += boids[i].second.first;
      grid[x][y].second += boids[i].second.second;
    }
    let show = [=](const V& v) -> char { return cardToChar[ vecToCard(v) ]; };
    std::stringstream ss;
    for (int i = 0; i < fp::length(grid); ++i) {
      for (int j = 0; j < fp::length(grid[0]); ++j) {
        ss << show(grid[i][j]);
      }
      ss << std::endl;
    }
    std::cout << ss.str();
  }

  return 0;
}

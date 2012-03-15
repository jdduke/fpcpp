/////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2012, Jared Duke.
// This code is released under the MIT License.
// www.opensource.org/licenses/mit-license.php
/////////////////////////////////////////////////////////////////////////////

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

///////////////////////////////////////////////////////////////////////////

enum {
  BOIDS        = 25,
  X            = 80,
  Y            = 30,
  NEIGHBORHOOD = 15,
  AVOIDANCE    = 5,
};

///////////////////////////////////////////////////////////////////////////

using fp::fst;
using fp::snd;
using fp::index;
using fp::types;
typedef types<float,float>::pair P;
typedef types<float,float>::pair D;
typedef types<P, D>::pair        Boid;
typedef types<Boid>::list        Boids;

P pos(const Boid& b) { return fst(b); }
D dir(const Boid& b) { return snd(b); }

namespace std {
template< typename T >
std::pair<T,T> operator+(const std::pair<T,T>& a, const std::pair<T,T>& b) {
  return std::make_pair( fst(a) + fst(b), snd(b) + snd(b) );
}
template< typename T >
std::pair<T,T> operator-(const std::pair<T,T>& a, const std::pair<T,T>& b) {
  return std::make_pair( fst(a) - fst(b), snd(b) - snd(b) );
}
template< typename T >
std::pair<T,T> operator*(const std::pair<T,T>& a, T b) {
  return std::make_pair( fst(a) * b, snd(a) * b );
}
template< typename T >
std::pair<T,T> operator/(const std::pair<T,T>& a, T b) {
  return std::make_pair( fst(a) / b, snd(a) / b );
}
}
using namespace std;

template <typename T>
inline float length( const T& t ) {
  const let x = fst(t);
  const let y = snd(t);
  return sqrtf( (float)x*x + (float)y*y );
}

template <typename T>
inline T normalize( const T& t ) {
  const let tLength = length( t );
  return tLength > 0.f ? t / tLength : t;
}

template <typename T>
inline float dist( const T& a, const T& b) {
  return length( a - b );
}

template <typename T, typename U, typename V> 
inline T clamp(const T& value, const U& low, const V& high) {
  return value < low ? low : (value > high ? high : value); 
}

template <typename T>
inline T fmod_(T t, T modulus) {
  return t >= (T)0 ? fmod(t, modulus) : modulus - fmod(-t, modulus);
}

template <typename T, typename U>
inline T pmod( const T& t, U fstMod, U sndMod ) {
  return T( fmod_( fst(t), fstMod ), fmod_( snd(t), sndMod ) );
}

///////////////////////////////////////////////////////////////////////////

D avoidance( const Boid& boid, const Boids& neighbors ) {
  return fp::sum( fp::map( [&](const Boid& otherBoid) -> D {
    const let avoidanceWeight = 1.f - (dist( pos(boid), pos(otherBoid) ) / NEIGHBORHOOD);
    return normalize( pos(boid) - pos(otherBoid) ) *        avoidanceWeight +
           dir( boid )                             * (1.f - avoidanceWeight);
  }, neighbors) ) / (float)neighbors.size();
}

D alignment( const Boid& boid, const Boids& neighbors ) {
  const let avgDir = fp::sum( fp::map( &dir, neighbors )) / (float)neighbors.size();
  return (avgDir + dir( boid )) *.5f;
}

D cohesion( const Boid& boid, const Boids& neighbors ) {
  const let avgPos = fp::sum( fp::map( &pos, neighbors) ) / (float)neighbors.size();
  return (normalize( avgPos - pos( boid ) ) + dir( boid )) *.5f;
}

Boid evolve( const Boid& boid, const Boids& neighbors ) {

  let newDir = dir( boid );
  
  if ( fp::length(neighbors) != 0) {

    std::array<float, 3> weights = { .5f, .2f, .3f };

    newDir = normalize( avoidance( boid, neighbors ) * weights[0] +
                        alignment( boid, neighbors ) * weights[1] +
                        cohesion(  boid, neighbors ) * weights[2] );

  }

  let newPos = pmod( pos( boid ) + newDir, (float)X, (float)Y);

  return Boid( newPos, newDir );
}

///////////////////////////////////////////////////////////////////////////

Boids evolve( const Boids& boids, size_t x, size_t y ) {

  return fp::map( [=,&boids]( const Boid& boid ) -> Boid {

    let neighbors = fp::filter( [=,&boid]( const Boid& otherBoid ) {
      return ( boid != otherBoid ) && 
             ( dist( pos(boid), pos(otherBoid) ) < NEIGHBORHOOD );
    }, boids );

    return evolve( boid, neighbors );

  }, boids);
}

int main(int argc, char **argv) {

  srand((unsigned)time((time_t*)NULL));

  std::array<char, 8> cardinalToChar = { '-', '/', '|', '\\', '-', '/', '|', '\\' };
  let showCell = [&]( const D& v ) -> char {
    if (length(v) < .25)
      return ' ';
    let theta = atan2f( snd(v), fst(v) ); 
    theta = theta < 0.f ? theta + 2.f*(float)M_PI : theta;
    let const index = (int)(theta * (4.f/M_PI)) % cardinalToChar.size();
    return cardinalToChar[index];
  };

  let boids = fp::zip(fp::zip(fp::uniformN(BOIDS, 0.f, (float)X),
                              fp::uniformN(BOIDS, 0.f, (float)Y)),
                      fp::zip(fp::uniformN(BOIDS, -1.f,1.f),
                              fp::uniformN(BOIDS, -1.f,1.f)));

  typedef std::array< D,   X > Row;
  typedef std::array< Row, Y > Grid;
  
  while (true) {
    boids = evolve(boids, X, Y);

    Grid grid;
    for (size_t i = 0; i < fp::length(boids); ++i) {
      let x = (int)fst( pos(index(i,boids)) ) % X;
      let y = (int)snd( pos(index(i,boids)) ) % Y;
      grid[y][x] = grid[y][x] + dir( index(i,boids) );
    }
    let showRow  = [=](const Row& r) -> std::string { return fp::show(fp::map( showCell, r) ); };
    std::cout << std::endl << fp::foldl1( [](const std::string& a, const std::string& b) {
      return a + "\n" + b;
    }, fp::map( showRow, grid )) << std::endl << std::endl;
  }

  return 0;
}

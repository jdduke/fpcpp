/////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2012, Jared Duke.
// This code is released under the MIT License.
// www.opensource.org/licenses/mit-license.php
/////////////////////////////////////////////////////////////////////////////

#include "fpcpp.h"

#include <iostream>
#include <complex>
#define _USE_MATH_DEFINES
#include <math.h>
#include <deque>
#include <set>

#include "benchmark_common.h"

using namespace fp;

///////////////////////////////////////////////////////////////////////////

inline types<int>::list unique1( const types<int>::list& rands ) {
  let sorted = fp::sort( rands );
  sorted.erase( std::unique( extent(sorted) ), fp::end(sorted) );
  return sorted;
}
inline types<int>::list unique2( types<int>::list rands ) {
  std::sort( extent(rands) );
  rands.erase( std::unique( extent(rands) ), fp::end(rands) );
  return rands;
}
inline void unique3( types<int>::list& rands ) {
  std::sort( extent(rands) );
  rands.resize( std::unique( extent(rands) ) - fp::begin(rands) );
}
inline types<int>::list unique4( const types<int>::list& rands ) {
  std::set<int> s;
  const size_t size = fp::length(rands);
  for( unsigned i = 0; i < size; ++i )
    s.insert( rands[i] );
  return types<int>::list( extent(s) );
}
inline void unique5( types<int>::list& rands ) {
  std::set<int> s( extent(rands) );
  rands.assign( extent(s) );
}
inline types<int>::list unique6( const types<int>::list& rands ) {
  std::set<int> s( extent(rands) );
  return types<int>::list( extent(s) );
}

void unique( size_t count ) {
  const let rands = fp::uniformN( count, 0, 100 );
  let rands3 = rands;
  let rands5 = rands;

  run( let t = unique1( rands );  (void)fp::length(t),  ITER_MULT );
  run( let t = unique2( rands );  (void)fp::length(t),  ITER_MULT );
  run( let t = rands; unique3(t); (void)fp::length(t),  ITER_MULT );
  run( let t = unique4( rands );  (void)fp::length(t),  ITER_MULT );
  run( let t = rands; unique5(t); (void)fp::length(t),  ITER_MULT );
  run( let t = unique6( rands5 ); (void)fp::length(t),  ITER_MULT );
}

///////////////////////////////////////////////////////////////////////////

int main(int argc, char **argv) {

  print( "" );

  unique( 100000 * ITER_MULT );

  print( "" );

  return 0;
}

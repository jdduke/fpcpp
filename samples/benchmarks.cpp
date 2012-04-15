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
#include <unordered_set>

#include "benchmark_common.h"

using namespace fp;

///////////////////////////////////////////////////////////////////////////

inline types<int>::list unique1( const types<int>::list& rands ) {
  let sorted = fp::sort( rands );
  sorted.erase( std::unique( extent(sorted) ), fp::end(sorted) );
  return move(sorted);
}
inline types<int>::list unique2( types<int>::list rands ) {
  rands = move( fp::sort( move(rands) ) );
  rands.resize( std::unique( extent(rands) ) - fp::begin(rands) );
  return move(rands);
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
  std::unordered_set<int> s( extent(rands) );
  return types<int>::list( extent(s) );
}
inline types<int>::list unique7( types<int>::list rands ) {
  rands = move( fp::sort( move(rands) ) );
  return types<int>::list( fp::begin(rands), std::unique( extent(rands) ) );
}

void unique( size_t count, size_t iters = ITER_MULT ) {
  const let rands = fp::uniformN( count, 0, 100 );
  print( "Length = " + show(count) + " - Iters = " + show(iters));

  run( let t = unique1( rands );  (void)fp::length(t),  iters );
  run( let t = unique2( rands );  (void)fp::length(t),  iters );
  run( let t = rands; unique3(t); (void)fp::length(t),  iters );
  run( let t = unique4( rands );  (void)fp::length(t),  iters );
  run( let t = rands; unique5(t); (void)fp::length(t),  iters );
  run( let t = unique6( rands );  (void)fp::length(t),  iters );
  run( let t = unique7( rands );  (void)fp::length(t),  iters );
}

///////////////////////////////////////////////////////////////////////////

int main(int argc, char **argv) {

  print( "" );

  unique( 100,    1000 * ITER_MULT );
  unique( 1000,    100 * ITER_MULT );
  unique( 10000,    10 * ITER_MULT );
  unique( 100000,    1 * ITER_MULT );

  print( "" );

  return 0;
}

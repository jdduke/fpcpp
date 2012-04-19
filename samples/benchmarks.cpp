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

template <typename C>
inline C unique1( const C& rands ) {
  let sorted = fp::sort( rands );
  sorted.erase( std::unique( extent(sorted) ), fp::end(sorted) );
  return move(sorted);
}
template <typename C>
inline C unique2( C rands ) {
  rands = move( fp::sort( move(rands) ) );
  rands.resize( std::unique( extent(rands) ) - fp::begin(rands) );
  return move(rands);
}
template <typename C>
inline void unique3( C& rands ) {
  std::sort( extent(rands) );
  rands.resize( std::unique( extent(rands) ) - fp::begin(rands) );
}
template <typename C>
inline C unique4( const C& rands ) {
  typedef nonconstref_type_of(decltype(head(rands))) T;
  std::set<T> s;
  const size_t size = fp::length(rands);
  for( unsigned i = 0; i < size; ++i )
    s.insert( rands[i] );
  return C( extent(s) );
}
template <typename C>
inline void unique5( C& rands ) {
  typedef nonconstref_type_of(decltype(head(rands))) T;
  std::set<T> s( extent(rands) );
  rands.assign( extent(s) );
}
template <typename C>
inline C unique6( const C& rands ) {
  typedef nonconstref_type_of(decltype(head(rands))) T;
  std::unordered_set<T> s( extent(rands) );
  return C( extent(s) );
}
template <typename C>
inline C unique7( C rands ) {
  rands = move( fp::sort( move(rands) ) );
  return C( fp::begin(rands), std::unique( extent(rands) ) );
}

template <typename C>
void unique_impl(const C& rands, size_t iters) {
  print( "Length = " + show(length(rands)) + " - Iters = " + show(iters));
  run( let t = unique1( rands );  (void)fp::length(t),  iters );
  run( let t = unique2( rands );  (void)fp::length(t),  iters );
  run( let t = rands; unique3(t); (void)fp::length(t),  iters );
  run( let t = unique4( rands );  (void)fp::length(t),  iters );
  run( let t = rands; unique5(t); (void)fp::length(t),  iters );
  run( let t = unique6( rands );  (void)fp::length(t),  iters );
  run( let t = unique7( rands );  (void)fp::length(t),  iters );
}

void unique( size_t count, size_t iters = ITER_MULT ) {
  {
    const let rands = fp::uniformN( count, 0, 100 );
    unique_impl(rands, iters);
  }
  {
    types<string>::list rands;
    for (size_t i = 0; i < count; ++i)
      rands.emplace_back( show( uniformN( 10, 'a', 'z' ) ) );
    unique_impl(rands, iters);
  }
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

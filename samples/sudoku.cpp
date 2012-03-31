/////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2012, Jared Duke.
// This code is released under the MIT License.
// www.opensource.org/licenses/mit-license.php
/////////////////////////////////////////////////////////////////////////////

#include "fpcpp.h"

#include <iostream>

#define ENABLE_BENCHMARK 1
#include "benchmark_common.h"

using namespace fp;

///////////////////////////////////////////////////////////////////////////

typedef types<string>::list strings;

strings cross( const string& s0, const string& s1 ) {
  /*return map([&](char c0) {
    return map( [&](char c1) -> std::string {
      std::string r; r+=c0; r+= c1; return r;
    }, list(s1) );
  }, list(s0));*/
  //std::for_each(extent(s0)
  types<string>::list result;
   for( size_t i = 0; i < s0.size(); ++i ) {
     for ( size_t j = 0; j < s1.size(); ++j ) {
       result.emplace_back( cons(show(s0[i]), show(s1[j])) );
     }
   }
  return result;
}

auto mapCross( const strings& s0, const strings& s1 ) -> strings {
  types<string>::list result;
   for( size_t i = 0; i < s0.size(); ++i ) {
     for ( size_t j = 0; j < s1.size(); ++j ) {
       auto r = cross( s0[i], s0[j] );
       result.insert( end(result), extent(r) );
     }
   }
  return result;
};

auto mapCrossInner(const string& s0, const string& s1) -> strings {
  strings result;
  for(size_t i = 0; i < s0.size(); ++i) {
    auto r = cross(s1, show(s0[i]));
    result.insert( end(result), extent(r) );
  }
  return result;
};

///////////////////////////////////////////////////////////////////////////

static const let digits   = show("123456789");
static const let rows     = show("ABCDEFGHI");
static const let cols     = digits;
static const let squares  = cross(rows, cols);
static const let unitlist = concat( concat( mapCrossInner(cols, rows),
                                            mapCrossInner(rows, cols) ),
                                    mapCross( cons(show("ABC"), cons(show("DEF"), cons(show("GHI"), strings()))),
                                              cons(show("123"), cons(show("456"), cons(show("789"), strings()))) ) );

///////////////////////////////////////////////////////////////////////////

int main(int argc, char **argv) {

  using namespace fp;

  print( "" );

  ///////////////////////////////////////////////////////////////////////////


#if defined(_MSC_VER)
  static const std::string filePrefix( "./../../../samples" );
#else
  static const std::string filePrefix( "./../../samples" );
#endif

  //run( );
  print( "" );

  return 0;
}

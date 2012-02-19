/////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2012, Jared Duke.
// This code is released under the MIT License.
// www.opensource.org/licenses/mit-license.php
/////////////////////////////////////////////////////////////////////////////

#include <fpcpp.h>

#include <iostream>

using std::string;

void pascalsTriangle() {

  fp::print( "Pascal's Triangle" );

  typedef std::vector<int> Row;

  let nextRow = [](const Row& r) {
    return fp::zipWith( std::plus<int>(), fp::cons(0, r), fp::append(r, 0) );
  };

  let pascals = fp::iterate( nextRow, Row(1,1) );

  fp::mapM( fp::print<Row>,
            fp::takeF(6, pascals) );
}

template<typename F, typename T>
bool groupon(F f, T x, T y) {
  return f(x) == f(y);
}

void anagrams() {

  fp::print( "\nAnagrams" );

  typedef std::pair<string,string> stringpair;
  typedef std::vector<stringpair> stringpairlist;
  
  let f     = fp::readFile( "./../../../samples/unixdict.txt" );
  let words = fp::lines( f );
  let wix   = fp::groupBy( fp::compose2(std::equal_to<string>(), fp::fst<string,string>, fp::fst<string,string>),
                           fp::sort( fp::zip( fp::map( fp::sort<string>, words ), words) ) );
  let mxl   = fp::maximum( fp::map( fp::length< stringpairlist >, wix ) );

  fp::mapM( [](const stringpairlist& sl) {
    fp::print( fp::map( fp::snd<string,string>, sl ) );
  },  fp::filter( [=](const stringpairlist& sl) { 
        return fp::length(sl) == mxl; 
      },  wix) );

  /* Compare with Haskell:
  
    import Data.List
    groupon f x y = f x == f y

    main = do
      f <- readFile "./../Puzzels/Rosetta/unixdict.txt"
      let  words = lines f
      wix = groupBy (groupon fst) . sort $ zip (map sort words) words
      mxl = maximum $ map length wix
      mapM_ (print . map snd) . filter ((==mxl).length) $ wix
  */
}

int main(int argc, char **argv) {

  pascalsTriangle();

  anagrams();

  return 0;
}

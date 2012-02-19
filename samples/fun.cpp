/////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2012, Jared Duke.
// This code is released under the MIT License.
// www.opensource.org/licenses/mit-license.php
/////////////////////////////////////////////////////////////////////////////

#include <fpcpp.h>

#include <iostream>

using std::string;
using std::vector;

// lambdas don't inherit "using" qualifiers, for better or worse...

void pascalsTriangle( size_t count ) {

  using namespace fp;

  print( "Pascal's Triangle" );

  typedef vector<int> Row;

  let nextRow = []( const Row& r ) {
    return fp::zipWith( std::plus<int>(), fp::cons( 0, r ), fp::append( r, 0 ) );
  };

  let pascals = iterate( nextRow, Row(1,1) );

  mapM( print<Row>,
        takeF( count, pascals ) );
}

void anagrams( const fp::FilePath& filePath ) {

  using namespace fp;

  print( "\nAnagrams" );

  typedef std::pair<string,string> stringpair;
  typedef std::vector<stringpair> stringpairlist;

  let fstr    = fst<string,string>;
  let f       = readFile( filePath );
  let words   = lines( f );
  let groupon = compose2( std::equal_to<string>(), fstr, fstr );
  let wix     = groupBy( groupon, sort( zip( map( sort<string>, words ), words) ) );
  let mxl     = maximum( map( length< stringpairlist >, wix ) );

  mapM( []( const stringpairlist& sl ) {
    fp::print( fp::map( fp::snd<string,string>, sl ) );
  },  fp::filter( [=]( const stringpairlist& sl ) { 
        return fp::length( sl ) == mxl; 
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

template <typename T>
void nthRoot( int n, T x ) {

  using namespace fp;


  print( show("\nNthRoot: ") +  show(n) + " " + show(x) );

  typedef std::pair<T,T> Guess;

  let root = fst( until( uncurry( std::equal_to<T>() ), [=]( Guess g ) -> Guess { 
    T x0 = fp::snd( g ); return Guess( x0, (x0*(n-1)+(x/pow(x0,n-1)))*(1./n) ); 
  }, Guess( x, x/n ) ) );

  print( root );


  /* Compare with Haskell:

    n `nthRoot` x = fst $ until (uncurry(==)) (\(_,x0) -> (x0,((n-1)*x0+x/x0**(n-1))/n)) (x,x/n)

  */

}


int main(int argc, char **argv) {

  pascalsTriangle( 6 );

  nthRoot( 5, 34. );

  anagrams( "./../../../samples/unixdict.txt" );

  return 0;
}

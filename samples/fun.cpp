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

#define ENABLE_BENCHMARK 1
#include "benchmark_common.h"

using fp::string;
using fp::types;
using fp::pair;

typedef types<int>::list Row;
typedef types<Row>::list Rows;

typedef pair<string,string>      StringPair;
typedef types<string>::list      StringList;
typedef types<StringList>::list  StringLists;
typedef types<StringPair>::list  StringPairList;


Rows pascalsTriangle( size_t count ) {

  using namespace fp;

  let nextRow = []( const Row& r ) -> Row {
    return fp::zipWith( fp::math::addF(), fp::cons( (int)0, r ), fp::append( r, 0 ) );
  };

  let pascals = iterate( nextRow, Row(1,1) );

  return takeF( count, pascals );

}

///////////////////////////////////////////////////////////////////////////

StringLists anagrams( const fp::FilePath& filePath ) {

  using namespace fp;

  typedef StringPair sp;
  typedef StringPairList spl;

  std::ifstream ifs;
  let& f      = readFile( filePath, ifs );
  let words   = lines( f );
  let groupon = compose2( math::equalsF(), fstF(), fstF() );
  let wix     = groupBy( groupon, sort( zip( map( sortF(), words ), words) ) );
  let mxl     = maximum( map( lengthF(), wix ) );

  return map( []( const spl& sl ) {
    return fp::map( fp::sndF(), sl );
  }, filter( [=]( const spl& sl ) { return fp::length( sl ) == mxl; }, wix) );

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

///////////////////////////////////////////////////////////////////////////

template<typename T>
T nthRoot( int n, T x ) {

  using namespace fp;

  typedef std::pair<T,T> Guess;

  return fst( until( uncurry( math::equalsF() ), [=]( Guess g ) -> Guess {
    T x0 = fp::snd( g ); return Guess( x0, (x0*(n-1)+(x/pow(x0,n-1)))*(1./n) );
  }, Guess( x, x/n ) ) );

  /* Compare with Haskell:
    n `nthRoot` x = fst $ until (uncurry(==)) (\(_,x0) -> (x0,((n-1)*x0+x/x0**(n-1))/n)) (x,x/n)
  */

}

///////////////////////////////////////////////////////////////////////////

template< typename T >
typename types< typename types<T>::list, typename types<T>::list >::pair fftSplit( const typename types<T>::list& s ) {
  using namespace fp;
  typedef typename types<T>::list FFTVec;

  if      ( length(s) == 0 ) return make_pair( FFTVec(), FFTVec() );
  else if ( length(s) == 1 ) return make_pair( FFTVec(1, head(s)), FFTVec() );
  else {
    let xt_yt = fftSplit<T>( drop(2, s) );
    return make_pair( cons( index(0,s), fst(move(xt_yt))), cons( index(1,s), snd(move(xt_yt)) ) );
  }
}

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

template <typename V>
auto fft( V v ) -> typename types< std::complex< value_type_of(V) > >::list {
  typedef value_type_of(V) T;
  using namespace fp;
  
  typedef std::complex<T> CT;
  typedef typename types< CT >::list FFTVec;

  let const n        = length( v );

  if ( n == 0 ) return FFTVec();
  if ( n == 1 ) return FFTVec( 1, head(v) );

  let const evenOdds = fftSplit<T>( v );
  let const ys       = fft( fst( evenOdds) );
  let const cx = [=](CT z, T k) { return z*std::polar((T)1, (T)(-2. * M_PI * (double)k/n)); };
  let const ts = zipWith( cx, fft( snd( evenOdds) ), increasingN( n/2, (T)0 ) );

  return concat( zipWith( std::plus<  CT >(), ys, ts ),
                 zipWith( std::minus< CT >(), ys, ts ) );

  /* Compare with Haskell:

  fft [] = []
  fft [x] = [x]
  fft xs = zipWith (+) ys ts ++ zipWith (-) ys ts
  where n = length xs
  ys = fft evens
    zs = fft odds
    (evens, odds) = split xs
    split [] = ([], [])
    split [x] = ([x], [])
    split (x:y:xs) = (x:xt, y:yt) where (xt, yt) = split xs
    ts = zipWith (\z k -> exp' k n * z) zs [0..]
    exp' k n = cis $ -2 * pi * (fromIntegral k) / (fromIntegral n)

  */
}

///////////////////////////////////////////////////////////////////////////

typedef pair<char,string> Code;
typedef pair<size_t,char> Freq;
typedef types< Code >::list     Codes;
typedef types< Freq >::list     Frequencies;
typedef fp::pair<size_t, Codes> FreqCodes;

Codes reduce( fp::types< FreqCodes >::list buf ) {

  using namespace fp;

  if (length(buf) == 1) return snd(head(buf));

  let consC = (string(*)(char,string))cons;
  let cons0 = curry(consC, '0');
  let cons1 = curry(consC, '1');

  let add = [&]( const FreqCodes& a, const FreqCodes& b ) {
    return FreqCodes(a.first+b.first,
                     fp::concat(fp::map(fp::mapSndF_(cons0), fp::snd(a)),
                                fp::map(fp::mapSndF_(cons1), fp::snd(b))));
  };

  return reduce( insertBy( comparing(fstF()),
                           add( index(0, buf),
                                index(1, buf) ),
                           drop(2, buf) ) );
}

StringList huffman( string s ) {

  using namespace fp;

  let freq = []( types<char>::list s ) {
    return fp::map( fp::mapArrowF_( fp::lengthF(), fp::headF() ), fp::group( fp::sort( std::move(s) ) ) );
  };

  let result = reduce( map( []( const Freq& f ) {
    return fp::make_pair( fp::fst(f), Codes(1, fp::make_pair( fp::snd(f), "" )) );
  }, sortBy( comparing(fstF()), freq(list(s)) ) ) );

  return map( [](const Code& c) {
    return fp::show("\'") + fp::fst(c) + "\' : " + fp::snd(c) + "\n";
  }, result );

  /*
huffman :: [(Int, Char)] -> [(Char, String)]
huffman = reduce . map (\(p, c) -> (p, [(c ,"")])) . sortBy (comparing fst)
  where
    reduce [(_, ys)]  = ys
    reduce (x1:x2:xs) = reduce $ insertBy (comparing fst) (add x1 x2) xs
    add (p1, xs1) (p2, xs2) = (p1 + p2, map (second ('0':)) xs1 ++ map (second ('1':)) xs2)

test s = mapM_ (\(a,b)->putStrLn ('\'' : a : "\' : " ++ b)) . huffman . freq $ s
 */

}

///////////////////////////////////////////////////////////////////////////

int main(int argc, char **argv) {

  using namespace fp;

  print( "" );


  ///////////////////////////////////////////////////////////////////////////

  run( pascalsTriangle( 6 ), 10000 * ITER_MULT );

  ///////////////////////////////////////////////////////////////////////////

#if defined(_MSC_VER)
  static const char* unixdict = "./../../../samples/unixdict.txt";
#else
  static const char* unixdict = "./../../samples/unixdict.txt";
#endif
  run( anagrams( unixdict ), 5 * ITER_MULT );

  ///////////////////////////////////////////////////////////////////////////

  std::array<float, 8> fftValues = {1, 1, 1, 1, 0, 0, 0, 0};
  let fftIn = fp::list( fftValues );
  run( fft( fftIn /*=1,1,1,1,0,0,0,0*/ ), 5000 * ITER_MULT );

  ///////////////////////////////////////////////////////////////////////////

  run( nthRoot( 5, 34. ),    100000 * ITER_MULT );
  run( nthRoot( 6, 25. ),    100000 * ITER_MULT );
  run( nthRoot( 7, 100. ),   100000 * ITER_MULT );

  ///////////////////////////////////////////////////////////////////////////

  run( huffman( "this is an example for huffman encoding" ),  50 * ITER_MULT );

  print( "" );

  return 0;
}

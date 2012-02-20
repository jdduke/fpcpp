/////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2012, Jared Duke.
// This code is released under the MIT License.
// www.opensource.org/licenses/mit-license.php
/////////////////////////////////////////////////////////////////////////////

#include <fpcpp.h>

#include <iostream>
#include <complex>
#define _USE_MATH_DEFINES
#include <math.h>

using std::string;
using std::vector;
using std::pair;

typedef vector<int> Row;

typedef pair<string,string> StringPair;
typedef vector<string>      StringList;
typedef vector<StringPair>  StringPairList;

///////////////////////////////////////////////////////////////////////////

std::vector< Row > pascalsTriangle( size_t count ) {

  using namespace fp;

  let nextRow = []( const Row& r ) {
    return fp::zipWith( std::plus<int>(), fp::cons( 0, r ), fp::append( r, 0 ) );
  };

  let pascals = iterate( nextRow, Row(1,1) );

  return takeF( count, pascals );

}

///////////////////////////////////////////////////////////////////////////

std::vector< StringList > anagrams( const fp::FilePath& filePath ) {

  using namespace fp;

  let fstr    = fst<string,string>;
  let f       = readFile( filePath );
  let words   = lines( f );
  let groupon = compose2( std::equal_to<string>(), fstr, fstr );
  let wix     = groupBy( groupon, sort( zip( map( sort<string>, words ), words) ) );
  let mxl     = maximum( map( length< StringPairList >, wix ) );

  return fp::map( []( const StringPairList& sl ) {
    return fp::map( fp::snd<string,string>, sl );
  }, fp::filter( [=]( const StringPairList& sl ) { 
    return fp::length( sl ) == mxl; 
  }, wix) );

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

  return fst( until( uncurry( std::equal_to<T>() ), [=]( Guess g ) -> Guess { 
    T x0 = fp::snd( g ); return Guess( x0, (x0*(n-1)+(x/pow(x0,n-1)))*(1./n) ); 
  }, Guess( x, x/n ) ) );

  /* Compare with Haskell:

    n `nthRoot` x = fst $ until (uncurry(==)) (\(_,x0) -> (x0,((n-1)*x0+x/x0**(n-1))/n)) (x,x/n)

  */

}

///////////////////////////////////////////////////////////////////////////

template< typename T >
std::pair< std::vector<T>, std::vector<T> > split( const std::vector<T>& s ) {
  using namespace fp;
  typedef std::vector<T> FFTVec;

  if      ( length(s) == 0 ) return make_pair( FFTVec(), FFTVec() );
  else if ( length(s) == 1 ) return make_pair( FFTVec(1, s[0]), FFTVec() );
  else {
    let xt_yt = split( drop(2, s) );
    return make_pair( cons( s[0], fst(xt_yt)), cons( s[1], snd(xt_yt) ) );
  }
}

template<typename T> 
std::vector< std::complex<T> > fft( std::vector<T> v ) {
  using namespace fp;
  typedef std::complex<T> CT;
  typedef std::vector< CT > FFTVec;

  if      ( length(v) == 0 ) return FFTVec();
  else if ( length(v) == 1 ) return FFTVec( 1, v[0] );
  else {
    let n        = length( v );
    let evenOdds = split( v );
    let ys       = fft( fst( evenOdds) );
    let zs       = fft( snd( evenOdds) );
    let cx = [=](CT z, T k) { return z*std::polar((T)1, (T)(-2. * M_PI * (double)k/n)); };
    let ts = zipWith( cx, zs, increasing_n( length(zs), (T)0 ) );

    return concat( zipWith( std::plus<  CT >(), ys, ts ),
                   zipWith( std::minus< CT >(), ys, ts ) );
  }

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

struct HTree;
typedef pair<char,string> Code;
typedef pair<size_t,char> Freq;
typedef pair<size_t,HTree> FreqTree;
typedef vector< Code > Encoding;
typedef vector< Freq > Frequencies;
typedef vector< FreqTree > FreqTrees;

struct HTree { 
  HTree() : left(nullptr), right(nullptr), value(0) { }
  HTree(const HTree& l, const HTree& r) : left(&l), right(&r), value(0) { }
  HTree(char c) : left(nullptr), right(nullptr), value(c) { }
  const HTree* left;
  const HTree* right;
  char value;
};

inline Encoding serialize( const HTree* t ) {
  using namespace fp;
  if (nullptr == t) {
    return Encoding();
  } else if (t->value) {
    return Encoding(1, Code(t->value, "") );
  } else {
    let leftS  = serialize(t->left);
    let rightS = serialize(t->right);

    let cons0 = [](std::string& s) { return fp::cons('0', s); };
    let cons1 = [](std::string& s) { return fp::cons('1', s); };
    let apply0 = [&](const Code& c) { return Code(fp::fst(c), cons0(fp::snd(c))); };
    let apply1 = [&](const Code& c) { return Code(fp::fst(c), cons1(fp::snd(c))); };

    return concat( map( apply0, leftS), 
                   map( apply1, leftS) );
  }
}

vector<string> huffman( std::string s ) {

  using namespace fp;

  std::vector<HTree> htree;

  let freq = []( const vector<char>& s ) -> Frequencies {
    return fp::map( fp::mapArrowF_( fp::lengthF(), fp::headF() ), fp::group( fp::sort(s) ) );
  };
  let hstep = []( const FreqTrees& fts ) -> FreqTrees {
    using namespace fp;
    if (length(fts) < 2)
      return FreqTrees();

    let wt1 = index(0, fts);
    let wt2 = index(1, fts);

    return FreqTrees();

    // TODO: Implement
    /*return insertBy( comparing(fst<size_t,HTree>), 
                     make_pair(wt1.first+wt2.second, HTree(wt1.second,wt2.second)) );*/

  };
  let huffmanTree = [&]( const Frequencies& fs ) -> HTree* {
    using namespace fp;
    htree.resize( length(fs) );
    // TODO: implement
    //return fp::snd( fp::head( fp::until() ))
    return &htree.front();
  };
  let vecs = make_vector(s);
  return map( []( const pair<char,std::string>& p ) {
    return fp::show('\'') + fp::fst(p) + "\' : " + fp::fst(p);
  }, serialize( huffmanTree( freq( vecs ) ) ) );

}

///////////////////////////////////////////////////////////////////////////

int main(int argc, char **argv) {

  using namespace fp;

  print( "" );

  ///////////////////////////////////////////////////////////////////////////

  print( "PascalsTriangle( 6 )" );
  print( pascalsTriangle( 6 ) );
  //map( print<Row>, pascalsTriangle( 6 ) );p

  ///////////////////////////////////////////////////////////////////////////

  print( "\nNthRoot( 5, 34 )" );
  print( nthRoot( 5, 34. ) );
  print( "\nNthRoot( 3, 27 )" );
  print( nthRoot( 3, 27. ) );

  ///////////////////////////////////////////////////////////////////////////

  print( "\nAnagrams( UnixDict )" );
  print( anagrams( "./../../../samples/unixdict.txt" ) );
  //map( print<StringList>, anagrams( "./../../../samples/unixdict.txt" ) );

  ///////////////////////////////////////////////////////////////////////////

  std::array<float, 8> fftIn = {1, 1, 1, 1, 0, 0, 0, 0};
  print( show("\nFFT( ") + show( fftIn ) + show(" )") );
  print( fft( make_vector( fftIn ) ) );

  ///////////////////////////////////////////////////////////////////////////

  print( show("\nhuffman( \"this is an example for huffman encoding\" )") );
  print( huffman( "this is an example for huffman encoding" ) );

  print( "" );

  return 0;
}

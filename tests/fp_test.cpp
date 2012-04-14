/////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2012, Jared Duke.
// This code is released under the MIT License.
// www.opensource.org/licenses/mit-license.php
/////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>

#include <fpcpp.h>

#include <array>
#include <string>
#include <map>
#include <fstream>

#include "common.h"

template<typename T> T add(T t0, T t1)  { return t0 + t1; }
template<typename T> T sub(T t0, T t1)  { return t0 - t1; }
template<typename T> T mult(T t0, T t1) { return t0 * t1; }
template<typename T> T div(T t0, T t1)  { return t0 / t1; }

///////////////////////////////////////////////////////////////////////////

static const fp::types<float>::list  fVec10_1(10, 1.f);
static const fp::types<double>::list dVec10_2(10, 2.f);
let dVec5_1_5 = fp::increasingN(5, 1.);
let iVec5_0_5 = fp::increasingN(6, 0);
let iVec5_5_0 = fp::decreasingN(6, 5);

template<typename MapOp, typename FilterOp, typename Source>
bool filteredMap(MapOp mapOp, FilterOp filterOp, Source source, std::function<bool(result_type_of(MapOp))> successOp = &fp::istrue) {
  using namespace fp;
  return all(successOp,
             map(mapOp,
                 filter(filterOp,
                        source)));
}

double pi(size_t samples = 10000) {
  using namespace fp;
  using namespace fp::math;
  typedef std::pair<double,double> point;
  return 4.* length(filter([](double d) { return d <= 1.0; },
                           map([](const point& p) { return p.first*p.first + p.second*p.second; },
                               zip(uniformN(samples, -1.0, 1.0),
                                   uniformN(samples, -1.0, 1.0))))) / samples;
}

double pi_(size_t samples = 10000) {
  using namespace fp;

  let sample = []() -> int {
    let x = math::uniform(-1.,1.);
    let y = math::uniform(-1.,1.);
    let dist2 = x*x + y*y;
    return dist2 < 1. ? 1 : 0;
  };

  return 4. * foldl1(std::plus<int>(), takeF(samples, sample)) / samples;
}

TEST(Prelude, Fun) {
  using namespace fp;
  using std::string;
  using std::ifstream;
  using std::cout;
  using std::string;

  if (false) {
    let mp3Filter = [](const string& mp3) {
      return (!mp3.empty())  &&
             ( mp3[0] != '#') &&
             ( mp3.find_first_of(".mp3") != std::string::npos );
    };
    let mp3Delete = [=](const string& mp3) {
      return remove(mp3.c_str()) == 0;
    };

    ifstream ifs("playlist.m3u");
    let successful = filteredMap(mp3Delete, mp3Filter, lines(ifs));

    cout << "Deleting Playlist..." << (successful ? "Success!" : "Failure!!");
  }

  ///////////////////////////////////////////////////////////////////////////

  EXPECT_NEAR(3.14, pi(), .1);
  EXPECT_NEAR(3.14, pi_(), .1);
}

/////////////////////////////////////////////////////////////////////////////

TEST(Prelude, Map) {
  using fp::fold;
  using fp::map;
  using fp::sum;

  EXPECT_EQ(sum( map(add_2,  fVec10_1)),    30.0f);
  EXPECT_EQ(sum( map(sub_3,  dVec10_2)),   -10.0 );
  EXPECT_EQ(sum( map(mult_4, iVec5_5_0)),  0*4.f + 1*4.f + 2*4.f + 3*4.f + 4*4.f + 5*4.f);
  EXPECT_EQ(sum( map(div_5,  iVec5_0_5)),  0/5.f + 1/5.f + 2/5.f + 3/5.f + 4/5.f + 5/5.f);

  ///////////////////////////////////////////////////////////////////////////

  EXPECT_EQ(map(toupper, "abc"), "ABC");
}

TEST(Prelude, FoldL) {
  using fp::fold;
  using fp::foldl;
  using fp::foldl1;
  using fp::sum;
  using fp::product;

  EXPECT_EQ(sum(fVec10_1),    10.f);
  EXPECT_EQ(sum(dVec10_2),    20. );
  EXPECT_EQ(sum(iVec5_0_5),  0+1+2+3+4+5);
  EXPECT_EQ(sum(iVec5_5_0),  5+4+3+2+1+0);

  EXPECT_EQ(product(dVec10_2), pow(2.,10) );
  EXPECT_EQ(foldl1(std::divides<double>(),dVec10_2), pow(.5,8) );

  EXPECT_EQ(sum(fp::words(std::string("a b c d e f g h i j"))), "abcdefghij");
}

TEST(Prelude, FoldR) {
  using fp::foldr;
  using fp::foldr_;
  using fp::foldr1;
  using fp::foldr1_;

  let addf = &add<float>;
  let subi = &sub<int>;

  EXPECT_EQ(foldr1(addf, fVec10_1),    10.f);
  EXPECT_EQ(foldr1(subi, iVec5_0_5), 5-4-3-2-1-0);
  EXPECT_EQ(foldr1(subi, iVec5_5_0), 0-1-2-3-4-5);

  let add_s    = [](const std::string& s0, const std::string& s1) { return s0 + s1; };
  let add_s2   = &add<std::string>;
  EXPECT_EQ(foldr1(add_s,  fp::words(std::string("a b c d e f g h i j"))), std::string("jihgfedcba"));
  EXPECT_EQ(foldr1(add_s2, fp::words(std::string("a b c"))),               fp::sum(fp::reverse(fp::words(std::string("a b c")))));
}

TEST(Prelude, ScanL) {
  using fp::fold;
  using fp::foldl;
  using fp::foldl1;
  using fp::scanl;
  using fp::scanl1;
  using fp::sum;
  using fp::product;

  EXPECT_EQ(          foldl1(std::divides<double>(), dVec10_2),
            fp::last( scanl1(std::divides<double>(), dVec10_2)));
}

TEST(Prelude, ScanR) {
  using fp::foldr;
  using fp::foldr_;
  using fp::foldr1;
  using fp::foldr1_;
  using fp::scanr;
  using fp::scanr1;

  let addf = &add<float>;
  let subi = &sub<int>;

  EXPECT_EQ(foldr1(addf, fVec10_1),  fp::last( scanr1(addf, fVec10_1)));
  EXPECT_EQ(foldr1(subi, iVec5_0_5), fp::last( scanr1(subi, iVec5_0_5)));
  EXPECT_EQ(foldr1(subi, iVec5_5_0), fp::last( scanr1(subi, iVec5_5_0)));
}

TEST(Prelude, Filter) {
  using fp::filter;

  let lessThanEq2 = fp::sum(filter([](int x) { return x <= 2; }, iVec5_0_5));
  EXPECT_EQ(lessThanEq2, 0+1+2);
  let greaterThan2 = fp::sum(filter([](int x) { return x >  2; }, iVec5_0_5));
  EXPECT_EQ(greaterThan2, 3+4+5);

  std::string str("a b c d e f g");
  let strFilter = [](const std::string& s) -> bool { return s[0] < 'd'; };
  EXPECT_EQ(fp::sum(filter(strFilter, fp::words(str))), std::string("abc"));
}

TEST(Prelude, Zip) {
  using fp::zip;

  let ifZip  = zip(iVec5_0_5, iVec5_5_0);
  let pairSum = [](const std::pair<int,int>& a) -> int { return a.first + a.second; };
  let pairIs5 = [&](const std::pair<int,int>& a) -> int { return pairSum(a) == 5 ? 1 : 0; };
  let emptyVec = fp::list<int>();
  EXPECT_EQ(fp::sum(fp::__map__(pairIs5, ifZip, emptyVec)), 6);

}

TEST(Prelude, ZipWith) {
  using fp::zipWith;

  std::array<int,3> expectedSum = {4,4,4};
  EXPECT_EQ(fp::list(expectedSum),    zipWith(std::plus<int>(), fp::increasingN(3, 1), fp::decreasingN(3, 3)));

  std::array<float,5> expectedPow = {5.0f,25.0f,125.0f,625.0f,3125.0f};
  EXPECT_EQ(fp::list(expectedPow),    zipWith(powf, fp::types<float>::list(5, 5.f), fp::increasingN(5, 1.f)));

  std::array<int,4> expectedLambda = {7, 10, 13, 16};
  let lambda = [](int x, int y) -> int { return 2*x+y; };
  EXPECT_EQ(fp::list(expectedLambda), zipWith(lambda, fp::increasingN(4, 1), fp::increasingN(4, 5)));
}


TEST(Prelude, All) {
  using fp::all;

  EXPECT_TRUE( all([](float x) { return x == 1.f; }, fVec10_1));
  EXPECT_FALSE(all([](float x) { return x != 1.f; }, fVec10_1));

  EXPECT_TRUE( all([](double x) { return x == 2.; }, dVec10_2));
  EXPECT_FALSE(all([](double x) { return x != 2.; }, dVec10_2));

  EXPECT_TRUE( all([](double x) { return x < 10; }, fp::increasingN(10, 0.)));
}

TEST(Prelude, MinMax) {
  using fp::maximum;
  using fp::minimum;

  let zeroToThousand = fp::increasingN(1001, 0);
  EXPECT_EQ(1000, maximum(zeroToThousand));
  EXPECT_EQ(0,    minimum(zeroToThousand));
  zeroToThousand.pop_back();
  zeroToThousand[0] = 999;
  EXPECT_EQ(999,  maximum(zeroToThousand));
  EXPECT_EQ(1,    minimum(zeroToThousand));

  ///////////////////////////////////////////////////////////////////////////

  let randFloat = fp::math::uniform_<float>(0.f, 1.f);
  fp::types<float>::list randFloats;
  randFloats.push_back(randFloat());
  float randMax = randFloats.back();
  while (randFloats.size() < 1000) {
    randFloats.push_back(randFloat());
    float newRandMax = maximum(randFloats);
    EXPECT_LE(randMax, newRandMax);
    newRandMax = randMax;
  }

  ///////////////////////////////////////////////////////////////////////////

  std::array<double,5> testArray = {0, 1, 2, 3, 4};
  EXPECT_EQ(4, maximum(testArray));
  EXPECT_EQ(0, minimum(testArray));

  ///////////////////////////////////////////////////////////////////////////

  EXPECT_EQ('o', maximum(std::string("Hello")));
  EXPECT_EQ('H', minimum(std::string("Hello")));
}

TEST(Prelude, Reverse) {
  using fp::reverse;

  EXPECT_EQ("dcba", reverse(std::string("abcd")));

  EXPECT_EQ(fp::increasingN(5, 0), reverse(fp::decreasingN(5, 4)));
}

TEST(Prelude, Random) {
  using fp::uniformN;
  using fp::math::uniform;
  using fp::math::uniform_;

  enum {
    numRands = 10000,
    numRandBins = 10,
    minRandsPerBin = (int)((numRands / numRandBins) * .9),
    maxRandsPerBin = (int)((numRands / numRandBins) * 1.1)
  };

  static const float  minRand = 0.f;
  static const float  maxRand = (float)numRandBins;

  let randFloats = fp::uniformN(numRands, minRand, maxRand);
  EXPECT_GE(maxRand, fp::maximum(randFloats));
  EXPECT_LE(minRand, fp::maximum(randFloats));
  EXPECT_LE(5.f, fp::maximum(randFloats));
  EXPECT_GE(5.f, fp::minimum(randFloats));

  std::array<int, numRandBins> buckets = {0,0,0,0,0,0,0,0,0,0};
  std::for_each(extent(randFloats), [&buckets](float x) {
    ++buckets[(size_t)std::floor(x)];
  });

  std::for_each(extent(buckets), [=](int x) {
    EXPECT_GE(maxRandsPerBin, x);
    EXPECT_LE(minRandsPerBin, x);
  });
}

TEST(Prelude, EnumFrom) {
  using fp::enumFrom;

  let Zero_Ten  = fp::takeWhileT([](int x) -> bool { return x < 11; }, enumFrom(0));
  let Zero_Ten2 = fp::takeF(11, enumFrom(0));
  EXPECT_EQ(fp::sum(Zero_Ten),  55);
  EXPECT_EQ(fp::sum(Zero_Ten2), 55);

  ///////////////////////////////////////////////////////////////////////////

  let a_z       = fp::takeWhileT([](char x) -> bool { return x <= 'z'; }, enumFrom('a'));
  let a_z2      = fp::takeF(26, enumFrom('a'));
  const std::string atoz("abcdefghijklmnopqrstuvwxyz");
  EXPECT_EQ(fp::show(a_z),  atoz);
  EXPECT_EQ(fp::show(a_z2), atoz);

  ///////////////////////////////////////////////////////////////////////////

  let charEnumFrom0 = enumFrom('0');
  EXPECT_EQ(charEnumFrom0(), '0');
  EXPECT_EQ(charEnumFrom0(), '1');

  enum TestEnum {
    First=0,
    Last=1,
  };

  let enumEnumFrom = enumFrom(First);
  EXPECT_EQ(enumEnumFrom(), First);
  EXPECT_EQ(enumEnumFrom(), Last);

  let floatEnumFromNeg10 = enumFrom(-10.f);
  EXPECT_EQ(floatEnumFromNeg10(), -10.f);
  EXPECT_EQ(floatEnumFromNeg10(), -9.f);

  let ptrEnumFromNull = enumFrom(NULL);
  EXPECT_EQ(ptrEnumFromNull(), 0);
  EXPECT_EQ(ptrEnumFromNull(), 1);
}

TEST(Prelude, Elem) {
  using fp::elem;
  using fp::notElem;

  bool hasFind = fp::has_find<std::map<int,int>,int>::value;

  EXPECT_TRUE( elem(5,   iVec5_0_5));
  EXPECT_FALSE(notElem(5, iVec5_0_5));
  EXPECT_FALSE(elem(-1,  iVec5_0_5));
  EXPECT_TRUE( notElem(-1,  iVec5_0_5));
  EXPECT_FALSE(elem(0,   fp::increasingN(10, 1)));
  EXPECT_TRUE( notElem(0,   fp::increasingN(10, 1)));
  EXPECT_FALSE(elem(-1,  iVec5_0_5));
  EXPECT_TRUE( notElem(-1,  iVec5_0_5));
  EXPECT_TRUE( elem('o', "Hello"));
  EXPECT_FALSE(notElem('o', "Hello"));
  EXPECT_FALSE(elem(-1,  iVec5_0_5));
  EXPECT_TRUE( notElem(-1,  iVec5_0_5));
  EXPECT_FALSE(elem(10,  "Hello"));
  EXPECT_TRUE( notElem(10,  "Hello"));

  let pairs = fp::zip(fp::increasingN(10, 0), fp::decreasingN(10, 9));
  typedef decltype(pairs) PairVec;
  typedef PairVec::value_type Pair;
  pairs.push_back( Pair(33,33) );

  EXPECT_TRUE( elem(Pair(0,9),   pairs));
  EXPECT_TRUE( elem(Pair(33,33), pairs));
  EXPECT_FALSE(elem(Pair(44,44), pairs));
}

TEST(Prelude, Drop) {
  using fp::drop;
  using fp::dropWhile;

  EXPECT_EQ(0,   fp::sum(drop(10, fp::increasingN(10, 0))));
  EXPECT_EQ(9,   fp::sum(drop(9,  fp::increasingN(10, 0))));
  EXPECT_EQ(9+8, fp::sum(drop(8,  fp::increasingN(10, 0))));
  EXPECT_EQ(fp::increasingN(5,0), drop(0, fp::increasingN(5, 0)));

  EXPECT_EQ(0,   fp::sum(dropWhile([](int x) { return x < 10; }, fp::increasingN(10, 0))));
  EXPECT_EQ(9,   fp::sum(dropWhile([](int x) { return x < 9; }, fp::increasingN(10, 0))));
  EXPECT_EQ(9+8, fp::sum(dropWhile([](int x) { return x < 8; }, fp::increasingN(10, 0))));
  EXPECT_EQ(fp::decreasingN(5, 4), dropWhile([](int x) { return x > 4; }, fp::decreasingN(10, 9)));

}

TEST(Prelude, Take) {
  using fp::take;
  using fp::takeF;
	using fp::takeWhile;
	using fp::takeWhileF;

	EXPECT_EQ(0,   fp::sum(take(0, fp::increasingN(10, 0))));
	EXPECT_EQ(0,   fp::sum(take(1, fp::increasingN(10, 0))));
	EXPECT_EQ(1,   fp::sum(take(2, fp::increasingN(10, 0))));
	EXPECT_EQ(1+2, fp::sum(take(3, fp::increasingN(10, 0))));
	EXPECT_EQ(fp::increasingN(5,0), take(5, fp::increasingN(10, 0)));

	EXPECT_EQ(0,   fp::sum(takeWhile([](int x) { return x < 0; }, fp::increasingN(10, 0))));
	EXPECT_EQ(0,   fp::sum(takeWhile([](int x) { return x < 1; }, fp::increasingN(10, 0))));
	EXPECT_EQ(0+1, fp::sum(takeWhile([](int x) { return x < 2; }, fp::increasingN(10, 0))));
	EXPECT_EQ(fp::decreasingN(5, 9), takeWhile([](int x) { return x > 4; }, fp::decreasingN(10, 9)));
}


///////////////////////////////////////////////////////////////////////////

TEST(General, Comparing) {
  using fp::comparing;

  const std::array<std::string, 5> strings = {"one", "two", "three", "four", "five"};
  std::vector<std::string> stringVec(extent(strings));
  //let stringVec = fp::list(strings);n
  //EXPECT_EQ("three", fp::maximumBy(fp::comparing(fp::length<std::string>), stringVec));

  let zippedList = fp::zip(fp::increasingN(10, 0), fp::decreasingN(10, 9));
  let sortedList = fp::sortBy(fp::comparing(&fp::snd<int,int>), zippedList);
  EXPECT_EQ(zippedList.back(), sortedList.front());
}

TEST(General, Flip) {
  using fp::flip;
  EXPECT_EQ(std::divides<float>()(3.f,4.f),     flip(std::divides<float>())(4.f,3.f));
  EXPECT_EQ(std::plus<std::string>()("3", "4"), flip(std::plus<std::string>())("4", "3"));
}

///////////////////////////////////////////////////////////////////////////

template <typename T>
T multBy2(T x) {
  return x * (T)2;
}

template <typename T>
T exor(T x, T y) {
  return x ^ y;
}

template <typename T>
T mult3(T x, T y, T z) {
  return x*y*z;
}

TEST(Curry, Everything) {
  using fp::curry;
  using fp::curry2;
  using fp::curry3;
  using fp::curryAll2;

  // No args
  EXPECT_EQ(2.f,   curry(&multBy2<float>, 1.f)());
  EXPECT_EQ(2.f,   curry2(std::plus<float>(),       1.f, 1.f)());
  EXPECT_EQ(2.f,   curry2(std::multiplies<float>(), 1.f, 2.f)());
  EXPECT_EQ(99^77, curry2(&exor<int>, 99, 77)());
  EXPECT_EQ(2.f,   curry3(mult3<float>, 8.f, .5f, .5f)());

  EXPECT_EQ(2.f,   curryAll2(fp::math::addF(),      1.f, 1.f)());
  EXPECT_EQ(2.f,   curryAll2(fp::math::multiplyF(), 1.f, 2.f)());

  // 1 arg
  EXPECT_EQ(2.f,   curry(std::plus<float>(),       1.f)(1.f));
  EXPECT_EQ(2.f,   curry(std::multiplies<float>(), 1.f)(2.f));
  EXPECT_EQ(99^77, curry(&exor<int>, 99)(77));
  EXPECT_EQ(2.f,   curry2(&mult3<float>, 8.f, .5f)(.5f));

  // 2 args
  EXPECT_EQ(2.f,   curry(&mult3<float>, 8.f)(.5f, .5f));

  EXPECT_EQ((float)1*2*3*4*5, curry(fp::foldl1<std::function<float(float,float)>,fp::types<float>::list >, std::multiplies<float>())(fp::increasingN(5, 1.f)));
#ifndef _MSC_VER
  EXPECT_EQ((float)1*2*3*4*5, std::bind(fp::foldl1F(), fp::math::multiplyF(), std::placeholders::_1)(fp::increasingN(5, 1.f)));
#endif
}

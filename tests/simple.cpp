#include <gtest/gtest.h>

#include <fc.h>
#include <fpcpp.h>

#include <array>
#include <string>
#include <map>
#include <vector>
#include <fstream>

#include "common.h"

template<typename T> T add(T t0, T t1)  { return t0 + t1; }
template<typename T> T sub(T t0, T t1)  { return t0 - t1; }
template<typename T> T mult(T t0, T t1) { return t0 * t1; }
template<typename T> T div(T t0, T t1)  { return t0 / t1; }

let fold_sub  = fp::foldl_(&sub<double>);
let fold_mult = fp::foldl_(&mult<double>);
let fold_div  = fp::foldl_(&div<double>);

///////////////////////////////////////////////////////////////////////////

static const std::vector<float>  fVec10_1(10, 1.f);
static const std::vector<double> dVec10_2(10, 2.f);
let dVec5_1_5 = fp::increasing_n(5, 1.);
let iVec5_0_5 = fp::increasing_n(6, 0);
let iVec5_5_0 = fp::decreasing_n(6, 5);

TEST(Prelude, Fun) {
  using namespace fp;
  using std::string;
  using std::ifstream;
  using std::cout;

  ifstream playlistFile("TestList.m3u");
  enum status { success = 0, failure };
  typedef std::pair<string,status> line;

  let lineSource = [=,&playlistFile]() -> line {
    string s; 
    return getline(playlistFile, s) ? line(s,success) : line("",failure);
  };
  let mp3Filter = [](const line& mp3) { 
    return (!mp3.first.empty())  && 
           ( mp3.first[0] != '#') &&
           ( mp3.first.find_first_of(".mp3") != std::string::npos);
  };
  let mp3Delete = [=](const line& mp3) {
    return remove(mp3.first.c_str()) == 0 ? success : failure; 
  };
  let successful = notElem(failure,
                           map(mp3Delete, 
                               filter(mp3Filter, 
                                      takeWhileF([=](const line& s) { return s.second != failure; },
                                                 lineSource))));

  cout << "Deleting Playlist..." << success ? "Success!" : "Failure!!";

  ///////////////////////////////////////////////////////////////////////////
  

}

/////////////////////////////////////////////////////////////////////////////

TEST(Prelude, Map) {
  using fp::fold;
  using fp::map;

  EXPECT_EQ(fold( map(add_2,  fVec10_1)),    30.0f);
  EXPECT_EQ(fold( map(sub_3,  dVec10_2)),   -10.0 );
  EXPECT_EQ(fold( map(mult_4, iVec5_5_0)),  0*4.f + 1*4.f + 2*4.f + 3*4.f + 4*4.f + 5*4.f);
  EXPECT_EQ(fold( map(div_5,  iVec5_0_5)),  0/5.f + 1/5.f + 2/5.f + 3/5.f + 4/5.f + 5/5.f);

  ///////////////////////////////////////////////////////////////////////////

  EXPECT_EQ(map(toupper, "abc"), "ABC");
}

TEST(Prelude, FoldL) {
  using fp::fold;
  using fp::foldl;

  EXPECT_EQ(fold(fVec10_1),    10.f);
  EXPECT_EQ(fold(dVec10_2),    20. );
  EXPECT_EQ(fold(iVec5_0_5),  0+1+2+3+4+5);
  EXPECT_EQ(fold(iVec5_5_0),  5+4+3+2+1+0);

  EXPECT_EQ(fold_mult(dVec10_2), pow(2.,10) );
  EXPECT_EQ(fold_div(dVec10_2), pow(.5,8) );
  //EXPECT_EQ(fold_div(dVec10_2) * fold_mult(dVec10_2), 1.);

  EXPECT_EQ(fold(fp::words(std::string("a b c d e f g h i j"))), "abcdefghij");
}

TEST(Prelude, FoldR) {
  using fp::foldr;
  using fp::foldr_;

  let addf = &add<float>;
  let subi = &sub<int>;

  EXPECT_EQ(foldr(addf, fVec10_1),    10.f);
  EXPECT_EQ(foldr(subi, iVec5_0_5), 5-4-3-2-1-0);
  EXPECT_EQ(foldr(subi, iVec5_5_0), 0-1-2-3-4-5);

  let add_s = [](const std::string& s0, const std::string& s1) { return s0 + s1; };
  let add_s2 = &add<std::string>;
  let foldr_s = foldr_(add_s);
  let foldr_s2 = foldr_(add_s2);
  EXPECT_EQ(foldr_s(fp::words(std::string("a b c d e f g h i j")), std::string()), "jihgfedcba");
  EXPECT_EQ(foldr_s(fp::words(std::string("a b c")), std::string()), fp::fold(fp::reverse(fp::words(std::string("a b c")))));
}

TEST(Prelude, Filter) {
  using fp::filter;

  let lessThanEq2 = fp::fold(filter([](int x) { return x <= 2; }, iVec5_0_5));
  EXPECT_EQ(lessThanEq2, 0+1+2);
  let greaterThan2 = fp::fold(filter([](int x) { return x >  2; }, iVec5_0_5));
  EXPECT_EQ(greaterThan2, 3+4+5);

  std::string str("a b c d e f g");
  let strFilter = [](const std::string& s) -> bool { return s[0] < 'd'; };
  EXPECT_EQ(fp::fold(filter(strFilter, fp::words(str))), std::string("abc"));
}

TEST(Prelude, Zip) {
  using fp::zip;

  let ifZip  = zip(iVec5_0_5, iVec5_5_0);
  let pairSum = [](const std::pair<int,int>& a) -> int { return a.first + a.second; };
  let pairIs5 = [&](const std::pair<int,int>& a) -> int { return pairSum(a) == 5 ? 1 : 0; };
  EXPECT_EQ(fp::fold(fp::__map__(pairIs5, ifZip, std::vector<int>())), 6);

  ///////////////////////////////////////////////////////////////////////////

}

TEST(Prelude, ZipWith) {
  using fp::zipWith;

  int expectedSum[] = {4,4,4};
  EXPECT_EQ(fp::make_vector(expectedSum),    zipWith(std::plus<int>(), fp::increasing_n(3, 1), fp::decreasing_n(3, 3)));

  float expectedPow[] = {5.0f,25.0f,125.0f,625.0f,3125.0f};
  EXPECT_EQ(fp::make_vector(expectedPow),    zipWith(std::powf, std::vector<float>(5, 5.f), fp::increasing_n(5, 1.f)));

  int expectedLambda[] = {7, 10, 13, 16};
  let lambda = [](int x, int y) -> int { return 2*x+y; };
  EXPECT_EQ(fp::make_vector(expectedLambda), zipWith(lambda, fp::increasing_n(4, 1), fp::increasing_n(4, 5)));
}

TEST(Prelude, MinMax) {
  using fp::maximum;
  using fp::minimum;

  let zeroToThousand = fp::increasing_n(1001, 0);
  EXPECT_EQ(1000, maximum(zeroToThousand));
  EXPECT_EQ(0,    minimum(zeroToThousand));
  zeroToThousand.pop_back();
  zeroToThousand[0] = 999;
  EXPECT_EQ(999,  maximum(zeroToThousand));
  EXPECT_EQ(1,    minimum(zeroToThousand));

  ///////////////////////////////////////////////////////////////////////////

  let randFloat = fp::rand_range_<float>();
  std::vector<float> randFloats;
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

TEST(Prelude, Random) {
  using fp::rand_n;
  using fp::rand_range;
  using fp::rand_range_;

  enum {
    numRands = 10000,
    numRandBins = 10,
    minRandsPerBin = (int)((numRands / numRandBins) * .9),
    maxRandsPerBin = (int)((numRands / numRandBins) * 1.1)
  };

  static const float  minRand = 0.f;
  static const float  maxRand = (float)numRandBins;

  let randFloats = generate_n(numRands, rand_range_<float>(minRand, maxRand));
  EXPECT_GE(maxRand, fp::maximum(randFloats));
  EXPECT_LE(minRand, fp::maximum(randFloats));
  EXPECT_LE(5.f, fp::maximum(randFloats));
  EXPECT_GE(5.f, fp::minimum(randFloats));

  std::array<int, numRandBins> buckets = {0,0,0,0,0,0,0,0,0,0};
  std::for_each(fp::head(randFloats), fp::tail(randFloats), [&buckets](float x) {
    ++buckets[(size_t)std::floor(x)];
  });

  std::for_each(fp::head(buckets), fp::tail(buckets), [=](int x) {
    EXPECT_GE(maxRandsPerBin, x);
    EXPECT_LE(minRandsPerBin, x);
  });
}

TEST(Prelude, EnumFrom) {
  using fp::enumFrom;

  let Zero_Ten  = fp::takeWhileF([](int x) -> bool { return x < 11; }, enumFrom(0));
  let Zero_Ten2 = fp::takeF(11, enumFrom(0));
  EXPECT_EQ(fp::fold(Zero_Ten),  55);
  EXPECT_EQ(fp::fold(Zero_Ten2), 55);

  ///////////////////////////////////////////////////////////////////////////

  let a_z       = fp::takeWhileF([](char x) -> bool { return x <= 'z'; }, enumFrom('a'));
  let a_z2      = fp::takeF(26, enumFrom('a'));
  let vectostring = [](std::vector<char> s) { return std::string(fp::head(s), fp::tail(s)); };
  const std::string atoz("abcdefghijklmnopqrstuvwxyz");
  EXPECT_EQ(vectostring(a_z),  atoz);
  EXPECT_EQ(vectostring(a_z2), atoz);

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

  bool hasFind = fp::has_find<std::map<int,int>,int>::value;

  EXPECT_EQ(true,  elem(5,  iVec5_0_5));
  EXPECT_EQ(false, elem(-1, iVec5_0_5));
  EXPECT_EQ(false, elem(0,  fp::increasing_n(10, 1)));
  EXPECT_EQ(true,  elem('o', "Hello"));
  EXPECT_EQ(false, !elem('o', "Hello"));
  EXPECT_EQ(false, elem(10, "Hello"));
  EXPECT_EQ(true,  !elem(10, "Hello"));

  std::map<int,int> testMap;
  let pairs = fp::zip(fp::increasing_n(10, 0), fp::decreasing_n(10, 9));
  testMap.insert(extent(pairs));
  //EXPECT_EQ(true, elem(std::pair<int,int>(0,9), testMap));
  //test.insert( std::pair<int,int>(1,2) );
  //test.insert( std::pair<int,int>(1,2) );
  //EXPECT_EQ()

}


///////////////////////////////////////////////////////////////////////////

TEST(Curry, NoArgs) {

}

TEST(Curry, Args) {

}

TEST(Curry, Compound) {

}

///////////////////////////////////////////////////////////////////////////


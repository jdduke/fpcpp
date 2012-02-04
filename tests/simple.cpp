#include <gtest/gtest.h>

#include <fc.h>
#include <fpcpp.h>

#include <array>
#include <string>
#include <vector>

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
let dVec10_0_9 = fp::increasing_n(5, 1.);
let iVec10_0_9 = fp::increasing_n(10, 0);
let iVec10_9_0 = fp::decreasing_n(10, 9);

/////////////////////////////////////////////////////////////////////////////

TEST(Prelude, Map) {
  using fp::fold;
  using fp::map;

  EXPECT_EQ(fold( map(add_2,  fVec10_1)),    30.0f);
  EXPECT_EQ(fold( map(sub_3,  dVec10_2)),   -10.0 );
  EXPECT_EQ(fold( map(div_5,  iVec10_0_9)),  5);
  EXPECT_EQ(fold( map(mult_4, iVec10_9_0)),  180);
}

TEST(Prelude, FoldL) {
  using fp::fold;
  using fp::foldl;

  EXPECT_EQ(fold(fVec10_1),    10.f);
  EXPECT_EQ(fold(dVec10_2),    20. );
  EXPECT_EQ(fold(iVec10_0_9),  45);
  EXPECT_EQ(fold(iVec10_9_0),  45);

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
  EXPECT_EQ(foldr(subi, iVec10_0_9), -27);
  EXPECT_EQ(foldr(subi, iVec10_0_9), fp::foldl(subi, iVec10_9_0));

  let add_s = [](const std::string& s0, const std::string& s1) { return s0 + s1; };
  let add_s2 = &add<std::string>;
  let foldr_s = foldr_(add_s);
  let foldr_s2 = foldr_(add_s2);
  EXPECT_EQ(foldr_s(fp::words(std::string("a b c d e f g h i j")), std::string()), "jihgfedcba");
  EXPECT_EQ(foldr_s(fp::words(std::string("a b c")), std::string()), fp::fold(fp::reverse(fp::words(std::string("a b c")))));
}

TEST(Prelude, Filter) {
  using fp::filter;

  let r15 = fp::fold(filter([](int x) { return x <= 5; }, iVec10_0_9));
  EXPECT_EQ(r15, 15);
  let r30 = fp::fold(filter([](int x) { return x >  5; }, iVec10_0_9));
  EXPECT_EQ(r30, 30);

  std::string str("a b c d e f g");
  let strFilter = [](const std::string& s) -> bool { return s[0] < 'd'; };
  EXPECT_EQ(fp::unwords(filter(strFilter, fp::words(str))), std::string("a b c"));
}

TEST(Prelude, Zip) {
  using fp::zip;

  let ifZip  = zip(iVec10_0_9, iVec10_9_0);
  let pairSum = [](const std::pair<int,int>& a) -> int { return a.first + a.second; };
  let pairIs9 = [&](const std::pair<int,int>& a) -> int { return pairSum(a) == 9 ? 1 : 0; };
  EXPECT_EQ(fp::fold(fp::__map__(pairIs9, ifZip, std::vector<int>())), 10);

  ///////////////////////////////////////////////////////////////////////////

}

TEST(Prelude, ZipWith) {



  
}

TEST(Prelude, MinMax) {
  using fp::maximum;
  using fp::minimum;

  let zeroToThousand = fp::increasing_n(1001, 0);
  EXPECT_EQ(maximum(zeroToThousand), 1000);
  EXPECT_EQ(minimum(zeroToThousand), 0);
  zeroToThousand.pop_back();
  EXPECT_EQ(maximum(zeroToThousand), 999);
  zeroToThousand[0] = 999;
  EXPECT_EQ(minimum(zeroToThousand), 1);

  ///////////////////////////////////////////////////////////////////////////

  let randFloat = fp::rand_range_<float>();
  std::vector<float> randFloats;
  randFloats.push_back(randFloat());
  float randMax = randFloats.back();
  while (randFloats.size() < 1000) {
    randFloats.push_back(randFloat());
    float newRandMax = maximum(randFloats);
    EXPECT_GE(newRandMax, randMax);
    newRandMax = randMax;
  }

  ///////////////////////////////////////////////////////////////////////////

  std::array<double,5> testArray = {0, 1, 2, 3, 4};
  EXPECT_EQ(maximum(testArray), 4);
  EXPECT_EQ(minimum(testArray), 0);

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

///////////////////////////////////////////////////////////////////////////

TEST(Curry, NoArgs) {

}

TEST(Curry, Args) {

}

TEST(Curry, Compound) {

}

///////////////////////////////////////////////////////////////////////////


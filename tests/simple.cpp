#include <gtest/gtest.h>

#include <fc.h>
#include <fpcpp.h>

#include "common.h"

using namespace fp;
using namespace prelude;

template<typename T> T add(T t0, T t1)  { return t0 + t1; }
template<typename T> T sub(T t0, T t1)  { return t0 - t1; }
template<typename T> T mult(T t0, T t1) { return t0 * t1; }
template<typename T> T div(T t0, T t1)  { return t0 / t1; }

let fold_f = foldl_with(&add<float>);
let fold_d = foldl_with(&add<double>);
let fold_i = foldl_with(&add<int>);
let fold_s = foldl_with(&add<std::string>);

let fold_add  = foldl_with(&add<double>);
let fold_sub  = foldl_with(&sub<double>);
let fold_mult = foldl_with(&mult<double>);
let fold_div  = foldl_with(&div<double>);

///////////////////////////////////////////////////////////////////////////

static const std::vector<float>  fVec10_1(10, 1.f);
static const std::vector<double> dVec10_2(10, 2.f);
let dVec10_0_9 = increasing_n(5, 1.);
let iVec10_0_9 = increasing_n(10, 0);
let iVec10_9_0 = decreasing_n(10, 9);

/////////////////////////////////////////////////////////////////////////////

template<typename A, typename B, typename C, typename D>
void testNoArgs(A a_, B b_, C c_, D d_)
{
  let a = compose(a_,a_);
  let b = compose(b_,c_);
  let c = compose(c_,b_);
  let d = compose(d_,c_);

  a();
  b();
  EXPECT_EQ(1.f, c(1.f));
  EXPECT_EQ(1.f, d());
  EXPECT_EQ(1.f, (d + a)());
}

TEST(Prelude, Map) {
  EXPECT_EQ(fold_f( map(add_2,  fVec10_1) ),    30.0f);
  EXPECT_EQ(fold_d( map(sub_3,  dVec10_2) ),   -10.0 );
  EXPECT_EQ(fold_i( map(div_5,  iVec10_0_9) ),  5);
  EXPECT_EQ(fold_i( map(mult_4, iVec10_9_0) ),  180);
}

TEST(Prelude, FoldL) {

  EXPECT_EQ(fold_f(fVec10_1),    10.f);
  EXPECT_EQ(fold_d(dVec10_2),    20. );
  EXPECT_EQ(fold_i(iVec10_0_9),  45);
  EXPECT_EQ(fold_i(iVec10_9_0),  45);

  EXPECT_EQ(fold_mult(dVec10_2, 1.), pow(2.,10) );
  EXPECT_EQ(fold_div(dVec10_2,  1.), pow(.5,10) );
  EXPECT_EQ(fold_div(dVec10_2,  1.) * fold_mult(dVec10_2, 1.), 1.);

  EXPECT_EQ(fold_s(words(std::string("a b c d e f g h i j"))), "abcdefghij");
}

TEST(Prelude, FoldR) {
  let addf = &add<float>;
  let subi = &sub<int>;

  EXPECT_EQ(foldr(addf, fVec10_1,   0.f),    10.f);
  EXPECT_EQ(foldr(subi, iVec10_0_9, 0),  -45);
  EXPECT_EQ(foldr(subi, iVec10_0_9, 0), foldl(subi, iVec10_9_0, 0));

  let foldr_s = foldr_with(&add<std::string>);
  EXPECT_EQ(foldr_s(words(std::string("a b c d e f g h i j"))), "jihgfedcba");
  EXPECT_EQ(foldr_s(words(std::string("a b c"))), fold_s(reverse(words(std::string("a b c")))));
}

TEST(Prelude, Filter) {
  let r15 = fold_i(filter([](int x) { return x <= 5; }, iVec10_0_9));
  EXPECT_EQ(r15, 15);
  let r30 = fold_i(filter([](int x) { return x >  5; }, iVec10_0_9));
  EXPECT_EQ(r30, 30);

  let str = std::string("a b c d e f g");
  let strFilter = [](const std::string& s) { return s[0] < 'd'; };
  EXPECT_EQ(unwords(filter(strFilter, words(str))), "a b c");
}

TEST(Prelude, Zip) {
  let ifZip  = zip(iVec10_0_9, iVec10_9_0);
  let pairSum = [](const std::pair<int,int>& a) -> int { return a.first + a.second; };
  let pairIs9 = [&](const std::pair<int,int>& a) -> int { return pairSum(a) == 9 ? 1 : 0; };
  EXPECT_EQ(fold_i(__map__(pairIs9, ifZip, std::vector<int>())), 10);
}

TEST(Prelude, ZipWith) {
  
}

TEST(Prelude, Maximum) {

}

TEST(Prelude, Random) {

}

///////////////////////////////////////////////////////////////////////////

TEST(Curry, NoArgs) {

}

TEST(Curry, Args) {

}

TEST(Curry, Compound) {

}

///////////////////////////////////////////////////////////////////////////


#ifndef _FC_TEST_COMMON_H_
#define _FC_TEST_COMMON_H_

inline void  Void_Void () { }
struct S_Void_Void{ void operator()() { }; };
auto   L_Void_Void = []() { };

inline float Float_Void () { return 1.f; }
struct S_Float_Void { float operator()() { return Float_Void(); } };
auto   L_Float_Void = []() { return Float_Void(); };

inline void  Void_Float (float x) { x = 1.f; }
struct S_Void_Float { void operator()(float x) { Void_Float(x); } };
auto   L_Void_Float = [](float x) { Void_Float(x); };

inline void Void_Float2 (float x, float y) { Void_Float(x); Void_Float(y); }
struct S_Void_Float2 { void operator()(float x, float y) { Void_Float2(x,y); } };
auto   L_Void_Float2 = [](float x, float y) { Void_Float2(x,y); };

inline float Float_Float (float x) { return x; }
struct S_Float_Float { float operator()(float x) { return Float_Float(x); } };
auto   L_Float_Float = [](float x) { return Float_Float(x); };

inline float Float_Float2 (float x, float y) { return x + y; }
struct S_Float_Float2 { float operator()(float x, float y) { return Float_Float2(x,y); } };
auto   L_Float_Float2 = [](float x, float y) { return Float_Float2(x,y); };

inline float Float_Float3 (float x, float y, float z) { return x + y + z; }
struct S_Float_Float3 { float operator()(float x, float y, float z) { return Float_Float3(x,y,z); } };
auto   L_Float_Float3 = [](float x, float y, float z) { return Float_Float3(x,y,z); };

inline float Float_Float4 (float x, float y, float z, float w) { return x + y + z + w; }
struct S_Float_Float4 { float operator()(float x, float y, float z, float w) { return Float_Float4(x,y,z,w); } };
auto   L_Float_Float4 = [](float x, float y, float z, float w) { return Float_Float4(x,y,z,w); };

auto add_2          = [](float x) -> float { return x + 2.f; };
auto sub_3          = [](float x) -> float { return x - 3.f; };
auto mult_4         = [](float x) -> float { return x * 4.f; };
auto div_5          = [](float x) -> float { return x / 5.f; };


#endif
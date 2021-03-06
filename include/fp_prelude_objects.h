/////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2012, Jared Duke.
// This code is released under the MIT License.
// www.opensource.org/licenses/mit-license.php
/////////////////////////////////////////////////////////////////////////////

#ifndef _FP_PRELUDE_OBJECTS_H_
#define _FP_PRELUDE_OBJECTS_H_

#include "fpcpp.h"

namespace fp {

///////////////////////////////////////////////////////////////////////////
// common

FP_DEFINE_FUNCTION_OBJECT(head,          headF);
FP_DEFINE_FUNCTION_OBJECT(tail,          tailF);
FP_DEFINE_FUNCTION_OBJECT(last,          lastF);
FP_DEFINE_FUNCTION_OBJECT(length,        lengthF);
FP_DEFINE_FUNCTION_OBJECT(null,          nullF);
FP_DEFINE_FUNCTION_OBJECT(list,          listF);
FP_DEFINE_FUNCTION_OBJECT(fromString,    fromStringF);
FP_DEFINE_FUNCTION_OBJECT(fromIntegral,  fromIntegralF);


///////////////////////////////////////////////////////////////////////////
// prelude

FP_DEFINE_FUNCTION_OBJECT(lines,     linesF);
FP_DEFINE_FUNCTION_OBJECT(unlines,   unlinesF);
FP_DEFINE_FUNCTION_OBJECT(words,     wordsF);
FP_DEFINE_FUNCTION_OBJECT(unwords,   unwordsF);
FP_DEFINE_FUNCTION_OBJECT(show,      showF);
FP_DEFINE_FUNCTION_OBJECT(putStr,    putStrF);
FP_DEFINE_FUNCTION_OBJECT(putStrLen, putStrLenF);
FP_DEFINE_FUNCTION_OBJECT(print,     printF);
FP_DEFINE_FUNCTION_OBJECT(sort,      sortF);
FP_DEFINE_FUNCTION_OBJECT(enumFrom,  enumFromF);
FP_DEFINE_FUNCTION_OBJECT(concat,    concatF);
FP_DEFINE_FUNCTION_OBJECT(append,    appendF);
FP_DEFINE_FUNCTION_OBJECT(cons,      consF);
FP_DEFINE_FUNCTION_OBJECT(fst,       fstF);
FP_DEFINE_FUNCTION_OBJECT(snd,       sndF);
FP_DEFINE_FUNCTION_OBJECT(swap,      swapF);


///////////////////////////////////////////////////////////////////////////
// math

namespace math {
FP_DEFINE_FUNCTION_OBJECT(add,      addF);
FP_DEFINE_FUNCTION_OBJECT(subtract, subtractF);
FP_DEFINE_FUNCTION_OBJECT(divide,   divideF);
FP_DEFINE_FUNCTION_OBJECT(multiply, multiplyF);
FP_DEFINE_FUNCTION_OBJECT(negate,   negateF);
FP_DEFINE_FUNCTION_OBJECT(mod,      modF);
FP_DEFINE_FUNCTION_OBJECT(sqrt,     sqrtF);
FP_DEFINE_FUNCTION_OBJECT(abs,      absF);
FP_DEFINE_FUNCTION_OBJECT(logicalAnd, logicalAndF);
FP_DEFINE_FUNCTION_OBJECT(logicalOr,  logicalOrF);
FP_DEFINE_FUNCTION_OBJECT(logicalNot, logicalNotF);
FP_DEFINE_FUNCTION_OBJECT(equals,        equalsF);
FP_DEFINE_FUNCTION_OBJECT(notEquals,     notEqualsF);
FP_DEFINE_FUNCTION_OBJECT(greaterEquals, greaterEqualsF);
FP_DEFINE_FUNCTION_OBJECT(greater,       greaterF);
FP_DEFINE_FUNCTION_OBJECT(lessEquals,    lessEqualsF);
FP_DEFINE_FUNCTION_OBJECT(less,          lessF);
FP_DEFINE_FUNCTION_OBJECT(even,          evenF);
FP_DEFINE_FUNCTION_OBJECT(odd,           oddF);
FP_DEFINE_FUNCTION_OBJECT(powerOfTwo,    powerOfTwoF);
FP_DEFINE_FUNCTION_OBJECT(uniform,       uniformF);

}


} /* namespace fp */

#endif /* _FP_PRELUDE_OBJECTS_H_ */

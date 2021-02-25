#ifndef NH_ECMASCRIPT_TEST_AND_COMPARE_H
#define NH_ECMASCRIPT_TEST_AND_COMPARE_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "ObjectType.h"

#endif

/** @addtogroup NhECMAScriptFunctions
 *  @{
 */

// https://www.262.ecma-international.org/11.0/index.html#sec-testing-and-comparison-operations

//    NH_BOOL Nh_ECMAScript_requireObjectCoercible(
//        argument
//    );
//
//    NH_BOOL Nh_ECMAScript_isArray(
//        argument
//    );

    NH_ECMASCRIPT_BOOLEAN Nh_ECMAScript_isCallable(
        Nh_ECMAScript_Any Value
    );

//    NH_BOOL Nh_ECMAScript_isConstructor(
//        Nh_ECMAScript_Type Value
//    );

    Nh_ECMAScript_Completion Nh_ECMAScript_isExtensible(
        Nh_ECMAScript_Object *Object_p
    );

//    NH_BOOL Nh_ECMAScript_isInteger(
//        argument
//    );
//
//    NH_BOOL Nh_ECMAScript_isNonNegativeInteger(
//        argument
//    );
//
//    NH_BOOL Nh_ECMAScript_isPropertyKey(
//        Nh_ECMAScript_Type Argument
//    );
//
//    NH_BOOL Nh_ECMAScript_isRegExp(
//        argument
//    );
//
//    NH_BOOL Nh_ECMAScript_isStringPrefix(
//        p, q
//    );
//
//    NH_BOOL Nh_ECMAScript_sameValue(
//        x, y
//    );
//
//    NH_BOOL Nh_ECMAScript_sameValueZero(
//        x, y
//    );
//
//    NH_BOOL Nh_ECMAScript_sameValueNonNumeric(
//        x, y
//    );
//
//    NH_BOOL Nh_ECMAScript_abstractRelationalComparison(
//    );
//
//    NH_BOOL Nh_ECMAScript_abstractEqualityComparison(
//    );
//
//    NH_BOOL Nh_ECMAScript_strictEqualityComparison(
//    );

/** @} */

#endif

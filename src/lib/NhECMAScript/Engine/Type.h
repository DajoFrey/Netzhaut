#ifndef NH_ECMASCRIPT_TYPE_H
#define NH_ECMASCRIPT_TYPE_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "../Common/Result.h"
#include "../../NhCore/Unicode.h"

typedef struct Nh_ECMAScript_Object Nh_ECMAScript_Object;

#endif

/** @addtogroup NhECMAScriptEnums
 *  @{
 */

    typedef enum NH_ECMASCRIPT_TYPE {
        NH_ECMASCRIPT_TYPE_UNDEFINED,
        NH_ECMASCRIPT_TYPE_NULL,
        NH_ECMASCRIPT_TYPE_BOOLEAN,
        NH_ECMASCRIPT_TYPE_STRING,
        NH_ECMASCRIPT_TYPE_SYMBOL,
        NH_ECMASCRIPT_TYPE_NUMBER,
        NH_ECMASCRIPT_TYPE_BIG_INT,
        NH_ECMASCRIPT_TYPE_OBJECT,
    } NH_ECMASCRIPT_TYPE;

/** @} */

/** @addtogroup NhECMAScriptMacros
 *  @{
 */

    #define NH_ECMASCRIPT_UNDEFINED 0
    #define NH_ECMASCRIPT_NULL NULL
    #define NH_ECMASCRIPT_BOOLEAN NH_BOOL
    #define NH_ECMASCRIPT_NUMBER double
    #define Nh_ECMAScript_String Nh_UTF8String

/** @} */

/** @addtogroup NhECMAScriptStructs
 *  @{
 */

    typedef struct Nh_ECMAScript_Symbol {
        Nh_ECMAScript_String *Description_p;
    } Nh_ECMAScript_Symbol;

    typedef struct Nh_ECMAScript_Any {
        NH_ECMASCRIPT_TYPE type;
        void *handle_p;
    } Nh_ECMAScript_Any;

/** @} */

/** @addtogroup NhECMAScriptFunctions
 *  @{
 */

// corresponds to https://tc39.es/ecma262/#sec-type-conversion

    void Nh_ECMAScript_toPrimitive(
        Nh_ECMAScript_Any Input, NH_ECMASCRIPT_TYPE preferredType 
    );

    void Nh_ECMAScript_toBoolean(
        Nh_ECMAScript_Any Argument 
    );

    void Nh_ECMAScript_toNumeric(
        Nh_ECMAScript_Any Value
    );

    void Nh_ECMAScript_toNumber(
        Nh_ECMAScript_Any Argument
    );

    void Nh_ECMAScript_toIntegerOrInfinity(
        Nh_ECMAScript_Any Argument
    );

    void Nh_ECMAScript_toInt32(
        Nh_ECMAScript_Any Argument
    );

    void Nh_ECMAScript_toUint32(
        Nh_ECMAScript_Any Argument
    );

    void Nh_ECMAScript_toInt16(
        Nh_ECMAScript_Any Argument
    );

    void Nh_ECMAScript_toUint16( 
        Nh_ECMAScript_Any Argument
    );

    void Nh_ECMAScript_toInt8( 
        Nh_ECMAScript_Any Argument
    );

    void Nh_ECMAScript_toUint8( 
        Nh_ECMAScript_Any Argument
    );

    void Nh_ECMAScript_toUint8Clamp( 
        Nh_ECMAScript_Any Argument
    );

    void Nh_ECMAScript_toBigInt( 
        Nh_ECMAScript_Any Argument
    );

    void Nh_ECMAScript_stringToBigInt( 
        Nh_ECMAScript_Any Argument
    );

    void Nh_ECMAScript_toBigInt64( 
        Nh_ECMAScript_Any Argument
    );

    void Nh_ECMAScript_toBigUint64( 
        Nh_ECMAScript_Any Argument
    );

    void Nh_ECMAScript_toString( 
        Nh_ECMAScript_Any Argument
    );

    void Nh_ECMAScript_toObject( 
        Nh_ECMAScript_Any Argument
    );

    void Nh_ECMAScript_toPropertyKey(
        Nh_ECMAScript_Any Argument
    );

    void Nh_ECMAScript_toLength(
        Nh_ECMAScript_Any Argument
    );

    void Nh_ECMAScript_canonicalNumericIndexString(
        Nh_ECMAScript_Any Argument
    );

    void Nh_ECMAScript_toIndex(
        Nh_ECMAScript_Any Value
    );

// misc 

    Nh_ECMAScript_Any Nh_ECMAScript_initAny(
        NH_ECMASCRIPT_TYPE type, void *handle_p
    );

    Nh_ECMAScript_Any Nh_ECMAScript_wrapUndefined(
    );

    Nh_ECMAScript_Any Nh_ECMAScript_wrapNull(
    );

    Nh_ECMAScript_Any Nh_ECMAScript_wrapBoolean(
        NH_ECMASCRIPT_BOOLEAN boolean
    );

    Nh_ECMAScript_Any Nh_ECMAScript_wrapString(
        Nh_ECMAScript_String *String_p
    );

    Nh_ECMAScript_Any Nh_ECMAScript_wrapObject(
        Nh_ECMAScript_Object *Object_p
    );

    Nh_ECMAScript_String *Nh_ECMAScript_allocateString(
        size_t chunkSize
    );
    
    NH_ECMASCRIPT_RESULT Nh_ECMAScript_appendFormatToString(
        Nh_ECMAScript_String *String_p, NH_BYTE *bytes_p, size_t len
    );

    Nh_ECMAScript_Any Nh_ECMAScript_wrapNumber(
        NH_ECMASCRIPT_NUMBER *number_p;
    );

    NH_ECMASCRIPT_NUMBER *Nh_ECMAScript_allocateNumber(
        NH_ECMASCRIPT_NUMBER number
    );

/** @} */

#endif

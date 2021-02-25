#ifndef NH_ECMASCRIPT_COMPLETION_H
#define NH_ECMASCRIPT_COMPLETION_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "Type.h"

#endif

/** @addtogroup NhECMAScriptEnums
 *  @{
 */

    typedef enum NH_ECMASCRIPT_COMPLETION {
        NH_ECMASCRIPT_COMPLETION_NORMAL,
        NH_ECMASCRIPT_COMPLETION_BREAK,
        NH_ECMASCRIPT_COMPLETION_CONTINUE,
        NH_ECMASCRIPT_COMPLETION_RETURN,
        NH_ECMASCRIPT_COMPLETION_THROW,
    } NH_ECMASCRIPT_COMPLETION;

/** @} */

/** @addtogroup NhECMAScriptStructs
 *  @{
 */

    typedef struct Nh_ECMAScript_CompletionValue {
        NH_BOOL empty;
        Nh_ECMAScript_Any Payload;
    } Nh_ECMAScript_CompletionValue;

    typedef struct Nh_ECMAScript_CompletionTarget {
        NH_BOOL empty;
        Nh_UTF8String String;
    } Nh_ECMAScript_CompletionTarget;

    // https://tc39.es/ecma262/#sec-completion-record-specification-type
    typedef struct Nh_ECMAScript_Completion {
        NH_ECMASCRIPT_COMPLETION type;
        Nh_ECMAScript_CompletionValue Value;
        Nh_ECMAScript_CompletionTarget Target;
    } Nh_ECMAScript_Completion;

/** @} */

/** @addtogroup NhECMAScriptFunctions
 *  @{
 */

    Nh_ECMAScript_Completion Nh_ECMAScript_await(
    );

    Nh_ECMAScript_Completion Nh_ECMAScript_throwError(
        Nh_ECMAScript_Any Argument 
    );

    Nh_ECMAScript_Completion Nh_ECMAScript_throwTypeError(
    );

    Nh_ECMAScript_Completion Nh_ECMAScript_throwReferenceError(
    );

    Nh_ECMAScript_Completion Nh_ECMAScript_normalCompletion(
        Nh_ECMAScript_Any Argument
    );

    Nh_ECMAScript_Completion Nh_ECMAScript_normalEmptyCompletion(
    );

/** @} */

#endif

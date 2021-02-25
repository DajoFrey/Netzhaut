#ifndef NH_ECMASCRIPT_COMPLETION_OR_REFERENCE_H
#define NH_ECMASCRIPT_COMPLETION_OR_REFERENCE_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "Completion.h"
#include "Reference.h"

#endif

/** @addtogroup NhECMAScriptStructs
 *  @{
 */

    typedef struct Nh_ECMAScript_CompletionOrReference {
        Nh_ECMAScript_Completion Completion;
        Nh_ECMAScript_Reference Reference;
    } Nh_ECMAScript_CompletionOrReference;

/** @} */

/** @addtogroup NhECMAScriptFunctions
 *  @{
 */

    Nh_ECMAScript_CompletionOrReference Nh_ECMAScript_normalEmptyCompletionOrReference(
    );

    Nh_ECMAScript_CompletionOrReference Nh_ECMAScript_wrapCompletion(
        Nh_ECMAScript_Completion Completion
    );

    Nh_ECMAScript_CompletionOrReference Nh_ECMAScript_wrapReference(
        Nh_ECMAScript_Reference Reference
    );

/** @} */

#endif

// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "CompletionOrReference.h"

#include "../Common/Macro.h"
#include NH_ECMASCRIPT_DEFAULT_CHECK
#include NH_ECMASCRIPT_FLOW

// COMPLETION ======================================================================================

Nh_ECMAScript_CompletionOrReference Nh_ECMAScript_normalEmptyCompletionOrReference()
{
NH_ECMASCRIPT_BEGIN()

    Nh_ECMAScript_CompletionOrReference CompletionOrReference;
    CompletionOrReference.Completion = Nh_ECMAScript_normalEmptyCompletion();
    CompletionOrReference.Reference = Nh_ECMAScript_initReference(); 

NH_ECMASCRIPT_END(CompletionOrReference)
}

Nh_ECMAScript_CompletionOrReference Nh_ECMAScript_wrapCompletion(
    Nh_ECMAScript_Completion Completion)
{
NH_ECMASCRIPT_BEGIN()

    Nh_ECMAScript_CompletionOrReference CompletionOrReference;
    CompletionOrReference.Completion = Completion;
    CompletionOrReference.Reference = Nh_ECMAScript_initReference(); 

NH_ECMASCRIPT_END(CompletionOrReference)
}

Nh_ECMAScript_CompletionOrReference Nh_ECMAScript_wrapReference(
    Nh_ECMAScript_Reference Reference)
{
NH_ECMASCRIPT_BEGIN()

    Nh_ECMAScript_CompletionOrReference CompletionOrReference;
    CompletionOrReference.Completion = Nh_ECMAScript_normalEmptyCompletion();
    CompletionOrReference.Reference = Reference; 

NH_ECMASCRIPT_END(CompletionOrReference)
}


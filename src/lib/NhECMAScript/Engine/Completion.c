// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// COMPLETION ======================================================================================

#include "Completion.h"

#include "../Common/Macro.h"
#include NH_ECMASCRIPT_DEFAULT_CHECK
#include NH_ECMASCRIPT_FLOW

// COMPLETION ======================================================================================

Nh_ECMAScript_Completion Nh_ECMAScript_throwTypeError()
{
NH_ECMASCRIPT_BEGIN()

    Nh_ECMAScript_Completion Completion;
    Completion.type = NH_ECMASCRIPT_COMPLETION_THROW;
    Completion.Value.empty   = NH_TRUE;
    Completion.Value.Payload = Nh_ECMAScript_wrapUndefined();
    Completion.Target.empty  = NH_TRUE;
    Completion.Target.String = Nh_initString(8);

NH_ECMASCRIPT_END(Completion)
}

Nh_ECMAScript_Completion Nh_ECMAScript_throwReferenceError()
{
NH_ECMASCRIPT_BEGIN()

    Nh_ECMAScript_Completion Completion;
    Completion.type = NH_ECMASCRIPT_COMPLETION_THROW;
    Completion.Value.empty   = NH_TRUE;
    Completion.Value.Payload = Nh_ECMAScript_wrapUndefined();
    Completion.Target.empty  = NH_TRUE;
    Completion.Target.String = Nh_initString(8);

NH_ECMASCRIPT_END(Completion)
}

Nh_ECMAScript_Completion Nh_ECMAScript_normalCompletion(
    Nh_ECMAScript_Any Argument)
{
NH_ECMASCRIPT_BEGIN()

    Nh_ECMAScript_Completion Completion;
    Completion.type = NH_ECMASCRIPT_COMPLETION_NORMAL;
    Completion.Value.empty   = NH_FALSE;
    Completion.Value.Payload = Argument;
    Completion.Target.empty  = NH_TRUE;
    Completion.Target.String = Nh_initString(8);

NH_ECMASCRIPT_END(Completion)
}

Nh_ECMAScript_Completion Nh_ECMAScript_normalEmptyCompletion()
{
NH_ECMASCRIPT_BEGIN()

    Nh_ECMAScript_Completion Completion;
    Completion.type = NH_ECMASCRIPT_COMPLETION_NORMAL;
    Completion.Value.empty   = NH_TRUE;
    Completion.Value.Payload = Nh_ECMAScript_wrapUndefined();
    Completion.Target.empty  = NH_TRUE;
    Completion.Target.String = Nh_initString(8);

NH_ECMASCRIPT_END(Completion)
}


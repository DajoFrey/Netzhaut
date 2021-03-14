// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "Reference.h"
#include "Lexer.h"
#include "Parser.h"
#include "ExecutionContext.h"
#include "Instantiation.h"

#include "../../NhCore/Memory.h"

#include "../Common/Log.h"
#include "../Common/Macro.h"
#include NH_ECMASCRIPT_FLOW
#include NH_ECMASCRIPT_CUSTOM_CHECK

#include <string.h>
#include <stdio.h>

// REFERENCE =======================================================================================

Nh_ECMAScript_Reference Nh_ECMAScript_initReference()
{
NH_ECMASCRIPT_BEGIN()

    Nh_ECMAScript_Reference Reference;
    Reference.Base.unresolvable = NH_TRUE;
    Reference.ReferencedName = Nh_ECMAScript_wrapUndefined();
    Reference.strict = NH_FALSE;
    Reference.ThisValue.empty = NH_TRUE;

NH_ECMASCRIPT_END(Reference)
}

Nh_ECMAScript_Reference Nh_ECMAScript_getIdentifierReference(
    Nh_ECMAScript_Environment *Environment_p, Nh_Encoding_UTF8String *Name_p, NH_ECMASCRIPT_BOOLEAN strict)
{
NH_ECMASCRIPT_BEGIN()

    Nh_ECMAScript_Reference Reference;

    if (Environment_p == NULL) {
        Reference.Base.unresolvable = NH_TRUE;
        Reference.ReferencedName = Nh_ECMAScript_wrapString(Name_p);
        Reference.strict = strict;
        Reference.ThisValue.empty = NH_TRUE;
        NH_ECMASCRIPT_END(Reference)
    }

    NH_BOOL exists = Nh_ECMAScript_hasBinding(Environment_p, Name_p);

    if (exists) {
        Reference.Base.unresolvable = NH_FALSE;
        Reference.Base.Environment_p = Environment_p;
        Reference.ReferencedName = Nh_ECMAScript_wrapString(Name_p);
        Reference.strict = strict;
        Reference.ThisValue.empty = NH_TRUE;
        NH_ECMASCRIPT_END(Reference)
    }

    Nh_ECMAScript_Environment *OuterEnvironment_p = Environment_p->Outer_p;

NH_ECMASCRIPT_END(Nh_ECMAScript_getIdentifierReference(OuterEnvironment_p, Name_p, strict))
}

NH_ECMASCRIPT_BOOLEAN Nh_ECMAScript_isPropertyReference(
    Nh_ECMAScript_Reference Reference)
{
NH_ECMASCRIPT_BEGIN()

    if (Reference.Base.unresolvable) {NH_ECMASCRIPT_END(NH_FALSE)}
    if (Reference.Base.Environment_p != NULL) {NH_ECMASCRIPT_END(NH_FALSE)}

NH_ECMASCRIPT_END(NH_TRUE)
}

Nh_ECMAScript_Completion Nh_ECMAScript_getValue(
    Nh_ECMAScript_Completion Completion, Nh_ECMAScript_Reference Reference)
{
NH_ECMASCRIPT_BEGIN()

    if (!Completion.Value.empty) {NH_ECMASCRIPT_END(Completion)}
    if (Reference.Base.unresolvable) {NH_ECMASCRIPT_END(Nh_ECMAScript_throwReferenceError())}

    if (Nh_ECMAScript_isPropertyReference(Reference)) {

    }
    else {

    }

NH_ECMASCRIPT_END(Nh_ECMAScript_normalEmptyCompletion())
}

Nh_ECMAScript_Completion Nh_ECMAScript_putValue(
    Nh_ECMAScript_Reference Reference, Nh_ECMAScript_Any Value)
{
NH_ECMASCRIPT_BEGIN()

    if (Reference.Base.unresolvable) {
        if (Reference.strict) {NH_ECMASCRIPT_END(Nh_ECMAScript_throwReferenceError())}
        Nh_ECMAScript_Object *GlobalObject_p = Nh_ECMAScript_getGlobalObject();
        NH_ECMASCRIPT_END(Nh_ECMAScript_abstractSet(GlobalObject_p, Reference.ReferencedName, Value, NH_FALSE))
    }
    else if (Nh_ECMAScript_isPropertyReference(Reference)) {
    }

NH_ECMASCRIPT_END(Nh_ECMAScript_normalEmptyCompletion())
}


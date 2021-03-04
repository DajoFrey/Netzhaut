// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE ========================================================================================

#include "ExecutionContext.h"
#include "Agent.h"

#include "../Common/Macro.h"
#include NH_ECMASCRIPT_CUSTOM_CHECK
#include NH_ECMASCRIPT_FLOW

#include "../../NhCore/Memory.h"

#include <string.h>

// EXECUTION CONTEXT ===============================================================================

Nh_ECMAScript_ExecutionContext *Nh_ECMAScript_allocateExecutionContext()
{
NH_ECMASCRIPT_BEGIN()

    Nh_ECMAScript_ExecutionContext *Context_p = Nh_allocate(sizeof(Nh_ECMAScript_ExecutionContext)); 
    NH_ECMASCRIPT_CHECK_MEM(NULL, Context_p)

    Context_p->Function_p = NULL;
    Context_p->Realm_p = NULL;

NH_ECMASCRIPT_END(Context_p)
}

Nh_ECMAScript_ExecutionContext *Nh_ECMAScript_getRunningExecutionContext()
{
NH_ECMASCRIPT_BEGIN()
NH_ECMASCRIPT_END(Nh_peekStack(&Nh_ECMAScript_getCurrentAgent()->ExecutionContextStack))
}

Nh_ECMAScript_ScriptOrModule *Nh_ECMAScript_getActiveScriptOrModule()
{
NH_ECMASCRIPT_BEGIN()

    Nh_ECMAScript_Agent *Agent_p = Nh_ECMAScript_getCurrentAgent();

    if (Agent_p->ExecutionContextStack.size == 0) {NH_ECMASCRIPT_END(NULL)}

    for (int i = Agent_p->ExecutionContextStack.size - 1; i >= 0; ++i) {
        Nh_ECMAScript_ExecutionContext *ExecutionContext_p = Agent_p->ExecutionContextStack.pp[i];
        if (ExecutionContext_p->ScriptOrModule.handle_p != NULL) {
            NH_ECMASCRIPT_END(&ExecutionContext_p->ScriptOrModule)
        }
    }

NH_ECMASCRIPT_END(NULL)
}

Nh_ECMAScript_Realm *Nh_ECMAScript_getCurrentRealm()
{
NH_ECMASCRIPT_BEGIN()
NH_ECMASCRIPT_END(Nh_ECMAScript_getRunningExecutionContext()->Realm_p)
}

Nh_ECMAScript_Object *Nh_ECMAScript_getGlobalObject()
{
NH_ECMASCRIPT_BEGIN()
NH_ECMASCRIPT_END(Nh_ECMAScript_getCurrentRealm()->GlobalObject_p)
}

Nh_ECMAScript_Reference Nh_ECMAScript_resolveBinding(
    Nh_UTF8String *Name_p, Nh_ECMAScript_Environment *Environment_p)
{
NH_ECMASCRIPT_BEGIN()

    if (Environment_p == NULL) {
        Environment_p = Nh_ECMAScript_getRunningExecutionContext()->LexicalEnvironment_p;
    }

    // TODO If the code matching the syntactic production that is being evaluated is contained in strict mode code, let strict be true; else let strict be false.
    NH_ECMASCRIPT_BOOLEAN strict = NH_FALSE;

NH_ECMASCRIPT_END(Nh_ECMAScript_getIdentifierReference(Environment_p, Name_p, strict))
}


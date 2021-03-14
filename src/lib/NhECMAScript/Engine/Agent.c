// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE ========================================================================================

#include "Agent.h"
#include "Script.h"
#include "ExecutionContext.h"

#include "../Common/IndexMap.h"
#include "../Common/Log.h"
#include "../Common/Macro.h"
#include NH_ECMASCRIPT_CUSTOM_CHECK
#include NH_ECMASCRIPT_FLOW

#include "../../NhCore/System.h"
#include "../../NhCore/Memory.h"
#include "../../NhCore/Thread.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// AGENT ===========================================================================================

void *Nh_ECMAScript_initAgent(
    void *args_p)
{
NH_ECMASCRIPT_BEGIN()

//    Nh_ECMAScript_AgentArgs *Args_p = args_p;
//
//    Nh_ECMAScript_Agent *Agent_p = Nh_allocate(sizeof(Nh_ECMAScript_Agent));
//    NH_ECMASCRIPT_CHECK_MEM(NULL, Agent_p)
//
//    Agent_p->ExecutionContextStack = Nh_initStack(8);
//    Agent_p->littleEndian = NH_SYSTEM.littleEndian;
//    Agent_p->KeptAlive = Nh_initList(8);
//
//    NH_ECMASCRIPT_CHECK(NULL, Nh_ECMAScript_initializeHostDefinedRealm(&Agent_p->ExecutionContextStack))
//
//    Nh_ECMAScript_ExecutionContext *ExecutionContext_p = Nh_peekStack(&Agent_p->ExecutionContextStack);
//    ExecutionContext_p->Realm_p->HostDefined.temporary_p = Nh_allocateBytes(Args_p->bytes_p);

NH_ECMASCRIPT_END(Agent_p)
}

NH_SIGNAL Nh_ECMAScript_runAgent(
    void *args_p)
{
NH_ECMASCRIPT_BEGIN()

//    NH_ECMASCRIPT_CHECK_NULL(NH_SIGNAL_ERROR, args_p)
//    Nh_ECMAScript_Agent *Agent_p = args_p;
//
//    Nh_ECMAScript_ExecutionContext *ExecutionContext_p = Nh_peekStack(&Agent_p->ExecutionContextStack);
//
//    Nh_ECMAScript_Script *Script_p = Nh_ECMAScript_parseScript(ExecutionContext_p->Realm_p->HostDefined.temporary_p, ExecutionContext_p->Realm_p, NH_UNICODE_ENCODING_UTF8);
//    NH_ECMASCRIPT_CHECK_MEM(NH_SIGNAL_DONE, Script_p)
//
//    Nh_ECMAScript_evaluateScript(Script_p);

NH_ECMASCRIPT_END(NH_SIGNAL_DONE)
}

Nh_ECMAScript_Agent *Nh_ECMAScript_getCurrentAgent()
{
NH_ECMASCRIPT_BEGIN()
NH_ECMASCRIPT_END(Nh_getThread()->CurrentWorkload_p->args_p)
}


#ifndef NH_ECMASCRIPT_AGENT_H
#define NH_ECMASCRIPT_AGENT_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "../../NhCore/Stack.h"
#include "../../NhCore/Unicode.h"
#include "../../NhCore/Array.h"

#endif

/** @addtogroup NhECMAScriptStructs
 *  @{
 */

    typedef struct Nh_ECMAScript_AgentArgs {
        NH_UNICODE_ENCODING encoding;
        NH_BYTE *bytes_p;
    } Nh_ECMAScript_AgentArgs;

    // https://tc39.es/ecma262/#sec-agents
    typedef struct Nh_ECMAScript_Agent {
        Nh_Stack ExecutionContextStack;
        NH_BOOL littleEndian;
        NH_BOOL canBlock;
        int signifier;
        NH_BOOL isLockFree1;
        NH_BOOL isLockFree2;
        NH_BOOL isLockFree8;
//        Nh_ECMAScript_CandidateExecution CandidateExecution;
        Nh_List KeptAlive;
    } Nh_ECMAScript_Agent;

/** @} */

/** @addtogroup NhECMAScriptTypedefs
 *  @{
 */

    typedef void *(*Nh_ECMAScript_initAgent_f)(
        void *args_p
    );
    typedef NH_SIGNAL (*Nh_ECMAScript_runAgent_f)(
        void *args_p
    );

/** @} */

/** @addtogroup NhECMAScriptFunctions
 *  @{
 */

    void *Nh_ECMAScript_initAgent(
        void *args_p
    );
    
    NH_SIGNAL Nh_ECMAScript_runAgent(
        void *args_p
    );

    Nh_ECMAScript_Agent *Nh_ECMAScript_getCurrentAgent(
    );

/** @} */

#endif

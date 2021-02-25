#ifndef NH_ECMASCRIPT_EXECUTION_CONTEXT_H
#define NH_ECMASCRIPT_EXECUTION_CONTEXT_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "Script.h"
#include "Realm.h"
#include "Environment.h"
#include "Reference.h"

#endif

/** @addtogroup NhECMAScriptStructs
 *  @{
 */

//    typedef struct Nh_ECMAScript_CodeEvaluation {
//        NH_BOOL suspend;
//    } Nh_ECMAScript_CodeEvaluation;

    typedef struct Nh_ECMAScript_ScriptOrModule {
        NH_BOOL isScript;
        void *handle_p;
    } Nh_ECMAScript_ScriptOrModule;

    // https://www.262.ecma-international.org/11.0/index.html#sec-execution-contexts
    typedef struct Nh_ECMAScript_ExecutionContext {
//        Nh_ECMAScript_CodeEvalution CodeEvalution;
        Nh_ECMAScript_Object *Function_p;
        Nh_ECMAScript_Realm *Realm_p;
        Nh_ECMAScript_ScriptOrModule ScriptOrModule;
        Nh_ECMAScript_Environment *LexicalEnvironment_p;
        Nh_ECMAScript_Environment *VariableEnvironment_p;
    } Nh_ECMAScript_ExecutionContext;

/** @} */

/** @addtogroup NhECMAScriptFunctions
 *  @{
 */

    Nh_ECMAScript_ExecutionContext *Nh_ECMAScript_allocateExecutionContext(
    );

    Nh_ECMAScript_ExecutionContext *Nh_ECMAScript_getRunningExecutionContext(
    );

    Nh_ECMAScript_Realm *Nh_ECMAScript_getCurrentRealm(
    );

    Nh_ECMAScript_Object *Nh_ECMAScript_getGlobalObject(
    );

    Nh_ECMAScript_ScriptOrModule *Nh_ECMAScript_getActiveScriptOrModule(
    );

    Nh_ECMAScript_Reference Nh_ECMAScript_resolveBinding(
        Nh_UTF8String *Name_p, Nh_ECMAScript_Environment *Environment_p
    );

/** @} */

#endif

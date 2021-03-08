// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "Script.h"
#include "Lexer.h"
#include "Parser.h"
#include "ExecutionContext.h"
#include "Instantiation.h"
#include "Evaluation.h"
#include "Agent.h"

#include "../../NhCore/Memory.h"

#include "../Common/Log.h"
#include "../Common/Macro.h"
#include NH_ECMASCRIPT_FLOW
#include NH_ECMASCRIPT_CUSTOM_CHECK

#include <string.h>
#include <stdio.h>

// PARSE ===========================================================================================

// https://tc39.es/ecma262/#sec-parse-script
Nh_ECMAScript_Script *Nh_ECMAScript_parseScript(
    NH_BYTE *sourceText_p, Nh_ECMAScript_Realm *Realm_p, NH_UNICODE_ENCODING encoding)
{
NH_ECMASCRIPT_BEGIN()

    Nh_ECMAScript_Script *Script_p = Nh_allocate(sizeof(Nh_ECMAScript_Script));
    NH_ECMASCRIPT_CHECK_MEM(NULL, Script_p)

    Script_p->Realm_p = Realm_p;
    Script_p->Environment_p = NULL;

    Nh_UnicodeString UnicodeCodepoints = Nh_initUnicodeString(128);

    switch (encoding) 
    {
        case NH_UNICODE_ENCODING_UTF8 :
            UnicodeCodepoints = Nh_decodeUTF8Text(sourceText_p);
            break;
    }

    if (UnicodeCodepoints.length <= 0) {
        Nh_free(Script_p);
        NH_ECMASCRIPT_END(NULL)
    }

    Nh_ECMAScript_logDecoder(Script_p, &UnicodeCodepoints);

    Nh_Array DirtyInputElements = Nh_ECMAScript_getInputElements(UnicodeCodepoints);
    Nh_ECMAScript_logLexer(Script_p, &DirtyInputElements, NH_TRUE);

    Nh_Array CleanInputElements = Nh_ECMAScript_discardRedundantInputElements(DirtyInputElements);
    Nh_ECMAScript_logLexer(Script_p, &CleanInputElements, NH_FALSE);

    Nh_ECMAScript_ParseResult Result = Nh_ECMAScript_parseText(CleanInputElements, NH_ECMASCRIPT_PARSE_NODE_SCRIPT);
    if (Result.Node_p == NULL || Result.SyntaxErrors.size > 0) {
        Nh_free(Script_p);
        NH_ECMASCRIPT_END(NULL);
    }

    NH_BOOL branch_p[1024] = {0};
    Nh_ECMAScript_logParseTree(Script_p, Result.Node_p, NULL, 0, branch_p);
    Script_p->ECMAScriptCode_p = Result.Node_p;

NH_ECMASCRIPT_END(Script_p)
}

// EVALUATE ========================================================================================

// https://tc39.es/ecma262/#sec-runtime-semantics-scriptevaluation
Nh_ECMAScript_Completion Nh_ECMAScript_evaluateScript(
    Nh_ECMAScript_Script *Script_p)
{
NH_ECMASCRIPT_BEGIN()

    Nh_ECMAScript_Environment *GlobalEnvironment_p = Script_p->Realm_p->GlobalEnvironment_p;

    Nh_ECMAScript_ExecutionContext *ScriptContext_p = Nh_ECMAScript_allocateExecutionContext();
//    NH_ECMASCRIPT_CHECK_MEM(NewContext_p)

    ScriptContext_p->Function_p = NULL;
    ScriptContext_p->Realm_p    = Script_p->Realm_p;

    ScriptContext_p->ScriptOrModule.isScript = NH_TRUE;
    ScriptContext_p->ScriptOrModule.handle_p = Script_p;

    ScriptContext_p->VariableEnvironment_p = GlobalEnvironment_p; 
    ScriptContext_p->LexicalEnvironment_p  = GlobalEnvironment_p; 
// TODO Suspend the currently running execution context. 

    Nh_pushStack(&Nh_ECMAScript_getCurrentAgent()->ExecutionContextStack, ScriptContext_p);

    Nh_ECMAScript_ParseNode *ScriptBody_p = Script_p->ECMAScriptCode_p->Children.pp[0];
    Nh_ECMAScript_Completion Result = Nh_ECMAScript_globalDeclarationInstantiation(ScriptBody_p, GlobalEnvironment_p);

    if (Result.type == NH_ECMASCRIPT_COMPLETION_NORMAL) {
        Result = Nh_ECMAScript_evaluateScriptBody(ScriptBody_p);
    }

NH_ECMASCRIPT_END(Result)
}


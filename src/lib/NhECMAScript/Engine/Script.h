#ifndef NH_ECMASCRIPT_SCRIPT_H
#define NH_ECMASCRIPT_SCRIPT_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "Realm.h"
#include "Parser.h"
#include "Environment.h"

#include "../../NhCore/Unicode.h"
#include "../../NhCore/Array.h"

#endif

/** @addtogroup NhECMAScriptStructs
 *  @{
 */

    typedef struct Nh_ECMAScript_ScriptHostDefined {
        int nothingYet;
    } Nh_ECMAScript_ScriptHostDefined;

    // corresponds to https://tc39.es/ecma262/#sec-script-records
    typedef struct Nh_ECMAScript_Script {
        Nh_ECMAScript_Realm *Realm_p;
        Nh_ECMAScript_Environment *Environment_p;
        Nh_ECMAScript_ParseNode *ECMAScriptCode_p;
        Nh_ECMAScript_ScriptHostDefined HostDefined;
    } Nh_ECMAScript_Script;

/** @} */

/** @addtogroup NhECMAScriptFunctions
 *  @{
 */

    // https://tc39.es/ecma262/#sec-parse-script
    Nh_ECMAScript_Script *Nh_ECMAScript_parseScript(
        NH_BYTE *sourceText_p, Nh_ECMAScript_Realm *Realm_p, NH_UNICODE_ENCODING encoding
    );

    // https://www.ecma-international.org/ecma-262/#sec-runtime-semantics-scriptevaluation
    Nh_ECMAScript_Completion Nh_ECMAScript_evaluateScript(
        Nh_ECMAScript_Script *Script_p
    );

/** @} */

#endif

#ifndef NH_ECMASCRIPT_EVALUATION_H
#define NH_ECMASCRIPT_EVALUATION_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "Completion.h"
#include "Parser.h"

#endif

/** @addtogroup NhECMAScriptFunctions
 *  @{
 */

    Nh_ECMAScript_Completion Nh_ECMAScript_evaluateScriptBody(
        Nh_ECMAScript_ParseNode *Node_p
    );

/** @} */

#endif

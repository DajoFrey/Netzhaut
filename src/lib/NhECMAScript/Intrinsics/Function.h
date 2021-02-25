#ifndef NH_ECMASCRIPT_FUNCTION_H
#define NH_ECMASCRIPT_FUNCTION_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "../Engine/Realm.h"

#endif

/** @addtogroup ECMAScriptFunctions Functions
 *  \ingroup ECMAScript 
 *  @{
 */

    NH_ECMASCRIPT_RESULT Nh_ECMAScript_createIntrinsicFunction(
        Nh_ECMAScript_IntrinsicFunction *Function_p, Nh_ECMAScript_Realm *Realm_p
    );
    
    NH_ECMASCRIPT_RESULT Nh_ECMAScript_freeIntrinsicFunction(
        Nh_ECMAScript_IntrinsicFunction *Function_p
    );

/** @} */

#endif

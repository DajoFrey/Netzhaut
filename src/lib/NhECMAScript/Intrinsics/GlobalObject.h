#ifndef NH_ECMASCRIPT_GLOBAL_OBJECT_H
#define NH_ECMASCRIPT_GLOBAL_OBJECT_H

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

    Nh_ECMAScript_Object *Nh_ECMAScript_setDefaultGlobalBindings(
        Nh_ECMAScript_Realm *Realm_p
    );
    
    NH_ECMASCRIPT_RESULT Nh_ECMAScript_freeDefaultGlobalBindings(
        Nh_ECMAScript_Object *Object_p
    );

/** @} */

#endif

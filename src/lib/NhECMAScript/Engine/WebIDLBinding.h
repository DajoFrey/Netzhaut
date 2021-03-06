#ifndef NH_ECMASCRIPT_WEB_BINDING_H
#define NH_ECMASCRIPT_WEB_BINDING_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "Intrinsics.h"

#endif

/** @addtogroup NhECMAScriptStructs
 *  @{
 */

    typedef struct Nh_ECMAScript_TemplateMapEntry {
        void *Site_p;
        Nh_ECMAScript_Object Array;
    } Nh_ECMAScript_TemplateMapEntry;

    // https://tc39.es/ecma262/#sec-code-realms
    typedef struct Nh_ECMAScript_Realm {
        Nh_ECMAScript_Intrinsics Intrinsics;
        Nh_ECMAScript_Object *GlobalObject_p;
        Nh_ECMAScript_Environment *GlobalEnvironment_p;
    } Nh_ECMAScript_Realm;

/** @} */

/** @addtogroup NhECMAScriptFunctions
 *  @{
 */

    Nh_ECMAScript_Realm *Nh_ECMAScript_createRealm(
    );

/** @} */

#endif

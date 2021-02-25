#ifndef NH_ECMASCRIPT_BUILTIN_FUNCTION_OBJECT_H
#define NH_ECMASCRIPT_BUILTIN_FUNCTION_OBJECT_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "ObjectType.h"
#include "Realm.h"

#endif

/** @addtogroup NhECMAScriptFunctions
 *  @{
 */

    Nh_ECMAScript_Object *Nh_ECMAScript_createBuiltinFunctionObject(
        Nh_ECMAScript_Object *FunctionObject_p, Nh_ECMAScript_Any (*call_f)(Nh_ECMAScript_Object *This_p, Nh_List Arguments),
        int *lookup_p, int lookupLength, Nh_ECMAScript_Realm *Realm_p, Nh_ECMAScript_Object *Prototype_p
    );

/** @} */

#endif

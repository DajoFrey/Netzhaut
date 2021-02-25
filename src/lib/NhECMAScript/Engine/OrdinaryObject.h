#ifndef NH_ECMASCRIPT_ORDINARY_OBJECT_H
#define NH_ECMASCRIPT_ORDINARY_OBJECT_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "Completion.h"
#include "InternalSlots.h"
#include "Properties.h"
#include "ObjectType.h"

#include "../../NhCore/List.h"

#endif

/** @addtogroup NhECMAScriptVars
 *  @{
 */

    extern const int NH_ECMASCRIPT_ORDINARY_OBJECT_LOOKUP_P[];
    extern Nh_ECMAScript_InternalMethods NH_ECMASCRIPT_ORDINARY_OBJECT_INTERNAL_METHODS;

/** @} */

/** @addtogroup NhECMAScriptFunctions Functions
 *  @{
 */

    Nh_ECMAScript_Object *Nh_ECMAScript_ordinaryGetPrototypeOf(
        Nh_ECMAScript_Object *This_p
    );
    
    NH_ECMASCRIPT_BOOLEAN Nh_ECMAScript_ordinarySetPrototypeOf(
        Nh_ECMAScript_Object *This_p
    );
    
    NH_ECMASCRIPT_BOOLEAN Nh_ECMAScript_ordinaryIsExtensible(
        Nh_ECMAScript_Object *This_p
    );
    
    NH_ECMASCRIPT_BOOLEAN Nh_ECMAScript_ordinaryPreventExtensions(
        Nh_ECMAScript_Object *This_p
    );
    
    Nh_ECMAScript_PropertyDescriptor Nh_ECMAScript_ordinaryGetOwnProperty(
        Nh_ECMAScript_Object *This_p, Nh_ECMAScript_Any PropertyKey
    );
    
    NH_ECMASCRIPT_BOOLEAN Nh_ECMAScript_ordinaryDefineOwnProperty(
        Nh_ECMAScript_Object *This_p, Nh_ECMAScript_Any PropertyKey, Nh_ECMAScript_PropertyDescriptor Desc
    );
    
    NH_ECMASCRIPT_BOOLEAN Nh_ECMAScript_ordinaryHasProperty(
        Nh_ECMAScript_Object *This_p, Nh_ECMAScript_Any PropertyKey
    );
    
    Nh_ECMAScript_Any Nh_ECMAScript_ordinaryGet(
        Nh_ECMAScript_Object *This_p, Nh_ECMAScript_Any PropertyKey, Nh_ECMAScript_Any Receiver
    );
    
    NH_ECMASCRIPT_BOOLEAN Nh_ECMAScript_ordinarySet(
        Nh_ECMAScript_Object *This_p, Nh_ECMAScript_Any PropertyKey, Nh_ECMAScript_Any V, Nh_ECMAScript_Any Receiver
    );
    
    NH_ECMASCRIPT_BOOLEAN Nh_ECMAScript_ordinaryDelete(
        Nh_ECMAScript_Object *This_p, Nh_ECMAScript_Any PropertyKey
    );
    
    Nh_List Nh_ECMAScript_ordinaryOwnPropertyKeys(
        Nh_ECMAScript_Object *This_p
    );

    // https://tc39.es/ecma262/#sec-ordinaryobjectcreate
    Nh_ECMAScript_Object *Nh_ECMAScript_ordinaryObjectCreate(
        Nh_ECMAScript_Object *Proto_p, const int *lookup_p, int lookupLength
    );

/** @} */

#endif

#ifndef NH_ECMASCRIPT_INTERNAL_SLOT_H
#define NH_ECMASCRIPT_INTERNAL_SLOT_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "../Common/Result.h"

#endif

/** @addtogroup NhECMAScriptEnums
 *  @{
 */

    typedef enum NH_ECMASCRIPT_INTERNAL_SLOT {
        NH_ECMASCRIPT_INTERNAL_SLOT_EXTENSIBLE,
        NH_ECMASCRIPT_INTERNAL_SLOT_PROTOTYPE,
        NH_ECMASCRIPT_INTERNAL_SLOT_REALM,
        NH_ECMASCRIPT_INTERNAL_SLOT_INITIAL_NAME,
        NH_ECMASCRIPT_INTERNAL_SLOT_BUILTIN,
        NH_ECMASCRIPT_INTERNAL_SLOT_ENVIRONMENT,
        NH_ECMASCRIPT_INTERNAL_SLOT_FORMAL_PARAMETERS,
        NH_ECMASCRIPT_INTERNAL_SLOT_ECMASCRIPT_CODE,
        NH_ECMASCRIPT_INTERNAL_SLOT_CONSTRUCTOR_KIND,
        NH_ECMASCRIPT_INTERNAL_SLOT_SCRIPT_OR_MODULE,
        NH_ECMASCRIPT_INTERNAL_SLOT_THIS_MODE,
        NH_ECMASCRIPT_INTERNAL_SLOT_STRICT,
        NH_ECMASCRIPT_INTERNAL_SLOT_HOME_OBJECT,
        NH_ECMASCRIPT_INTERNAL_SLOT_SOURCE_TEXT,
        NH_ECMASCRIPT_INTERNAL_SLOT_IS_CLASS_CONSTRUCTOR,
    } NH_ECMASCRIPT_INTERNAL_SLOT;

/** @} */

/** @addtogroup NhECMAScriptStructs
 *  @{
 */

    typedef struct Nh_ECMAScript_InternalSlots {
        int maxLookup;
        const int *lookup_p;
        void **values_pp;
    } Nh_ECMAScript_InternalSlots;

/** @} */

/** @addtogroup NhECMAScriptFunctions
 *  @{
 */ 

    Nh_ECMAScript_InternalSlots Nh_ECMAScript_initInternalSlots(
        const int *lookup_p, int length
    );

    void *Nh_ECMAScript_getInternalSlot(
        Nh_ECMAScript_InternalSlots *InternalSlots_p, NH_ECMASCRIPT_INTERNAL_SLOT slot
    );

    NH_ECMASCRIPT_RESULT Nh_ECMAScript_setInternalSlot(
        Nh_ECMAScript_InternalSlots *InternalSlots_p, NH_ECMASCRIPT_INTERNAL_SLOT slot, void *value_p
    );

/** @} */

#endif

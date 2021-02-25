// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE ========================================================================================

#include "InternalSlots.h"

#include "../Common/Macro.h"
#include NH_ECMASCRIPT_DEFAULT_CHECK
#include NH_ECMASCRIPT_FLOW

#include "../../NhCore/Memory.h"

#include <string.h>

// LOOKUP ==========================================================================================

static int ordinaryLookup_p[] = {
    0, // [[Extensible]]
    1, // [[Prototype]]
};

// DATA ============================================================================================

Nh_ECMAScript_InternalSlots Nh_ECMAScript_initInternalSlots(
    const int *lookup_p, int length)
{
NH_ECMASCRIPT_BEGIN()

    Nh_ECMAScript_InternalSlots InternalSlots;
    InternalSlots.lookup_p = NULL;
    InternalSlots.values_pp = NULL;

    if (lookup_p != NULL && length > 0) 
    {
        int valueCount = 0;

        for (int i = 0; i < length; ++i) {
            if (lookup_p[i] > -1) {valueCount++;}
        }

        if (valueCount > 0)
        {
            InternalSlots.maxLookup = length;
            InternalSlots.lookup_p  = lookup_p;
            InternalSlots.values_pp = Nh_allocate(sizeof(void*) * valueCount);
        }
    }
    else {
        InternalSlots.maxLookup = sizeof(ordinaryLookup_p) / sizeof(ordinaryLookup_p[0]);
        InternalSlots.lookup_p  = ordinaryLookup_p;
        InternalSlots.values_pp = Nh_allocate(sizeof(void*) * (sizeof(ordinaryLookup_p) / sizeof(ordinaryLookup_p[0])));
    }

NH_ECMASCRIPT_END(InternalSlots)
}

// GET =============================================================================================

void *Nh_ECMAScript_getInternalSlot(
    Nh_ECMAScript_InternalSlots *InternalSlots_p, NH_ECMASCRIPT_INTERNAL_SLOT slot)
{
NH_ECMASCRIPT_BEGIN()

    if (slot >= InternalSlots_p->maxLookup || slot < 0 || InternalSlots_p->lookup_p[slot] == -1) {
        NH_ECMASCRIPT_END(NULL)
    }

NH_ECMASCRIPT_END(InternalSlots_p->values_pp[InternalSlots_p->lookup_p[slot]])
}

// SET =============================================================================================

NH_ECMASCRIPT_RESULT Nh_ECMAScript_setInternalSlot(
    Nh_ECMAScript_InternalSlots *InternalSlots_p, NH_ECMASCRIPT_INTERNAL_SLOT slot, void *value_p)
{
NH_ECMASCRIPT_BEGIN()

    if (slot >= InternalSlots_p->maxLookup || slot < 0 || InternalSlots_p->lookup_p[slot] == -1) {
        NH_ECMASCRIPT_END(NH_ECMASCRIPT_ERROR_BAD_STATE)
    }

    InternalSlots_p->values_pp[InternalSlots_p->lookup_p[slot]] = value_p;

NH_ECMASCRIPT_END(NH_ECMASCRIPT_SUCCESS)
}


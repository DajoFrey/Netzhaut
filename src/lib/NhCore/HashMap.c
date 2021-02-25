// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE ========================================================================================

#include "HashMap.h"
#include "Memory.h"
#include "MediaType.h"

#include "Common/Macro.h"
#include NH_FLOW
#include NH_DEFAULT_CHECK

#include <stdio.h>
#include <string.h>

// CREATE ==========================================================================================

Nh_HashMap Nh_createHashMap()
{
NH_BEGIN()
NH_DIAGNOSTIC_END(hashmap_new())
}

// ADD =============================================================================================

NH_RESULT Nh_addToHashMap(
    Nh_HashMap *Map_p, NH_BYTE *key_p, void *value_p)
{
NH_BEGIN()

    if (hashmap_put(*Map_p, key_p, value_p) != MAP_OK) {
        NH_DIAGNOSTIC_END(NH_ERROR_BAD_STATE)
    }

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

// FREE ============================================================================================

void Nh_freeHashMap(
    Nh_HashMap Map)
{
NH_BEGIN()

    hashmap_free(Map);

NH_SILENT_END()
}

// GET =============================================================================================

void *Nh_getFromHashMap(
    Nh_HashMap *Map_p, NH_BYTE *key_p)
{
NH_BEGIN()

    void *handle_p = NULL;
    if (hashmap_get(*Map_p, key_p, (void**)(&handle_p)) != MAP_OK) {
        NH_END(NULL)
    }

NH_END(handle_p)
}


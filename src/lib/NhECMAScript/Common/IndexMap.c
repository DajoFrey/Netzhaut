// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE ========================================================================================

#include "IndexMap.h"

#include "MACRO/FLOW.h"
#include "MACRO/DEFAULT_CHECK.h"

#include "../Engine/Parser.h"
#include "../../NhCore/Memory.h"

#include <string.h>

// DECLARE =========================================================================================

typedef enum NH_ECMASCRIPT_INDEXMAP_E {
    NH_ECMASCRIPT_INDEXMAP_PARSE_NODE_NAMES = 0,
    NH_ECMASCRIPT_INDEXMAP_RESERVED_WORDS = 1,
    NH_ECMASCRIPT_INDEXMAP_COUNT,
} NH_ECMASCRIPT_INDEXMAP_E;

// DATA ============================================================================================

Nh_ECMAScript_IndexMap NH_ECMASCRIPT_INDEXMAP;
static unsigned int *indices_pp[NH_ECMASCRIPT_INDEXMAP_COUNT] = {NULL};
static NH_BOOL allocated = NH_FALSE;

// CREATE/DESTROY ==================================================================================

static NH_ECMASCRIPT_RESULT Nh_ECMAScript_getNames(
    NH_ECMASCRIPT_INDEXMAP_E type, NH_BYTE ***array_ppp, int *count_p)
{
NH_ECMASCRIPT_BEGIN()

    switch (type)
    {
        case NH_ECMASCRIPT_INDEXMAP_PARSE_NODE_NAMES :
        {
            *array_ppp = (NH_BYTE**) NH_ECMASCRIPT_PARSE_NODE_NAMES_PP; 
            *count_p = NH_ECMASCRIPT_PARSE_NODE_NAMES_PP_COUNT; 
            break;
        }
        case NH_ECMASCRIPT_INDEXMAP_RESERVED_WORDS :
        {
            *array_ppp = (NH_BYTE**) NH_ECMASCRIPT_RESERVED_WORDS_PP; 
            *count_p = NH_ECMASCRIPT_RESERVED_WORDS_PP_COUNT; 
            break;
        }
        default : NH_ECMASCRIPT_DIAGNOSTIC_END(NH_ECMASCRIPT_ERROR_BAD_STATE)
    }

    if (*array_ppp == NULL) {NH_ECMASCRIPT_DIAGNOSTIC_END(NH_ECMASCRIPT_ERROR_BAD_STATE)}

NH_ECMASCRIPT_DIAGNOSTIC_END(NH_ECMASCRIPT_SUCCESS)
}

static NH_ECMASCRIPT_RESULT Nh_ECMAScript_createSingleIndexMap(
    NH_ECMASCRIPT_INDEXMAP_E type, Nh_HashMap *map_p)
{
NH_ECMASCRIPT_BEGIN()

    int count = 0; NH_BYTE **names_pp = NULL;
    NH_ECMASCRIPT_CHECK(Nh_ECMAScript_getNames(type, &names_pp, &count))

    *map_p = Nh_createHashMap();

    for (int i = 0; i < count; ++i) {
        Nh_addToHashMap(map_p, names_pp[i], &indices_pp[type][i]);
    }

NH_ECMASCRIPT_DIAGNOSTIC_END(NH_ECMASCRIPT_SUCCESS)
}

NH_ECMASCRIPT_RESULT Nh_ECMAScript_createIndexMap()
{
NH_ECMASCRIPT_BEGIN()

    if (allocated) {NH_ECMASCRIPT_DIAGNOSTIC_END(NH_ECMASCRIPT_SUCCESS)}

    for (int type = 0; type < NH_ECMASCRIPT_INDEXMAP_COUNT; ++type)  
    {
        int count = 0;
        const NH_BYTE **names_pp = NULL;
        if (Nh_ECMAScript_getNames(type, (NH_BYTE***)&names_pp, &count) != NH_ECMASCRIPT_SUCCESS) {
            NH_ECMASCRIPT_DIAGNOSTIC_END(NH_ECMASCRIPT_ERROR_BAD_STATE)
        }

        indices_pp[type] = Nh_allocate(sizeof(unsigned int) * count);
        NH_ECMASCRIPT_CHECK_MEM(indices_pp[type])
        
        for (int i = 0; i < count; ++i) {
            indices_pp[type][i] = i;
        }
    }    

    NH_ECMASCRIPT_CHECK(Nh_ECMAScript_createSingleIndexMap(NH_ECMASCRIPT_INDEXMAP_PARSE_NODE_NAMES, &NH_ECMASCRIPT_INDEXMAP.ParseNodeNames))
    NH_ECMASCRIPT_CHECK(Nh_ECMAScript_createSingleIndexMap(NH_ECMASCRIPT_INDEXMAP_RESERVED_WORDS, &NH_ECMASCRIPT_INDEXMAP.ReservedWords))

    allocated = NH_TRUE;

NH_ECMASCRIPT_DIAGNOSTIC_END(NH_ECMASCRIPT_SUCCESS)
}

void Nh_ECMAScript_freeIndexMap()
{
NH_ECMASCRIPT_BEGIN()

    if (allocated) {NH_ECMASCRIPT_SILENT_END()}

    for (int type = 0; type < NH_ECMASCRIPT_INDEXMAP_COUNT; ++type)  
    {
         Nh_free(indices_pp[type]);
         indices_pp[type] = NULL;
    }

    Nh_freeHashMap(NH_ECMASCRIPT_INDEXMAP.ParseNodeNames);
    Nh_freeHashMap(NH_ECMASCRIPT_INDEXMAP.ReservedWords);

    allocated = NH_FALSE;

NH_ECMASCRIPT_SILENT_END()
}


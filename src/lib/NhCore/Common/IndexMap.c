// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE ========================================================================================

#include "IndexMap.h"
#include "../MediaType.h"
#include "../Memory.h"
#include "../Array.h"

#include "MACRO/FLOW.h"
#include "MACRO/DEFAULT_CHECK.h"

#include <string.h>

// DECLARE =========================================================================================

typedef enum NH_INDEXMAP_E {
    NH_INDEXMAP_MEDIA_TYPE = 0,
    NH_INDEXMAP_COUNT,
} NH_INDEXMAP_E;

// DATA ============================================================================================

Nh_IndexMap NH_INDEXMAP;
static unsigned int *indices_pp[NH_INDEXMAP_COUNT] = {NULL};

// CREATE/DESTROY ==================================================================================

static NH_RESULT Nh_getNames(
    NH_INDEXMAP_E type, NH_BYTE ***array_ppp, int *count_p)
{
NH_BEGIN()

    switch (type)
    {
        case NH_INDEXMAP_MEDIA_TYPE :
        {
            *array_ppp = (NH_BYTE**) NH_MEDIA_TYPE_TEMPLATES_PP; 
            *count_p = NH_MEDIA_TYPE_TEMPLATES_PP_COUNT; 
            break;
        }
        default : NH_DIAGNOSTIC_END(NH_ERROR_BAD_STATE)
    }

    if (*array_ppp == NULL) {NH_DIAGNOSTIC_END(NH_ERROR_BAD_STATE)}

NH_DIAGNOSTIC_END(NH_SUCCESS)
}


static NH_RESULT Nh_createSingleIndexMap(
    NH_INDEXMAP_E type, Nh_HashMap *map_p)
{
NH_BEGIN()

    int count = 0; NH_BYTE **names_pp = NULL;
    NH_CHECK(Nh_getNames(type, &names_pp, &count))

    *map_p = Nh_createHashMap();

    for (int i = 0; i < count; ++i) {
        NH_CHECK(Nh_addToHashMap(map_p, names_pp[i], &indices_pp[type][i]))
    }

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

NH_RESULT Nh_createIndexMap()
{
NH_BEGIN()

    for (int type = 0; type < NH_INDEXMAP_COUNT; ++type)  
    {
        int count = 0;
        const NH_BYTE **names_pp = NULL;
        if (Nh_getNames(type, (NH_BYTE***)&names_pp, &count) != NH_SUCCESS) {
            NH_DIAGNOSTIC_END(NH_ERROR_BAD_STATE)
        }

        indices_pp[type] = Nh_allocate(sizeof(unsigned int) * count);
        NH_CHECK_MEM(indices_pp[type])
        
        for (int i = 0; i < count; ++i) {
            indices_pp[type][i] = i;
        }
    }    

    NH_CHECK(Nh_createSingleIndexMap(NH_INDEXMAP_MEDIA_TYPE, &NH_INDEXMAP.MediaTypes))

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

void Nh_freeIndexMap()
{
NH_BEGIN()

    for (int type = 0; type < NH_INDEXMAP_COUNT; ++type)  
    {
         Nh_free(indices_pp[type]);
         indices_pp[type] = NULL;
    }

    Nh_freeHashMap(NH_INDEXMAP.MediaTypes);

NH_SILENT_END()
}


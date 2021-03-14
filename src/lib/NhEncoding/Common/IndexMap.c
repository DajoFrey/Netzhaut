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

#include "../Base/Encodings.h"
#include "../../NhCore/Memory.h"

#include <string.h>

// DECLARE =========================================================================================

typedef enum NH_ENCODING_INDEXMAP_E {
    NH_ENCODING_INDEXMAP_LABELS = 0,
    NH_ENCODING_INDEXMAP_COUNT,
} NH_ENCODING_INDEXMAP_E;

// DATA ============================================================================================

Nh_Encoding_IndexMap NH_ENCODING_INDEXMAP;
static unsigned int *indices_pp[NH_ENCODING_INDEXMAP_COUNT] = {NULL};
static NH_BOOL allocated = NH_FALSE;

// CREATE/DESTROY ==================================================================================

static NH_ENCODING_RESULT Nh_Encoding_getNames(
    NH_ENCODING_INDEXMAP_E type, NH_BYTE ***array_ppp, int *count_p)
{
NH_ENCODING_BEGIN()

    switch (type)
    {
        case NH_ENCODING_INDEXMAP_LABELS :
        {
            *array_ppp = (NH_BYTE**)NH_ENCODING_LABELS_PP; 
            *count_p = NH_ENCODING_LABELS_PP_COUNT; 
            break;
        }
        default : NH_ENCODING_DIAGNOSTIC_END(NH_ENCODING_ERROR_BAD_STATE)
    }

    if (*array_ppp == NULL) {NH_ENCODING_DIAGNOSTIC_END(NH_ENCODING_ERROR_BAD_STATE)}

NH_ENCODING_DIAGNOSTIC_END(NH_ENCODING_SUCCESS)
}

static NH_ENCODING_RESULT Nh_Encoding_createSingleIndexMap(
    NH_ENCODING_INDEXMAP_E type, Nh_HashMap *map_p)
{
NH_ENCODING_BEGIN()

    int count = 0; NH_BYTE **names_pp = NULL;
    NH_ENCODING_CHECK(Nh_Encoding_getNames(type, &names_pp, &count))

    *map_p = Nh_createHashMap();

    for (int i = 0; i < count; ++i) {
        Nh_addToHashMap(map_p, names_pp[i], &indices_pp[type][i]);
    }

NH_ENCODING_DIAGNOSTIC_END(NH_ENCODING_SUCCESS)
}

NH_ENCODING_RESULT Nh_Encoding_createIndexMap()
{
NH_ENCODING_BEGIN()

    if (allocated) {NH_ENCODING_DIAGNOSTIC_END(NH_ENCODING_SUCCESS)}

    for (int type = 0; type < NH_ENCODING_INDEXMAP_COUNT; ++type)  
    {
        int count = 0;
        const NH_BYTE **names_pp = NULL;
        if (Nh_Encoding_getNames(type, (NH_BYTE***)&names_pp, &count) != NH_ENCODING_SUCCESS) {
            NH_ENCODING_DIAGNOSTIC_END(NH_ENCODING_ERROR_BAD_STATE)
        }

        indices_pp[type] = Nh_allocate(sizeof(unsigned int) * count);
        NH_ENCODING_CHECK_MEM(indices_pp[type])
        
        for (int i = 0; i < count; ++i) {
            indices_pp[type][i] = i;
        }
    }    

    NH_ENCODING_CHECK(Nh_Encoding_createSingleIndexMap(NH_ENCODING_INDEXMAP_LABELS, &NH_ENCODING_INDEXMAP.Labels))

    allocated = NH_TRUE;

NH_ENCODING_DIAGNOSTIC_END(NH_ENCODING_SUCCESS)
}

void Nh_Encoding_freeIndexMap()
{
NH_ENCODING_BEGIN()

    if (allocated) {NH_ENCODING_SILENT_END()}

    for (int type = 0; type < NH_ENCODING_INDEXMAP_COUNT; ++type)  
    {
         Nh_free(indices_pp[type]);
         indices_pp[type] = NULL;
    }

    Nh_freeHashMap(NH_ENCODING_INDEXMAP.Labels);

    allocated = NH_FALSE;

NH_ENCODING_SILENT_END()
}


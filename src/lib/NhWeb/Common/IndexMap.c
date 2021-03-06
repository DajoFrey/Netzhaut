// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE ========================================================================================

#include "IndexMap.h"
#include "Macro.h"

#include "../Runtime/Builtin.h"
#include "../Runtime/Parser.h"

#include "../../NhCore/Memory.h"
#include "../../NhCore/Common/Macro.h"

#include NH_FLOW2
#include NH_CUSTOM_CHECK2

#include NH_WEB_FLOW
#include NH_WEB_DEFAULT_CHECK

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// DECLARE =========================================================================================

typedef enum NH_WEB_INDEXMAP_E {
    NH_WEB_INDEXMAP_FRAGMENT_NAMES = 0,
    NH_WEB_INDEXMAP_FRAGMENT_FUNCTION_NAMES,
    NH_WEB_INDEXMAP_PARSE_NODE_NAMES,
    NH_WEB_INDEXMAP_COUNT,
} NH_WEB_INDEXMAP_E;

// DATA ============================================================================================

static NH_BOOL init = NH_FALSE;
Nh_Web_IndexMap NH_WEB_INDEXMAP;
unsigned int *indices_pp[NH_WEB_INDEXMAP_COUNT] = {NULL};

// CREATE/FREE =====================================================================================

static NH_WEB_RESULT Nh_Web_getNames(
    NH_WEB_INDEXMAP_E type, char ***array_ppp, int *size_p)
{
NH_WEB_BEGIN()

    switch (type)
    {
        case NH_WEB_INDEXMAP_FRAGMENT_NAMES :
        {
            *array_ppp = (char**) NH_WEB_FRAGMENT_NAMES_PP; 
            *size_p = NH_WEB_FRAGMENT_NAMES_PP_COUNT; 
            break;
        }
        case NH_WEB_INDEXMAP_FRAGMENT_FUNCTION_NAMES :
        {
//            *array_ppp = (char**) NH_WEB_FRAGMENT_FUNCTION_NAMES_PP; 
//            *size_p = NH_WEB_FRAGMENT_FUNCTION_NAMES_PP_COUNT; 
            break;
        }
        case NH_WEB_INDEXMAP_PARSE_NODE_NAMES :
        {
            *array_ppp = (char**) NH_WEB_PARSE_NODE_NAMES_PP; 
            *size_p = NH_WEB_PARSE_NODE_NAMES_PP_COUNT; 
            break;
        }
        default : NH_WEB_DIAGNOSTIC_END(NH_WEB_ERROR_BAD_STATE)
    }

    if (array_ppp == NULL) {NH_WEB_DIAGNOSTIC_END(NH_WEB_ERROR_BAD_STATE)}

NH_WEB_DIAGNOSTIC_END(NH_WEB_SUCCESS)
}

static NH_WEB_RESULT Nh_Web_createSingleIndexMap(
    NH_WEB_INDEXMAP_E type, Nh_HashMap *map_p)
{
NH_WEB_BEGIN()

    int count = 0; char **names_pp = NULL;
    NH_WEB_CHECK(Nh_Web_getNames(type, &names_pp, &count))

    *map_p = Nh_createHashMap();

    for (int i = 0; i < count; ++i) {
        NH_CHECK(NH_WEB_ERROR_BAD_STATE, Nh_addToHashMap(map_p, names_pp[i], &indices_pp[type][i]))
    }

NH_WEB_DIAGNOSTIC_END(NH_WEB_SUCCESS)
}

NH_WEB_RESULT Nh_Web_createIndexMap()
{
NH_WEB_BEGIN()

    if (init) {NH_WEB_DIAGNOSTIC_END(NH_WEB_SUCCESS)}

    for (int type = 0; type < NH_WEB_INDEXMAP_COUNT; ++type)  
    {
        int count = 0;
        const char **names_pp = NULL;
        if (Nh_Web_getNames(type, (char***)&names_pp, &count) != NH_SUCCESS) {
            NH_WEB_DIAGNOSTIC_END(NH_WEB_ERROR_BAD_STATE)
        }
        if (count == 0) {continue;}

        indices_pp[type] = Nh_allocate(sizeof(unsigned int) * count);
        NH_WEB_CHECK_MEM(indices_pp[type])
        
        for (int i = 0; i < count; ++i) {indices_pp[type][i] = i;}
    }    

    NH_WEB_CHECK(Nh_Web_createSingleIndexMap(NH_WEB_INDEXMAP_FRAGMENT_NAMES, &NH_WEB_INDEXMAP.FragmentNames))
//    NH_WEB_CHECK(Nh_Web_createSingleIndexMap(NH_WEB_INDEXMAP_FRAGMENT_FUNCTION_NAMES, &NH_WEB_INDEXMAP.FragmentFunctionNames))
    NH_WEB_CHECK(Nh_Web_createSingleIndexMap(NH_WEB_INDEXMAP_PARSE_NODE_NAMES, &NH_WEB_INDEXMAP.ParseNodeNames))

    init = NH_TRUE;

NH_WEB_DIAGNOSTIC_END(NH_WEB_SUCCESS)
}

void Nh_Web_freeIndexMap()
{
NH_WEB_BEGIN()

    if (!init) {NH_WEB_SILENT_END()}

    for (int type = 0; type < NH_WEB_INDEXMAP_COUNT; ++type)  
    {
         Nh_free(indices_pp[type]);
         indices_pp[type] = NULL;
    }

    Nh_freeHashMap(NH_WEB_INDEXMAP.FragmentNames);
    Nh_freeHashMap(NH_WEB_INDEXMAP.FragmentFunctionNames);
    Nh_freeHashMap(NH_WEB_INDEXMAP.ParseNodeNames);

    init = NH_FALSE;

NH_WEB_SILENT_END()
}


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

#include NH_WEBIDL_FLOW
#include NH_WEBIDL_DEFAULT_CHECK

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// DECLARE =========================================================================================

typedef enum NH_WEBIDL_INDEXMAP_E {
    NH_WEBIDL_INDEXMAP_FRAGMENT_NAMES = 0,
    NH_WEBIDL_INDEXMAP_FRAGMENT_FUNCTION_NAMES,
    NH_WEBIDL_INDEXMAP_PARSE_NODE_NAMES,
    NH_WEBIDL_INDEXMAP_COUNT,
} NH_WEBIDL_INDEXMAP_E;

// DATA ============================================================================================

static NH_BOOL init = NH_FALSE;
Nh_WebIDL_IndexMap NH_WEBIDL_INDEXMAP;
unsigned int *indices_pp[NH_WEBIDL_INDEXMAP_COUNT] = {NULL};

// CREATE/FREE =====================================================================================

static NH_WEBIDL_RESULT Nh_WebIDL_getNames(
    NH_WEBIDL_INDEXMAP_E type, char ***array_ppp, int *size_p)
{
NH_WEBIDL_BEGIN()

    switch (type)
    {
        case NH_WEBIDL_INDEXMAP_FRAGMENT_NAMES :
        {
            *array_ppp = (char**) NH_WEBIDL_FRAGMENT_NAMES_PP; 
            *size_p = NH_WEBIDL_FRAGMENT_NAMES_PP_COUNT; 
            break;
        }
        case NH_WEBIDL_INDEXMAP_FRAGMENT_FUNCTION_NAMES :
        {
//            *array_ppp = (char**) NH_WEBIDL_FRAGMENT_FUNCTION_NAMES_PP; 
//            *size_p = NH_WEBIDL_FRAGMENT_FUNCTION_NAMES_PP_COUNT; 
            break;
        }
        case NH_WEBIDL_INDEXMAP_PARSE_NODE_NAMES :
        {
            *array_ppp = (char**) NH_WEBIDL_PARSE_NODE_NAMES_PP; 
            *size_p = NH_WEBIDL_PARSE_NODE_NAMES_PP_COUNT; 
            break;
        }
        default : NH_WEBIDL_DIAGNOSTIC_END(NH_WEBIDL_ERROR_BAD_STATE)
    }

    if (array_ppp == NULL) {NH_WEBIDL_DIAGNOSTIC_END(NH_WEBIDL_ERROR_BAD_STATE)}

NH_WEBIDL_DIAGNOSTIC_END(NH_WEBIDL_SUCCESS)
}

static NH_WEBIDL_RESULT Nh_WebIDL_createSingleIndexMap(
    NH_WEBIDL_INDEXMAP_E type, Nh_HashMap *map_p)
{
NH_WEBIDL_BEGIN()

    int count = 0; char **names_pp = NULL;
    NH_WEBIDL_CHECK(Nh_WebIDL_getNames(type, &names_pp, &count))

    *map_p = Nh_createHashMap();

    for (int i = 0; i < count; ++i) {
        NH_CHECK(NH_WEBIDL_ERROR_BAD_STATE, Nh_addToHashMap(map_p, names_pp[i], &indices_pp[type][i]))
    }

NH_WEBIDL_DIAGNOSTIC_END(NH_WEBIDL_SUCCESS)
}

NH_WEBIDL_RESULT Nh_WebIDL_createIndexMap()
{
NH_WEBIDL_BEGIN()

    if (init) {NH_WEBIDL_DIAGNOSTIC_END(NH_WEBIDL_SUCCESS)}

    for (int type = 0; type < NH_WEBIDL_INDEXMAP_COUNT; ++type)  
    {
        int count = 0;
        const char **names_pp = NULL;
        if (Nh_WebIDL_getNames(type, (char***)&names_pp, &count) != NH_SUCCESS) {
            NH_WEBIDL_DIAGNOSTIC_END(NH_WEBIDL_ERROR_BAD_STATE)
        }
        if (count == 0) {continue;}

        indices_pp[type] = Nh_allocate(sizeof(unsigned int) * count);
        NH_WEBIDL_CHECK_MEM(indices_pp[type])
        
        for (int i = 0; i < count; ++i) {indices_pp[type][i] = i;}
    }    

    NH_WEBIDL_CHECK(Nh_WebIDL_createSingleIndexMap(NH_WEBIDL_INDEXMAP_FRAGMENT_NAMES, &NH_WEBIDL_INDEXMAP.FragmentNames))
//    NH_WEBIDL_CHECK(Nh_WebIDL_createSingleIndexMap(NH_WEBIDL_INDEXMAP_FRAGMENT_FUNCTION_NAMES, &NH_WEBIDL_INDEXMAP.FragmentFunctionNames))
    NH_WEBIDL_CHECK(Nh_WebIDL_createSingleIndexMap(NH_WEBIDL_INDEXMAP_PARSE_NODE_NAMES, &NH_WEBIDL_INDEXMAP.ParseNodeNames))

    init = NH_TRUE;

NH_WEBIDL_DIAGNOSTIC_END(NH_WEBIDL_SUCCESS)
}

void Nh_WebIDL_freeIndexMap()
{
NH_WEBIDL_BEGIN()

    if (!init) {NH_WEBIDL_SILENT_END()}

    for (int type = 0; type < NH_WEBIDL_INDEXMAP_COUNT; ++type)  
    {
         Nh_free(indices_pp[type]);
         indices_pp[type] = NULL;
    }

    Nh_freeHashMap(NH_WEBIDL_INDEXMAP.FragmentNames);
    Nh_freeHashMap(NH_WEBIDL_INDEXMAP.FragmentFunctionNames);
    Nh_freeHashMap(NH_WEBIDL_INDEXMAP.ParseNodeNames);

    init = NH_FALSE;

NH_WEBIDL_SILENT_END()
}


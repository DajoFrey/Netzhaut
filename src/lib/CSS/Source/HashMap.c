// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE ========================================================================================

#include "../Header/HashMap.h"
#include "../Header/Macros.h"
#include "../Header/Memory.h"
#include "../Header/MediaType.h"

#include "../../HTML/Main/Header/Attribute.h"

#include "../../CSS/Header/Pseudo.h"
#include "../../CSS/Header/Color.h"

#include NH_FLOW
#include NH_DEFAULT_CHECK

#include <string.h>

// DATA ============================================================================================

Nh_HashMaps NH_HASHMAPS;

// DECLARE =========================================================================================

typedef enum NH_HASHMAP {
    NH_HASHMAP_MEDIA_TYPE,
    NH_HASHMAP_HTML_ATTRIBUTES,   
    NH_HASHMAP_HTML_TAGS,   
    NH_HASHMAP_CSS_PROPERTIES,    
    NH_HASHMAP_CSS_COLORS,        
    NH_HASHMAP_CSS_PSEUDO_CLASSES,
    NH_HASHMAP_CSS_PSEUDO_ELEMENTS,
} NH_HASHMAP;

static NH_RESULT Nh_createSingleHashMap(
    NH_HASHMAP type, Nh_HashMap *map_p
);
static NH_RESULT Nh_createHashMapArray(
    NH_HASHMAP type, Nh_HashMap *maps_p
);

static void Nh_freeSingleHashMap(
    NH_HASHMAP type, Nh_HashMap map
);
static void Nh_freeHashMapArray(
    NH_HASHMAP type, Nh_HashMap *maps_p
);

static NH_RESULT Nh_getNames(
    NH_HASHMAP type, char ***array_ppp, int *size_p
);

// API =============================================================================================

NH_RESULT Nh_createHashMaps()
{
NH_BEGIN()

    NH_CHECK(Nh_createSingleHashMap(NH_HASHMAP_MEDIA_TYPE, &NH_HASHMAPS.MediaTypes))
    NH_CHECK(Nh_createSingleHashMap(NH_HASHMAP_HTML_ATTRIBUTES, &NH_HASHMAPS.HTML.Attributes))
    NH_CHECK(Nh_createSingleHashMap(NH_HASHMAP_HTML_TAGS, &NH_HASHMAPS.HTML.Tags))
    NH_CHECK(Nh_createSingleHashMap(NH_HASHMAP_CSS_PROPERTIES, &NH_HASHMAPS.CSS.Properties))
    NH_CHECK(Nh_createSingleHashMap(NH_HASHMAP_CSS_COLORS, &NH_HASHMAPS.CSS.Colors))
    NH_CHECK(Nh_createSingleHashMap(NH_HASHMAP_CSS_PSEUDO_CLASSES, &NH_HASHMAPS.CSS.PseudoClasses))
    NH_CHECK(Nh_createSingleHashMap(NH_HASHMAP_CSS_PSEUDO_ELEMENTS, &NH_HASHMAPS.CSS.PseudoElements))

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

void Nh_freeHashMaps()
{
NH_BEGIN()

    Nh_freeSingleHashMap(NH_HASHMAP_MEDIA_TYPE, NH_HASHMAPS.MediaTypes);
    Nh_freeSingleHashMap(NH_HASHMAP_HTML_ATTRIBUTES, NH_HASHMAPS.HTML.Attributes);
    Nh_freeSingleHashMap(NH_HASHMAP_HTML_TAGS, NH_HASHMAPS.HTML.Tags);  
    Nh_freeSingleHashMap(NH_HASHMAP_CSS_PROPERTIES, NH_HASHMAPS.CSS.Properties);    
    Nh_freeSingleHashMap(NH_HASHMAP_CSS_COLORS, NH_HASHMAPS.CSS.Colors);  
    Nh_freeSingleHashMap(NH_HASHMAP_CSS_PSEUDO_CLASSES, NH_HASHMAPS.CSS.PseudoClasses);
    Nh_freeSingleHashMap(NH_HASHMAP_CSS_PSEUDO_ELEMENTS, NH_HASHMAPS.CSS.PseudoElements);

NH_SILENT_END()
}

// CREATE ==========================================================================================

static NH_RESULT Nh_createSingleHashMap(
    NH_HASHMAP type, Nh_HashMap *map_p)
{
NH_BEGIN()

    int size = 0;
    const char **array_pp = NULL;
    
    NH_CHECK(Nh_getNames(type, (char***)&array_pp, &size))
    NH_CHECK(Nh_createHashMap(map_p, size, array_pp))

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

static NH_RESULT Nh_createHashMapArray(
    NH_HASHMAP type, Nh_HashMap *maps_p)
{
NH_BEGIN()

    int size = 0;
    const char **names_pp = NULL;

    switch (type)
    {
        default : break;
    }

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

NH_RESULT Nh_createHashMap(
    Nh_HashMap *map_p, int size, const char **names_pp)
{
NH_BEGIN()

    *map_p = hashmap_new();

    for (int i = 0; i < size; i++)
    {
        Nh_HashValue *value_p = Nh_allocate(sizeof(Nh_HashValue));
        NH_CHECK_MEM(value_p)

        memset(value_p->key_p, '\0', sizeof(char) * 128);
        snprintf(value_p->key_p, 128, "%s", names_pp[i]); 
        value_p->number = i;
        
        if (hashmap_put(*map_p, value_p->key_p, value_p) != MAP_OK) {NH_DIAGNOSTIC_END(NH_ERROR_BAD_STATE)}
    }

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

// FREE ============================================================================================

static void Nh_freeSingleHashMap(
    NH_HASHMAP type, Nh_HashMap map)
{
NH_BEGIN()

    int size = 0;
    const char **names_pp = NULL;

    if (Nh_getNames(type, (char***)&names_pp, &size) != NH_SUCCESS) {NH_SILENT_END()}
    Nh_freeHashMap(map, size, names_pp);

NH_SILENT_END()
}

static void Nh_freeHashMapArray(
    NH_HASHMAP type, Nh_HashMap *maps_p)
{
NH_BEGIN()

    int size = 0;
    const char **names_pp = NULL;

    switch (type)
    {
        default : break;
    }

NH_SILENT_END()
}

void Nh_freeHashMap(
    Nh_HashMap map, int size, const char **names_pp)
{
NH_BEGIN()

    for (int i = 0; i < size; i++)
    {
        Nh_HashValue* HashValue_p;

        if (hashmap_get(map, (char*)names_pp[i], (void**) &HashValue_p) != MAP_OK) {
            NH_SILENT_END()
        }
        if (hashmap_remove(map, (char*)names_pp[i]) != MAP_OK) {NH_SILENT_END()}
        Nh_free(HashValue_p);  
    }
    
    hashmap_free(map);

NH_SILENT_END()
}

// HELPER ==========================================================================================

static NH_RESULT Nh_getNames(
    NH_HASHMAP type, char ***array_ppp, int *size_p)
{
NH_BEGIN()

    switch (type)
    {
        case NH_HASHMAP_MEDIA_TYPE          :
        {
            *array_ppp = (char**) NH_MEDIA_TYPE_TEMPLATES_PP; 
            *size_p = NH_MEDIA_TYPE_TEMPLATES_PP_COUNT; 
            break;
        }
//        case NH_HASHMAP_HTML_ATTRIBUTES     :
//        {
//            *array_ppp = (char**) NH_HTML_ATTRIBUTES_PP; 
//            *size_p = NH_HTML_ATTRIBUTES_PP_COUNT; 
//            break;
//        }
//        case NH_HASHMAP_HTML_TAGS           : 
//        {
//            *array_ppp = (char**) NH_HTML_TAGS_PP; 
//            *size_p = NH_HTML_TAGS_PP_COUNT; 
//            break;
//        }
//        case NH_HASHMAP_CSS_PROPERTIES      : 
//        {
//            *array_ppp = (char**) NH_CSS_PROPERTIES_PP;
//            *size_p = NH_CSS_PROPERTIES_PP_COUNT;
//            break;
//        }
//        case NH_HASHMAP_CSS_COLORS          : 
//        {
//            *array_ppp = (char**) NH_CSS_COLORS_PP; 
//            *size_p = NH_CSS_COLORS_PP_COUNT; 
//            break;
//        }
//        case NH_HASHMAP_CSS_PSEUDO_CLASSES  :
//        {
//            *array_ppp = (char**) NH_CSS_PSEUDO_CLASSES_PP; 
//            *size_p = NH_CSS_PSEUDO_CLASSES_PP_COUNT; 
//            break;
//        }
//        case NH_HASHMAP_CSS_PSEUDO_ELEMENTS :
//        {
//            *array_ppp = (char**) NH_CSS_PSEUDO_ELEMENTS_PP; 
//            *size_p = NH_CSS_PSEUDO_ELEMENTS_PP_COUNT; 
//            break;
//        }
        default                             : NH_DIAGNOSTIC_END(NH_ERROR_BAD_STATE)
    }

    if (array_ppp == NULL) {NH_DIAGNOSTIC_END(NH_ERROR_BAD_STATE)}

NH_DIAGNOSTIC_END(NH_SUCCESS)
}


#ifndef NH_HASHMAP_H
#define NH_HASHMAP_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "Array.h"
#include "Common/Result.h"
#include "External/c_hashmap.h"

#endif

/** @addtogroup NhCoreMacros
 *  @{
 */

    #define Nh_HashMap map_t 

/** @} */

/** @addtogroup NhCoreFunctions
 *  @{
 */

    Nh_HashMap Nh_createHashMap(
    );
    
    NH_RESULT Nh_addToHashMap(
        Nh_HashMap *Map_p, NH_BYTE *key_p, void *value_p
    );
    
    void Nh_freeHashMap(
        Nh_HashMap Map
    );

    void *Nh_getFromHashMap(
        Nh_HashMap *Map_p, NH_BYTE *key_p
    );

/** @} */

#endif 

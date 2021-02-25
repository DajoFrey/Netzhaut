#ifndef NH_HASHMAP_H
#define NH_HASHMAP_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "External/c_hashmap.h"

#include "../../Netzhaut/Netzhaut.h"

#endif

/** @addtogroup CoreStructs Structs
 *  \ingroup Core
 *  @{
 */

    typedef struct Nh_HashValue {
        char key_p[128];     
        unsigned int number;
    } Nh_HashValue;

    typedef struct Nh_HashMaps {
        Nh_HashMap MediaTypes;
        struct {
            Nh_HashMap Attributes;   
            Nh_HashMap Tags; 
        } HTML;
        struct {
            Nh_HashMap Properties;   
            Nh_HashMap Colors;        
            Nh_HashMap PseudoClasses;
            Nh_HashMap PseudoElements; 
        } CSS;
    } Nh_HashMaps;

/** @} */

/** @addtogroup CoreData Data
 *  \ingroup Core
 *  @{
 */

    extern Nh_HashMaps NH_HASHMAPS;

/** @} */

/** @addtogroup CoreFunctions Functions
 *  \ingroup Core
 *  @{
 */

    NH_RESULT Nh_createHashMaps(
    );

    void Nh_freeHashMaps(
    );

    NH_RESULT Nh_createHashMap(
        Nh_HashMap *map_p, int size, const char **names_pp
    );
    
    void Nh_freeHashMap(
        Nh_HashMap map, int size, const char **names_pp
    );

/** @} */

#endif 

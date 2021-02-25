#ifndef NH_ARRAY_H
#define NH_ARRAY_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "Common/API.h"
#include "Common/API.h"
#include "Common/Result.h"

#endif

/** @addtogroup NhCoreFunctions
 *  @{
 */

    Nh_Array Nh_initArray(
        int elementSize, int allocatedLengthPerChunk
    );

    NH_RESULT Nh_appendToArray(
        Nh_Array *Array_p, void *bytes_p, int count
    );

    NH_RESULT Nh_insertIntoArray(
        Nh_Array *Array_p, int index, void *elements_p, int length
    );

    void *_Nh_getFromArray(
        Nh_Array *Array_p, int index
    );

    void *Nh_getFromArray(
        Nh_Array *Array_p, int index
    );
   
    NH_RESULT Nh_removeTailFromArray(
        Nh_Array *Array_p, unsigned int count
    );

    NH_RESULT Nh_removeFromArray(
        Nh_Array *Array_p, int index, unsigned int count
    );

    void _Nh_freeArray(
        Nh_Array *Array_p
    );

    void Nh_freeArray(
        Nh_Array *Array_p
    );

/** @} */

#endif 

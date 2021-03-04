#ifndef NH_LIST_H
#define NH_LIST_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "Common/API.h"
#include "Common/Result.h"

#endif

/** @addtogroup NhCoreStructs
 *  @{
 */

    /**
     * Dynamic list implementation using a two dimensional array. 
     */
    typedef struct Nh_List {
        int chunkSize;   
        int chunkCount;
        int size;
        void **pp;
    } Nh_List;

/** @} */

/** @addtogroup NhCoreFunctions
 *  @{
 */

    Nh_List Nh_initList(
        unsigned int chunkSize
    );
    
    NH_RESULT _Nh_appendToList(
        Nh_List *List_p, void *handle_p
    );
    
    NH_RESULT Nh_appendToList(
        Nh_List *List_p, void *handle_p
    );

    NH_RESULT Nh_appendItemsToList(
        Nh_List *List_p, Nh_List *Append_p
    );

    NH_RESULT Nh_prependToList(
        Nh_List *List_p, void *handle_p
    );
   
    NH_RESULT Nh_insertIntoList(
        Nh_List *List_p, void *handle_p, int index
    );

    void *Nh_getFromList(
        Nh_List *List_p, int index
    );

    void _Nh_freeList(
        Nh_List *List_p, NH_BOOL freeHandles
    );
    
    void Nh_freeList(
        Nh_List *List_p, NH_BOOL freeHandles
    );
    
    NH_RESULT _Nh_removeFromList(
        Nh_List *List_p, NH_BOOL freeHandle, unsigned int index
    );
    
    NH_RESULT Nh_removeFromList(
        Nh_List *List_p, NH_BOOL freeHandle, unsigned int index
    );

    NH_RESULT Nh_removeFromList2(
        Nh_List *List_p, NH_BOOL freeHandle, void *handle_p
    );

/** @} */

#endif 

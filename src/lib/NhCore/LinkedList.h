#ifndef NH_LINKED_LIST_H
#define NH_LINKED_LIST_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "Common/Result.h"
#include "Common/API.h"

#include <stddef.h>
#include <stdbool.h>

#endif

/** @addtogroup NhCoreMacros
 *  @{
 */

    #define NH_INIT_LINKED_LIST(List) List.Head_p = NULL; List.count = 0;

/** @} */

/** @addtogroup NhCoreStructs
 *  @{
 */

    /**
     * Opaque structure holding the actual data of @ref Nh_LinkedList.
     */
    typedef struct Nh_LinkedListItem Nh_LinkedListItem;

    /**
     * Generic linked-list which holds data pointers. 
     */
    typedef struct Nh_LinkedList {
        int count;                 /**<Number of items.*/
        Nh_LinkedListItem *Head_p; /**<Opaque pointer to the first item in the list. A list with \ref count == 0 should have this set as NULL. The actual data should always be queried using @ref Nh_getFromLinkedList.*/
    } Nh_LinkedList;

/** @} */

/** @addtogroup NhCoreFunctions
 *  @{
 */

    Nh_LinkedList Nh_initLinkedList(
    );

    /**
     * Adds \p data_p to the list pointed by \p List_p.
     *
     * @param  List_p A pointer to the list to which \p data_p will be added.
     * @param  data_p A pointer to data that you want to add. Must be allocated beforehand.
     * @return        @ref NH_SUCCESS when \p data_p was added, various error codes if something went wrong. 
     */
    NH_RESULT Nh_appendToLinkedList(
        Nh_LinkedList *List_p, void *data_p
    );
   
    NH_RESULT _Nh_appendToLinkedList(
        Nh_LinkedList *List_p, void *data_p
    );

    /**
     * Prepends \p data_p to the list pointed by \p List_p.
     *
     * @param  List_p A pointer to the list to which \p data_p will be prepended.
     * @param  data_p A pointer to data that you want to prepend. Must be allocated beforehand.
     * @return        @ref NH_SUCCESS when \p data_p was prepended, various error codes if something went wrong. 
     */
    NH_RESULT Nh_prependToLinkedList(
        Nh_LinkedList *List_p, void *data_p
    );

    NH_RESULT Nh_replaceInLinkedList(
        Nh_LinkedList *List_p, void *replace_p, void *replacement_p
    );

    NH_RESULT Nh_insertIntoLinkedList(
        Nh_LinkedList *List_p, void *data_p, int index
    );

    /**
     * Get item data specified by \p index from \p List_p.
     *
     * @param  List_p A pointer to the list that you want to get data from.
     * @param  index  Item location. 0 would be the index of the first item in \p List_p.
     * @return        A pointer to the data from the specified list-item, or NULL if some error occured.
     */ 
    void *Nh_getFromLinkedList(
        Nh_LinkedList *List_p, int index
    );
   
    void *_Nh_getFromLinkedList(
        Nh_LinkedList *List_p, int index
    );

    NH_RESULT Nh_setInLinkedList(
        Nh_LinkedList *List_p, int index, void *data_p
    );

    /**
     * Destroy the list pointed to by \p List_p. This operation always destroys list internal data.
     *
     * @param List_p   A pointer to the list that you want to destroy.
     * @param freeData Flag that indicates if the data that was added using @ref Nh_appendToLinkedList should be freed.
     * @return         Nothing.
     */ 
    void Nh_destroyLinkedList(
        Nh_LinkedList *List_p, bool freeData
    );

    void Nh_removeFromLinkedList(
        Nh_LinkedList *List_p, int index, bool freeData
    );

    void Nh_removeFromLinkedList2(
        Nh_LinkedList *List_p, void *pointer, NH_BOOL freeData
    );

    void _Nh_removeFromLinkedList2( // TODO multithreading
        Nh_LinkedList *List_p, void *pointer, NH_BOOL freeData
    );

    NH_BOOL Nh_inLinkedList(
        Nh_LinkedList *List_p, void *pointer
    );

    int Nh_getLinkedListIndex(
        Nh_LinkedList *List_p, void *pointer
    );

/** @} */

#endif 

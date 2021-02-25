// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE ========================================================================================

#include "LinkedList.h"
#include "Memory.h"

#include "Common/Macro.h"
#include NH_FLOW
#include NH_DEFAULT_CHECK

#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <limits.h>

#ifdef __unix__
    #include <unistd.h>
    #include <sys/types.h>
    #include <sys/wait.h>
#endif

// DECLARE =========================================================================================

struct Nh_LinkedListItem {
    void *data_p;
    struct Nh_LinkedListItem *Next_p;
};

// LINKED LIST =====================================================================================

Nh_LinkedList Nh_initLinkedList()
{
NH_BEGIN()

    Nh_LinkedList List;
    List.count = 0;
    List.Head_p = NULL;

NH_END(List)
}

NH_RESULT _Nh_appendToLinkedList( // TODO multithreading
    Nh_LinkedList *List_p, void *data_p)
{
    if (List_p == NULL || data_p == NULL) {return NH_ERROR_BAD_STATE;}

    Nh_LinkedListItem **Item_pp = &List_p->Head_p;
    while (*Item_pp != NULL) {Item_pp = &((*Item_pp)->Next_p);}

    *Item_pp = _Nh_allocate(sizeof(Nh_LinkedListItem));
    if (*Item_pp == NULL) {return NH_ERROR_BAD_STATE;}
    
    (*Item_pp)->data_p = data_p;
    (*Item_pp)->Next_p = NULL;

    List_p->count++;

    return NH_SUCCESS;
}

NH_RESULT Nh_appendToLinkedList( // TODO multithreading
    Nh_LinkedList *List_p, void *data_p)
{
NH_BEGIN()

    NH_CHECK_NULL(List_p)
    NH_CHECK_NULL(data_p)

    Nh_LinkedListItem **Item_pp = &List_p->Head_p;
    while (*Item_pp != NULL) {Item_pp = &((*Item_pp)->Next_p);}

    *Item_pp = Nh_allocate(sizeof(Nh_LinkedListItem));
    NH_CHECK_MEM(*Item_pp)
    
    (*Item_pp)->data_p = data_p;
    (*Item_pp)->Next_p = NULL;

    List_p->count++;

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

NH_RESULT Nh_insertIntoLinkedList( // TODO multithreading
    Nh_LinkedList *List_p, void *data_p, int index)
{
NH_BEGIN()

    NH_CHECK_NULL(List_p)
    NH_CHECK_NULL(data_p)

    if (index >= List_p->count) {
        NH_DIAGNOSTIC_END(Nh_appendToLinkedList(List_p, data_p))
    }
    if (index <= 0) {
        NH_DIAGNOSTIC_END(Nh_prependToLinkedList(List_p, data_p))
    }

    int tmp = 0;
    Nh_LinkedListItem **Item_pp = &List_p->Head_p;
    Nh_LinkedListItem **Prev_pp = NULL;

    while (*Item_pp != NULL && tmp < index) {
        Prev_pp = Item_pp;
        Item_pp = &((*Item_pp)->Next_p);
        tmp++;
    }
    
    Nh_LinkedListItem *New_p = Nh_allocate(sizeof(Nh_LinkedListItem));
    NH_CHECK_MEM(New_p)
    
    New_p->data_p = data_p;
    New_p->Next_p = *Item_pp;

    (*Prev_pp)->Next_p = New_p;

    List_p->count++;

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

NH_RESULT Nh_prependToLinkedList( // TODO multithreading
    Nh_LinkedList *List_p, void *data_p)
{
NH_BEGIN()

    NH_CHECK_NULL(List_p)
    NH_CHECK_NULL(data_p)

    if (List_p->Head_p == NULL) {NH_END(Nh_appendToLinkedList(List_p, data_p))}
    else {

        Nh_LinkedListItem *Next_p = List_p->Head_p;
        List_p->Head_p = Nh_allocate(sizeof(Nh_LinkedListItem));
        NH_CHECK_MEM(List_p->Head_p)

        List_p->Head_p->data_p = data_p;
        List_p->Head_p->Next_p = Next_p;

        List_p->count++;
    }

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

NH_RESULT Nh_setInLinkedList(
    Nh_LinkedList *List_p, int index, void *data_p)
{
NH_BEGIN()

    Nh_LinkedListItem **Item_pp = &List_p->Head_p;

    for (int i = 0; i < index; ++i) {
        if ((*Item_pp)->Next_p != NULL) {Item_pp = &(*Item_pp)->Next_p;}
        else {NH_DIAGNOSTIC_END(NH_ERROR_BAD_STATE)}
    }

    (*Item_pp)->data_p = data_p;

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

NH_RESULT Nh_replaceInLinkedList(
    Nh_LinkedList *List_p, void *replace_p, void *replacement_p)
{
NH_BEGIN()

    NH_CHECK_NULL(List_p)
    NH_CHECK_NULL(replace_p) 
    NH_CHECK_NULL(replacement_p)

    int index = 0;
    Nh_LinkedListItem **Item_pp = &List_p->Head_p;
    while (*Item_pp != NULL && (*Item_pp)->data_p != replace_p) {
        Item_pp = &((*Item_pp)->Next_p);
        index++;
    }

    if (index == List_p->count) {NH_DIAGNOSTIC_END(NH_ERROR_BAD_STATE)}
    (*Item_pp)->data_p = replacement_p;

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

void *_Nh_getFromLinkedList(
    Nh_LinkedList *List_p, int index)
{
    if (List_p == NULL || List_p->Head_p == NULL) {return NULL;}

    Nh_LinkedListItem **Item_pp = &List_p->Head_p;

    for (int i = 0; i < index; ++i) {
        if ((*Item_pp)->Next_p != NULL) {Item_pp = &(*Item_pp)->Next_p;}
        else {return NULL;}
    }

    return (*Item_pp)->data_p;
}

void *Nh_getFromLinkedList(
    Nh_LinkedList *List_p, int index)
{
NH_BEGIN()

    if (List_p == NULL || List_p->Head_p == NULL) {NH_END(NULL)}

    Nh_LinkedListItem **Item_pp = &List_p->Head_p;

    for (int i = 0; i < index; ++i) {
        if ((*Item_pp)->Next_p != NULL) {Item_pp = &(*Item_pp)->Next_p;}
        else {NH_END(NULL)}
    }
    
NH_END((*Item_pp)->data_p)
}

void Nh_destroyLinkedList( // TODO multithreading
    Nh_LinkedList *List_p, bool freeData)
{
NH_BEGIN()

    while (List_p->count > 0) {
        Nh_removeFromLinkedList(List_p, List_p->count - 1, freeData);
    }

    *List_p = Nh_initLinkedList();
    
NH_SILENT_END()
}

void Nh_removeFromLinkedList( // TODO multithreading
    Nh_LinkedList *List_p, int index, bool freeData)
{
NH_BEGIN()

    if (List_p == NULL) {NH_SILENT_END()}

    Nh_LinkedListItem **Item_pp = &List_p->Head_p;
    Nh_LinkedListItem **Prev_pp = NULL;

    for (int i = 0; i < index; ++i) 
    {
        if (*Item_pp != NULL) 
        {
            Prev_pp = Item_pp;
            Item_pp = &(*Item_pp)->Next_p;
        }
        else {NH_SILENT_END()}
    }

    Nh_LinkedListItem *Next_p = (*Item_pp)->Next_p;

    if (freeData) {Nh_free((*Item_pp)->data_p);}
    Nh_free(*Item_pp);
   
    if (index == 0) {List_p->Head_p = Next_p;}
    else {(*Prev_pp)->Next_p = Next_p;}

    List_p->count -= 1;

NH_SILENT_END()
}

void Nh_removeFromLinkedList2( // TODO multithreading
    Nh_LinkedList *List_p, void *pointer, NH_BOOL freeData)
{
NH_BEGIN()

    _Nh_removeFromLinkedList2(List_p, pointer, freeData);

NH_SILENT_END()
}

void _Nh_removeFromLinkedList2( // TODO multithreading
    Nh_LinkedList *List_p, void *pointer, NH_BOOL freeData)
{
    if (List_p == NULL) {return;}

    Nh_LinkedListItem *Item_p = List_p->Head_p;
    Nh_LinkedListItem *Previous_p = NULL;

    int index = 0;
    for (index = 0; index < List_p->count && Item_p != NULL; ++index) 
    {
        if (Item_p->data_p == pointer) {break;}
        Previous_p = Item_p;
        Item_p = Item_p->Next_p;
    }

    if (Item_p == NULL || index == List_p->count) {return;}

    Nh_LinkedListItem *Next_p = Item_p->Next_p;

    if (freeData) {_Nh_free(Item_p->data_p);}
    _Nh_free(Item_p);
   
    if (index == 0) {List_p->Head_p = Next_p;}
    else {Previous_p->Next_p = Next_p;}

    List_p->count -= 1;
}

NH_BOOL Nh_inLinkedList(
    Nh_LinkedList *List_p, void *pointer)
{
NH_BEGIN()

#include NH_CUSTOM_CHECK

    NH_CHECK_NULL(NH_FALSE, List_p)
    NH_CHECK_NULL(NH_FALSE, pointer)

    for (int i = 0; i < List_p->count; ++i) {
        if (Nh_getFromLinkedList(List_p, i) == pointer) {NH_END(NH_TRUE)}
    }

#include NH_DEFAULT_CHECK

NH_END(NH_FALSE)
}

int Nh_getLinkedListIndex(
    Nh_LinkedList *List_p, void *pointer)
{
NH_BEGIN()

#include NH_CUSTOM_CHECK

    NH_CHECK_NULL(NH_FALSE, List_p)
    NH_CHECK_NULL(NH_FALSE, pointer)

    for (int i = 0; i < List_p->count; ++i) {
        if (Nh_getFromLinkedList(List_p, i) == pointer) {NH_END(i)}
    }

#include NH_DEFAULT_CHECK

NH_END(-1)
}


// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE ========================================================================================

#include "List.h"
#include "Memory.h"

#include "Common/Macro.h"
#include NH_FLOW
#include NH_DEFAULT_CHECK

#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

// LIST ============================================================================================

Nh_List Nh_initList(
    unsigned int chunkSize)
{
NH_BEGIN()

    Nh_List List;

    List.chunkSize  = chunkSize;
    List.chunkCount = 0;
    List.size       = 0;
    List.pp = NULL;

NH_END(List)
}

NH_RESULT _Nh_appendToList(
    Nh_List *List_p, void *handle_p)
{
    if (List_p->size >= List_p->chunkSize * List_p->chunkCount) 
    {
        if (List_p->pp == NULL) {
            List_p->pp = Nh_allocate(sizeof(void*) * List_p->chunkSize);
            if (List_p->pp == NULL) {return NH_ERROR_BAD_STATE;}
        }
        else {
            List_p->pp = realloc(List_p->pp, sizeof(void*) * List_p->chunkSize * (List_p->chunkCount + 1));
            if (List_p->pp == NULL) {return NH_ERROR_BAD_STATE;}
        }
        List_p->chunkCount++;
    }

    List_p->pp[List_p->size] = handle_p;
    List_p->size++;

    return NH_SUCCESS;
}

NH_RESULT Nh_appendToList(
    Nh_List *List_p, void *handle_p)
{
NH_BEGIN()
NH_DIAGNOSTIC_END(_Nh_appendToList(List_p, handle_p))
}

NH_RESULT Nh_appendItemsToList(
    Nh_List *List_p, Nh_List *Append_p)
{
NH_BEGIN()

    for (int i = 0; i < Append_p->size; ++i) {
        NH_CHECK(Nh_appendToList(List_p, Append_p->pp[i]))
    }

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

NH_RESULT Nh_insertIntoList(
    Nh_List *List_p, void *handle_p, int index)
{
NH_BEGIN()

    NH_CHECK_NULL(List_p)
    NH_CHECK_NULL(handle_p)

    if (index >= List_p->size) {
        NH_DIAGNOSTIC_END(Nh_appendToList(List_p, handle_p))
    }
    if (index < 0) {index = 0;}

    NH_CHECK(Nh_appendToList(List_p, List_p->pp[List_p->size - 1]))

    for (int i = List_p->size - 1; i > index; --i) {
        List_p->pp[i] = List_p->pp[i - 1];
    }

    List_p->pp[index] = handle_p;

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

NH_RESULT Nh_prependToList(
    Nh_List *List_p, void *handle_p)
{
NH_BEGIN()
NH_DIAGNOSTIC_END(Nh_insertIntoList(List_p, handle_p, 0))
}

void *Nh_getFromList(
    Nh_List *List_p, int index)
{
NH_BEGIN()

    if (List_p == NULL || index < 0 || List_p->size <= index) {
        NH_END(NULL)
    }

NH_END(List_p->pp[index])
}

void _Nh_freeList(
    Nh_List *List_p, NH_BOOL freeHandles)
{
NH_BEGIN()

    if (freeHandles) {
        for (int i = 0; i < List_p->size; ++i) {
            _Nh_free(List_p->pp[i]);
        }
    }
    _Nh_free(List_p->pp);

    List_p->chunkCount = 0;
    List_p->size       = 0;
    List_p->pp = NULL;

NH_SILENT_END()
}

void Nh_freeList(
    Nh_List *List_p, NH_BOOL freeHandles)
{
NH_BEGIN()

    _Nh_freeList(List_p, freeHandles);

NH_SILENT_END()
}

NH_RESULT _Nh_removeFromList(
    Nh_List *List_p, NH_BOOL freeHandle, unsigned int index)
{
    if (index < 0 || List_p->size == 0) {return NH_ERROR_BAD_STATE;}
    if (index >= List_p->size) {index = List_p->size - 1;}

    if (freeHandle) {
        _Nh_free(List_p->pp[index]);
    }

    for (int i = index; i < List_p->size - 1; ++i) {
        List_p->pp[i] = List_p->pp[i + 1];
    }

    List_p->size--;

    if (List_p->size == 0) {
        Nh_free(List_p->pp);
        List_p->pp = NULL;
        List_p->chunkCount = 0;
    }
    else if (List_p->size == List_p->chunkSize * (List_p->chunkCount - 1)) {
        List_p->pp = realloc(List_p->pp, sizeof(void*) * List_p->size);
        if (List_p->pp == NULL) {return NH_ERROR_BAD_STATE;}
        List_p->chunkCount--;
    }

    return NH_SUCCESS;
}

NH_RESULT Nh_removeFromList(
    Nh_List *List_p, NH_BOOL freeHandle, unsigned int index)
{
NH_BEGIN()
NH_DIAGNOSTIC_END(_Nh_removeFromList(List_p, freeHandle, index))
}

NH_RESULT Nh_removeFromList2(
    Nh_List *List_p, NH_BOOL freeHandle, void *handle_p)
{
NH_BEGIN()

    int index = -1;
    for (int i = 0; i < List_p->size; ++i) {
        if (List_p->pp[i] == handle_p) {index = i; break;}
    }

    if (index > -1) {
        NH_DIAGNOSTIC_END(Nh_removeFromList(List_p, freeHandle, index))
    }

NH_DIAGNOSTIC_END(NH_ERROR_BAD_STATE)
}


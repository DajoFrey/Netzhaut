// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE ========================================================================================

#include "Array.h"
#include "Memory.h"

#include "Common/Macro.h"
#include NH_FLOW
#include NH_DEFAULT_CHECK

#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

// LIST ============================================================================================

Nh_Array Nh_initArray(
    int elementSize, int allocatedLengthPerChunk)
{
NH_BEGIN()

    Nh_Array Array;

    Array.elementSize = elementSize;
    Array.allocatedLengthPerChunk = allocatedLengthPerChunk;
    Array.allocatedLength = 0;
    Array.length = 0;
    Array.bytes_p = NULL;

NH_END(Array)
}

NH_RESULT Nh_appendToArray(
    Nh_Array *Array_p, void *bytes_p, unsigned long count)
{
NH_BEGIN()

    if (count <= 0) {NH_DIAGNOSTIC_END(NH_SUCCESS)}

    Array_p->length += count;

    while (Array_p->allocatedLength < Array_p->length)
    {
        if (Array_p->allocatedLength == 0) {
            Array_p->bytes_p = _Nh_allocate(Array_p->allocatedLengthPerChunk * Array_p->elementSize);
            memset(Array_p->bytes_p, 0, Array_p->allocatedLengthPerChunk * Array_p->elementSize);
        }
        else {
            Array_p->bytes_p = realloc(Array_p->bytes_p, (Array_p->allocatedLength * Array_p->elementSize) + (Array_p->allocatedLengthPerChunk * Array_p->elementSize));
            memset(Array_p->bytes_p + (Array_p->allocatedLength * Array_p->elementSize), 0, Array_p->allocatedLengthPerChunk * Array_p->elementSize);
        }

        NH_CHECK_NULL(Array_p->bytes_p)
        Array_p->allocatedLength += Array_p->allocatedLengthPerChunk;
    }

    size_t offset = (Array_p->length - count) * Array_p->elementSize;
    memcpy(&Array_p->bytes_p[offset], bytes_p, Array_p->elementSize * count);

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

void *_Nh_getFromArray(
    Nh_Array *Array_p, unsigned long index)
{
#include NH_CUSTOM_CHECK

    if (index < 0) {index = 0;}

    while (Array_p->allocatedLength <= index) 
    {
        if (Array_p->allocatedLength == 0) {
            Array_p->bytes_p = _Nh_allocate(Array_p->elementSize * Array_p->allocatedLengthPerChunk);
            memset(Array_p->bytes_p, 0, Array_p->allocatedLengthPerChunk * Array_p->elementSize);
        }
        else {
            Array_p->bytes_p = realloc(Array_p->bytes_p, (Array_p->allocatedLength * Array_p->elementSize) + (Array_p->allocatedLengthPerChunk * Array_p->elementSize));
            memset(Array_p->bytes_p + (Array_p->allocatedLength * Array_p->elementSize), 0, Array_p->allocatedLengthPerChunk * Array_p->elementSize);
        }

        NH_CHECK_NULL(NULL, Array_p->bytes_p)
        Array_p->allocatedLength += Array_p->allocatedLengthPerChunk;
    }

    if (index >= Array_p->length) {Array_p->length = index + 1;}
   
    return Array_p->bytes_p + (index * Array_p->elementSize);

#include NH_DEFAULT_CHECK
}

void *Nh_getFromArray(
    Nh_Array *Array_p, unsigned long index)
{
NH_BEGIN()
NH_END(_Nh_getFromArray(Array_p, index))
}

void *_Nh_incrementArray(
    Nh_Array *Array_p)
{
    return _Nh_getFromArray(Array_p, Array_p->length);
}

void *Nh_incrementArray(
    Nh_Array *Array_p)
{
NH_BEGIN()
NH_END(_Nh_incrementArray(Array_p))
}

NH_RESULT Nh_removeTailFromArray(
    Nh_Array *Array_p, unsigned int count)
{
NH_BEGIN()

    Array_p->length -= count;

    if (Array_p->length < 0) {Array_p->length = 0;}

    while (Array_p->allocatedLength - Array_p->length >= Array_p->allocatedLengthPerChunk) 
    {
        if (Array_p->allocatedLength - Array_p->allocatedLengthPerChunk == 0) {
            Nh_free(Array_p->bytes_p);
            Array_p->bytes_p = NULL;
        } else {
            Array_p->bytes_p = realloc(Array_p->bytes_p, Array_p->allocatedLength - Array_p->allocatedLengthPerChunk);
            NH_CHECK_NULL(Array_p->bytes_p)
        }
        Array_p->allocatedLength -= Array_p->allocatedLengthPerChunk;
    }

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

NH_RESULT Nh_removeFromArray(
    Nh_Array *Array_p, int index, unsigned int count)
{
NH_BEGIN()

    if (index >= Array_p->length) {NH_END(NH_SUCCESS)}

    if (index + count >= Array_p->length) {
        NH_DIAGNOSTIC_END(Nh_removeTailFromArray(Array_p, count))
    }

    int cpyLength = Array_p->length - index - count;

    void *cpy_p = Nh_allocate(Array_p->elementSize * cpyLength);
    NH_CHECK_MEM(cpy_p)
    memcpy(cpy_p, Nh_getFromArray(Array_p, index + count), Array_p->elementSize * cpyLength);

    NH_CHECK(Nh_removeTailFromArray(Array_p, cpyLength + count))
    NH_CHECK(Nh_appendToArray(Array_p, cpy_p, cpyLength))

    Nh_free(cpy_p);

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

NH_RESULT Nh_insertIntoArray(
    Nh_Array *Array_p, int index, void *elements_p, int length)
{
NH_BEGIN()

    NH_BOOL copy = NH_TRUE;
    if (index >= Array_p->length) {
        copy = NH_FALSE;
        index = Array_p->length;
    }

    int removeLength = Array_p->length - index;

    void *cpy_p = NULL;
    if (copy) {
        cpy_p = Nh_allocate(Array_p->elementSize * removeLength);
        NH_CHECK_MEM(cpy_p)
        memcpy(cpy_p, Nh_getFromArray(Array_p, index), Array_p->elementSize * removeLength);
    }

    NH_CHECK(Nh_removeTailFromArray(Array_p, removeLength))
    NH_CHECK(Nh_appendToArray(Array_p, elements_p, length))

    if (copy) {
        NH_CHECK(Nh_appendToArray(Array_p, cpy_p, removeLength))
        Nh_free(cpy_p);
    }

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

void _Nh_freeArray(
    Nh_Array *Array_p)
{
NH_BEGIN()

    if (Array_p->bytes_p != NULL) {_Nh_free(Array_p->bytes_p);}

    Array_p->allocatedLength = 0;
    Array_p->length = 0;
    Array_p->bytes_p = NULL;
   
NH_SILENT_END()
}

void Nh_freeArray(
    Nh_Array *Array_p)
{
NH_BEGIN()

    _Nh_freeArray(Array_p);

NH_SILENT_END()
}


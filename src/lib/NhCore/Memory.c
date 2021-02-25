// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE ========================================================================================

#include "Memory.h"

#include "Common/Macro.h"
#include NH_FLOW
#include NH_DEFAULT_CHECK

#include <stdlib.h>
#include <stdio.h>

// IMPLEMENT ======================================================================================

void* _Nh_allocate(
    size_t size)
{
    return malloc(size);
}

void* Nh_allocate(
    size_t size)
{
NH_BEGIN()
NH_END(_Nh_allocate(size))
}

void _Nh_free(
    void *ptr)
{
    return free(ptr);
}

void Nh_free(
    void *ptr)
{
NH_BEGIN()

    _Nh_free(ptr);

NH_SILENT_END()
}

NH_RESULT Nh_initMemory()
{
NH_BEGIN()
NH_END(NH_SUCCESS)
}

NH_RESULT Nh_initThreadMemory()
{
NH_BEGIN()
NH_END(NH_SUCCESS)
}


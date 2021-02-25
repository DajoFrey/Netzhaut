#ifndef NH_MEMORY_H
#define NH_MEMORY_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "Common/Result.h"

#include <stddef.h>

#endif

/** @addtogroup NhCoreFunctions
 *  @{
 */

    void *Nh_allocate(
        size_t size
    );

    void *_Nh_allocate(
        size_t size
    );
   
    void Nh_free(
        void *ptr
    );
    
    void _Nh_free(
        void *ptr
    );

    NH_RESULT Nh_initMemory(
    );
    
    NH_RESULT Nh_initThreadMemory(
    );

/** @} */

#endif

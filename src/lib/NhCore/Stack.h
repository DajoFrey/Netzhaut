#ifndef NH_STACK_H
#define NH_STACK_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "List.h"

#include "Common/API.h"
#include "Common/Result.h"

#endif

/** @addtogroup NhCoreTypedefs
 *  @{
 */

    typedef Nh_List Nh_Stack;

/** @} */

/** @addtogroup NhCoreFunctions
 *  @{
 */

    Nh_Stack Nh_initStack(
        int chunkSize
    );
     
    NH_RESULT Nh_pushStack(
        Nh_Stack *Stack_p, void *handle_p
    );
     
    void *Nh_peekStack(
        Nh_Stack *Stack_p
    );
     
    void *Nh_popStack(
        Nh_Stack *Stack_p
    );

/** @} */

#endif 

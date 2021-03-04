// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE ========================================================================================

#include "Stack.h"
#include "Memory.h"

#include "Common/Macro.h"
#include NH_FLOW
#include NH_DEFAULT_CHECK

#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

// STACK ===========================================================================================

Nh_Stack Nh_initStack(
    int chunkSize)
{
NH_BEGIN()
NH_END(Nh_initList(chunkSize))
}
 
NH_RESULT Nh_pushStack(
    Nh_Stack *Stack_p, void *handle_p)
{
NH_BEGIN()
NH_DIAGNOSTIC_END(Nh_appendToList(Stack_p, handle_p)) 
}
 
void *Nh_peekStack(
    Nh_Stack *Stack_p)
{
NH_BEGIN()
NH_END(Stack_p->size <= 0 ? NULL : Stack_p->pp[Stack_p->size - 1])
}
 
void *Nh_popStack(
    Nh_Stack *Stack_p)
{
NH_BEGIN()

    if (Stack_p->size == 0) {NH_END(NULL)}

    void *handle_p = Stack_p->pp[Stack_p->size - 1];
    Nh_removeFromList(Stack_p, NH_FALSE, Stack_p->size - 1);    

NH_END(handle_p)
}


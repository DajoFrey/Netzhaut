// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE ==========================================================================================

#include "Result.h"

// RESULTS =========================================================================================

const char *NH_IO_RESULTS_PP[] = 
{
    "NH_IO_SUCCESS",                    
    "NH_IO_ERROR_NULL_POINTER",         
    "NH_IO_ERROR_BAD_STATE", 
    "NH_IO_ERROR_MEMORY_ALLOCATION",
};

unsigned int NH_IO_RESULTS_PP_COUNT = sizeof(NH_IO_RESULTS_PP) / sizeof(NH_IO_RESULTS_PP[0]);


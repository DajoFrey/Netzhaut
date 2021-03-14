// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE ==========================================================================================

#include "Result.h"

// RESULTS =========================================================================================

const char *NH_WSI_RESULTS_PP[] = 
{
    "NH_WSI_SUCCESS",                    
    "NH_WSI_ERROR_NULL_POINTER",         
    "NH_WSI_ERROR_BAD_STATE", 
    "NH_WSI_ERROR_MEMORY_ALLOCATION",
};

unsigned int NH_WSI_RESULTS_PP_COUNT = sizeof(NH_WSI_RESULTS_PP) / sizeof(NH_WSI_RESULTS_PP[0]);


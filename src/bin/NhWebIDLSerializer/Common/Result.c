// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE ==========================================================================================

#include "Result.h"

// RESULTS =========================================================================================

const char *NH_WEBIDL_SERIALIZER_RESULTS_PP[] = 
{
    "NH_WEBIDL_SERIALIZER_SUCCESS",                    
    "NH_WEBIDL_SERIALIZER_ERROR_NULL_POINTER",         
    "NH_WEBIDL_SERIALIZER_ERROR_BAD_STATE", 
    "NH_WEBIDL_SERIALIZER_ERROR_MEMORY_ALLOCATION",
};

unsigned int NH_WEBIDL_SERIALIZER_RESULTS_PP_COUNT = sizeof(NH_WEBIDL_SERIALIZER_RESULTS_PP) / sizeof(NH_WEBIDL_SERIALIZER_RESULTS_PP[0]);


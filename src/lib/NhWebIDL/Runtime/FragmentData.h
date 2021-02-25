#ifndef NH_WEBIDL_FRAGMENT_DATA_H
#define NH_WEBIDL_FRAGMENT_DATA_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "../Common/Result.h"
#include <stddef.h>

#endif

/** @addtogroup WebIDLData Data
 *  \ingroup WebIDL
 *  @{
 */

    extern const char *NH_WEBIDL_FRAGMENT_NAMES_PP[];
    extern size_t NH_WEBIDL_FRAGMENT_NAMES_PP_COUNT;

    extern unsigned char *NH_WEBIDL_FRAGMENTS_PP[];
    extern size_t NH_WEBIDL_FRAGMENTS_PP_COUNT;

    extern const char *NH_WEBIDL_FRAGMENT_FUNCTION_NAMES_PP[];
    extern size_t NH_WEBIDL_FRAGMENT_FUNCTION_NAMES_PP_COUNT;

    extern Nh_WebIDL_Result (*NH_WEBIDL_FRAGMENT_FUNCTIONS_PP[])(void *This_p, int argc, void *args_pp[]);
    extern size_t NH_WEBIDL_FRAGMENT_FUNCTIONS_PP_COUNT;

/** @} */

#endif 

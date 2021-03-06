#ifndef NH_WEB_BUILTIN_H
#define NH_WEB_BUILTIN_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "../Common/API.h"
#include <stddef.h>

#endif

/** @addtogroup WebVar
 *  @{
 */

    extern NH_BYTE *NH_WEB_FRAGMENT_NAMES_PP[];
    extern size_t NH_WEB_FRAGMENT_NAMES_PP_COUNT;

    extern unsigned NH_BYTE *NH_WEB_FRAGMENTS_PP[];
    extern size_t NH_WEB_FRAGMENTS_PP_COUNT;

    extern const unsigned int NH_WEB_FRAGMENT_LENGTHS_P[];

/** @} */

#endif 

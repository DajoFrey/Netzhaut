#ifndef NH_WEBIDL_BIND_H
#define NH_WEBIDL_BIND_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "../Common/Result.h"

#endif

/** @addtogroup WebIDLStructs Structs
 *  \ingroup WebIDL
 *  @{
 */

    void *Nh_WebIDL_getFragment(
        char *spec_p, char *fragment_p
    );

    Nh_WebIDL_getFragmentFunction(
        char *spec_p, char *fragment_p, char *function_p
    );

/** @} */

#endif

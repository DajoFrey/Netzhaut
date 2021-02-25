#ifndef NH_WEBIDL_DESERIALIZER_H
#define NH_WEBIDL_DESERIALIZER_H

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

    NH_WEBIDL_RESULT Nh_WebIDL_deserializeFragments(
    );

    Nh_WebIDL_Interface *Nh_WebIDL_getInterface(
        char *spec_p, char *fragment_p
    );

//    Nh_WebIDL_Interface *Nh_WebIDL_getOperation(
//        char *spec_p, char *fragment_p, char *operation_p
//    );

/** @} */

#endif

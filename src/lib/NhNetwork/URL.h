#ifndef NH_NET_URL_H
#define NH_NET_URL_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "Common/Result.h"
#include "../NhCore/URI.h"

#include <stddef.h>
#include <stdbool.h>

#endif

/** @addtogroup NetworkDefinitions Definitions
 *  \ingroup Network
 *  @{
 */

    typedef struct Nh_Net_URL Nh_Net_URL;

    typedef NH_NET_RESULT (*Nh_Net_createURL_f)(
        Nh_URI *URI_p
    );

    typedef void (*Nh_Net_freeURL_f)(
        Nh_URI *URI_p
    );

    typedef NH_MEDIA (*Nh_Net_getMediaType_f)(
        Nh_Net_URL *URL_p
    );

    typedef void *(*Nh_Net_getPayloadFromURL_f)(
        Nh_Net_URL *URL_p, size_t *size_p, NH_URI_SCHEME *scheme_p, int index
    );

    typedef void (*Nh_Net_freePayload_f)(
        Nh_Net_URL *URL_p
    );

/** @} */

/** @addtogroup NetworkFunctions Functions
 *  \ingroup Network
 *  @{
 */

    NH_NET_RESULT Nh_Net_createURL(
        Nh_URI *URI_p
    );

    void Nh_Net_freeURL(
        Nh_URI *URI_p
    );
    
    Nh_URI Nh_Net_copyURL(
        Nh_URI *URI_p
    );
    
    bool Nh_Net_equivalentURL(
        Nh_URI URI1, Nh_URI URI2
    );
    
    NH_MEDIA Nh_Net_getMediaType(
        Nh_Net_URL *URL_p
    );
    
    void *Nh_Net_getPayloadFromURL(
        Nh_Net_URL *URL_p, size_t *size_p, NH_URI_SCHEME *scheme_p, int index
    );

    void Nh_Net_freePayload(
        Nh_Net_URL *URL_p
    );

/** @} */

#endif 

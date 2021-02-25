#ifndef NH_CHANNEL_H
#define NH_CHANNEL_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "Common/Result.h"

#include <stddef.h>

#endif

/** @addtogroup NhCoreStructs
 *  @{
 */

    // TODO semaphore etc.
    typedef struct Nh_Channel {
#ifdef __unix__
        int rw_p[2];
#endif
    } Nh_Channel;

/** @} */

/** @addtogroup NhCoreFunctions
 *  @{
 */

    NH_RESULT Nh_initChannel(
        Nh_Channel *Channel_p
    );
    
    NH_RESULT Nh_openChannel(
        Nh_Channel *Channel_p
    );
    
    void Nh_closeChannelReadAccess(
        Nh_Channel *Channel_p
    );
    
    void Nh_closeChannelWriteAccess(
        Nh_Channel *Channel_p
    );
    
    int Nh_writeToChannel(
        Nh_Channel *Channel_p, char *bytes_p, int byteCount
    );
    
    char *Nh_readFromChannel(
        Nh_Channel *Channel_p, size_t *size_p
    );

/** @} */

#endif 

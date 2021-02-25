// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "Channel.h"

#include "Common/Macro.h"
#include NH_FLOW
#include NH_DEFAULT_CHECK

#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// INIT ============================================================================================

NH_RESULT Nh_initChannel(
    Nh_Channel *Channel_p)
{
NH_BEGIN()

#ifdef __unix__
    Channel_p->rw_p[0] = 0;
    Channel_p->rw_p[1] = 0;
#endif

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

NH_RESULT Nh_openChannel(
    Nh_Channel *Channel_p)
{
NH_BEGIN()

#ifdef __unix__
    pipe(Channel_p->rw_p);
#endif

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

void Nh_closeChannelReadAccess(
    Nh_Channel *Channel_p)
{
NH_BEGIN()

#ifdef __unix__
    close(Channel_p->rw_p[0]);
#endif

NH_SILENT_END()
}

void Nh_closeChannelWriteAccess(
    Nh_Channel *Channel_p)
{
NH_BEGIN()

#ifdef __unix__
    close(Channel_p->rw_p[1]);
#endif

NH_SILENT_END()
}

int Nh_writeToChannel(
    Nh_Channel *Channel_p, char *bytes_p, int byteCount)
{
NH_BEGIN()

#ifdef __unix__
    int result = write(Channel_p->rw_p[1], bytes_p, byteCount);
#endif

NH_END(result)
}

char *Nh_readFromChannel(
    Nh_Channel *Channel_p, size_t *size_p)
{
NH_BEGIN()

#ifdef __unix__

    struct timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = 1;

    int r = Channel_p->rw_p[0];

    fd_set set;
    FD_ZERO(&set);
    FD_SET(r, &set);

    int rv = select(r + 1, &set, NULL, NULL, &timeout);
    if (rv > 0)
    {
        int offset = 0;
        char *buff_p = malloc(128);
        memset(buff_p, '\0', 128);
        int result;
        while (result = read(r, buff_p + offset, 128)) {
            if (size_p != NULL) {*size_p += result;}
            if (result < 128) {break;}
            offset += 128;
            buff_p = realloc(buff_p, offset + 128);
            memset(buff_p + offset, '\0', 128);
        }
        NH_END(buff_p);
    }

#endif

NH_END(NULL)
}


// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE ========================================================================================

#include "Network.h"
#include "Socket.h"
#include "HTTP.h"

#include "../NhCore/Memory.h"
#include "../NhCore/String.h"

#include "../NhCore/Common/Macro.h"
#include NH_FLOW
#include NH_CUSTOM_CHECK

#include "Common/Macro.h"
#include NH_NET_FLOW
#include NH_NET_DEFAULT_CHECK

#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <limits.h>

// DATA ============================================================================================

Nh_Net_Network Network;

// INIT ============================================================================================

static void Nh_Net_initNetwork()
{
NH_NET_BEGIN()

    Network.hostName_p = NULL;
    Network.SSL_p = NULL;
    NH_INIT_LINKED_LIST(Network.Sockets)

NH_NET_SILENT_END()
}

static NH_NET_RESULT Nh_Net_getHostName()
{
NH_NET_BEGIN()

    if (Network.hostName_p != NULL) {NH_NET_DIAGNOSTIC_END(NH_NET_SUCCESS)}

#ifdef __unix__

    char hostname_p[2048] = {'\0'};
    gethostname(hostname_p, 2048);
    Network.hostName_p = Nh_allocateBytes(hostname_p);

#endif

NH_NET_DIAGNOSTIC_END(NH_NET_SUCCESS)
}

NH_NET_RESULT Nh_Net_createNetwork()
{
NH_NET_BEGIN()

    Nh_Net_initNetwork();

    NH_CHECK(NH_NET_ERROR_BAD_STATE, 
        Nh_appendToLinkedList(&Network.Sockets, Nh_Net_createClientSocket(NH_NET_PROTOCOL_FAMILY_IPV4, NH_NET_SOCKET_TCP)
    ))
    NH_CHECK(NH_NET_ERROR_BAD_STATE, 
        Nh_appendToLinkedList(&Network.Sockets, Nh_Net_createClientSocket(NH_NET_PROTOCOL_FAMILY_IPV6, NH_NET_SOCKET_TCP)
    ))

    NH_NET_CHECK(Nh_Net_getHostName())

NH_NET_DIAGNOSTIC_END(NH_NET_SUCCESS)
}

Nh_Net_Network *Nh_Net_getNetwork()
{
NH_NET_BEGIN()
NH_NET_END(&Network)
}


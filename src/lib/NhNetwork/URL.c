// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE ========================================================================================

#include "URL.h"
#include "Socket.h"
#include "HTTP.h"
#include "HTTP.h"
#include "Network.h"

#include "../NhCore/Memory.h"
#include "../NhCore/String.h"
#include "../NhCore/MediaType.h"

#include "Common/Macro.h"
#include NH_NET_FLOW
#include NH_NET_DEFAULT_CHECK

#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <limits.h>

// DECLARE =========================================================================================

typedef enum NH_NET_PAYLOAD {
    NH_NET_PAYLOAD_HTTP,
} NH_NET_PAYLOAD;

typedef struct Nh_Net_Payload {
    NH_NET_PAYLOAD type;
    Nh_Net_HTTPPayload HTTP;
} Nh_Net_Payload;

/**
 * Standard: https://url.spec.whatwg.org/
 */
typedef struct Nh_Net_URL {
    char *hostName_p;
    char *resolvedHostName_p;
    char *pathName_p;
    char *search_p;
    char *hash_p;
    Nh_LinkedList Sockets;
    struct Connection {
        Nh_Net_ClientSocket *Socket_p;
        NH_NET_PORT port;
    } Connection;
    struct Nh_Net_URL *Forward_p;
    Nh_Net_Payload Payload;
} Nh_Net_URL;

static Nh_Net_Payload Nh_Net_getGenericPayloadFromURL(
    Nh_Net_URL *URL_p, size_t *size_p, NH_URI_SCHEME *scheme_p, int index
);

// INIT ============================================================================================

static void Nh_Net_initURL(
    Nh_Net_URL *URL_p)
{
NH_NET_BEGIN()

    URL_p->hostName_p = NULL;
    URL_p->resolvedHostName_p = NULL;
    URL_p->pathName_p = NULL;
    URL_p->search_p = NULL;
    URL_p->hash_p = NULL;
    URL_p->Forward_p = NULL;

    URL_p->Connection.port = -1;
    URL_p->Connection.Socket_p = NULL;

    URL_p->Payload.type = -1;
    URL_p->Payload.HTTP.body_p = NULL;
    URL_p->Payload.HTTP.header_p = NULL;

    NH_INIT_LINKED_LIST(URL_p->Sockets)

NH_NET_SILENT_END()
}

// CREATE ==========================================================================================

static Nh_LinkedList Nh_Net_getHostSockets(
    char *hostName_p, char *resolvedHostName_p, NH_URI_SCHEME scheme)
{
NH_NET_BEGIN()

#include NH_NET_CUSTOM_CHECK

    Nh_LinkedList Sockets;
    NH_INIT_LINKED_LIST(Sockets)

    switch (scheme)
    {
        case NH_URI_SCHEME_HTTP      : NH_NET_CHECK(Sockets, Nh_Net_getSockets(hostName_p, resolvedHostName_p, NH_NET_PORT_HTTP, &Sockets)) break;
        case NH_URI_SCHEME_HTTPS     : NH_NET_CHECK(Sockets, Nh_Net_getSockets(hostName_p, resolvedHostName_p, NH_NET_PORT_HTTPS, &Sockets)) break;
        case NH_URI_SCHEME_UNDEFINED :
        {
            NH_NET_CHECK(Sockets, Nh_Net_getSockets(hostName_p, resolvedHostName_p, NH_NET_PORT_HTTP, &Sockets))
            // TODO maybe try others
            break;
        }
    }

#include NH_NET_DEFAULT_CHECK

NH_NET_END(Sockets)
}

static NH_URI_SCHEME Nh_Net_parseURL(
    char *URL_p, char *hostName_p, char *pathName_p)
{
NH_NET_BEGIN()

    NH_URI_SCHEME scheme = NH_URI_SCHEME_UNDEFINED;

    if (URL_p[0] == 'h' && URL_p[1] == 't' && URL_p[2] == 't' && URL_p[3] == 'p' && URL_p[5] == ':') {
        scheme = NH_URI_SCHEME_HTTP;
    }
    if (URL_p[0] == 'h' && URL_p[1] == 't' && URL_p[2] == 't' && URL_p[3] == 'p' && URL_p[4] == 's' && URL_p[5] == ':') {
        scheme = NH_URI_SCHEME_HTTPS;
    }

    if (scheme != NH_URI_SCHEME_UNDEFINED) { // skip over scheme 
        while (URL_p[0] != ':') {URL_p = URL_p + 1;}
        while (URL_p[0] == ':' || URL_p[0] == '/') {URL_p = URL_p + 1;}
    }

    int pathBegin = 0;
    for (pathBegin = 0; URL_p[pathBegin] != '/' && URL_p[pathBegin] != '\0'; ++pathBegin);
    strcpy(pathName_p, &URL_p[pathBegin]);

    char revert = URL_p[pathBegin];    
    URL_p[pathBegin] = '\0'; // cut off path
    strcpy(hostName_p, URL_p);
    URL_p[pathBegin] = revert;

NH_NET_END(scheme)
}

static Nh_Net_URL *Nh_Net_createURLFromLocation(
    char *location_p, NH_URI_SCHEME *scheme_p)
{
NH_NET_BEGIN()

#include NH_NET_CUSTOM_CHECK

    char path_p[1024] = {'\0'};
    char hostName_p[1024] = {'\0'};

    NH_URI_SCHEME scheme = Nh_Net_parseURL(location_p, hostName_p, path_p);
    if (scheme_p != NULL) {*scheme_p = scheme;}

    char resolvedHostName_p[1024] = {'\0'};
    Nh_LinkedList Sockets = Nh_Net_getHostSockets(hostName_p, resolvedHostName_p, scheme);
    if (Sockets.count <= 0) {NH_NET_END(NULL)}

    Nh_Net_URL *URL_p = Nh_allocate(sizeof(Nh_Net_URL));
    NH_NET_CHECK_MEM(NULL, URL_p)

    Nh_Net_initURL(URL_p);
    URL_p->Sockets = Sockets;

    URL_p->hostName_p = Nh_allocateBytes(hostName_p);
    NH_NET_CHECK_MEM(NULL, URL_p->hostName_p)

    URL_p->resolvedHostName_p = Nh_allocateBytes(resolvedHostName_p);
    NH_NET_CHECK_MEM(NULL, URL_p->resolvedHostName_p)

    URL_p->pathName_p = Nh_allocateBytes(path_p);
    NH_NET_CHECK_MEM(NULL, URL_p->pathName_p)

#include NH_NET_DEFAULT_CHECK

NH_NET_END(URL_p)
}

NH_NET_RESULT Nh_Net_createURL(
    Nh_URI *URI_p)
{
NH_NET_BEGIN()

    // TODO URI_p->base ignored

    URI_p->data_p = Nh_Net_createURLFromLocation(URI_p->location_p, &URI_p->scheme);
    NH_NET_CHECK_MEM(URI_p->data_p)

NH_NET_DIAGNOSTIC_END(NH_NET_SUCCESS)
}

// FREE ============================================================================================

void Nh_Net_freeURL(
    Nh_URI *URI_p)
{
NH_NET_BEGIN()

NH_NET_SILENT_END()
}

// COPY ============================================================================================

Nh_URI Nh_Net_copyURL(
    Nh_URI *URI_p)
{
}

// COMPARE =========================================================================================

bool Nh_Net_equivalentURL(
    Nh_URI URI1, Nh_URI URI2)
{
}

// DATA FORMAT =====================================================================================

NH_MEDIA Nh_Net_getMediaType(
    Nh_Net_URL *URL_p)
{
NH_NET_BEGIN()

#include NH_NET_CUSTOM_CHECK

     if (URL_p == NULL) {NH_NET_END(-1)}

     switch (URL_p->Payload.type)
     {
         case NH_NET_PAYLOAD_HTTP :
         {
             char headerValue_p[512] = {'\0'};
             NH_NET_CHECK(-1, Nh_Net_getHTTPHeaderValue(
                 URL_p->Payload.HTTP.header_p, headerValue_p, NH_NET_HTTP_HEADER_CONTENT_TYPE
             ))

             for (int i = 0; i < strlen(headerValue_p); ++i) {
                 if (headerValue_p[i] == ';') {
                     headerValue_p[i] = '\0'; break;
                 }
             }

             NH_NET_END(Nh_getMediaTypeFromTemplate(headerValue_p))
             break;
         }
     }

#include NH_NET_DEFAULT_CHECK

NH_NET_END(-1)
}

// GET DATA ========================================================================================

#include NH_NET_CUSTOM_CHECK

static Nh_Net_Data Nh_Net_getDataFromHost(
    Nh_Net_URL *URL_p, size_t *size_p, int *index_p)
{
NH_NET_BEGIN()

    Nh_Net_Data Data = Nh_Net_initData();

    for (int i = *index_p; i < URL_p->Sockets.count; ++i, *index_p += 1)
    {
        Nh_Net_Socket *Socket_p = Nh_getFromLinkedList(&URL_p->Sockets, i);
        Nh_Net_ClientSocket *ClientSocket_p = Nh_Net_getClientSocket(*Socket_p);

        if (ClientSocket_p != NULL && Nh_Net_connect(URL_p->hostName_p, ClientSocket_p, *Socket_p) == NH_NET_SUCCESS) 
        {
            Data = Nh_Net_getDataFromSocket(
                ClientSocket_p, URL_p->hostName_p, URL_p->pathName_p, Socket_p->port
            );
            if (Data.size > 0 && Data.data_p != NULL) {
                *index_p += 1; 
                NH_NET_END(Data)
            }
        }
    }

NH_NET_END(Data)
}

static Nh_Net_Payload Nh_Net_handleHTTPResponse(
    Nh_Net_URL *URL_p, size_t *size_p, int index, void *data_p, NH_URI_SCHEME *scheme_p)
{
NH_NET_BEGIN()

    Nh_Net_Payload Payload;
    Payload.type = -1;

    switch (Nh_Net_getHTTPResponseCode(data_p))
    {
        case NH_NET_HTTP_RESPONSE_MOVED_PERMANENTLY :  
        {
            char headerValue_p[512] = {'\0'};
            NH_NET_CHECK(Payload, Nh_Net_getHTTPHeaderValue(data_p, headerValue_p, NH_NET_HTTP_HEADER_LOCATION))

            Nh_free(URL_p->Forward_p);
            URL_p->Forward_p = Nh_Net_createURLFromLocation(headerValue_p, NULL);
            NH_NET_CHECK_NULL(Payload, URL_p->Forward_p)

            Nh_free(data_p);
            NH_NET_END(Nh_Net_getGenericPayloadFromURL(URL_p->Forward_p, size_p, scheme_p, 0))
        }
        case NH_NET_HTTP_RESPONSE_FOUND : // TODO temporary forward
        {
            char headerValue_p[512] = {'\0'};
            NH_NET_CHECK(Payload, Nh_Net_getHTTPHeaderValue(data_p, headerValue_p, NH_NET_HTTP_HEADER_LOCATION))

            URL_p->Forward_p = Nh_Net_createURLFromLocation(headerValue_p, NULL);
            NH_NET_CHECK_NULL(Payload, URL_p->Forward_p)

            Nh_free(data_p);
            NH_NET_END(Nh_Net_getGenericPayloadFromURL(URL_p->Forward_p, size_p, scheme_p, 0))
        }
        case NH_NET_HTTP_RESPONSE_BAD_GATEWAY : 
        {
            Nh_free(data_p);
            NH_NET_END(Nh_Net_getGenericPayloadFromURL(URL_p, size_p, scheme_p, index)) // try next socket
        }
        case NH_NET_HTTP_RESPONSE_MISDIRECTED_REQUEST :
        {
            break;
        }
        case NH_NET_HTTP_RESPONSE_BAD_REQUEST :
        {
            Nh_free(data_p);
            NH_NET_END(Nh_Net_getGenericPayloadFromURL(URL_p, size_p, scheme_p, index)) // try next socket
        }
        case NH_NET_HTTP_RESPONSE_OK : 
        {
            Payload.type = NH_NET_PAYLOAD_HTTP;
            Payload.HTTP = Nh_Net_convertToHTTPPayload(data_p);
            Nh_free(data_p);
            break;
        }
        default : printf("unhandled HTTP response code!"); exit(0);
    }

NH_NET_END(Payload)
}

static Nh_Net_Payload Nh_Net_getGenericPayloadFromURL(
    Nh_Net_URL *URL_p, size_t *size_p, NH_URI_SCHEME *scheme_p, int index)
{
NH_NET_BEGIN()

    Nh_Net_Payload Payload;
    Payload.type = -1;

    Nh_Net_Data Data = Nh_Net_getDataFromHost(URL_p, size_p, &index);
    if (Data.size == 0 || Data.data_p == NULL) {NH_NET_END(Payload)}

    switch (Data.port)
    {
        case NH_NET_PORT_HTTP  : 
        {
            if (scheme_p != NULL) {*scheme_p = NH_URI_SCHEME_HTTP;}
            NH_NET_END(Nh_Net_handleHTTPResponse(URL_p, size_p, index, Data.data_p, scheme_p))
            break;
        }
        case NH_NET_PORT_HTTPS : 
        {
            if (scheme_p != NULL) {*scheme_p = NH_URI_SCHEME_HTTPS;}
            NH_NET_END(Nh_Net_handleHTTPResponse(URL_p, size_p, index, Data.data_p, scheme_p))
            break;
        }
    }

NH_NET_END(Payload)
}

void *Nh_Net_getPayloadFromURL(
    Nh_Net_URL *URL_p, size_t *size_p, NH_URI_SCHEME *scheme_p, int index)
{
NH_NET_BEGIN()

    URL_p->Payload = Nh_Net_getGenericPayloadFromURL(URL_p, size_p, scheme_p, index);

    switch (URL_p->Payload.type)
    {
        case NH_NET_PAYLOAD_HTTP : NH_NET_END(URL_p->Payload.HTTP.body_p)
    }

NH_NET_END(NULL)
}

// FREE ============================================================================================

void Nh_Net_freePayload(
    Nh_Net_URL *URL_p)
{
NH_NET_BEGIN()

    Nh_free(URL_p->Payload.HTTP.body_p);
    Nh_free(URL_p->Payload.HTTP.header_p);

NH_NET_SILENT_END()
}


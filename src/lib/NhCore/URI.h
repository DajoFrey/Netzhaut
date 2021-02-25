#ifndef NH_URI_H
#define NH_URI_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "MediaType.h"

#include "Common/Result.h"
#include "Common/API.h"

#include <stddef.h>
#include <stdbool.h>

#endif

/** @addtogroup NhCoreEnums
 *  @{
 */

    typedef enum NH_URI_SCHEME {
        NH_URI_SCHEME_FILE, 	/**<https://tools.ietf.org/html/rfc8089*/             
        NH_URI_SCHEME_HTTP, 	                
        NH_URI_SCHEME_HTTPS, 	                
        NH_URI_SCHEME_INTERNAL_URN,   
        NH_URI_SCHEME_INTERNAL_URL, 
        NH_URI_SCHEME_UNDEFINED,
    } NH_URI_SCHEME;

    typedef enum NH_INTERNAL_FILE {
        NH_INTERNAL_FILE_SELECT_OPTION,
        NH_INTERNAL_FILE_SELECT_TEXT,
        NH_INTERNAL_FILE_SCROLL,
        NH_INTERNAL_FILE_MISC,
        NH_INTERNAL_FILE_HEADER,
        NH_INTERNAL_FILE_LIST,
        NH_INTERNAL_FILE_EXPAND,
        NH_INTERNAL_FILE_DISC,
        NH_INTERNAL_FILE_CIRCLE,
        NH_INTERNAL_FILE_SQUARE,
        NH_INTERNAL_FILE_INDEX,
        NH_INTERNAL_FILE_BACKGROUND,
        NH_INTERNAL_FILE_BORDER,
        NH_INTERNAL_FILE_CANVAS,
        NH_INTERNAL_FILE_IMAGE,
        NH_INTERNAL_FILE_INPUT, 
        NH_INTERNAL_FILE_SELECTOR,
        NH_INTERNAL_FILE_TEXT,
    } NH_INTERNAL_FILE;

    typedef enum NH_INTERNAL_URL {
        NH_INTERNAL_URL_HTML_NODE_TEXT,
        NH_INTERNAL_URL_HTML_NODE_ATTRIBUTE_VALUE,
    } NH_INTERNAL_URL;

/** @} */

/** @addtogroup NhCoreStructs
 *  @{
 */

    /**
     * Standard: https://tools.ietf.org/html/rfc3986
     */
    typedef struct Nh_URI {
        NH_URI_SCHEME scheme; 
        long size;
        void *data_p;
        void *payload_p;
        char *location_p;
        char *base_p;
    } Nh_URI;

/** @} */

/** @addtogroup NhCoreFunctions
 *  @{
 */

    int Nh_addInternalURNSyntax(
        char *syntax_p
    );

    NH_RESULT Nh_createInternalURNSyntax(
    );

    Nh_URI Nh_createURI(
        char *location_p, char *base_p, void *pointer, NH_INTERNAL_URL type
    );

    Nh_URI Nh_copyURI(
        Nh_URI *URI_p
    );

    void Nh_freeURI(
        Nh_URI *URI_p
    );

    void *Nh_getPayload(
        Nh_URI *URI_p
    );

    void Nh_freePayload(
       Nh_URI *URI_p
    );

    NH_BOOL Nh_correspondingURI(
        Nh_URI *URI_p, char *location_p, char *base_p
    );

    NH_BOOL Nh_equivalentURI(
        Nh_URI *URI1_p, Nh_URI *URI2_p
    );

    NH_MEDIA Nh_getMediaType(
        Nh_URI *URI_p
    );

    void Nh_stringifyURI(
        Nh_URI *URI_p, char *set_p
    );

/** @} */

#endif 

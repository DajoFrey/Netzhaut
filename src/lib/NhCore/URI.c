// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE ========================================================================================

#include "URI.h"
#include "Memory.h"
#include "String.h"
#include "File.h"
#include "Config.h"

#include "Common/Macro.h"
#include NH_FLOW
#include NH_DEFAULT_CHECK

#include "../NhNetwork/URL.h"

#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <limits.h>

// DATA ============================================================================================

static const char *syntax_pp[] =
{
    "test:html:index",
    "test:html:border",
    "test:html:image",
    "test:html:input",
    "test:html:canvas",
    "test:html:text",
    "test:html:background",
    "test:html:list",
    "test:media:jpg",
    "icon:expand",
    "icon:disc",
    "icon:circle",
    "icon:square",
    "css:list",
    "css:selectoption",
    "css:misc",
    "css:header",
    "js:selectoption",
    "js:scroll",
    "js:textselection",
    "js:pseudo",
    "win:$",
    "tab:$",
    "win:$:tab:$",
};

// DECLARE =========================================================================================

typedef struct Nh_InternalURN {
    int syntaxIndex;
    int indices_p[10];
} Nh_InternalURN;

typedef struct Nh_InternalURL {
    NH_INTERNAL_URL type;
    void *pointer;
} Nh_InternalURL;

typedef struct Nh_FileURL {
    char *authority_p; /**<Currently always NULL*/ 
    char *path_p;
} Nh_FileURL;

static NH_MEDIA Nh_getInternalURNMediaType(
    int syntaxIndex
);

static char *Nh_getPayloadFromInternalURN(
    Nh_InternalURN *URN_p, long *size_p
);
static char *Nh_getPayloadFromInternalURL(
    Nh_InternalURL *URL_p
);

// DEFAULT =========================================================================================

static Nh_URI Nh_getDefaultURI()
{
    Nh_URI URI;
    URI.scheme     = -1;
    URI.size       = 0;
    URI.data_p     = NULL;
    URI.payload_p  = NULL;
    URI.location_p = NULL;
    URI.base_p     = NULL;
    return URI;
}

// CREATE ==========================================================================================

static NH_RESULT Nh_createFileURL(
    Nh_URI *URI_p)
{
NH_BEGIN()

    URI_p->scheme = NH_URI_SCHEME_FILE;
    URI_p->data_p = Nh_allocate(sizeof(Nh_FileURL));
    NH_CHECK_MEM(URI_p->data_p)

    Nh_FileURL *URL_p = URI_p->data_p;
    URL_p->authority_p = NULL;
    URL_p->path_p = NULL;

    char path_p[2048] = {'\0'};
    NH_CHECK(Nh_getFilePath(URI_p->location_p, URI_p->base_p, path_p))

    URL_p->path_p = Nh_allocateBytes(path_p);
    NH_CHECK_MEM(URL_p->path_p)

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

static NH_RESULT Nh_createInternalURN(
    Nh_URI *URI_p)
{
NH_BEGIN()

    char *location_p = URI_p->location_p + 3; // jump over "nh:"

    URI_p->scheme = NH_URI_SCHEME_INTERNAL_URN;
    URI_p->data_p = Nh_allocate(sizeof(Nh_InternalURN));
    NH_CHECK_MEM(URI_p->data_p)

    Nh_InternalURN *URN_p = URI_p->data_p;
    URN_p->syntaxIndex = -1;
    memset(URN_p->indices_p, 0, sizeof(int) * 10);

    for (int i = 0; i < NH_CONFIG.InternalURNSyntax.count; ++i) 
    {
        bool hit = true;

        char parts_p[512] = {'\0'};
        strcpy(parts_p, location_p);

        char syntax_p[512] = {'\0'};
        strcpy(syntax_p, Nh_getFromLinkedList(&NH_CONFIG.InternalURNSyntax, i));
        
        char *parts_pp[100] = {NULL};
        char *part_p = strtok(parts_p, ":");

        int index = 0;
        while (part_p != NULL) {
            parts_pp[index++] = part_p;
            part_p = strtok(NULL, ":");
        }

        char *syntaxPart_p = strtok(syntax_p, ":");
        
        index = 0;
        int partIndex = 0;
        while (syntaxPart_p != NULL) 
        {
            if (parts_pp[partIndex] == NULL) {hit = false; break;}
            if (strcmp(syntaxPart_p, parts_pp[partIndex]) != 0) 
            {
                if (syntaxPart_p[0] == '$') 
                {
                    char *end_p;
                    long int value = strtol(parts_pp[partIndex], &end_p, 10);
                    if (end_p == parts_pp[partIndex]) {hit = false; break;}
                    URN_p->indices_p[index++] = value;
                }
                else {hit = false;}
            } 
            partIndex++;
            syntaxPart_p = strtok(NULL, ":");
        }

        if (hit) {
            URN_p->syntaxIndex = i;
            break;
        }
    }

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

static NH_RESULT Nh_createInternalURL(
    Nh_URI *URI_p, void *pointer, NH_INTERNAL_URL type)
{
NH_BEGIN()

    URI_p->scheme = NH_URI_SCHEME_INTERNAL_URL;
    URI_p->data_p = Nh_allocate(sizeof(Nh_InternalURL));
    NH_CHECK_MEM(URI_p->data_p)

    Nh_InternalURL *URL_p = URI_p->data_p;
    URL_p->pointer = pointer;
    URL_p->type = type;

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

static char *Nh_cleanURIString(
    char *string_p)
{
NH_BEGIN()

    while (string_p[0] == ' ' || string_p[0] == '\t') {
        string_p = string_p + 1; // remove leading spaces or tabs
    }
    while (string_p[strlen(string_p) - 1] == ' ' || string_p[strlen(string_p) - 1] == '\t') {
        string_p[strlen(string_p) - 1] = '\0'; // remove trailing spaces
    }

NH_END(string_p)
}

static char *Nh_allocateURIString(
    char *string_p)
{
NH_BEGIN()

    if (string_p == NULL) {NH_END(NULL)}

    char cpy_p[2048] = {'\0'};
    strcpy(cpy_p, string_p);
    string_p = Nh_cleanURIString(cpy_p);

NH_END(Nh_allocateBytes(string_p))
}

Nh_URI Nh_createURI(
    char *location_p, char *base_p, void *pointer, NH_INTERNAL_URL type)
{
NH_BEGIN()

#include NH_CUSTOM_CHECK

    Nh_URI URI = Nh_getDefaultURI();

    if (location_p != NULL) {
        URI.location_p = Nh_allocateURIString(location_p);
        NH_CHECK_MEM(URI, URI.location_p)
    }

    if (base_p != NULL){
        URI.base_p = Nh_allocateURIString(base_p);
        NH_CHECK_MEM(URI, URI.base_p)
    }

    if (pointer != NULL) {
        NH_CHECK(URI, Nh_createInternalURL(&URI, pointer, type))
    }
    else if (URI.location_p != NULL && URI.location_p[0] == 'n' && URI.location_p[1] == 'h' && URI.location_p[2] == ':') {
        NH_CHECK(URI, Nh_createInternalURN(&URI))
    }  
    else if (URI.location_p != NULL && Nh_fileExistsOnMachine(URI.location_p, URI.base_p)) {
        NH_CHECK(URI, Nh_createFileURL(&URI))
    }
//    else if (URI.location_p != NULL && Nh_isLoaded("NhNetwork")) {
//        NH_CHECK(URI, ((Nh_Net_createURL_f)Nh_loadFunction("NhNetwork", "Nh_Net_createURL"))(&URI))
//    }

#include NH_DEFAULT_CHECK

NH_END(URI)
}

// FREE ============================================================================================

void Nh_freeURI(
    Nh_URI *URI_p)
{
NH_BEGIN()

    Nh_freePayload(URI_p);

    switch (URI_p->scheme)
    {
        case NH_URI_SCHEME_HTTP  :
        case NH_URI_SCHEME_HTTPS : 

//            if (Nh_isLoaded("NhNetwork")) {((Nh_Net_freeURL_f)Nh_loadFunction("NhNetwork", "Nh_Net_freeURL"))(URI_p);} 
            break;

        default : Nh_free(URI_p->data_p); break;
    }

    Nh_free(URI_p->location_p);
    Nh_free(URI_p->base_p);

NH_SILENT_END()
}

// SYNTAX ==========================================================================================

NH_RESULT Nh_createInternalURNSyntax()
{
NH_BEGIN()

    for (int i = 0; i < (sizeof(syntax_pp) / sizeof(syntax_pp[0])); ++i) {
        if (Nh_addInternalURNSyntax((char*)syntax_pp[i]) == -1) {NH_DIAGNOSTIC_END(NH_ERROR_BAD_STATE)}
    }

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

int Nh_addInternalURNSyntax(
    char *syntax_p)
{
NH_BEGIN()

    if (Nh_appendToLinkedList(&NH_CONFIG.InternalURNSyntax, Nh_allocateBytes(syntax_p)) != NH_SUCCESS) {
        NH_END(-1)
    }
  
NH_END(NH_CONFIG.InternalURNSyntax.count - 1)
}

// COPY ============================================================================================

Nh_URI Nh_copyURI(
    Nh_URI *URI_p)
{
NH_BEGIN()

    Nh_URI URI = Nh_getDefaultURI();

    switch (URI_p->scheme)
    {
        case NH_URI_SCHEME_INTERNAL_URN : 
        {
            printf("copy");exit(0);
        }
        case NH_URI_SCHEME_INTERNAL_URL : 
        {
            printf("copy");exit(0);
        }
        case NH_URI_SCHEME_FILE         : 
        {
            Nh_FileURL *Old_p = URI_p->data_p;
            Nh_FileURL *New_p = Nh_allocate(sizeof(Nh_FileURL));

            URI.data_p = New_p;
            URI.scheme = NH_URI_SCHEME_FILE;

            New_p->path_p = NULL;
            New_p->authority_p = NULL;

            if (Old_p->path_p != NULL) {
                New_p->path_p = Nh_allocate(sizeof(char) * (strlen(Old_p->path_p) + 1));
                strcpy(New_p->path_p, Old_p->path_p);
            }
            if (Old_p->authority_p != NULL) {
                New_p->authority_p = Nh_allocate(sizeof(char) * (strlen(Old_p->authority_p) + 1));
                strcpy(New_p->authority_p, Old_p->authority_p);
            }
        }
    }

NH_END(URI)
}

// COMPARE =========================================================================================

NH_BOOL Nh_correspondingURI(
    Nh_URI *URI_p, char *location_p, char *base_p)
{
NH_BEGIN()

    char cpy1_p[2048] = {'\0'};
    char cpy2_p[2048] = {'\0'};

    if (location_p != NULL && URI_p->location_p != NULL) {
        strcpy(cpy1_p, location_p);
        location_p = Nh_cleanURIString(cpy1_p);
        if (strcmp(URI_p->location_p, location_p)) {NH_END(NH_FALSE)}
    }
    else if (URI_p->location_p != NULL || location_p != NULL) {NH_END(NH_FALSE)}

    // TODO base could be problematic

    if (base_p != NULL && URI_p->base_p != NULL) {
        strcpy(cpy2_p, base_p);
        base_p = Nh_cleanURIString(cpy2_p);
        if (strcmp(URI_p->base_p, base_p)) {NH_END(NH_FALSE)}
    }
    else if (URI_p->base_p != NULL || base_p != NULL) {NH_END(NH_FALSE)}

NH_DIAGNOSTIC_END(NH_TRUE)
}

NH_BOOL Nh_equivalentURI(
    Nh_URI *URI1_p, Nh_URI *URI2_p)
{
NH_BEGIN()

    if (URI1_p->scheme != URI2_p->scheme) {NH_DIAGNOSTIC_END(NH_FALSE)}

    switch (URI1_p->scheme)
    {
        case NH_URI_SCHEME_INTERNAL_URN :
        {
            Nh_InternalURN *Tmp1_p = URI1_p->data_p, *Tmp2_p = URI2_p->data_p;
            if (Tmp1_p->syntaxIndex != Tmp2_p->syntaxIndex) {NH_DIAGNOSTIC_END(NH_FALSE)}
            for (int i = 0; i < 10; ++i) {if (Tmp1_p->indices_p[i] != Tmp2_p->indices_p[i]) {NH_DIAGNOSTIC_END(NH_FALSE)}}
            NH_DIAGNOSTIC_END(NH_TRUE)
        }
        case NH_URI_SCHEME_FILE :
        {
            Nh_FileURL *URL1_p = URI1_p->data_p, *URL2_p = URI2_p->data_p;
            if (URL1_p->path_p == NULL && URL2_p->path_p == NULL) {NH_DIAGNOSTIC_END(NH_TRUE)}
            if (URL1_p->path_p != NULL && URL2_p->path_p != NULL && !strcmp(URL1_p->path_p, URL2_p->path_p)) {NH_DIAGNOSTIC_END(NH_TRUE)}
            break;
        }
       
        default : break;
    }

NH_DIAGNOSTIC_END(NH_FALSE)
}

// MEDIA TYPE ======================================================================================

NH_MEDIA Nh_getMediaType(
    Nh_URI *URI_p)
{
NH_BEGIN()

    switch (URI_p->scheme)
    {
        case NH_URI_SCHEME_UNDEFINED :
        case NH_URI_SCHEME_HTTP : 
        case NH_URI_SCHEME_HTTPS : 
        {
//            if (Nh_isLoaded("NhNetwork")) {
//                Nh_getPayload(URI_p);
//                NH_END(((Nh_Net_getMediaType_f)Nh_loadFunction("NhNetwork", "Nh_Net_getMediaType"))(URI_p->data_p))
//            }
            break;
        }
        case NH_URI_SCHEME_INTERNAL_URL : 
        {
            NH_END(-1) 
        }
        case NH_URI_SCHEME_INTERNAL_URN :
        {
            NH_END(Nh_getInternalURNMediaType(((Nh_InternalURN*)URI_p->data_p)->syntaxIndex))
        }
        case NH_URI_SCHEME_FILE : 
        {
            NH_END(Nh_getMediaTypeFromPath(((Nh_FileURL*)URI_p->data_p)->path_p))
        }
    }

NH_END(-1)
}

static NH_MEDIA Nh_getInternalURNMediaType(
    int syntaxIndex)
{
NH_BEGIN()

    switch (syntaxIndex)
    {
        case  0:
        case  1:
        case  2: 
        case  3: 
        case  4: 
        case  5: 
        case  6: 
        case  7: NH_DIAGNOSTIC_END(NH_MEDIA_TEXT_HTML) 
        case  8: NH_DIAGNOSTIC_END(NH_MEDIA_IMAGE_JPEG)
        case  9: 
        case 10: 
        case 11: 
        case 12: NH_DIAGNOSTIC_END(NH_MEDIA_IMAGE_PNG)
        case 13: 
        case 14: 
        case 15: 
        case 16: NH_DIAGNOSTIC_END(NH_MEDIA_TEXT_CSS) 
        case 17: 
        case 18: 
        case 19:  
        case 20: NH_DIAGNOSTIC_END(NH_MEDIA_TEXT_JAVASCRIPT)
        case 21: break;
        case 22: break;
        case 23: break;
    }

NH_END(-1)
}

// PAYLOAD =========================================================================================

void *Nh_getPayload(
    Nh_URI *URI_p)
{
NH_BEGIN()

    if (URI_p->payload_p != NULL) {NH_END(URI_p->payload_p)}

    void *payload_p = NULL;

    switch (URI_p->scheme)
    {
        case NH_URI_SCHEME_HTTP         : 
        case NH_URI_SCHEME_HTTPS        : 
        case NH_URI_SCHEME_UNDEFINED    : 

//            if (Nh_isLoaded("NhNetwork")) {
//                payload_p = ((Nh_Net_getPayloadFromURL_f)Nh_loadFunction("NhNetwork", "Nh_Net_getPayloadFromURL"))(URI_p->data_p, &URI_p->size, &URI_p->scheme, 0); 
//            }
            break;

        case NH_URI_SCHEME_INTERNAL_URN : payload_p = Nh_getPayloadFromInternalURN(URI_p->data_p, &URI_p->size); break;
        case NH_URI_SCHEME_INTERNAL_URL : payload_p = Nh_getPayloadFromInternalURL(URI_p->data_p); break;
        case NH_URI_SCHEME_FILE         : payload_p = Nh_getFileData(((Nh_FileURL*)URI_p->data_p)->path_p, &URI_p->size); break;
        default : break;
    }

    URI_p->payload_p = payload_p;

NH_END(payload_p)
}

void Nh_freePayload(
    Nh_URI *URI_p)
{
NH_BEGIN()

    if (URI_p->payload_p == NULL) {NH_SILENT_END()}

    switch (URI_p->scheme)
    {
        case NH_URI_SCHEME_FILE  : Nh_free(URI_p->payload_p); break;
        case NH_URI_SCHEME_HTTP  :
        case NH_URI_SCHEME_HTTPS : 

//            if (Nh_isLoaded("NhNetwork")) {
//                ((Nh_Net_freePayload_f)Nh_loadFunction("NhNetwork", "Nh_Net_freePayload"))(URI_p->data_p);
//            } 
            break;

        default : break;
    }

    URI_p->payload_p = NULL;
    URI_p->size = 0;

NH_SILENT_END()
}

static char *Nh_getPayloadFromInternalURN(
    Nh_InternalURN *URN_p, long *size_p)
{
NH_BEGIN()

    switch (URN_p->syntaxIndex)
    {
//        case  0: NH_END(IndexHTML)
//        case  1: NH_END(BorderHTML)
//        case  2: NH_END(ImageHTML)
//        case  3: NH_END(InputHTML)
//        case  4: NH_END(CanvasHTML)
//        case  5: NH_END(TextHTML)
//        case  6: NH_END(BackgroundHTML)
//        case  7: NH_END(ListHTML)
//        case  8: *size_p = JPG_len; NH_END(JPG)
//        case  9: *size_p = ExpandPNG_len; NH_END(ExpandPNG)
//        case 10: *size_p = DiscPNG_len; NH_END(DiscPNG)
//        case 11: *size_p = CirclePNG_len; NH_END(CirclePNG)
//        case 12: *size_p = SquarePNG_len; NH_END(SquarePNG)
//        case 13: NH_END(ListCSS)
//        case 14: NH_END(SelectOptionCSS)
//        case 15: NH_END(MiscCSS)
//        case 16: NH_END(HeaderCSS)
//        case 17:
//        case 18:
//        case 19:
//        case 20:
//        case 21: NH_END(NULL) 
//        case 22: NH_END(NULL) 
    }

NH_END(NULL)
}

static char *Nh_getPayloadFromInternalURL(
    Nh_InternalURL *URL_p)
{
NH_BEGIN()

//    switch (URL_p->type)
//    {
//        case NH_INTERNAL_URL_HTML_NODE_TEXT            : NH_END( ((Nh_HTML_Node*)URL_p->pointer)->text_p       )
//        case NH_INTERNAL_URL_HTML_NODE_ATTRIBUTE_VALUE : NH_END( ((Nh_HTML_Attribute*)URL_p->pointer)->value_p )
//    }

NH_END(NULL)
}

// STRINGIFY =======================================================================================

void Nh_stringifyURI(
    Nh_URI *URI_p, char *set_p)
{
NH_BEGIN()

    switch (URI_p->scheme)
    {
        case NH_URI_SCHEME_FILE         : strcpy(set_p, ((Nh_FileURL*)URI_p->data_p)->path_p); break;
        case NH_URI_SCHEME_HTTP         : break;                
        case NH_URI_SCHEME_HTTPS        : break;                
        case NH_URI_SCHEME_INTERNAL_URN : break;  
        case NH_URI_SCHEME_INTERNAL_URL : break;
        case NH_URI_SCHEME_UNDEFINED    : break;
    }

NH_SILENT_END()
}


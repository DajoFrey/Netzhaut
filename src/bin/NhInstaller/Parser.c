// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "Parser.h"
#include "Util.h"
#include "Main.h"

#include "Common/Macro.h"
#include NH_INSTALLER_FLOW
#include NH_INSTALLER_DEFAULT_CHECK

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

// DECLARE =========================================================================================

typedef enum NH_INSTALLER_DEFINITION {
    NH_INSTALLER_DEFINITION_UNDEFINED,
    NH_INSTALLER_DEFINITION_LIBRARY,
    NH_INSTALLER_DEFINITION_COMPILE_ARGS,
    NH_INSTALLER_DEFINITION_LINK_ARGS,
    NH_INSTALLER_DEFINITION_SOURCE,
} NH_INSTALLER_DEFINITION;

typedef struct Nh_Installer_Definition {
    NH_INSTALLER_DEFINITION type;
    NH_BYTE *content_p;
} Nh_Installer_Definition;

// LIBRARY DEFINITION ==============================================================================

static NH_INSTALLER_RESULT Nh_Installer_parseLibrary(
    Nh_Installer_Definition *Definition_p) 
{
NH_INSTALLER_BEGIN()

    Nh_Installer_Library *Library_p = NULL;
    for (int i = 0; i < 255; ++i) {
        Library_p = &NH_INSTALLER_LIBRARIES_P[i];
        if (!Library_p->name_p) {break;}
        Library_p = NULL;
    }

    if (!Library_p) {NH_INSTALLER_DIAGNOSTIC_END(NH_INSTALLER_ERROR_BAD_STATE)}

    Library_p->name_p = Definition_p->content_p;

NH_INSTALLER_DIAGNOSTIC_END(NH_INSTALLER_SUCCESS)
}

// ARGUMENTS DEFINITION ============================================================================

static NH_INSTALLER_RESULT Nh_Installer_parseCompileOrLinkArgs(
    Nh_Installer_Definition *Definition_p, NH_BOOL compileArgs) 
{
NH_INSTALLER_BEGIN()

    NH_BYTE *bytes_p = Definition_p->content_p;

    while (*bytes_p && *bytes_p != ',') {bytes_p++;}
    if (!*bytes_p) {NH_INSTALLER_DIAGNOSTIC_END(NH_INSTALLER_ERROR_BAD_STATE)}

    *bytes_p = 0;

    Nh_Installer_Library *Library_p = NULL;
    for (int i = 0; i < 255; ++i) {
        Library_p = &NH_INSTALLER_LIBRARIES_P[i];
        if (Library_p->name_p != NULL && !strcmp(Library_p->name_p, Definition_p->content_p)) {break;}
        Library_p = NULL;
    }

    if (!Library_p) {NH_INSTALLER_DIAGNOSTIC_END(NH_INSTALLER_ERROR_BAD_STATE)}
    bytes_p++;

    while (*bytes_p && *bytes_p != '"') {bytes_p++;}
    if (!*bytes_p) {NH_INSTALLER_DIAGNOSTIC_END(NH_INSTALLER_ERROR_BAD_STATE)}

    if (compileArgs) {
        Library_p->compileArgs_p = ++bytes_p;
    }
    else {
        Library_p->linkArgs_p = ++bytes_p;
    }

    while (*bytes_p && *bytes_p != '"') {bytes_p++;}
    if (!*bytes_p) {NH_INSTALLER_DIAGNOSTIC_END(NH_INSTALLER_ERROR_BAD_STATE)}

    *bytes_p = 0;

NH_INSTALLER_DIAGNOSTIC_END(NH_INSTALLER_SUCCESS)
}

// SOURCE DEFINITION ===============================================================================

static NH_INSTALLER_RESULT Nh_Installer_parseSource(
    Nh_Installer_Definition *Definition_p) 
{
NH_INSTALLER_BEGIN()

    NH_BYTE *bytes_p = Definition_p->content_p;

    while (*bytes_p)
    {
        Nh_Installer_Source *Source_p = NULL;
        for (int i = 0; i < 1024; ++i) {
            Source_p = &NH_INSTALLER_SOURCES_P[i];
            if (!Source_p->path_p) {break;}
            Source_p = NULL;
        }

        if (!Source_p) {NH_INSTALLER_DIAGNOSTIC_END(NH_INSTALLER_ERROR_BAD_STATE)}

        while (*bytes_p && *bytes_p != '"') {bytes_p++;}
        if (!*bytes_p) {NH_INSTALLER_DIAGNOSTIC_END(NH_INSTALLER_ERROR_BAD_STATE)}

        Source_p->path_p = ++bytes_p;

        while (*bytes_p && *bytes_p != '"') {bytes_p++;}
        if (!*bytes_p) {NH_INSTALLER_DIAGNOSTIC_END(NH_INSTALLER_ERROR_BAD_STATE)}

        *bytes_p = 0;
        bytes_p++;

        Nh_Installer_Library *Library_p = NULL;
        for (int i = 0; i < 255; ++i) {
            Library_p = &NH_INSTALLER_LIBRARIES_P[i];
            if (Library_p->name_p && strstr(Source_p->path_p, Library_p->name_p)) {
                Source_p->Library_p = Library_p;
                break;
            }
        }
    }

NH_INSTALLER_DIAGNOSTIC_END(NH_INSTALLER_SUCCESS)
}

// DEFINITIONS =====================================================================================

static NH_BOOL Nh_Installer_isASCIIUpperAlpha(
    NH_BYTE codepoint)
{
NH_INSTALLER_BEGIN()
NH_INSTALLER_END(codepoint >= 0x41 && codepoint <= 0x5A)
}

static NH_BOOL Nh_Installer_isASCIILowerAlpha(
    NH_BYTE codepoint)
{
NH_INSTALLER_BEGIN()
NH_INSTALLER_END(codepoint >= 0x61 && codepoint <= 0x7A)
}

static NH_BOOL Nh_Installer_isASCIIAlpha(
    NH_BYTE codepoint)
{
NH_INSTALLER_BEGIN()
NH_INSTALLER_END(Nh_Installer_isASCIIUpperAlpha(codepoint) || Nh_Installer_isASCIILowerAlpha(codepoint))
}

static NH_BOOL Nh_Installer_caseInsensitiveMatch(
    NH_BYTE *str1_p, NH_BYTE *str2_p)
{
NH_INSTALLER_BEGIN()

    if (strlen(str1_p) != strlen(str2_p)) {NH_INSTALLER_END(NH_FALSE)}

    for (int i = 0; i < strlen(str1_p); ++i) {
        int d = tolower((unsigned char)str1_p[i]) - tolower((unsigned char)str2_p[i]);
        if (d != 0) {
            NH_INSTALLER_END(NH_FALSE)
        }
    }
   
NH_INSTALLER_END(NH_TRUE)
}

static NH_BYTE *Nh_Installer_getDefinition(
    Nh_Installer_Definition *Definition_p, char *bytes_p) 
{
NH_INSTALLER_BEGIN()

    while (*bytes_p && !Nh_Installer_isASCIIAlpha(*bytes_p)) {bytes_p++;}
    if (!*bytes_p) {NH_INSTALLER_END(NULL)}

    NH_BYTE *identifierBegin_p = bytes_p;

    while (*bytes_p && Nh_Installer_isASCIIAlpha(*bytes_p)) {bytes_p++;}
    if (!*bytes_p) {NH_INSTALLER_END(NULL)}

    *bytes_p = 0;    

    if (Nh_Installer_caseInsensitiveMatch(identifierBegin_p, "library")) {
        Definition_p->type = NH_INSTALLER_DEFINITION_LIBRARY;
    }
    else if (Nh_Installer_caseInsensitiveMatch(identifierBegin_p, "compilearguments")) {
        Definition_p->type = NH_INSTALLER_DEFINITION_COMPILE_ARGS;
    }
    else if (Nh_Installer_caseInsensitiveMatch(identifierBegin_p, "linkarguments")) {
        Definition_p->type = NH_INSTALLER_DEFINITION_LINK_ARGS;
    }
    else if (Nh_Installer_caseInsensitiveMatch(identifierBegin_p, "source")) {
        Definition_p->type = NH_INSTALLER_DEFINITION_SOURCE;
    }
    else {
        NH_INSTALLER_END(NULL)
    }

    bytes_p++;

    while (*bytes_p && !Nh_Installer_isASCIIAlpha(*bytes_p) && *bytes_p != '"') {bytes_p++;}
    if (!*bytes_p) {NH_INSTALLER_END(NULL)}

    Definition_p->content_p = bytes_p;

    while (*bytes_p && *bytes_p != ')') {bytes_p++;}
    if (!*bytes_p) {NH_INSTALLER_END(NULL)}

    *bytes_p = 0;

NH_INSTALLER_END(&bytes_p[1])
}

static NH_INSTALLER_RESULT Nh_Installer_getDefinitions(
    Nh_Installer_Definition *Definitions_p, NH_BYTE *bytes_p) 
{
NH_INSTALLER_BEGIN()

    int definition = 0;

    while (bytes_p)
    {
        Nh_Installer_Definition *Definition_p = &Definitions_p[definition++];
        bytes_p = Nh_Installer_getDefinition(Definition_p, bytes_p);
    }

NH_INSTALLER_DIAGNOSTIC_END(NH_INSTALLER_SUCCESS)
}

// BUILD ===========================================================================================

NH_INSTALLER_RESULT Nh_Installer_parseInstallerFile(
    char *projDir_p) 
{
NH_INSTALLER_BEGIN()

    char inPath_p[255] = {'\0'};
    sprintf(inPath_p, "%s/data/installer/installer.txt", projDir_p);

    NH_BYTE *bytes_p = Nh_Installer_getFileData(inPath_p, NULL);
    NH_INSTALLER_CHECK_NULL(bytes_p)

    NH_INSTALLER_FILE_DATA_P = bytes_p;

    Nh_Installer_Definition Definitions_p[255];

    for (int i = 0; i < 255; ++i) {
        Definitions_p[i].type = NH_INSTALLER_DEFINITION_UNDEFINED;
        Definitions_p[i].content_p = NULL;
    }
 
    Nh_Installer_getDefinitions(Definitions_p, bytes_p);

    for (int i = 0; i < 255; ++i)
    {
        Nh_Installer_Definition *Definition_p = &Definitions_p[i];

        switch (Definition_p->type)
        {
            case NH_INSTALLER_DEFINITION_LIBRARY :
                Nh_Installer_parseLibrary(Definition_p);
                break; 
            case NH_INSTALLER_DEFINITION_COMPILE_ARGS :
                Nh_Installer_parseCompileOrLinkArgs(Definition_p, NH_TRUE);
                break; 
            case NH_INSTALLER_DEFINITION_LINK_ARGS :
                Nh_Installer_parseCompileOrLinkArgs(Definition_p, NH_FALSE);
                break; 
            case NH_INSTALLER_DEFINITION_SOURCE :
                Nh_Installer_parseSource(Definition_p);
                break; 
        }
    }

NH_INSTALLER_DIAGNOSTIC_END(NH_INSTALLER_SUCCESS)
}


// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "Util.h"

#include "Common/API.h"
#include "Common/Macro.h"
#include NH_WEBIDL_SERIALIZER_FLOW
#include NH_WEBIDL_SERIALIZER_DEFAULT_CHECK

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

// TOOLS ===========================================================================================

int Nh_WebIDLSerializer_getWordLength(
    NH_BYTE *current_p, NH_BYTE *delimiters_p, int delimiterCount)
{
NH_WEBIDL_SERIALIZER_BEGIN()

    int length = 0;

    while (current_p != NULL) {
        for (int i = 0; i < delimiterCount; ++i) {
            if (delimiters_p[i] == *current_p) {NH_WEBIDL_SERIALIZER_END(length)}
        }
        current_p = current_p + 1;
        length++;
    }

NH_WEBIDL_SERIALIZER_END(0)
}

NH_BYTE *Nh_WebIDLSerializer_allocateWord(
    NH_BYTE *current_p, NH_BYTE *delimiters_p, int delimiterCount)
{
NH_WEBIDL_SERIALIZER_BEGIN()

    NH_BYTE *result_p = NULL;
    int length = Nh_WebIDLSerializer_getWordLength(current_p, delimiters_p, delimiterCount);

    if (length > 0) {
        NH_BYTE replace = current_p[length];
        current_p[length] = '\0';
        result_p = malloc(sizeof(NH_BYTE) * (length + 1));
        strcpy(result_p, current_p);
        current_p[length] = replace;
    }

NH_WEBIDL_SERIALIZER_END(result_p)
}

NH_BYTE *Nh_WebIDLSerializer_nextValid(
    NH_BYTE *current_p)
{
NH_WEBIDL_SERIALIZER_BEGIN()

    NH_BOOL lineComment = NH_FALSE, blockComment = NH_FALSE;

    while (current_p != NULL && strlen(current_p) > 0) 
    {
        if (*current_p == '\0') {NH_WEBIDL_SERIALIZER_END(NULL)}
        if (*current_p == '/' && current_p[1] == '/') {lineComment = NH_TRUE;}
        if (*current_p == '/' && current_p[1] == '*') {blockComment = NH_TRUE;}
        if (*current_p == '*' && current_p[1] == '/') {blockComment = NH_FALSE;}
        if (*current_p == '\n') {lineComment = NH_FALSE;}

        if (*current_p != ' ' && *current_p != '\t' && *current_p != '\n' && !lineComment && !blockComment) {break;}

        current_p = current_p + 1;
    }

NH_WEBIDL_SERIALIZER_END(current_p)
}

void *Nh_WebIDLSerializer_getFileData( // TODO use NhCore implementation
    const NH_BYTE* path_p, long *size_p)
{
NH_WEBIDL_SERIALIZER_BEGIN()

    FILE *fh = fopen(path_p, "rb");
    if (fh == NULL) {NH_WEBIDL_SERIALIZER_END(NULL)}
    
    if (fseek(fh, 0L, SEEK_END) != 0) {NH_WEBIDL_SERIALIZER_END(NULL)}

    long size = ftell(fh);

    if (fseek(fh, 0L, SEEK_SET) != 0) {NH_WEBIDL_SERIALIZER_END(NULL)}

    if(size <= 0) {
        fclose(fh);
        NH_WEBIDL_SERIALIZER_END(NULL)
    }

    NH_BYTE *data_p = (NH_BYTE*)malloc(((size_t)size) + 1); 
    if (data_p == NULL) {NH_WEBIDL_SERIALIZER_END(NULL)}
    
    fread(data_p, 1, size, fh);
    fclose(fh);

    data_p[size] = '\0';
    if (size_p != NULL) {*size_p = size;}

NH_WEBIDL_SERIALIZER_END(data_p)
}


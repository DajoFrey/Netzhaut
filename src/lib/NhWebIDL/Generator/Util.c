// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "Util.h"

#include "../Common/API.h"
#include "../Common/Macro.h"
#include NH_WEBIDL_FLOW
#include NH_WEBIDL_DEFAULT_CHECK

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

// TOOLS ===========================================================================================

NH_BOOL Nh_WebIDL_isWord(
    char *current_p, const char *word_p)
{
NH_WEBIDL_BEGIN()

    for (int i = 0; i < strlen(word_p); ++i) {
        if (current_p[i] != word_p[i]) {NH_WEBIDL_END(NH_FALSE)}
    }

NH_WEBIDL_END(NH_TRUE)
}

int Nh_WebIDL_getWordLength(
    char *current_p, char *delimiters_p, int delimiterCount)
{
NH_WEBIDL_BEGIN()

    int length = 0;

    while (current_p != NULL) {
        for (int i = 0; i < delimiterCount; ++i) {
            if (delimiters_p[i] == *current_p) {NH_WEBIDL_END(length)}
        }
        current_p = current_p + 1;
        length++;
    }

NH_WEBIDL_END(0)
}

char *Nh_WebIDL_allocateWord(
    char *current_p, char *delimiters_p, int delimiterCount)
{
NH_WEBIDL_BEGIN()

    char *result_p = NULL;
    int length = Nh_WebIDL_getWordLength(current_p, delimiters_p, delimiterCount);

    if (length > 0) {
        char replace = current_p[length];
        current_p[length] = '\0';
        result_p = malloc(sizeof(char) * (length + 1));
        strcpy(result_p, current_p);
        current_p[length] = replace;
    }

NH_WEBIDL_END(result_p)
}

char *Nh_WebIDL_nextValid(
    char *current_p)
{
NH_WEBIDL_BEGIN()

    NH_BOOL lineComment = NH_FALSE, blockComment = NH_FALSE;

    while (current_p != NULL && strlen(current_p) > 0) 
    {
        if (*current_p == '\0') {NH_WEBIDL_END(NULL)}
        if (*current_p == '/' && current_p[1] == '/') {lineComment = NH_TRUE;}
        if (*current_p == '/' && current_p[1] == '*') {blockComment = NH_TRUE;}
        if (*current_p == '*' && current_p[1] == '/') {blockComment = NH_FALSE;}
        if (*current_p == '\n') {lineComment = NH_FALSE;}

        if (*current_p != ' ' && *current_p != '\t' && *current_p != '\n' && !lineComment && !blockComment) {break;}

        current_p = current_p + 1;
    }

NH_WEBIDL_END(current_p)
}

void *Nh_WebIDL_getFileData( // TODO use NhCore implementation
    const char* path_p, long *size_p)
{
NH_WEBIDL_BEGIN()

    FILE *fh = fopen(path_p, "rb");
    if (fh == NULL) {NH_WEBIDL_END(NULL)}
    
    if (fseek(fh, 0L, SEEK_END) != 0) {NH_WEBIDL_END(NULL)}

    long size = ftell(fh);

    if (fseek(fh, 0L, SEEK_SET) != 0) {NH_WEBIDL_END(NULL)}

    if(size <= 0) {
        fclose(fh);
        NH_WEBIDL_END(NULL)
    }

    char *data_p = (char*)malloc(((size_t)size) + 1); 
    if (data_p == NULL) {NH_WEBIDL_END(NULL)}
    
    fread(data_p, 1, size, fh);
    fclose(fh);

    data_p[size] = '\0';
    if (size_p != NULL) {*size_p = size;}

NH_WEBIDL_END(data_p)
}

NH_WEBIDL_RESULT Nh_WebIDL_getFragmentName(
    char *filepath_p, char *fragmentname_p)
{
NH_WEBIDL_BEGIN()

    filepath_p = filepath_p + strlen(filepath_p);
    while (*filepath_p != '/') {filepath_p = filepath_p - 1;}
    filepath_p = filepath_p - 1;
    while (*filepath_p != '/') {filepath_p = filepath_p - 1;}
    filepath_p = filepath_p + 1;
    for (int i = 0; strcmp(filepath_p, ".idl"); ++i) {fragmentname_p[i] = *filepath_p; filepath_p = filepath_p + 1;}
    for (int i = 0; i < strlen(fragmentname_p); ++i) {if (fragmentname_p[i] == '/') {fragmentname_p[i] = '_';}}

NH_WEBIDL_END(NH_WEBIDL_SUCCESS)
}


// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE ========================================================================================

#include "String.h"
#include "Memory.h"
#include "LinkedList.h"

#include "Common/Macro.h"
#include NH_FLOW
#include NH_DEFAULT_CHECK

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <stdarg.h>
#include <ctype.h>

#ifdef __unix__
    #include <unistd.h>
    #include <sys/types.h>
    #include <sys/wait.h>
#endif

// DATA STORAGE UNITS ==============================================================================

Nh_DataStorageUnits Nh_humanizeBytes(
    long long bytes)
{
NH_BEGIN()

    Nh_DataStorageUnits Units;
    Units.gigabytes = bytes / (1024*1024*1024);
    Units.megabytes = (bytes / (1024*1024)) % 1024; 
    Units.kilobytes = (bytes / 1024) % 1024;
    Units.bytes     = bytes % 1024;
   
NH_END(Units)
}

// BYTES ===========================================================================================

NH_BYTE *_Nh_allocateBytes(
    NH_BYTE *bytes_p)
{
    if (bytes_p == NULL || strlen(bytes_p) == 0) {return NULL;}
    NH_BYTE *allocatedBytes_p = _Nh_allocate(sizeof(NH_BYTE) * (strlen(bytes_p) + 1));
    if (allocatedBytes_p == NULL) {return NULL;}
    strcpy(allocatedBytes_p, bytes_p);
    return allocatedBytes_p;
}

NH_BYTE *Nh_allocateBytes(
    NH_BYTE *bytes_p)
{
NH_BEGIN()
NH_END(_Nh_allocateBytes(bytes_p))
}

// LEVENSHTEIN =====================================================================================

int Nh_levenshteinDistance(
    NH_BYTE *s1_p, NH_BYTE *s2_p, NH_BOOL caseSensitive) 
{
NH_BEGIN()

    if (strlen(s1_p) > 255 || strlen(s2_p) > 255) {NH_END(-1)}

    unsigned int s1len, s2len, x, y, lastdiag, olddiag;

    NH_BYTE s1cpy_p[256] = {'\0'}, s2cpy_p[256] = {'\0'};
    strcpy(s1cpy_p, s1_p);
    strcpy(s2cpy_p, s2_p);

    if (!caseSensitive)
    {
        for (int i = 0; i < strlen(s1_p); ++i) {
            s1cpy_p[i] = tolower(s1_p[i]);
        }
        for (int i = 0; i < strlen(s2_p); ++i) {
            s2cpy_p[i] = tolower(s2_p[i]);
        }
    }

    s1len = strlen(s1_p);
    s2len = strlen(s2_p);

    unsigned int column[s1len+1];

    for (y = 1; y <= s1len; y++) {
        column[y] = y;
    }

    for (x = 1; x <= s2len; x++) 
    {
        column[0] = x;
        for (y = 1, lastdiag = x-1; y <= s1len; y++) {
            olddiag = column[y];
#define                 MIN3(a, b, c) ((a) < (b) ? ((a) < (c) ? (a) : (c)) : ((b) < (c) ? (b) : (c)))
            column[y] = MIN3(column[y] + 1, column[y-1] + 1, lastdiag + (s1cpy_p[y-1] == s2cpy_p[x-1] ? 0 : 1));
            lastdiag = olddiag;
        }
    }

NH_END(column[s1len])
}

// STRING ===========================================================================================

Nh_String Nh_initString(
    int chunkSize)
{
NH_BEGIN()
NH_END(Nh_initArray(sizeof(NH_BYTE), chunkSize))
}

NH_RESULT Nh_appendFormatToString(
    Nh_String *String_p, NH_BYTE *format_p, ...)
{
NH_BEGIN()

    if (format_p == NULL) {NH_DIAGNOSTIC_END(NH_SUCCESS)}

    va_list args;
    va_start(args, format_p);

    int byteCount = vsnprintf(NULL, 0, format_p, args);
    NH_BYTE *append_p = Nh_allocate(sizeof(NH_BYTE) * (byteCount + 1));
    NH_CHECK_MEM(append_p)

    va_end(args);

    va_start(args, format_p);
    vsprintf(append_p, format_p, args);
    va_end(args);

    NH_CHECK(Nh_appendToArray(String_p, append_p, byteCount * sizeof(NH_BYTE)))

    Nh_free(append_p);

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

NH_RESULT Nh_appendToString(
    Nh_String *String_p, NH_BYTE *bytes_p, int length)
{
NH_BEGIN()
NH_DIAGNOSTIC_END(Nh_appendToArray(String_p, bytes_p, length))
}

NH_RESULT Nh_removeTailFromString(
    Nh_String *String_p, unsigned int count)
{
NH_BEGIN()
NH_DIAGNOSTIC_END(Nh_removeTailFromArray(String_p, count))
}

void Nh_freeString(
    Nh_String *String_p)
{
NH_BEGIN()

    Nh_freeArray(String_p);

NH_SILENT_END()
}


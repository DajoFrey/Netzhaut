#ifndef Nh_String_H
#define Nh_String_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "Array.h"

#include "Common/API.h"
#include "Common/Result.h"
#include "Common/API.h"

#include <stdint.h>
#include <stddef.h>

#endif

/** @addtogroup NhCoreEnums
 *  @{
 */

    typedef enum NH_ESCAPE {
        NH_ESCAPE_START      = 1,
        NH_ESCAPE_CSI        = 2,
        NH_ESCAPE_STR        = 4,  /* OSC, PM, APC */
        NH_ESCAPE_ALTCHARSET = 8,
        NH_ESCAPE_STR_END    = 16, /* a final string was encountered */
        NH_ESCAPE_TEST       = 32, /* Enter in test mode */
        NH_ESCAPE_UTF8       = 64,
        NH_ESCAPE_DCS        = 128,
    } NH_ESCAPE;

/** @} */

/** @addtogroup NhCoreStructs
 *  @{
 */

    typedef struct Nh_DataStorageUnits {
        long long gigabytes;
        long long megabytes;
        long long kilobytes;
        long long bytes;
    } Nh_DataStorageUnits;

/** @} */

/** @addtogroup NhCoreFunctions
 *  @{
 */

// HUMANIZE
    Nh_DataStorageUnits Nh_humanizeBytes(
        long long bytes
    );

// BYTES
    NH_BYTE *_Nh_allocateBytes(
        NH_BYTE *bytes_p
    );

    NH_BYTE *Nh_allocateBytes(
        NH_BYTE *bytes_p
    );

// LEVENSHTEIN
    int Nh_levenshteinDistance(
        NH_BYTE *s1_p, NH_BYTE *s2_p, NH_BOOL caseSensitive
    );

// STRING
    Nh_String Nh_initString(
        int chunkSize
    );

    NH_RESULT Nh_appendToString(
        Nh_String *String_p, NH_BYTE *bytes_p, int length
    );

    NH_RESULT Nh_appendFormatToString(
        Nh_String *String_p, NH_BYTE *format_p, ...
    );
    
    NH_RESULT Nh_removeTailFromString(
        Nh_String *String_p, unsigned int count
    );

    void Nh_freeString(
        Nh_String *String_p
    );

/** @} */

#endif 

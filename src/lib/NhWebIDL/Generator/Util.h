#ifndef NH_WEBIDL_UTIL_H
#define NH_WEBIDL_UTIL_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "../Common/Result.h"
#include "../Common/API.h"

#endif

/** @addtogroup WebIDLFunctions Functions
 *  \ingroup WebIDL
 *  @{
 */

    NH_BOOL Nh_WebIDL_isWord(
        char *current_p, const char *word_p
    );
    
    int Nh_WebIDL_getWordLength(
        char *current_p, char *delimiters_p, int delimiterCount
    );
    
    char *Nh_WebIDL_allocateWord(
        char *current_p, char *delimiters_p, int delimiterCount
    );
    
    char *Nh_WebIDL_nextValid(
        char *current_p
    );
    
    void *Nh_WebIDL_getFileData(
        const char* path_p, long *size_p
    );

    NH_WEBIDL_RESULT Nh_WebIDL_getFragmentName(
        char *filepath_p, char *fragmentname_p
    );

/** @} */

#endif

#ifndef NH_WEBIDL_SERIALIZER_UTIL_H
#define NH_WEBIDL_SERIALIZER_UTIL_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "Common/Result.h"
#include "Common/API.h"

#endif

/** @addtogroup WebIDLSerializerFunctions Functions
 *  \ingroup WebIDLSerializer
 *  @{
 */

    int Nh_WebIDLSerializer_getWordLength(
        NH_BYTE *current_p, NH_BYTE *delimiters_p, int delimiterCount
    );
    
    NH_BYTE *Nh_WebIDLSerializer_allocateWord(
        NH_BYTE *current_p, NH_BYTE *delimiters_p, int delimiterCount
    );
    
    NH_BYTE *Nh_WebIDLSerializer_nextValid(
        NH_BYTE *current_p
    );
    
    void *Nh_WebIDLSerializer_getFileData(
        const NH_BYTE* path_p, long *size_p
    );

/** @} */

#endif

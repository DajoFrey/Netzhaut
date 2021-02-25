#ifndef NH_WEBIDL_SERIALIZER_H
#define NH_WEBIDL_SERIALIZER_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "Parser.h"
#include "Lexer.h"

#include "../Common/Result.h"

#endif

/** @addtogroup WebIDLStructs Structs
 *  \ingroup WebIDL
 *  @{
 */

    typedef struct Nh_WebIDL_SerializationBuffer {
        char *array_pp[1024];
        int size;
        int index;
        int maxSize;
    } Nh_WebIDL_SerializationBuffer;

    typedef struct Nh_WebIDL_SerializationData {
        Nh_WebIDL_FragmentToken FragmentToken;
        Nh_WebIDL_Fragment Fragment;
        Nh_WebIDL_SerializationBuffer Buffer;
    } Nh_WebIDL_SerializationData;

/** @} */

/** @addtogroup WebIDLStructs Structs
 *  \ingroup WebIDL
 *  @{
 */

    Nh_WebIDL_SerializationData Nh_WebIDL_serialize(
        char *infilepath_p, void (*callback_f)(char *message_p)
    );

    void Nh_WebIDL_freeSerializationData(
        Nh_WebIDL_SerializationData Data 
    );

/** @} */

#endif

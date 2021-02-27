#ifndef NH_WEBIDL_SERIALIZER_SERIALIZER_H
#define NH_WEBIDL_SERIALIZER_SERIALIZER_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "Parser.h"
#include "Tokenizer.h"

#include "Common/Result.h"

#endif

/** @addtogroup NhWebIDLSerializerStructs
 *  @{
 */

    typedef struct Nh_WebIDLSerializer_SerializationBuffer {
        char *array_pp[1024];
        int size;
        int index;
        int maxSize;
    } Nh_WebIDLSerializer_SerializationBuffer;

    typedef struct Nh_WebIDLSerializer_SerializationData {
        Nh_WebIDLSerializer_FragmentToken FragmentToken;
        Nh_WebIDLSerializer_Fragment Fragment;
        Nh_WebIDLSerializer_SerializationBuffer Buffer;
    } Nh_WebIDLSerializer_SerializationData;

/** @} */

/** @addtogroup NhWebIDLSerializerFunctions
 *  @{
 */

    Nh_WebIDLSerializer_SerializationData Nh_WebIDLSerializer_serialize(
        char *infilepath_p, void (*callback_f)(char *message_p)
    );

    void Nh_WebIDLSerializer_freeSerializationData(
        Nh_WebIDLSerializer_SerializationData Data 
    );

/** @} */

#endif

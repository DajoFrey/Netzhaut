#ifndef NH_WEBIDL_SERIALIZER_LOG_H
#define NH_WEBIDL_SERIALIZER_LOG_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "Result.h"

#endif

/** @addtogroup NhWebIDLSerializerFunctions
 *  @{
 */

    NH_WEBIDL_SERIALIZER_RESULT _Nh_WebIDLSerializer_logBegin(
        const char *file_p, const char *function_p
    );
    
    NH_WEBIDL_SERIALIZER_RESULT _Nh_WebIDLSerializer_logEnd(
        const char *file_p, const char *function_p
    );
    
    NH_WEBIDL_SERIALIZER_RESULT _Nh_WebIDLSerializer_logDiagnosticEnd(
        const char *file_p, const char *function_p, NH_WEBIDL_SERIALIZER_RESULT result, int line
    );

/** @} */

#endif 

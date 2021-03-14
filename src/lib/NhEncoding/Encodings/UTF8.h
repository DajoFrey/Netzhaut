#ifndef NH_ENCODING_UTF8_H
#define NH_ENCODING_UTF8_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "../Common/API.h"

#endif

/** @addtogroup NhEncodingFunctions
 *  @{
 */

    Nh_UnicodeString Nh_Encoding_decodeUTF8(
        NH_UNSIGNED_BYTE *bytes_p, unsigned long length
    );

    NH_UNICODE_CODEPOINT Nh_Encoding_decodeUTF8Single(
        NH_UNSIGNED_BYTE *bytes_p, unsigned long length
    );

/** @} */

#endif

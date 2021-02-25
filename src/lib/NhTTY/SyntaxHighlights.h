#ifndef NH_TTY_SYNTAX_HIGHLIGHT_H
#define NH_TTY_SYNTAX_HIGHLIGHT_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "Common/Result.h"

#include "../NhCore/Unicode.h"
#include "../NhCore/String.h"
#include "../NhCore/LinkedList.h"
#include "../NhCore/List.h"

#endif

/** @addtogroup NhTTYEnums
 *  @{
 */

    typedef enum NH_TTY_TEXT {
        NH_TTY_TEXT_UNDEFINED,
        NH_TTY_TEXT_C,
        NH_TTY_TEXT_CPP,
    } NH_TTY_TEXT;

/** @} */

/** @addtogroup NhTTYFunctions
 *  @{
 */

    Nh_UnicodeString Nh_TTY_cSyntaxHighlights(
        Nh_UnicodeString *String_p
    );

    Nh_UnicodeString Nh_TTY_cppSyntaxHighlights(
        Nh_UnicodeString *String_p
    );
    
    NH_TTY_TEXT Nh_TTY_getTextType(
        NH_BYTE *path_p         
    );

/** @} */

#endif 

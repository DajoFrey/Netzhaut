#ifndef NH_TTY_MESSAGES_H
#define NH_TTY_MESSAGES_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

    #include "../NhCore/Unicode.h"

#endif

/** @addtogroup NhTTYEnums
 *  @{
 */

    typedef enum NH_TTY_MESSAGE {
        NH_TTY_MESSAGE_TILING_FOCUS,
        NH_TTY_MESSAGE_TILING_INSERT,
        NH_TTY_MESSAGE_TILING_WASD,
        NH_TTY_MESSAGE_TILING_WASDF,
        NH_TTY_MESSAGE_TILING_FOCUS_SWITCHED,
        NH_TTY_MESSAGE_TILING_ALREADY_FOCUSED,
        NH_TTY_MESSAGE_EDITOR_SHOW_TREE,
        NH_TTY_MESSAGE_EDITOR_HIDE_TREE,
        NH_TTY_MESSAGE_EDITOR_INSERT_ACTIVATED,
        NH_TTY_MESSAGE_EDITOR_INSERT_DEACTIVATED,
        NH_TTY_MESSAGE_BINARY_QUERY_YES_NO,
        NH_TTY_MESSAGE_BINARY_QUERY_DELETE,
        NH_TTY_MESSAGE_BINARY_QUERY_DELETE_INTERRUPTED,
        NH_TTY_MESSAGE_MISC_NO_CURRENT_PROGRAM,
    } NH_TTY_MESSAGE;

/** @} */

/** @addtogroup NhTTYFunctions
 *  @{
 */

    NH_UNICODE_CODEPOINT *Nh_TTY_getMessage(
        NH_TTY_MESSAGE message, int *length_p
    );

/** @} */

#endif 

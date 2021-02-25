// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "Messages.h"
#include "Common/Macro.h"

#include NH_TTY_FLOW

#include <stdio.h>
#include <stdlib.h>

// MESSAGES ========================================================================================

NH_UNICODE_CODEPOINT NH_TTY_MESSAGE_EN_TILING_FOCUS_P[] = {93, 32, 102, 111, 99, 117, 115, 32, 116, 105, 108, 101};
NH_UNICODE_CODEPOINT NH_TTY_MESSAGE_EN_TILING_INSERT_P[] = {93, 32, 105, 110, 115, 101, 114, 116, 32, 116, 105, 108, 101, 32, 91, 119, 93, 91, 97, 93, 91, 115, 93, 91, 100, 93, 32, 109, 111, 118, 101, 32, 116, 105, 108, 101};
NH_UNICODE_CODEPOINT NH_TTY_MESSAGE_EN_TILING_WASD_P[] = {91, 119, 93, 91, 97, 93, 91, 115, 93, 91, 100, 93, 32, 97, 100, 100, 32, 116, 105, 108, 101};
NH_UNICODE_CODEPOINT NH_TTY_MESSAGE_EN_TILING_WASDF_P[] = {91, 119, 93, 91, 97, 93, 91, 115, 93, 91, 100, 93, 32, 97, 100, 100, 32, 116, 105, 108, 101, 32, 91, 102, 93, 32, 115, 112, 108, 105, 116, 32, 116, 105, 108, 101};
NH_UNICODE_CODEPOINT NH_TTY_MESSAGE_EN_TILING_FOCUS_SWITCHED_P[] = {70, 111, 99, 117, 115, 32, 115, 119, 105, 116, 99, 104, 101, 100, 32, 116, 111, 32, 116, 104, 105, 115, 32, 116, 105, 108, 101};
NH_UNICODE_CODEPOINT NH_TTY_MESSAGE_EN_TILING_ALREADY_FOCUSED_P[] = {84, 105, 108, 101, 32, 105, 115, 32, 97, 108, 114, 101, 97, 100, 121, 32, 102, 111, 99, 117, 115, 101, 100};

NH_UNICODE_CODEPOINT NH_TTY_MESSAGE_EN_EDITOR_SHOW_TREE_P[] = {83, 104, 111, 119, 32, 116, 114, 101, 101, 32, 108, 105, 115, 116, 105, 110, 103, 46};
NH_UNICODE_CODEPOINT NH_TTY_MESSAGE_EN_EDITOR_HIDE_TREE_P[] = {72, 105, 100, 101, 32, 116, 114, 101, 101, 32, 108, 105, 115, 116, 105, 110, 103, 46};
NH_UNICODE_CODEPOINT NH_TTY_MESSAGE_EN_EDITOR_INSERT_ACTIVATED_P[]   = {73, 110, 115, 101, 114, 116, 32, 109, 111, 100, 101, 32, 97, 99, 116, 105, 118, 97, 116, 101, 100, 46};
NH_UNICODE_CODEPOINT NH_TTY_MESSAGE_EN_EDITOR_INSERT_DEACTIVATED_P[] = {73, 110, 115, 101, 114, 116, 32, 109, 111, 100, 101, 32, 100, 101, 97, 99, 116, 105, 118, 97, 116, 101, 100, 46};

NH_UNICODE_CODEPOINT NH_TTY_MESSAGE_EN_BINARY_QUERY_YES_NO_P[] = {63, 32, 91, 121, 93, 32, 121, 101, 115, 32, 91, 110, 93, 32, 110, 111};
NH_UNICODE_CODEPOINT NH_TTY_MESSAGE_EN_BINARY_QUERY_DELETE_P[] = {68, 101, 108, 101, 116, 101, 32};
NH_UNICODE_CODEPOINT NH_TTY_MESSAGE_EN_BINARY_QUERY_DELETE_INTERRUPTED_P[] = {68, 101, 108, 101, 116, 101, 32, 105, 110, 116, 101, 114, 114, 117, 112, 116, 101, 100};

NH_UNICODE_CODEPOINT NH_TTY_MESSAGE_EN_MISC_NO_CURRENT_PROGRAM_P[] = {78, 111, 32, 99, 117, 114, 114, 101, 110, 116, 32, 112, 114, 111, 103, 114, 97, 109};

// GET MESSAGE =====================================================================================

NH_UNICODE_CODEPOINT *Nh_TTY_getMessage(
    NH_TTY_MESSAGE message, int *length_p)
{
NH_TTY_BEGIN()

    NH_UNICODE_CODEPOINT *message_p = NULL;

    switch (message)
    {
        case NH_TTY_MESSAGE_TILING_FOCUS :
            message_p = NH_TTY_MESSAGE_EN_TILING_FOCUS_P;
            *length_p = sizeof(NH_TTY_MESSAGE_EN_TILING_FOCUS_P)/sizeof(NH_UNICODE_CODEPOINT);
            break;
        case NH_TTY_MESSAGE_TILING_INSERT:
            message_p = NH_TTY_MESSAGE_EN_TILING_INSERT_P;
            *length_p = sizeof(NH_TTY_MESSAGE_EN_TILING_INSERT_P)/sizeof(NH_UNICODE_CODEPOINT);
            break;
        case NH_TTY_MESSAGE_TILING_WASD  :
            message_p = NH_TTY_MESSAGE_EN_TILING_WASD_P;
            *length_p = sizeof(NH_TTY_MESSAGE_EN_TILING_WASD_P)/sizeof(NH_UNICODE_CODEPOINT);
            break;
        case NH_TTY_MESSAGE_TILING_WASDF :
            message_p = NH_TTY_MESSAGE_EN_TILING_WASDF_P;
            *length_p = sizeof(NH_TTY_MESSAGE_EN_TILING_WASDF_P)/sizeof(NH_UNICODE_CODEPOINT);
            break;
        case NH_TTY_MESSAGE_TILING_FOCUS_SWITCHED : 
            message_p = NH_TTY_MESSAGE_EN_TILING_FOCUS_SWITCHED_P;
            *length_p = sizeof(NH_TTY_MESSAGE_EN_TILING_FOCUS_SWITCHED_P)/sizeof(NH_UNICODE_CODEPOINT);
            break;
        case NH_TTY_MESSAGE_TILING_ALREADY_FOCUSED : 
            message_p = NH_TTY_MESSAGE_EN_TILING_ALREADY_FOCUSED_P;
            *length_p = sizeof(NH_TTY_MESSAGE_EN_TILING_ALREADY_FOCUSED_P)/sizeof(NH_UNICODE_CODEPOINT);
            break;
        case NH_TTY_MESSAGE_EDITOR_SHOW_TREE  :
            message_p = NH_TTY_MESSAGE_EN_EDITOR_SHOW_TREE_P;
            *length_p = sizeof(NH_TTY_MESSAGE_EN_EDITOR_SHOW_TREE_P)/sizeof(NH_UNICODE_CODEPOINT);
            break;
        case NH_TTY_MESSAGE_EDITOR_HIDE_TREE :
            message_p = NH_TTY_MESSAGE_EN_EDITOR_HIDE_TREE_P;
            *length_p = sizeof(NH_TTY_MESSAGE_EN_EDITOR_HIDE_TREE_P)/sizeof(NH_UNICODE_CODEPOINT);
            break;
        case NH_TTY_MESSAGE_EDITOR_INSERT_ACTIVATED : 
            message_p = NH_TTY_MESSAGE_EN_EDITOR_INSERT_ACTIVATED_P;
            *length_p = sizeof(NH_TTY_MESSAGE_EN_EDITOR_INSERT_ACTIVATED_P)/sizeof(NH_UNICODE_CODEPOINT);
            break;
        case NH_TTY_MESSAGE_EDITOR_INSERT_DEACTIVATED : 
            message_p = NH_TTY_MESSAGE_EN_EDITOR_INSERT_DEACTIVATED_P;
            *length_p = sizeof(NH_TTY_MESSAGE_EN_EDITOR_INSERT_DEACTIVATED_P)/sizeof(NH_UNICODE_CODEPOINT);
            break;
        case NH_TTY_MESSAGE_BINARY_QUERY_YES_NO : 
            message_p = NH_TTY_MESSAGE_EN_BINARY_QUERY_YES_NO_P;
            *length_p = sizeof(NH_TTY_MESSAGE_EN_BINARY_QUERY_YES_NO_P)/sizeof(NH_UNICODE_CODEPOINT);
            break;
        case NH_TTY_MESSAGE_BINARY_QUERY_DELETE : 
            message_p = NH_TTY_MESSAGE_EN_BINARY_QUERY_DELETE_P;
            *length_p = sizeof(NH_TTY_MESSAGE_EN_BINARY_QUERY_DELETE_P)/sizeof(NH_UNICODE_CODEPOINT);
            break;
        case NH_TTY_MESSAGE_BINARY_QUERY_DELETE_INTERRUPTED : 
            message_p = NH_TTY_MESSAGE_EN_BINARY_QUERY_DELETE_INTERRUPTED_P;
            *length_p = sizeof(NH_TTY_MESSAGE_EN_BINARY_QUERY_DELETE_INTERRUPTED_P)/sizeof(NH_UNICODE_CODEPOINT);
            break;
        case NH_TTY_MESSAGE_MISC_NO_CURRENT_PROGRAM : 
            message_p = NH_TTY_MESSAGE_EN_MISC_NO_CURRENT_PROGRAM_P;
            *length_p = sizeof(NH_TTY_MESSAGE_EN_MISC_NO_CURRENT_PROGRAM_P)/sizeof(NH_UNICODE_CODEPOINT);
            break;
    }

NH_TTY_END(message_p)
}


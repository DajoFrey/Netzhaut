#ifndef NH_TTY_CONSOLE_H
#define NH_TTY_CONSOLE_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "Program.h"
#include "Messages.h"
#include "Common/Result.h"

#include "../NhCore/Unicode.h"
#include "../NhCore/String.h"

#endif

/** @addtogroup NhTTYStructs
 *  @{
 */

    typedef struct Nh_TTY_Console {
        int currentProgram;
        Nh_Array *Programs_p;
        NH_BOOL hasFocus;
        int cursorX;
        int cursorY;
        int height;
        int cachedHeight;
        Nh_Array History;
        NH_BOOL block;
        NH_BOOL displace;
        NH_BOOL hide;
        void *args_p;
        NH_TTY_RESULT (*callback_f)(struct Nh_TTY_Console *Console_p, NH_UNICODE_CODEPOINT input, NH_BOOL *continue_p);
    } Nh_TTY_Console;

/** @} */

/** @addtogroup NhTTYFunctions
 *  @{
 */

    Nh_TTY_Console Nh_TTY_initConsole(
    );

    NH_BOOL Nh_TTY_compareMessage(
        Nh_TTY_Console *Console_p, NH_UNICODE_CODEPOINT *codepoints_p, int length
    );
    
    NH_BOOL Nh_TTY_compareMessage2(
        NH_UNICODE_CODEPOINT *codepoints_p, int length
    );

    NH_TTY_RESULT Nh_TTY_setMessage(
        Nh_TTY_Console *Console_p, NH_UNICODE_CODEPOINT *codepoints_p, int length
    );

    NH_TTY_RESULT Nh_TTY_setMessage2(
        NH_UNICODE_CODEPOINT *codepoints_p, int length
    );

    NH_TTY_RESULT Nh_TTY_setDefaultMessage(
        Nh_TTY_Console *Console_p, NH_TTY_MESSAGE message
    );
    
    NH_TTY_RESULT Nh_TTY_setDefaultMessage2(
        NH_TTY_MESSAGE message
    );

    NH_TTY_RESULT Nh_TTY_setBinaryQueryMessage(
        Nh_TTY_Console *Console_p, NH_UNICODE_CODEPOINT *query_p, int length, void *args_p, 
        NH_TTY_RESULT (*callback_f)(Nh_TTY_Console *Console_p, NH_UNICODE_CODEPOINT input, NH_BOOL *continue_p)
    );
    
    NH_TTY_RESULT Nh_TTY_setBinaryQueryMessage2(
        NH_UNICODE_CODEPOINT *query_p, int length, void *args_p, NH_TTY_RESULT (*callback_f)(Nh_TTY_Console *Console_p, NH_UNICODE_CODEPOINT input, NH_BOOL *continue_p)
    );

    NH_TTY_RESULT Nh_TTY_toggleConsole(
        Nh_TTY_Console *Console_p
    );

    NH_TTY_RESULT Nh_TTY_handleConsoleInput(
        Nh_TTY_Console *Console_p, NH_UNICODE_CODEPOINT c
    );

    NH_TTY_RESULT Nh_TTY_drawConsoleRow(
        Nh_TTY_Console *Console_p, Nh_String *Data_p, int row, int cols
    );

    Nh_TTY_Program *Nh_TTY_getCurrentProgram(
        Nh_TTY_Console *Console_p
    );

/** @} */

#endif 

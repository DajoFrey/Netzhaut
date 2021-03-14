#ifndef NH_TTY_API_H
#define NH_TTY_API_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "Result.h"

#include "../../NhCore/Common/API.h"
#include "../../NhEncoding/Common/API.h"

#endif

/** @addtogroup NhTTYTypedefs
 *  @{
 */

    typedef struct Nh_TTY_Terminal Nh_TTY_Terminal;
    typedef struct Nh_TTY_Program Nh_TTY_Program;

    typedef void *(*Nh_TTY_init_f)();
    typedef NH_TTY_RESULT (*Nh_TTY_draw_f)(Nh_TTY_Program *Program_p, Nh_Encoding_UTF8String *Row_p, int width, int height, int row);
    typedef NH_TTY_RESULT (*Nh_TTY_handleInput_f)(Nh_TTY_Program *Program_p, NH_UNICODE_CODEPOINT c);
    typedef NH_TTY_RESULT (*Nh_TTY_getCursorPosition_f)(Nh_TTY_Program *Program_p, int *x_p, int *y_p);
    typedef NH_TTY_RESULT (*Nh_TTY_update_f)(Nh_TTY_Program *Program_p);
    typedef NH_TTY_RESULT (*Nh_TTY_handleConsoleInput_f)(Nh_TTY_Program *Program_p, NH_UNICODE_CODEPOINT);

/** @} */

/** @addtogroup NhTTYStructs
 *  @{
 */

    typedef struct Nh_TTY_ProgramPrototype {
        Nh_UnicodeString Name;
        Nh_TTY_init_f init_f;
        Nh_TTY_draw_f draw_f;
        Nh_TTY_handleInput_f handleInput_f;
        Nh_TTY_getCursorPosition_f getCursorPosition_f;
        Nh_TTY_update_f update_f;
        Nh_TTY_handleConsoleInput_f handleConsoleInput_f;
    } Nh_TTY_ProgramPrototype;

    typedef struct Nh_TTY_Program {
        Nh_TTY_ProgramPrototype *Prototype_p;
        void *handle_p;
        NH_BOOL refresh;
        Nh_Array *Programs_p;
    } Nh_TTY_Program;

/** @} */

#endif

#ifndef NH_TTY_TERMINAL_H
#define NH_TTY_TERMINAL_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "Tab.h"
#include "Common/Result.h"

#include "../NhCore/LinkedList.h"
#include "../NhCore/List.h"
#include "../NhCore/Common/Result.h"

#ifdef __unix__
    #include <termios.h>
#endif

#define CTRL_KEY(k) ((k) & 0x1f)

#endif

/** @addtogroup NhTTYStructs
 *  @{
 */

    typedef struct Nh_TTY_Terminal {

        NH_BOOL close;
        NH_BOOL standalone;

        Nh_List Prototypes;
        Nh_LinkedList Tabs;
        Nh_TTY_Tab *Tab_p;

        int screenRows;
        int screenCols;

#ifdef __unix__
        struct {
            struct termios origTermios;
        } Linux;
#endif

#ifdef __unix__
        struct IPC {
            int readFD_p[2];
            int writeFD_p[2];
        } IPC;
#endif

    } Nh_TTY_Terminal;

/** @} */

/** @addtogroup NhTTYTypedefs
 *  @{
 */

    typedef Nh_TTY_Terminal *(*Nh_TTY_openTerminal_f)(
    ); 

/** @} */

/** @addtogroup NhTTYFunctions
 *  @{
 */

    void Nh_TTY_write(
        char *message_p
    );

    Nh_TTY_Terminal *Nh_TTY_openTerminal(
    );

    Nh_TTY_Terminal *Nh_TTY_getTerminal(
    );

/** @} */

#endif 

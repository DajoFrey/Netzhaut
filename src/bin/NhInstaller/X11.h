/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#ifndef NH_INSTALLER_X11_H
#define NH_INSTALLER_X11_H

#include "Common/Result.h"

#ifdef __unix__
    #include <X11/Xlib.h>
    #include <X11/Xatom.h>
    #include <linux/input.h>
    #include <X11/Xlib-xcb.h>
    #include <xkbcommon/xkbcommon-x11.h>
    #include <sys/mman.h>
#endif

/** @addtogroup InstallerStructs Structs
 *  \ingroup Installer
 *  @{
 */

    typedef struct Nh_Installer_X11 {

    #ifdef __unix__
        int screen;
        XFontStruct *Font_p;
        GC Gfx;
        Display *Display_p;
        Window Root;
        Window Window;
        Atom DeleteAtom;
        xcb_connection_t *Connection_p;
        int32_t keyboardDeviceID;
        struct xkb_keymap *Keymap_p;
        struct xkb_state *State_p;
        struct xkb_context *Context_p;
        unsigned long black, white;
        int width, height;
    #endif

    } Nh_Installer_X11;

/** @} */

/** @addtogroup InstallerFunctions Functions
 *  \ingroup Installer
 *  @{
 */

    NH_INSTALLER_RESULT Nh_Installer_X11_createWindow(
        char *title, int width, int heigh
    );
    
    NH_INSTALLER_RESULT Nh_Installer_X11_destroyWindow(
    );
 
    NH_INSTALLER_RESULT Nh_Installer_X11_getInput(
    );

    void Nh_Installer_X11_drawTextLine(
        int x, int y, char *text_p
    );

    void Nh_Installer_X11_drawLine(
       int x1, int y1, int x2, int y2
    );
 
    void Nh_Installer_X11_fillRectangle(
       int x1, int y1, int x2, int y2
    );

    void Nh_Installer_X11_clearWindow(
    );

/** @} */

#endif 

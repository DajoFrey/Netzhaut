#ifndef NH_X11_H
#define NH_X11_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "Input.h"

#include "Common/Result.h"
#include "Common/API.h"

#ifdef __unix__
    #include <wayland-server.h>
    #include <X11/Xlib.h>
    #include <X11/Xatom.h>
    #include <linux/input.h>
    #include <X11/Xlib-xcb.h>
    #include <xkbcommon/xkbcommon-x11.h>
    #include <sys/mman.h>
#endif

#endif

/** @addtogroup IOStructs Structs
 *  \ingroup IO
 *  @{
 */

    typedef struct Nh_X11_Window {

        int ignore;

    #ifdef __unix__
        Display *Display_p;             
        Window XWindow;                 
        Atom DeleteAtom;                
        xcb_connection_t *Connection_p;
        int32_t keyboardDeviceID;      
        struct xkb_keymap *Keymap_p;    
        struct xkb_state *State_p;      
        struct xkb_context *Context_p;  
    #endif

    } Nh_X11_Window;

/** @} */

/** @addtogroup IOFunctions Function
 *  \ingroup IO
 *  @{
 */

    NH_WSI_RESULT Nh_X11_getInput(
        Nh_X11_Window *Window_p, Nh_WSI_Input *Input_p
    ); 

    NH_WSI_RESULT Nh_X11_createWindow(
        char *title, int width, int height, Nh_X11_Window *Window_p
    );
    
    NH_WSI_RESULT Nh_X11_destroyWindow(
        Nh_X11_Window *Window_p
    );
    
    NH_WSI_RESULT Nh_X11_setWindowBackgroundColor(
        Nh_X11_Window *Window_p, Nh_Color Color 
    );
    
    NH_WSI_RESULT Nh_X11_setWindowTitle(
        Nh_X11_Window *Window_p, char *title_p
    );
    
    NH_WSI_RESULT Nh_X11_setMouseCursor(
        Nh_X11_Window *Window_p, int type
    );

/** @} */

#endif 

#ifndef NH_IO_WINDOW_H
#define NH_IO_WINDOW_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "X11.h"
#include "Win32.h"
#include "Mouse.h"

#include "Common/API.h"

#endif

/** @addtogroup IOEnums Enums
 *  \ingroup IO
 *  @{
 */

    typedef enum NH_IO_TYPE {
        NH_IO_TYPE_X11,
        NH_IO_TYPE_WAYLAND,
        NH_IO_TYPE_WIN32,
    } NH_IO_TYPE;
   
/** @} */

/** @addtogroup IOStructs Structs
 *  \ingroup IO
 *  @{
 */

    typedef struct Nh_IO_Window {
        NH_IO_TYPE type;
        Nh_IO_Input Input;
        Nh_X11_Window X11;
        Nh_Win32_Window Win32;
    } Nh_IO_Window;

/** @} */

/** @addtogroup IODefines Defines
 *  \ingroup IO
 *  @{
 */

    typedef NH_IO_RESULT (*Nh_IO_createWindow_f)(
        Nh_IO_Window *Window_p, char *title_p, Nh_PixelSize Size
    );

/** @} */

/** @addtogroup IOFunctions Functions
 *  \ingroup IO
 *  @{
 */

    void *Nh_IO_init(
        void *args_p
    ); 
    
    NH_SIGNAL Nh_IO_run(
        void *args_p
    ); 
    
    NH_IO_RESULT Nh_IO_createWindow(
        Nh_IO_Window *Window_p, char *title_p, Nh_PixelSize Size
    );
    
    NH_IO_RESULT Nh_IO_destroyWindow(
        Nh_IO_Window *Window_p
    );
    
    NH_IO_RESULT Nh_IO_setWindowBackgroundColor(
        Nh_IO_Window *Window_p, Nh_Color Color
    );
    
    NH_IO_RESULT Nh_IO_setWindowTitle(
        Nh_IO_Window *Window_p, char *title_p
    );
    
    NH_IO_RESULT Nh_IO_setMouseCursor(
        Nh_IO_Window *Window_p, NH_IO_MOUSE_CURSOR type
    );

    char *Nh_IO_dumpConfigureEvent(
        Nh_IO_ConfigureEvent *Event_p
    );

/** @} */

#endif 

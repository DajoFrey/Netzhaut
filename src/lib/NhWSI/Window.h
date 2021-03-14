#ifndef NH_WSI_WINDOW_H
#define NH_WSI_WINDOW_H

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

    typedef enum NH_WSI_TYPE {
        NH_WSI_TYPE_X11,
        NH_WSI_TYPE_WAYLAND,
        NH_WSI_TYPE_WIN32,
    } NH_WSI_TYPE;
   
/** @} */

/** @addtogroup IOStructs Structs
 *  \ingroup IO
 *  @{
 */

    typedef struct Nh_WSI_Window {
        NH_WSI_TYPE type;
        Nh_WSI_Input Input;
        Nh_X11_Window X11;
        Nh_Win32_Window Win32;
    } Nh_WSI_Window;

/** @} */

/** @addtogroup IODefines Defines
 *  \ingroup IO
 *  @{
 */

    typedef NH_WSI_RESULT (*Nh_WSI_createWindow_f)(
        Nh_WSI_Window *Window_p, char *title_p, Nh_PixelSize Size
    );

/** @} */

/** @addtogroup IOFunctions Functions
 *  \ingroup IO
 *  @{
 */

    void *Nh_WSI_init(
        void *args_p
    ); 
    
    NH_SIGNAL Nh_WSI_run(
        void *args_p
    ); 
    
    NH_WSI_RESULT Nh_WSI_createWindow(
        Nh_WSI_Window *Window_p, char *title_p, Nh_PixelSize Size
    );
    
    NH_WSI_RESULT Nh_WSI_destroyWindow(
        Nh_WSI_Window *Window_p
    );
    
    NH_WSI_RESULT Nh_WSI_setWindowBackgroundColor(
        Nh_WSI_Window *Window_p, Nh_Color Color
    );
    
    NH_WSI_RESULT Nh_WSI_setWindowTitle(
        Nh_WSI_Window *Window_p, char *title_p
    );
    
    NH_WSI_RESULT Nh_WSI_setMouseCursor(
        Nh_WSI_Window *Window_p, NH_WSI_MOUSE_CURSOR type
    );

    char *Nh_WSI_dumpConfigureEvent(
        Nh_WSI_ConfigureEvent *Event_p
    );

/** @} */

#endif 

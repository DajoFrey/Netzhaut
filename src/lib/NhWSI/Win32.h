#ifndef NH_WSI_WIN32_H
#define NH_WSI_WIN32_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "Common/Result.h"
#include "Common/API.h"

#if defined(_WIN32) || defined (WIN32)
    #include <windows.h>
#endif

#endif

/** @addtogroup Structs
 *  @{
 */

    typedef struct Nh_Win32_Window {

        int ignore;

    #if defined(_WIN32) || defined (WIN32)
        NHND hwnd;
        WNDCLASS wc;
        HINSTANCE hinstance;
    #endif

    } Nh_Win32_Window;

/** @} */


/** @addtogroup Functions
 *  @{
 */

    NH_WSI_RESULT Nh_Win32_getInput(
        Nh_Win32_Window *Window_p
    ); 

    NH_WSI_RESULT Nh_Win32_createWindow(
        char *title, int width, int height, Nh_Win32_Window *Window_p
    );
    
    NH_WSI_RESULT Nh_Win32_destroyWindow(
        Nh_Win32_Window *Window_p
    );
    
    NH_WSI_RESULT Nh_Win32_setWindowBackgroundColor(
        Nh_Win32_Window *Window_p, Nh_Color Color
    );
    
    NH_WSI_RESULT Nh_Win32_setWindowTitle(
        Nh_Win32_Window *Window_p, char *title_p
    );
    
    NH_WSI_RESULT Nh_Win32_setMouseCursor(
        Nh_Win32_Window *Window_p, int type
    );

/** @} */

#endif 

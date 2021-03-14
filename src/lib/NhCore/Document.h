#ifndef NH_WINDOW_H
#define NH_WINDOW_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS
    
/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "System.h"
#include "Thread.h"
#include "LinkedList.h"

#include "Common/API.h"

#include "../NhGraphics/Vulkan/Header/Vulkan.h"
#include "../NhWSI/Window.h"

#endif

/** @addtogroup NhCoreStructs
 *  @{
 */

    typedef struct Nh_WindowContext {
        Nh_Gfx_RenderEngine *RenderEngine_p;
        Nh_Viewport Viewport;
    } Nh_WindowContext;

    typedef struct Nh_Document {
        Nh_CSS_LayoutEngine *LayoutEngine_p;
        Nh_HTML_Parser *Parser_p;
        Nh_List WindowContexts;
    } Nh_Document; 

/** @} */

/** @addtogroup NhCoreFunctions
 *  @{
 */

    NH_RESULT Nh_setWindowBackgroundColor(
        Nh_Window *Window_p, Nh_Color Color
    );
    
    NH_RESULT Nh_setWindowTitle(
        Nh_Window *Window_p, char *title_p
    );

    Nh_Window *Nh_openWindow(
    );

    NH_RESULT Nh_closeWindow(
        Nh_Window *Window_p
    );

/** @} */

#endif 

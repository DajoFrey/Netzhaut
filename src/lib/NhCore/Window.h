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
#include "../NhIO/Window.h"

#endif

/** @addtogroup NhCoreStructs
 *  @{
 */

    typedef struct Nh_Window {

        NH_API API;        
        NH_SIGNAL signal;
        Nh_IO_Window IO;
        Nh_LinkedList Tabs;

        Nh_PixelSize Size;
        Nh_PixelPosition Position;    

        void *args_p;

        struct {
            Nh_Color BackgroundColor;
            char *title_p;
        } Settings;

        struct {
            NH_BOOL halt;      
            NH_BOOL undoHalt;  
            NH_BOOL rendering; 
            NH_BOOL resize;   
            NH_BOOL ready;    
            NH_BOOL atomicRender;
        } Sync;

        struct {
            NH_BOOL getNextImage;       
            NH_BOOL skipRender;          
            bool atomic;
        } Internal;

        struct {
            NH_BOOL deduceBackground;
            NH_BOOL deduceTitle;
        } Flags;

        struct {
            VkSurfaceKHR SurfaceKHR;
            Nh_LinkedList Surfaces;
        } Vulkan;

    } Nh_Window;           

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

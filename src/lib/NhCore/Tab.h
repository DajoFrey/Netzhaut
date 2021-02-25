#ifndef NH_TAB_H
#define NH_TAB_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "Window.h"
#include "URI.h"

#include "Common/API.h"
#include "../NhIO/Input.h"

#endif

/** @addtogroup NhCoreEnums
 *  @{
 */

    typedef enum NH_TAB_STATE {
        NH_TAB_STATE_IDLE,
        NH_TAB_STATE_OPEN,
        NH_TAB_STATE_CLOSE,
        NH_TAB_STATE_UPDATE,
    } NH_TAB_STATE;

/** @} */

/** @addtogroup NhCoreStructs
 *  @{
 */

    typedef struct Nh_Tab {

        Nh_GPU GPU;
        NH_SIGNAL signal;
        void *args_p;

        Nh_Window *Window_p;       /**<A pointer to the window which contains the tab.*/
        Nh_IO_Input Input;
       
        Nh_LinkedList Contents;
        Nh_LinkedList Textures;  

        Nh_PixelSize Size;    
        Nh_PixelPosition Position;    

        struct {
            Nh_Color BackgroundColor;
            int priority;              /**<Defines the rendering order. 0 means highest priority aka the front tab. Lesser priorities ascend to +infinity.*/
        } Settings;

        struct {
            int newestBuffers_p[3];    
            int submittedBuffers_p[3]; 
            NH_BOOL atomicRecording;      
            NH_BOOL render;              
            NH_BOOL atomic;
        } Sync;

        struct {
            NH_BOOL alwaysOnTop;    /**<Tab will remain on top of the tab stacking order when true.*/
            NH_BOOL crop;           /**<Tab will be rendered using a cropped view when true.*/
            NH_BOOL fixedSize;
        } Flags;

        struct {
            VkCommandBuffer CommandBuffers_p[6];
        } Vulkan;     

    } Nh_Tab;

/** @} */

/** @addtogroup NhCoreFunctions
 *  @{
 */

    NH_RESULT Nh_resizeTab(
        Nh_Tab *Tab_p
    );

    Nh_Tab *Nh_openTab(
        Nh_Window *Window_p
    );

    NH_RESULT Nh_openTabContent(
        Nh_Tab *Tab_p, char *URI_p
    );

    NH_RESULT Nh_closeTab(
        Nh_Tab *Tab_p
    );
    
    NH_RESULT Nh_focusTab(
        Nh_Tab *Tab_p
    );
    
    NH_BOOL Nh_isForemostTab(
        Nh_Tab *Tab_p, Nh_PixelPosition Position
    );

    Nh_Tab *Nh_getTab(
        Nh_Window *Window_p, int index
    );

/** @} */

#endif

// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "Window.h"
#include "Tab.h"
#include "Memory.h"
#include "Sync.h"
#include "String.h"
#include "Time.h"
#include "Config.h"

//#include "../../Graphics/Main/Header/Window.h"

#include "Common/Macro.h"
#include NH_FLOW
#include NH_DEFAULT_CHECK

#include <string.h>
#include <limits.h>

// RESIZE ==========================================================================================

static NH_RESULT Nh_resize(
    Nh_Window *Window_p)
{
NH_BEGIN()

//    Nh_syncResizeBegin(Window_p);

//    switch (Window_p->API) 
//    { 
//        case NH_API_VULKAN : NH_CHECK(Nh_Vk_resize(Window_p)) break;
//    }
//
//    for (int i = 0; i < Window_p->Tabs.count; ++i) {
//        NH_CHECK(Nh_resizeTab(Nh_getTab(Window_p, i)))
//    }
//
//    Nh_syncResizeEnd(Window_p);

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

// RENDER ==========================================================================================

static NH_RESULT Nh_prepareRendering(
    Nh_Window *Window_p)
{
//NH_BEGIN()
//
//    NH_RESULT result = NH_ERROR_BAD_STATE;
//
//    switch (Window_p->API)
//    {
//        case NH_API_VULKAN : result = Nh_Vk_prepareRendering(Window_p); break;
//    }
//
//    if (result == NH_VULKAN_ERROR_OUT_OF_DATE_KHR) {
//        result = NH_ERROR_RESIZE_NEEDED;
//    }
//
//NH_END(result)
}

static NH_RESULT Nh_render(
    Nh_Window *Window_p)
{
//NH_BEGIN()
//
//    NH_RESULT result = NH_ERROR_BAD_STATE;
//
//    switch (Window_p->API)
//    {
//        case NH_API_VULKAN : result = Nh_Vk_render(Window_p); break;
//    }
//
//    if (result == NH_VULKAN_ERROR_OUT_OF_DATE_KHR) {
//        result = NH_ERROR_RESIZE_NEEDED;
//    }
//
//NH_END(result)
}

// UPDATE ==========================================================================================

static NH_RESULT Nh_processConfigureEvent(
    Nh_Window *Window_p, Nh_WSI_ConfigureEvent *Event_p)
{
NH_BEGIN()

    if (Window_p->Size.width  == Event_p->Size.width
    &&  Window_p->Size.height == Event_p->Size.height) 
    {
        Window_p->Position.x = Event_p->Position.x;
        Window_p->Position.y = Event_p->Position.y;
    }
    else {

        if (Event_p->Size.width < 0 || Event_p->Size.height < 0) {NH_DIAGNOSTIC_END(NH_SUCCESS)}

        for (int i = 0; i < Window_p->Tabs.count; ++i) 
        {
            Nh_Tab *Tab_p = Nh_getFromLinkedList(&Window_p->Tabs, i);

            if (Tab_p->Flags.fixedSize) {

            }
            else {
                Tab_p->Size.width  += Event_p->Size.width - Window_p->Size.width;
                Tab_p->Size.height += Event_p->Size.height - Window_p->Size.height;
            }
        }

        Window_p->Size.width  = Event_p->Size.width;
        Window_p->Size.height = Event_p->Size.height;
        Window_p->Position.x = Event_p->Position.x;
        Window_p->Position.y = Event_p->Position.y;

        Nh_resize(Window_p);
    }

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

static NH_RESULT Nh_processMouseEvent(
    Nh_Window *Window_p, Nh_WSI_MouseEvent *Event_p)
{
NH_BEGIN()

    Nh_Tab *Hit_p = NULL;

    for (int i = 0; i < Window_p->Tabs.count; ++i) 
    {
        Nh_Tab *Tab_p = Nh_getFromLinkedList(&Window_p->Tabs, i);

        if ((Tab_p->Position.x > Event_p->Position.x || (Tab_p->Position.x + Tab_p->Size.width) < Event_p->Position.x)
        ||  (Tab_p->Position.y > Event_p->Position.y || (Tab_p->Position.y + Tab_p->Size.height) < Event_p->Position.y))
            continue;

        if (Nh_isForemostTab(Tab_p, Event_p->Position)) {Hit_p = Tab_p;}
    }

    if (Hit_p == NULL) {NH_DIAGNOSTIC_END(NH_SUCCESS)}
    if (Hit_p->Settings.priority != 0 && Event_p->trigger == NH_WSI_TRIGGER_PRESS) {Nh_focusTab(Hit_p);} 

    Nh_WSI_MouseEvent *New_p = Nh_advanceRingIterator(&Hit_p->Input.Mouse.Events);
    New_p->type       = Event_p->type;
    New_p->trigger    = Event_p->trigger;
    New_p->Position.x = Event_p->Position.x - Hit_p->Position.x;
    New_p->Position.y = Event_p->Position.y - Hit_p->Position.y;

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

static NH_RESULT Nh_updateWindow(
    Nh_Window *Window_p)
{
NH_BEGIN()

    while (1) {
        Nh_WSI_ConfigureEvent *Event_p = Nh_incrementRingIteratorMark(
            &Window_p->IO.Input.Configure.Events, &Window_p->IO.Input.Configure.Mark
        );
        if (Event_p == NULL) {break;}
        NH_CHECK(Nh_processConfigureEvent(Window_p, Event_p))
    }

    while (1) {
        Nh_WSI_MouseEvent *Event_p = Nh_incrementRingIteratorMark(
            &Window_p->IO.Input.Mouse.Events, &Window_p->IO.Input.Mouse.Mark
        );
        if (Event_p == NULL) {break;}
        NH_CHECK(Nh_processMouseEvent(Window_p, Event_p))
    }

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

// INIT ============================================================================================

void *Nh_initWindow(
    void *args_p)
{
NH_BEGIN()

#include NH_CUSTOM_CHECK

    Nh_Window *Window_p = Nh_allocate(sizeof(Nh_Window));
    NH_CHECK_NULL(NULL, Window_p)
 
    if (NH_SYSTEM.available_p[NH_API_VULKAN]) {
        Window_p->API = NH_API_VULKAN;
    }
    else {
        Window_p->API = NH_API_UNDEFINED;
    }

    NH_INIT_LINKED_LIST(Window_p->Tabs) 

    Window_p->args_p = NULL;
    Window_p->signal = NH_SIGNAL_UPDATE;

    Window_p->Internal.atomic       = false;
    Window_p->Internal.skipRender   = NH_FALSE;
    Window_p->Internal.getNextImage = NH_TRUE;

    Window_p->Sync.halt         = NH_FALSE;
    Window_p->Sync.ready        = NH_FALSE;
    Window_p->Sync.resize       = NH_FALSE;
    Window_p->Sync.rendering    = NH_FALSE;
    Window_p->Sync.atomicRender = NH_FALSE;

    Window_p->Flags.deduceBackground = NH_FALSE;
    Window_p->Flags.deduceTitle = NH_TRUE;

    Window_p->Size.width  = 1000;
    Window_p->Size.height = 1000;
    Window_p->Position.x = -1;
    Window_p->Position.y = -1;

    Window_p->Settings.BackgroundColor.r = 0.0f;
    Window_p->Settings.BackgroundColor.g = 0.0f;
    Window_p->Settings.BackgroundColor.b = 0.0f;
    Window_p->Settings.BackgroundColor.a = 1.0f;
    Window_p->Settings.title_p = NULL;

//    NH_CHECK(NULL, ((Nh_WSI_createWindow_f)Nh_loadFunction("NhWSI", "Nh_WSI_createWindow"))(&Window_p->IO, "Empty Window", Window_p->Size))

//    switch (Window_p->API)
//    {
//        case NH_API_VULKAN : NH_CHECK(Nh_Vk_createWindowResources(Window_p)) break;
//        case NH_API_UNDEFINED : exit(0);
//    }

    Window_p->Sync.ready = NH_TRUE;

#include NH_DEFAULT_CHECK

NH_END(Window_p)
}

NH_SIGNAL Nh_runWindow(
    void *args_p)
{
NH_BEGIN()

#include NH_CUSTOM_CHECK

    NH_CHECK_NULL(NH_SIGNAL_ERROR, args_p)
    Nh_Window *Window_p = args_p;

    switch (Window_p->signal)
    {
        case NH_SIGNAL_SET_BACKGROUND_COLOR :
        {
//            memcpy(&Window_p->Settings.BackgroundColor, Window_p->args_p, sizeof(Nh_Color));
//            Nh_WSI_setWindowBackgroundColor(&Window_p->IO, *((Nh_Color*)Window_p->args_p));
//            Window_p->signal = NH_SIGNAL_DONE;
//
//            break;
        }
        case NH_SIGNAL_SET_TITLE :
        {
//            Nh_free(Window_p->Settings.title_p);
//
//            Window_p->Settings.title_p = Nh_allocateBytes(Window_p->args_p);
//            if (Window_p->Settings.title_p == NULL) {
//                Window_p->signal = NH_SIGNAL_ERROR;
//                NH_END(NH_SIGNAL_ERROR)
//            }
//
//            Nh_WSI_setWindowTitle(&Window_p->IO, Window_p->args_p);
//            Window_p->signal = NH_SIGNAL_DONE;
//       
//            break;
        }
        case NH_SIGNAL_UPDATE : 
        {
//            NH_CHECK(NH_SIGNAL_ERROR, Nh_updateWindow(Window_p))
//
//            if (Window_p->Tabs.count > 0 && Window_p->Size.width > 0 && Window_p->Size.height > 0) 
//            {
//                if (Nh_Gfx_prepareRendering(Window_p) != NH_SUCCESS) {break;}
//                Nh_Gfx_render(Window_p);
//            } 
//
            break;
        }
        case NH_SIGNAL_CLOSE :
        {
//            for (int i = 0; i < Window_p->Tabs.count; ++i) {
//                NH_CHECK(NH_SIGNAL_ERROR, Nh_closeTab(Nh_getTab(Window_p, i)))
//            }
//
//    switch (Window_p->API)
//    {
//        case NH_API_VULKAN : NH_CHECK(Nh_Vk_destroyWindowResources(Window_p)) break;
//    }
//            NH_CHECK(NH_SIGNAL_ERROR, Nh_WSI_destroyWindow(&Window_p->IO))
//
//            Nh_free(Window_p->Settings.title_p);
//            Nh_free(Window_p);
//
//            NH_END(NH_SIGNAL_DONE)
        }
        case NH_SIGNAL_DONE :
        case NH_SIGNAL_IDLE :
        {
            NH_END(NH_SIGNAL_IDLE)
        }
        default : 
        {
            NH_END(NH_SIGNAL_ERROR)
        }
    }

    if (NH_CONFIG.Flags.shutdown) {
        NH_CHECK(NH_SIGNAL_ERROR, Nh_closeWindow(Window_p))
        NH_END(NH_SIGNAL_DONE)
    }

#include NH_DEFAULT_CHECK

NH_END(NH_SIGNAL_OK)
}

// CONFIGURE SIGNALS ===============================================================================

static NH_RESULT Nh_sendWindowSignal(
    Nh_Window *Window_p, void *args_p, NH_SIGNAL signal)
{
NH_BEGIN()

    NH_SIGNAL previous = Window_p->signal;
    Window_p->signal = signal;
    Window_p->args_p = args_p;

    if (Nh_getThreadFromArgs(Window_p) == Nh_getThread()) {
        NH_CHECK(Nh_runWorkload(Nh_getWorkloadFromArgs(Window_p)))
    }
    else {
        while (Window_p->signal != NH_SIGNAL_DONE) {
            if (Window_p->signal == NH_SIGNAL_ERROR) {NH_DIAGNOSTIC_END(NH_ERROR_BAD_STATE)}
        }
    }

    Window_p->signal = previous;
    Window_p->args_p = NULL;

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

NH_RESULT Nh_setWindowBackgroundColor(
    Nh_Window *Window_p, Nh_Color Color)
{
NH_BEGIN()

    NH_CHECK_NULL(Window_p)

NH_DIAGNOSTIC_END(Nh_sendWindowSignal(Window_p, &Color, NH_SIGNAL_SET_BACKGROUND_COLOR))
}

NH_RESULT Nh_setWindowTitle(
    Nh_Window *Window_p, char *title_p)
{
NH_BEGIN()

    NH_CHECK_NULL(Window_p)
    NH_CHECK_NULL(title_p)

NH_DIAGNOSTIC_END(Nh_sendWindowSignal(Window_p, title_p, NH_SIGNAL_SET_TITLE))
}

// OPEN/CLOSE ======================================================================================

Nh_Window *Nh_openWindow()
{
NH_BEGIN()
NH_END(Nh_activateWorkload(Nh_initWindow, Nh_runWindow, NULL, NH_FALSE))
}

static NH_RESULT Nh_waitForWindowClosure(
    Nh_Window *Window_p)
{
NH_BEGIN()

    if (Nh_getThreadFromArgs(Window_p) == Nh_getThread()) {
        NH_DIAGNOSTIC_END(Nh_runWorkload(Nh_getWorkloadFromArgs(Window_p)))
    }
    else {
        while (Nh_getWorkloadFromArgs(Window_p)->signal != NH_SIGNAL_INACTIVE) {}
    }

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

NH_RESULT Nh_closeWindow(
    Nh_Window *Window_p)
{
NH_BEGIN()

    NH_CHECK_NULL(Window_p)
    Window_p->signal = NH_SIGNAL_CLOSE;
    NH_CHECK(Nh_waitForWindowClosure(Window_p))

NH_DIAGNOSTIC_END(NH_SUCCESS)
}


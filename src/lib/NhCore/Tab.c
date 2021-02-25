// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE ========================================================================================

#include "Tab.h"
#include "Sync.h"
#include "Time.h"
#include "Memory.h"
#include "Callbacks.h"
#include "String.h"
#include "Content.h"

#include "../NhIO/Mouse.h"
#include "../NhIO/Keyboard.h"

#include "../Graphics/Main/Header/Record.h"
#include "../Graphics/Vulkan/Header/Vulkan.h"

#include "Common/Macro.h"
#include NH_FLOW
#include NH_DEFAULT_CHECK

#include <string.h>

// UPDATE ==========================================================================================

static NH_RESULT Nh_processMouseEvent(
    Nh_Tab *Tab_p, Nh_IO_MouseEvent *Event_p)
{
NH_BEGIN()

    for (int i = 0; i < Tab_p->Contents.count; ++i) 
    {
        Nh_Content *Content_p = Nh_getFromLinkedList(&Tab_p->Contents, i);
        if (!Content_p->Flags.visible) {continue;}

        Nh_IO_MouseEvent *New_p = Nh_advanceRingIterator(&Content_p->Input.Mouse.Events);
        New_p->type = Event_p->type;
        New_p->trigger = Event_p->trigger;
        New_p->Position.x = Event_p->Position.x;
        New_p->Position.y = Event_p->Position.y;
    }

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

static NH_RESULT Nh_updateTab(
    Nh_Tab *Tab_p)
{
NH_BEGIN()

//    while (1) {
//        Nh_IO_MouseEvent *Event_p = Nh_incrementRingIteratorMark(
//            &Tab_p->Input.Mouse.Events, &Tab_p->Input.Mouse.Mark
//        );
//        if (Event_p == NULL) {break;}
//        NH_CHECK(Nh_processMouseEvent(Tab_p, Event_p))
//    }
//
//    for (int i = 0; i < Tab_p->Contents.count; ++i) {
//        Nh_Content *Content_p = Nh_getFromLinkedList(&Tab_p->Contents, i);
//        if (Content_p->Flags.visible) {NH_CHECK(Nh_updateContent(Content_p))}
//    }
//
//    NH_CHECK(Nh_Gfx_record(Tab_p))

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

// ADD/REMOVE ======================================================================================

static NH_RESULT Nh_addTabToWindow(
    Nh_Tab *Tab_p)
{
NH_BEGIN()

    for (int i = 0; i < Tab_p->Window_p->Tabs.count; ++i) {
        if (Nh_getTab(Tab_p->Window_p, i) == Tab_p) {
            NH_DIAGNOSTIC_END(NH_SUCCESS)
            break;
        }
    }
    NH_CHECK(Nh_appendToLinkedList(&Tab_p->Window_p->Tabs, Tab_p))

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

static NH_RESULT Nh_removeTabFromWindow(
    Nh_Tab *Tab_p)
{
NH_BEGIN()

    for (int i = 0; i < Tab_p->Window_p->Tabs.count; ++i) {
        if (Nh_getTab(Tab_p->Window_p, i) == Tab_p) {
            Nh_removeFromLinkedList(&Tab_p->Window_p->Tabs, i, false);
            break;
        }
    }

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

// INIT ============================================================================================

void *Nh_initTab(
    void *args_p)
{
NH_BEGIN()

#include NH_CUSTOM_CHECK

    Nh_Tab *Tab_p = Nh_allocate(sizeof(Nh_Tab));
    NH_CHECK_MEM(NULL, Tab_p)

    Tab_p->signal   = NH_SIGNAL_IDLE;
    Tab_p->args_p   = NULL;
    Tab_p->Window_p = args_p;

    Tab_p->Settings.BackgroundColor.r = 1.0f;
    Tab_p->Settings.BackgroundColor.g = 1.0f;
    Tab_p->Settings.BackgroundColor.b = 1.0f;
    Tab_p->Settings.BackgroundColor.a = 1.0f;

    Tab_p->Size.width  = Tab_p->Window_p->Size.width;
    Tab_p->Size.height = Tab_p->Window_p->Size.height;
    Tab_p->Position.x  = 0;
    Tab_p->Position.y  = 0;

    Tab_p->Flags.alwaysOnTop = NH_FALSE;
    Tab_p->Flags.crop        = NH_FALSE;
    Tab_p->Flags.fixedSize   = NH_FALSE;

    Tab_p->Sync.newestBuffers_p[0] = 0;
    Tab_p->Sync.newestBuffers_p[1] = 2;
    Tab_p->Sync.newestBuffers_p[2] = 4;

    Tab_p->Sync.atomicRecording = NH_FALSE;
    Tab_p->Sync.render          = NH_FALSE;
    Tab_p->Sync.atomic          = NH_FALSE;

    NH_INIT_LINKED_LIST(Tab_p->Contents)

//    if (Tab_p->Window_p->API == NH_API_VULKAN) {
//        Tab_p->GPU.API     = NH_API_VULKAN;
//        Tab_p->GPU.Pointer = Nh_Vk_getGPU(0);
//    }
//    else {
//        Tab_p->GPU.API     = NH_API_UNDEFINED;
//        Tab_p->GPU.Pointer = NULL;
//    }
//
//    NH_CHECK(NULL, Nh_Gfx_createRecordResources(Tab_p))
//    NH_CHECK(NULL, ((Nh_IO_initInput_f)Nh_loadFunction("NhIO", "Nh_IO_initInput"))(&Tab_p->Input))
    NH_CHECK(NULL, Nh_addTabToWindow(Tab_p))

#include NH_DEFAULT_CHECK

NH_END(Tab_p)
}

// RUN =============================================================================================

NH_SIGNAL Nh_runTab(
    void *args_p)
{
NH_BEGIN()

#include NH_CUSTOM_CHECK

    NH_CHECK_NULL(NH_SIGNAL_ERROR, args_p)
    Nh_Tab *Tab_p = args_p;

    switch (Tab_p->signal)
    {
        case NH_SIGNAL_RESIZE : 
        {
            for (int i = 0; i < Tab_p->Contents.count; ++i) {
                if (Nh_resizeContent(Nh_getContent(Tab_p, i)) != NH_SUCCESS) {
                    Tab_p->signal = NH_SIGNAL_ERROR;
                    NH_END(NH_SIGNAL_ERROR)
                }
            }

            Tab_p->signal = NH_SIGNAL_DONE;

            break;
        }
        case NH_SIGNAL_UPDATE : 
        {
            NH_CHECK(NH_SIGNAL_ERROR, Nh_updateTab(Tab_p))
            break;
        }
        case NH_SIGNAL_OPEN :
        {
            if (Nh_openContent(Tab_p, Tab_p->args_p) != NH_SUCCESS) {
                Tab_p->signal = NH_SIGNAL_ERROR;
                NH_END(NH_SIGNAL_ERROR)
            }
            if (Nh_focusTab(Tab_p) != NH_SUCCESS) {
                Tab_p->signal = NH_SIGNAL_ERROR;
                NH_END(NH_SIGNAL_ERROR)
            }

            Tab_p->signal = NH_SIGNAL_DONE;

            break;
        }
        case NH_SIGNAL_CLOSE :
        {
//            NH_CHECK(NH_SIGNAL_ERROR, Nh_destroyContents(Tab_p))
//        
//            Nh_Gfx_destroyRecordResources(Tab_p);
//            Nh_IO_freeInput(&Tab_p->Input);
//            Nh_removeTabFromWindow(Tab_p);
//
//            Nh_free(Tab_p);
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

#include NH_DEFAULT_CHECK

NH_END(NH_SIGNAL_OK)
}

// CONFIGURE SIGNALS ===============================================================================

static NH_RESULT Nh_sendTabSignal(
    Nh_Tab *Tab_p, void *args_p, NH_SIGNAL signal)
{
NH_BEGIN()

    NH_SIGNAL previous = Tab_p->signal;
    Tab_p->signal = signal;
    Tab_p->args_p = args_p;

    if (Nh_getThreadFromArgs(Tab_p) == Nh_getThread()) {
        NH_CHECK(Nh_runWorkload(Nh_getWorkloadFromArgs(Tab_p)))
    }
    else {
        while (Tab_p->signal != NH_SIGNAL_DONE) {
            if (Tab_p->signal == NH_SIGNAL_ERROR) {NH_DIAGNOSTIC_END(NH_ERROR_BAD_STATE)}
        }
    }

    Tab_p->signal = previous;
    Tab_p->args_p = NULL;

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

NH_RESULT Nh_resizeTab(
    Nh_Tab *Tab_p)
{
NH_BEGIN()

    NH_CHECK_NULL(Tab_p)

NH_DIAGNOSTIC_END(Nh_sendTabSignal(Tab_p, NULL, NH_SIGNAL_RESIZE))
}

NH_RESULT Nh_openTabContent(
    Nh_Tab *Tab_p, char *URI_p)
{
NH_BEGIN()

    NH_CHECK_NULL(Tab_p)

    if (URI_p == NULL) {NH_DIAGNOSTIC_END(NH_ERROR_INVALID_ARGUMENT)}

    NH_CHECK(Nh_sendTabSignal(Tab_p, URI_p, NH_SIGNAL_OPEN))
    Tab_p->signal = NH_SIGNAL_UPDATE;

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

// OPEN/CLOSE =======================================================================================

Nh_Tab *Nh_openTab(
    Nh_Window *Window_p)
{
NH_BEGIN()

#include NH_CUSTOM_CHECK

    NH_CHECK_NULL(NULL, Window_p)

    Nh_Tab *Tab_p = Nh_activateWorkload(Nh_initTab, Nh_runTab, Window_p, NH_TRUE);

#include NH_DEFAULT_CHECK

NH_END(Tab_p)
}

static NH_RESULT Nh_waitForTabClosure(
    Nh_Tab *Tab_p)
{
NH_BEGIN()

    if (Nh_getThreadFromArgs(Tab_p) == Nh_getThread()) {
        NH_DIAGNOSTIC_END(Nh_runWorkload(Nh_getWorkloadFromArgs(Tab_p)))
    }
    else {
        while (Nh_getWorkloadFromArgs(Tab_p)->signal != NH_SIGNAL_INACTIVE) {}
    }

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

NH_RESULT Nh_closeTab(
    Nh_Tab *Tab_p)
{
NH_BEGIN()

    NH_CHECK_NULL(Tab_p)
    Tab_p->signal = NH_SIGNAL_CLOSE;
    NH_CHECK(Nh_waitForTabClosure(Tab_p))

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

// STACKING ========================================================================================

NH_BOOL Nh_isForemostTab(
    Nh_Tab *Tab_p, Nh_PixelPosition Position)
{
NH_BEGIN()

    NH_CHECK_NULL(Tab_p->Window_p)

    if ((Tab_p->Position.x > Position.x || (Tab_p->Position.x + Tab_p->Size.width) < Position.x)
    ||  (Tab_p->Position.y > Position.y || (Tab_p->Position.y + Tab_p->Size.height) < Position.y))
        NH_DIAGNOSTIC_END(NH_FALSE)

    for (int i = 0; i < Tab_p->Window_p->Tabs.count; ++i) 
    {
        Nh_Tab *Tmp_p = Nh_getTab(Tab_p->Window_p, i);
        
        if (Tmp_p != NULL && Tmp_p != Tab_p) 
        {
            if ((Tmp_p->Position.x > Position.x || (Tmp_p->Position.x + Tmp_p->Size.width) < Position.x)
            ||  (Tmp_p->Position.y > Position.y || (Tmp_p->Position.y + Tmp_p->Size.height) < Position.y))
                continue;

            if (!Tab_p->Flags.alwaysOnTop && Tmp_p->Settings.priority < Tab_p->Settings.priority) {
                NH_DIAGNOSTIC_END(NH_FALSE)
            }
            if (!Tab_p->Flags.alwaysOnTop && Tmp_p->Flags.alwaysOnTop) {
                NH_DIAGNOSTIC_END(NH_FALSE)
            }
            if (Tab_p->Flags.alwaysOnTop && Tmp_p->Flags.alwaysOnTop && Tmp_p->Settings.priority < Tab_p->Settings.priority) {
                NH_DIAGNOSTIC_END(NH_FALSE)
            }
        }
    }

NH_DIAGNOSTIC_END(NH_TRUE)
}

NH_RESULT Nh_focusTab(
    Nh_Tab *Tab_p)
{
NH_BEGIN()

    NH_CHECK_NULL(Tab_p->Window_p)

    if (Tab_p->Window_p->Flags.deduceTitle) {
        Nh_setWindowTitle(Tab_p->Window_p, Nh_getTitle(Nh_getVisibleContent(Tab_p)));
    }

    for (int i = 0; i < Tab_p->Window_p->Tabs.count; ++i) 
    {
        Nh_Tab *Other_p = Nh_getTab(Tab_p->Window_p, i);
        Other_p->Settings.priority += 1;
    }

    Tab_p->Settings.priority = 0;

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

// GET =============================================================================================

Nh_Tab *Nh_getTab(
    Nh_Window *Window_p, int index)
{
NH_BEGIN()
NH_END(Nh_getFromLinkedList(&Window_p->Tabs, index))
}


// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE ========================================================================================

#include "Input.h"
#include "Win32.h"
#include "X11.h"

#include "../NhCore/Window.h"
#include "../NhCore/Memory.h"
#include "../NhCore/Config.h"

#include "Common/Macro.h"
#include NH_WSI_FLOW
#include NH_WSI_DEFAULT_CHECK

#include "../NhCore/Common/Macro.h"
#include NH_FLOW
#include NH_CUSTOM_CHECK

#include <fcntl.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

// DATA ============================================================================================

const char *trigger_pp[] = {
    "UNDEFINED",
    "PRESS",
    "RELEASE",
    "UP",
    "DOWN",
    "MOVE",
    "GET_FOCUS",
    "LOOSE_FOCUS",
};

// INPUT ===========================================================================================

static void Nh_WSI_initConfigureEventBuffer(
    Nh_RingIterator *Iterator_p, int itemCount)
{
NH_WSI_BEGIN()

    for (int i = 0; i < itemCount; ++i) 
    {
        Nh_WSI_ConfigureEvent *Event_p = Nh_advanceRingIterator(Iterator_p);
        Event_p->Position.x  = -1;
        Event_p->Position.y  = -1;
        Event_p->Size.width  = -1;
        Event_p->Size.height = -1;
    }

NH_WSI_SILENT_END()
}

NH_WSI_RESULT Nh_WSI_initInput(
    Nh_WSI_Input *Input_p) 
{
NH_WSI_BEGIN()

    NH_CHECK(NH_WSI_ERROR_BAD_STATE, Nh_initRingIterator(
        &Input_p->Configure.Events, NH_CONFIG.Settings.inputBufferSize, sizeof(Nh_WSI_ConfigureEvent),
        Nh_WSI_initConfigureEventBuffer
    ))
    Nh_initRingIteratorMark(&Input_p->Configure.Mark);

    NH_WSI_CHECK(Nh_WSI_initMouse(&Input_p->Mouse, NH_CONFIG.Settings.inputBufferSize))
    NH_WSI_CHECK(Nh_WSI_initKeyboard(&Input_p->Keyboard, NH_CONFIG.Settings.inputBufferSize))

NH_WSI_DIAGNOSTIC_END(NH_WSI_SUCCESS);
}

Nh_WSI_InputMark Nh_WSI_initInputMark()
{
NH_WSI_BEGIN()

    Nh_WSI_InputMark Mark;

    Nh_initRingIteratorMark(&Mark.Configure);
    Nh_initRingIteratorMark(&Mark.Mouse);
    Nh_initRingIteratorMark(&Mark.Keyboard);

NH_WSI_END(Mark)
}

void Nh_WSI_freeInput(
    Nh_WSI_Input *Input_p) 
{
NH_WSI_BEGIN()

    Nh_freeRingIterator(&Input_p->Configure.Events);
    Nh_freeRingIterator(&Input_p->Mouse.Events);
    Nh_freeRingIterator(&Input_p->Keyboard.Events);

NH_WSI_SILENT_END();
}

const char *Nh_WSI_stringifyTrigger(
    NH_WSI_TRIGGER trigger)
{
NH_WSI_BEGIN()
NH_WSI_END(trigger_pp[trigger])
}

// RUN =============================================================================================

//static NH_WSI_RESULT Nh_WSI_getInput(
//    Nh_WSI_Window *Window_p)
//{
//NH_WSI_BEGIN()
//
//    switch (Window_p->type)
//    {
//        case NH_WSI_TYPE_X11   : NH_WSI_CHECK(Nh_X11_getInput(&Window_p->X11, &Window_p->Input)) break;
//        case NH_WSI_TYPE_WIN32 : NH_WSI_CHECK(Nh_Win32_getInput(&Window_p->Win32)) break;
//        default               : NH_WSI_DIAGNOSTIC_END(NH_WSI_ERROR_BAD_STATE)
//    }
//
//NH_WSI_DIAGNOSTIC_END(NH_WSI_SUCCESS)
//}
//
//NH_SIGNAL Nh_WSI_run(
//    void *args_p) 
//{
//NH_WSI_BEGIN()
//
//    if ( Controller.init && Controller.Windows.count == 0) {NH_WSI_END(NH_SIGNAL_IDLE)}
//    if (!Controller.init && Controller.Windows.count == 0) {NH_WSI_END(NH_SIGNAL_DONE)}
//
//    Controller.init = NH_FALSE;
//
//    for (int i = 0; i < Controller.Windows.count; ++i) {
//        Nh_WSI_getInput(Nh_getFromLinkedList(&Controller.Windows, i));
//    }
//
//NH_WSI_END(NH_SIGNAL_OK)
//}


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
#include NH_IO_FLOW
#include NH_IO_DEFAULT_CHECK

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

static void Nh_IO_initConfigureEventBuffer(
    Nh_RingIterator *Iterator_p, int itemCount)
{
NH_IO_BEGIN()

    for (int i = 0; i < itemCount; ++i) 
    {
        Nh_IO_ConfigureEvent *Event_p = Nh_advanceRingIterator(Iterator_p);
        Event_p->Position.x  = -1;
        Event_p->Position.y  = -1;
        Event_p->Size.width  = -1;
        Event_p->Size.height = -1;
    }

NH_IO_SILENT_END()
}

NH_IO_RESULT Nh_IO_initInput(
    Nh_IO_Input *Input_p) 
{
NH_IO_BEGIN()

    NH_CHECK(NH_IO_ERROR_BAD_STATE, Nh_initRingIterator(
        &Input_p->Configure.Events, NH_CONFIG.Settings.inputBufferSize, sizeof(Nh_IO_ConfigureEvent),
        Nh_IO_initConfigureEventBuffer
    ))
    Nh_initRingIteratorMark(&Input_p->Configure.Mark);

    NH_IO_CHECK(Nh_IO_initMouse(&Input_p->Mouse, NH_CONFIG.Settings.inputBufferSize))
    NH_IO_CHECK(Nh_IO_initKeyboard(&Input_p->Keyboard, NH_CONFIG.Settings.inputBufferSize))

NH_IO_DIAGNOSTIC_END(NH_IO_SUCCESS);
}

Nh_IO_InputMark Nh_IO_initInputMark()
{
NH_IO_BEGIN()

    Nh_IO_InputMark Mark;

    Nh_initRingIteratorMark(&Mark.Configure);
    Nh_initRingIteratorMark(&Mark.Mouse);
    Nh_initRingIteratorMark(&Mark.Keyboard);

NH_IO_END(Mark)
}

void Nh_IO_freeInput(
    Nh_IO_Input *Input_p) 
{
NH_IO_BEGIN()

    Nh_freeRingIterator(&Input_p->Configure.Events);
    Nh_freeRingIterator(&Input_p->Mouse.Events);
    Nh_freeRingIterator(&Input_p->Keyboard.Events);

NH_IO_SILENT_END();
}

const char *Nh_IO_stringifyTrigger(
    NH_IO_TRIGGER trigger)
{
NH_IO_BEGIN()
NH_IO_END(trigger_pp[trigger])
}

// RUN =============================================================================================

//static NH_IO_RESULT Nh_IO_getInput(
//    Nh_IO_Window *Window_p)
//{
//NH_IO_BEGIN()
//
//    switch (Window_p->type)
//    {
//        case NH_IO_TYPE_X11   : NH_IO_CHECK(Nh_X11_getInput(&Window_p->X11, &Window_p->Input)) break;
//        case NH_IO_TYPE_WIN32 : NH_IO_CHECK(Nh_Win32_getInput(&Window_p->Win32)) break;
//        default               : NH_IO_DIAGNOSTIC_END(NH_IO_ERROR_BAD_STATE)
//    }
//
//NH_IO_DIAGNOSTIC_END(NH_IO_SUCCESS)
//}
//
//NH_SIGNAL Nh_IO_run(
//    void *args_p) 
//{
//NH_IO_BEGIN()
//
//    if ( Controller.init && Controller.Windows.count == 0) {NH_IO_END(NH_SIGNAL_IDLE)}
//    if (!Controller.init && Controller.Windows.count == 0) {NH_IO_END(NH_SIGNAL_DONE)}
//
//    Controller.init = NH_FALSE;
//
//    for (int i = 0; i < Controller.Windows.count; ++i) {
//        Nh_IO_getInput(Nh_getFromLinkedList(&Controller.Windows, i));
//    }
//
//NH_IO_END(NH_SIGNAL_OK)
//}


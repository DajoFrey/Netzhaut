// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE ========================================================================================

#include "Window.h"
#include "Win32.h"
#include "X11.h"

#include "../NhCore/String.h"
#include "../NhCore/Memory.h"
#include "../NhCore/LinkedList.h"
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
#include <stdint.h>

#ifdef __unix__
    #include <X11/Xcursor/Xcursor.h>
    #include <X11/Xutil.h>
#elif defined(_WIN32) || defined (WIN32)
    #include <windows.h>
    #include <windowsx.h>
    #include <winuser.h>
    #include <Shellapi.h>
#endif

// DATA ============================================================================================

typedef struct Nh_IO_Controller {
    NH_BOOL init;
    Nh_LinkedList Windows;
} Nh_IO_Controller;

Nh_IO_Controller Controller;

// TYPE ============================================================================================

static NH_IO_TYPE Nh_IO_getType()
{
NH_IO_BEGIN()

    NH_IO_TYPE type = -1;

    #ifdef __unix__
        type = NH_IO_TYPE_X11;
    #elif defined(_WIN32) || defined (WIN32)
        type = NH_IO_TYPE_WIN32;
    #endif

NH_IO_END(type)
}

// INIT ============================================================================================

void *Nh_IO_init(
    void *args_p) 
{
NH_IO_BEGIN()

    Controller.init = NH_TRUE;
    NH_INIT_LINKED_LIST(Controller.Windows)

NH_IO_END(&Controller)
}

// RUN =============================================================================================

static NH_IO_RESULT Nh_IO_getInput(
    Nh_IO_Window *Window_p)
{
NH_IO_BEGIN()

    switch (Window_p->type)
    {
        case NH_IO_TYPE_X11   : NH_IO_CHECK(Nh_X11_getInput(&Window_p->X11, &Window_p->Input)) break;
        case NH_IO_TYPE_WIN32 : NH_IO_CHECK(Nh_Win32_getInput(&Window_p->Win32)) break;
        default               : NH_IO_DIAGNOSTIC_END(NH_IO_ERROR_BAD_STATE)
    }

NH_IO_DIAGNOSTIC_END(NH_IO_SUCCESS)
}

NH_SIGNAL Nh_IO_run(
    void *args_p) 
{
NH_IO_BEGIN()

    if ( Controller.init && Controller.Windows.count == 0) {NH_IO_END(NH_SIGNAL_IDLE)}
    if (!Controller.init && Controller.Windows.count == 0) {NH_IO_END(NH_SIGNAL_DONE)}

    Controller.init = NH_FALSE;

    for (int i = 0; i < Controller.Windows.count; ++i) {
        Nh_IO_getInput(Nh_getFromLinkedList(&Controller.Windows, i));
    }

NH_IO_END(NH_SIGNAL_OK)
}

// CREATE ==========================================================================================

NH_IO_RESULT Nh_IO_createWindow(
    Nh_IO_Window *Window_p, char *title_p, Nh_PixelSize Size)
{
NH_IO_BEGIN();

    Window_p->type = Nh_IO_getType();
    NH_IO_CHECK(Nh_IO_initInput(&Window_p->Input))

    switch (Window_p->type)
    {
        case NH_IO_TYPE_X11   : NH_IO_CHECK(Nh_X11_createWindow(title_p, Size.width, Size.height, &Window_p->X11)) break;
        case NH_IO_TYPE_WIN32 : NH_IO_CHECK(Nh_Win32_createWindow(title_p, Size.width, Size.height, &Window_p->Win32)) break;
        default               : NH_IO_DIAGNOSTIC_END(NH_IO_ERROR_BAD_STATE)
    }

    NH_IO_CHECK(Nh_appendToLinkedList(&Controller.Windows, Window_p))

NH_IO_DIAGNOSTIC_END(NH_IO_SUCCESS);
}

// DESTROY =========================================================================================

NH_IO_RESULT Nh_IO_destroyWindow(
    Nh_IO_Window *Window_p)
{
NH_IO_BEGIN()

    switch (Window_p->type)
    {
        case NH_IO_TYPE_X11   : NH_IO_CHECK(Nh_X11_destroyWindow(&Window_p->X11)) break;
        case NH_IO_TYPE_WIN32 : NH_IO_CHECK(Nh_Win32_destroyWindow(&Window_p->Win32)) break;
        default               : NH_IO_DIAGNOSTIC_END(NH_IO_ERROR_BAD_STATE)
    }

    Nh_IO_freeInput(&Window_p->Input);
    Nh_removeFromLinkedList2(&Controller.Windows, Window_p, false);

NH_IO_DIAGNOSTIC_END(NH_IO_SUCCESS)
}

// SET =============================================================================================

NH_IO_RESULT Nh_IO_setWindowBackgroundColor(
    Nh_IO_Window *Window_p, Nh_Color Color)
{
NH_IO_BEGIN()

    switch (Window_p->type)
    {
        case NH_IO_TYPE_X11   : NH_IO_CHECK(Nh_X11_setWindowBackgroundColor(&Window_p->X11, Color)) break;
        case NH_IO_TYPE_WIN32 : NH_IO_CHECK(Nh_Win32_setWindowBackgroundColor(&Window_p->Win32, Color)) break;
        default               : NH_IO_DIAGNOSTIC_END(NH_IO_ERROR_BAD_STATE)
    }

NH_IO_DIAGNOSTIC_END(NH_IO_SUCCESS)
}

NH_IO_RESULT Nh_IO_setWindowTitle(
    Nh_IO_Window *Window_p, char *title_p)
{
NH_IO_BEGIN()

    switch (Window_p->type)
    {
        case NH_IO_TYPE_X11   : NH_IO_CHECK(Nh_X11_setWindowTitle(&Window_p->X11, title_p)) break;
        case NH_IO_TYPE_WIN32 : NH_IO_CHECK(Nh_Win32_setWindowTitle(&Window_p->Win32, title_p)) break;
        default               : NH_IO_DIAGNOSTIC_END(NH_IO_ERROR_BAD_STATE)
    }

NH_IO_DIAGNOSTIC_END(NH_IO_SUCCESS)
}

NH_IO_RESULT Nh_IO_setMouseCursor(
    Nh_IO_Window *Window_p, NH_IO_MOUSE_CURSOR type)
{
NH_IO_BEGIN()

    switch (Window_p->type)
    {
        case NH_IO_TYPE_X11   : NH_IO_CHECK(Nh_X11_setMouseCursor(&Window_p->X11, type)) break;
        case NH_IO_TYPE_WIN32 : NH_IO_CHECK(Nh_Win32_setMouseCursor(&Window_p->Win32, type)) break;
        default               : NH_IO_DIAGNOSTIC_END(NH_IO_ERROR_BAD_STATE)
    }

NH_IO_DIAGNOSTIC_END(NH_IO_SUCCESS)
}


// DUMP ============================================================================================

char *Nh_IO_dumpConfigureEvent(
    Nh_IO_ConfigureEvent *Event_p)
{
NH_IO_BEGIN()

    Nh_String String = Nh_initString(16);
    NH_CHECK(NULL, Nh_appendFormatToString(&String, "WINDOW SIZE:%d/%d POSITION:%d/%d\n", Event_p->Size.width, Event_p->Size.height, Event_p->Position.x, Event_p->Position.y))
  
NH_IO_END(String.bytes_p)
}


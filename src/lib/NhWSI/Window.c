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

typedef struct Nh_WSI_Controller {
    NH_BOOL init;
    Nh_LinkedList Windows;
} Nh_WSI_Controller;

Nh_WSI_Controller Controller;

// TYPE ============================================================================================

static NH_WSI_TYPE Nh_WSI_getType()
{
NH_WSI_BEGIN()

    NH_WSI_TYPE type = -1;

    #ifdef __unix__
        type = NH_WSI_TYPE_X11;
    #elif defined(_WIN32) || defined (WIN32)
        type = NH_WSI_TYPE_WIN32;
    #endif

NH_WSI_END(type)
}

// INIT ============================================================================================

void *Nh_WSI_init(
    void *args_p) 
{
NH_WSI_BEGIN()

    Controller.init = NH_TRUE;
    NH_INIT_LINKED_LIST(Controller.Windows)

NH_WSI_END(&Controller)
}

// RUN =============================================================================================

static NH_WSI_RESULT Nh_WSI_getInput(
    Nh_WSI_Window *Window_p)
{
NH_WSI_BEGIN()

    switch (Window_p->type)
    {
        case NH_WSI_TYPE_X11   : NH_WSI_CHECK(Nh_X11_getInput(&Window_p->X11, &Window_p->Input)) break;
        case NH_WSI_TYPE_WIN32 : NH_WSI_CHECK(Nh_Win32_getInput(&Window_p->Win32)) break;
        default               : NH_WSI_DIAGNOSTIC_END(NH_WSI_ERROR_BAD_STATE)
    }

NH_WSI_DIAGNOSTIC_END(NH_WSI_SUCCESS)
}

NH_SIGNAL Nh_WSI_run(
    void *args_p) 
{
NH_WSI_BEGIN()

    if ( Controller.init && Controller.Windows.count == 0) {NH_WSI_END(NH_SIGNAL_IDLE)}
    if (!Controller.init && Controller.Windows.count == 0) {NH_WSI_END(NH_SIGNAL_DONE)}

    Controller.init = NH_FALSE;

    for (int i = 0; i < Controller.Windows.count; ++i) {
        Nh_WSI_getInput(Nh_getFromLinkedList(&Controller.Windows, i));
    }

NH_WSI_END(NH_SIGNAL_OK)
}

// CREATE ==========================================================================================

NH_WSI_RESULT Nh_WSI_createWindow(
    Nh_WSI_Window *Window_p, char *title_p, Nh_PixelSize Size)
{
NH_WSI_BEGIN();

    Window_p->type = Nh_WSI_getType();
    NH_WSI_CHECK(Nh_WSI_initInput(&Window_p->Input))

    switch (Window_p->type)
    {
        case NH_WSI_TYPE_X11   : NH_WSI_CHECK(Nh_X11_createWindow(title_p, Size.width, Size.height, &Window_p->X11)) break;
        case NH_WSI_TYPE_WIN32 : NH_WSI_CHECK(Nh_Win32_createWindow(title_p, Size.width, Size.height, &Window_p->Win32)) break;
        default                : NH_WSI_DIAGNOSTIC_END(NH_WSI_ERROR_BAD_STATE)
    }

    NH_WSI_CHECK(Nh_appendToLinkedList(&Controller.Windows, Window_p))

NH_WSI_DIAGNOSTIC_END(NH_WSI_SUCCESS);
}

// DESTROY =========================================================================================

NH_WSI_RESULT Nh_WSI_destroyWindow(
    Nh_WSI_Window *Window_p)
{
NH_WSI_BEGIN()

    switch (Window_p->type)
    {
        case NH_WSI_TYPE_X11   : NH_WSI_CHECK(Nh_X11_destroyWindow(&Window_p->X11)) break;
        case NH_WSI_TYPE_WIN32 : NH_WSI_CHECK(Nh_Win32_destroyWindow(&Window_p->Win32)) break;
        default               : NH_WSI_DIAGNOSTIC_END(NH_WSI_ERROR_BAD_STATE)
    }

    Nh_WSI_freeInput(&Window_p->Input);
    Nh_removeFromLinkedList2(&Controller.Windows, Window_p, false);

NH_WSI_DIAGNOSTIC_END(NH_WSI_SUCCESS)
}

// SET =============================================================================================

NH_WSI_RESULT Nh_WSI_setWindowBackgroundColor(
    Nh_WSI_Window *Window_p, Nh_Color Color)
{
NH_WSI_BEGIN()

    switch (Window_p->type)
    {
        case NH_WSI_TYPE_X11   : NH_WSI_CHECK(Nh_X11_setWindowBackgroundColor(&Window_p->X11, Color)) break;
        case NH_WSI_TYPE_WIN32 : NH_WSI_CHECK(Nh_Win32_setWindowBackgroundColor(&Window_p->Win32, Color)) break;
        default               : NH_WSI_DIAGNOSTIC_END(NH_WSI_ERROR_BAD_STATE)
    }

NH_WSI_DIAGNOSTIC_END(NH_WSI_SUCCESS)
}

NH_WSI_RESULT Nh_WSI_setWindowTitle(
    Nh_WSI_Window *Window_p, char *title_p)
{
NH_WSI_BEGIN()

    switch (Window_p->type)
    {
        case NH_WSI_TYPE_X11   : NH_WSI_CHECK(Nh_X11_setWindowTitle(&Window_p->X11, title_p)) break;
        case NH_WSI_TYPE_WIN32 : NH_WSI_CHECK(Nh_Win32_setWindowTitle(&Window_p->Win32, title_p)) break;
        default               : NH_WSI_DIAGNOSTIC_END(NH_WSI_ERROR_BAD_STATE)
    }

NH_WSI_DIAGNOSTIC_END(NH_WSI_SUCCESS)
}

NH_WSI_RESULT Nh_WSI_setMouseCursor(
    Nh_WSI_Window *Window_p, NH_WSI_MOUSE_CURSOR type)
{
NH_WSI_BEGIN()

    switch (Window_p->type)
    {
        case NH_WSI_TYPE_X11   : NH_WSI_CHECK(Nh_X11_setMouseCursor(&Window_p->X11, type)) break;
        case NH_WSI_TYPE_WIN32 : NH_WSI_CHECK(Nh_Win32_setMouseCursor(&Window_p->Win32, type)) break;
        default               : NH_WSI_DIAGNOSTIC_END(NH_WSI_ERROR_BAD_STATE)
    }

NH_WSI_DIAGNOSTIC_END(NH_WSI_SUCCESS)
}


// DUMP ============================================================================================

char *Nh_WSI_dumpConfigureEvent(
    Nh_WSI_ConfigureEvent *Event_p)
{
NH_WSI_BEGIN()

    Nh_String String = Nh_initString(16);
    NH_CHECK(NULL, Nh_appendFormatToString(&String, "WINDOW SIZE:%d/%d POSITION:%d/%d\n", Event_p->Size.width, Event_p->Size.height, Event_p->Position.x, Event_p->Position.y))
  
NH_WSI_END(String.bytes_p)
}


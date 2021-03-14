// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE ========================================================================================

#include "Mouse.h"
#include "Window.h"

#include "../NhCore/String.h"
#include "../NhCore/Memory.h"

#include "Common/Macro.h"
#include NH_WSI_FLOW
#include NH_WSI_DEFAULT_CHECK

#include "../NhCore/Common/Macro.h"
#include NH_FLOW
#include NH_CUSTOM_CHECK

#include <string.h>

// INIT ============================================================================================

static void Nh_WSI_initMouseEventBuffer(
    Nh_RingIterator *Iterator_p, int itemCount)
{
NH_WSI_BEGIN()

    for (int i = 0; i < itemCount; ++i) 
    {
        Nh_WSI_MouseEvent *Event_p = Nh_advanceRingIterator(Iterator_p);
        Event_p->Position.x = -1;
        Event_p->Position.y = -1;
        Event_p->type    = NH_WSI_MOUSE_UNDEFINED;
        Event_p->trigger = NH_WSI_TRIGGER_UNDEFINED;
    }

NH_WSI_SILENT_END()
}

NH_WSI_RESULT Nh_WSI_initMouse(
    Nh_WSI_Mouse *Mouse_p, int bufferSize)
{
NH_WSI_BEGIN()

    NH_CHECK(NH_WSI_ERROR_BAD_STATE, Nh_initRingIterator(
        &Mouse_p->Events, bufferSize, sizeof(Nh_WSI_MouseEvent), Nh_WSI_initMouseEventBuffer
    ))
    Nh_initRingIteratorMark(&Mouse_p->Mark);

NH_WSI_DIAGNOSTIC_END(NH_WSI_SUCCESS)
}

// DUMP ============================================================================================

char *Nh_WSI_dumpMouse(
    Nh_WSI_MouseEvent *Event_p)
{
NH_WSI_BEGIN()

    Nh_String String = Nh_initString(16);

    NH_CHECK(NULL, Nh_appendFormatToString(&String, "MOUSE %d/%d ", Event_p->Position.x, Event_p->Position.y))
    NH_CHECK(NULL, Nh_appendFormatToString(&String, "%s\n", Nh_WSI_stringifyTrigger(Event_p->trigger)))
  
//    Nh_WSI_stringifyMouseState(MouseEvent_p->type, str_p);
//    NH_WSI_CHECK(NULL, Nh_appendFormatToString(String_p, "Type   : %s\n", str_p))

NH_WSI_END(String.bytes_p)
}

// STRINGIFY =======================================================================================

NH_WSI_RESULT Nh_WSI_stringifyMouseState(
    NH_WSI_MOUSE_FLAG state, char *string_p)
{
NH_WSI_BEGIN()

    int index = 0;
    char *values_pp[] = {
        "LEFT ", 
        "RIGHT ",  
        "MIDDLE ",
        "SCROLL ", 
        "MOVE ",   
    };

    if (state & NH_WSI_MOUSE_FLAG_LEFT)   {memcpy(string_p + index, values_pp[0], sizeof(char) * 5); index += 5;}
    if (state & NH_WSI_MOUSE_FLAG_RIGHT)  {memcpy(string_p + index, values_pp[1], sizeof(char) * 6); index += 6;}
    if (state & NH_WSI_MOUSE_FLAG_MIDDLE) {memcpy(string_p + index, values_pp[2], sizeof(char) * 7); index += 7;}
    if (state & NH_WSI_MOUSE_FLAG_SCROLL) {memcpy(string_p + index, values_pp[3], sizeof(char) * 7); index += 7;}
    if (state & NH_WSI_MOUSE_FLAG_MOVE)   {memcpy(string_p + index, values_pp[4], sizeof(char) * 5); index += 5;}

    string_p[index] = '\0';

NH_WSI_DIAGNOSTIC_END(NH_WSI_SUCCESS)
}


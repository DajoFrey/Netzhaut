// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE ========================================================================================

#include "../Header/Input.h"
#include "../Header/Macros.h"
#include "../Header/Document.h"
#include "../Header/Box.h"

#include "../../../Netzhaut/Memory.h"
#include "../../../Netzhaut/Tab.h"
#include "../../../Netzhaut/HashMap.h"
#include "../../../Netzhaut/RingIterator.h"

#include NH_FLOW
#include NH_DEFAULT_CHECK
#include NH_HTML_UTILS
#include NH_CSS_UTILS

#include <string.h>

// IMPLEMENT ======================================================================================

static NH_RESULT Nh_HTML_initMouseEventBuffer(
    Nh_RingIterator *Iterator_p, int itemCount)
{
NH_BEGIN()

    for (int i = 0; i < itemCount; ++i) 
    {
        Nh_HTML_MouseEvent *Event_p = Nh_advanceRingIterator(Iterator_p);
        Event_p->Target_p   = NULL;
        Event_p->Previous_p = NULL;
    }

NH_END(NH_SUCCESS)
}

NH_RESULT Nh_HTML_initInput(
    Nh_HTML_Input *Input_p)
{
NH_BEGIN()

    Input_p->Previous_p = NULL;
    Input_p->Mark = Nh_IO_initInputMark();

    NH_CHECK(Nh_initRingIterator(
        &Input_p->MouseEvents, NH_CONFIG.Settings.inputBufferSize, sizeof(Nh_HTML_MouseEvent), 
        Nh_HTML_initMouseEventBuffer
    ))

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

void Nh_HTML_freeInput(
    Nh_HTML_Input *Input_p)
{
NH_BEGIN()

    Nh_freeRingIterator(&Input_p->MouseEvents);

NH_SILENT_END()
}

// HIT =============================================================================================

static Nh_HTML_Node *Nh_HTML_getHit(
    Nh_HTML_Document *Document_p, Nh_PixelPosition MousePosition)
{
NH_BEGIN()

    float mouseX = MousePosition.x - Document_p->Content_p->Position.x;
    float mouseY = MousePosition.y - Document_p->Content_p->Position.y;

    Nh_HTML_Node *Return_p = NULL;

    for (int i = 0; i <= Document_p->Tree.Render.last; ++i) 
    {
        Nh_HTML_Node *Node_p = Document_p->Tree.Render.data_pp[i];

        Nh_HTML_PixelBox *Box_p = &Node_p->Box;
        while (Box_p != NULL)
        {
            // check if inside node
            if ((Box_p->Position.x < mouseX && (Box_p->Position.x + Box_p->Size.width) > mouseX)
            &&  (Box_p->Position.y < mouseY && (Box_p->Position.y + Box_p->Size.height) > mouseY))
            {
                Return_p = Node_p;
                break;
            }
            Box_p = Box_p->Next_p;
        }
    }

NH_END(Return_p)
}

// PROCESS =========================================================================================

static NH_RESULT Nh_HTML_processMouseEvent(
    Nh_HTML_Document *Document_p, Nh_IO_MouseEvent *Event_p)
{
NH_BEGIN()

    Nh_HTML_Node *Target_p = Nh_HTML_getHit(Document_p, Event_p->Position);

    if (Target_p != NULL && Event_p->type == NH_IO_MOUSE_MOVE) {
        Nh_IO_setMouseCursor(&Document_p->Content_p->Tab_p->Window_p->IO, Nh_CSS_normalizeMouseCursor(Target_p));
    }

    Nh_HTML_MouseEvent *Tmp_p = Nh_advanceRingIterator(&Document_p->Input.MouseEvents);
    Tmp_p->Target_p   = Target_p;
    Tmp_p->Event      = *Event_p;
    Tmp_p->Previous_p = Document_p->Input.Previous_p;

    Document_p->Input.Previous_p = Target_p;

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

NH_RESULT Nh_HTML_processInput(
    Nh_HTML_Document *Document_p)
{
NH_BEGIN()

    while (1) 
    {
        Nh_IO_MouseEvent *Event_p = Nh_incrementRingIteratorMark(
            &Document_p->Content_p->Input.Mouse.Events, &Document_p->Input.Mark.Mouse
        );
        if (Event_p == NULL) {break;}
        NH_CHECK(Nh_HTML_processMouseEvent(Document_p, Event_p))
    }

NH_DIAGNOSTIC_END(NH_SUCCESS)
}


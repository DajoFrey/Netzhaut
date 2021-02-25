// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE ========================================================================================

#include "../Header/Selection.h"
#include "../Header/Document.h"
#include "../Header/Macros.h"
#include "../Header/Node.h"
#include "../Header/Box.h"

#include "../../../Netzhaut/Tab.h"
#include "../../../Netzhaut/Memory.h"
#include "../../../Netzhaut/String.h"

#include NH_FLOW
#include NH_DEFAULT_CHECK
#include NH_HTML_UTILS
#include NH_CSS_UTILS

#include <string.h>

// CREATE ==========================================================================================

static void Nh_HTML_freeBoxes(
    Nh_HTML_PixelBox *Box_p)
{
NH_BEGIN()

    if (Box_p == NULL) {NH_SILENT_END()}

    Nh_HTML_freeBoxes(Box_p->Next_p);
    Nh_free(Box_p);

NH_SILENT_END()
}

static NH_RESULT Nh_HTML_recreateBoxes(
    Nh_HTML_Document *Document_p, Nh_HTML_SelectionRange *Range_p)
{
NH_BEGIN()

    NH_PIXEL y = -1;
    Nh_HTML_Node *Text_p = NULL;
    Nh_HTML_PixelBox *Box_p = NULL;

    for (int i = 0; i <= Document_p->Tree.Render.last; ++i) 
    {
        Nh_HTML_Node *Node_p = Document_p->Tree.Render.data_pp[i];

        if (Node_p->Pseudo.elements_p[NH_CSS_PSEUDO_ELEMENT_SELECTION]
        &&  Node_p->tag == NH_HTML_TAG_CHAR) 
        {
            if (y == Node_p->Box.Position.y && Text_p == Node_p->Parent_p) {
                Box_p->Size.width += Node_p->Box.Size.width;
                if (Node_p->Box.Size.height > Box_p->Size.height) {Box_p->Size.height = Node_p->Box.Size.height;}
            }
            else 
            {
                if (Text_p != Node_p->Parent_p) {
                    Nh_HTML_freeBoxes(Node_p->Parent_p->Box.Next_p);
                    float depth = Node_p->Parent_p->Box.depth;
                    memcpy(&Node_p->Parent_p->Box, &Node_p->Box, sizeof(Nh_HTML_PixelBox));
                    Node_p->Parent_p->Box.depth = depth;
                    Box_p = &Node_p->Parent_p->Box;
                }
                else {
                    Nh_HTML_PixelBox *NewBox_p = Nh_allocate(sizeof(Nh_HTML_PixelBox));
                    NH_CHECK_MEM(NewBox_p)
                    memcpy(NewBox_p, &Node_p->Box, sizeof(Nh_HTML_PixelBox));
                    NewBox_p->depth = Node_p->Parent_p->Box.depth;
                    Box_p->Next_p = NewBox_p;
                    Box_p = NewBox_p;
                }
                y = Node_p->Box.Position.y;
                Text_p = Node_p->Parent_p;
            }
        }
    }

NH_END(NH_SUCCESS)
}

static void Nh_HTML_select(
    Nh_HTML_Document *Document_p, Nh_HTML_SelectionRange *Range_p)
{
NH_BEGIN()

    NH_BOOL inRange = NH_FALSE;
    for (int i = 0; i <= Document_p->Tree.Render.last; ++i) 
    {
        Nh_HTML_Node *Node_p = Document_p->Tree.Render.data_pp[i];
        if (Node_p == Range_p->Begin_p) {inRange = NH_TRUE;}
        if (Node_p->tag == NH_HTML_TAG_CHAR && inRange) {
            Node_p->Pseudo.elements_p[NH_CSS_PSEUDO_ELEMENT_SELECTION] = NH_TRUE;
            Node_p->Parent_p->Pseudo.elements_p[NH_CSS_PSEUDO_ELEMENT_SELECTION] = NH_TRUE;
        }
        if (Node_p == Range_p->End_p) {break;}
    }

NH_SILENT_END()
}

NH_RESULT Nh_HTML_addSelectionRange(
    Nh_HTML_Document *Document_p, Nh_HTML_SelectionRange Range)
{
NH_BEGIN()

    Nh_HTML_SelectionRange *Range_p = Nh_allocate(sizeof(Nh_HTML_SelectionRange));
    NH_CHECK_MEM(Range_p)
    memcpy(Range_p, &Range, sizeof(Nh_HTML_SelectionRange));

    Nh_HTML_select(Document_p, Range_p);
    NH_CHECK(Nh_HTML_recreateBoxes(Document_p, Range_p))
  
    Document_p->Selection.update = NH_TRUE;

NH_END(NH_SUCCESS)
}

NH_RESULT Nh_HTML_removeAllSelectionRanges(
    Nh_HTML_Document *Document_p)
{
NH_BEGIN()

    Document_p->Selection.update = NH_TRUE;

NH_END(NH_SUCCESS)
}


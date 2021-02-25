// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "../Header/Properties.h"
#include "../Header/Background.h"
#include "../Header/Border.h"
#include "../Header/Position.h"
#include "../Header/Padding.h"
#include "../Header/Margin.h"
#include "../Header/List.h"
#include "../Header/Text.h"
#include "../Header/Macros.h"
#include "../Header/Associate.h"
#include "../Header/Helper.h"
#include "../Header/GenericProperty.h"
#include "../Header/Image.h"

#include "../../Graphics/Main/Header/Node.h"

#include "../../Netzhaut/Memory.h"
#include "../../HTML/Main/Header/Document.h"

#include NH_FLOW
#include NH_DEFAULT_CHECK
#include NH_HTML_UTILS

#include <string.h>
#include <ctype.h>

// INIT ============================================================================================

NH_RESULT Nh_CSS_initProperties(
    Nh_HTML_Node *Node_p)
{
NH_BEGIN()

    NH_INIT_LINKED_LIST(Node_p->Properties.Candidates)
    NH_INIT_LINKED_LIST(Node_p->Properties.Background.Images)

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

// COMPUTE =========================================================================================

static NH_RESULT Nh_CSS_computeAllNodeProperties(
    Nh_Content *Content_p, Nh_HTML_Node *Node_p, Nh_CSS_GenericProperty *Properties_pp[NH_CSS_PROPERTY_COUNT])
{
NH_BEGIN()

    // em length has text-font dependency, so we calculate text first
    NH_CHECK(Nh_CSS_computeTextProperties(Content_p, Node_p, Properties_pp))
    NH_CHECK(Nh_CSS_computeBackgroundProperties(Content_p, Node_p, Properties_pp))
    NH_CHECK(Nh_CSS_computeBorderProperties(Content_p, Node_p, Properties_pp))
    NH_CHECK(Nh_CSS_computePositionProperties(Content_p, Node_p, Properties_pp))
    NH_CHECK(Nh_CSS_computeMarginProperties(Content_p, Node_p, Properties_pp))
    NH_CHECK(Nh_CSS_computePaddingProperties(Content_p, Node_p, Properties_pp))
    NH_CHECK(Nh_CSS_computeListProperties(Node_p, Properties_pp))

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

NH_RESULT Nh_CSS_computeNodeProperties(
    Nh_Content *Content_p, Nh_HTML_Node *Node_p, NH_BOOL *recomputeTree_p, NH_BOOL lazy)
{
NH_BEGIN()

    Nh_CSS_GenericProperty *Properties_pp[NH_CSS_PROPERTY_COUNT];
    for (int i = 0; i < NH_CSS_PROPERTY_COUNT; ++i) {Properties_pp[i] = NULL;}

    Nh_CSS_configureGenericProperties(Node_p);
    Nh_CSS_getGenericProperties(Node_p, Properties_pp);

    NH_BOOL compute = NH_FALSE;
    for (int i = 0; i < Node_p->Properties.Candidates.count; ++i)
    {
        Nh_CSS_GenericProperty *Property_p = Nh_getFromLinkedList(&Node_p->Properties.Candidates, i);
        if (Property_p->update) {compute = NH_TRUE;}
        if (Property_p->update && Property_p->triggerRecompute && recomputeTree_p != NULL) {
            *recomputeTree_p = NH_TRUE;
        }
        Property_p->update = NH_FALSE;
    }

    if (compute || !lazy)
    {
        NH_CHECK(Nh_CSS_computeAllNodeProperties(Content_p, Node_p, Properties_pp))
    }

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

NH_RESULT Nh_CSS_computeProperties(
    Nh_HTML_Document *Document_p, NH_BOOL *recomputeTree_p, NH_BOOL lazy)
{
NH_BEGIN()

    *recomputeTree_p = NH_FALSE;

    for (int i = 0; i <= Document_p->Tree.RenderNoChars.last; ++i) 
    {
        NH_CHECK(Nh_CSS_computeNodeProperties(
            Document_p->Content_p, Document_p->Tree.RenderNoChars.data_pp[i], recomputeTree_p, lazy
        ))
    }

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

// DESTROY =========================================================================================

void Nh_CSS_destroyProperties(
    Nh_CSS_Properties *Properties_p)
{
NH_BEGIN()

    Nh_CSS_destroyImages(&Properties_p->Background.Images);
    Nh_CSS_destroyGenericProperties(&Properties_p->Candidates);

NH_SILENT_END()
}


// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "../Header/Margin.h"
#include "../Header/Helper.h"
#include "../Header/Macros.h"

#include "../../Netzhaut/Tab.h"
#include "../../Netzhaut/Memory.h"

#include NH_FLOW
#include NH_CSS_UTILS

#include <string.h>
#include <ctype.h>

// MARGIN COLLAPSE =================================================================================

static inline Nh_HTML_Node *Nh_CSS_getMarginCollapsePartner(
    Nh_HTML_Node *Node_p)
{
NH_BEGIN()

    if (Node_p->Parent_p == NULL) {NH_END(NULL)}

    Nh_HTML_Node *CollapsePartner_p = NULL;

    if (Node_p->Properties.Position.display == NH_CSS_DISPLAY_INLINE
    ||  Node_p->Properties.Position.display == NH_CSS_DISPLAY_INLINE_BLOCK)
    {
        for (int i = 0; i <= Node_p->Parent_p->Children.last; ++i) 
        {
            Nh_HTML_Node *Child_p = Node_p->Parent_p->Children.data_pp[i];

            if (Child_p == Node_p) {break;}
            if (Child_p->Properties.Position.display != NH_CSS_DISPLAY_INLINE
            ||  Child_p->Properties.Position.display != NH_CSS_DISPLAY_INLINE_BLOCK) {
                CollapsePartner_p = Node_p->Parent_p->Children.data_pp[i];
            }
        }
        if (CollapsePartner_p == NULL) {CollapsePartner_p = Node_p->Parent_p;}
    }
    else
    {
        for (int i = 0; i <= Node_p->Parent_p->Children.last; ++i) 
        {
            Nh_HTML_Node *Child_p = Node_p->Parent_p->Children.data_pp[i];
            if (Child_p == Node_p) {break;}
            CollapsePartner_p = Node_p->Parent_p->Children.data_pp[i];
        }
        if (CollapsePartner_p == NULL) {CollapsePartner_p = Node_p->Parent_p;}
    }

NH_END(CollapsePartner_p)
}

// https://developer.mozilla.org/en-US/docs/W../CSS/CSS_Box_Model/Mastering_margin_collapsing
static inline void Nh_CSS_collapseMargin(
    Nh_HTML_Node *Node_p)
{
NH_BEGIN()

    Nh_HTML_Node *CollapsePartner_p = Nh_CSS_getMarginCollapsePartner(Node_p);

    if (CollapsePartner_p != NULL)
    {
        if (CollapsePartner_p == Node_p->Parent_p) 
        {
            if (Node_p->Properties.Margin.top >= CollapsePartner_p->Properties.Margin.top)
            {
                // the body margin should not be affected by this
                if (Node_p->Parent_p->tag != NH_HTML_TAG_BODY) {
                    CollapsePartner_p->Properties.Margin.top = 0;
                }
            } 
            else {Node_p->Properties.Margin.top = 0;}

        } else if (Node_p->Properties.Margin.top > CollapsePartner_p->Properties.Margin.bottom) {
            CollapsePartner_p->Properties.Margin.bottom = 0;
        } 
        else {Node_p->Properties.Margin.top = 0;}
    } 

NH_SILENT_END()
}

// SET =============================================================================================

static inline void Nh_CSS_setMarginTop(
    Nh_HTML_Node *Node_p, Nh_Content *Content_p, Nh_CSS_GenericProperty *Property_p)
{
NH_BEGIN()

    Node_p->Properties.Margin.top = 0;

    if (Property_p != NULL) 
    {
        NH_PIXEL pixel = Nh_CSS_getLengthInPixel(Node_p, Content_p, Property_p->values_pp[0]);
        if (pixel != -1) {Node_p->Properties.Margin.top = pixel;}
    }

NH_SILENT_END()
}

static inline void Nh_CSS_setMarginBottom(
    Nh_HTML_Node *Node_p, Nh_Content *Content_p, Nh_CSS_GenericProperty *Property_p)
{
NH_BEGIN()

    Node_p->Properties.Margin.bottom = 0;

    if (Property_p != NULL) 
    {
        NH_PIXEL pixel = Nh_CSS_getLengthInPixel(Node_p, Content_p, Property_p->values_pp[0]);
        if (pixel != -1) {Node_p->Properties.Margin.bottom = pixel;}
    }

NH_SILENT_END()
}

static inline NH_BOOL Nh_CSS_setMarginBlockStart(
    Nh_HTML_Node *Node_p, Nh_Content *Content_p, Nh_CSS_GenericProperty *Property_p)
{
NH_BEGIN()

    if (Property_p != NULL) 
    {
        if (Node_p->Properties.Text.writingMode == NH_CSS_WRITING_MODE_HORIZONTAL_TB) {
            Nh_CSS_setMarginTop(Node_p, Content_p, Property_p);
        }

        NH_END(NH_TRUE)
    }

NH_END(NH_FALSE)
}

static inline NH_BOOL Nh_CSS_setMarginBlockEnd(
    Nh_HTML_Node *Node_p, Nh_Content *Content_p, Nh_CSS_GenericProperty *Property_p)
{
NH_BEGIN()

    if (Property_p != NULL) 
    {
        if (Node_p->Properties.Text.writingMode == NH_CSS_WRITING_MODE_HORIZONTAL_TB) {
            Nh_CSS_setMarginBottom(Node_p, Content_p, Property_p);
        }

        NH_END(NH_TRUE)
    }

NH_END(NH_FALSE)
}

static inline NH_BOOL Nh_CSS_setMarginBlock(
    Nh_HTML_Node *Node_p, Nh_Content *Content_p, Nh_CSS_GenericProperty *Property_p)
{
NH_BEGIN()

    if (Property_p != NULL) 
    {
        Nh_CSS_GenericProperty SingleValue;
        SingleValue.values_pp[0] = Property_p->values_pp[0];
        
        if (Property_p->valueCount == 2) 
        {
            Nh_CSS_setMarginBlockStart(Node_p, Content_p, &SingleValue);
            SingleValue.values_pp[0] = Property_p->values_pp[1];
            Nh_CSS_setMarginBlockEnd(Node_p, Content_p, &SingleValue);
        } 
        else if (Property_p->valueCount == 1) 
        {
            Nh_CSS_setMarginBlockStart(Node_p, Content_p, &SingleValue);
            Nh_CSS_setMarginBlockEnd(Node_p, Content_p, &SingleValue);
        }

        NH_END(NH_TRUE)
    }

NH_END(NH_FALSE)
}

static inline void Nh_CSS_setMarginLeft(
    Nh_HTML_Node *Node_p, Nh_Content *Content_p, Nh_CSS_GenericProperty *Property_p)
{
NH_BEGIN()

    Node_p->Properties.Margin.left = 0;

    if (Property_p != NULL) 
    {
        NH_PIXEL pixel = Nh_CSS_getLengthInPixel(Node_p, Content_p, Property_p->values_pp[0]);
        if (pixel != -1) {Node_p->Properties.Margin.left = pixel;}
    }

NH_SILENT_END()
}

static inline void Nh_CSS_setMarginRight(
    Nh_HTML_Node *Node_p, Nh_Content *Content_p, Nh_CSS_GenericProperty *Property_p)
{
NH_BEGIN()

    Node_p->Properties.Margin.right = 0;

    if (Property_p != NULL) 
    {
        NH_PIXEL pixel = Nh_CSS_getLengthInPixel(Node_p, Content_p, Property_p->values_pp[0]);
        if (pixel != -1) {Node_p->Properties.Margin.right = pixel;}
    }

NH_SILENT_END()
}

static inline NH_BOOL Nh_CSS_setMargin(
    Nh_HTML_Node *Node_p, Nh_Content *Content_p, Nh_CSS_GenericProperty *Property_p)
{
NH_BEGIN()

    if (Property_p != NULL) 
    {
        Nh_CSS_GenericProperty SingleValue;
        SingleValue.values_pp = Nh_allocate(sizeof(void*));
        
        switch (Property_p->valueCount)
        {
            case 1:
                SingleValue.values_pp[0] = Property_p->values_pp[0];
                Nh_CSS_setMarginBottom(Node_p, Content_p, &SingleValue);
                Nh_CSS_setMarginLeft(Node_p, Content_p, &SingleValue);
                Nh_CSS_setMarginRight(Node_p, Content_p, &SingleValue);
                Nh_CSS_setMarginTop(Node_p, Content_p, &SingleValue);
                break;
            
            case 2:
                SingleValue.values_pp[0] = Property_p->values_pp[0];
                Nh_CSS_setMarginTop(Node_p, Content_p, &SingleValue);
                Nh_CSS_setMarginBottom(Node_p, Content_p, &SingleValue);
                SingleValue.values_pp[0] = Property_p->values_pp[1];
                Nh_CSS_setMarginRight(Node_p, Content_p, &SingleValue);
                Nh_CSS_setMarginLeft(Node_p, Content_p, &SingleValue);
                break;
            
            case 3:
                SingleValue.values_pp[0] = Property_p->values_pp[0];
                Nh_CSS_setMarginTop(Node_p, Content_p, &SingleValue);
                SingleValue.values_pp[0] = Property_p->values_pp[1];
                Nh_CSS_setMarginRight(Node_p, Content_p, &SingleValue);
                Nh_CSS_setMarginLeft(Node_p, Content_p, &SingleValue);
                SingleValue.values_pp[0] = Property_p->values_pp[2];
                Nh_CSS_setMarginBottom(Node_p, Content_p, &SingleValue);
                break;
            
            case 4:
                SingleValue.values_pp[0] = Property_p->values_pp[0];
                Nh_CSS_setMarginTop(Node_p, Content_p, &SingleValue);
                SingleValue.values_pp[0] = Property_p->values_pp[1];
                Nh_CSS_setMarginRight(Node_p, Content_p, &SingleValue);
                SingleValue.values_pp[0] = Property_p->values_pp[2];
                Nh_CSS_setMarginBottom(Node_p, Content_p, &SingleValue);
                SingleValue.values_pp[0] = Property_p->values_pp[3];
                Nh_CSS_setMarginLeft(Node_p, Content_p, &SingleValue);
                break;
        }
        
        Nh_free(SingleValue.values_pp);
        NH_END(NH_TRUE)
    }

NH_END(NH_FALSE)
}

// COMPUTE =========================================================================================

NH_RESULT Nh_CSS_computeMarginProperties(
    Nh_Content *Content_p, Nh_HTML_Node *Node_p, Nh_CSS_GenericProperty **Properties_pp)
{
NH_BEGIN()

    if (!Nh_CSS_setMarginBlock(Node_p, Content_p, Properties_pp[NH_CSS_PROPERTY_MARGIN_BLOCK]))
    {
        Nh_CSS_setMarginBlockStart(Node_p, Content_p, Properties_pp[NH_CSS_PROPERTY_MARGIN_BLOCK_START]);
        Nh_CSS_setMarginBlockEnd(Node_p, Content_p, Properties_pp[NH_CSS_PROPERTY_MARGIN_BLOCK_END]);
    }
    
    if (!Nh_CSS_setMargin(Node_p, Content_p, Properties_pp[NH_CSS_PROPERTY_MARGIN])) 
    {
        Nh_CSS_setMarginBottom(Node_p, Content_p, Properties_pp[NH_CSS_PROPERTY_MARGIN_BOTTOM]);
        Nh_CSS_setMarginLeft(Node_p, Content_p, Properties_pp[NH_CSS_PROPERTY_MARGIN_LEFT]);
        Nh_CSS_setMarginRight(Node_p, Content_p, Properties_pp[NH_CSS_PROPERTY_MARGIN_RIGHT]);
        Nh_CSS_setMarginTop(Node_p, Content_p, Properties_pp[NH_CSS_PROPERTY_MARGIN_TOP]);
    }

    Nh_CSS_collapseMargin(Node_p);

NH_DIAGNOSTIC_END(NH_SUCCESS)
}


// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "../Header/Padding.h"
#include "../Header/Helper.h"
#include "../Header/Macros.h"

#include "../../Netzhaut/Tab.h"
#include "../../Netzhaut/Memory.h"

#include NH_FLOW
#include NH_CSS_UTILS

#include <string.h>
#include <ctype.h>

// SET =============================================================================================

static inline void Nh_JS_setPaddingBottom(
    Nh_HTML_Node *Node_p, Nh_Content *Content_p, Nh_CSS_GenericProperty *Property_p)
{
NH_BEGIN()

    Node_p->Properties.Padding.bottom = 0;

    if (Property_p != NULL) 
    {
        int pixel = Nh_CSS_getLengthInPixel(Node_p, Content_p, Property_p->values_pp[0]);
        if (pixel != -1) {Node_p->Properties.Padding.bottom = pixel;}
    } 

NH_SILENT_END()
}

static inline void Nh_JS_setPaddingLeft(
    Nh_HTML_Node *Node_p, Nh_Content *Content_p, Nh_CSS_GenericProperty *Property_p)
{
NH_BEGIN()

    Node_p->Properties.Padding.left = 0;
 
    if (Property_p != NULL) 
    {
        int pixel = Nh_CSS_getLengthInPixel(Node_p, Content_p, Property_p->values_pp[0]);
        if (pixel != -1) {Node_p->Properties.Padding.left = pixel;}
    }

NH_SILENT_END()
}

static inline void Nh_JS_setPaddingRight(
    Nh_HTML_Node *Node_p, Nh_Content *Content_p, Nh_CSS_GenericProperty *Property_p)
{
NH_BEGIN()

    Node_p->Properties.Padding.right = 0;

    if (Property_p != NULL) 
    {
        int pixel = Nh_CSS_getLengthInPixel(Node_p, Content_p, Property_p->values_pp[0]);
        if (pixel != -1) {Node_p->Properties.Padding.right = pixel;}
    } 

NH_SILENT_END()
}

static inline void Nh_JS_setPaddingTop(
    Nh_HTML_Node *Node_p, Nh_Content *Content_p, Nh_CSS_GenericProperty *Property_p)
{
NH_BEGIN()

    Node_p->Properties.Padding.top = 0;

    if (Property_p != NULL) 
    {
        int pixel = Nh_CSS_getLengthInPixel(Node_p, Content_p, Property_p->values_pp[0]);
        if (pixel != -1) {Node_p->Properties.Padding.top = pixel;}
    } 

NH_SILENT_END()
}

static inline NH_BOOL Nh_JS_setPadding(
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
                Nh_JS_setPaddingBottom(Node_p, Content_p, &SingleValue);
                Nh_JS_setPaddingLeft(Node_p, Content_p, &SingleValue);
                Nh_JS_setPaddingRight(Node_p, Content_p, &SingleValue);
                Nh_JS_setPaddingTop(Node_p, Content_p, &SingleValue);
                break;
            
            case 2:
                SingleValue.values_pp[0] = Property_p->values_pp[0];
                Nh_JS_setPaddingTop(Node_p, Content_p, &SingleValue);
                Nh_JS_setPaddingBottom(Node_p, Content_p, &SingleValue);
                SingleValue.values_pp[0] = Property_p->values_pp[1];
                Nh_JS_setPaddingRight(Node_p, Content_p, &SingleValue);
                Nh_JS_setPaddingLeft(Node_p, Content_p, &SingleValue);
                break;
            
            case 3:
                SingleValue.values_pp[0] = Property_p->values_pp[0];
                Nh_JS_setPaddingTop(Node_p, Content_p, &SingleValue);
                SingleValue.values_pp[0] = Property_p->values_pp[1];
                Nh_JS_setPaddingRight(Node_p, Content_p, &SingleValue);
                Nh_JS_setPaddingLeft(Node_p, Content_p, &SingleValue);
                SingleValue.values_pp[0] = Property_p->values_pp[2];
                Nh_JS_setPaddingBottom(Node_p, Content_p, &SingleValue);
                break;
            
            case 4:
                SingleValue.values_pp[0] = Property_p->values_pp[0];
                Nh_JS_setPaddingTop(Node_p, Content_p, &SingleValue);
                SingleValue.values_pp[0] = Property_p->values_pp[1];
                Nh_JS_setPaddingRight(Node_p, Content_p, &SingleValue);
                SingleValue.values_pp[0] = Property_p->values_pp[2];
                Nh_JS_setPaddingBottom(Node_p, Content_p, &SingleValue);
                SingleValue.values_pp[0] = Property_p->values_pp[3];
                Nh_JS_setPaddingLeft(Node_p, Content_p, &SingleValue);
                break;
        }
        
        free(SingleValue.values_pp);
        NH_END(NH_TRUE)
    }

NH_END(NH_FALSE)
}

// COMPUTE =========================================================================================

NH_RESULT Nh_CSS_computePaddingProperties(
    Nh_Content *Content_p, Nh_HTML_Node *Node_p, Nh_CSS_GenericProperty **Properties_pp)
{
NH_BEGIN()

    if (!Nh_JS_setPadding(Node_p, Content_p, Properties_pp[NH_CSS_PROPERTY_PADDING]))
    {
        Nh_JS_setPaddingBottom(Node_p, Content_p, Properties_pp[NH_CSS_PROPERTY_PADDING_BOTTOM]);
        Nh_JS_setPaddingLeft(Node_p, Content_p, Properties_pp[NH_CSS_PROPERTY_PADDING_LEFT]);
        Nh_JS_setPaddingRight(Node_p, Content_p, Properties_pp[NH_CSS_PROPERTY_PADDING_RIGHT]);
        Nh_JS_setPaddingTop(Node_p, Content_p, Properties_pp[NH_CSS_PROPERTY_PADDING_TOP]);
    }

NH_DIAGNOSTIC_END(NH_SUCCESS)
}


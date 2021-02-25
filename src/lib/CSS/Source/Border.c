// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "../Header/Border.h"
#include "../Header/Color.h"
#include "../Header/Helper.h"
#include "../Header/Macros.h"

#include "../../Netzhaut/Tab.h"
#include "../../Netzhaut/Memory.h"

#include NH_FLOW
#include NH_CSS_UTILS

#include <string.h>
#include <ctype.h>

// SET =============================================================================================

static inline void Nh_CSS_setBorderBottom(
    Nh_HTML_Node *Node_p, Nh_Content *Content_p, Nh_CSS_GenericProperty *Property_p)
{
NH_BEGIN()

//    if (Property_p != NULL) 
//    {
//        int pixel = Nh_CSS_getLengthInPixel(Node_p, Content_p, Property_p->values_pp[0]);
//        float relative = ((float)pixel) / ((float)Content_p->Size.height);
//        Node_p->Properties.Position.Border.bottom = relative * 2.0;
//    } 

NH_SILENT_END()
}

static inline void Nh_CSS_setBorderLeft(
    Nh_HTML_Node *Node_p, Nh_Content *Content_p, Nh_CSS_GenericProperty *Property_p)
{
NH_BEGIN()

//    if (Property_p != NULL) 
//    {
//        int pixel = Nh_CSS_getLengthInPixel(Node_p, Content_p, Property_p->values_pp[0]);
//        float relative = ((float)pixel) / ((float)Content_p->Size.width);
//        Node_p->Properties.Position.Border.left = relative * 2.0;
//    }

NH_SILENT_END()
}

static inline void Nh_CSS_setBorderRight(
    Nh_HTML_Node *Node_p, Nh_Content *Content_p, Nh_CSS_GenericProperty *Property_p)
{
NH_BEGIN()

//    if (Property_p != NULL) 
//    {
//        int pixel = Nh_CSS_getLengthInPixel(Node_p, Content_p, Property_p->values_pp[0]);
//        float relative = ((float)pixel) / ((float)Content_p->Size.width);
//        Node_p->Properties.Position.Border.right = relative * 2.0;
//    } 

NH_SILENT_END()
}

static inline void Nh_CSS_setBorderTop(
    Nh_HTML_Node *Node_p, Nh_Content *Content_p, Nh_CSS_GenericProperty *Property_p)
{
NH_BEGIN()

//    if (Property_p != NULL) 
//    {
//        int pixel = Nh_CSS_getLengthInPixel(Node_p, Content_p, Property_p->values_pp[0]);
//        float relative = ((float)pixel) / ((float)Content_p->Size.height);
//        Node_p->Properties.Position.Border.top = relative * 2.0;
//    } 

NH_SILENT_END()
}

static inline bool Nh_CSS_setBorder(
    Nh_HTML_Node *Node_p, Nh_Content *Content_p, Nh_CSS_GenericProperty *Property_p)
{
NH_BEGIN()

//    if (Property_p != NULL) 
//    {
//        Nh_CSS_GenericProperty SingleValue;
//        SingleValue.values_pp = Nh_allocate(sizeof(void*));
//        
//        switch (Property_p->valueCount)
//        {
//            case 1:
//                SingleValue.values_pp[0] = Property_p->values_pp[0];
//                break;
//            
//            case 2:
//                SingleValue.values_pp[0] = Property_p->values_pp[0];
//                SingleValue.values_pp[0] = Property_p->values_pp[1];
//                break;
//            
//            case 3:
//                SingleValue.values_pp[0] = Property_p->values_pp[0];
//                SingleValue.values_pp[0] = Property_p->values_pp[1];
//                SingleValue.values_pp[0] = Property_p->values_pp[2];
//                break;
//            
//            case 4:
//                SingleValue.values_pp[0] = Property_p->values_pp[0];
//                SingleValue.values_pp[0] = Property_p->values_pp[1];
//                SingleValue.values_pp[0] = Property_p->values_pp[2];
//                SingleValue.values_pp[0] = Property_p->values_pp[3];
//                break;
//        }
//        
//        free(SingleValue.values_pp);
//        NH_END(true)
//    }

NH_END(false)
}

static inline void Nh_CSS_setBorderTopColor(
    Nh_HTML_Node *Node_p, Nh_CSS_GenericProperty *Property_p)
{
NH_BEGIN()

    for (int i = 0; i < 3; ++i) {Node_p->Properties.Border.Color.top_p[i] = 0.0f;}
    Node_p->Properties.Border.Color.top_p[3] = 1.0f;

    if (Property_p != NULL) 
    {
        char color_p[64];
        strcpy(color_p, Property_p->values_pp[0]);
        color_p[strlen(Property_p->values_pp[0])] = '\0';
        Nh_CSS_getColor(color_p, Node_p->Properties.Border.Color.top_p);
    }

NH_SILENT_END()
}

static inline void Nh_CSS_setBorderRightColor(
    Nh_HTML_Node *Node_p, Nh_CSS_GenericProperty *Property_p)
{
NH_BEGIN()

    for (int i = 0; i < 3; ++i) {Node_p->Properties.Border.Color.right_p[i] = 0.0f;}
    Node_p->Properties.Border.Color.right_p[3] = 1.0f;

    if (Property_p != NULL) 
    {
        char color_p[64];
        strcpy(color_p, Property_p->values_pp[0]);
        color_p[strlen(Property_p->values_pp[0])] = '\0';
        Nh_CSS_getColor(color_p, Node_p->Properties.Border.Color.right_p);
    }

NH_SILENT_END()
}

static inline void Nh_CSS_setBorderBottomColor(
    Nh_HTML_Node *Node_p, Nh_CSS_GenericProperty *Property_p)
{
NH_BEGIN()

    for (int i = 0; i < 3; ++i) {Node_p->Properties.Border.Color.bottom_p[i] = 0.0f;}
    Node_p->Properties.Border.Color.bottom_p[3] = 1.0f;

    if (Property_p != NULL) 
    {
        char color_p[64];
        strcpy(color_p, Property_p->values_pp[0]);
        color_p[strlen(Property_p->values_pp[0])] = '\0';
        Nh_CSS_getColor(color_p, Node_p->Properties.Border.Color.bottom_p);
    }

NH_SILENT_END()
}

static inline void Nh_CSS_setBorderLeftColor(
    Nh_HTML_Node *Node_p, Nh_CSS_GenericProperty *Property_p)
{
NH_BEGIN()

    for (int i = 0; i < 3; ++i) {Node_p->Properties.Border.Color.left_p[i] = 0.0f;}
    Node_p->Properties.Border.Color.left_p[3] = 1.0f;

    if (Property_p != NULL) 
    {
        char color_p[64];
        strcpy(color_p, Property_p->values_pp[0]);
        color_p[strlen(Property_p->values_pp[0])] = '\0';
        Nh_CSS_getColor(color_p, Node_p->Properties.Border.Color.left_p);
    }

NH_SILENT_END()
}

static inline bool Nh_CSS_setBorderColor(
    Nh_HTML_Node *Node_p, Nh_CSS_GenericProperty *Property_p)
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
                Nh_CSS_setBorderBottomColor(Node_p, &SingleValue);
                Nh_CSS_setBorderLeftColor(Node_p, &SingleValue);
                Nh_CSS_setBorderRightColor(Node_p, &SingleValue);
                Nh_CSS_setBorderTopColor(Node_p, &SingleValue);
                break;
            case 2:
                SingleValue.values_pp[0] = Property_p->values_pp[0];
                Nh_CSS_setBorderTopColor(Node_p, &SingleValue);
                Nh_CSS_setBorderBottomColor(Node_p, &SingleValue);
                SingleValue.values_pp[0] = Property_p->values_pp[1];
                Nh_CSS_setBorderRightColor(Node_p, &SingleValue);
                Nh_CSS_setBorderLeftColor(Node_p, &SingleValue);
                break;
            case 3:
                SingleValue.values_pp[0] = Property_p->values_pp[0];
                Nh_CSS_setBorderTopColor(Node_p, &SingleValue);
                SingleValue.values_pp[0] = Property_p->values_pp[1];
                Nh_CSS_setBorderRightColor(Node_p, &SingleValue);
                Nh_CSS_setBorderLeftColor(Node_p, &SingleValue);
                SingleValue.values_pp[0] = Property_p->values_pp[2];
                Nh_CSS_setBorderBottomColor(Node_p, &SingleValue);
                break;
            case 4:
                SingleValue.values_pp[0] = Property_p->values_pp[0];
                Nh_CSS_setBorderTopColor(Node_p, &SingleValue);
                SingleValue.values_pp[0] = Property_p->values_pp[1];
                Nh_CSS_setBorderRightColor(Node_p, &SingleValue);
                SingleValue.values_pp[0] = Property_p->values_pp[2];
                Nh_CSS_setBorderBottomColor(Node_p, &SingleValue);
                SingleValue.values_pp[0] = Property_p->values_pp[3];
                Nh_CSS_setBorderLeftColor(Node_p, &SingleValue);
                break;
        }
        
        Nh_free(SingleValue.values_pp);
        NH_END(true)
    }

NH_END(false)
}

static inline NH_CSS_BORDER_STYLE Nh_CSS_parseBorderStyle(
    char *str_p)
{
NH_BEGIN()

         if (strstr(str_p, "none"))   {NH_DIAGNOSTIC_END(NH_CSS_BORDER_STYLE_NONE)}
    else if (strstr(str_p, "hidden")) {NH_DIAGNOSTIC_END(NH_CSS_BORDER_STYLE_HIDDEN)}
    else if (strstr(str_p, "dotted")) {NH_DIAGNOSTIC_END(NH_CSS_BORDER_STYLE_DOTTED)}
    else if (strstr(str_p, "dashed")) {NH_DIAGNOSTIC_END(NH_CSS_BORDER_STYLE_DASHED)}
    else if (strstr(str_p, "solid"))  {NH_DIAGNOSTIC_END(NH_CSS_BORDER_STYLE_SOLID)}
    else if (strstr(str_p, "double")) {NH_DIAGNOSTIC_END(NH_CSS_BORDER_STYLE_DOUBLE)}
    else if (strstr(str_p, "groove")) {NH_DIAGNOSTIC_END(NH_CSS_BORDER_STYLE_GROOVE)}
    else if (strstr(str_p, "ridge"))  {NH_DIAGNOSTIC_END(NH_CSS_BORDER_STYLE_RIDGE)}
    else if (strstr(str_p, "inset"))  {NH_DIAGNOSTIC_END(NH_CSS_BORDER_STYLE_INSET)}
    else if (strstr(str_p, "outset")) {NH_DIAGNOSTIC_END(NH_CSS_BORDER_STYLE_OUTSET)}

NH_DIAGNOSTIC_END(NH_CSS_BORDER_STYLE_NONE)
}

static inline void Nh_CSS_setBorderTopStyle(
    Nh_HTML_Node *Node_p, Nh_CSS_GenericProperty *Property_p)
{
NH_BEGIN()

    Node_p->Properties.Border.Style.top = NH_CSS_BORDER_STYLE_NONE;

    if (Property_p != NULL) {
        Node_p->Properties.Border.Style.top = Nh_CSS_parseBorderStyle(Property_p->values_pp[0]); 
    }

NH_SILENT_END()
}

static inline void Nh_CSS_setBorderRightStyle(
    Nh_HTML_Node *Node_p, Nh_CSS_GenericProperty *Property_p)
{
NH_BEGIN()

    Node_p->Properties.Border.Style.right = NH_CSS_BORDER_STYLE_NONE;

    if (Property_p != NULL) {
        Node_p->Properties.Border.Style.right = Nh_CSS_parseBorderStyle(Property_p->values_pp[0]); 
    }

NH_SILENT_END()
}

static inline void Nh_CSS_setBorderBottomStyle(
    Nh_HTML_Node *Node_p, Nh_CSS_GenericProperty *Property_p)
{
NH_BEGIN()

    Node_p->Properties.Border.Style.bottom = NH_CSS_BORDER_STYLE_NONE;

    if (Property_p != NULL) {
        Node_p->Properties.Border.Style.bottom = Nh_CSS_parseBorderStyle(Property_p->values_pp[0]); 
    }

NH_SILENT_END()
}

static inline void Nh_CSS_setBorderLeftStyle(
    Nh_HTML_Node *Node_p, Nh_CSS_GenericProperty *Property_p)
{
NH_BEGIN()

    Node_p->Properties.Border.Style.left = NH_CSS_BORDER_STYLE_NONE;

    if (Property_p != NULL) {
        Node_p->Properties.Border.Style.left = Nh_CSS_parseBorderStyle(Property_p->values_pp[0]); 
    }

NH_SILENT_END()
}

static inline bool Nh_CSS_setBorderStyle(
    Nh_HTML_Node *Node_p, Nh_CSS_GenericProperty *Property_p)
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
                Nh_CSS_setBorderBottomStyle(Node_p, &SingleValue);
                Nh_CSS_setBorderLeftStyle(Node_p, &SingleValue);
                Nh_CSS_setBorderRightStyle(Node_p, &SingleValue);
                Nh_CSS_setBorderTopStyle(Node_p, &SingleValue);
                break;
            case 2:
                SingleValue.values_pp[0] = Property_p->values_pp[0];
                Nh_CSS_setBorderTopStyle(Node_p, &SingleValue);
                Nh_CSS_setBorderBottomStyle(Node_p, &SingleValue);
                SingleValue.values_pp[0] = Property_p->values_pp[1];
                Nh_CSS_setBorderRightStyle(Node_p, &SingleValue);
                Nh_CSS_setBorderLeftStyle(Node_p, &SingleValue);
                break;
            case 3:
                SingleValue.values_pp[0] = Property_p->values_pp[0];
                Nh_CSS_setBorderTopStyle(Node_p, &SingleValue);
                SingleValue.values_pp[0] = Property_p->values_pp[1];
                Nh_CSS_setBorderRightStyle(Node_p, &SingleValue);
                Nh_CSS_setBorderLeftStyle(Node_p, &SingleValue);
                SingleValue.values_pp[0] = Property_p->values_pp[2];
                Nh_CSS_setBorderBottomStyle(Node_p, &SingleValue);
                break;
            case 4:
                SingleValue.values_pp[0] = Property_p->values_pp[0];
                Nh_CSS_setBorderTopStyle(Node_p, &SingleValue);
                SingleValue.values_pp[0] = Property_p->values_pp[1];
                Nh_CSS_setBorderRightStyle(Node_p, &SingleValue);
                SingleValue.values_pp[0] = Property_p->values_pp[2];
                Nh_CSS_setBorderBottomStyle(Node_p, &SingleValue);
                SingleValue.values_pp[0] = Property_p->values_pp[3];
                Nh_CSS_setBorderLeftStyle(Node_p, &SingleValue);
                break;
        }
        
        Nh_free(SingleValue.values_pp);
        NH_END(true)
    }

NH_END(false)
}

static inline void Nh_CSS_setBorderBottomWidth(
    Nh_HTML_Node *Node_p, Nh_Content *Content_p, Nh_CSS_GenericProperty *Property_p)
{
NH_BEGIN()

    Node_p->Properties.Border.Width.bottom = 5;

    if (Property_p != NULL) 
    {
        int pixel = 0;

             if (strstr(Property_p->values_pp[0], "thin"))   {pixel = 2;}
        else if (strstr(Property_p->values_pp[0], "medium")) {pixel = 5;}
        else if (strstr(Property_p->values_pp[0], "thick"))  {pixel = 11;}
        else {pixel = Nh_CSS_getLengthInPixel(Node_p, Content_p, Property_p->values_pp[0]);}

        if (pixel != -1) {Node_p->Properties.Border.Width.bottom = pixel;}
    } 

NH_SILENT_END()
}

static inline void Nh_CSS_setBorderLeftWidth(
    Nh_HTML_Node *Node_p, Nh_Content *Content_p, Nh_CSS_GenericProperty *Property_p)
{
NH_BEGIN()

    Node_p->Properties.Border.Width.left = 5;

    if (Property_p != NULL) 
    {
        int pixel = 0;

             if (strstr(Property_p->values_pp[0], "thin"))   {pixel = 2;}
        else if (strstr(Property_p->values_pp[0], "medium")) {pixel = 5;}
        else if (strstr(Property_p->values_pp[0], "thick"))  {pixel = 11;}
        else {pixel = Nh_CSS_getLengthInPixel(Node_p, Content_p, Property_p->values_pp[0]);}

        if (pixel != -1) {Node_p->Properties.Border.Width.left = pixel;}
    }

NH_SILENT_END()
}

static inline void Nh_CSS_setBorderRightWidth(
    Nh_HTML_Node *Node_p, Nh_Content *Content_p, Nh_CSS_GenericProperty *Property_p)
{
NH_BEGIN()

    Node_p->Properties.Border.Width.right = 5;

    if (Property_p != NULL) 
    {
        int pixel = 0;

             if (strstr(Property_p->values_pp[0], "thin"))   {pixel = 2;}
        else if (strstr(Property_p->values_pp[0], "medium")) {pixel = 5;}
        else if (strstr(Property_p->values_pp[0], "thick"))  {pixel = 11;}
        else {pixel = Nh_CSS_getLengthInPixel(Node_p, Content_p, Property_p->values_pp[0]);}

        if (pixel != -1) {Node_p->Properties.Border.Width.right = pixel;}
    } 

NH_SILENT_END()
}

static inline void Nh_CSS_setBorderTopWidth(
    Nh_HTML_Node *Node_p, Nh_Content *Content_p, Nh_CSS_GenericProperty *Property_p)
{
NH_BEGIN()

    Node_p->Properties.Border.Width.top = 5;

    if (Property_p != NULL) 
    {
        int pixel = 0;

             if (strstr(Property_p->values_pp[0], "thin"))   {pixel = 2;}
        else if (strstr(Property_p->values_pp[0], "medium")) {pixel = 5;}
        else if (strstr(Property_p->values_pp[0], "thick"))  {pixel = 11;}
        else {pixel = Nh_CSS_getLengthInPixel(Node_p, Content_p, Property_p->values_pp[0]);}

        if (pixel != -1) {Node_p->Properties.Border.Width.top = pixel;}
    } 

NH_SILENT_END()
}

static inline void Nh_CSS_setBorderWidth(
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
                Nh_CSS_setBorderBottomWidth(Node_p, Content_p, &SingleValue);
                Nh_CSS_setBorderLeftWidth(Node_p, Content_p, &SingleValue);
                Nh_CSS_setBorderRightWidth(Node_p, Content_p, &SingleValue);
                Nh_CSS_setBorderTopWidth(Node_p, Content_p, &SingleValue);
                break;
            case 2:
                SingleValue.values_pp[0] = Property_p->values_pp[0];
                Nh_CSS_setBorderTopWidth(Node_p, Content_p, &SingleValue);
                Nh_CSS_setBorderBottomWidth(Node_p, Content_p, &SingleValue);
                SingleValue.values_pp[0] = Property_p->values_pp[1];
                Nh_CSS_setBorderRightWidth(Node_p, Content_p, &SingleValue);
                Nh_CSS_setBorderLeftWidth(Node_p, Content_p, &SingleValue);
                break;
            case 3:
                SingleValue.values_pp[0] = Property_p->values_pp[0];
                Nh_CSS_setBorderTopWidth(Node_p, Content_p, &SingleValue);
                SingleValue.values_pp[0] = Property_p->values_pp[1];
                Nh_CSS_setBorderRightWidth(Node_p, Content_p, &SingleValue);
                Nh_CSS_setBorderLeftWidth(Node_p, Content_p, &SingleValue);
                SingleValue.values_pp[0] = Property_p->values_pp[2];
                Nh_CSS_setBorderBottomWidth(Node_p, Content_p, &SingleValue);
                break;
            case 4:
                SingleValue.values_pp[0] = Property_p->values_pp[0];
                Nh_CSS_setBorderTopWidth(Node_p, Content_p, &SingleValue);
                SingleValue.values_pp[0] = Property_p->values_pp[1];
                Nh_CSS_setBorderRightWidth(Node_p, Content_p, &SingleValue);
                SingleValue.values_pp[0] = Property_p->values_pp[2];
                Nh_CSS_setBorderBottomWidth(Node_p, Content_p, &SingleValue);
                SingleValue.values_pp[0] = Property_p->values_pp[3];
                Nh_CSS_setBorderLeftWidth(Node_p, Content_p, &SingleValue);
                break;
        }
        
        Nh_free(SingleValue.values_pp);
    }

NH_SILENT_END()
}

static inline void Nh_CSS_setBorderTopLeftRadius(
    Nh_HTML_Node *Node_p, Nh_Content *Content_p, Nh_CSS_GenericProperty *Property_p)
{
NH_BEGIN()

    Node_p->Properties.Border.Radii.Values_p[0].topLeft = 0.0f;
    Node_p->Properties.Border.Radii.Values_p[1].topLeft = 0.0f;

    if (Property_p != NULL) 
    {
        switch (Property_p->valueCount)
        {
            case 1:
            {
                int pixel = Nh_CSS_getLengthInPixel(Node_p, Content_p, Property_p->values_pp[0]);
                if (pixel != -1) {Node_p->Properties.Border.Radii.Values_p[0].topLeft = pixel;}
                break;
            }
        }
    }

NH_SILENT_END()
}

static inline void Nh_CSS_setBorderTopRightRadius(
    Nh_HTML_Node *Node_p, Nh_Content *Content_p, Nh_CSS_GenericProperty *Property_p)
{
NH_BEGIN()

    Node_p->Properties.Border.Radii.Values_p[0].topRight = 0.0f;
    Node_p->Properties.Border.Radii.Values_p[1].topRight = 0.0f;
    
    if (Property_p != NULL) 
    {
        switch (Property_p->valueCount)
        {
            case 1:
            {
                int pixel = Nh_CSS_getLengthInPixel(Node_p, Content_p, Property_p->values_pp[0]);
                if (pixel != -1) {Node_p->Properties.Border.Radii.Values_p[0].topRight = pixel;}
                break;
            }
        }
    }

NH_SILENT_END()
}

static inline void Nh_CSS_setBorderBottomLeftRadius(
    Nh_HTML_Node *Node_p, Nh_Content *Content_p, Nh_CSS_GenericProperty *Property_p)
{
NH_BEGIN()

    Node_p->Properties.Border.Radii.Values_p[0].bottomLeft = 0.0f;
    Node_p->Properties.Border.Radii.Values_p[1].bottomLeft = 0.0f;

    if (Property_p != NULL) 
    {
        switch (Property_p->valueCount)
        {
            case 1:
            {
                int pixel = Nh_CSS_getLengthInPixel(Node_p, Content_p, Property_p->values_pp[0]);
                if (pixel != -1) {Node_p->Properties.Border.Radii.Values_p[0].bottomLeft = pixel;}
                break;
            }
        }
    }

NH_SILENT_END()
}

static inline void Nh_CSS_setBorderBottomRightRadius(
    Nh_HTML_Node *Node_p, Nh_Content *Content_p, Nh_CSS_GenericProperty *Property_p)
{
NH_BEGIN()

    Node_p->Properties.Border.Radii.Values_p[0].bottomRight = 0.0f;
    Node_p->Properties.Border.Radii.Values_p[1].bottomRight = 0.0f;

    if (Property_p != NULL) 
    {
        switch (Property_p->valueCount)
        {
            case 1:
            {
                int pixel = Nh_CSS_getLengthInPixel(Node_p, Content_p, Property_p->values_pp[0]);
                if (pixel != -1) {Node_p->Properties.Border.Radii.Values_p[0].bottomRight = pixel;}
                break;
            }
        }
    }

NH_SILENT_END()
}

static inline void Nh_CSS_setBorderRadius(
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
                SingleValue.valueCount = 1;
                Nh_CSS_setBorderTopLeftRadius(Node_p, Content_p, &SingleValue);
                Nh_CSS_setBorderTopRightRadius(Node_p, Content_p, &SingleValue);
                Nh_CSS_setBorderBottomLeftRadius(Node_p, Content_p, &SingleValue);
                Nh_CSS_setBorderBottomRightRadius(Node_p, Content_p, &SingleValue);
                break;
        }

        Nh_free(SingleValue.values_pp);
    }

NH_SILENT_END()
}

// COMPUTE =========================================================================================

NH_RESULT Nh_CSS_computeBorderProperties(
    Nh_Content *Content_p, Nh_HTML_Node *Node_p, Nh_CSS_GenericProperty **Properties_pp)
{
NH_BEGIN()

    if (!Nh_CSS_setBorder(Node_p, Content_p, Properties_pp[NH_CSS_PROPERTY_BORDER])) 
    {
        Nh_CSS_setBorderBottom(Node_p, Content_p, Properties_pp[NH_CSS_PROPERTY_BORDER_BOTTOM]);
        Nh_CSS_setBorderLeft(Node_p, Content_p, Properties_pp[NH_CSS_PROPERTY_BORDER_LEFT]);
        Nh_CSS_setBorderRight(Node_p, Content_p, Properties_pp[NH_CSS_PROPERTY_BORDER_RIGHT]);
        Nh_CSS_setBorderTop(Node_p, Content_p, Properties_pp[NH_CSS_PROPERTY_BORDER_TOP]);
    }

    if (!Nh_CSS_setBorderColor(Node_p, Properties_pp[NH_CSS_PROPERTY_BORDER_COLOR]))
    {
        Nh_CSS_setBorderTopColor(Node_p, Properties_pp[NH_CSS_PROPERTY_BORDER_TOP_COLOR]);
        Nh_CSS_setBorderRightColor(Node_p, Properties_pp[NH_CSS_PROPERTY_BORDER_RIGHT_COLOR]);
        Nh_CSS_setBorderBottomColor(Node_p, Properties_pp[NH_CSS_PROPERTY_BORDER_BOTTOM_COLOR]);
        Nh_CSS_setBorderLeftColor(Node_p, Properties_pp[NH_CSS_PROPERTY_BORDER_LEFT_COLOR]);
    }

    if (!Nh_CSS_setBorderStyle(Node_p, Properties_pp[NH_CSS_PROPERTY_BORDER_STYLE]))
    {
        Nh_CSS_setBorderTopStyle(Node_p, Properties_pp[NH_CSS_PROPERTY_BORDER_TOP_STYLE]);
        Nh_CSS_setBorderRightStyle(Node_p, Properties_pp[NH_CSS_PROPERTY_BORDER_RIGHT_STYLE]);
        Nh_CSS_setBorderBottomStyle(Node_p, Properties_pp[NH_CSS_PROPERTY_BORDER_BOTTOM_STYLE]);
        Nh_CSS_setBorderLeftStyle(Node_p, Properties_pp[NH_CSS_PROPERTY_BORDER_LEFT_STYLE]);
    }

    Nh_CSS_setBorderWidth(Node_p, Content_p, Properties_pp[NH_CSS_PROPERTY_BORDER_WIDTH]);
    Nh_CSS_setBorderTopWidth(Node_p, Content_p, Properties_pp[NH_CSS_PROPERTY_BORDER_TOP_WIDTH]);
    Nh_CSS_setBorderRightWidth(Node_p, Content_p, Properties_pp[NH_CSS_PROPERTY_BORDER_RIGHT_WIDTH]);
    Nh_CSS_setBorderBottomWidth(Node_p, Content_p, Properties_pp[NH_CSS_PROPERTY_BORDER_BOTTOM_WIDTH]);
    Nh_CSS_setBorderLeftWidth(Node_p, Content_p, Properties_pp[NH_CSS_PROPERTY_BORDER_LEFT_WIDTH]);

    Nh_CSS_setBorderRadius(Node_p, Content_p, Properties_pp[NH_CSS_PROPERTY_BORDER_RADIUS]);
    Nh_CSS_setBorderTopLeftRadius(Node_p, Content_p, Properties_pp[NH_CSS_PROPERTY_BORDER_TOP_LEFT_RADIUS]);
    Nh_CSS_setBorderTopRightRadius(Node_p, Content_p, Properties_pp[NH_CSS_PROPERTY_BORDER_TOP_RIGHT_RADIUS]);
    Nh_CSS_setBorderBottomLeftRadius(Node_p, Content_p, Properties_pp[NH_CSS_PROPERTY_BORDER_BOTTOM_LEFT_RADIUS]);
    Nh_CSS_setBorderBottomRightRadius(Node_p, Content_p, Properties_pp[NH_CSS_PROPERTY_BORDER_BOTTOM_RIGHT_RADIUS]);

NH_DIAGNOSTIC_END(NH_SUCCESS)
}


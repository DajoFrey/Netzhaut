// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "../Header/Position.h"
#include "../Header/Helper.h"
#include "../Header/Macros.h"

#include "../../Netzhaut/Tab.h"
#include "../../Netzhaut/Memory.h"

#include "../../HTML/Main/Header/Document.h"

#include NH_FLOW
#include NH_CSS_UTILS

#include <string.h>
#include <ctype.h>

// SET =============================================================================================

static inline void Nh_CSS_setPosition(
    Nh_HTML_Node *Node_p, Nh_CSS_GenericProperty *Property_p)
{
NH_BEGIN()

    Node_p->Properties.Position.position = NH_CSS_POSITION_STATIC;

    if (Property_p != NULL)
    {
        char *positionValue = Property_p->values_pp[0];

             if (!strcmp(positionValue, "fixed"))    {Node_p->Properties.Position.position = NH_CSS_POSITION_FIXED;}
        else if (!strcmp(positionValue, "relative")) {Node_p->Properties.Position.position = NH_CSS_POSITION_RELATIVE;}
        else if (!strcmp(positionValue, "absolute")) {Node_p->Properties.Position.position = NH_CSS_POSITION_ABSOLUTE;}
        else if (!strcmp(positionValue, "sticky"))   {Node_p->Properties.Position.position = NH_CSS_POSITION_STICKY;}
        else                                         {Node_p->Properties.Position.position = NH_CSS_POSITION_STATIC;}
    } 

NH_SILENT_END()
}

static inline void Nh_CSS_setDisplay(
    Nh_HTML_Node *Node_p, Nh_CSS_GenericProperty *Property_p)
{
NH_BEGIN()

    Node_p->Properties.Position.display = NH_CSS_DISPLAY_BLOCK;
    Nh_HTML_Node *Parent_p = Node_p->Parent_p;
    if (Parent_p != NULL) {
        if (Parent_p->Properties.Position.display == NH_CSS_DISPLAY_FLEX) {
            Node_p->Properties.Position.display = NH_CSS_DISPLAY_FLEX;
        }
        if (Parent_p->Properties.Position.display == NH_CSS_DISPLAY_NONE) {
            Node_p->Properties.Position.display = NH_CSS_DISPLAY_NONE;
        }
    }

    if (Property_p != NULL) 
    {
        char *displayValue = Property_p->values_pp[0];

        if (Node_p->Parent_p != NULL 
        &&  Node_p->Parent_p->Properties.Position.display == NH_CSS_DISPLAY_NONE) {NH_SILENT_END()}

             if (!strcmp(displayValue, "inline"))             {Node_p->Properties.Position.display = NH_CSS_DISPLAY_INLINE;}
        else if (!strcmp(displayValue, "block"))              {Node_p->Properties.Position.display = NH_CSS_DISPLAY_BLOCK;}
        else if (!strcmp(displayValue, "contents"))           {Node_p->Properties.Position.display = NH_CSS_DISPLAY_CONTENTS;}
        else if (!strcmp(displayValue, "flex"))               {Node_p->Properties.Position.display = NH_CSS_DISPLAY_FLEX;}
        else if (!strcmp(displayValue, "grid"))               {Node_p->Properties.Position.display = NH_CSS_DISPLAY_GRID;}
        else if (!strcmp(displayValue, "inline-block"))       {Node_p->Properties.Position.display = NH_CSS_DISPLAY_INLINE_BLOCK;}
        else if (!strcmp(displayValue, "inline-flex"))        {Node_p->Properties.Position.display = NH_CSS_DISPLAY_INLINE_FLEX;}
        else if (!strcmp(displayValue, "inline-grid"))        {Node_p->Properties.Position.display = NH_CSS_DISPLAY_INLINE_GRID;}
        else if (!strcmp(displayValue, "inline-table"))       {Node_p->Properties.Position.display = NH_CSS_DISPLAY_INLINE_TABLE;}
        else if (!strcmp(displayValue, "list-item"))          {Node_p->Properties.Position.display = NH_CSS_DISPLAY_LIST_ITEM;}
        else if (!strcmp(displayValue, "run-in"))             {Node_p->Properties.Position.display = NH_CSS_DISPLAY_RUN_IN;}
        else if (!strcmp(displayValue, "table"))              {Node_p->Properties.Position.display = NH_CSS_DISPLAY_TABLE;}
        else if (!strcmp(displayValue, "table-caption"))      {Node_p->Properties.Position.display = NH_CSS_DISPLAY_TABLE_CAPTION;}
        else if (!strcmp(displayValue, "table-column-group")) {Node_p->Properties.Position.display = NH_CSS_DISPLAY_TABLE_COLUMN_GROUP;}
        else if (!strcmp(displayValue, "table-header-group")) {Node_p->Properties.Position.display = NH_CSS_DISPLAY_TABLE_HEADER_GROUP;}
        else if (!strcmp(displayValue, "table-footer-group")) {Node_p->Properties.Position.display = NH_CSS_DISPLAY_TABLE_FOOTER_GROUP;}
        else if (!strcmp(displayValue, "table-row-group"))    {Node_p->Properties.Position.display = NH_CSS_DISPLAY_TABLE_ROW_GROUP;}
        else if (!strcmp(displayValue, "table-cell"))         {Node_p->Properties.Position.display = NH_CSS_DISPLAY_TABLE_CELL;}
        else if (!strcmp(displayValue, "table-column"))       {Node_p->Properties.Position.display = NH_CSS_DISPLAY_TABLE_COLUMN;}
        else if (!strcmp(displayValue, "table-row"))          {Node_p->Properties.Position.display = NH_CSS_DISPLAY_TABLE_ROW;}
        else if (!strcmp(displayValue, "none"))               {Node_p->Properties.Position.display = NH_CSS_DISPLAY_NONE;}
    }

NH_SILENT_END()
}

static inline void Nh_CSS_setLeft(
    Nh_Content *Content_p, Nh_HTML_Node *Node_p, Nh_CSS_GenericProperty *Property_p)
{
NH_BEGIN()
    
    Node_p->Properties.Position.Left.type = NH_CSS_SIZE_AUTO;

    if (Property_p != NULL)
    {
        NH_PIXEL pixel = Nh_CSS_getLengthInPixel(Node_p, Content_p, Property_p->values_pp[0]);
        if (pixel != -1) {
            Node_p->Properties.Position.Left.type  = NH_CSS_SIZE__LENGTH;
            Node_p->Properties.Position.Left.value = pixel;
        }
    }

NH_SILENT_END()
}

static inline void Nh_CSS_setRight(
    Nh_Content *Content_p, Nh_HTML_Node *Node_p, Nh_CSS_GenericProperty *Property_p)
{
NH_BEGIN()

    Node_p->Properties.Position.Right.type = NH_CSS_SIZE_AUTO;
 
    if (Property_p != NULL)
    {
        NH_PIXEL pixel = Nh_CSS_getLengthInPixel(Node_p, Content_p, Property_p->values_pp[0]);
        if (pixel != -1) {
            Node_p->Properties.Position.Right.type  = NH_CSS_SIZE__LENGTH;
            Node_p->Properties.Position.Right.value = pixel; 
        }
    }
       
NH_SILENT_END()
}

static inline void Nh_CSS_setTop(
    Nh_Content *Content_p, Nh_HTML_Node *Node_p, Nh_CSS_GenericProperty *Property_p)
{
NH_BEGIN()

    Node_p->Properties.Position.Top.type = NH_CSS_SIZE_AUTO;

    if (Property_p != NULL)
    {
        NH_PIXEL pixel = Nh_CSS_getLengthInPixel(Node_p, Content_p, Property_p->values_pp[0]);
        if (pixel != -1) {
            Node_p->Properties.Position.Top.type  = NH_CSS_SIZE__LENGTH;
            Node_p->Properties.Position.Top.value = pixel; 
        }
    }

NH_SILENT_END()
}

static inline void Nh_CSS_setBottom(
    Nh_Content *Content_p, Nh_HTML_Node *Node_p, Nh_CSS_GenericProperty *Property_p)
{
NH_BEGIN()

    Node_p->Properties.Position.Bottom.type = NH_CSS_SIZE_AUTO;

    if (Property_p != NULL)
    {
        NH_PIXEL pixel = Nh_CSS_getLengthInPixel(Node_p, Content_p, Property_p->values_pp[0]);
        if (pixel != -1) {
            Node_p->Properties.Position.Bottom.type  = NH_CSS_SIZE__LENGTH;
            Node_p->Properties.Position.Bottom.value = pixel; 
        }
    }

NH_SILENT_END()
}

static inline void Nh_CSS_setWidth(
    Nh_Content *Content_p, Nh_HTML_Node *Node_p, Nh_CSS_GenericProperty *Property_p)
{
NH_BEGIN()

    Node_p->Properties.Position.Width.type = NH_CSS_SIZE_AUTO;
    if (Node_p->tag == NH_HTML_TAG_CANVAS) {
        Node_p->Properties.Position.Width.type = NH_CSS_SIZE__LENGTH;
        Node_p->Properties.Position.Width.value = 300; 
    }

    if (Property_p != NULL) 
    {
             if (strstr(Property_p->values_pp[0], "max-content")) {Node_p->Properties.Position.Width.type = NH_CSS_SIZE_MAX_CONTENT;}
        else if (strstr(Property_p->values_pp[0], "min-content")) {Node_p->Properties.Position.Width.type = NH_CSS_SIZE_MIN_CONTENT;}
        else 
        {
            NH_PIXEL pixel = Nh_CSS_getLengthInPixel(Node_p, Content_p, Property_p->values_pp[0]);
            if (pixel != -1) {
                Node_p->Properties.Position.Width.type = NH_CSS_SIZE__LENGTH;
                Node_p->Properties.Position.Width.value = pixel; 
            }
        }
    }

NH_SILENT_END()
}

static inline void Nh_CSS_setHeight(
    Nh_Content *Content_p, Nh_HTML_Node *Node_p, Nh_CSS_GenericProperty *Property_p)
{
NH_BEGIN()

    Node_p->Properties.Position.Height.type = NH_CSS_SIZE_AUTO;
    if (Node_p->tag == NH_HTML_TAG_CANVAS) {
        Node_p->Properties.Position.Height.type = NH_CSS_SIZE__LENGTH;
        Node_p->Properties.Position.Height.value = 150; 
    }

    if (Property_p != NULL) 
    {
             if (strstr(Property_p->values_pp[0], "max-content")) {Node_p->Properties.Position.Height.type = NH_CSS_SIZE_MAX_CONTENT;}
        else if (strstr(Property_p->values_pp[0], "min-content")) {Node_p->Properties.Position.Height.type = NH_CSS_SIZE_MIN_CONTENT;}
        else 
        {
            NH_PIXEL pixel = Nh_CSS_getLengthInPixel(Node_p, Content_p, Property_p->values_pp[0]);
            if (pixel != -1) {
                Node_p->Properties.Position.Height.type = NH_CSS_SIZE__LENGTH;
                Node_p->Properties.Position.Height.value = pixel; 
            }
        }
    }

NH_SILENT_END()
}

static inline void Nh_CSS_setMinWidth(
    Nh_Content *Content_p, Nh_HTML_Node *Node_p, Nh_CSS_GenericProperty *Property_p)
{
NH_BEGIN()

    Node_p->Properties.Position.MinWidth.type = NH_CSS_SIZE_AUTO;

    if (Property_p != NULL) 
    {
             if (strstr(Property_p->values_pp[0], "max-content")) {Node_p->Properties.Position.MinWidth.type = NH_CSS_SIZE_MAX_CONTENT;}
        else if (strstr(Property_p->values_pp[0], "min-content")) {Node_p->Properties.Position.MinWidth.type = NH_CSS_SIZE_MIN_CONTENT;}
        else 
        {
            NH_PIXEL pixel = Nh_CSS_getLengthInPixel(Node_p, Content_p, Property_p->values_pp[0]);
            if (pixel != -1) {
                Node_p->Properties.Position.MinWidth.type = NH_CSS_SIZE__LENGTH;
                Node_p->Properties.Position.MinWidth.value = pixel; 
            }
        }
    }

NH_SILENT_END()
}

static inline void Nh_CSS_setMinHeight(
    Nh_Content *Content_p, Nh_HTML_Node *Node_p, Nh_CSS_GenericProperty *Property_p)
{
NH_BEGIN()

    Node_p->Properties.Position.MinHeight.type = NH_CSS_SIZE_AUTO;

    if (Property_p != NULL) 
    {
             if (strstr(Property_p->values_pp[0], "max-content")) {Node_p->Properties.Position.MinHeight.type = NH_CSS_SIZE_MAX_CONTENT;}
        else if (strstr(Property_p->values_pp[0], "min-content")) {Node_p->Properties.Position.MinHeight.type = NH_CSS_SIZE_MIN_CONTENT;}
        else 
        {
            NH_PIXEL pixel = Nh_CSS_getLengthInPixel(Node_p, Content_p, Property_p->values_pp[0]);
            if (pixel != -1) {
                Node_p->Properties.Position.MinHeight.type = NH_CSS_SIZE__LENGTH;
                Node_p->Properties.Position.MinHeight.value = pixel; 
            }
        }
    }

NH_SILENT_END()
}

static inline void Nh_CSS_setAlignContent(
    Nh_HTML_Node *Node_p, Nh_CSS_GenericProperty *Property_p)
{
NH_BEGIN()

    Node_p->Properties.Position.alignContent = NH_CSS_ALIGN_CONTENT_NORMAL;

    if (Property_p != NULL) 
    {
             if (strstr(Property_p->values_pp[0], "center"))         {Node_p->Properties.Position.alignContent = NH_CSS_ALIGN_CONTENT_CENTER;}
        else if (strstr(Property_p->values_pp[0], "start"))          {Node_p->Properties.Position.alignContent = NH_CSS_ALIGN_CONTENT_START;}
        else if (strstr(Property_p->values_pp[0], "end"))            {Node_p->Properties.Position.alignContent = NH_CSS_ALIGN_CONTENT_END;}
        else if (strstr(Property_p->values_pp[0], "flex-start"))     {Node_p->Properties.Position.alignContent = NH_CSS_ALIGN_CONTENT_FLEX_START;}
        else if (strstr(Property_p->values_pp[0], "flex-end"))       {Node_p->Properties.Position.alignContent = NH_CSS_ALIGN_CONTENT_FLEX_END;}
        else if (strstr(Property_p->values_pp[0], "normal"))         {Node_p->Properties.Position.alignContent = NH_CSS_ALIGN_CONTENT_NORMAL;}
        else if (strstr(Property_p->values_pp[0], "baseline"))       {Node_p->Properties.Position.alignContent = NH_CSS_ALIGN_CONTENT_BASELINE;}
        else if (strstr(Property_p->values_pp[0], "space-between"))  {Node_p->Properties.Position.alignContent = NH_CSS_ALIGN_CONTENT_SPACE_BETWEEN;}
        else if (strstr(Property_p->values_pp[0], "space-around"))   {Node_p->Properties.Position.alignContent = NH_CSS_ALIGN_CONTENT_SPACE_AROUND;}
        else if (strstr(Property_p->values_pp[0], "space-evenly"))   {Node_p->Properties.Position.alignContent = NH_CSS_ALIGN_CONTENT_SPACE_EVENLY;}
        else if (strstr(Property_p->values_pp[0], "stretch"))        {Node_p->Properties.Position.alignContent = NH_CSS_ALIGN_CONTENT_STRETCH;}
        else if (strstr(Property_p->values_pp[0], "first baseline")) {Node_p->Properties.Position.alignContent = NH_CSS_ALIGN_CONTENT_FIRST_BASELINE;}
        else if (strstr(Property_p->values_pp[0], "last baseline"))  {Node_p->Properties.Position.alignContent = NH_CSS_ALIGN_CONTENT_LAST_BASELINE;}
    }

NH_SILENT_END()
}

// COMPUTE =========================================================================================

NH_RESULT Nh_CSS_computePositionProperties(
    Nh_Content *Content_p, Nh_HTML_Node *Node_p, Nh_CSS_GenericProperty **Properties_pp)
{
NH_BEGIN()

    Nh_CSS_setPosition(Node_p, Properties_pp[NH_CSS_PROPERTY_POSITION]);
    Nh_CSS_setDisplay(Node_p, Properties_pp[NH_CSS_PROPERTY_DISPLAY]);

    Nh_CSS_setLeft(Content_p, Node_p, Properties_pp[NH_CSS_PROPERTY_LEFT]);
    Nh_CSS_setRight(Content_p, Node_p, Properties_pp[NH_CSS_PROPERTY_RIGHT]);
    Nh_CSS_setTop(Content_p, Node_p, Properties_pp[NH_CSS_PROPERTY_TOP]);
    Nh_CSS_setBottom(Content_p, Node_p, Properties_pp[NH_CSS_PROPERTY_BOTTOM]);

    Nh_CSS_setHeight(Content_p, Node_p, Properties_pp[NH_CSS_PROPERTY_HEIGHT]);
    Nh_CSS_setWidth(Content_p, Node_p, Properties_pp[NH_CSS_PROPERTY_WIDTH]);
    Nh_CSS_setMinHeight(Content_p, Node_p, Properties_pp[NH_CSS_PROPERTY_MIN_HEIGHT]);
    Nh_CSS_setMinWidth(Content_p, Node_p, Properties_pp[NH_CSS_PROPERTY_MIN_WIDTH]);

    Nh_CSS_setAlignContent(Node_p, Properties_pp[NH_CSS_PROPERTY_ALIGN_CONTENT]);

NH_DIAGNOSTIC_END(NH_SUCCESS)
}


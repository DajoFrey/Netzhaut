// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "../Header/Text.h"
#include "../Header/Helper.h"
#include "../Header/Macros.h"
#include "../Header/Color.h"

#include "../../Netzhaut/Tab.h"
#include "../../Netzhaut/Memory.h"

#include NH_FLOW

#include <string.h>
#include <ctype.h>

// SET =============================================================================================

inline static void Nh_CSS_setDirection(
    Nh_HTML_Node *Node_p, Nh_CSS_GenericProperty *Property_p)
{
NH_BEGIN()

    Node_p->Properties.Text.direction = NH_CSS_DIRECTION_LTR;

    if (Property_p != NULL) 
    {
        if (!strcmp(Property_p->values_pp[0], "rtl")) {
            Node_p->Properties.Text.direction = NH_CSS_DIRECTION_RTL;
        }
    }

NH_SILENT_END()
}

inline static void Nh_CSS_setTextAlign(
    Nh_HTML_Node *Node_p, Nh_CSS_GenericProperty *Property_p)
{
NH_BEGIN()

    Node_p->Properties.Text.align = 
        Node_p->Properties.Text.direction == NH_CSS_DIRECTION_LTR ? NH_CSS_TEXT_ALIGN_LEFT : NH_CSS_TEXT_ALIGN_RIGHT;

    if (Property_p != NULL) 
    {
             if (!strcmp(Property_p->values_pp[0], "left"))         {Node_p->Properties.Text.writingMode = NH_CSS_TEXT_ALIGN_LEFT;} 
        else if (!strcmp(Property_p->values_pp[0], "right"))        {Node_p->Properties.Text.writingMode = NH_CSS_TEXT_ALIGN_RIGHT;} 
        else if (!strcmp(Property_p->values_pp[0], "center"))       {Node_p->Properties.Text.writingMode = NH_CSS_TEXT_ALIGN_CENTER;} 
        else if (!strcmp(Property_p->values_pp[0], "justify"))      {Node_p->Properties.Text.writingMode = NH_CSS_TEXT_ALIGN_JUSTIFY;} 
        else if (!strcmp(Property_p->values_pp[0], "justify-all"))  {Node_p->Properties.Text.writingMode = NH_CSS_TEXT_ALIGN_JUSTIFY_ALL;} 
        else if (!strcmp(Property_p->values_pp[0], "start"))        {Node_p->Properties.Text.writingMode = NH_CSS_TEXT_ALIGN_START;} 
        else if (!strcmp(Property_p->values_pp[0], "end"))          {Node_p->Properties.Text.writingMode = NH_CSS_TEXT_ALIGN_END;} 
        else if (!strcmp(Property_p->values_pp[0], "match-parent")) {Node_p->Properties.Text.writingMode = NH_CSS_TEXT_ALIGN_MATCH_PARENT;}
    }

NH_SILENT_END()
}

void Nh_CSS_setColor(
    Nh_HTML_Node *Node_p, Nh_CSS_GenericProperty *Property_p)
{
NH_BEGIN()

    for (int i = 0; i < 3; ++i) {Node_p->Properties.Text.color_p[i] = 0.0f;}
    Node_p->Properties.Text.color_p[3] = 1.0;

    if (Property_p != NULL) 
    {
        char color_p[64];
        strcpy(color_p, Property_p->values_pp[0]);
        color_p[strlen(Property_p->values_pp[0])] = '\0';
        Nh_CSS_getColor(color_p, Node_p->Properties.Text.color_p);
    }

NH_SILENT_END()
}

void Nh_CSS_setFontSize(
    Nh_Content *Content_p, Nh_HTML_Node *Node_p, Nh_CSS_GenericProperty *Property_p)
{
NH_BEGIN()

    Node_p->Properties.Text.fontSize = 16;

    if (Property_p != NULL) 
    {
        Nh_HTML_Node *Parent_p = Node_p->Parent_p;

             if (!strcmp(Property_p->values_pp[0], "xx-small"))  {Node_p->Properties.Text.fontSize -= 9;} 
        else if (!strcmp(Property_p->values_pp[0], "x-small"))   {Node_p->Properties.Text.fontSize -= 6;} 
        else if (!strcmp(Property_p->values_pp[0], "small"))     {Node_p->Properties.Text.fontSize -= 3;} 
        else if (!strcmp(Property_p->values_pp[0], "medium"))    {Node_p->Properties.Text.fontSize -= 0;} 
        else if (!strcmp(Property_p->values_pp[0], "large"))     {Node_p->Properties.Text.fontSize += 25;} 
        else if (!strcmp(Property_p->values_pp[0], "x-large"))   {Node_p->Properties.Text.fontSize += 30;} 
        else if (!strcmp(Property_p->values_pp[0], "xx-large"))  {Node_p->Properties.Text.fontSize += 45;} 
        else if (!strcmp(Property_p->values_pp[0], "xxx-large")) {Node_p->Properties.Text.fontSize += 50;} 
        else if (strstr(Property_p->values_pp[0], "em")) 
        {
            float em = strtof(Property_p->values_pp[0], NULL);
            Node_p->Properties.Text.fontSize = 
                (NH_PIXEL)(((float)Node_p->Properties.Text.fontSize) * em);
        }
        else if (strstr(Property_p->values_pp[0], "inherit")) 
        {
            Node_p->Properties.Text.fontSize = Node_p->Parent_p->Properties.Text.fontSize;
        }
    }

NH_SILENT_END()
}

inline static void Nh_CSS_setWritingMode(
    Nh_HTML_Node *Node_p, Nh_CSS_GenericProperty *Property_p)
{
NH_BEGIN()

    Node_p->Properties.Text.writingMode = NH_CSS_WRITING_MODE_HORIZONTAL_TB;

    if (Property_p != NULL) 
    {
             if (!strcmp(Property_p->values_pp[0], "vertical-rl")) {Node_p->Properties.Text.writingMode = NH_CSS_WRITING_MODE_VERTICAL_RL;} 
        else if (!strcmp(Property_p->values_pp[0], "vertical-lr")) {Node_p->Properties.Text.writingMode = NH_CSS_WRITING_MODE_VERTICAL_LR;} 
        else if (!strcmp(Property_p->values_pp[0], "sideways-rl")) {Node_p->Properties.Text.writingMode = NH_CSS_WRITING_MODE_SIDEWAYS_RL;} 
        else if (!strcmp(Property_p->values_pp[0], "sideways-lr")) {Node_p->Properties.Text.writingMode = NH_CSS_WRITING_MODE_SIDEWAYS_LR;}
    }

NH_SILENT_END()
}

void Nh_CSS_setFontFamily(
    Nh_HTML_Node *Node_p, Nh_CSS_GenericProperty *Property_p)
{
NH_BEGIN()

    Node_p->Properties.Text.Font_p = Nh_getDefaultFont();

    if (Property_p != NULL) 
    {
        Nh_Font *Font_p = NULL;

        for (int i = 0; i < Property_p->valueCount; ++i)
        {
            if (!strcmp(Property_p->values_pp[i], "serif")) {
                break;
            } else if (!strcmp(Property_p->values_pp[i], "sans-serif")) {
                break;
            } else if (!strcmp(Property_p->values_pp[i], "monospace")) {
                break;
            } else if (!strcmp(Property_p->values_pp[i], "cursive")) {
                break;
            } else if (!strcmp(Property_p->values_pp[i], "fantasy")) {
                break;
            } else if (!strcmp(Property_p->values_pp[i], "system-ui")) {
                break;
            } else if (!strcmp(Property_p->values_pp[i], "math")) {
                break;
            } else if (!strcmp(Property_p->values_pp[i], "emoji")) {
                break;
            } else if (!strcmp(Property_p->values_pp[i], "fangsong")) {
                break;
            } else if (!strcmp(Property_p->values_pp[i], "inherit")) {
                Font_p = Node_p->Parent_p != NULL ? Node_p->Parent_p->Properties.Text.Font_p : NULL;
                if (Font_p != NULL) {break;}
            } else {
                Font_p = Nh_getFontFromFamilyName(Property_p->values_pp[i]);
                if (Font_p != NULL) {break;}
            }
        }
   
        if (Font_p != NULL) {Node_p->Properties.Text.Font_p = Font_p;}
    }

NH_SILENT_END()
}

void Nh_CSS_setFontWeight(
    Nh_HTML_Node *Node_p, Nh_CSS_GenericProperty *Property_p)
{
NH_BEGIN()

    if (Property_p != NULL) 
    {
        if (!strcmp(Property_p->values_pp[0], "bold"))
        {
            Nh_Font *Font_p = Node_p->Properties.Text.Font_p;
            Nh_Font Prototype = {
                .familyName_p = Font_p->familyName_p,
                .styleName_p  = "Bold",
            };
            Font_p = Nh_getFontFromPrototype(&Prototype);
            if (Font_p != NULL) {Node_p->Properties.Text.Font_p = Font_p;}
        }
    }

NH_SILENT_END()
}

// COMPUTE =========================================================================================

NH_RESULT Nh_CSS_computeTextProperties(
    Nh_Content *Content_p, Nh_HTML_Node *Node_p, Nh_CSS_GenericProperty **Properties_pp)
{
NH_BEGIN()

    Nh_CSS_setFontSize(Content_p, Node_p, Properties_pp[NH_CSS_PROPERTY_FONT_SIZE]);
    Nh_CSS_setDirection(Node_p, Properties_pp[NH_CSS_PROPERTY_LIST_STYLE_TYPE]);
    Nh_CSS_setTextAlign(Node_p, Properties_pp[NH_CSS_PROPERTY_DISPLAY]);
    Nh_CSS_setColor(Node_p, Properties_pp[NH_CSS_PROPERTY_COLOR]);
    Nh_CSS_setWritingMode(Node_p, Properties_pp[NH_CSS_PROPERTY_WRITING_MODE]);

    Nh_CSS_setFontFamily(Node_p, Properties_pp[NH_CSS_PROPERTY_FONT_FAMILY]);
    Nh_CSS_setFontWeight(Node_p, Properties_pp[NH_CSS_PROPERTY_FONT_WEIGHT]);

NH_DIAGNOSTIC_END(NH_SUCCESS)
}


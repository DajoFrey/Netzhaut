// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE ========================================================================================

#include "../Header/Attribute.h"
#include "../Header/Document.h"
#include "../Header/Macros.h"

#include "../../../Netzhaut/Memory.h"
#include "../../../Netzhaut/Tab.h"
#include "../../../Netzhaut/HashMap.h"

#include "../../../CSS/Header/Properties.h"
#include "../../../CSS/Header/Associate.h"

#include NH_FLOW
#include NH_DEFAULT_CHECK

#include <string.h>

// DATA ============================================================================================

const char *NH_HTML_ATTRIBUTES_PP[] = 
{
    "accept",                 
    "accept-charset",          
    "accesskey",              
    "action",                 
    "align",                     
    "alt",                    
    "async",                  
    "autocomplete",           
    "autofocus",              
    "autoplay",               
    "bgcolor",                
    "border",                 
    "charset", 	            
    "checked", 	            
    "cite", 	                
    "class", 	                
    "color", 	                
    "cols", 	                
    "colspan", 	            
    "content", 	              
    "contenteditable",        
    "controls", 	            
    "coords", 	            
    "data", 	                
    "data-*", 	            
    "datetime",               
    "default",                
    "defer", 	                
    "dir", 	                
    "dirname",                
    "disabled",               
    "download",               
    "draggable",              
    "dropzone",               
    "enctype",                
    "for", 	                
    "form", 	                
    "formaction",             
    "headers",                
    "height", 	            
    "hidden", 	            
    "high", 	                
    "href", 	                
    "hreflang", 	            
    "http-equiv", 	        
    "id", 	                
    "ismap", 	                
    "kind", 	                
    "label",                  
    "lang", 	                
    "list", 	                
    "loop", 	                
    "low", 	                
    "max", 	                
    "maxlength",              
    "media",                  
    "method", 	            
    "min", 	                
    "multiple", 	            
    "muted", 	                
    "name", 	                
    "novalidate", 	        
    "onabort",                
    "onafterprint", 	          
    "onbeforeprint", 	        
    "onbeforeunload", 	    
    "onblur",                 
    "oncanplay",              
    "oncanplaythrough",       
    "onchange",               
    "onclick",                
    "oncontextmenu",          
    "oncopy",                 
    "oncuechange",            
    "oncut",                  
    "ondblclick",             
    "ondrag",                 
    "ondragend",              
    "ondragenter",            
    "ondragleave",            
    "ondragover",             
    "ondragstart",            
    "ondrop",                 
    "ondurationchange",       
    "onemptied",              
    "onended",                
    "onerror",                
    "onfocus",                
    "onhashchange",           
    "oninput", 	            
    "oninvalid", 	            
    "onkeydown", 	            
    "onkeypress", 	        
    "onkeyup", 	            
    "onload", 	            
    "onloadeddata",           
    "onloadedmetadata",       
    "onloadstart", 	        
    "onmousedown", 	        
    "onmousemove", 	        
    "onmouseout", 	        
    "onmouseover",            
    "onmouseup", 	             
    "onmousewheel",           
    "onoffline", 	            
    "ononline", 	            
    "onpagehide", 	        
    "onpageshow", 	        
    "onpaste", 	            
    "onpause", 	            
    "onplay", 	            
    "onplaying",              
    "onpopstate",             
    "onprogress", 	        
    "onratechange",           
    "onreset", 	            
    "onresize", 	            
    "onscroll", 	            
    "onsearch", 	            
    "onseeked", 	            
    "onseeking", 	            
    "onselect", 	            
    "onstalled",              
    "onstorage", 	            
    "onsubmit", 	            
    "onsuspend", 	            
    "ontimeupdate",           
    "ontoggle", 	            
    "onunload", 	            
    "onvolumechange",         
    "onwaiting", 	            
    "onwheel", 	            
    "open", 	                
    "optimum",                
    "pattern",                
    "placeholder",            
    "poster", 	            
    "preload",                
    "readonly",               
    "rel", 	                
    "required",               
    "reversed",               
    "rows", 	                
    "rowspan",                
    "sandbox",                
    "scope", 	                
    "selected",               
    "shape", 	                
    "size", 	                
    "sizes", 	                
    "span", 	 	            
    "spellcheck",             
    "src", 	                
    "srcdoc", 	            
    "srclang",                
    "srcset", 	            
    "start", 	                
    "step", 	                
    "style", 	                
    "tabindex",               
    "target", 	            
    "title", 	                
    "translate",              
    "type", 	                
    "usemap", 	            
    "value", 	                
    "width", 	                
    "wrap", 	                 
    "link",
    "alink",
    "vlink",
};

size_t NH_HTML_ATTRIBUTES_PP_COUNT = sizeof(NH_HTML_ATTRIBUTES_PP) / sizeof(NH_HTML_ATTRIBUTES_PP[0]);

// GET =============================================================================================

static Nh_HTML_Attribute *Nh_HTML_getAttributeFromType(
    Nh_LinkedList *Attributes_p, NH_HTML_ATTRIBUTE type)
{
NH_BEGIN()

    for (int i = 0; i < Attributes_p->count; ++i) {
        Nh_HTML_Attribute *Attribute_p = Nh_getFromLinkedList(Attributes_p, i);
        if (Attribute_p->type == type) {NH_END(Attribute_p)}
    }

NH_END(NULL)
}

char *Nh_HTML_getHRef(
    Nh_HTML_Node *Node_p)
{
NH_BEGIN()

    Nh_HTML_Attribute *Attribute_p = Nh_HTML_getAttributeFromType(&Node_p->Attributes, NH_HTML_ATTRIBUTE_HREF);
    if (Attribute_p != NULL) {NH_END(Attribute_p->value_p)} 

NH_END(NULL)
}

char *Nh_HTML_getId(
    Nh_HTML_Node *Node_p)
{
NH_BEGIN()

    Nh_HTML_Attribute *Attribute_p = Nh_HTML_getAttributeFromType(&Node_p->Attributes, NH_HTML_ATTRIBUTE_ID);
    if (Attribute_p != NULL) {NH_END(Attribute_p->value_p)} 

NH_END(NULL)
}

char *Nh_HTML_getClass(
    Nh_HTML_Node *Node_p)
{
NH_BEGIN()

    Nh_HTML_Attribute *Attribute_p = Nh_HTML_getAttributeFromType(&Node_p->Attributes, NH_HTML_ATTRIBUTE_CLASS);
    if (Attribute_p != NULL) {NH_END(Attribute_p->value_p)} 

NH_END(NULL)
}

char *Nh_HTML_getOnClick(
    Nh_HTML_Node *Node_p)
{
NH_BEGIN()

    Nh_HTML_Attribute *Attribute_p = Nh_HTML_getAttributeFromType(&Node_p->Attributes, NH_HTML_ATTRIBUTE_ON_CLICK);
    if (Attribute_p != NULL) {NH_END(Attribute_p->value_p)} 

NH_END(NULL)
}

char *Nh_HTML_getSrc(
    Nh_HTML_Node *Node_p)
{
NH_BEGIN()

    Nh_HTML_Attribute *Attribute_p = Nh_HTML_getAttributeFromType(&Node_p->Attributes, NH_HTML_ATTRIBUTE_SRC);
    if (Attribute_p != NULL) {NH_END(Attribute_p->value_p)} 

NH_END(NULL)
}

char *Nh_HTML_getValue(
    Nh_HTML_Node *Node_p)
{
NH_BEGIN()

    Nh_HTML_Attribute *Attribute_p = Nh_HTML_getAttributeFromType(&Node_p->Attributes, NH_HTML_ATTRIBUTE_VALUE);
    if (Attribute_p != NULL) {NH_END(Attribute_p->value_p)} 

NH_END(NULL)
}

NH_HTML_TYPE Nh_HTML_getType(
    Nh_HTML_Node *Node_p)
{
NH_BEGIN()

    Nh_HTML_Attribute *Attribute_p = Nh_HTML_getAttributeFromType(&Node_p->Attributes, NH_HTML_ATTRIBUTE_TYPE);
    NH_HTML_TYPE result = NH_HTML_TYPE_TEXT;

    if (Attribute_p != NULL)
    {
             if (!strcmp(Attribute_p->value_p, "button"))         {result = NH_HTML_TYPE_BUTTON;}
        else if (!strcmp(Attribute_p->value_p, "checkbox"))       {result = NH_HTML_TYPE_CHECKBOX;}
        else if (!strcmp(Attribute_p->value_p, "color"))          {result = NH_HTML_TYPE_COLOR;}
        else if (!strcmp(Attribute_p->value_p, "date"))           {result = NH_HTML_TYPE_DATE;}
        else if (!strcmp(Attribute_p->value_p, "datetime"))       {result = NH_HTML_TYPE_DATETIME;}
        else if (!strcmp(Attribute_p->value_p, "datetime-local")) {result = NH_HTML_TYPE_DATETIME_LOCAL;}
        else if (!strcmp(Attribute_p->value_p, "email"))          {result = NH_HTML_TYPE_EMAIL;}
        else if (!strcmp(Attribute_p->value_p, "file"))           {result = NH_HTML_TYPE_FILE;}
        else if (!strcmp(Attribute_p->value_p, "hidden"))         {result = NH_HTML_TYPE_HIDDEN;}
        else if (!strcmp(Attribute_p->value_p, "image"))          {result = NH_HTML_TYPE_IMAGE;}
        else if (!strcmp(Attribute_p->value_p, "month"))          {result = NH_HTML_TYPE_MONTH;}
        else if (!strcmp(Attribute_p->value_p, "number"))         {result = NH_HTML_TYPE_NUMBER;}
        else if (!strcmp(Attribute_p->value_p, "password"))       {result = NH_HTML_TYPE_PASSWORD;}
        else if (!strcmp(Attribute_p->value_p, "radio"))          {result = NH_HTML_TYPE_RADIO;}
        else if (!strcmp(Attribute_p->value_p, "range"))          {result = NH_HTML_TYPE_RANGE;}
        else if (!strcmp(Attribute_p->value_p, "reset"))          {result = NH_HTML_TYPE_RESET;}
        else if (!strcmp(Attribute_p->value_p, "search"))         {result = NH_HTML_TYPE_SEARCH;}
        else if (!strcmp(Attribute_p->value_p, "submit"))         {result = NH_HTML_TYPE_SUBMIT;}
        else if (!strcmp(Attribute_p->value_p, "tel"))            {result = NH_HTML_TYPE_TEL;}
        else if (!strcmp(Attribute_p->value_p, "text"))           {result = NH_HTML_TYPE_TEXT;}
        else if (!strcmp(Attribute_p->value_p, "time"))           {result = NH_HTML_TYPE_TIME;}
        else if (!strcmp(Attribute_p->value_p, "url"))            {result = NH_HTML_TYPE_URL;}
        else if (!strcmp(Attribute_p->value_p, "week"))           {result = NH_HTML_TYPE_WEEK;}
    }

NH_DIAGNOSTIC_END(result)
}

NH_BOOL Nh_HTML_getSelected(
    Nh_HTML_Node *Node_p)
{
NH_BEGIN()

    Nh_HTML_Attribute *Attribute_p = Nh_HTML_getAttributeFromType(&Node_p->Attributes, NH_HTML_ATTRIBUTE_SELECTED);
    if (Attribute_p != NULL) {NH_END(NH_TRUE)} 

NH_END(NH_FALSE)
}

char *Nh_HTML_getRel(
    Nh_HTML_Node *Node_p)
{
NH_BEGIN()

    Nh_HTML_Attribute *Attribute_p = Nh_HTML_getAttributeFromType(&Node_p->Attributes, NH_HTML_ATTRIBUTE_REL);
    if (Attribute_p != NULL) {NH_END(Attribute_p->value_p)} 

NH_END(NULL)
}

char *Nh_HTML_getOnMouseOver(
    Nh_HTML_Node *Node_p)
{
NH_BEGIN()

    Nh_HTML_Attribute *Attribute_p = Nh_HTML_getAttributeFromType(&Node_p->Attributes, NH_HTML_ATTRIBUTE_ON_MOUSE_OVER);
    if (Attribute_p != NULL) {NH_END(Attribute_p->value_p)} 

NH_END(NULL)
}

char *Nh_HTML_getOnMouseDown(
    Nh_HTML_Node *Node_p)
{
NH_BEGIN()

    Nh_HTML_Attribute *Attribute_p = Nh_HTML_getAttributeFromType(&Node_p->Attributes, NH_HTML_ATTRIBUTE_ON_MOUSE_DOWN);
    if (Attribute_p != NULL) {NH_END(Attribute_p->value_p)} 

NH_END(NULL)
}

char *Nh_HTML_getOnMouseOut(
    Nh_HTML_Node *Node_p)
{
NH_BEGIN()

    Nh_HTML_Attribute *Attribute_p = Nh_HTML_getAttributeFromType(&Node_p->Attributes, NH_HTML_ATTRIBUTE_ON_MOUSE_OUT);
    if (Attribute_p != NULL) {NH_END(Attribute_p->value_p)} 

NH_END(NULL)
}

char *Nh_HTML_getOnMouseMove(
    Nh_HTML_Node *Node_p)
{
NH_BEGIN()

    Nh_HTML_Attribute *Attribute_p = Nh_HTML_getAttributeFromType(&Node_p->Attributes, NH_HTML_ATTRIBUTE_ON_MOUSE_MOVE);
    if (Attribute_p != NULL) {NH_END(Attribute_p->value_p)} 

NH_END(NULL)
}

char *Nh_HTML_getOnChange(
    Nh_HTML_Node *Node_p)
{
NH_BEGIN()

    Nh_HTML_Attribute *Attribute_p = Nh_HTML_getAttributeFromType(&Node_p->Attributes, NH_HTML_ATTRIBUTE_ON_CHANGE);
    if (Attribute_p != NULL) {NH_END(Attribute_p->value_p)} 

NH_END(NULL)
}

// ATTRIBUTE LIST ==================================================================================

NH_RESULT Nh_HTML_addAttribute(
    Nh_LinkedList *Attributes_p, NH_HTML_ATTRIBUTE type, char *value_p)
{
NH_BEGIN()

    NH_CHECK_NULL(Attributes_p)
    Nh_HTML_Attribute *Attribute_p = NULL;

    Attribute_p = Nh_HTML_getAttributeFromType(Attributes_p, type);
    if (Attribute_p != NULL) {
        Attribute_p->value_p = value_p;
        NH_DIAGNOSTIC_END(NH_SUCCESS)
    }

    Attribute_p = Nh_allocate(sizeof(Nh_HTML_Attribute));
    NH_CHECK_MEM(Attribute_p)
    
    Attribute_p->value_p = value_p;
    Attribute_p->type    = type;

    NH_CHECK(Nh_appendToLinkedList(Attributes_p, Attribute_p))

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

void Nh_HTML_removeAttribute(
    Nh_LinkedList *Attributes_p, NH_HTML_ATTRIBUTE type)
{
NH_BEGIN()

    if (Attributes_p == NULL) {NH_SILENT_END()}

    for (int i = 0; i < Attributes_p->count; ++i) 
    {
        Nh_HTML_Attribute *Attribute_p = Nh_getFromLinkedList(Attributes_p, i);

        if (Attribute_p->type == type) 
        {
            Nh_free(Attribute_p->value_p);
            Nh_removeFromLinkedList(Attributes_p, i, true);
            NH_SILENT_END()
        }
    }

NH_SILENT_END()
}

void Nh_HTML_destroyAttributes(
    Nh_LinkedList *Attributes_p)
{
NH_BEGIN()

    for (int i = 0; i < Attributes_p->count; i++)
    {
        Nh_HTML_Attribute *Attribute_p = Nh_getFromLinkedList(Attributes_p, i);
        Nh_free(Attribute_p->value_p);
    }

    Nh_destroyLinkedList(Attributes_p, true);

NH_SILENT_END()
}


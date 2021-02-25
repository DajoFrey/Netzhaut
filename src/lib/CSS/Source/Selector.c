// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE ========================================================================================

#include "../Header/Associate.h"
#include "../Header/Properties.h"
#include "../Header/Macros.h"

#include "../../Netzhaut/LinkedList.h"
#include "../../Netzhaut/HashMap.h"
#include "../../Netzhaut/Memory.h"
#include "../../Netzhaut/String.h"

#include "../../HTML/Main/Header/Attribute.h"
#include "../../HTML/Main/Header/Document.h"

#include NH_FLOW
#include NH_DEFAULT_CHECK

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

// DECLARE ========================================================================================

static void Nh_CSS_getSelectorParts(
    char *selectorString_p, NH_CSS_SELECTOR type, char **parts_pp
);
static NH_BOOL Nh_CSS_isAttributeSelector(
    const char *selector_p
);

/**
 * Checks if the attribute selector matches a specific node attribute. The implementation currently 
 * also checks for matching node properties.
 */
static NH_BOOL Nh_CSS_attributeSelectorHit(
    const char *selector_p, Nh_HTML_Node *Node_p
);

// INIT ============================================================================================

static void Nh_CSS_initSelector(
    Nh_CSS_Selector *Selector_p, char *selectorString_p)
{
NH_BEGIN()

    Selector_p->type = NH_CSS_SELECTOR_UNDEFINED;
    Selector_p->string_p = selectorString_p;
    Selector_p->pseudoClass = -1;
    Selector_p->pseudoElement = -1;
    NH_INIT_LINKED_LIST(Selector_p->Parts)

NH_SILENT_END()
}

// PART ============================================================================================

static NH_RESULT Nh_CSS_addSelectorPart(
    Nh_CSS_Selector *Selector_p, Nh_CSS_Selector *Part_p)
{
NH_BEGIN()

    Nh_CSS_Selector *Alloc_p = Nh_allocate(sizeof(Nh_CSS_Selector));
    NH_CHECK_MEM(Alloc_p)
    memcpy(Alloc_p, Part_p, sizeof(Nh_CSS_Selector));
    Alloc_p->string_p = Nh_allocateBytes(Part_p->string_p);
    NH_CHECK_MEM(Alloc_p->string_p)
    NH_CHECK(Nh_appendToLinkedList(&Selector_p->Parts, Alloc_p))

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

// HIT =============================================================================================

NH_BOOL Nh_CSS_selectorPreHit(
    Nh_HTML_Node *Node_p, char *selectorString_p, Nh_CSS_Selector *Selector_p)
{
NH_BEGIN()

#include NH_CUSTOM_CHECK

    Nh_CSS_initSelector(Selector_p, selectorString_p);

         if (strstr(selectorString_p, ">"))                           {Selector_p->type = NH_CSS_SELECTOR_CHILD_COMBINATOR;}
    else if (strstr(selectorString_p, "::"))                          {Selector_p->type = NH_CSS_SELECTOR_PSEUDO_ELEMENT;}
    else if (strstr(selectorString_p, ":"))                           {Selector_p->type = NH_CSS_SELECTOR_PSEUDO_CLASS;} 
    else if (strstr(selectorString_p, "*"))                           {Selector_p->type = NH_CSS_SELECTOR_UNIVERSAL;}
    else if (selectorString_p[0] == '#')                              {Selector_p->type = NH_CSS_SELECTOR_ID;}
    else if (selectorString_p[0] == '.')                              {Selector_p->type = NH_CSS_SELECTOR_CLASS;}
    else if (!strcmp(selectorString_p, NH_HTML_TAGS_PP[Node_p->tag])) {Selector_p->type = NH_CSS_SELECTOR_TYPE;}
    else if (Nh_CSS_isAttributeSelector(selectorString_p))            {Selector_p->type = NH_CSS_SELECTOR_ATTRIBUTE;}

    switch (Selector_p->type)
    {
        case NH_CSS_SELECTOR_UNIVERSAL :
        case NH_CSS_SELECTOR_TYPE : 
        {
            NH_END(NH_TRUE)
            break;
        }
        case NH_CSS_SELECTOR_ID :
        {    
            if (Nh_HTML_getId(Node_p) != NULL) {
                if (!strcmp(selectorString_p + 1, Nh_HTML_getId(Node_p))) {
                    NH_END(NH_TRUE)
                }
            }
            break;
        }
        case NH_CSS_SELECTOR_CLASS :
        {    
            if (Nh_HTML_getClass(Node_p) != NULL) {
                if (!strcmp(selectorString_p + 1, Nh_HTML_getClass(Node_p))) {
                    NH_END(NH_TRUE)
                }
            }
            break;
        }
        case NH_CSS_SELECTOR_ATTRIBUTE :
        {
            char tag_p[256] = {'\0'};
            int count = 0;
            for (int i = 0; i < strlen(selectorString_p); ++i) {if (selectorString_p[i] == '[') {break;}++count;}
            memcpy((void*)tag_p, (void*)selectorString_p, sizeof(char) * count);
            if (!strcmp(tag_p, NH_HTML_TAGS_PP[Node_p->tag])) {NH_END(NH_TRUE)}
            break;
        }
        case NH_CSS_SELECTOR_CHILD_COMBINATOR :
        {
            if (Node_p->Parent_p != NULL) 
            {
                char *parts_pp[2], part1_p[256] = {'\0'}, part2_p[256] = {'\0'};
                parts_pp[0] = part1_p, parts_pp[1] = part2_p;
                Nh_CSS_getSelectorParts(selectorString_p, NH_CSS_SELECTOR_CHILD_COMBINATOR, (char**)parts_pp);

                Nh_CSS_Selector Left, Right;

                if (Nh_CSS_selectorPreHit(Node_p->Parent_p, parts_pp[0], &Left) 
                &&  Nh_CSS_selectorPreHit(Node_p, parts_pp[1], &Right)) {
                    NH_CHECK(NH_FALSE, Nh_CSS_addSelectorPart(Selector_p, &Left))
                    NH_CHECK(NH_FALSE, Nh_CSS_addSelectorPart(Selector_p, &Right))
                    NH_END(NH_TRUE)
                }
            }
            break;
        }
        case NH_CSS_SELECTOR_PSEUDO_CLASS :
        {
            char pseudoClassString_p[255] = {'\0'};
            strcpy(pseudoClassString_p, strstr(selectorString_p, ":") + 1);

            Nh_HashValue *HashValue_p;
            if (hashmap_get(NH_HASHMAPS.CSS.PseudoClasses, pseudoClassString_p, (void**)(&HashValue_p)) != MAP_OK) {NH_END(NH_FALSE)}
            Selector_p->pseudoClass = HashValue_p->number;

            char leftSelect_p[255] = {'\0'};
            strcpy(leftSelect_p, selectorString_p);
            leftSelect_p[strlen(selectorString_p) - (strlen(pseudoClassString_p) + 1)] = '\0';

            Nh_CSS_Selector Selector;
            if (Nh_CSS_selectorPreHit(Node_p, leftSelect_p, &Selector)) {
                NH_CHECK(NH_FALSE, Nh_CSS_addSelectorPart(Selector_p, &Selector))
                NH_END(NH_TRUE)
            }
        }
        case NH_CSS_SELECTOR_PSEUDO_ELEMENT :
        {
            char pseudoElementString_p[255] = {'\0'};
            strcpy(pseudoElementString_p, strstr(selectorString_p, ":") + 2);

            Nh_HashValue *HashValue_p;
            if (hashmap_get(NH_HASHMAPS.CSS.PseudoElements, pseudoElementString_p, (void**)(&HashValue_p)) != MAP_OK) {NH_END(NH_FALSE)}
            Selector_p->pseudoElement = HashValue_p->number;

            char leftSelect_p[255] = {'\0'};
            strcpy(leftSelect_p, selectorString_p);
            leftSelect_p[strlen(selectorString_p) - strlen(pseudoElementString_p) - 2] = '\0';
            if (strlen(leftSelect_p) == 0) {NH_END(NH_TRUE)}

            Nh_CSS_Selector Selector;
            if (Nh_CSS_selectorPreHit(Node_p, leftSelect_p, &Selector)) {
                NH_CHECK(NH_FALSE, Nh_CSS_addSelectorPart(Selector_p, &Selector))
                NH_END(NH_TRUE)
            }

            break;
        }
    }

#include NH_DEFAULT_CHECK

NH_END(NH_FALSE)
}

NH_BOOL Nh_CSS_selectorHit(
    Nh_HTML_Node *Node_p, Nh_CSS_Selector *Selector_p)
{
NH_BEGIN()

#include NH_CUSTOM_CHECK

    switch (Selector_p->type)
    {
        case NH_CSS_SELECTOR_UNIVERSAL :
        case NH_CSS_SELECTOR_TYPE      : 
        case NH_CSS_SELECTOR_ID        :
        case NH_CSS_SELECTOR_CLASS     :
        {
            NH_END(NH_TRUE)
        }
        case NH_CSS_SELECTOR_ATTRIBUTE :
        {
            if (Nh_CSS_attributeSelectorHit(Selector_p->string_p, Node_p)) {
                NH_END(NH_TRUE)
            }
            break;
        }
        case NH_CSS_SELECTOR_CHILD_COMBINATOR :
        {
            if (Node_p->Parent_p != NULL) 
            {
                if (Nh_CSS_selectorHit(Node_p->Parent_p, Nh_getFromLinkedList(&Selector_p->Parts, 0)) 
                &&  Nh_CSS_selectorHit(Node_p, Nh_getFromLinkedList(&Selector_p->Parts, 1))) {
                    NH_END(NH_TRUE)
                }
            }
            break;
        }
        case NH_CSS_SELECTOR_PSEUDO_CLASS :
        {
            if (Node_p->Pseudo.classes_p[Selector_p->pseudoClass] 
            &&  Nh_CSS_selectorHit(Node_p, Nh_getFromLinkedList(&Selector_p->Parts, 0))) {

                NH_END(NH_TRUE)
            }
            break;
        }
        case NH_CSS_SELECTOR_PSEUDO_ELEMENT :
        {
            if (Node_p->Pseudo.elements_p[Selector_p->pseudoElement]
            &&  Selector_p->Parts.count == 0) {
                NH_END(NH_TRUE)
            }
            if (Node_p->Pseudo.elements_p[Selector_p->pseudoElement] 
            &&  Nh_CSS_selectorHit(Node_p, Nh_getFromLinkedList(&Selector_p->Parts, 0))) {
                NH_END(NH_TRUE)
            }
            break;
        }
    }

#include NH_DEFAULT_CHECK

NH_END(NH_FALSE)
}

// PRECEDENCE ======================================================================================

int Nh_CSS_getPrecedence(
    NH_CSS_SELECTOR selector)
{
NH_BEGIN()

    switch (selector)
    {
        case NH_CSS_SELECTOR_UNIVERSAL                   : NH_END(20)
        case NH_CSS_SELECTOR_TYPE                        : NH_END(5)
        case NH_CSS_SELECTOR_CLASS                       : NH_END(3)
        case NH_CSS_SELECTOR_ID                          : NH_END(2)
        case NH_CSS_SELECTOR_ATTRIBUTE                   : NH_END(0)
        case NH_CSS_SELECTOR_PSEUDO_CLASS                : NH_END(1)
        case NH_CSS_SELECTOR_DESCENDANT_COMBINATOR       : 
        case NH_CSS_SELECTOR_CHILD_COMBINATOR            : 
        case NH_CSS_SELECTOR_GENERAL_SIBLING_COMBINATOR  :
        case NH_CSS_SELECTOR_ADJACENT_SIBLING_COMBINATOR :
        case NH_CSS_SELECTOR_COLUMN_COMBINATOR           : NH_END(4)
    }
    
NH_END(30)
}

// HELPER ==========================================================================================

static NH_BOOL Nh_CSS_isAttributeSelector(
    const char *selector_p)
{
NH_BEGIN()

    int c = 0;
    for (int i = 0; i < strlen(selector_p); ++i) {
        if (selector_p[i] == '[') {c = 1;}
        if (c == 1 && selector_p[i] == ']') {c = 2;}
    }

NH_END(c == 2)
}

static NH_BOOL Nh_CSS_attributeHit(
    Nh_HTML_Node *Node_p, NH_HTML_ATTRIBUTE type, char *value_p)
{
NH_BEGIN()

    for (int i = 0; i < Node_p->Attributes.count; ++i) {
        Nh_HTML_Attribute *Attribute_p = Nh_getFromLinkedList(&Node_p->Attributes, i);
        if (Attribute_p->type == type) {
            if (value_p != NULL) {NH_END(!strcmp(value_p, Attribute_p->value_p))}
            else {NH_DIAGNOSTIC_END(NH_TRUE)}
        }
    }

NH_DIAGNOSTIC_END(NH_FALSE)
}

static NH_BOOL Nh_CSS_propertyHit(
    Nh_HTML_Node *Node_p, NH_CSS_PROPERTY type, char *value_p)
{
NH_BEGIN()

    for (int i = 0; i < Node_p->Properties.Candidates.count; ++i) {
        if (Nh_CSS_getProperty(&Node_p->Properties.Candidates, i)->type == type) {
            if (value_p == NULL) {NH_DIAGNOSTIC_END(NH_TRUE)}
            for (int j = 0; j < Nh_CSS_getProperty(&Node_p->Properties.Candidates, i)->valueCount; ++j) {
                if (!strcmp(value_p, Nh_CSS_getProperty(&Node_p->Properties.Candidates, i)->values_pp[j])) {NH_DIAGNOSTIC_END(NH_TRUE)}
            }
        }
    }

NH_DIAGNOSTIC_END(NH_FALSE)
}

static NH_BOOL Nh_CSS_attributeSelectorHit(
    const char *selector_p, Nh_HTML_Node *Node_p)
{
NH_BEGIN()

    char element_p[256] = {'\0'};
    int count = 0;
    
    for (int i = 0; i < strlen(selector_p); ++i) {if (selector_p[i] == '[') {break;}++count;}
    memcpy((void*)element_p, (void*)selector_p, sizeof(char) * count);
    
    if (!strcmp(element_p, NH_HTML_TAGS_PP[Node_p->tag]))
    {
        char attr_p[256] = {'\0'}, val_p[256] = {'\0'};
        NH_BOOL attr = NH_FALSE, val = NH_FALSE;
        
        count = 0;
        for (int i = 0; i < strlen(selector_p); ++i) 
        {
            if (selector_p[i] == ']')        {break;}
            if (val && selector_p[i] != '"') {val_p[count++] = selector_p[i];}
            if (selector_p[i] == '=')        {attr = NH_FALSE; val = NH_TRUE; count = 0;}
            if (attr)                        {attr_p[count++] = selector_p[i];}
            if (selector_p[i] == '[')        {attr = NH_TRUE;}
        }

        Nh_HashValue *HashValue_p;
        NH_BOOL validAttribute = NH_FALSE, validProperty = NH_FALSE;

        if (hashmap_get(NH_HASHMAPS.HTML.Attributes, attr_p, (void**)(&HashValue_p)) != MAP_OK) {
            if (hashmap_get(NH_HASHMAPS.CSS.Properties, attr_p, (void**)(&HashValue_p)) != MAP_OK) {NH_DIAGNOSTIC_END(NH_FALSE)}
            else {validProperty = NH_TRUE;}
        } 
        else {validAttribute = NH_TRUE;}

        if (validAttribute) {NH_END(Nh_CSS_attributeHit(Node_p, HashValue_p->number, val_p[0] != '\0' ? val_p : NULL))}
        if (validProperty)  {NH_END(Nh_CSS_propertyHit(Node_p, HashValue_p->number, val_p[0] != '\0' ? val_p : NULL))}
    }

NH_DIAGNOSTIC_END(NH_FALSE)
}

static void Nh_CSS_getSelectorParts(
    char *selectorString_p, NH_CSS_SELECTOR type, char **parts_pp)
{
NH_BEGIN()

    switch (type)
    {
        case NH_CSS_SELECTOR_CHILD_COMBINATOR :
        {
            char *p = strstr(selectorString_p, ">") + 1;
            while (*p == ' ') {p++;}             
            strcpy(parts_pp[1], p);

            strcpy(parts_pp[0], selectorString_p);
            p = strstr(parts_pp[0], ">");
            while (*p == ' ' || *p == '>') {*p = '\0'; p--;}

            break;
        }
     }

NH_SILENT_END()
}


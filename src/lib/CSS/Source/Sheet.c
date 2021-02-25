// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE ========================================================================================

#include "../Header/Sheet.h"
#include "../Header/Properties.h"
#include "../Header/Macros.h"

#include "../../Netzhaut/LinkedList.h"
#include "../../Netzhaut/String.h"
#include "../../Netzhaut/Tab.h"
#include "../../Netzhaut/HashMap.h"
#include "../../Netzhaut/Memory.h"
#include "../../Netzhaut/File.h"

#include "../../HTML/Main/Header/Attribute.h"
#include "../../HTML/Main/Header/Document.h"

#include NH_FLOW
#include NH_DEFAULT_CHECK
#include NH_CSS_UTILS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

// DECLARE ========================================================================================

static inline NH_RESULT Nh_CSS_addDefaultSheets(
    Nh_HTML_Document *Document_p
);
static inline NH_RESULT Nh_CSS_addSheetsFromStyleElements(
    Nh_HTML_Document *Document_p, Nh_HTML_Node *Node_p
);
static inline NH_RESULT Nh_CSS_addSheetsFromLinks(
    Nh_HTML_Document *Document_p, Nh_HTML_Node *Node_p
);
static inline NH_RESULT Nh_CSS_addSheetsFromAttributes(
    Nh_HTML_Document *Document_p, Nh_HTML_Node *Node_p
);

// INIT ============================================================================================

void Nh_CSS_initSheet(
    Nh_CSS_Sheet *Sheet_p)
{
NH_BEGIN()

    Sheet_p->ruleSetCount = 0;
    Sheet_p->RuleSets_p   = NULL;
    Sheet_p->Apply_p      = NULL;

NH_SILENT_END()
}

// GET =============================================================================================

Nh_CSS_Sheet *Nh_CSS_getSheet(
    Nh_LinkedList *Sheets_p, int index)
{
NH_BEGIN()
NH_END(Nh_getFromLinkedList(Sheets_p, index))
}

// ADD =============================================================================================

NH_RESULT Nh_CSS_addSheets(
    Nh_HTML_Document *Document_p, NH_BOOL defaultSheets, NH_BOOL customSheets)
{
NH_BEGIN()

    if (defaultSheets) {NH_CHECK(Nh_CSS_addDefaultSheets(Document_p))}

    if (customSheets) {
        NH_CHECK(Nh_CSS_addSheetsFromStyleElements(Document_p, Document_p->Tree.Root_p))
        NH_CHECK(Nh_CSS_addSheetsFromLinks(Document_p, Document_p->Tree.Root_p))
        NH_CHECK(Nh_CSS_addSheetsFromAttributes(Document_p, Document_p->Tree.Root_p))
    }

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

NH_RESULT Nh_CSS_addSheet(
    Nh_LinkedList *Sheets_p, Nh_CSS_Sheet *Prototype_p)
{
NH_BEGIN()

    NH_CHECK_NULL(Sheets_p)
    NH_CHECK_NULL(Prototype_p)

    Nh_CSS_Sheet *Sheet_p = Nh_allocate(sizeof(Nh_CSS_Sheet));
    NH_CHECK_MEM(Sheet_p)
    
    Sheet_p->RuleSets_p   = NULL;
    Sheet_p->ruleSetCount = Prototype_p->ruleSetCount;
    Sheet_p->Apply_p      = Prototype_p->Apply_p;
    Sheet_p->URI          = Prototype_p->URI;

    if (Sheet_p->ruleSetCount > 0) {Sheet_p->RuleSets_p = Prototype_p->RuleSets_p;}
    
    NH_CHECK(Nh_appendToLinkedList(Sheets_p, Sheet_p))

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

// ADD SHEETS ======================================================================================

static inline NH_RESULT Nh_CSS_addDefaultSheets(
    Nh_HTML_Document *Document_p)
{
NH_BEGIN()

    Nh_CSS_Sheet Sheet;
    Nh_CSS_initSheet(&Sheet);

    Sheet.URI = Nh_createURI("nh:css:misc", NULL, NULL, -1);
    NH_CHECK(Nh_CSS_addSheet(&Document_p->Sheets, &Sheet))

    Sheet.URI = Nh_createURI("nh:css:selectoption", NULL, NULL, -1); 
    NH_CHECK(Nh_CSS_addSheet(&Document_p->Sheets, &Sheet))

    Sheet.URI = Nh_createURI("nh:css:header", NULL, NULL, -1);
    NH_CHECK(Nh_CSS_addSheet(&Document_p->Sheets, &Sheet))

    Sheet.URI = Nh_createURI("nh:css:list", NULL, NULL, -1);
    NH_CHECK(Nh_CSS_addSheet(&Document_p->Sheets, &Sheet))

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

static inline NH_RESULT Nh_CSS_addSheetsFromStyleElements(
    Nh_HTML_Document *Document_p, Nh_HTML_Node *Node_p)
{
NH_BEGIN()

    Nh_CSS_Sheet Sheet;
    Nh_CSS_initSheet(&Sheet);

    if (Node_p->tag == NH_HTML_TAG_STYLE) 
    {
        if (Node_p->Children.last == 0) {
            Sheet.URI = Nh_createURI(NULL, NULL, Node_p->Children.data_pp[0], NH_INTERNAL_URL_HTML_NODE_TEXT);
            NH_CHECK(Nh_CSS_addSheet(&Document_p->Sheets, &Sheet))
        }
    }

    for (int i = 0; i <= Node_p->Children.last; ++i) {
        NH_CHECK(Nh_CSS_addSheetsFromStyleElements(Document_p, Node_p->Children.data_pp[i]))
    }

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

static inline NH_RESULT Nh_CSS_addSheetsFromLinks(
    Nh_HTML_Document *Document_p, Nh_HTML_Node *Node_p)
{
NH_BEGIN()

    Nh_CSS_Sheet Sheet;
    Nh_CSS_initSheet(&Sheet);

    if (Node_p->tag == NH_HTML_TAG_LINK) 
    {
        NH_BOOL sheet = NH_FALSE;
        
        if (Nh_HTML_getRel(Node_p) != NULL) {
            sheet = !strcmp(Nh_HTML_getRel(Node_p), "stylesheet");
        }
        
        if (sheet) 
        {
            if (Nh_HTML_getHRef(Node_p) != NULL) 
            {
                char base_p[512] = {'\0'};
                Nh_HTML_getBase(Document_p, base_p);
                Sheet.URI = Nh_createURI(Nh_HTML_getHRef(Node_p), base_p, NULL, -1);
                NH_CHECK(Nh_CSS_addSheet(&Document_p->Sheets, &Sheet))
            }
        }
    }

    for (int i = 0; i <= Node_p->Children.last; ++i) {
        NH_CHECK(Nh_CSS_addSheetsFromLinks(Document_p, Node_p->Children.data_pp[i]))
    }

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

static inline NH_RESULT Nh_CSS_addSheetsFromAttributes(
    Nh_HTML_Document *Document_p, Nh_HTML_Node *Node_p)
{
NH_BEGIN()

    Nh_CSS_Sheet Sheet;
    Nh_CSS_initSheet(&Sheet);

    for (int i = 0; i < Node_p->Attributes.count; ++i) 
    {
        Nh_HTML_Attribute *Attribute_p = Nh_getFromLinkedList(&Node_p->Attributes, i);
        if (Attribute_p->type == NH_HTML_ATTRIBUTE_STYLE) 
        {
            Sheet.URI = Nh_createURI(NULL, NULL, Attribute_p, NH_INTERNAL_URL_HTML_NODE_ATTRIBUTE_VALUE);
            Sheet.Apply_p = Node_p;

            NH_CHECK(Nh_CSS_addSheet(&Document_p->Sheets, &Sheet))
        }
    }

    for (int i = 0; i <= Node_p->Children.last; ++i) {
        NH_CHECK(Nh_CSS_addSheetsFromAttributes(Document_p, Node_p->Children.data_pp[i]))
    }

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

// DESTROY =========================================================================================

void Nh_CSS_destroySheetList(
    Nh_LinkedList *Sheets_p)
{
NH_BEGIN()

    if (Sheets_p == NULL) {NH_SILENT_END()}

    for (int i = 0; i < Sheets_p->count; ++i) 
    {
        Nh_CSS_Sheet *Sheet_p = Nh_CSS_getSheet(Sheets_p, i);
        if (Sheet_p != NULL) {Nh_CSS_destroySheet(Sheet_p);}
    }
    
    Nh_destroyLinkedList(Sheets_p, true);
    
NH_SILENT_END()
}

void Nh_CSS_destroySheet(
    Nh_CSS_Sheet *Sheet_p)
{
NH_BEGIN()

    Nh_freeURI(&Sheet_p->URI);

    for (int i = 0; i < Sheet_p->ruleSetCount; ++i) 
    {
        if (Sheet_p->RuleSets_p[i].type == NH_CSS_RULE_SET_STYLE) 
        {
            NH_CSS_StyleRuleSet *RuleSet_p = Sheet_p->RuleSets_p[i].Data_p;
            
            for (int j = 0; j < RuleSet_p->selectorCount; ++j) {Nh_free(RuleSet_p->selectors_pp[j]);}
            Nh_free(RuleSet_p->selectors_pp);
            
            for (int j = 0; j < RuleSet_p->declarationCount; ++j) 
            {
                NH_CSS_Declaration *Declaration_p = &RuleSet_p->Declarations_p[j];
                for (int k = 0; k < Declaration_p->valueCount; ++k) {Nh_free(Declaration_p->values_pp[k]);}
                Nh_free(Declaration_p->values_pp);
                Nh_free(Declaration_p->property_p);
            }
            Nh_free(RuleSet_p->Declarations_p);
            Nh_free(RuleSet_p);
        }
    }

    Nh_free(Sheet_p->RuleSets_p);

NH_SILENT_END()
}

// STRINGIFY =======================================================================================

char *Nh_CSS_stringifySheet(
    Nh_CSS_Sheet *Sheet_p)
{
NH_BEGIN()

#include NH_CUSTOM_CHECK

    Nh_StringConstructor *String_p = Nh_allocateStringConstructor(1024);

    NH_CHECK(NULL, Nh_appendFormatToStringConstructor(String_p, "{\n"))
//    NH_CHECK(NULL, Nh_appendFormatToString(String_p, "  \e[1;32mSheet\e[0m: %s\n", Sheet_p->path_p))
    NH_CHECK(NULL, Nh_appendFormatToStringConstructor(String_p, "  Total Rule-Set count: %d\n\n", Sheet_p->ruleSetCount))

    for (int i = 0; i < Sheet_p->ruleSetCount; ++i) 
    {
        if (Sheet_p->RuleSets_p[i].type == NH_CSS_RULE_SET_STYLE)
        {
            NH_CSS_StyleRuleSet *RuleSet_p = Sheet_p->RuleSets_p[i].Data_p;
 
            NH_CHECK(NULL, Nh_appendFormatToStringConstructor(String_p, "  \e[1;32mStyle Rule-Set Nr. %d\e[0m\n", i))              
            NH_CHECK(NULL, Nh_appendFormatToStringConstructor(String_p, "    Selectors:"))

            for (int j = 0; j < RuleSet_p->selectorCount; ++j) {
                if (j == 0) {NH_CHECK(NULL, Nh_appendFormatToStringConstructor(String_p, " \e[1;34m%s\e[0m\n", RuleSet_p->selectors_pp[j]))}
                else        {NH_CHECK(NULL, Nh_appendFormatToStringConstructor(String_p, "               \e[1;34m%s\e[0m\n", RuleSet_p->selectors_pp[j]))}
            }

            NH_CHECK(NULL, Nh_appendFormatToStringConstructor(String_p, "    Declarations:\n"))

            int max = 0;
            for (int j = 0; j < RuleSet_p->declarationCount; ++j) {
                if (max < strlen(RuleSet_p->Declarations_p[j].property_p)) {max = strlen(RuleSet_p->Declarations_p[j].property_p);}
            }

            for (int j = 0; j < RuleSet_p->declarationCount; ++j)
            {
                NH_CHECK(NULL, Nh_appendFormatToStringConstructor(String_p, "      Property: \e[1;36m%s\e[0m", RuleSet_p->Declarations_p[j].property_p))

                for (int k = 0; k < max - strlen(RuleSet_p->Declarations_p[j].property_p); ++k) {NH_CHECK(NULL, Nh_appendFormatToStringConstructor(String_p, " "))}
                NH_CHECK(NULL, Nh_appendFormatToStringConstructor(String_p, " : "))

                for (int k = 0; k < RuleSet_p->Declarations_p[j].valueCount; ++k) 
                {
                    if (k == 0) {NH_CHECK(NULL, Nh_appendFormatToStringConstructor(String_p, "%s\n", RuleSet_p->Declarations_p[j].values_pp[k]))}
                    else {
                        for (int l = 0; l < max + 16; ++l) {
                             NH_CHECK(NULL, Nh_appendFormatToStringConstructor(String_p, " "))
                        } 
                        NH_CHECK(NULL, Nh_appendFormatToStringConstructor(String_p, " : %s\n", RuleSet_p->Declarations_p[j].values_pp[k]))
                    }
                }
            }

            if (i + 1 < Sheet_p->ruleSetCount) {NH_CHECK(NULL, Nh_appendFormatToStringConstructor(String_p, "\n"))}
        }
    }

    NH_CHECK(NULL, Nh_appendFormatToStringConstructor(String_p, "}\n"))

    char *string_p = Nh_getChars2(String_p);
    Nh_freeStringConstructor(String_p);

#include NH_DEFAULT_CHECK

NH_END(string_p)
}


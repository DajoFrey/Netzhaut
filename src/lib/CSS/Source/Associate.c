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
#include "../Header/Selector.h"

#include "../../Netzhaut/LinkedList.h"
#include "../../Netzhaut/HashMap.h"

#include "../../HTML/Main/Header/Attribute.h"

#include NH_FLOW
#include NH_DEFAULT_CHECK

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

// DECLARE =========================================================================================

static NH_RESULT Nh_CSS_dispatchRuleSet(
    Nh_CSS_Sheet *Sheet_p, NH_CSS_RuleSet *RuleSet_p, Nh_HTML_Node *Node_p, bool recursive, 
    bool hitRequired
);
static NH_RESULT Nh_CSS_handleStyleRuleSet(
    Nh_CSS_Sheet *Sheet_p, NH_CSS_StyleRuleSet *RuleSet_p, Nh_HTML_Node *Node_p, bool recursive, 
    bool hitRequired
);
static NH_RESULT Nh_CSS_addStyleProperties(
    Nh_CSS_Sheet *Sheet_p, NH_CSS_StyleRuleSet *RuleSet_p, Nh_HTML_Node* Node_p,
    Nh_CSS_Selector *Selector_p
);

// APPLY ===========================================================================================

static NH_RESULT Nh_CSS_associateSheet(
    Nh_HTML_Document *Document_p, Nh_CSS_Sheet *Sheet_p, Nh_HTML_Node *Node_p)
{   
NH_BEGIN()

    for (size_t i = 0; i < Sheet_p->ruleSetCount; ++i) 
    {
        if (Node_p != NULL && (Sheet_p->Apply_p == NULL || Sheet_p->Apply_p == Node_p)) {
            NH_CHECK(Nh_CSS_dispatchRuleSet(
                Sheet_p, &Sheet_p->RuleSets_p[i], Node_p, false, true
            ))
        }
        else if (Node_p == NULL && Sheet_p->Apply_p == NULL) {
            NH_CHECK(Nh_CSS_dispatchRuleSet(
                Sheet_p, &Sheet_p->RuleSets_p[i], Nh_HTML_getNode(Document_p, NH_HTML_TAG_BODY), 
                true, true
            ))
        }
        else if (Node_p == NULL && Sheet_p->Apply_p != NULL) {
            NH_CHECK(Nh_CSS_dispatchRuleSet(
                Sheet_p, &Sheet_p->RuleSets_p[i], Sheet_p->Apply_p, false, false
            ))
        }
    }

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

NH_RESULT Nh_CSS_associateSheets(
    Nh_HTML_Document *Document_p, Nh_HTML_Node *Node_p)
{   
NH_BEGIN()

    for (int i = 0; i < Document_p->Sheets.count; ++i) {
        NH_CHECK(Nh_CSS_associateSheet(
            Document_p, Nh_CSS_getSheet(&Document_p->Sheets, i), Node_p
        ))
    }

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

NH_RESULT Nh_CSS_reassociateSheets(
    Nh_HTML_Document *Document_p, Nh_HTML_Node *Node_p)
{   
NH_BEGIN()

    Nh_CSS_destroyGenericProperties(&Node_p->Properties.Candidates);
    NH_CHECK(Nh_CSS_associateSheets(Document_p, Node_p))

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

// DISPATCH ========================================================================================

static NH_RESULT Nh_CSS_dispatchRuleSet(
    Nh_CSS_Sheet *Sheet_p, NH_CSS_RuleSet* RuleSet_p, Nh_HTML_Node* Node_p, bool recursive, 
    bool hitRequired)
{
NH_BEGIN()

    switch (RuleSet_p->type) 
    {
        case NH_CSS_RULE_SET_STYLE      : 
        {
            Nh_CSS_handleStyleRuleSet(
                Sheet_p, RuleSet_p->Data_p, Node_p, recursive, hitRequired
            ); 
            break;
        }
        case NH_CSS_RULE_SET_IMPORT     : break;
        case NH_CSS_RULE_SET_FONT_FACE  : break;
        case NH_CSS_RULE_SET_KEY_FRAMES : break;
        case NH_CSS_RULE_SET_MEDIA      : break;
        case NH_CSS_RULE_SET_SUPPORTS   : break;
        default                         : break;
    }

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

// STYLE RULE-SET ==================================================================================

static NH_RESULT Nh_CSS_handleStyleRuleSet(
    Nh_CSS_Sheet *Sheet_p, NH_CSS_StyleRuleSet *RuleSet_p, Nh_HTML_Node* Node_p, bool recursive, 
    bool hitRequired)
{
NH_BEGIN()

    if (!hitRequired) {
        // TODO
    }
    else {
        for (size_t i = 0; i < RuleSet_p->selectorCount; ++i) 
        {
            Nh_CSS_Selector Selector;
            if (Nh_CSS_selectorPreHit(Node_p, RuleSet_p->selectors_pp[i], &Selector)) {
                Nh_CSS_addStyleProperties(Sheet_p, RuleSet_p, Node_p, &Selector);
            }
        }
    }

    if (recursive) {
        for (int i = 0; i <= Node_p->Children.last; ++i) {
            Nh_CSS_handleStyleRuleSet(
                Sheet_p, RuleSet_p, Node_p->Children.data_pp[i], true, hitRequired
            );
        }
    }

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

static NH_RESULT Nh_CSS_addStyleProperties(
    Nh_CSS_Sheet *Sheet_p, NH_CSS_StyleRuleSet *RuleSet_p, Nh_HTML_Node* Node_p,
    Nh_CSS_Selector *Selector_p)
{
NH_BEGIN()

    for (int i = 0; i < RuleSet_p->declarationCount; ++i)
    {
        NH_CSS_Declaration *Declaration_p = &RuleSet_p->Declarations_p[i];

        Nh_HashValue* HashValue_p;
        int error = hashmap_get(NH_HASHMAPS.CSS.Properties, (char*)Declaration_p->property_p, (void**)(&HashValue_p));
        if (error != MAP_OK) {NH_DIAGNOSTIC_END(NH_ERROR_BAD_STATE)}
    
        Nh_CSS_GenericProperty Property = {0};

        Property.Sheet_p    = Sheet_p;
        Property.type       = HashValue_p->number;
        Property.Selector   = *Selector_p;
        Property.active     = NH_FALSE;
        Property.update     = NH_TRUE;
        Property.valueCount = Declaration_p->valueCount;
        Property.values_pp  = Declaration_p->values_pp;

        NH_CHECK(Nh_CSS_addGenericProperty(&Node_p->Properties.Candidates, &Property))
    }

NH_DIAGNOSTIC_END(NH_SUCCESS)
}


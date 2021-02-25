// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE ========================================================================================

#include "../Header/Parser.h"
#include "../Header/Sheet.h"
#include "../Header/Properties.h"
#include "../Header/Macros.h"
#include "../Header/Log.h"

#include "../../Netzhaut/LinkedList.h"
#include "../../Netzhaut/Tab.h"
#include "../../Netzhaut/HashMap.h"
#include "../../Netzhaut/Memory.h"

#include "../../HTML/Main/Header/Attribute.h"

#include NH_FLOW
#include NH_DEFAULT_CHECK

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

// DECLARE ========================================================================================

static inline NH_RESULT Nh_CSS_parseRuleSet(
    char *begin_p, NH_CSS_StyleRuleSet *Rule_p, int i
);
static inline NH_RESULT Nh_CSS_parseDeclaration(
    char *declBegin_p, NH_CSS_Declaration *Declaration_p
);
static inline NH_RESULT Nh_CSS_parseSelectors(
    char *begin_p, NH_CSS_StyleRuleSet *RuleSet_p
);
static inline NH_RESULT Nh_CSS_parseProperty(
    char *declBegin_p, NH_CSS_Declaration *Declaration_p
);
static inline NH_RESULT Nh_CSS_parsePropertyValues(
    char *valueBegin_p, NH_CSS_Declaration *Declaration_p
);

static inline char *Nh_CSS_next(
    char *str_p
);
static inline void Nh_CSS_getSelectors(
    char *begin_p, char *selector_p, size_t size
);
static inline char *Nh_CSS_jumpOverSelectors(
    char *begin_p
);

static inline Nh_LinkedList Nh_CSS_getRuleSetBeginnings(
    Nh_CSS_Sheet *Sheet_p
);
static inline char *Nh_CSS_getDeclarationBegin(
    char *begin_p, int number, int *count_p
);
static inline Nh_LinkedList Nh_CSS_getPropertyValueBeginnings(
    char *valueBegin_p
);

// PARSE ===========================================================================================

NH_RESULT Nh_CSS_parseSheet(
    Nh_CSS_Sheet *Sheet_p)
{
NH_BEGIN()

    NH_CHECK_NULL(Sheet_p)

    Nh_LinkedList RuleSets = Nh_CSS_getRuleSetBeginnings(Sheet_p);

    if (RuleSets.count > 0) 
    {
        Sheet_p->RuleSets_p = Nh_allocate(sizeof(NH_CSS_RuleSet) * RuleSets.count);
        NH_CHECK_MEM(Sheet_p->RuleSets_p)

        for (int i = 0; i < RuleSets.count; ++i) {
            Sheet_p->RuleSets_p[i].type = NH_CSS_RULE_SET_STYLE;
            Sheet_p->RuleSets_p[i].Data_p = Nh_allocate(sizeof(NH_CSS_StyleRuleSet));
            NH_CHECK_MEM(Sheet_p->RuleSets_p[i].Data_p)
        }
    }

    for (int i = 0; i < RuleSets.count; ++i) {
        NH_CHECK(Nh_CSS_parseRuleSet(
            Nh_getFromLinkedList(&RuleSets, i), Sheet_p->RuleSets_p[i].Data_p, i
        ))
    }

    Sheet_p->ruleSetCount = RuleSets.count;
    Nh_destroyLinkedList(&RuleSets, false);

    Nh_CSS_logSheet(Sheet_p);

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

NH_RESULT Nh_CSS_parseSheets(
    Nh_LinkedList *Sheets_p)
{
NH_BEGIN()

    for (int i = 0; i < Sheets_p->count; ++i) {
        NH_CHECK(Nh_CSS_parseSheet(Nh_getFromLinkedList(Sheets_p, i)))
    }

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

static inline NH_RESULT Nh_CSS_parseRuleSet(
    char *begin_p, NH_CSS_StyleRuleSet *RuleSet_p, int i)
{
NH_BEGIN()

    Nh_CSS_parseSelectors(begin_p, RuleSet_p);

    begin_p = Nh_CSS_jumpOverSelectors(begin_p);

    Nh_CSS_getDeclarationBegin(begin_p, 0, &RuleSet_p->declarationCount);

    RuleSet_p->Declarations_p = Nh_allocate(sizeof(NH_CSS_Declaration) * RuleSet_p->declarationCount);
    NH_CHECK_MEM(RuleSet_p->Declarations_p)

    for (int j = 1; j <= RuleSet_p->declarationCount; ++j) 
    {
        char *declBegin_p = Nh_CSS_getDeclarationBegin(begin_p, j, NULL);
        NH_CHECK_NULL(declBegin_p)

        Nh_CSS_parseDeclaration(declBegin_p, &RuleSet_p->Declarations_p[j - 1]);
    }

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

static inline NH_RESULT Nh_CSS_parseSelectors(
    char *begin_p, NH_CSS_StyleRuleSet *RuleSet_p)
{
NH_BEGIN()

    char selectors_p[2048] = {'\0'}; // TODO possible segfault
    Nh_CSS_getSelectors(Nh_CSS_next(begin_p), selectors_p, 2048);

    RuleSet_p->selectorCount = 1;
    
    for (int i = 0; i < strlen(selectors_p); ++i) {
        if (selectors_p[i] == ',') {RuleSet_p->selectorCount++;}
    }

    RuleSet_p->selectors_pp = Nh_allocate(sizeof(char*) * RuleSet_p->selectorCount);
    NH_CHECK_MEM(RuleSet_p->selectors_pp)

    char tmp_p[128] = {'\0'};
    NH_BOOL hit = NH_FALSE;

    for (int i = 0, charIndex = 0, selectorIndex = 0; i < strlen(selectors_p); ++i) 
    {
        if (selectors_p[i] != ' ') {hit = NH_TRUE;}
        if (hit) {
            tmp_p[charIndex++] = selectors_p[i];
        }

        if (selectors_p[i] == ',' || i == strlen(selectors_p) - 1) 
        {
            while (tmp_p[charIndex - 1] == ',' || tmp_p[charIndex -1] == ' ') {tmp_p[--charIndex] = '\0';}

            RuleSet_p->selectors_pp[selectorIndex] = Nh_allocate(sizeof(char) * (charIndex + 1));
            NH_CHECK_MEM(RuleSet_p->selectors_pp[selectorIndex])

            strcpy(RuleSet_p->selectors_pp[selectorIndex++], tmp_p);

            memset(tmp_p, '\0', sizeof(char) * 128);
            charIndex = 0;
            hit = NH_FALSE;
        }
    }

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

static inline NH_RESULT Nh_CSS_parseDeclaration(
    char *declBegin_p, NH_CSS_Declaration *Declaration_p)
{
NH_BEGIN()

    NH_CHECK(Nh_CSS_parseProperty(declBegin_p, Declaration_p))

    int count = 0;
    for (count = 0; declBegin_p[count] != ':'; ++count);
    
    char *valueBegin_p = declBegin_p + count + 1;
    valueBegin_p = Nh_CSS_next(valueBegin_p);

    NH_CHECK(Nh_CSS_parsePropertyValues(valueBegin_p, Declaration_p))

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

static inline NH_RESULT Nh_CSS_parseProperty(
    char *declBegin_p, NH_CSS_Declaration *Declaration_p)
{
NH_BEGIN()

    int count = 0;
    for (int i = 0; declBegin_p[i] != ':'; ++i) {if (declBegin_p[i] != ' ' && declBegin_p[i] != '\n') {count++;}}

    Declaration_p->property_p = Nh_allocate(sizeof(char) * count + 1);
    NH_CHECK_MEM(Declaration_p->property_p)

    count = 0;
    for (int i = 0; declBegin_p[i] != ':'; ++i) {if (declBegin_p[i] != ' ' && declBegin_p[i] != '\n') {Declaration_p->property_p[count++] = declBegin_p[i];}}
    Declaration_p->property_p[count] = '\0';

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

static inline NH_RESULT Nh_CSS_parsePropertyValues(
    char *valuesBegin_p, NH_CSS_Declaration *Declaration_p)
{
NH_BEGIN()

    bool inFunction = false;
    bool inBetween = false;
    bool inString = false;

    Nh_LinkedList Values = Nh_CSS_getPropertyValueBeginnings(valuesBegin_p);

    Declaration_p->valueCount = Values.count;
    Declaration_p->values_pp = Nh_allocate(sizeof(char*) * Values.count);
    NH_CHECK_MEM(Declaration_p->values_pp)

    for (int i = 0; i < Declaration_p->valueCount; ++i) 
    {
        char *valueBegin_p = Nh_getFromLinkedList(&Values, i);

        int charCount = 0;
        for (charCount = 0; inFunction || inString || (valueBegin_p[charCount] != ' ' && valueBegin_p[charCount] != '\r' && valueBegin_p[charCount] != '\n' && valueBegin_p[charCount] != ';' && valueBegin_p[charCount] != '}'); ++charCount) {
            if (valueBegin_p[charCount] == '(') {inFunction = true;}
            if (valueBegin_p[charCount] == ')') {inFunction = false;}
            if (valueBegin_p[charCount] == '"' || valueBegin_p[charCount] == 39) {inString = !inString;}
        }

        Declaration_p->values_pp[i] = Nh_allocate(sizeof(char) * (charCount + 1));
        NH_CHECK_MEM(Declaration_p->values_pp[i])

        charCount = 0;
        for (charCount = 0; inFunction || inString || (valueBegin_p[charCount] != ' ' && valueBegin_p[charCount] != '\r' && valueBegin_p[charCount] != '\n' && valueBegin_p[charCount] != ';' && valueBegin_p[charCount] != '}'); ++charCount) {
            if (valueBegin_p[charCount] == '(') {inFunction = true;}
            if (valueBegin_p[charCount] == ')') {inFunction = false;}
            if (valueBegin_p[charCount] == '"' || valueBegin_p[charCount] == 39) {inString = !inString;}
            Declaration_p->values_pp[i][charCount] = valueBegin_p[charCount];
        }
        Declaration_p->values_pp[i][charCount] = '\0';
    }

    Nh_destroyLinkedList(&Values, false);

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

static inline Nh_LinkedList Nh_CSS_getRuleSetBeginnings(
    Nh_CSS_Sheet *Sheet_p)
{
NH_BEGIN()

    Nh_LinkedList List;
    NH_INIT_LINKED_LIST(List)

    char *source_p = Nh_getPayload(&Sheet_p->URI);
    char *begin_p = Nh_CSS_next(Nh_getPayload(&Sheet_p->URI));
    bool inRule = false, atRule = false;

    int depth = 0;    
    while (source_p != NULL) 
    {
        source_p = Nh_CSS_next(source_p);
        if (source_p == NULL) {break;}

        if (inRule && *source_p == '}') {
            --depth;
            if (atRule && depth == 0) {
                atRule = inRule = false;
                begin_p = source_p + 1;
            }
            else if (!atRule) {
                inRule = false;
                Nh_appendToLinkedList(&List, begin_p);
                begin_p = Nh_CSS_next(source_p + 1);
            }
        }

        if (*source_p == '{')            {inRule = true; depth += 1;} 
        if (!inRule && *source_p == '@') {atRule = true;}

        if (strlen(source_p) > 1) {source_p = source_p + 1;}
        else {break;} 
    }

NH_END(List)
}

static inline char *Nh_CSS_next(
    char *str_p)
{
NH_BEGIN()

    int comment = 0;

    for (int i = 0; i < strlen(str_p); ++i)
    {
        if (comment == 3 && str_p[i] != '/') {comment = 2;}
        if (comment == 3 && str_p[i] == '/') {comment = 0;}
        
        if (comment == 2 && str_p[i] == '*') {comment = 3;}

        if (comment == 1 && str_p[i] == '*') {comment = 2;}
        if (comment == 1 && str_p[i] != '*') {comment = 0;}

        if (comment == 0 && str_p[i] == '/') {comment = 1;}
        if (comment == 0 && str_p[i] != ' ' && str_p[i] != '\n' && str_p[i] != '\t' && str_p[i] != '\v' && str_p[i] != '\r') {NH_END(str_p + i)}
    }

NH_END(NULL)
}

static inline void Nh_CSS_getSelectors(
    char *begin_p, char *selector_p, size_t size)
{
NH_BEGIN()

    int count = 0;
    for (int i = 0; i < size; ++i) {
        if (begin_p[i] != '{') {
            if (begin_p[i] != '\n' && begin_p[i] != '\r') {selector_p[count++] = begin_p[i];} 
        } else {break;}
    }

NH_SILENT_END()
}

static inline char *Nh_CSS_jumpOverSelectors(
    char *begin_p)
{
NH_BEGIN()

    bool rule = false;

    for (int i = 0; i < strlen(begin_p); ++i) 
    {
        if (rule) {NH_END(Nh_CSS_next(begin_p + i))}
        if (begin_p[i] == '{') {rule = true;}
    }

NH_END(NULL)
}

static inline char *Nh_CSS_getDeclarationBegin(
    char *begin_p, int number, int *count_p)
{
NH_BEGIN()

    char *declBegin_p = begin_p;
    int declCount = 0;
    bool inDecl = false;

    for (int i = 0; i < strlen(begin_p); ++i) 
    {
        if (begin_p[i] == ':') {inDecl = true;}
        
        if (inDecl && (begin_p[i] == ';' || begin_p[i] == '}')) {
            inDecl = false; 
            declCount++; 
            if (number > 0 && declCount == number) {NH_END(Nh_CSS_next(declBegin_p))}
            declBegin_p = begin_p + i + 1;
        }
 
        if (begin_p[i] == '}') {break;}
    }

    if (count_p != NULL) {*count_p = declCount;}

NH_END(NULL)
}

static inline Nh_LinkedList Nh_CSS_getPropertyValueBeginnings(
    char *valueBegin_p)
{
NH_BEGIN()

    Nh_LinkedList Values;
    NH_INIT_LINKED_LIST(Values)

    char *begin_p = valueBegin_p;
    bool inFunction = false, inBetween = false, inString = false; 

    for (int i = 0; strlen(valueBegin_p); ++i) 
    {
        if (valueBegin_p[i] == '(')               {inFunction = true;}
        if (valueBegin_p[i] == ')' && inFunction) {inFunction = false;}

        if (valueBegin_p[i] == '"' || valueBegin_p[i] == 39) {inString = !inString;}

        if (!inFunction && !inString && inBetween && (valueBegin_p[i] != ' ' || valueBegin_p[i] != '\n' || valueBegin_p[i] != '\r')) {
            begin_p = valueBegin_p + i; inBetween = false;
        }
        if (!inFunction && !inString && !inBetween && (valueBegin_p[i] == ' ' || valueBegin_p[i] == ';' || valueBegin_p[i] == '}')) {
            Nh_appendToLinkedList(&Values, Nh_CSS_next(begin_p));
            inBetween = true;
        }

        if (!inFunction && !inString && (valueBegin_p[i] == ';' || valueBegin_p[i] == '}')) {break;}
    }

NH_END(Values)
}


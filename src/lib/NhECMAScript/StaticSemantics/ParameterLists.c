// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "ParameterLists.h"

#include "../Engine/Type.h"
#include "../Common/Macro.h"
#include NH_ECMASCRIPT_FLOW
#include NH_ECMASCRIPT_CUSTOM_CHECK

#include "../../NhCore/Memory.h"
#include "../../NhCore/Unicode.h"

#include <string.h>

// HAS INITIALIZER =================================================================================

// https://tc39.es/ecma262/#sec-static-semantics-hasinitializer
static NH_BOOL Nh_ECMAScript_hasInitializer(
    Nh_ECMAScript_ParseNode *Node_p)
{
NH_ECMASCRIPT_BEGIN()

    switch (Node_p->type)
    {
        case NH_ECMASCRIPT_PARSE_NODE_FORMAL_PARAMETER :
            NH_ECMASCRIPT_END(Nh_ECMAScript_hasInitializer(Node_p->Children.handles_pp[0]))

        case NH_ECMASCRIPT_PARSE_NODE_BINDING_ELEMENT :
            if (Node_p->Children.size == 1) {
                if (((Nh_ECMAScript_ParseNode*)Node_p->Children.handles_pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_BINDING_PATTERN) {
                    NH_ECMASCRIPT_END(NH_FALSE)
                }
            }
            else {NH_ECMASCRIPT_END(NH_TRUE)}
            break;

        case NH_ECMASCRIPT_PARSE_NODE_SINGLE_NAME_BINDING :
            if (Node_p->Children.size == 1) {
                NH_ECMASCRIPT_END(NH_FALSE)
            }
            else {NH_ECMASCRIPT_END(NH_TRUE)}
            break;

        case NH_ECMASCRIPT_PARSE_NODE_FORMAL_PARAMETER_LIST :
        {
            NH_BOOL hasInitializer = Nh_ECMAScript_hasInitializer(Node_p->Children.handles_pp[0]);
            if (!hasInitializer && Node_p->Children.size == 2) {
                hasInitializer = Nh_ECMAScript_hasInitializer(Node_p->Children.handles_pp[1]);
            }
            NH_ECMASCRIPT_END(hasInitializer)
        }
    }

NH_ECMASCRIPT_END(NH_FALSE)
}

// PARAMETER LISTS =================================================================================

int Nh_ECMAScript_getExpectedArgumentCount(
    Nh_ECMAScript_ParseNode *Node_p)
{
NH_ECMASCRIPT_BEGIN()

    switch (Node_p->type)
    {
        case NH_ECMASCRIPT_PARSE_NODE_FORMAL_PARAMETERS :
            if (Node_p->Children.size == 2) {
                NH_ECMASCRIPT_END(Nh_ECMAScript_getExpectedArgumentCount(Node_p->Children.handles_pp[0])) 
            }
            break;

        case NH_ECMASCRIPT_PARSE_NODE_FORMAL_PARAMETER_LIST :
            if (Node_p->Children.size == 1) {
                if (Nh_ECMAScript_hasInitializer(Node_p->Children.handles_pp[0])) {NH_ECMASCRIPT_END(0)}
                NH_ECMASCRIPT_END(1)
            }
            else {
                int count = Nh_ECMAScript_getExpectedArgumentCount(Node_p->Children.handles_pp[0]);
                if (Nh_ECMAScript_hasInitializer(Node_p->Children.handles_pp[0]) || Nh_ECMAScript_hasInitializer(Node_p->Children.handles_pp[1])) {
                    NH_ECMASCRIPT_END(count)
                }
                NH_ECMASCRIPT_END(count + 1)
            }

// ArrowParameters : BindingIdentifier
//
//    Return 1.
//
//ArrowParameters : CoverParenthesizedExpressionAndArrowParameterList
//
//    Let formals be CoveredFormalsList of CoverParenthesizedExpressionAndArrowParameterList.
//    Return ExpectedArgumentCount of formals.
//
//PropertySetParameterList : FormalParameter
//
//    If HasInitializer of FormalParameter is true, return 0.
//    Return 1.
//
//AsyncArrowBindingIdentifier : BindingIdentifier
//
//    Return 1.
    }

NH_ECMASCRIPT_END(0)
}


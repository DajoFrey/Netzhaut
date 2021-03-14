// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "Misc.h"

#include "../Engine/Type.h"
#include "../Common/Macro.h"
#include NH_ECMASCRIPT_FLOW
#include NH_ECMASCRIPT_CUSTOM_CHECK

#include "../../NhCore/Memory.h"
#include "../../NhCore/Unicode.h"

#include <string.h>

// HAS NAME ========================================================================================

static NH_BOOL Nh_ECMAScript_hasName(
    Nh_ECMAScript_ParseNode *Node_p)
{
NH_ECMASCRIPT_BEGIN()

//    switch (Node_p->type)
//    {
//        case NH_ECMASCRIPT_PARSE_NODE_PRIMARY_EXPRESSION :
//            // TODO
//            break;
//
//        case NH_ECMASCRIPT_PARSE_NODE_FUNCTION_EXPRESSION :
//        case NH_ECMASCRIPT_PARSE_NODE_GENERATOR_EXPRESSION :
//        case NH_ECMASCRIPT_PARSE_NODE_ASYNC_GENERATOR_EXPRESSION :
//        case NH_ECMASCRIPT_PARSE_NODE_ASYNC_FUNCTION_EXPRESSION :
//        case NH_ECMASCRIPT_PARSE_NODE_CLASS_EXPRESSION :
//            if (((Nh_ECMAScript_ParseNode*)Node_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_BINDING_IDENTIFIER) {
//                NH_ECMASCRIPT_END(NH_TRUE)
//            }
//            break;
//    }

NH_ECMASCRIPT_END(NH_FALSE)
}

// IS FUNCTION DEFINITION ==========================================================================

static NH_BOOL Nh_ECMAScript_isFunctionDefinition(
    Nh_ECMAScript_ParseNode *Node_p)
{
NH_ECMASCRIPT_BEGIN()

//    switch (Node_p->type)
//    {
//        case NH_ECMASCRIPT_PARSE_NODE_PRIMARY_EXPRESSION :
//            // TODO
//            break;
//
//        case NH_ECMASCRIPT_PARSE_NODE_ASSIGNMENT_EXPRESSION :
//            if (((Nh_ECMAScript_ParseNode*)Node_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_ARROW_FUNCTION
//            ||  ((Nh_ECMAScript_ParseNode*)Node_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_ASYNC_ARROW_FUNCTION) {
//                NH_ECMASCRIPT_END(NH_TRUE)
//            }
//            break;
//
//        case NH_ECMASCRIPT_PARSE_NODE_FUNCTION_EXPRESSION :
//        case NH_ECMASCRIPT_PARSE_NODE_GENERATOR_EXPRESSION :
//        case NH_ECMASCRIPT_PARSE_NODE_ASYNC_GENERATOR_EXPRESSION :
//        case NH_ECMASCRIPT_PARSE_NODE_ASYNC_FUNCTION_EXPRESSION :
//        case NH_ECMASCRIPT_PARSE_NODE_CLASS_EXPRESSION :
//            NH_ECMASCRIPT_END(NH_TRUE)
//    }

NH_ECMASCRIPT_END(NH_FALSE)
}

// IS ANONYMOUS FUNCTION DEFINITION ================================================================

NH_BOOL Nh_ECMAScript_isAnonymousFunctionDefinition(
    Nh_ECMAScript_ParseNode *Node_p)
{
NH_ECMASCRIPT_BEGIN()

    if (!Nh_ECMAScript_isFunctionDefinition(Node_p)) {NH_ECMASCRIPT_END(NH_FALSE)}
    NH_BOOL hasName = Nh_ECMAScript_hasName(Node_p);
    if (hasName) {NH_ECMASCRIPT_END(NH_FALSE)}

NH_ECMASCRIPT_END(NH_TRUE)
}

// GET STRING VALUE ================================================================================

Nh_Encoding_UTF8String *Nh_ECMAScript_getStringValue(
    Nh_ECMAScript_ParseNode *Node_p)
{
NH_ECMASCRIPT_BEGIN()

//    switch (Node_p->type)
//    {
//        case NH_ECMASCRIPT_PARSE_NODE_IDENTIFIER_REFERENCE :
//        case NH_ECMASCRIPT_PARSE_NODE_BINDING_IDENTIFIER :
//        case NH_ECMASCRIPT_PARSE_NODE_LABEL_IDENTIFIER :
//            if (Node_p->Value_p != NULL) {NH_ECMASCRIPT_END(&Node_p->Value_p->String)}
//            NH_ECMASCRIPT_END(Nh_ECMAScript_getStringValue(Node_p->Children.pp[0]))
//
//        case NH_ECMASCRIPT_PARSE_NODE_IDENTIFIER :
//            NH_ECMASCRIPT_END(&Node_p->Value_p->String)
//    }

NH_ECMASCRIPT_END(NULL)
}

// IS CONSTANT DECLARATION =========================================================================

NH_BOOL Nh_ECMAScript_isConstantDeclaration(
    Nh_ECMAScript_ParseNode *Node_p)
{
NH_ECMASCRIPT_BEGIN()

//    switch (Node_p->type)
//    {
//        case NH_ECMASCRIPT_PARSE_NODE_LEXICAL_DECLARATION :
//            NH_ECMASCRIPT_END(Nh_ECMAScript_isConstantDeclaration(Node_p->Children.pp[0]))
//
//        case NH_ECMASCRIPT_PARSE_NODE_LET_OR_CONST :
//            if (!strcmp(Node_p->Value_p->String.bytes_p, "let")) {
//                NH_ECMASCRIPT_END(NH_FALSE)
//            }
//            else if (!strcmp(Node_p->Value_p->String.bytes_p, "const")) {
//                NH_ECMASCRIPT_END(NH_TRUE)
//            }
//            break;
//
//        case NH_ECMASCRIPT_PARSE_NODE_FUNCTION_DECLARATION :
//        case NH_ECMASCRIPT_PARSE_NODE_GENERATOR_DECLARATION :
//        case NH_ECMASCRIPT_PARSE_NODE_ASYNC_GENERATOR_DECLARATION :
//        case NH_ECMASCRIPT_PARSE_NODE_ASYNC_FUNCTION_DECLARATION :
//            NH_ECMASCRIPT_END(NH_FALSE)
//
//        case NH_ECMASCRIPT_PARSE_NODE_CLASS_DECLARATION :
//            NH_ECMASCRIPT_END(NH_FALSE)
//
//        case NH_ECMASCRIPT_PARSE_NODE_EXPORT_DECLARATION :
//            NH_ECMASCRIPT_END(NH_FALSE)
//    }

NH_ECMASCRIPT_END(NH_FALSE)
}


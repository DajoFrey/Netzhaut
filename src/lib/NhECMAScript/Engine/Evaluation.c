// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE ========================================================================================

#include "Evaluation.h"
#include "ExecutionContext.h"
#include "CompletionOrReference.h"

#include "../StaticSemantics/Misc.h"
#include "../Common/Macro.h"
#include NH_ECMASCRIPT_FLOW
#include NH_ECMASCRIPT_DEFAULT_CHECK

#include <string.h>
#include <stdio.h>

// EVALUATE ========================================================================================

static Nh_ECMAScript_CompletionOrReference Nh_ECMAScript_evaluateIdentifierReference(
    Nh_ECMAScript_ParseNode *Node_p)
{
NH_ECMASCRIPT_BEGIN()
NH_ECMASCRIPT_END(Nh_ECMAScript_wrapReference(Nh_ECMAScript_resolveBinding(&((Nh_ECMAScript_ParseNode*)Node_p->Children.pp[0])->Value_p->String, NULL)))
}

static Nh_ECMAScript_CompletionOrReference Nh_ECMAScript_evaluateNullLiteral(
    Nh_ECMAScript_ParseNode *Node_p)
{
NH_ECMASCRIPT_BEGIN()
NH_ECMASCRIPT_END(Nh_ECMAScript_wrapCompletion(Nh_ECMAScript_normalCompletion(Nh_ECMAScript_wrapNull())))
}

static Nh_ECMAScript_CompletionOrReference Nh_ECMAScript_evaluateBooleanLiteral(
    Nh_ECMAScript_ParseNode *Node_p)
{
NH_ECMASCRIPT_BEGIN()

    if (!strcmp("true", ((Nh_ECMAScript_ParseNode*)Node_p->Children.pp[0])->Value_p->String.bytes_p)) {
        NH_ECMASCRIPT_END(Nh_ECMAScript_wrapCompletion(Nh_ECMAScript_normalCompletion(Nh_ECMAScript_wrapBoolean(NH_TRUE))))
    }
    else if (!strcmp("false", ((Nh_ECMAScript_ParseNode*)Node_p->Children.pp[0])->Value_p->String.bytes_p)) {
        NH_ECMASCRIPT_END(Nh_ECMAScript_wrapCompletion(Nh_ECMAScript_normalCompletion(Nh_ECMAScript_wrapBoolean(NH_FALSE))))
    }

NH_ECMASCRIPT_END(Nh_ECMAScript_wrapCompletion(Nh_ECMAScript_throwTypeError()))
}

static Nh_ECMAScript_CompletionOrReference Nh_ECMAScript_evaluateLiteral(
    Nh_ECMAScript_ParseNode *Node_p)
{
NH_ECMASCRIPT_BEGIN()

    switch (((Nh_ECMAScript_ParseNode*)Node_p->Children.pp[0])->type)
    {
        case NH_ECMASCRIPT_PARSE_NODE_NULL_LITERAL :
            NH_ECMASCRIPT_END(Nh_ECMAScript_evaluateNullLiteral(Node_p->Children.pp[0]))
        case NH_ECMASCRIPT_PARSE_NODE_BOOLEAN_LITERAL :
            NH_ECMASCRIPT_END(Nh_ECMAScript_evaluateBooleanLiteral(Node_p->Children.pp[0]))
    }

NH_ECMASCRIPT_END(Nh_ECMAScript_normalEmptyCompletionOrReference())
}

static Nh_ECMAScript_CompletionOrReference Nh_ECMAScript_evaluatePrimaryExpression(
    Nh_ECMAScript_ParseNode *Node_p)
{
NH_ECMASCRIPT_BEGIN()

    switch (((Nh_ECMAScript_ParseNode*)Node_p->Children.pp[0])->type)
    {
        case NH_ECMASCRIPT_PARSE_NODE_IDENTIFIER_REFERENCE :
            NH_ECMASCRIPT_END(Nh_ECMAScript_evaluateIdentifierReference(Node_p->Children.pp[0]))
        case NH_ECMASCRIPT_PARSE_NODE_LITERAL :
            NH_ECMASCRIPT_END(Nh_ECMAScript_evaluateLiteral(Node_p->Children.pp[0]))
    }

NH_ECMASCRIPT_END(Nh_ECMAScript_normalEmptyCompletionOrReference())
}

static Nh_ECMAScript_CompletionOrReference Nh_ECMAScript_evaluateMemberExpression(
    Nh_ECMAScript_ParseNode *Node_p)
{
NH_ECMASCRIPT_BEGIN()

    if (((Nh_ECMAScript_ParseNode*)Node_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_PRIMARY_EXPRESSION) {
        NH_ECMASCRIPT_END(Nh_ECMAScript_evaluatePrimaryExpression(Node_p->Children.pp[0]))
    }

NH_ECMASCRIPT_END(Nh_ECMAScript_normalEmptyCompletionOrReference())
}

static Nh_ECMAScript_CompletionOrReference Nh_ECMAScript_evaluateNewExpression(
    Nh_ECMAScript_ParseNode *Node_p)
{
NH_ECMASCRIPT_BEGIN()

    if (((Nh_ECMAScript_ParseNode*)Node_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_MEMBER_EXPRESSION) {
        NH_ECMASCRIPT_END(Nh_ECMAScript_evaluateMemberExpression(Node_p->Children.pp[0]))
    }

NH_ECMASCRIPT_END(Nh_ECMAScript_normalEmptyCompletionOrReference())
}

static Nh_ECMAScript_CompletionOrReference Nh_ECMAScript_evaluateOptionalExpression(
    Nh_ECMAScript_ParseNode *Node_p)
{
NH_ECMASCRIPT_BEGIN()
NH_ECMASCRIPT_END(Nh_ECMAScript_normalEmptyCompletionOrReference())
}

// https://tc39.es/ecma262/#sec-function-calls
static Nh_ECMAScript_CompletionOrReference Nh_ECMAScript_evaluateCallExpression(
    Nh_ECMAScript_ParseNode *Node_p)
{
NH_ECMASCRIPT_BEGIN()

    if (Node_p->Children.size == 1)
    {
        if (((Nh_ECMAScript_ParseNode*)Node_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_COVER_CALL_EXPRESSION_AND_ASYNC_ARROW_HEAD) {
        }
    }

NH_ECMASCRIPT_END(Nh_ECMAScript_normalEmptyCompletionOrReference())
}

static Nh_ECMAScript_CompletionOrReference Nh_ECMAScript_evaluateLeftHandSideExpression(
    Nh_ECMAScript_ParseNode *Node_p)
{
NH_ECMASCRIPT_BEGIN()

    switch (((Nh_ECMAScript_ParseNode*)Node_p->Children.pp[0])->type)
    {
        case NH_ECMASCRIPT_PARSE_NODE_NEW_EXPRESSION :
            NH_ECMASCRIPT_END(Nh_ECMAScript_evaluateNewExpression(Node_p->Children.pp[0]))
            break;
        case NH_ECMASCRIPT_PARSE_NODE_CALL_EXPRESSION :
            NH_ECMASCRIPT_END(Nh_ECMAScript_evaluateCallExpression(Node_p->Children.pp[0]))
            break;
        case NH_ECMASCRIPT_PARSE_NODE_OPTIONAL_EXPRESSION :
            NH_ECMASCRIPT_END(Nh_ECMAScript_evaluateOptionalExpression(Node_p->Children.pp[0]))
            break;
    }

NH_ECMASCRIPT_END(Nh_ECMAScript_normalEmptyCompletionOrReference())
}

static Nh_ECMAScript_CompletionOrReference Nh_ECMAScript_evaluateUpdateExpression(
    Nh_ECMAScript_ParseNode *Node_p)
{
NH_ECMASCRIPT_BEGIN()

    if (Node_p->Children.size == 1) {
        NH_ECMASCRIPT_END(Nh_ECMAScript_evaluateLeftHandSideExpression(Node_p->Children.pp[0]))
    }

NH_ECMASCRIPT_END(Nh_ECMAScript_normalEmptyCompletionOrReference())
}

static Nh_ECMAScript_CompletionOrReference Nh_ECMAScript_evaluateUnaryExpression(
    Nh_ECMAScript_ParseNode *Node_p)
{
NH_ECMASCRIPT_BEGIN()

    if (((Nh_ECMAScript_ParseNode*)Node_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_UPDATE_EXPRESSION) {
        NH_ECMASCRIPT_END(Nh_ECMAScript_evaluateUpdateExpression(Node_p->Children.pp[0]))
    }

NH_ECMASCRIPT_END(Nh_ECMAScript_normalEmptyCompletionOrReference())
}

static Nh_ECMAScript_CompletionOrReference Nh_ECMAScript_evaluateExponentiationExpression(
    Nh_ECMAScript_ParseNode *Node_p)
{
NH_ECMASCRIPT_BEGIN()

    if (Node_p->Children.size == 1) {
        NH_ECMASCRIPT_END(Nh_ECMAScript_evaluateUnaryExpression(Node_p->Children.pp[0]))
    }

NH_ECMASCRIPT_END(Nh_ECMAScript_normalEmptyCompletionOrReference())
}

static Nh_ECMAScript_CompletionOrReference Nh_ECMAScript_evaluateMultiplicativeExpression(
    Nh_ECMAScript_ParseNode *Node_p)
{
NH_ECMASCRIPT_BEGIN()

    if (Node_p->Children.size == 1) {
        NH_ECMASCRIPT_END(Nh_ECMAScript_evaluateExponentiationExpression(Node_p->Children.pp[0]))
    }

NH_ECMASCRIPT_END(Nh_ECMAScript_normalEmptyCompletionOrReference())
}

static Nh_ECMAScript_CompletionOrReference Nh_ECMAScript_evaluateAdditiveExpression(
    Nh_ECMAScript_ParseNode *Node_p)
{
NH_ECMASCRIPT_BEGIN()

    if (Node_p->Children.size == 1) {
        NH_ECMASCRIPT_END(Nh_ECMAScript_evaluateMultiplicativeExpression(Node_p->Children.pp[0]))
    }

NH_ECMASCRIPT_END(Nh_ECMAScript_normalEmptyCompletionOrReference())
}

static Nh_ECMAScript_CompletionOrReference Nh_ECMAScript_evaluateShiftExpression(
    Nh_ECMAScript_ParseNode *Node_p)
{
NH_ECMASCRIPT_BEGIN()

    if (Node_p->Children.size == 1) {
        NH_ECMASCRIPT_END(Nh_ECMAScript_evaluateAdditiveExpression(Node_p->Children.pp[0]))
    }

NH_ECMASCRIPT_END(Nh_ECMAScript_normalEmptyCompletionOrReference())
}

static Nh_ECMAScript_CompletionOrReference Nh_ECMAScript_evaluateRelationalExpression(
    Nh_ECMAScript_ParseNode *Node_p)
{
NH_ECMASCRIPT_BEGIN()

    if (Node_p->Children.size == 1) {
        NH_ECMASCRIPT_END(Nh_ECMAScript_evaluateShiftExpression(Node_p->Children.pp[0]))
    }

NH_ECMASCRIPT_END(Nh_ECMAScript_normalEmptyCompletionOrReference())
}

static Nh_ECMAScript_CompletionOrReference Nh_ECMAScript_evaluateEqualityExpression(
    Nh_ECMAScript_ParseNode *Node_p)
{
NH_ECMASCRIPT_BEGIN()

    if (Node_p->Children.size == 1) {
        NH_ECMASCRIPT_END(Nh_ECMAScript_evaluateRelationalExpression(Node_p->Children.pp[0]))
    }

NH_ECMASCRIPT_END(Nh_ECMAScript_normalEmptyCompletionOrReference())
}

static Nh_ECMAScript_CompletionOrReference Nh_ECMAScript_evaluateBitwiseANDExpression(
    Nh_ECMAScript_ParseNode *Node_p)
{
NH_ECMASCRIPT_BEGIN()

    if (Node_p->Children.size == 1) {
        NH_ECMASCRIPT_END(Nh_ECMAScript_evaluateEqualityExpression(Node_p->Children.pp[0]))
    }

NH_ECMASCRIPT_END(Nh_ECMAScript_normalEmptyCompletionOrReference())
}

static Nh_ECMAScript_CompletionOrReference Nh_ECMAScript_evaluateBitwiseXORExpression(
    Nh_ECMAScript_ParseNode *Node_p)
{
NH_ECMASCRIPT_BEGIN()

    if (Node_p->Children.size == 1) {
        NH_ECMASCRIPT_END(Nh_ECMAScript_evaluateBitwiseANDExpression(Node_p->Children.pp[0]))
    }

NH_ECMASCRIPT_END(Nh_ECMAScript_normalEmptyCompletionOrReference())
}

static Nh_ECMAScript_CompletionOrReference Nh_ECMAScript_evaluateBitwiseORExpression(
    Nh_ECMAScript_ParseNode *Node_p)
{
NH_ECMASCRIPT_BEGIN()

    if (Node_p->Children.size == 1) {
        NH_ECMASCRIPT_END(Nh_ECMAScript_evaluateBitwiseXORExpression(Node_p->Children.pp[0]))
    }

NH_ECMASCRIPT_END(Nh_ECMAScript_normalEmptyCompletionOrReference())
}

static Nh_ECMAScript_CompletionOrReference Nh_ECMAScript_evaluateLogicalANDExpression(
    Nh_ECMAScript_ParseNode *Node_p)
{
NH_ECMASCRIPT_BEGIN()

    if (Node_p->Children.size == 1) {
        NH_ECMASCRIPT_END(Nh_ECMAScript_evaluateBitwiseORExpression(Node_p->Children.pp[0]))
    }

NH_ECMASCRIPT_END(Nh_ECMAScript_normalEmptyCompletionOrReference())
}

static Nh_ECMAScript_CompletionOrReference Nh_ECMAScript_evaluateLogicalORExpression(
    Nh_ECMAScript_ParseNode *Node_p)
{
NH_ECMASCRIPT_BEGIN()

    if (Node_p->Children.size == 1) {
        NH_ECMASCRIPT_END(Nh_ECMAScript_evaluateLogicalANDExpression(Node_p->Children.pp[0]))
    }

NH_ECMASCRIPT_END(Nh_ECMAScript_normalEmptyCompletionOrReference())
}

static Nh_ECMAScript_CompletionOrReference Nh_ECMAScript_evaluateShortCircuitExpression(
    Nh_ECMAScript_ParseNode *Node_p)
{
NH_ECMASCRIPT_BEGIN()

    switch (((Nh_ECMAScript_ParseNode*)Node_p->Children.pp[0])->type) 
    {
        case NH_ECMASCRIPT_PARSE_NODE_LOGICAL_OR_EXPRESSION :
            NH_ECMASCRIPT_END(Nh_ECMAScript_evaluateLogicalORExpression(Node_p->Children.pp[0]))
    }

NH_ECMASCRIPT_END(Nh_ECMAScript_normalEmptyCompletionOrReference())
}

static Nh_ECMAScript_CompletionOrReference Nh_ECMAScript_evaluateConditionalExpression(
    Nh_ECMAScript_ParseNode *Node_p)
{
NH_ECMASCRIPT_BEGIN()

    if (Node_p->Children.size == 1) {
        NH_ECMASCRIPT_END(Nh_ECMAScript_evaluateShortCircuitExpression(Node_p->Children.pp[0]))
    }

NH_ECMASCRIPT_END(Nh_ECMAScript_normalEmptyCompletionOrReference())
}

static Nh_ECMAScript_CompletionOrReference Nh_ECMAScript_evaluateAssignmentExpression(
    Nh_ECMAScript_ParseNode *Node_p)
{
NH_ECMASCRIPT_BEGIN()

    if (Node_p->Children.size == 1)
    {
        switch (((Nh_ECMAScript_ParseNode*)Node_p->Children.pp[0])->type)
        {
            case NH_ECMASCRIPT_PARSE_NODE_CONDITIONAL_EXPRESSION :
                NH_ECMASCRIPT_END(Nh_ECMAScript_evaluateConditionalExpression(Node_p->Children.pp[0]))
        }
    }
    else if (((Nh_ECMAScript_ParseNode*)Node_p->Children.pp[1])->type != NH_ECMASCRIPT_PARSE_NODE_ASSIGNMENT_OPERATOR) {

        if (!strcmp("=", ((Nh_ECMAScript_ParseNode*)Node_p->Children.pp[1])->Value_p->String.bytes_p)) 
        {
//            // TODO If LeftHandSideExpression is neither an ObjectLiteral nor an ArrayLiteral, then
//            Nh_ECMAScript_CompletionOrReference Left = Nh_ECMAScript_evaluateLeftHandSideExpression(Node_p->Children.pp[0]);
//            if (Left.Completion.type != NH_ECMASCRIPT_COMPLETION_NORMAL) {NH_ECMASCRIPT_END(Left)}
//            if (Nh_ECMAScript_isAnonymousFunctionDefinition(Node_p->Children.pp[2]) && Nh_ECMAScript_isIdentifierRef(Node_p->Children.pp[0])) {
//
//            }
        }
    }

NH_ECMASCRIPT_END(Nh_ECMAScript_normalEmptyCompletionOrReference())
}

static Nh_ECMAScript_CompletionOrReference Nh_ECMAScript_evaluateInitializer(
    Nh_ECMAScript_ParseNode *Node_p)
{
NH_ECMASCRIPT_BEGIN()
NH_ECMASCRIPT_END(Nh_ECMAScript_evaluateAssignmentExpression(Node_p->Children.pp[1]))
}

static Nh_ECMAScript_CompletionOrReference Nh_ECMAScript_evaluateVariableDeclaration(
    Nh_ECMAScript_ParseNode *Node_p)
{
NH_ECMASCRIPT_BEGIN()

    if (Node_p->Children.size == 2 && ((Nh_ECMAScript_ParseNode*)Node_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_BINDING_IDENTIFIER) 
    {
        Nh_UTF8String *BindingId_p = Nh_ECMAScript_getStringValue(Node_p->Children.pp[0]); 
        Nh_ECMAScript_Reference Reference = Nh_ECMAScript_resolveBinding(BindingId_p, NULL);

        Nh_ECMAScript_Completion InitializerGetValue = Nh_ECMAScript_normalEmptyCompletion();

        if (Nh_ECMAScript_isAnonymousFunctionDefinition(Node_p->Children.pp[1])) {
            // TODO
        }
        else {
            Nh_ECMAScript_CompletionOrReference InitializerEval = Nh_ECMAScript_evaluateInitializer(Node_p->Children.pp[1]);
            if (InitializerEval.Completion.type != NH_ECMASCRIPT_COMPLETION_NORMAL) {
                NH_ECMASCRIPT_END(InitializerEval)
            }

            InitializerGetValue = Nh_ECMAScript_getValue(InitializerEval.Completion, InitializerEval.Reference);

            if (InitializerGetValue.type != NH_ECMASCRIPT_COMPLETION_NORMAL) {
                NH_ECMASCRIPT_END(Nh_ECMAScript_wrapCompletion(InitializerGetValue))
            }
        }

        Nh_ECMAScript_putValue(Reference, InitializerGetValue.Value.Payload);
    }

NH_ECMASCRIPT_END(Nh_ECMAScript_normalEmptyCompletionOrReference())
}

static Nh_ECMAScript_CompletionOrReference Nh_ECMAScript_evaluateVariableDeclarationList(
    Nh_ECMAScript_ParseNode *Node_p)
{
NH_ECMASCRIPT_BEGIN()

    if (Node_p->Children.size == 3)
    {
        Nh_ECMAScript_CompletionOrReference Result = Nh_ECMAScript_evaluateVariableDeclarationList(Node_p->Children.pp[0]);
        if (Result.Completion.type != NH_ECMASCRIPT_COMPLETION_NORMAL) {NH_ECMASCRIPT_END(Result)}
        NH_ECMASCRIPT_END(Nh_ECMAScript_evaluateVariableDeclaration(Node_p->Children.pp[2]))
    }
    else {
        NH_ECMASCRIPT_END(Nh_ECMAScript_evaluateVariableDeclaration(Node_p->Children.pp[0]))
    }

NH_ECMASCRIPT_END(Nh_ECMAScript_normalEmptyCompletionOrReference())
}

static Nh_ECMAScript_CompletionOrReference Nh_ECMAScript_evaluateVariableStatement(
    Nh_ECMAScript_ParseNode *Node_p)
{
NH_ECMASCRIPT_BEGIN()

    Nh_ECMAScript_CompletionOrReference Result = Nh_ECMAScript_evaluateVariableDeclarationList(Node_p->Children.pp[1]);
    if (Result.Completion.type != NH_ECMASCRIPT_COMPLETION_NORMAL) {NH_ECMASCRIPT_END(Result)}

NH_ECMASCRIPT_END(Nh_ECMAScript_normalEmptyCompletionOrReference())
}

static Nh_ECMAScript_CompletionOrReference Nh_ECMAScript_evaluateExpression(
    Nh_ECMAScript_ParseNode *Node_p)
{
NH_ECMASCRIPT_BEGIN()

    if (Node_p->Children.size == 3) 
    {
        Nh_ECMAScript_CompletionOrReference Left = Nh_ECMAScript_evaluateExpression(Node_p->Children.pp[0]);
        if (Left.Completion.type != NH_ECMASCRIPT_COMPLETION_NORMAL) {NH_ECMASCRIPT_END(Left)}
        Nh_ECMAScript_CompletionOrReference Right = Nh_ECMAScript_evaluateAssignmentExpression(Node_p->Children.pp[2]);
        NH_ECMASCRIPT_END(Nh_ECMAScript_wrapCompletion(Nh_ECMAScript_getValue(Right.Completion, Right.Reference)))
    }

NH_ECMASCRIPT_END(Nh_ECMAScript_evaluateAssignmentExpression(Node_p->Children.pp[0]))
}

static Nh_ECMAScript_CompletionOrReference Nh_ECMAScript_evaluateExpressionStatement(
    Nh_ECMAScript_ParseNode *Node_p)
{
NH_ECMASCRIPT_BEGIN()

    Nh_ECMAScript_CompletionOrReference ExpressionResult = Nh_ECMAScript_evaluateExpression(Node_p->Children.pp[0]);

NH_ECMASCRIPT_END(Nh_ECMAScript_wrapCompletion(Nh_ECMAScript_getValue(ExpressionResult.Completion, ExpressionResult.Reference)))
}

static Nh_ECMAScript_CompletionOrReference Nh_ECMAScript_evaluateStatement(
    Nh_ECMAScript_ParseNode *Node_p)
{
NH_ECMASCRIPT_BEGIN()

    switch (((Nh_ECMAScript_ParseNode*)Node_p->Children.pp[0])->type)
    {
        case NH_ECMASCRIPT_PARSE_NODE_VARIABLE_STATEMENT : 
            NH_ECMASCRIPT_END(Nh_ECMAScript_evaluateVariableStatement(Node_p->Children.pp[0]))
        case NH_ECMASCRIPT_PARSE_NODE_EXPRESSION_STATEMENT : 
            NH_ECMASCRIPT_END(Nh_ECMAScript_evaluateExpressionStatement(Node_p->Children.pp[0]))
    }

NH_ECMASCRIPT_END(Nh_ECMAScript_normalEmptyCompletionOrReference())
}

static Nh_ECMAScript_CompletionOrReference Nh_ECMAScript_evaluateDeclaration(
    Nh_ECMAScript_ParseNode *Node_p)
{
NH_ECMASCRIPT_BEGIN()
NH_ECMASCRIPT_END(Nh_ECMAScript_normalEmptyCompletionOrReference())
}

static Nh_ECMAScript_CompletionOrReference Nh_ECMAScript_evaluateStatementListItem(
    Nh_ECMAScript_ParseNode *Node_p)
{
NH_ECMASCRIPT_BEGIN()

    switch (((Nh_ECMAScript_ParseNode*)Node_p->Children.pp[0])->type)
    {
        case NH_ECMASCRIPT_PARSE_NODE_STATEMENT : 
            NH_ECMASCRIPT_END(Nh_ECMAScript_evaluateStatement(Node_p->Children.pp[0]))
        case NH_ECMASCRIPT_PARSE_NODE_DECLARATION : 
            NH_ECMASCRIPT_END(Nh_ECMAScript_evaluateDeclaration(Node_p->Children.pp[0]))
    }

NH_ECMASCRIPT_END(Nh_ECMAScript_normalEmptyCompletionOrReference())
}

static Nh_ECMAScript_CompletionOrReference Nh_ECMAScript_evaluateStatementList(
    Nh_ECMAScript_ParseNode *Node_p)
{
NH_ECMASCRIPT_BEGIN()

    if (Node_p->Children.size == 2) 
    {
        Nh_ECMAScript_CompletionOrReference StatementList = Nh_ECMAScript_evaluateStatementList(Node_p->Children.pp[0]);
        if (StatementList.Completion.type != NH_ECMASCRIPT_COMPLETION_NORMAL) {NH_ECMASCRIPT_END(StatementList)}

        Nh_ECMAScript_CompletionOrReference Statement = Nh_ECMAScript_evaluateStatementListItem(Node_p->Children.pp[1]);
        if (!Statement.Completion.Value.empty) {NH_ECMASCRIPT_END(Statement)}
        else {
            Statement.Completion.Value = StatementList.Completion.Value;
            NH_ECMASCRIPT_END(Statement)
        }
    }
    else if (Node_p->Children.size == 1) {
        NH_ECMASCRIPT_END(Nh_ECMAScript_evaluateStatementListItem(Node_p->Children.pp[0]))
    }

NH_ECMASCRIPT_END(Nh_ECMAScript_normalEmptyCompletionOrReference())
}

Nh_ECMAScript_Completion Nh_ECMAScript_evaluateScriptBody(
    Nh_ECMAScript_ParseNode *Node_p)
{
NH_ECMASCRIPT_BEGIN()

    if (Node_p->Children.size > 0) {
        NH_ECMASCRIPT_END(Nh_ECMAScript_evaluateStatementList(Node_p->Children.pp[0]).Completion)
    }

NH_ECMASCRIPT_END(Nh_ECMAScript_normalEmptyCompletion())
}


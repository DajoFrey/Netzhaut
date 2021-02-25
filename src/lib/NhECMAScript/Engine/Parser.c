// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "Parser.h"
#include "Lexer.h"

#include "../Common/IndexMap.h"
#include "../Common/Macro.h"
#include NH_ECMASCRIPT_FLOW
#include NH_ECMASCRIPT_DEFAULT_CHECK

#include "../../NhCore/Unicode.h"
#include "../../NhCore/Memory.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

#ifdef __unix__
    #include <unistd.h>
    #include <sys/types.h>
    #include <sys/wait.h>
#endif

// DATA ============================================================================================

const NH_BYTE *NH_ECMASCRIPT_RESERVED_WORDS_PP[] = {
    "await",
    "break",
    "case",
    "catch",
    "class",
    "const",
    "continue",
    "debugger",
    "default",
    "delete", 
    "do",
    "else",
    "enum",
    "export",
    "extends", 
    "false",
    "finally", 
    "for",
    "function",
    "if",
    "import", 
    "in",
    "instanceof",
    "new",
    "null",
    "return",
    "super",
    "switch", 
    "this",
    "throw",
    "true",
    "try",
    "typeof", 
    "var",
    "void", 
    "while", 
    "with", 
    "yield",
};

const NH_BYTE *NH_ECMASCRIPT_PARSE_NODE_NAMES_PP[] = {
    "AdditiveExpression",
    "Arguments",
    "ArrayBindingPattern",
    "ArrowFunction",
    "AssignmentExpression",
    "AssignmentOperator",
    "AsyncArrowFunction",
    "AsyncConciseBody",
    "AsyncFunctionBody",
    "AsyncFunctionDeclaration",
    "AsyncFunctionExpression",
    "AsyncGeneratorDeclaration",
    "AsyncGeneratorExpression",
    "BindingElement",
    "BindingElementList",
    "BindingElisionElement",
    "BindingIdentifier",
    "BindingList",
    "BindingPattern",
    "BindingProperty",
    "BindingPropertyList",
    "BindingRestElement",
    "BindingRestProperty",
    "BitwiseANDExpression",
    "BitwiseORExpression",
    "BitwiseXORExpression",
    "Block",
    "BlockStatement",
    "BooleanLiteral",
    "BreakStatement",
    "CallExpression",
    "CaseBlock",
    "CaseClause",
    "CaseClauses",
    "Catch",
    "ClassDeclaration",
    "ClassExpression",
    "ConciseBody",
    "ConditionalExpression",
    "ContinueStatement",
    "CoverCallExpressionAndAsyncArrowHead",
    "CoverParenthesizedExpressionAndArrowParameterList",
    "DebuggerStatement",
    "Declaration",
    "DefaultClause",
    "DoWhileStatement",
    "Elision",
    "EmptyStatement",
    "EqualityExpression",
    "ExponentiationExpression",
    "ExportDeclaration",
    "ExportFromClause",
    "Expression",
    "ExpressionStatement",
    "ForBinding",
    "ForDeclaration",
    "ForInOfStatement",
    "ForStatement",
    "FormalParameter",
    "FormalParameterList",
    "FormalParameters",
    "FunctionBody",
    "FunctionDeclaration",
    "FunctionExpression",
    "FunctionStatementList",
    "GeneratorDeclaration",
    "GeneratorExpression",
    "HoistableDeclaration",
    "Identifier",
    "IdentifierReference",
    "IfStatement",
    "ImportDeclaration",
    "Initializer",
    "LabelIdentifier",
    "LabelledItem",
    "LabelledStatement",
    "LeftHandSideExpression",
    "LetOrConst",
    "LexicalBinding",
    "LexicalDeclaration",
    "Literal",
    "LogicalANDExpression",
    "LogicalORExpression",
    "MemberExpression",
    "Module",
    "ModuleBody",
    "ModuleItem",
    "ModuleItemList",
    "MultiplicativeExpression",
    "NamedExports",
    "NewExpression",
    "NullLiteral",
    "NumericLiteral",
    "ObjectBindingPattern",
    "OptionalChain",
    "OptionalExpression",
    "PrimaryExpression",
    "RelationalExpression",
    "ReturnStatement",
    "Script",
    "ScriptBody",
    "ShiftExpression",
    "ShortCircuitExpression",
    "SingleNameBinding",
    "Statement",
    "StatementList",
    "StatementListItem",
    "StringLiteral",
    "SwitchStatement",
    "ThrowStatement",
    "TryStatement",
    "UnaryExpression",
    "UpdateExpression",
    "VariableDeclaration",
    "VariableDeclarationList",
    "VariableStatement",
    "WhileStatement",
    "WithStatement",
};

size_t NH_ECMASCRIPT_RESERVED_WORDS_PP_COUNT = sizeof(NH_ECMASCRIPT_RESERVED_WORDS_PP) / sizeof(NH_ECMASCRIPT_RESERVED_WORDS_PP[0]);
size_t NH_ECMASCRIPT_PARSE_NODE_NAMES_PP_COUNT = sizeof(NH_ECMASCRIPT_PARSE_NODE_NAMES_PP) / sizeof(NH_ECMASCRIPT_PARSE_NODE_NAMES_PP[0]);

// DECLARE =========================================================================================

typedef struct Nh_ECMAScript_Parser {
    Nh_ECMAScript_InputElement *Tokens_p;
    int unparsed;
} Nh_ECMAScript_Parser;

static Nh_ECMAScript_ParseResult Nh_ECMAScript_parseStatementList(
    Nh_ECMAScript_Parser *Parser_p
);

static Nh_ECMAScript_ParseResult Nh_ECMAScript_parseIdentifier(
    Nh_ECMAScript_Parser *Parser_p
);

// HELPER ==========================================================================================

static Nh_ECMAScript_ParseResult Nh_ECMAScript_initParseResult(
    Nh_ECMAScript_ParseNode *Node_p)
{
NH_ECMASCRIPT_BEGIN()

    Nh_ECMAScript_ParseResult Result;
    Result.Node_p = Node_p;
    Result.SyntaxErrors = Nh_initList(8);

NH_ECMASCRIPT_END(Result)
}

static Nh_ECMAScript_ParseNode *Nh_ECMAScript_allocateNonTerminalParseNode(
    const NH_BYTE *name_p, int chunkSize)
{
NH_ECMASCRIPT_BEGIN()

    unsigned int *index_p = Nh_getFromHashMap(&NH_ECMASCRIPT_INDEXMAP.ParseNodeNames, (NH_BYTE*)name_p);
    if (index_p == NULL) {NH_ECMASCRIPT_END(NULL)}

    Nh_ECMAScript_ParseNode *Node_p = Nh_allocate(sizeof(Nh_ECMAScript_ParseNode));
    Node_p->Children = Nh_initList(chunkSize);
    Node_p->Value_p  = NULL;
    Node_p->type     = *index_p;

NH_ECMASCRIPT_END(Node_p)
}

static Nh_ECMAScript_ParseNode *Nh_ECMAScript_allocateTerminalParseNode(
    Nh_ECMAScript_InputElement *InputElement_p)
{
NH_ECMASCRIPT_BEGIN()

    Nh_ECMAScript_ParseNode *Node_p = Nh_allocate(sizeof(Nh_ECMAScript_ParseNode));
    Node_p->Children = Nh_initList(1);
    Node_p->Value_p  = InputElement_p;
    Node_p->type     = -1;

NH_ECMASCRIPT_END(Node_p)
}

static void Nh_ECMAScript_freeParseNode(
    Nh_ECMAScript_ParseNode *Node_p)
{
NH_ECMASCRIPT_BEGIN()
NH_ECMASCRIPT_SILENT_END()
}

static Nh_ECMAScript_Parser Nh_ECMAScript_advanceParser(
    Nh_ECMAScript_Parser Parser, int steps)
{
NH_ECMASCRIPT_BEGIN()

    Parser.unparsed -= steps;
    Parser.Tokens_p  = Parser.Tokens_p + steps;
 
NH_ECMASCRIPT_END(Parser)
}

static Nh_ECMAScript_ParseResult Nh_ECMAScript_parseEasyRecursiveProductionStep(
    Nh_ECMAScript_Parser *Parser_p, Nh_ECMAScript_ParseNode *Previous_p, NH_BYTE *name_p, NH_BOOL commaSeparated, 
    Nh_ECMAScript_ParseResult (*parseItem_f)(Nh_ECMAScript_Parser *Parser_p)) 
{
NH_ECMASCRIPT_BEGIN()

    if (commaSeparated && Previous_p != NULL) {
        if (Parser_p->unparsed > 0 && Parser_p->Tokens_p[0].String.bytes_p[0] != ',') {
            NH_ECMASCRIPT_END(Nh_ECMAScript_initParseResult(Previous_p))
        }
        *Parser_p = Nh_ECMAScript_advanceParser(*Parser_p, 1);
    }

    Nh_ECMAScript_ParseResult Result = parseItem_f(Parser_p);
    if (Result.Node_p == NULL || Result.SyntaxErrors.size > 0) {
        NH_ECMASCRIPT_END(Nh_ECMAScript_initParseResult(Previous_p))
    }

    Nh_ECMAScript_ParseNode *NewNonTerminal_p = Nh_ECMAScript_allocateNonTerminalParseNode(name_p, 2);

    if (Previous_p == NULL) {
        NewNonTerminal_p->Children = Nh_initList(1);
        Nh_appendToList(&NewNonTerminal_p->Children, Result.Node_p);
    }
    else {
        Nh_appendToList(&NewNonTerminal_p->Children, Previous_p);
        Nh_appendToList(&NewNonTerminal_p->Children, Result.Node_p);
    }

NH_ECMASCRIPT_END(Nh_ECMAScript_parseEasyRecursiveProductionStep(Parser_p, NewNonTerminal_p, name_p, commaSeparated, parseItem_f))
}

static Nh_ECMAScript_ParseResult Nh_ECMAScript_parseEasyRecursiveProduction(
    Nh_ECMAScript_Parser *Parser_p, NH_BYTE *name_p, NH_BOOL commaSeparated, Nh_ECMAScript_ParseResult (*parseItem_f)(Nh_ECMAScript_Parser *Parser_p))
{
NH_ECMASCRIPT_BEGIN()

    Nh_ECMAScript_Parser LocalParser = *Parser_p;

    Nh_ECMAScript_ParseResult Result = Nh_ECMAScript_parseEasyRecursiveProductionStep(
        &LocalParser, NULL, name_p, commaSeparated, parseItem_f 
    );

    if (Result.Node_p != NULL && Result.SyntaxErrors.size <= 0) {
        *Parser_p = LocalParser;
    }

NH_ECMASCRIPT_END(Result)
}

// PARSER ==========================================================================================

static Nh_ECMAScript_ParseResult Nh_ECMAScript_parseBlock(
    Nh_ECMAScript_Parser *Parser_p)
{
NH_ECMASCRIPT_BEGIN()

    if (Parser_p->unparsed <= 0 || Parser_p->Tokens_p[0].String.bytes_p[0] != '{') {
        NH_ECMASCRIPT_END(Nh_ECMAScript_initParseResult(NULL))
    }

    Nh_ECMAScript_Parser LocalParser = Nh_ECMAScript_advanceParser(*Parser_p, 1);
    Nh_ECMAScript_ParseResult Result = Nh_ECMAScript_parseStatementList(&LocalParser);

    if (Result.Node_p != NULL)
    {
        if (LocalParser.unparsed <= 0 || LocalParser.Tokens_p[0].String.bytes_p[0] != '}') {
            Nh_ECMAScript_freeParseNode(Result.Node_p);
            NH_ECMASCRIPT_END(Nh_ECMAScript_initParseResult(NULL))
        }

        Nh_ECMAScript_ParseNode *Block_p = Nh_ECMAScript_allocateNonTerminalParseNode("Block", 3);
        Nh_ECMAScript_ParseNode *OpenBracket_p = Nh_ECMAScript_allocateTerminalParseNode(&Parser_p->Tokens_p[0]);
        Nh_ECMAScript_ParseNode *CloseBracket_p = Nh_ECMAScript_allocateTerminalParseNode(&LocalParser.Tokens_p[0]);

        Nh_appendToList(&Block_p->Children, OpenBracket_p);
        Nh_appendToList(&Block_p->Children, Result.Node_p);
        Nh_appendToList(&Block_p->Children, CloseBracket_p);

        *Parser_p = Nh_ECMAScript_advanceParser(LocalParser, 1);
        NH_ECMASCRIPT_END(Nh_ECMAScript_initParseResult(Block_p))
    }

    // empty block
    else if (LocalParser.unparsed > 0 && *LocalParser.Tokens_p[0].String.bytes_p == '}')
    {
        Nh_ECMAScript_ParseNode *Block_p = Nh_ECMAScript_allocateNonTerminalParseNode("Block", 2);
        Nh_ECMAScript_ParseNode *OpenBracket_p = Nh_ECMAScript_allocateTerminalParseNode(&Parser_p->Tokens_p[0]);
        Nh_ECMAScript_ParseNode *CloseBracket_p = Nh_ECMAScript_allocateTerminalParseNode(&LocalParser.Tokens_p[0]);

        Nh_appendToList(&Block_p->Children, OpenBracket_p);
        Nh_appendToList(&Block_p->Children, CloseBracket_p);

        *Parser_p = Nh_ECMAScript_advanceParser(LocalParser, 1);
        NH_ECMASCRIPT_END(Nh_ECMAScript_initParseResult(Block_p))
    }

NH_ECMASCRIPT_END(Nh_ECMAScript_initParseResult(NULL))
}

static Nh_ECMAScript_ParseResult Nh_ECMAScript_parseBlockStatement(
    Nh_ECMAScript_Parser *Parser_p)
{
NH_ECMASCRIPT_BEGIN()

    Nh_ECMAScript_ParseResult Result = Nh_ECMAScript_parseBlock(Parser_p);

    if (Result.Node_p != NULL) {
        Nh_ECMAScript_ParseNode *BlockStatement_p = Nh_ECMAScript_allocateNonTerminalParseNode("BlockStatement", 1);
        Nh_appendToList(&BlockStatement_p->Children, Result.Node_p);
        NH_ECMASCRIPT_END(Nh_ECMAScript_initParseResult(BlockStatement_p))
    }

NH_ECMASCRIPT_END(Result)
}

static Nh_ECMAScript_ParseResult Nh_ECMAScript_parseIdentifierReference(
    Nh_ECMAScript_Parser *Parser_p)
{
NH_ECMASCRIPT_BEGIN()

    Nh_ECMAScript_ParseResult Result = Nh_ECMAScript_initParseResult(NULL);

    if (Parser_p->unparsed <= 0) {NH_ECMASCRIPT_END(Result)}

    if (!strcmp("yield", Parser_p->Tokens_p[0].String.bytes_p) || !strcmp("await", Parser_p->Tokens_p[0].String.bytes_p)) 
    {
        Nh_ECMAScript_ParseNode *IdentifierReference_p = Nh_ECMAScript_allocateNonTerminalParseNode("IdentifierReference", 1);
        Nh_ECMAScript_ParseNode *YieldOrAwait_p = Nh_ECMAScript_allocateTerminalParseNode(&Parser_p->Tokens_p[0]);

        Nh_appendToList(&IdentifierReference_p->Children, YieldOrAwait_p);

        *Parser_p = Nh_ECMAScript_advanceParser(*Parser_p, 1);
        NH_ECMASCRIPT_END(Nh_ECMAScript_initParseResult(IdentifierReference_p))
    }
    else {

        Result = Nh_ECMAScript_parseIdentifier(Parser_p);

        if (Result.Node_p != NULL && Result.SyntaxErrors.size == 0) {
            Nh_ECMAScript_ParseNode *IdentifierReference_p = Nh_ECMAScript_allocateNonTerminalParseNode("IdentifierReference", 1);
            Nh_appendToList(&IdentifierReference_p->Children, Result.Node_p);
            NH_ECMASCRIPT_END(Nh_ECMAScript_initParseResult(IdentifierReference_p))
        }
    }

NH_ECMASCRIPT_END(Result)
}

static Nh_ECMAScript_ParseResult Nh_ECMAScript_parseNumericLiteral(
    Nh_ECMAScript_Parser *Parser_p)
{
NH_ECMASCRIPT_BEGIN()

    if (Parser_p->unparsed > 0 && Nh_ECMAScript_isNumericToken(&Parser_p->Tokens_p[0])) 
    {
        Nh_ECMAScript_ParseNode *NumericLiteral_p = Nh_ECMAScript_allocateNonTerminalParseNode("NumericLiteral", 1);
        Nh_ECMAScript_ParseNode *Number_p = Nh_ECMAScript_allocateTerminalParseNode(&Parser_p->Tokens_p[0]);

        Nh_appendToList(&NumericLiteral_p->Children, Number_p);

        *Parser_p = Nh_ECMAScript_advanceParser(*Parser_p, 1);
        NH_ECMASCRIPT_END(Nh_ECMAScript_initParseResult(NumericLiteral_p))
    }

NH_ECMASCRIPT_END(Nh_ECMAScript_initParseResult(NULL))
}

static Nh_ECMAScript_ParseResult Nh_ECMAScript_parseBooleanLiteral(
    Nh_ECMAScript_Parser *Parser_p)
{
NH_ECMASCRIPT_BEGIN()

    if (Parser_p->unparsed > 0 && (!strcmp("true", Parser_p->Tokens_p[0].String.bytes_p) || !strcmp("false", Parser_p->Tokens_p[0].String.bytes_p))) 
    {
        Nh_ECMAScript_ParseNode *BooleanLiteral_p = Nh_ECMAScript_allocateNonTerminalParseNode("BooleanLiteral", 1);
        Nh_ECMAScript_ParseNode *TrueOrFalse_p = Nh_ECMAScript_allocateTerminalParseNode(&Parser_p->Tokens_p[0]);

        Nh_appendToList(&BooleanLiteral_p->Children, TrueOrFalse_p);

        *Parser_p = Nh_ECMAScript_advanceParser(*Parser_p, 1);
        NH_ECMASCRIPT_END(Nh_ECMAScript_initParseResult(BooleanLiteral_p))
    }

NH_ECMASCRIPT_END(Nh_ECMAScript_initParseResult(NULL))
}

static Nh_ECMAScript_ParseResult Nh_ECMAScript_parseNullLiteral(
    Nh_ECMAScript_Parser *Parser_p)
{
NH_ECMASCRIPT_BEGIN()

    if (Parser_p->unparsed > 0 && !strcmp("null", Parser_p->Tokens_p[0].String.bytes_p)) 
    {
        Nh_ECMAScript_ParseNode *NullLiteral_p = Nh_ECMAScript_allocateNonTerminalParseNode("NullLiteral", 1);
        Nh_ECMAScript_ParseNode *Null_p = Nh_ECMAScript_allocateTerminalParseNode(&Parser_p->Tokens_p[0]);

        Nh_appendToList(&NullLiteral_p->Children, Null_p);

        *Parser_p = Nh_ECMAScript_advanceParser(*Parser_p, 1);
        NH_ECMASCRIPT_END(Nh_ECMAScript_initParseResult(NullLiteral_p))
    }

NH_ECMASCRIPT_END(Nh_ECMAScript_initParseResult(NULL))
}

static Nh_ECMAScript_ParseResult Nh_ECMAScript_parseStringLiteral(
    Nh_ECMAScript_Parser *Parser_p)
{
NH_ECMASCRIPT_BEGIN()

    if (Parser_p->unparsed > 0 && Parser_p->Tokens_p[0].type == NH_ECMASCRIPT_INPUT_ELEMENT_TOKEN_STRING_LITERAL) 
    {
        Nh_ECMAScript_ParseNode *StringLiteral_p = Nh_ECMAScript_allocateNonTerminalParseNode("StringLiteral", 1);
        Nh_ECMAScript_ParseNode *String_p = Nh_ECMAScript_allocateTerminalParseNode(&Parser_p->Tokens_p[0]);

        Nh_appendToList(&StringLiteral_p->Children, String_p);

        *Parser_p = Nh_ECMAScript_advanceParser(*Parser_p, 1);
        NH_ECMASCRIPT_END(Nh_ECMAScript_initParseResult(StringLiteral_p))
    }

NH_ECMASCRIPT_END(Nh_ECMAScript_initParseResult(NULL))
}

static Nh_ECMAScript_ParseResult Nh_ECMAScript_parseLiteral(
    Nh_ECMAScript_Parser *Parser_p)
{
NH_ECMASCRIPT_BEGIN()

    Nh_ECMAScript_ParseResult Result = Nh_ECMAScript_parseNullLiteral(Parser_p);

    if (Result.Node_p != NULL) {
        Nh_ECMAScript_ParseNode *Literal_p = Nh_ECMAScript_allocateNonTerminalParseNode("Literal", 1);
        Nh_appendToList(&Literal_p->Children, Result.Node_p);
        NH_ECMASCRIPT_END(Nh_ECMAScript_initParseResult(Literal_p))
    }

    Result = Nh_ECMAScript_parseBooleanLiteral(Parser_p);

    if (Result.Node_p != NULL) {
        Nh_ECMAScript_ParseNode *Literal_p = Nh_ECMAScript_allocateNonTerminalParseNode("Literal", 1);
        Nh_appendToList(&Literal_p->Children, Result.Node_p);
        NH_ECMASCRIPT_END(Nh_ECMAScript_initParseResult(Literal_p))
    }

    Result = Nh_ECMAScript_parseNumericLiteral(Parser_p);

    if (Result.Node_p != NULL) {
        Nh_ECMAScript_ParseNode *Literal_p = Nh_ECMAScript_allocateNonTerminalParseNode("Literal", 1);
        Nh_appendToList(&Literal_p->Children, Result.Node_p);
        NH_ECMASCRIPT_END(Nh_ECMAScript_initParseResult(Literal_p))
    }

    Result = Nh_ECMAScript_parseStringLiteral(Parser_p);

    if (Result.Node_p != NULL) {
        Nh_ECMAScript_ParseNode *Literal_p = Nh_ECMAScript_allocateNonTerminalParseNode("Literal", 1);
        Nh_appendToList(&Literal_p->Children, Result.Node_p);
        NH_ECMASCRIPT_END(Nh_ECMAScript_initParseResult(Literal_p))
    }

NH_ECMASCRIPT_END(Nh_ECMAScript_initParseResult(NULL))
}

static Nh_ECMAScript_ParseResult Nh_ECMAScript_parsePrimaryExpression(
    Nh_ECMAScript_Parser *Parser_p)
{
NH_ECMASCRIPT_BEGIN()

    Nh_ECMAScript_ParseResult Result = Nh_ECMAScript_initParseResult(NULL);

    if (!strcmp("this", Parser_p->Tokens_p[0].String.bytes_p)) 
    {
        Nh_ECMAScript_ParseNode *PrimaryExpression_p = Nh_ECMAScript_allocateNonTerminalParseNode("PrimaryExpression", 1);
        Nh_ECMAScript_ParseNode *This_p = Nh_ECMAScript_allocateTerminalParseNode(&Parser_p->Tokens_p[0]);

        Nh_appendToList(&PrimaryExpression_p->Children, This_p);

        *Parser_p = Nh_ECMAScript_advanceParser(*Parser_p, 1);
        NH_ECMASCRIPT_END(Nh_ECMAScript_initParseResult(PrimaryExpression_p))
    }

    Result = Nh_ECMAScript_parseIdentifierReference(Parser_p);

    if (Result.Node_p != NULL && Result.SyntaxErrors.size == 0) {
        Nh_ECMAScript_ParseNode *PrimaryExpression_p = Nh_ECMAScript_allocateNonTerminalParseNode("PrimaryExpression", 1);
        Nh_appendToList(&PrimaryExpression_p->Children, Result.Node_p);
        NH_ECMASCRIPT_END(Nh_ECMAScript_initParseResult(PrimaryExpression_p))
    }

    Result = Nh_ECMAScript_parseLiteral(Parser_p);

    if (Result.Node_p != NULL && Result.SyntaxErrors.size == 0) {
        Nh_ECMAScript_ParseNode *PrimaryExpression_p = Nh_ECMAScript_allocateNonTerminalParseNode("PrimaryExpression", 1);
        Nh_appendToList(&PrimaryExpression_p->Children, Result.Node_p);
        NH_ECMASCRIPT_END(Nh_ECMAScript_initParseResult(PrimaryExpression_p))
    }

NH_ECMASCRIPT_END(Result)
}

static Nh_ECMAScript_ParseResult Nh_ECMAScript_parseMemberExpressionRecursively(
    Nh_ECMAScript_Parser *Parser_p, Nh_ECMAScript_ParseNode *MemberExpression_p)
{
NH_ECMASCRIPT_BEGIN()

    if (MemberExpression_p != NULL)
    {
        if (Parser_p->unparsed > 0 && Parser_p->Tokens_p[0].String.bytes_p[0] == '.') 
        {
            Nh_ECMAScript_Parser NextParser = Nh_ECMAScript_advanceParser(*Parser_p, 1);

            if (NextParser.unparsed > 0 && NextParser.Tokens_p[0].type == NH_ECMASCRIPT_INPUT_ELEMENT_TOKEN_IDENTIFIER_NAME)
            {
                Nh_ECMAScript_ParseNode *NewMemberExpression_p = Nh_ECMAScript_allocateNonTerminalParseNode("MemberExpression", 1);
                Nh_ECMAScript_ParseNode *FullStop_p = Nh_ECMAScript_allocateTerminalParseNode(&Parser_p->Tokens_p[0]);
                Nh_ECMAScript_ParseNode *IdentifierName_p = Nh_ECMAScript_allocateTerminalParseNode(&NextParser.Tokens_p[0]);

                Nh_appendToList(&NewMemberExpression_p->Children, MemberExpression_p);
                Nh_appendToList(&NewMemberExpression_p->Children, FullStop_p);
                Nh_appendToList(&NewMemberExpression_p->Children, IdentifierName_p);

                *Parser_p = Nh_ECMAScript_advanceParser(NextParser, 1);
                NH_ECMASCRIPT_END(Nh_ECMAScript_parseMemberExpressionRecursively(Parser_p, NewMemberExpression_p))
            }  
        }
    }
    else {

        Nh_ECMAScript_ParseResult Result = Nh_ECMAScript_parsePrimaryExpression(Parser_p);
    
        if (Result.Node_p != NULL && Result.SyntaxErrors.size == 0) {
            Nh_ECMAScript_ParseNode *MemberExpression_p = Nh_ECMAScript_allocateNonTerminalParseNode("MemberExpression", 1);
            Nh_appendToList(&MemberExpression_p->Children, Result.Node_p);
            NH_ECMASCRIPT_END(Nh_ECMAScript_parseMemberExpressionRecursively(Parser_p, MemberExpression_p))
        }
    }

NH_ECMASCRIPT_END(Nh_ECMAScript_initParseResult(MemberExpression_p))
}

static Nh_ECMAScript_ParseResult Nh_ECMAScript_parseMemberExpression(
    Nh_ECMAScript_Parser *Parser_p)
{
NH_ECMASCRIPT_BEGIN()

    Nh_ECMAScript_Parser LocalParser = *Parser_p;
    Nh_ECMAScript_ParseResult Result = Nh_ECMAScript_parseMemberExpressionRecursively(&LocalParser, NULL);
    
    if (Result.Node_p != NULL && Result.SyntaxErrors.size == 0) {
        *Parser_p = LocalParser;
    }

NH_ECMASCRIPT_END(Result)
}

static Nh_ECMAScript_ParseResult Nh_ECMAScript_parseOptionalChain(
    Nh_ECMAScript_Parser *Parser_p)
{
NH_ECMASCRIPT_BEGIN()
NH_ECMASCRIPT_END(Nh_ECMAScript_initParseResult(NULL))
}

static Nh_ECMAScript_ParseResult Nh_ECMAScript_parseOptionalExpression(
    Nh_ECMAScript_Parser *Parser_p)
{
NH_ECMASCRIPT_BEGIN()

    Nh_ECMAScript_Parser LocalParser = *Parser_p;
    Nh_ECMAScript_ParseResult Result = Nh_ECMAScript_parseMemberExpression(&LocalParser);

    if (Result.Node_p != NULL && Result.SyntaxErrors.size == 0) 
    {
        Nh_ECMAScript_ParseResult OptionalChainResult = Nh_ECMAScript_parseOptionalChain(&LocalParser);

        if (OptionalChainResult.Node_p != NULL && OptionalChainResult.SyntaxErrors.size == 0) 
        {
            Nh_ECMAScript_ParseNode *OptionalExpression_p = Nh_ECMAScript_allocateNonTerminalParseNode("OptionalExpression", 2);
            Nh_appendToList(&OptionalExpression_p->Children, Result.Node_p);
            Nh_appendToList(&OptionalExpression_p->Children, OptionalChainResult.Node_p);
            *Parser_p = LocalParser;
            NH_ECMASCRIPT_END(Nh_ECMAScript_initParseResult(OptionalExpression_p))
        }
    }

NH_ECMASCRIPT_END(Nh_ECMAScript_initParseResult(NULL))
}

static Nh_ECMAScript_ParseResult Nh_ECMAScript_parseNewExpression(
    Nh_ECMAScript_Parser *Parser_p)
{
NH_ECMASCRIPT_BEGIN()

    Nh_ECMAScript_ParseResult Result = Nh_ECMAScript_parseMemberExpression(Parser_p);

    if (Result.Node_p != NULL && Result.SyntaxErrors.size == 0) {
        Nh_ECMAScript_ParseNode *NewExpression_p = Nh_ECMAScript_allocateNonTerminalParseNode("NewExpression", 1);
        Nh_appendToList(&NewExpression_p->Children, Result.Node_p);
        NH_ECMASCRIPT_END(Nh_ECMAScript_initParseResult(NewExpression_p))
    }

NH_ECMASCRIPT_END(Nh_ECMAScript_initParseResult(NULL))
}

static Nh_ECMAScript_ParseResult Nh_ECMAScript_parseArguments(
    Nh_ECMAScript_Parser *Parser_p)
{
NH_ECMASCRIPT_BEGIN()

    if (Parser_p->unparsed <= 0 || Parser_p->Tokens_p[0].String.bytes_p[0] != '(') {
        NH_ECMASCRIPT_END(Nh_ECMAScript_initParseResult(NULL))
    }

    Nh_ECMAScript_Parser LocalParser = Nh_ECMAScript_advanceParser(*Parser_p, 1);

    if (LocalParser.unparsed > 0 && LocalParser.Tokens_p[0].String.bytes_p[0] == ')') 
    {
        Nh_ECMAScript_ParseNode *Arguments_p    = Nh_ECMAScript_allocateNonTerminalParseNode("Arguments", 1);
        Nh_ECMAScript_ParseNode *LeftBracket_p  = Nh_ECMAScript_allocateTerminalParseNode(&Parser_p->Tokens_p[0]);
        Nh_ECMAScript_ParseNode *RightBracket_p = Nh_ECMAScript_allocateTerminalParseNode(&LocalParser.Tokens_p[0]);

        Nh_appendToList(&Arguments_p->Children, LeftBracket_p);
        Nh_appendToList(&Arguments_p->Children, RightBracket_p);

        *Parser_p = Nh_ECMAScript_advanceParser(LocalParser, 1);
        NH_ECMASCRIPT_END(Nh_ECMAScript_initParseResult(Arguments_p))
    }

NH_ECMASCRIPT_END(Nh_ECMAScript_initParseResult(NULL))
}

static Nh_ECMAScript_ParseResult Nh_ECMAScript_parseCoverCallExpressionAndAsyncArrowHead(
    Nh_ECMAScript_Parser *Parser_p)
{
NH_ECMASCRIPT_BEGIN()

    Nh_ECMAScript_ParseResult MemberExpression = Nh_ECMAScript_parseMemberExpression(Parser_p);

    if (MemberExpression.Node_p != NULL && MemberExpression.SyntaxErrors.size == 0) 
    {
        Nh_ECMAScript_ParseResult Arguments = Nh_ECMAScript_parseArguments(Parser_p);

        if (Arguments.Node_p != NULL && Arguments.SyntaxErrors.size == 0) 
        {
            Nh_ECMAScript_ParseNode *CoverCallExpressionAndAsyncArrowHead_p = Nh_ECMAScript_allocateNonTerminalParseNode("CoverCallExpressionAndAsyncArrowHead", 2);
            Nh_appendToList(&CoverCallExpressionAndAsyncArrowHead_p->Children, MemberExpression.Node_p);
            Nh_appendToList(&CoverCallExpressionAndAsyncArrowHead_p->Children, Arguments.Node_p);
            NH_ECMASCRIPT_END(Nh_ECMAScript_initParseResult(CoverCallExpressionAndAsyncArrowHead_p))
        }

        Nh_ECMAScript_freeParseNode(MemberExpression.Node_p);
    }

NH_ECMASCRIPT_END(Nh_ECMAScript_initParseResult(NULL))
}

static Nh_ECMAScript_ParseResult Nh_ECMAScript_parseCallExpressionRecursively(
    Nh_ECMAScript_Parser *Parser_p, Nh_ECMAScript_ParseNode *PreviousCallExpression_p)
{
NH_ECMASCRIPT_BEGIN()

    if (PreviousCallExpression_p != NULL)
    {
        Nh_ECMAScript_ParseResult Arguments = Nh_ECMAScript_parseArguments(Parser_p);
    
        if (Arguments.Node_p != NULL && Arguments.SyntaxErrors.size == 0) {
            Nh_ECMAScript_ParseNode *CallExpression_p = Nh_ECMAScript_allocateNonTerminalParseNode("CallExpression", 2);
            Nh_appendToList(&CallExpression_p->Children, PreviousCallExpression_p);
            Nh_appendToList(&CallExpression_p->Children, Arguments.Node_p);
            NH_ECMASCRIPT_END(Nh_ECMAScript_parseCallExpressionRecursively(Parser_p, CallExpression_p))
        }

        if (Parser_p->unparsed > 0 && Parser_p->Tokens_p[0].String.bytes_p[0] == '.') {
            Nh_ECMAScript_Parser NextParser = Nh_ECMAScript_advanceParser(*Parser_p, 1);
            if (NextParser.unparsed > 0 && NextParser.Tokens_p[0].type == NH_ECMASCRIPT_INPUT_ELEMENT_TOKEN_IDENTIFIER_NAME)
            {
                Nh_ECMAScript_ParseNode *CallExpression_p = Nh_ECMAScript_allocateNonTerminalParseNode("CallExpression", 1);
                Nh_ECMAScript_ParseNode *FullStop_p = Nh_ECMAScript_allocateTerminalParseNode(&Parser_p->Tokens_p[0]);
                Nh_ECMAScript_ParseNode *IdentifierName_p = Nh_ECMAScript_allocateTerminalParseNode(&NextParser.Tokens_p[0]);

                Nh_appendToList(&CallExpression_p->Children, PreviousCallExpression_p);
                Nh_appendToList(&CallExpression_p->Children, FullStop_p);
                Nh_appendToList(&CallExpression_p->Children, IdentifierName_p);

                *Parser_p = Nh_ECMAScript_advanceParser(NextParser, 1);
                NH_ECMASCRIPT_END(Nh_ECMAScript_parseCallExpressionRecursively(Parser_p, CallExpression_p))
            }  
        }
    }
    else {

        Nh_ECMAScript_ParseResult CoverCallExpressionAndAsyncArrowHead = Nh_ECMAScript_parseCoverCallExpressionAndAsyncArrowHead(Parser_p);
    
        if (CoverCallExpressionAndAsyncArrowHead.Node_p != NULL && CoverCallExpressionAndAsyncArrowHead.SyntaxErrors.size == 0) {
            Nh_ECMAScript_ParseNode *CallExpression_p = Nh_ECMAScript_allocateNonTerminalParseNode("CallExpression", 1);
            Nh_appendToList(&CallExpression_p->Children, CoverCallExpressionAndAsyncArrowHead.Node_p);
            NH_ECMASCRIPT_END(Nh_ECMAScript_parseCallExpressionRecursively(Parser_p, CallExpression_p))
        }
    }

NH_ECMASCRIPT_END(Nh_ECMAScript_initParseResult(PreviousCallExpression_p))
}

static Nh_ECMAScript_ParseResult Nh_ECMAScript_parseCallExpression(
    Nh_ECMAScript_Parser *Parser_p)
{
NH_ECMASCRIPT_BEGIN()

    Nh_ECMAScript_Parser LocalParser = *Parser_p;
    Nh_ECMAScript_ParseResult CallExpression = Nh_ECMAScript_parseCallExpressionRecursively(&LocalParser, NULL);
    
    if (CallExpression.Node_p != NULL && CallExpression.SyntaxErrors.size == 0) {
        *Parser_p = LocalParser;
    }

NH_ECMASCRIPT_END(CallExpression)
}

static Nh_ECMAScript_ParseResult Nh_ECMAScript_parseLeftHandSideExpression(
    Nh_ECMAScript_Parser *Parser_p)
{
NH_ECMASCRIPT_BEGIN()

    Nh_ECMAScript_ParseResult Call = Nh_ECMAScript_parseCallExpression(Parser_p);

    if (Call.Node_p != NULL && Call.SyntaxErrors.size == 0) {
        Nh_ECMAScript_ParseNode *LeftHandSideExpression_p = Nh_ECMAScript_allocateNonTerminalParseNode("LeftHandSideExpression", 1);
        Nh_appendToList(&LeftHandSideExpression_p->Children, Call.Node_p);
        NH_ECMASCRIPT_END(Nh_ECMAScript_initParseResult(LeftHandSideExpression_p))
    }

    Nh_ECMAScript_ParseResult New = Nh_ECMAScript_parseNewExpression(Parser_p);

    if (New.Node_p != NULL && New.SyntaxErrors.size == 0) {
        Nh_ECMAScript_ParseNode *LeftHandSideExpression_p = Nh_ECMAScript_allocateNonTerminalParseNode("LeftHandSideExpression", 1);
        Nh_appendToList(&LeftHandSideExpression_p->Children, New.Node_p);
        NH_ECMASCRIPT_END(Nh_ECMAScript_initParseResult(LeftHandSideExpression_p))
    }

    Nh_ECMAScript_ParseResult Optional = Nh_ECMAScript_parseOptionalExpression(Parser_p);

    if (Optional.Node_p != NULL && Optional.SyntaxErrors.size == 0) {
        Nh_ECMAScript_ParseNode *LeftHandSideExpression_p = Nh_ECMAScript_allocateNonTerminalParseNode("LeftHandSideExpression", 1);
        Nh_appendToList(&LeftHandSideExpression_p->Children, Optional.Node_p);
        NH_ECMASCRIPT_END(Nh_ECMAScript_initParseResult(LeftHandSideExpression_p))
    }

NH_ECMASCRIPT_END(Nh_ECMAScript_initParseResult(NULL))
}

static Nh_ECMAScript_ParseResult Nh_ECMAScript_parseUpdateExpression(
    Nh_ECMAScript_Parser *Parser_p)
{
NH_ECMASCRIPT_BEGIN()

    Nh_ECMAScript_ParseResult LeftHandSide = Nh_ECMAScript_parseLeftHandSideExpression(Parser_p);

    if (LeftHandSide.Node_p != NULL && LeftHandSide.SyntaxErrors.size == 0) {
        Nh_ECMAScript_ParseNode *UpdateExpression_p = Nh_ECMAScript_allocateNonTerminalParseNode("UpdateExpression", 1);
        Nh_appendToList(&UpdateExpression_p->Children, LeftHandSide.Node_p);
        NH_ECMASCRIPT_END(Nh_ECMAScript_initParseResult(UpdateExpression_p))
    }

NH_ECMASCRIPT_END(LeftHandSide)
}

static Nh_ECMAScript_ParseResult Nh_ECMAScript_parseUnaryExpression(
    Nh_ECMAScript_Parser *Parser_p)
{
NH_ECMASCRIPT_BEGIN()

    Nh_ECMAScript_ParseResult Update = Nh_ECMAScript_parseUpdateExpression(Parser_p);

    if (Update.Node_p != NULL && Update.SyntaxErrors.size == 0) {
        Nh_ECMAScript_ParseNode *UnaryExpression_p = Nh_ECMAScript_allocateNonTerminalParseNode("UnaryExpression", 1);
        Nh_appendToList(&UnaryExpression_p->Children, Update.Node_p);
        NH_ECMASCRIPT_END(Nh_ECMAScript_initParseResult(UnaryExpression_p))
    }

NH_ECMASCRIPT_END(Update)
}

static Nh_ECMAScript_ParseResult Nh_ECMAScript_parseExponentiationExpression(
    Nh_ECMAScript_Parser *Parser_p)
{
NH_ECMASCRIPT_BEGIN()

    Nh_ECMAScript_ParseResult Unary = Nh_ECMAScript_parseUnaryExpression(Parser_p);

    if (Unary.Node_p != NULL && Unary.SyntaxErrors.size == 0) {
        Nh_ECMAScript_ParseNode *ExponentiationExpression_p = Nh_ECMAScript_allocateNonTerminalParseNode("ExponentiationExpression", 1);
        Nh_appendToList(&ExponentiationExpression_p->Children, Unary.Node_p);
        NH_ECMASCRIPT_END(Nh_ECMAScript_initParseResult(ExponentiationExpression_p))
    }

NH_ECMASCRIPT_END(Unary)
}

static Nh_ECMAScript_ParseResult Nh_ECMAScript_parseMultiplicativeExpression(
    Nh_ECMAScript_Parser *Parser_p)
{
NH_ECMASCRIPT_BEGIN()

    Nh_ECMAScript_ParseResult Exponentiation = Nh_ECMAScript_parseExponentiationExpression(Parser_p);

    if (Exponentiation.Node_p != NULL && Exponentiation.SyntaxErrors.size == 0) {
        Nh_ECMAScript_ParseNode *MultiplicativeExpression_p = Nh_ECMAScript_allocateNonTerminalParseNode("MultiplicativeExpression", 1);
        Nh_appendToList(&MultiplicativeExpression_p->Children, Exponentiation.Node_p);
        NH_ECMASCRIPT_END(Nh_ECMAScript_initParseResult(MultiplicativeExpression_p))
    }

NH_ECMASCRIPT_END(Exponentiation)
}

static Nh_ECMAScript_ParseResult Nh_ECMAScript_parseAdditiveExpression(
    Nh_ECMAScript_Parser *Parser_p)
{
NH_ECMASCRIPT_BEGIN()

    Nh_ECMAScript_ParseResult Multiplicative = Nh_ECMAScript_parseMultiplicativeExpression(Parser_p);

    if (Multiplicative.Node_p != NULL && Multiplicative.SyntaxErrors.size == 0) {
        Nh_ECMAScript_ParseNode *AdditiveExpression_p = Nh_ECMAScript_allocateNonTerminalParseNode("AdditiveExpression", 1);
        Nh_appendToList(&AdditiveExpression_p->Children, Multiplicative.Node_p);
        NH_ECMASCRIPT_END(Nh_ECMAScript_initParseResult(AdditiveExpression_p))
    }

NH_ECMASCRIPT_END(Multiplicative)
}

static Nh_ECMAScript_ParseResult Nh_ECMAScript_parseShiftExpression(
    Nh_ECMAScript_Parser *Parser_p)
{
NH_ECMASCRIPT_BEGIN()

    Nh_ECMAScript_ParseResult Result = Nh_ECMAScript_parseAdditiveExpression(Parser_p);

    if (Result.Node_p != NULL && Result.SyntaxErrors.size == 0) {
        Nh_ECMAScript_ParseNode *ShiftExpression_p = Nh_ECMAScript_allocateNonTerminalParseNode("ShiftExpression", 1);
        Nh_appendToList(&ShiftExpression_p->Children, Result.Node_p);
        NH_ECMASCRIPT_END(Nh_ECMAScript_initParseResult(ShiftExpression_p))
    }

NH_ECMASCRIPT_END(Result)
}

static Nh_ECMAScript_ParseResult Nh_ECMAScript_parseRelationalExpression(
    Nh_ECMAScript_Parser *Parser_p)
{
NH_ECMASCRIPT_BEGIN()

    Nh_ECMAScript_ParseResult Result = Nh_ECMAScript_parseShiftExpression(Parser_p);

    if (Result.Node_p != NULL && Result.SyntaxErrors.size == 0) {
        Nh_ECMAScript_ParseNode *RelationalExpression_p = Nh_ECMAScript_allocateNonTerminalParseNode("RelationalExpression", 1);
        Nh_appendToList(&RelationalExpression_p->Children, Result.Node_p);
        NH_ECMASCRIPT_END(Nh_ECMAScript_initParseResult(RelationalExpression_p))
    }

NH_ECMASCRIPT_END(Result)
}

static Nh_ECMAScript_ParseResult Nh_ECMAScript_parseEqualityExpression(
    Nh_ECMAScript_Parser *Parser_p)
{
NH_ECMASCRIPT_BEGIN()

    Nh_ECMAScript_ParseResult Result = Nh_ECMAScript_parseRelationalExpression(Parser_p);

    if (Result.Node_p != NULL && Result.SyntaxErrors.size == 0) {
        Nh_ECMAScript_ParseNode *EqualityExpression_p = Nh_ECMAScript_allocateNonTerminalParseNode("EqualityExpression", 1);
        Nh_appendToList(&EqualityExpression_p->Children, Result.Node_p);
        NH_ECMASCRIPT_END(Nh_ECMAScript_initParseResult(EqualityExpression_p))
    }

NH_ECMASCRIPT_END(Result)
}

static Nh_ECMAScript_ParseResult Nh_ECMAScript_parseBitwiseANDExpression(
    Nh_ECMAScript_Parser *Parser_p)
{
NH_ECMASCRIPT_BEGIN()

    Nh_ECMAScript_ParseResult Result = Nh_ECMAScript_parseEqualityExpression(Parser_p);

    if (Result.Node_p != NULL && Result.SyntaxErrors.size == 0) {
        Nh_ECMAScript_ParseNode *BitwiseANDExpression_p = Nh_ECMAScript_allocateNonTerminalParseNode("BitwiseANDExpression", 1);
        Nh_appendToList(&BitwiseANDExpression_p->Children, Result.Node_p);
        NH_ECMASCRIPT_END(Nh_ECMAScript_initParseResult(BitwiseANDExpression_p))
    }

NH_ECMASCRIPT_END(Result)
}

static Nh_ECMAScript_ParseResult Nh_ECMAScript_parseBitwiseXORExpression(
    Nh_ECMAScript_Parser *Parser_p)
{
NH_ECMASCRIPT_BEGIN()

    Nh_ECMAScript_ParseResult Result = Nh_ECMAScript_parseBitwiseANDExpression(Parser_p);

    if (Result.Node_p != NULL && Result.SyntaxErrors.size == 0) {
        Nh_ECMAScript_ParseNode *BitwiseXORExpression_p = Nh_ECMAScript_allocateNonTerminalParseNode("BitwiseXORExpression", 1);
        Nh_appendToList(&BitwiseXORExpression_p->Children, Result.Node_p);
        NH_ECMASCRIPT_END(Nh_ECMAScript_initParseResult(BitwiseXORExpression_p))
    }

NH_ECMASCRIPT_END(Result)
}

static Nh_ECMAScript_ParseResult Nh_ECMAScript_parseBitwiseORExpression(
    Nh_ECMAScript_Parser *Parser_p)
{
NH_ECMASCRIPT_BEGIN()

    Nh_ECMAScript_ParseResult Result = Nh_ECMAScript_parseBitwiseXORExpression(Parser_p);

    if (Result.Node_p != NULL && Result.SyntaxErrors.size == 0) {
        Nh_ECMAScript_ParseNode *BitwiseORExpression_p = Nh_ECMAScript_allocateNonTerminalParseNode("BitwiseORExpression", 1);
        Nh_appendToList(&BitwiseORExpression_p->Children, Result.Node_p);
        NH_ECMASCRIPT_END(Nh_ECMAScript_initParseResult(BitwiseORExpression_p))
    }

NH_ECMASCRIPT_END(Result)
}

static Nh_ECMAScript_ParseResult Nh_ECMAScript_parseLogicalANDExpression(
    Nh_ECMAScript_Parser *Parser_p)
{
NH_ECMASCRIPT_BEGIN()

    Nh_ECMAScript_ParseResult Result = Nh_ECMAScript_parseBitwiseORExpression(Parser_p);

    if (Result.Node_p != NULL && Result.SyntaxErrors.size == 0) {
        Nh_ECMAScript_ParseNode *LogicalANDExpression_p = Nh_ECMAScript_allocateNonTerminalParseNode("LogicalANDExpression", 1);
        Nh_appendToList(&LogicalANDExpression_p->Children, Result.Node_p);
        NH_ECMASCRIPT_END(Nh_ECMAScript_initParseResult(LogicalANDExpression_p))
    }

NH_ECMASCRIPT_END(Result)
}

static Nh_ECMAScript_ParseResult Nh_ECMAScript_parseLogicalORExpression(
    Nh_ECMAScript_Parser *Parser_p)
{
NH_ECMASCRIPT_BEGIN()

    Nh_ECMAScript_ParseResult Result = Nh_ECMAScript_parseLogicalANDExpression(Parser_p);

    if (Result.Node_p != NULL && Result.SyntaxErrors.size == 0) {
        Nh_ECMAScript_ParseNode *LogicalORExpression_p = Nh_ECMAScript_allocateNonTerminalParseNode("LogicalORExpression", 1);
        Nh_appendToList(&LogicalORExpression_p->Children, Result.Node_p);
        NH_ECMASCRIPT_END(Nh_ECMAScript_initParseResult(LogicalORExpression_p))
    }

NH_ECMASCRIPT_END(Result)
}

static Nh_ECMAScript_ParseResult Nh_ECMAScript_parseShortCircuitExpression(
    Nh_ECMAScript_Parser *Parser_p)
{
NH_ECMASCRIPT_BEGIN()

    Nh_ECMAScript_ParseResult Result = Nh_ECMAScript_parseLogicalORExpression(Parser_p);

    if (Result.Node_p != NULL && Result.SyntaxErrors.size == 0) {
        Nh_ECMAScript_ParseNode *ShortCircuitExpression_p = Nh_ECMAScript_allocateNonTerminalParseNode("ShortCircuitExpression", 1);
        Nh_appendToList(&ShortCircuitExpression_p->Children, Result.Node_p);
        NH_ECMASCRIPT_END(Nh_ECMAScript_initParseResult(ShortCircuitExpression_p))
    }

NH_ECMASCRIPT_END(Result)
}

static Nh_ECMAScript_ParseResult Nh_ECMAScript_parseConditionalExpression(
    Nh_ECMAScript_Parser *Parser_p)
{
NH_ECMASCRIPT_BEGIN()

    Nh_ECMAScript_ParseResult ShortCircuit = Nh_ECMAScript_parseShortCircuitExpression(Parser_p);

    if (ShortCircuit.Node_p != NULL && ShortCircuit.SyntaxErrors.size == 0) {
        Nh_ECMAScript_ParseNode *ConditionalExpression_p = Nh_ECMAScript_allocateNonTerminalParseNode("ConditionalExpression", 1);
        Nh_appendToList(&ConditionalExpression_p->Children, ShortCircuit.Node_p);
        NH_ECMASCRIPT_END(Nh_ECMAScript_initParseResult(ConditionalExpression_p))
    }

NH_ECMASCRIPT_END(ShortCircuit)
}

static Nh_ECMAScript_ParseResult Nh_ECMAScript_parseAssignmentExpression(
    Nh_ECMAScript_Parser *Parser_p)
{
NH_ECMASCRIPT_BEGIN()

    Nh_ECMAScript_Parser LocalParser = *Parser_p;
    Nh_ECMAScript_ParseResult Result = Nh_ECMAScript_parseLeftHandSideExpression(&LocalParser);

    if (Result.Node_p != NULL && Result.SyntaxErrors.size == 0) 
    {
        if (LocalParser.unparsed > 0 && LocalParser.Tokens_p[0].String.bytes_p[0] == '=') 
        {
            Nh_ECMAScript_Parser Equal = LocalParser;
            LocalParser = Nh_ECMAScript_advanceParser(LocalParser, 1);
            Nh_ECMAScript_ParseResult AssignmentResult = Nh_ECMAScript_parseAssignmentExpression(&LocalParser);

            if (AssignmentResult.Node_p != NULL && AssignmentResult.SyntaxErrors.size == 0) 
            {
                Nh_ECMAScript_ParseNode *AssignmentExpression_p = Nh_ECMAScript_allocateNonTerminalParseNode("AssignmentExpression", 3);
                Nh_ECMAScript_ParseNode *Equal_p = Nh_ECMAScript_allocateTerminalParseNode(&Equal.Tokens_p[0]);

                Nh_appendToList(&AssignmentExpression_p->Children, Result.Node_p);
                Nh_appendToList(&AssignmentExpression_p->Children, Equal_p);
                Nh_appendToList(&AssignmentExpression_p->Children, AssignmentResult.Node_p);

                *Parser_p = LocalParser;
                NH_ECMASCRIPT_END(Nh_ECMAScript_initParseResult(AssignmentExpression_p))
            }
        }
        Nh_ECMAScript_freeParseNode(Result.Node_p);
    }

    LocalParser = *Parser_p;
    Result = Nh_ECMAScript_parseConditionalExpression(&LocalParser);

    if (Result.Node_p != NULL && Result.SyntaxErrors.size == 0) {
        Nh_ECMAScript_ParseNode *AssignmentExpression_p = Nh_ECMAScript_allocateNonTerminalParseNode("AssignmentExpression", 1);
        Nh_appendToList(&AssignmentExpression_p->Children, Result.Node_p);
        *Parser_p = LocalParser;
        NH_ECMASCRIPT_END(Nh_ECMAScript_initParseResult(AssignmentExpression_p))
    }

NH_ECMASCRIPT_END(Result)
}

static Nh_ECMAScript_ParseResult Nh_ECMAScript_parseInitializer(
    Nh_ECMAScript_Parser *Parser_p)
{
NH_ECMASCRIPT_BEGIN()

    if (Parser_p->unparsed <= 0 || strcmp(Parser_p->Tokens_p[0].String.bytes_p, "=")) {
        NH_ECMASCRIPT_END(Nh_ECMAScript_initParseResult(NULL))
    }

    Nh_ECMAScript_Parser LocalParser = Nh_ECMAScript_advanceParser(*Parser_p, 1);
    Nh_ECMAScript_ParseResult Result = Nh_ECMAScript_parseAssignmentExpression(&LocalParser);

    if (Result.Node_p != NULL && Result.SyntaxErrors.size == 0) 
    {
        Nh_ECMAScript_ParseNode *Initializer_p = Nh_ECMAScript_allocateNonTerminalParseNode("Initializer", 2);
        Nh_ECMAScript_ParseNode *Equal_p = Nh_ECMAScript_allocateTerminalParseNode(&Parser_p->Tokens_p[0]);

        Nh_appendToList(&Initializer_p->Children, Equal_p);
        Nh_appendToList(&Initializer_p->Children, Result.Node_p);

        *Parser_p = LocalParser;
        NH_ECMASCRIPT_END(Nh_ECMAScript_initParseResult(Initializer_p))
    }

NH_ECMASCRIPT_END(Result)
}

static Nh_ECMAScript_ParseResult Nh_ECMAScript_parseIdentifier(
    Nh_ECMAScript_Parser *Parser_p)
{
NH_ECMASCRIPT_BEGIN()

    if (Parser_p->unparsed <= 0 || Parser_p->Tokens_p[0].type != NH_ECMASCRIPT_INPUT_ELEMENT_TOKEN_IDENTIFIER_NAME) {
        NH_ECMASCRIPT_END(Nh_ECMAScript_initParseResult(NULL))
    }

    unsigned int *index_p = Nh_getFromHashMap(&NH_ECMASCRIPT_INDEXMAP.ReservedWords, Parser_p->Tokens_p[0].String.bytes_p);

    if (index_p == NULL) 
    {
        Nh_ECMAScript_ParseNode *Identifier_p = Nh_ECMAScript_allocateNonTerminalParseNode("Identifier", 1);
        Nh_ECMAScript_ParseNode *IdentifierName_p = Nh_ECMAScript_allocateTerminalParseNode(&Parser_p->Tokens_p[0]);

        Nh_appendToList(&Identifier_p->Children, IdentifierName_p);
        *Parser_p = Nh_ECMAScript_advanceParser(*Parser_p, 1);
        NH_ECMASCRIPT_END(Nh_ECMAScript_initParseResult(Identifier_p))
    }

NH_ECMASCRIPT_END(Nh_ECMAScript_initParseResult(NULL))
}

static Nh_ECMAScript_ParseResult Nh_ECMAScript_parseBindingIdentifier(
    Nh_ECMAScript_Parser *Parser_p)
{
NH_ECMASCRIPT_BEGIN()

    Nh_ECMAScript_ParseResult Result = Nh_ECMAScript_initParseResult(NULL);

    if (Parser_p->unparsed <= 0) {NH_ECMASCRIPT_END(Result)}

    if (!strcmp("yield", Parser_p->Tokens_p[0].String.bytes_p) || !strcmp("await", Parser_p->Tokens_p[0].String.bytes_p)) 
    {
        Nh_ECMAScript_ParseNode *BindingIdentifier_p = Nh_ECMAScript_allocateNonTerminalParseNode("BindingIdentifier", 1);
        Nh_ECMAScript_ParseNode *YieldOrAwait_p = Nh_ECMAScript_allocateTerminalParseNode(&Parser_p->Tokens_p[0]);

        Nh_appendToList(&BindingIdentifier_p->Children, YieldOrAwait_p);
        *Parser_p = Nh_ECMAScript_advanceParser(*Parser_p, 1);
        NH_ECMASCRIPT_END(Nh_ECMAScript_initParseResult(BindingIdentifier_p))
    }
    else {

        Result = Nh_ECMAScript_parseIdentifier(Parser_p);        

        if (Result.Node_p != NULL && Result.SyntaxErrors.size == 0) {
            Nh_ECMAScript_ParseNode *BindingIdentifier_p = Nh_ECMAScript_allocateNonTerminalParseNode("BindingIdentifier", 1);
            Nh_appendToList(&BindingIdentifier_p->Children, Result.Node_p);
            NH_ECMASCRIPT_END(Nh_ECMAScript_initParseResult(BindingIdentifier_p))
        }
    }

NH_ECMASCRIPT_END(Result)
}

static Nh_ECMAScript_ParseResult Nh_ECMAScript_parseVariableDeclaration(
    Nh_ECMAScript_Parser *Parser_p)
{
NH_ECMASCRIPT_BEGIN()

    Nh_ECMAScript_ParseResult Result = Nh_ECMAScript_parseBindingIdentifier(Parser_p);

    if (Result.Node_p != NULL && Result.SyntaxErrors.size == 0) 
    {
        Nh_ECMAScript_ParseNode *VariableDeclaration_p = Nh_ECMAScript_allocateNonTerminalParseNode("VariableDeclaration", 1);
        Nh_ECMAScript_ParseResult InitializerResult = Nh_ECMAScript_parseInitializer(Parser_p);

        if (InitializerResult.Node_p != NULL && InitializerResult.SyntaxErrors.size == 0) {
            VariableDeclaration_p->Children = Nh_initList(2);
            Nh_appendToList(&VariableDeclaration_p->Children, Result.Node_p);
            Nh_appendToList(&VariableDeclaration_p->Children, InitializerResult.Node_p);
        }
        else {
            VariableDeclaration_p->Children = Nh_initList(1);
            Nh_appendToList(&VariableDeclaration_p->Children, Result.Node_p);
        }

        Result = Nh_ECMAScript_initParseResult(VariableDeclaration_p);
    }

NH_ECMASCRIPT_END(Result)
}

static Nh_ECMAScript_ParseResult Nh_ECMAScript_parseVariableDeclarationList(
    Nh_ECMAScript_Parser *Parser_p)
{
NH_ECMASCRIPT_BEGIN()
NH_ECMASCRIPT_END(Nh_ECMAScript_parseEasyRecursiveProduction(Parser_p, "VariableDeclarationList", NH_TRUE, Nh_ECMAScript_parseVariableDeclaration))
}

static Nh_ECMAScript_ParseResult Nh_ECMAScript_parseVariableStatement(
    Nh_ECMAScript_Parser *Parser_p)
{
NH_ECMASCRIPT_BEGIN()

    if (Parser_p->unparsed <= 0 || strcmp(Parser_p->Tokens_p[0].String.bytes_p, "var")) {
        NH_ECMASCRIPT_END(Nh_ECMAScript_initParseResult(NULL))
    }

    Nh_ECMAScript_Parser LocalParser = Nh_ECMAScript_advanceParser(*Parser_p, 1);
    Nh_ECMAScript_ParseResult Result = Nh_ECMAScript_parseVariableDeclarationList(&LocalParser);

    if (Result.Node_p != NULL && Result.SyntaxErrors.size == 0) 
    {
        if (LocalParser.unparsed <= 0 || LocalParser.Tokens_p[0].String.bytes_p[0] != ';') {
            Nh_ECMAScript_freeParseNode(Result.Node_p);
            NH_ECMASCRIPT_END(Nh_ECMAScript_initParseResult(NULL))
        }

        Nh_ECMAScript_ParseNode *VariableStatement_p = Nh_ECMAScript_allocateNonTerminalParseNode("VariableStatement", 3);
        Nh_ECMAScript_ParseNode *Var_p = Nh_ECMAScript_allocateTerminalParseNode(&Parser_p->Tokens_p[0]);
        Nh_ECMAScript_ParseNode *Semicolon_p = Nh_ECMAScript_allocateTerminalParseNode(&LocalParser.Tokens_p[0]);

        Nh_appendToList(&VariableStatement_p->Children, Var_p);
        Nh_appendToList(&VariableStatement_p->Children, Result.Node_p);
        Nh_appendToList(&VariableStatement_p->Children, Semicolon_p);

        *Parser_p = Nh_ECMAScript_advanceParser(LocalParser, 1);
        NH_ECMASCRIPT_END(Nh_ECMAScript_initParseResult(VariableStatement_p))
    }

NH_ECMASCRIPT_END(Result)
}

static Nh_ECMAScript_ParseResult Nh_ECMAScript_parseEmptyStatement(
    Nh_ECMAScript_Parser *Parser_p)
{
NH_ECMASCRIPT_BEGIN()

    if (Parser_p->unparsed <= 0 || Parser_p->Tokens_p[0].String.bytes_p[0] != ';') {
        NH_ECMASCRIPT_END(Nh_ECMAScript_initParseResult(NULL))
    }

    Nh_ECMAScript_ParseNode *EmptyStatement_p = Nh_ECMAScript_allocateNonTerminalParseNode("EmptyStatement", 1);
    Nh_ECMAScript_ParseNode *Semicolon_p = Nh_ECMAScript_allocateTerminalParseNode(&Parser_p->Tokens_p[0]);

    Nh_appendToList(&EmptyStatement_p->Children, Semicolon_p);
    *Parser_p = Nh_ECMAScript_advanceParser(*Parser_p, 1);

NH_ECMASCRIPT_END(Nh_ECMAScript_initParseResult(EmptyStatement_p))
}

static Nh_ECMAScript_ParseResult Nh_ECMAScript_parseExpression(
    Nh_ECMAScript_Parser *Parser_p)
{
NH_ECMASCRIPT_BEGIN()
NH_ECMASCRIPT_END(Nh_ECMAScript_parseEasyRecursiveProduction(Parser_p, "Expression", NH_TRUE, Nh_ECMAScript_parseAssignmentExpression))
}

static Nh_ECMAScript_ParseResult Nh_ECMAScript_parseExpressionStatement(
    Nh_ECMAScript_Parser *Parser_p)
{
NH_ECMASCRIPT_BEGIN()

// TODO [lookahead ∉ { {, function, async [no LineTerminator here] function, class, let [ }] Expression[+In, ?Yield, ?Await] ; 

    if (Parser_p->unparsed <= 0 || Parser_p->Tokens_p[0].String.bytes_p[0] == '{' || !strcmp("function", Parser_p->Tokens_p[0].String.bytes_p) || !strcmp("class", Parser_p->Tokens_p[0].String.bytes_p)) {
        NH_ECMASCRIPT_END(Nh_ECMAScript_initParseResult(NULL))
    }
    else if (Parser_p->unparsed >= 2) {
        if (!strcmp("let", Parser_p->Tokens_p[0].String.bytes_p) && Parser_p->Tokens_p[1].String.bytes_p[0] == '[') {
            NH_ECMASCRIPT_END(Nh_ECMAScript_initParseResult(NULL))
        }
    }

    Nh_ECMAScript_Parser LocalParser = *Parser_p;
    Nh_ECMAScript_ParseResult Result = Nh_ECMAScript_parseExpression(&LocalParser);

    if (Result.Node_p != NULL && Result.SyntaxErrors.size == 0) 
    {
        if (LocalParser.unparsed > 0 && LocalParser.Tokens_p[0].String.bytes_p[0] != ';') {
            Nh_ECMAScript_freeParseNode(Result.Node_p);
            NH_ECMASCRIPT_END(Nh_ECMAScript_initParseResult(NULL))
        }

        Nh_ECMAScript_ParseNode *ExpressionStatement_p = Nh_ECMAScript_allocateNonTerminalParseNode("ExpressionStatement", 2);
        Nh_ECMAScript_ParseNode *Semicolon_p = Nh_ECMAScript_allocateTerminalParseNode(&LocalParser.Tokens_p[0]);

        Nh_appendToList(&ExpressionStatement_p->Children, Result.Node_p);
        Nh_appendToList(&ExpressionStatement_p->Children, Semicolon_p);

        *Parser_p = Nh_ECMAScript_advanceParser(LocalParser, 1);

        NH_ECMASCRIPT_END(Nh_ECMAScript_initParseResult(ExpressionStatement_p))
    }

NH_ECMASCRIPT_END(Nh_ECMAScript_initParseResult(NULL))
}

static Nh_ECMAScript_ParseResult Nh_ECMAScript_parseStatement(
    Nh_ECMAScript_Parser *Parser_p)
{
NH_ECMASCRIPT_BEGIN()

    Nh_ECMAScript_ParseResult Result = Nh_ECMAScript_parseBlockStatement(Parser_p);
    if (Result.Node_p != NULL) {goto PARSE_STATEMENT_SUCCESS;}

    Result = Nh_ECMAScript_parseVariableStatement(Parser_p);
    if (Result.Node_p != NULL) {goto PARSE_STATEMENT_SUCCESS;}

    Result = Nh_ECMAScript_parseEmptyStatement(Parser_p);
    if (Result.Node_p != NULL) {goto PARSE_STATEMENT_SUCCESS;}

    Result = Nh_ECMAScript_parseExpressionStatement(Parser_p);
    if (Result.Node_p != NULL) {goto PARSE_STATEMENT_SUCCESS;}

NH_ECMASCRIPT_END(Result)

PARSE_STATEMENT_SUCCESS: ;

    Nh_ECMAScript_ParseNode *Statement_p = Nh_ECMAScript_allocateNonTerminalParseNode("Statement", 1);
    Nh_appendToList(&Statement_p->Children, Result.Node_p);

NH_ECMASCRIPT_END(Nh_ECMAScript_initParseResult(Statement_p))
}

static Nh_ECMAScript_ParseResult Nh_ECMAScript_parseStatementListItem(
    Nh_ECMAScript_Parser *Parser_p)
{
NH_ECMASCRIPT_BEGIN()

    Nh_ECMAScript_ParseResult Result = Nh_ECMAScript_parseStatement(Parser_p);

    if (Result.Node_p != NULL) 
    {
        Nh_ECMAScript_ParseNode *StatementListItem_p = Nh_ECMAScript_allocateNonTerminalParseNode("StatementListItem", 1);
        Nh_appendToList(&StatementListItem_p->Children, Result.Node_p);
        NH_ECMASCRIPT_END(Nh_ECMAScript_initParseResult(StatementListItem_p))
    }

NH_ECMASCRIPT_END(Result)
}

static Nh_ECMAScript_ParseResult Nh_ECMAScript_parseStatementList(
    Nh_ECMAScript_Parser *Parser_p)
{
NH_ECMASCRIPT_BEGIN()
NH_ECMASCRIPT_END(Nh_ECMAScript_parseEasyRecursiveProduction(Parser_p, "StatementList", NH_FALSE, Nh_ECMAScript_parseStatementListItem))
}

static Nh_ECMAScript_ParseResult Nh_ECMAScript_parseScriptBody(
    Nh_ECMAScript_Parser *Parser_p)
{
NH_ECMASCRIPT_BEGIN()

    Nh_ECMAScript_ParseResult Result = Nh_ECMAScript_parseStatementList(Parser_p);

    if (Result.Node_p != NULL && Result.SyntaxErrors.size <= 0) 
    {
        Nh_ECMAScript_ParseNode *ScriptBody_p = Nh_ECMAScript_allocateNonTerminalParseNode("ScriptBody", 1);
        Nh_appendToList(&ScriptBody_p->Children, Result.Node_p);
        NH_ECMASCRIPT_END(Nh_ECMAScript_initParseResult(ScriptBody_p))
    }

NH_ECMASCRIPT_END(Result)
}

static Nh_ECMAScript_ParseResult Nh_ECMAScript_parseScript(
    Nh_ECMAScript_Parser *Parser_p)
{
NH_ECMASCRIPT_BEGIN()

    Nh_ECMAScript_ParseResult Result = Nh_ECMAScript_parseScriptBody(Parser_p);

    if (Result.Node_p != NULL && Result.SyntaxErrors.size <= 0) 
    {
        Nh_ECMAScript_ParseNode *Script_p = Nh_ECMAScript_allocateNonTerminalParseNode("Script", 1);
        Nh_appendToList(&Script_p->Children, Result.Node_p);
        Result = Nh_ECMAScript_initParseResult(Script_p);
    }

NH_ECMASCRIPT_END(Result)
}

// https://tc39.es/ecma262/#sec-parsetext
Nh_ECMAScript_ParseResult Nh_ECMAScript_parseText(
    Nh_Array InputElements, NH_ECMASCRIPT_PARSE_NODE goalSymbol)
{
NH_ECMASCRIPT_BEGIN()

    Nh_ECMAScript_Parser Parser;
    Parser.Tokens_p = (Nh_ECMAScript_InputElement*)InputElements.bytes_p;
    Parser.unparsed = InputElements.length;

    Nh_ECMAScript_ParseResult Result = Nh_ECMAScript_initParseResult(NULL);

    switch (goalSymbol)
    {
        case NH_ECMASCRIPT_PARSE_NODE_SCRIPT :
            Result = Nh_ECMAScript_parseScript(&Parser);
            break;
    }

NH_ECMASCRIPT_END(Result)
}


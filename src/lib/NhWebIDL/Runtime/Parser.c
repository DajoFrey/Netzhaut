// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "Parser.h"

#include "../Common/IndexMap.h"
#include "../Common/Log.h"
#include "../Common/Macro.h"

#include NH_WEBIDL_DEFAULT_CHECK
#include NH_WEBIDL_FLOW

#include "../../NhCore/Memory.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// DATA ============================================================================================

const NH_BYTE *NH_WEBIDL_PARSE_NODE_NAMES_PP[] = {
    "Definitions",
    "Definition",
    "ArgumentNameKeyword",
    "CallbackOrInterfaceOrMixin",
    "InterfaceOrMixin",
    "InterfaceRest",
    "Partial",
    "PartialDefinition",
    "PartialInterfaceOrPartialMixin",
    "PartialInterfaceRest",
    "InterfaceMembers",
    "InterfaceMember",
    "PartialInterfaceMembers",
    "PartialInterfaceMember",
    "Inheritance",
    "MixinRest",
    "MixinMembers",
    "MixinMember",
    "IncludesStatement",
    "CallbackRestOrInterface",
    "CallbackInterfaceMembers",
    "CallbackInterfaceMember",
    "Const",
    "ConstValue",
    "BooleanLiteral",
    "FloatLiteral",
    "ConstType",
    "ReadOnlyMember",
    "ReadOnlyMemberRest",
    "ReadWriteAttribute",
    "InheritAttribute",
    "AttributeRest",
    "AttributeName",
    "AttributeNameKeyword",
    "OptionalReadOnly",
    "DefaultValue",
    "Operation",
    "RegularOperation",
    "SpecialOperation",
    "Special",
    "OperationRest",
    "OptionalOperationName",
    "OperationName",
    "OperationNameKeyword",
    "ArgumentList",
    "Arguments",
    "Argument",
    "ArgumentRest",
    "ArgumentName",
    "Ellipsis",
    "Constructor",
    "Stringifier",
    "StringifierRest",
    "StaticMember",
    "StaticMemberRest",
    "Iterable",
    "OptionalType",
    "AsyncIterable",
    "OptionalArgumentList",
    "ReadWriteMaplike",
    "MaplikeRest",
    "ReadWriteSetlike",
    "SetlikeRest",
    "Namespace",
    "NamespaceMembers",
    "NamespaceMember",
    "Dictionary",
    "DictionaryMembers",
    "DictionaryMember",
    "DictionaryMemberRest",
    "PartialDictionary",
    "Default",
    "Enum",
    "EnumValueList",
    "EnumValueListComma",
    "EnumValueListString",
    "CallbackRest",
    "Typedef",
    "Type",
    "TypeWithExtendedAttributes",
    "SingleType",
    "UnionType",
    "UnionMemberType",
    "UnionMemberTypes",
    "DistinguishableType",
    "PrimitiveType",
    "UnrestrictedFloatType",
    "FloatType",
    "UnsignedIntegerType",
    "IntegerType",
    "OptionalLong",
    "StringType",
    "PromiseType",
    "RecordType",
    "Null",
    "BufferRelatedType",
    "ExtendedAttributeList",
    "ExtendedAttributes",
    "ExtendedAttribute",
    "ExtendedAttributeRest",
    "ExtendedAttributeInner",
    "Other",
    "OtherOrComma",
    "IdentifierList",
    "Identifiers",
    "ExtendedAttributeNoArgs",
    "ExtendedAttributeArgList",
    "ExtendedAttributeIdent",
    "ExtendedAttributeIdentList",
    "ExtendedAttributeNamedArgList",
    "NON_STANDARD_Specifier",
};

size_t NH_WEBIDL_PARSE_NODE_NAMES_PP_COUNT = sizeof(NH_WEBIDL_PARSE_NODE_NAMES_PP) / sizeof(NH_WEBIDL_PARSE_NODE_NAMES_PP[0]);

// DECLARE =========================================================================================

typedef struct Nh_WebIDL_ParseResult {
    Nh_WebIDL_ParseNode *Node_p;
    Nh_List SyntaxErrors;
} Nh_WebIDL_ParseResult;

typedef struct Nh_WebIDL_Parser {
    Nh_WebIDL_Token *Tokens_p;
    int unparsed;
} Nh_WebIDL_Parser;

static Nh_WebIDL_ParseResult Nh_WebIDL_parseExtendedAttribute(
    Nh_WebIDL_Parser *Parser_p
);
static Nh_WebIDL_ParseResult Nh_WebIDL_parseType(
    Nh_WebIDL_Parser *Parser_p
);
static Nh_WebIDL_ParseResult Nh_WebIDL_parseTypeWithExtendedAttributes(
    Nh_WebIDL_Parser *Parser_p
);

// HELPER ==========================================================================================

static Nh_WebIDL_ParseResult Nh_WebIDL_initParseResult(
    Nh_WebIDL_ParseNode *Node_p)
{
NH_WEBIDL_BEGIN()

    Nh_WebIDL_ParseResult Result;
    Result.Node_p = Node_p;
    Result.SyntaxErrors = Nh_initList(8);

NH_WEBIDL_END(Result)
}

static Nh_WebIDL_ParseNode *Nh_WebIDL_allocateNonTerminalParseNode(
    const NH_BYTE *name_p, int chunkSize)
{
NH_WEBIDL_BEGIN()

    unsigned int *index_p = Nh_getFromHashMap(&NH_WEBIDL_INDEXMAP.ParseNodeNames, (NH_BYTE*)name_p);
    if (index_p == NULL) {NH_WEBIDL_END(NULL)}

    Nh_WebIDL_ParseNode *Node_p = Nh_allocate(sizeof(Nh_WebIDL_ParseNode));
    Node_p->Children = Nh_initList(chunkSize);
    Node_p->Token_p  = NULL;
    Node_p->type     = *index_p;

NH_WEBIDL_END(Node_p)
}

static Nh_WebIDL_ParseNode *Nh_WebIDL_allocateTerminalParseNode(
    Nh_WebIDL_Token *Token_p)
{
NH_WEBIDL_BEGIN()

    Nh_WebIDL_ParseNode *Node_p = Nh_allocate(sizeof(Nh_WebIDL_ParseNode));
    Node_p->Children = Nh_initList(1);
    Node_p->Token_p  = Token_p;
    Node_p->type     = -1;

NH_WEBIDL_END(Node_p)
}

static void Nh_WebIDL_freeParseNode(
    Nh_WebIDL_ParseNode *Node_p)
{
NH_WEBIDL_BEGIN()

    // TODO

NH_WEBIDL_SILENT_END()
}

static Nh_WebIDL_Parser Nh_WebIDL_advanceParser(
    Nh_WebIDL_Parser Parser, int steps)
{
NH_WEBIDL_BEGIN()

    Parser.unparsed -= steps;
    Parser.Tokens_p  = Parser.Tokens_p + steps;
 
NH_WEBIDL_END(Parser)
}

// PARSER ==========================================================================================

static Nh_WebIDL_ParseResult Nh_WebIDL_parseOther(
    Nh_WebIDL_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    if (Parser_p->unparsed == 0 
    ||  Parser_p->Tokens_p[0].String.bytes_p[0] == ','
    ||  Parser_p->Tokens_p[0].String.bytes_p[0] == '('
    ||  Parser_p->Tokens_p[0].String.bytes_p[0] == '['
    ||  Parser_p->Tokens_p[0].String.bytes_p[0] == '{'
    ||  Parser_p->Tokens_p[0].String.bytes_p[0] == ')'
    ||  Parser_p->Tokens_p[0].String.bytes_p[0] == ']'
    ||  Parser_p->Tokens_p[0].String.bytes_p[0] == '}') 
    {
        NH_WEBIDL_END(Nh_WebIDL_initParseResult(NULL))   
    }

    Nh_WebIDL_ParseNode *Other_p = Nh_WebIDL_allocateNonTerminalParseNode("Other", 1);
    Nh_WebIDL_ParseNode *Child_p = Nh_WebIDL_allocateTerminalParseNode(&Parser_p->Tokens_p[0]);
    Nh_appendToList(&Other_p->Children, Child_p);
    *Parser_p = Nh_WebIDL_advanceParser(*Parser_p, 1);

NH_WEBIDL_END(Nh_WebIDL_initParseResult(Other_p))
}

static Nh_WebIDL_ParseResult Nh_WebIDL_parseOtherOrComma(
    Nh_WebIDL_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    Nh_WebIDL_ParseResult Result = Nh_WebIDL_parseOther(Parser_p);

    if (Result.Node_p != NULL) {
        Nh_WebIDL_ParseNode *OtherOrComma_p = Nh_WebIDL_allocateNonTerminalParseNode("OtherOrComma", 1);
        Nh_appendToList(&OtherOrComma_p->Children, Result.Node_p);
        NH_WEBIDL_END(Nh_WebIDL_initParseResult(OtherOrComma_p))
    } 
    else if (Parser_p->unparsed > 0 && Parser_p->Tokens_p[0].String.bytes_p[0] == ',') 
    {
        Nh_WebIDL_ParseNode *OtherOrComma_p = Nh_WebIDL_allocateNonTerminalParseNode("OtherOrComma", 1);
        Nh_WebIDL_ParseNode *Child_p = Nh_WebIDL_allocateTerminalParseNode(&Parser_p->Tokens_p[0]);
        Nh_appendToList(&OtherOrComma_p->Children, Child_p);
        *Parser_p = Nh_WebIDL_advanceParser(*Parser_p, 1);
        NH_WEBIDL_END(Nh_WebIDL_initParseResult(OtherOrComma_p))
    }

NH_WEBIDL_END(Nh_WebIDL_initParseResult(NULL))
}

static Nh_WebIDL_ParseResult Nh_WebIDL_parseExtendedAttributeInner(
    Nh_WebIDL_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    NH_BYTE openBrackets_p[3] = "([{";
    NH_BYTE closeBrackets_p[3] = ")]}";

    for (int i = 0; i < 3; ++i) 
    {
        if (Parser_p->unparsed > 0 && Parser_p->Tokens_p[0].String.bytes_p[0] == openBrackets_p[i]) 
        {
            Nh_WebIDL_Parser LocalParser = *Parser_p;
            Nh_WebIDL_ParseResult Result1 = Nh_WebIDL_parseExtendedAttributeInner(&LocalParser);
    
            if (LocalParser.unparsed > 0 && LocalParser.Tokens_p[0].String.bytes_p[0] == closeBrackets_p[i]) 
            {
                Nh_WebIDL_ParseResult Result2 = Nh_WebIDL_parseExtendedAttributeInner(&LocalParser);
                Nh_WebIDL_ParseNode *ExtendedAttributeInner_p = Nh_WebIDL_allocateNonTerminalParseNode("ExtendedAttributeInner", 4);
                Nh_appendToList(&ExtendedAttributeInner_p->Children, Nh_WebIDL_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
                Nh_appendToList(&ExtendedAttributeInner_p->Children, Result1.Node_p);
                Nh_appendToList(&ExtendedAttributeInner_p->Children, Nh_WebIDL_allocateTerminalParseNode(&LocalParser.Tokens_p[0]));
                Nh_appendToList(&ExtendedAttributeInner_p->Children, Result2.Node_p);
                *Parser_p = Nh_WebIDL_advanceParser(LocalParser, 1);
                NH_WEBIDL_END(Nh_WebIDL_initParseResult(ExtendedAttributeInner_p))
            }
            else {Nh_WebIDL_freeParseNode(Result1.Node_p);}
        }
    }

    Nh_WebIDL_ParseNode *ExtendedAttributeInner_p = Nh_WebIDL_allocateNonTerminalParseNode("ExtendedAttributeInner", 1);
    Nh_WebIDL_ParseResult Result = Nh_WebIDL_parseOtherOrComma(Parser_p);

    if (Result.Node_p != NULL) {
        Nh_appendToList(&ExtendedAttributeInner_p->Children, Result.Node_p);
        Nh_WebIDL_ParseResult Result = Nh_WebIDL_parseExtendedAttributeInner(Parser_p);
        Nh_appendToList(&ExtendedAttributeInner_p->Children, Result.Node_p);
    } 

NH_WEBIDL_END(Nh_WebIDL_initParseResult(ExtendedAttributeInner_p))
}

static Nh_WebIDL_ParseResult Nh_WebIDL_parseExtendedAttributeRest(
    Nh_WebIDL_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    Nh_WebIDL_ParseResult Result = Nh_WebIDL_parseExtendedAttribute(Parser_p);
    Nh_WebIDL_ParseNode *ExtendedAttributeRest_p = Nh_WebIDL_allocateNonTerminalParseNode("ExtendedAttributeRest", 1);

    if (Result.Node_p != NULL) {
        Nh_appendToList(&ExtendedAttributeRest_p->Children, Result.Node_p);
    } 

NH_WEBIDL_END(Nh_WebIDL_initParseResult(ExtendedAttributeRest_p))
}

static Nh_WebIDL_ParseResult Nh_WebIDL_parseExtendedAttribute(
    Nh_WebIDL_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    NH_BYTE openBrackets_p[3] = "([{";
    NH_BYTE closeBrackets_p[3] = ")]}";

    for (int i = 0; i < 3; ++i) 
    {
        if (Parser_p->unparsed > 0 && Parser_p->Tokens_p[0].String.bytes_p[0] == openBrackets_p[i]) 
        {
            Nh_WebIDL_Parser LocalParser = Nh_WebIDL_advanceParser(*Parser_p, 1);
            Nh_WebIDL_ParseResult ExtendedAttributeInner = Nh_WebIDL_parseExtendedAttributeInner(&LocalParser);
    
            if (LocalParser.unparsed > 0 && LocalParser.Tokens_p[0].String.bytes_p[0] == closeBrackets_p[i]) 
            {
                Nh_WebIDL_ParseResult ExtendedAttributeRest = Nh_WebIDL_parseExtendedAttributeRest(&LocalParser);
                Nh_WebIDL_ParseNode *ExtendedAttribute_p = Nh_WebIDL_allocateNonTerminalParseNode("ExtendedAttribute", 4);

                Nh_appendToList(&ExtendedAttribute_p->Children, Nh_WebIDL_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
                Nh_appendToList(&ExtendedAttribute_p->Children, ExtendedAttributeInner.Node_p);
                Nh_appendToList(&ExtendedAttribute_p->Children, Nh_WebIDL_allocateTerminalParseNode(&LocalParser.Tokens_p[0]));
                Nh_appendToList(&ExtendedAttribute_p->Children, ExtendedAttributeRest.Node_p);

                *Parser_p = Nh_WebIDL_advanceParser(LocalParser, 1);
                NH_WEBIDL_END(Nh_WebIDL_initParseResult(ExtendedAttribute_p))
            }
            else {Nh_WebIDL_freeParseNode(ExtendedAttributeInner.Node_p);}
        }
    }

    Nh_WebIDL_ParseResult Other = Nh_WebIDL_parseOther(Parser_p);

    if (Other.Node_p != NULL) 
    {
        Nh_WebIDL_ParseResult ExtendedAttributeRest = Nh_WebIDL_parseExtendedAttributeRest(Parser_p);
        Nh_WebIDL_ParseNode *ExtendedAttribute_p = Nh_WebIDL_allocateNonTerminalParseNode("ExtendedAttribute", 2);
        Nh_appendToList(&ExtendedAttribute_p->Children, Other.Node_p);
        Nh_appendToList(&ExtendedAttribute_p->Children, ExtendedAttributeRest.Node_p);
        NH_WEBIDL_END(Nh_WebIDL_initParseResult(ExtendedAttribute_p))
    } 

NH_WEBIDL_END(Nh_WebIDL_initParseResult(NULL))
}

static Nh_WebIDL_ParseResult Nh_WebIDL_parseExtendedAttributes(
    Nh_WebIDL_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    Nh_WebIDL_ParseNode *ExtendedAttributes_p = Nh_WebIDL_allocateNonTerminalParseNode("ExtendedAttributes", 2);

    if (Parser_p->unparsed > 0 && Parser_p->Tokens_p[0].String.bytes_p[0] == ',') 
    {
        Nh_WebIDL_Parser LocalParser = Nh_WebIDL_advanceParser(*Parser_p, 1);
        Nh_WebIDL_ParseResult ExtendedAttribute = Nh_WebIDL_parseExtendedAttribute(&LocalParser);

        if (ExtendedAttribute.Node_p != NULL) 
        {
            Nh_WebIDL_ParseResult ExtendedAttributes = Nh_WebIDL_parseExtendedAttributes(&LocalParser);
            Nh_appendToList(&ExtendedAttributes_p->Children, Nh_WebIDL_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
            Nh_appendToList(&ExtendedAttributes_p->Children, ExtendedAttribute.Node_p);
            Nh_appendToList(&ExtendedAttributes_p->Children, ExtendedAttributes.Node_p);
            *Parser_p = LocalParser;
        }
    }

NH_WEBIDL_END(Nh_WebIDL_initParseResult(ExtendedAttributes_p))
}

static Nh_WebIDL_ParseResult Nh_WebIDL_parseExtendedAttributeList(
    Nh_WebIDL_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    Nh_WebIDL_ParseNode *ExtendedAttributeList_p = Nh_WebIDL_allocateNonTerminalParseNode("ExtendedAttributeList", 1);

    if (Parser_p->unparsed > 0 && Parser_p->Tokens_p[0].String.bytes_p[0] == '[') 
    {
        Nh_WebIDL_Parser LocalParser = Nh_WebIDL_advanceParser(*Parser_p, 1);
        Nh_WebIDL_ParseResult ExtendedAttribute = Nh_WebIDL_parseExtendedAttribute(&LocalParser);

        if (ExtendedAttribute.Node_p != NULL) 
        {
            Nh_WebIDL_ParseResult ExtendedAttributes = Nh_WebIDL_parseExtendedAttributes(&LocalParser);

            if (LocalParser.unparsed > 0 && LocalParser.Tokens_p[0].String.bytes_p[0] == ']') 
            {
                Nh_appendToList(&ExtendedAttributeList_p->Children, Nh_WebIDL_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
                Nh_appendToList(&ExtendedAttributeList_p->Children, ExtendedAttribute.Node_p);
                Nh_appendToList(&ExtendedAttributeList_p->Children, ExtendedAttributes.Node_p);
                Nh_appendToList(&ExtendedAttributeList_p->Children, Nh_WebIDL_allocateTerminalParseNode(&LocalParser.Tokens_p[0]));
                *Parser_p = Nh_WebIDL_advanceParser(LocalParser, 1);
            }
            else {
                Nh_WebIDL_freeParseNode(ExtendedAttributes.Node_p);
                Nh_WebIDL_freeParseNode(ExtendedAttribute.Node_p);
            }
        }
    }

NH_WEBIDL_END(Nh_WebIDL_initParseResult(ExtendedAttributeList_p))
}

static Nh_WebIDL_ParseResult Nh_WebIDL_parseOptionalLong(
    Nh_WebIDL_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    Nh_WebIDL_ParseNode *OptionalLong_p = Nh_WebIDL_allocateNonTerminalParseNode("OptionalLong", 1);

    if (!strcmp(Parser_p->Tokens_p[0].String.bytes_p, "long")) 
    {
        Nh_appendToList(&OptionalLong_p->Children, Nh_WebIDL_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
        *Parser_p = Nh_WebIDL_advanceParser(*Parser_p, 1);
    }

NH_WEBIDL_END(Nh_WebIDL_initParseResult(OptionalLong_p))
}

static Nh_WebIDL_ParseResult Nh_WebIDL_parseIntegerType(
    Nh_WebIDL_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    if (!strcmp(Parser_p->Tokens_p[0].String.bytes_p, "short"))
    {
        Nh_WebIDL_ParseNode *IntegerType_p = Nh_WebIDL_allocateNonTerminalParseNode("IntegerType", 1);
        Nh_appendToList(&IntegerType_p->Children, Nh_WebIDL_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
        *Parser_p = Nh_WebIDL_advanceParser(*Parser_p, 1);
        NH_WEBIDL_END(Nh_WebIDL_initParseResult(IntegerType_p))
    }

    if (!strcmp(Parser_p->Tokens_p[0].String.bytes_p, "long")) 
    {
        Nh_WebIDL_ParseNode *IntegerType_p = Nh_WebIDL_allocateNonTerminalParseNode("IntegerType", 1);
        Nh_appendToList(&IntegerType_p->Children, Nh_WebIDL_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
        *Parser_p = Nh_WebIDL_advanceParser(*Parser_p, 1);
        Nh_WebIDL_ParseResult OptionalLong = Nh_WebIDL_parseOptionalLong(Parser_p);
        Nh_appendToList(&IntegerType_p->Children, OptionalLong.Node_p);
        NH_WEBIDL_END(Nh_WebIDL_initParseResult(IntegerType_p))
    }

NH_WEBIDL_END(Nh_WebIDL_initParseResult(NULL))
}

static Nh_WebIDL_ParseResult Nh_WebIDL_parseUnsignedIntegerType(
    Nh_WebIDL_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    if (!strcmp(Parser_p->Tokens_p[0].String.bytes_p, "unsigned"))
    {
        Nh_WebIDL_Parser LocalParser = Nh_WebIDL_advanceParser(*Parser_p, 1);
        Nh_WebIDL_ParseResult IntegerType = Nh_WebIDL_parseIntegerType(&LocalParser);

        if (IntegerType.Node_p != NULL) 
        {
            Nh_WebIDL_ParseNode *UnsignedIntegerType_p = Nh_WebIDL_allocateNonTerminalParseNode("UnsignedIntegerType", 1);
            Nh_WebIDL_ParseNode *Unsigned_p = Nh_WebIDL_allocateTerminalParseNode(&Parser_p->Tokens_p[0]);
            Nh_appendToList(&UnsignedIntegerType_p->Children, Unsigned_p); 
            Nh_appendToList(&UnsignedIntegerType_p->Children, IntegerType.Node_p); 
            *Parser_p = LocalParser;
            NH_WEBIDL_END(Nh_WebIDL_initParseResult(UnsignedIntegerType_p))
        }
    }

    Nh_WebIDL_ParseResult IntegerType = Nh_WebIDL_parseIntegerType(Parser_p);
    if (IntegerType.Node_p != NULL) 
    {
        Nh_WebIDL_ParseNode *UnsignedIntegerType_p = Nh_WebIDL_allocateNonTerminalParseNode("UnsignedIntegerType", 1);
        Nh_appendToList(&UnsignedIntegerType_p->Children, IntegerType.Node_p);
        NH_WEBIDL_END(Nh_WebIDL_initParseResult(UnsignedIntegerType_p))
    }

NH_WEBIDL_END(Nh_WebIDL_initParseResult(NULL))
}

static Nh_WebIDL_ParseResult Nh_WebIDL_parseFloatType(
    Nh_WebIDL_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    if (!strcmp(Parser_p->Tokens_p[0].String.bytes_p, "float")
    ||  !strcmp(Parser_p->Tokens_p[0].String.bytes_p, "double"))
    {
        Nh_WebIDL_ParseNode *FloatType_p = Nh_WebIDL_allocateNonTerminalParseNode("FloatType", 1);
        Nh_WebIDL_ParseNode *Terminal_p = Nh_WebIDL_allocateTerminalParseNode(&Parser_p->Tokens_p[0]);
        Nh_appendToList(&FloatType_p->Children, Terminal_p);
        *Parser_p = Nh_WebIDL_advanceParser(*Parser_p, 1);
        NH_WEBIDL_END(Nh_WebIDL_initParseResult(FloatType_p))
    }

NH_WEBIDL_END(Nh_WebIDL_initParseResult(NULL))
}

static Nh_WebIDL_ParseResult Nh_WebIDL_parseUnrestrictedFloatType(
    Nh_WebIDL_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    if (!strcmp(Parser_p->Tokens_p[0].String.bytes_p, "unrestricted"))
    {
        Nh_WebIDL_Parser LocalParser = Nh_WebIDL_advanceParser(*Parser_p, 1);
        Nh_WebIDL_ParseResult FloatType = Nh_WebIDL_parseFloatType(&LocalParser);

        if (FloatType.Node_p != NULL) 
        {
            Nh_WebIDL_ParseNode *UnrestrictedFloatType_p = Nh_WebIDL_allocateNonTerminalParseNode("UnrestrictedFloatType", 1);
            Nh_WebIDL_ParseNode *Unrestricted_p = Nh_WebIDL_allocateTerminalParseNode(&Parser_p->Tokens_p[0]);
            Nh_appendToList(&UnrestrictedFloatType_p->Children, Unrestricted_p); 
            Nh_appendToList(&UnrestrictedFloatType_p->Children, FloatType.Node_p); 
            *Parser_p = LocalParser;
            NH_WEBIDL_END(Nh_WebIDL_initParseResult(UnrestrictedFloatType_p))
        }
    }

    Nh_WebIDL_ParseResult FloatType = Nh_WebIDL_parseFloatType(Parser_p);
    if (FloatType.Node_p != NULL) 
    {
        Nh_WebIDL_ParseNode *UnrestrictedFloatType_p = Nh_WebIDL_allocateNonTerminalParseNode("UnrestrictedFloatType", 1);
        Nh_appendToList(&UnrestrictedFloatType_p->Children, FloatType.Node_p);
        NH_WEBIDL_END(Nh_WebIDL_initParseResult(UnrestrictedFloatType_p))
    }

NH_WEBIDL_END(Nh_WebIDL_initParseResult(NULL))
}

static Nh_WebIDL_ParseResult Nh_WebIDL_parsePrimitiveType(
    Nh_WebIDL_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    if (Parser_p->unparsed == 0) {NH_WEBIDL_END(Nh_WebIDL_initParseResult(NULL))}

    if (!strcmp(Parser_p->Tokens_p[0].String.bytes_p, "undefined")
    ||  !strcmp(Parser_p->Tokens_p[0].String.bytes_p, "boolean")
    ||  !strcmp(Parser_p->Tokens_p[0].String.bytes_p, "byte")
    ||  !strcmp(Parser_p->Tokens_p[0].String.bytes_p, "octet")
    ||  !strcmp(Parser_p->Tokens_p[0].String.bytes_p, "bigint")) 
    {
        Nh_WebIDL_ParseNode *PrimitiveType_p = Nh_WebIDL_allocateNonTerminalParseNode("PrimitiveType", 1);
        Nh_appendToList(&PrimitiveType_p->Children, Nh_WebIDL_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
        *Parser_p = Nh_WebIDL_advanceParser(*Parser_p, 1); 
        NH_WEBIDL_END(Nh_WebIDL_initParseResult(PrimitiveType_p))
    }

    Nh_WebIDL_ParseResult UnsignedIntegerType = Nh_WebIDL_parseUnsignedIntegerType(Parser_p);
    if (UnsignedIntegerType.Node_p != NULL) 
    {
        Nh_WebIDL_ParseNode *PrimitiveType_p = Nh_WebIDL_allocateNonTerminalParseNode("PrimitiveType", 1);
        Nh_appendToList(&PrimitiveType_p->Children, UnsignedIntegerType.Node_p);
        NH_WEBIDL_END(Nh_WebIDL_initParseResult(PrimitiveType_p))
    }

    Nh_WebIDL_ParseResult UnrestrictedFloatType = Nh_WebIDL_parseUnrestrictedFloatType(Parser_p);
    if (UnrestrictedFloatType.Node_p != NULL) 
    {
        Nh_WebIDL_ParseNode *PrimitiveType_p = Nh_WebIDL_allocateNonTerminalParseNode("PrimitiveType", 1);
        Nh_appendToList(&PrimitiveType_p->Children, UnrestrictedFloatType.Node_p);
        NH_WEBIDL_END(Nh_WebIDL_initParseResult(PrimitiveType_p))
    }

NH_WEBIDL_END(Nh_WebIDL_initParseResult(NULL))
}

static Nh_WebIDL_ParseResult Nh_WebIDL_parseNull(
    Nh_WebIDL_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    Nh_WebIDL_ParseNode *Null_p = Nh_WebIDL_allocateNonTerminalParseNode("Null", 1);

    if (Parser_p->Tokens_p[0].String.bytes_p[0] == '?') {
        Nh_appendToList(&Null_p->Children, Nh_WebIDL_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
        *Parser_p = Nh_WebIDL_advanceParser(*Parser_p, 1);
    }

NH_WEBIDL_END(Nh_WebIDL_initParseResult(Null_p))
}

static Nh_WebIDL_ParseResult Nh_WebIDL_parseStringType(
    Nh_WebIDL_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    if (!strcmp(Parser_p->Tokens_p[0].String.bytes_p, "ByteString") 
    ||  !strcmp(Parser_p->Tokens_p[0].String.bytes_p, "DOMString") 
    ||  !strcmp(Parser_p->Tokens_p[0].String.bytes_p, "USVString")) 
    {
        Nh_WebIDL_ParseNode *StringType_p = Nh_WebIDL_allocateNonTerminalParseNode("StringType", 1);
        Nh_appendToList(&StringType_p->Children, Nh_WebIDL_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
        *Parser_p = Nh_WebIDL_advanceParser(*Parser_p, 1);
        NH_WEBIDL_END(Nh_WebIDL_initParseResult(StringType_p))
    }

NH_WEBIDL_END(Nh_WebIDL_initParseResult(NULL))
}

static Nh_WebIDL_ParseResult Nh_WebIDL_parseBufferRelatedType(
    Nh_WebIDL_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    if (Parser_p->unparsed == 0) {NH_WEBIDL_END(Nh_WebIDL_initParseResult(NULL))}
    
    if (!strcmp(Parser_p->Tokens_p[0].String.bytes_p, "ArrayBuffer")
    ||  !strcmp(Parser_p->Tokens_p[0].String.bytes_p, "DataView")
    ||  !strcmp(Parser_p->Tokens_p[0].String.bytes_p, "Int8Array")
    ||  !strcmp(Parser_p->Tokens_p[0].String.bytes_p, "Int16Array")
    ||  !strcmp(Parser_p->Tokens_p[0].String.bytes_p, "Int32Array")
    ||  !strcmp(Parser_p->Tokens_p[0].String.bytes_p, "Uint8Array")
    ||  !strcmp(Parser_p->Tokens_p[0].String.bytes_p, "Uint16Array")
    ||  !strcmp(Parser_p->Tokens_p[0].String.bytes_p, "Uint32Array")
    ||  !strcmp(Parser_p->Tokens_p[0].String.bytes_p, "Uint8ClampedArray")
    ||  !strcmp(Parser_p->Tokens_p[0].String.bytes_p, "Float32Array")
    ||  !strcmp(Parser_p->Tokens_p[0].String.bytes_p, "Float64Array"))
    {
        Nh_WebIDL_ParseNode *BufferRelatedType_p = Nh_WebIDL_allocateNonTerminalParseNode("BufferRelatedType", 1);
        Nh_appendToList(&BufferRelatedType_p->Children, Nh_WebIDL_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
        *Parser_p = Nh_WebIDL_advanceParser(*Parser_p, 1);
        NH_WEBIDL_END(Nh_WebIDL_initParseResult(BufferRelatedType_p))
    }

NH_WEBIDL_END(Nh_WebIDL_initParseResult(NULL))
}

static Nh_WebIDL_ParseResult Nh_WebIDL_parseRecordType(
    Nh_WebIDL_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    if (Parser_p->unparsed <= 4) {NH_WEBIDL_END(Nh_WebIDL_initParseResult(NULL))}

    if (!strcmp(Parser_p->Tokens_p[0].String.bytes_p, "record") && Parser_p->Tokens_p[1].String.bytes_p[0] == '<')
    {
        Nh_WebIDL_Parser LocalParser = Nh_WebIDL_advanceParser(*Parser_p, 2);
        Nh_WebIDL_ParseResult StringType = Nh_WebIDL_parseStringType(&LocalParser);

        if (StringType.Node_p != NULL && LocalParser.unparsed > 2 && LocalParser.Tokens_p[0].String.bytes_p[0] == ',')
        {
            Nh_WebIDL_Token *Comma_p = &LocalParser.Tokens_p[0];
            LocalParser = Nh_WebIDL_advanceParser(LocalParser, 1);
            Nh_WebIDL_ParseResult TypeWithExtendedAttributes = Nh_WebIDL_parseTypeWithExtendedAttributes(&LocalParser);

            if (TypeWithExtendedAttributes.Node_p != NULL && LocalParser.unparsed >= 1 && LocalParser.Tokens_p[0].String.bytes_p[0] == '>')
            {
                Nh_WebIDL_ParseNode *RecordType_p = Nh_WebIDL_allocateNonTerminalParseNode("RecordType", 6);
                Nh_appendToList(&RecordType_p->Children, Nh_WebIDL_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
                Nh_appendToList(&RecordType_p->Children, Nh_WebIDL_allocateTerminalParseNode(&Parser_p->Tokens_p[1]));
                Nh_appendToList(&RecordType_p->Children, StringType.Node_p);
                Nh_appendToList(&RecordType_p->Children, Nh_WebIDL_allocateTerminalParseNode(Comma_p));
                Nh_appendToList(&RecordType_p->Children, TypeWithExtendedAttributes.Node_p);
                Nh_appendToList(&RecordType_p->Children, Nh_WebIDL_allocateTerminalParseNode(&LocalParser.Tokens_p[0]));
                *Parser_p = Nh_WebIDL_advanceParser(LocalParser, 1);
                NH_WEBIDL_END(Nh_WebIDL_initParseResult(RecordType_p))
            }
        }
    }

NH_WEBIDL_END(Nh_WebIDL_initParseResult(NULL))
}

static Nh_WebIDL_ParseResult Nh_WebIDL_parseDistinguishableType(
    Nh_WebIDL_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    Nh_WebIDL_ParseResult PrimitiveType = Nh_WebIDL_parsePrimitiveType(Parser_p);
    if (PrimitiveType.Node_p != NULL) 
    {
        Nh_WebIDL_ParseNode *DistinguishableType_p = Nh_WebIDL_allocateNonTerminalParseNode("DistinguishableType", 1);
        Nh_appendToList(&DistinguishableType_p->Children, PrimitiveType.Node_p);
        Nh_WebIDL_ParseResult Null = Nh_WebIDL_parseNull(Parser_p);
        Nh_appendToList(&DistinguishableType_p->Children, Null.Node_p);
        NH_WEBIDL_END(Nh_WebIDL_initParseResult(DistinguishableType_p))
    } 
    
    Nh_WebIDL_ParseResult StringType = Nh_WebIDL_parseStringType(Parser_p);
    if (StringType.Node_p != NULL) 
    {
        Nh_WebIDL_ParseNode *DistinguishableType_p = Nh_WebIDL_allocateNonTerminalParseNode("DistinguishableType", 1);
        Nh_appendToList(&DistinguishableType_p->Children, StringType.Node_p);
        Nh_WebIDL_ParseResult Null = Nh_WebIDL_parseNull(Parser_p);
        Nh_appendToList(&DistinguishableType_p->Children, Null.Node_p);
        NH_WEBIDL_END(Nh_WebIDL_initParseResult(DistinguishableType_p))
    } 

    Nh_WebIDL_ParseResult BufferRelatedType = Nh_WebIDL_parseStringType(Parser_p);
    if (BufferRelatedType.Node_p != NULL) 
    {
        Nh_WebIDL_ParseNode *DistinguishableType_p = Nh_WebIDL_allocateNonTerminalParseNode("DistinguishableType", 1);
        Nh_appendToList(&DistinguishableType_p->Children, BufferRelatedType.Node_p);
        Nh_WebIDL_ParseResult Null = Nh_WebIDL_parseNull(Parser_p);
        Nh_appendToList(&DistinguishableType_p->Children, Null.Node_p);
        NH_WEBIDL_END(Nh_WebIDL_initParseResult(DistinguishableType_p))
    } 

    Nh_WebIDL_ParseResult RecordType = Nh_WebIDL_parseRecordType(Parser_p);
    if (RecordType.Node_p != NULL) 
    {
        Nh_WebIDL_ParseNode *DistinguishableType_p = Nh_WebIDL_allocateNonTerminalParseNode("DistinguishableType", 1);
        Nh_appendToList(&DistinguishableType_p->Children, RecordType.Node_p);
        Nh_WebIDL_ParseResult Null = Nh_WebIDL_parseNull(Parser_p);
        Nh_appendToList(&DistinguishableType_p->Children, Null.Node_p);
        NH_WEBIDL_END(Nh_WebIDL_initParseResult(DistinguishableType_p))
    } 

    if (Parser_p->unparsed > 3)
    {
        if (!strcmp(Parser_p->Tokens_p[0].String.bytes_p, "sequence") && Parser_p->Tokens_p[1].String.bytes_p[0] == '<'
        ||  !strcmp(Parser_p->Tokens_p[0].String.bytes_p, "FrozenArray") && Parser_p->Tokens_p[1].String.bytes_p[0] == '<'
        ||  !strcmp(Parser_p->Tokens_p[0].String.bytes_p, "ObservableArray") && Parser_p->Tokens_p[1].String.bytes_p[0] == '<')
        {
            Nh_WebIDL_Parser LocalParser = Nh_WebIDL_advanceParser(*Parser_p, 2);
            Nh_WebIDL_ParseResult TypeWithExtendedAttributes = Nh_WebIDL_parseTypeWithExtendedAttributes(&LocalParser);

            if (TypeWithExtendedAttributes.Node_p != NULL && LocalParser.unparsed > 0 && LocalParser.Tokens_p[0].String.bytes_p[0] == '>')
            {
                Nh_WebIDL_ParseNode *DistinguishableType_p = Nh_WebIDL_allocateNonTerminalParseNode("DistinguishableType", 5);
                Nh_appendToList(&DistinguishableType_p->Children, Nh_WebIDL_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
                Nh_appendToList(&DistinguishableType_p->Children, Nh_WebIDL_allocateTerminalParseNode(&Parser_p->Tokens_p[1]));
                Nh_appendToList(&DistinguishableType_p->Children, TypeWithExtendedAttributes.Node_p);
                Nh_appendToList(&DistinguishableType_p->Children, Nh_WebIDL_allocateTerminalParseNode(&LocalParser.Tokens_p[0]));
                *Parser_p = Nh_WebIDL_advanceParser(LocalParser, 1);
                Nh_WebIDL_ParseResult Null = Nh_WebIDL_parseNull(Parser_p);
                Nh_appendToList(&DistinguishableType_p->Children, Null.Node_p);
                NH_WEBIDL_END(Nh_WebIDL_initParseResult(DistinguishableType_p))
            }
        }
    }

    if (Parser_p->unparsed > 0 && Parser_p->Tokens_p[0].type == NH_WEBIDL_TOKEN_IDENTIFIER) 
    {
        Nh_WebIDL_ParseNode *DistinguishableType_p = Nh_WebIDL_allocateNonTerminalParseNode("DistinguishableType", 1);
        Nh_appendToList(&DistinguishableType_p->Children, Nh_WebIDL_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
        *Parser_p = Nh_WebIDL_advanceParser(*Parser_p, 1);
        Nh_WebIDL_ParseResult Null = Nh_WebIDL_parseNull(Parser_p);
        Nh_appendToList(&DistinguishableType_p->Children, Null.Node_p);
        NH_WEBIDL_END(Nh_WebIDL_initParseResult(DistinguishableType_p))
    }

NH_WEBIDL_END(Nh_WebIDL_initParseResult(NULL))
}

static Nh_WebIDL_ParseResult Nh_WebIDL_parsePromiseType(
    Nh_WebIDL_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    if (Parser_p->unparsed > 3 && !strcmp(Parser_p->Tokens_p[0].String.bytes_p, "Promise") && Parser_p->Tokens_p[1].String.bytes_p[0] == '<') 
    {
        Nh_WebIDL_Parser LocalParser = Nh_WebIDL_advanceParser(*Parser_p, 2);
        Nh_WebIDL_ParseResult Type = Nh_WebIDL_parseType(&LocalParser);

        if (Type.Node_p != NULL && LocalParser.unparsed > 0 && LocalParser.Tokens_p[0].String.bytes_p[0] == '>')
        {
            Nh_WebIDL_ParseNode *PromiseType_p = Nh_WebIDL_allocateNonTerminalParseNode("PromiseType", 4);
            Nh_appendToList(&PromiseType_p->Children, Nh_WebIDL_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
            Nh_appendToList(&PromiseType_p->Children, Nh_WebIDL_allocateTerminalParseNode(&Parser_p->Tokens_p[1]));
            Nh_appendToList(&PromiseType_p->Children, Type.Node_p);
            Nh_appendToList(&PromiseType_p->Children, Nh_WebIDL_allocateTerminalParseNode(&LocalParser.Tokens_p[0]));
            *Parser_p = Nh_WebIDL_advanceParser(LocalParser, 1);
            NH_WEBIDL_END(Nh_WebIDL_initParseResult(PromiseType_p))
        }
    }

NH_WEBIDL_END(Nh_WebIDL_initParseResult(NULL))
}

static Nh_WebIDL_ParseResult Nh_WebIDL_parseSingleType(
    Nh_WebIDL_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    Nh_WebIDL_ParseResult DistinguishableType = Nh_WebIDL_parseDistinguishableType(Parser_p);
    if (DistinguishableType.Node_p != NULL) 
    {
        Nh_WebIDL_ParseNode *SingleType_p = Nh_WebIDL_allocateNonTerminalParseNode("SingleType", 1);
        Nh_appendToList(&SingleType_p->Children, DistinguishableType.Node_p);
        NH_WEBIDL_END(Nh_WebIDL_initParseResult(SingleType_p))
    }
 
    if (Parser_p->unparsed > 0 && !strcmp(Parser_p->Tokens_p[0].String.bytes_p, "any"))
    {
        Nh_WebIDL_ParseNode *SingleType_p = Nh_WebIDL_allocateNonTerminalParseNode("SingleType", 1);
        Nh_appendToList(&SingleType_p->Children, Nh_WebIDL_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
        *Parser_p = Nh_WebIDL_advanceParser(*Parser_p, 1);
        NH_WEBIDL_END(Nh_WebIDL_initParseResult(SingleType_p))
    }

    Nh_WebIDL_ParseResult PromiseType = Nh_WebIDL_parsePromiseType(Parser_p);
    if (PromiseType.Node_p != NULL) 
    {
        Nh_WebIDL_ParseNode *SingleType_p = Nh_WebIDL_allocateNonTerminalParseNode("SingleType", 1);
        Nh_appendToList(&SingleType_p->Children, PromiseType.Node_p);
        NH_WEBIDL_END(Nh_WebIDL_initParseResult(SingleType_p))
    } 

NH_WEBIDL_END(Nh_WebIDL_initParseResult(NULL))
}

static Nh_WebIDL_ParseResult Nh_WebIDL_parseUnionType(
    Nh_WebIDL_Parser *Parser_p
);

static Nh_WebIDL_ParseResult Nh_WebIDL_parseUnionMemberType(
    Nh_WebIDL_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    Nh_WebIDL_ParseResult ExtendedAttributeList = Nh_WebIDL_parseExtendedAttributeList(Parser_p);
    Nh_WebIDL_ParseResult DistinguishableType = Nh_WebIDL_parseDistinguishableType(Parser_p);

    if (DistinguishableType.Node_p != NULL) 
    {
        Nh_WebIDL_ParseNode *UnionMemberType_p = Nh_WebIDL_allocateNonTerminalParseNode("UnionMemberType", 2);
        Nh_appendToList(&UnionMemberType_p->Children, ExtendedAttributeList.Node_p);
        Nh_appendToList(&UnionMemberType_p->Children, DistinguishableType.Node_p);
        NH_WEBIDL_END(Nh_WebIDL_initParseResult(UnionMemberType_p))
    }
    else {Nh_WebIDL_freeParseNode(DistinguishableType.Node_p);}

    Nh_WebIDL_ParseResult UnionType = Nh_WebIDL_parseUnionType(Parser_p);

    if (UnionType.Node_p != NULL)
    {
        Nh_WebIDL_ParseResult Null = Nh_WebIDL_parseNull(Parser_p);
        Nh_WebIDL_ParseNode *UnionMemberType_p = Nh_WebIDL_allocateNonTerminalParseNode("UnionMemberType", 2);
        Nh_appendToList(&UnionMemberType_p->Children, UnionType.Node_p);
        Nh_appendToList(&UnionMemberType_p->Children, Null.Node_p);
        NH_WEBIDL_END(Nh_WebIDL_initParseResult(UnionMemberType_p))
    }

NH_WEBIDL_END(Nh_WebIDL_initParseResult(NULL))
}

static Nh_WebIDL_ParseResult Nh_WebIDL_parseUnionMemberTypes(
    Nh_WebIDL_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    Nh_WebIDL_ParseNode *UnionMemberTypes_p = Nh_WebIDL_allocateNonTerminalParseNode("UnionMemberTypes", 3);

    if (Parser_p->unparsed > 0 && !strcmp(Parser_p->Tokens_p[0].String.bytes_p, "or")) 
    {
        Nh_WebIDL_Parser LocalParser = Nh_WebIDL_advanceParser(*Parser_p, 1);
        Nh_WebIDL_ParseResult UnionMemberType = Nh_WebIDL_parseUnionMemberType(&LocalParser);

        if (UnionMemberType.Node_p != NULL) 
        {
            Nh_WebIDL_ParseResult UnionMemberTypes = Nh_WebIDL_parseUnionMemberTypes(&LocalParser);
            Nh_appendToList(&UnionMemberTypes_p->Children, Nh_WebIDL_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
            Nh_appendToList(&UnionMemberTypes_p->Children, UnionMemberType.Node_p);
            Nh_appendToList(&UnionMemberTypes_p->Children, UnionMemberTypes.Node_p);
            *Parser_p = LocalParser;
        }
    }

NH_WEBIDL_END(Nh_WebIDL_initParseResult(UnionMemberTypes_p))
}

static Nh_WebIDL_ParseResult Nh_WebIDL_parseUnionType(
    Nh_WebIDL_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    if (Parser_p->unparsed > 0 && Parser_p->Tokens_p[0].String.bytes_p[0] == '(') 
    {
        Nh_WebIDL_Parser LocalParser = Nh_WebIDL_advanceParser(*Parser_p, 1);
        Nh_WebIDL_ParseResult UnionMemberType1 = Nh_WebIDL_parseUnionMemberType(&LocalParser);

        if (UnionMemberType1.Node_p != NULL && LocalParser.unparsed > 0 && !strcmp(LocalParser.Tokens_p[0].String.bytes_p, "or")) 
        {
            Nh_WebIDL_Token *OrToken_p = &LocalParser.Tokens_p[0];
            LocalParser = Nh_WebIDL_advanceParser(LocalParser, 1);
            Nh_WebIDL_ParseResult UnionMemberType2 = Nh_WebIDL_parseUnionMemberType(&LocalParser);

            if (UnionMemberType2.Node_p != NULL) 
            {
                Nh_WebIDL_ParseResult UnionMemberTypes = Nh_WebIDL_parseUnionMemberTypes(&LocalParser);
                if (LocalParser.unparsed > 0 && LocalParser.Tokens_p[0].String.bytes_p[0] == ')') 
                {
                    Nh_WebIDL_ParseNode *UnionType_p = Nh_WebIDL_allocateNonTerminalParseNode("UnionType", 6);
                    Nh_appendToList(&UnionType_p->Children, Nh_WebIDL_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
                    Nh_appendToList(&UnionType_p->Children, UnionMemberType1.Node_p);
                    Nh_appendToList(&UnionType_p->Children, Nh_WebIDL_allocateTerminalParseNode(OrToken_p));
                    Nh_appendToList(&UnionType_p->Children, UnionMemberType2.Node_p);
                    Nh_appendToList(&UnionType_p->Children, UnionMemberTypes.Node_p);
                    Nh_appendToList(&UnionType_p->Children, Nh_WebIDL_allocateTerminalParseNode(&LocalParser.Tokens_p[0]));
                    *Parser_p = Nh_WebIDL_advanceParser(LocalParser, 1);
                    NH_WEBIDL_END(Nh_WebIDL_initParseResult(UnionType_p))
                }
            }
        }
    }

NH_WEBIDL_END(Nh_WebIDL_initParseResult(NULL))
}

static Nh_WebIDL_ParseResult Nh_WebIDL_parseType(
    Nh_WebIDL_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    Nh_WebIDL_ParseResult SingleType = Nh_WebIDL_parseSingleType(Parser_p);
    if (SingleType.Node_p != NULL) 
    {
        Nh_WebIDL_ParseNode *Type_p = Nh_WebIDL_allocateNonTerminalParseNode("Type", 1);
        Nh_appendToList(&Type_p->Children, SingleType.Node_p);
        NH_WEBIDL_END(Nh_WebIDL_initParseResult(Type_p))
    } 
   
    Nh_WebIDL_ParseResult UnionType = Nh_WebIDL_parseUnionType(Parser_p);
    if (UnionType.Node_p != NULL) 
    {
        Nh_WebIDL_ParseResult Null = Nh_WebIDL_parseNull(Parser_p);
        Nh_WebIDL_ParseNode *Type_p = Nh_WebIDL_allocateNonTerminalParseNode("Type", 1);
        Nh_appendToList(&Type_p->Children, UnionType.Node_p);
        Nh_appendToList(&Type_p->Children, Null.Node_p);
        NH_WEBIDL_END(Nh_WebIDL_initParseResult(Type_p))
    } 

NH_WEBIDL_END(Nh_WebIDL_initParseResult(NULL))
}

static Nh_WebIDL_ParseResult Nh_WebIDL_parseTypeWithExtendedAttributes(
    Nh_WebIDL_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    Nh_WebIDL_Parser LocalParser = *Parser_p;

    Nh_WebIDL_ParseResult ExtendedAttributeList = Nh_WebIDL_parseExtendedAttributeList(&LocalParser);
    Nh_WebIDL_ParseResult Type = Nh_WebIDL_parseType(&LocalParser);

    if (Type.Node_p != NULL) 
    {
        Nh_WebIDL_ParseNode *TypeWithExtendedAttributes_p = Nh_WebIDL_allocateNonTerminalParseNode("TypeWithExtendedAttributes", 2);
        Nh_appendToList(&TypeWithExtendedAttributes_p->Children, ExtendedAttributeList.Node_p);
        Nh_appendToList(&TypeWithExtendedAttributes_p->Children, Type.Node_p);
        *Parser_p = LocalParser;
        NH_WEBIDL_END(Nh_WebIDL_initParseResult(TypeWithExtendedAttributes_p))
    }

    Nh_WebIDL_freeParseNode(ExtendedAttributeList.Node_p);

NH_WEBIDL_END(Nh_WebIDL_initParseResult(NULL))
}

static Nh_WebIDL_ParseResult Nh_WebIDL_parseArgumentNameKeyword(
    Nh_WebIDL_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    if (Parser_p->unparsed == 0) {NH_WEBIDL_END(Nh_WebIDL_initParseResult(NULL))}

    if (!strcmp(Parser_p->Tokens_p[0].String.bytes_p, "async")
    ||  !strcmp(Parser_p->Tokens_p[0].String.bytes_p, "attribute")
    ||  !strcmp(Parser_p->Tokens_p[0].String.bytes_p, "callback")
    ||  !strcmp(Parser_p->Tokens_p[0].String.bytes_p, "const")
    ||  !strcmp(Parser_p->Tokens_p[0].String.bytes_p, "constructor")
    ||  !strcmp(Parser_p->Tokens_p[0].String.bytes_p, "deleter")
    ||  !strcmp(Parser_p->Tokens_p[0].String.bytes_p, "dictionary")
    ||  !strcmp(Parser_p->Tokens_p[0].String.bytes_p, "enum")
    ||  !strcmp(Parser_p->Tokens_p[0].String.bytes_p, "getter")
    ||  !strcmp(Parser_p->Tokens_p[0].String.bytes_p, "includes")
    ||  !strcmp(Parser_p->Tokens_p[0].String.bytes_p, "inherit")
    ||  !strcmp(Parser_p->Tokens_p[0].String.bytes_p, "interface")
    ||  !strcmp(Parser_p->Tokens_p[0].String.bytes_p, "iterable")
    ||  !strcmp(Parser_p->Tokens_p[0].String.bytes_p, "maplike")
    ||  !strcmp(Parser_p->Tokens_p[0].String.bytes_p, "mixin")
    ||  !strcmp(Parser_p->Tokens_p[0].String.bytes_p, "namespace")
    ||  !strcmp(Parser_p->Tokens_p[0].String.bytes_p, "partial")
    ||  !strcmp(Parser_p->Tokens_p[0].String.bytes_p, "readonly")
    ||  !strcmp(Parser_p->Tokens_p[0].String.bytes_p, "required")
    ||  !strcmp(Parser_p->Tokens_p[0].String.bytes_p, "setlike")
    ||  !strcmp(Parser_p->Tokens_p[0].String.bytes_p, "setter")
    ||  !strcmp(Parser_p->Tokens_p[0].String.bytes_p, "static")
    ||  !strcmp(Parser_p->Tokens_p[0].String.bytes_p, "stringifier")
    ||  !strcmp(Parser_p->Tokens_p[0].String.bytes_p, "typedef")
    ||  !strcmp(Parser_p->Tokens_p[0].String.bytes_p, "unrestricted"))
    {
        Nh_WebIDL_ParseNode *ArgumentNameKeyword_p = Nh_WebIDL_allocateNonTerminalParseNode("ArgumentNameKeyword", 1);
        Nh_appendToList(&ArgumentNameKeyword_p->Children, Nh_WebIDL_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
        *Parser_p = Nh_WebIDL_advanceParser(*Parser_p, 1);
        NH_WEBIDL_END(Nh_WebIDL_initParseResult(ArgumentNameKeyword_p))
    }

NH_WEBIDL_END(Nh_WebIDL_initParseResult(NULL))
}

static Nh_WebIDL_ParseResult Nh_WebIDL_parseArgumentName(
    Nh_WebIDL_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    Nh_WebIDL_ParseResult ArgumentNameKeyword = Nh_WebIDL_parseArgumentNameKeyword(Parser_p);

    if (ArgumentNameKeyword.Node_p != NULL) 
    {
        Nh_WebIDL_ParseNode *ArgumentName_p = Nh_WebIDL_allocateNonTerminalParseNode("ArgumentName", 1);
        Nh_appendToList(&ArgumentName_p->Children, ArgumentNameKeyword.Node_p);
        NH_WEBIDL_END(Nh_WebIDL_initParseResult(ArgumentName_p))
    }

    if (Parser_p->unparsed > 0 && Parser_p->Tokens_p[0].type == NH_WEBIDL_TOKEN_IDENTIFIER) 
    {
        Nh_WebIDL_ParseNode *ArgumentName_p = Nh_WebIDL_allocateNonTerminalParseNode("ArgumentName", 1);
        Nh_appendToList(&ArgumentName_p->Children, Nh_WebIDL_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
        *Parser_p = Nh_WebIDL_advanceParser(*Parser_p, 1);
        NH_WEBIDL_END(Nh_WebIDL_initParseResult(ArgumentName_p))
    }

NH_WEBIDL_END(Nh_WebIDL_initParseResult(NULL))
}

static Nh_WebIDL_ParseResult Nh_WebIDL_parseBooleanLiteral(
    Nh_WebIDL_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    if (Parser_p->unparsed == 0) {NH_WEBIDL_END(Nh_WebIDL_initParseResult(NULL))}

    if (!strcmp(Parser_p->Tokens_p[0].String.bytes_p, "true")
    ||  !strcmp(Parser_p->Tokens_p[0].String.bytes_p, "false"))
    {
        Nh_WebIDL_ParseNode *BooleanLiteral_p = Nh_WebIDL_allocateNonTerminalParseNode("BooleanLiteral", 1);
        Nh_appendToList(&BooleanLiteral_p->Children, Nh_WebIDL_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
        *Parser_p = Nh_WebIDL_advanceParser(*Parser_p, 1);
        NH_WEBIDL_END(Nh_WebIDL_initParseResult(BooleanLiteral_p))
    }

NH_WEBIDL_END(Nh_WebIDL_initParseResult(NULL))
}

static Nh_WebIDL_ParseResult Nh_WebIDL_parseFloatLiteral(
    Nh_WebIDL_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    if (Parser_p->unparsed == 0) {NH_WEBIDL_END(Nh_WebIDL_initParseResult(NULL))}

    if (!strcmp(Parser_p->Tokens_p[0].String.bytes_p, "-Infinity")
    ||  !strcmp(Parser_p->Tokens_p[0].String.bytes_p, "Infinity")
    ||  !strcmp(Parser_p->Tokens_p[0].String.bytes_p, "NaN")
    ||  Nh_WebIDL_isNumericToken(&Parser_p->Tokens_p[0]))
    {
        Nh_WebIDL_ParseNode *FloatLiteral_p = Nh_WebIDL_allocateNonTerminalParseNode("FloatLiteral", 1);
        Nh_appendToList(&FloatLiteral_p->Children, Nh_WebIDL_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
        *Parser_p = Nh_WebIDL_advanceParser(*Parser_p, 1);
        NH_WEBIDL_END(Nh_WebIDL_initParseResult(FloatLiteral_p))
    }

NH_WEBIDL_END(Nh_WebIDL_initParseResult(NULL))
}

static Nh_WebIDL_ParseResult Nh_WebIDL_parseConstValue(
    Nh_WebIDL_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    if (Parser_p->unparsed == 0) {NH_WEBIDL_END(Nh_WebIDL_initParseResult(NULL))}

    Nh_WebIDL_ParseResult BooleanLiteral = Nh_WebIDL_parseBooleanLiteral(Parser_p);
    if (BooleanLiteral.Node_p != NULL) {
        Nh_WebIDL_ParseNode *ConstValue_p = Nh_WebIDL_allocateNonTerminalParseNode("ConstValue", 1);
        Nh_appendToList(&ConstValue_p->Children, BooleanLiteral.Node_p);
        NH_WEBIDL_END(Nh_WebIDL_initParseResult(ConstValue_p))
    }

    Nh_WebIDL_ParseResult FloatLiteral = Nh_WebIDL_parseFloatLiteral(Parser_p);
    if (FloatLiteral.Node_p != NULL) {
        Nh_WebIDL_ParseNode *ConstValue_p = Nh_WebIDL_allocateNonTerminalParseNode("ConstValue", 1);
        Nh_appendToList(&ConstValue_p->Children, FloatLiteral.Node_p);
        NH_WEBIDL_END(Nh_WebIDL_initParseResult(ConstValue_p))
    }

    if (Nh_WebIDL_isNumericToken(&Parser_p->Tokens_p[0])) 
    {
        Nh_WebIDL_ParseNode *ConstValue_p = Nh_WebIDL_allocateNonTerminalParseNode("ConstValue", 1);
        Nh_appendToList(&ConstValue_p->Children, Nh_WebIDL_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
        *Parser_p = Nh_WebIDL_advanceParser(*Parser_p, 1);
        NH_WEBIDL_END(Nh_WebIDL_initParseResult(ConstValue_p))
    }

NH_WEBIDL_END(Nh_WebIDL_initParseResult(NULL))
}

static Nh_WebIDL_ParseResult Nh_WebIDL_parseDefaultValue(
    Nh_WebIDL_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    if (Parser_p->unparsed == 0) {NH_WEBIDL_END(Nh_WebIDL_initParseResult(NULL))}

    Nh_WebIDL_ParseResult ConstValue = Nh_WebIDL_parseConstValue(Parser_p);
    if (ConstValue.Node_p != NULL) 
    {
        Nh_WebIDL_ParseNode *DefaultValue_p = Nh_WebIDL_allocateNonTerminalParseNode("DefaultValue", 1);
        Nh_appendToList(&DefaultValue_p->Children, ConstValue.Node_p);
        NH_WEBIDL_END(Nh_WebIDL_initParseResult(DefaultValue_p))
    }

    if (!strcmp(Parser_p->Tokens_p[0].String.bytes_p, "null")) 
    {
        Nh_WebIDL_ParseNode *DefaultValue_p = Nh_WebIDL_allocateNonTerminalParseNode("DefaultValue", 1);
        Nh_appendToList(&DefaultValue_p->Children, Nh_WebIDL_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
        *Parser_p = Nh_WebIDL_advanceParser(*Parser_p, 1);
        NH_WEBIDL_END(Nh_WebIDL_initParseResult(DefaultValue_p))
    }

    if (Parser_p->Tokens_p[0].type == NH_WEBIDL_TOKEN_STRING) 
    {
        Nh_WebIDL_ParseNode *DefaultValue_p = Nh_WebIDL_allocateNonTerminalParseNode("DefaultValue", 1);
        Nh_appendToList(&DefaultValue_p->Children, Nh_WebIDL_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
        *Parser_p = Nh_WebIDL_advanceParser(*Parser_p, 1);
        NH_WEBIDL_END(Nh_WebIDL_initParseResult(DefaultValue_p))
    }

    if (Parser_p->unparsed <= 1) {NH_WEBIDL_END(Nh_WebIDL_initParseResult(NULL))}

    if ((Parser_p->Tokens_p[0].String.bytes_p[0] == '[' && Parser_p->Tokens_p[1].String.bytes_p[0] == ']')
    ||  (Parser_p->Tokens_p[0].String.bytes_p[0] == '{' && Parser_p->Tokens_p[1].String.bytes_p[0] == '}'))
    {
        Nh_WebIDL_ParseNode *DefaultValue_p = Nh_WebIDL_allocateNonTerminalParseNode("DefaultValue", 1);
        Nh_appendToList(&DefaultValue_p->Children, Nh_WebIDL_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
        Nh_appendToList(&DefaultValue_p->Children, Nh_WebIDL_allocateTerminalParseNode(&Parser_p->Tokens_p[1]));
        *Parser_p = Nh_WebIDL_advanceParser(*Parser_p, 2);
        NH_WEBIDL_END(Nh_WebIDL_initParseResult(DefaultValue_p))
    }

NH_WEBIDL_END(Nh_WebIDL_initParseResult(NULL))
}

static Nh_WebIDL_ParseResult Nh_WebIDL_parseDefault(
    Nh_WebIDL_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    Nh_WebIDL_ParseNode *Default_p = Nh_WebIDL_allocateNonTerminalParseNode("Default", 1);

    if (Parser_p->unparsed > 0 && Parser_p->Tokens_p[0].String.bytes_p[0] == '=') 
    {
        Nh_WebIDL_Parser LocalParser = Nh_WebIDL_advanceParser(*Parser_p, 1);
        Nh_WebIDL_ParseResult DefaultValue = Nh_WebIDL_parseDefaultValue(&LocalParser);

        if (DefaultValue.Node_p != NULL) 
        {
            Nh_appendToList(&Default_p->Children, Nh_WebIDL_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
            Nh_appendToList(&Default_p->Children, DefaultValue.Node_p);
            *Parser_p = LocalParser;    
        }
    } 

NH_WEBIDL_END(Nh_WebIDL_initParseResult(Default_p))
}

static Nh_WebIDL_ParseResult Nh_WebIDL_parseEllipsis(
    Nh_WebIDL_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    Nh_WebIDL_ParseNode *Ellipsis_p = Nh_WebIDL_allocateNonTerminalParseNode("Ellipsis", 1);

    if (Parser_p->unparsed > 0 && !strcmp(Parser_p->Tokens_p[0].String.bytes_p, "...")) 
    {
        Nh_WebIDL_ParseNode *Terminal_p = Nh_WebIDL_allocateTerminalParseNode(&Parser_p->Tokens_p[0]);
        Nh_appendToList(&Ellipsis_p->Children, Terminal_p);
        *Parser_p = Nh_WebIDL_advanceParser(*Parser_p, 1);
    }

NH_WEBIDL_END(Nh_WebIDL_initParseResult(Ellipsis_p))
}

static Nh_WebIDL_ParseResult Nh_WebIDL_parseArgumentRest(
    Nh_WebIDL_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    if (Parser_p->unparsed > 0 && !strcmp("optional", Parser_p->Tokens_p[0].String.bytes_p)) 
    {
        Nh_WebIDL_Parser LocalParser = Nh_WebIDL_advanceParser(*Parser_p, 1); 
        Nh_WebIDL_ParseResult TypeWithExtendedAttributes = Nh_WebIDL_parseTypeWithExtendedAttributes(&LocalParser);

        if (TypeWithExtendedAttributes.Node_p != NULL) 
        {
            Nh_WebIDL_ParseResult ArgumentName = Nh_WebIDL_parseArgumentName(&LocalParser);

            if (ArgumentName.Node_p != NULL) 
            {
                Nh_WebIDL_ParseResult Default = Nh_WebIDL_parseDefault(&LocalParser);
                Nh_WebIDL_ParseNode *ArgumentRest_p = Nh_WebIDL_allocateNonTerminalParseNode("ArgumentRest", 4);
                Nh_appendToList(&ArgumentRest_p->Children, Nh_WebIDL_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
                Nh_appendToList(&ArgumentRest_p->Children, TypeWithExtendedAttributes.Node_p);
                Nh_appendToList(&ArgumentRest_p->Children, ArgumentName.Node_p);
                Nh_appendToList(&ArgumentRest_p->Children, Default.Node_p);
                *Parser_p = LocalParser;
                NH_WEBIDL_END(Nh_WebIDL_initParseResult(ArgumentRest_p))
            }
            Nh_WebIDL_freeParseNode(TypeWithExtendedAttributes.Node_p);
        }
        NH_WEBIDL_END(Nh_WebIDL_initParseResult(NULL))
    }

    Nh_WebIDL_Parser LocalParser = *Parser_p;
    Nh_WebIDL_ParseResult Type = Nh_WebIDL_parseType(&LocalParser);

    if (Type.Node_p != NULL) 
    {
        Nh_WebIDL_ParseResult Ellipsis = Nh_WebIDL_parseEllipsis(&LocalParser);
        Nh_WebIDL_ParseResult ArgumentName = Nh_WebIDL_parseArgumentName(&LocalParser);

        if (ArgumentName.Node_p != NULL) 
        {
            Nh_WebIDL_ParseNode *ArgumentRest_p = Nh_WebIDL_allocateNonTerminalParseNode("ArgumentRest", 3);
            Nh_appendToList(&ArgumentRest_p->Children, Type.Node_p);
            Nh_appendToList(&ArgumentRest_p->Children, Ellipsis.Node_p);
            Nh_appendToList(&ArgumentRest_p->Children, ArgumentName.Node_p);
            *Parser_p = LocalParser;
            NH_WEBIDL_END(Nh_WebIDL_initParseResult(ArgumentRest_p))
        }

        Nh_WebIDL_freeParseNode(Type.Node_p);
        Nh_WebIDL_freeParseNode(Ellipsis.Node_p);
    }

NH_WEBIDL_END(Nh_WebIDL_initParseResult(NULL))
}

static Nh_WebIDL_ParseResult Nh_WebIDL_parseArgument(
    Nh_WebIDL_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    Nh_WebIDL_Parser LocalParser = *Parser_p;

    Nh_WebIDL_ParseResult ExtendedAttributeList = Nh_WebIDL_parseExtendedAttributeList(&LocalParser);
    Nh_WebIDL_ParseResult ArgumentRest = Nh_WebIDL_parseArgumentRest(&LocalParser);

    if (ArgumentRest.Node_p != NULL) 
    {
        Nh_WebIDL_ParseNode *Argument_p = Nh_WebIDL_allocateNonTerminalParseNode("Argument", 2);
        Nh_appendToList(&Argument_p->Children, ExtendedAttributeList.Node_p);
        Nh_appendToList(&Argument_p->Children, ArgumentRest.Node_p);
        *Parser_p = LocalParser;
        NH_WEBIDL_END(Nh_WebIDL_initParseResult(Argument_p))
    }

NH_WEBIDL_END(Nh_WebIDL_initParseResult(NULL))
}

static Nh_WebIDL_ParseResult Nh_WebIDL_parseArguments(
    Nh_WebIDL_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    Nh_WebIDL_ParseNode *Arguments_p = Nh_WebIDL_allocateNonTerminalParseNode("Arguments", 3);

    if (Parser_p->unparsed > 0 && Parser_p->Tokens_p[0].String.bytes_p[0] == ',') 
    {
        Nh_WebIDL_Parser LocalParser = Nh_WebIDL_advanceParser(*Parser_p, 1);
        Nh_WebIDL_ParseResult Argument = Nh_WebIDL_parseArgument(&LocalParser);

        if (Argument.Node_p != NULL) 
        {
            Nh_WebIDL_ParseResult Arguments = Nh_WebIDL_parseArguments(&LocalParser);
            Nh_appendToList(&Arguments_p->Children, Nh_WebIDL_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
            Nh_appendToList(&Arguments_p->Children, Argument.Node_p);
            Nh_appendToList(&Arguments_p->Children, Arguments.Node_p);
            *Parser_p = LocalParser;
        }
    }

NH_WEBIDL_END(Nh_WebIDL_initParseResult(Arguments_p))
}

static Nh_WebIDL_ParseResult Nh_WebIDL_parseArgumentList(
    Nh_WebIDL_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    Nh_WebIDL_ParseNode *ArgumentList_p = Nh_WebIDL_allocateNonTerminalParseNode("ArgumentList", 2);

    Nh_WebIDL_ParseResult Argument = Nh_WebIDL_parseArgument(Parser_p);
    if (Argument.Node_p != NULL) 
    {
        Nh_WebIDL_ParseResult Arguments = Nh_WebIDL_parseArguments(Parser_p);
        Nh_appendToList(&ArgumentList_p->Children, Argument.Node_p);
        Nh_appendToList(&ArgumentList_p->Children, Arguments.Node_p);
    } 
   
NH_WEBIDL_END(Nh_WebIDL_initParseResult(ArgumentList_p))
}

static Nh_WebIDL_ParseResult Nh_WebIDL_parseCallbackRest(
    Nh_WebIDL_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    if (Parser_p->unparsed > 0 && Parser_p->Tokens_p[0].type == NH_WEBIDL_TOKEN_IDENTIFIER) 
    {
        Nh_WebIDL_Parser LocalParser = Nh_WebIDL_advanceParser(*Parser_p, 1);

        if (LocalParser.unparsed > 0 && LocalParser.Tokens_p[0].String.bytes_p[0] == '=') 
        {
            Nh_WebIDL_Token *Equal_p = &LocalParser.Tokens_p[0];
            LocalParser = Nh_WebIDL_advanceParser(LocalParser, 1);
            Nh_WebIDL_ParseResult Type = Nh_WebIDL_parseType(&LocalParser);

            if (Type.Node_p != NULL && LocalParser.unparsed > 0 && LocalParser.Tokens_p[0].String.bytes_p[0] == '(') 
            {
                Nh_WebIDL_Token *LeftBracket_p = &LocalParser.Tokens_p[0];
                LocalParser = Nh_WebIDL_advanceParser(LocalParser, 1);
                Nh_WebIDL_ParseResult ArgumentList = Nh_WebIDL_parseArgumentList(&LocalParser);

                if (LocalParser.unparsed > 1 
                &&  LocalParser.Tokens_p[0].String.bytes_p[0] == ')'
                &&  LocalParser.Tokens_p[1].String.bytes_p[0] == ';')
                {
                    Nh_WebIDL_ParseNode *CallbackRest_p = Nh_WebIDL_allocateNonTerminalParseNode("CallbackRest", 1);
                    Nh_appendToList(&CallbackRest_p->Children, Nh_WebIDL_allocateTerminalParseNode(&Parser_p->Tokens_p[0])); 
                    Nh_appendToList(&CallbackRest_p->Children, Nh_WebIDL_allocateTerminalParseNode(Equal_p)); 
                    Nh_appendToList(&CallbackRest_p->Children, Type.Node_p); 
                    Nh_appendToList(&CallbackRest_p->Children, Nh_WebIDL_allocateTerminalParseNode(LeftBracket_p)); 
                    Nh_appendToList(&CallbackRest_p->Children, ArgumentList.Node_p); 
                    Nh_appendToList(&CallbackRest_p->Children, Nh_WebIDL_allocateTerminalParseNode(&LocalParser.Tokens_p[0])); 
                    Nh_appendToList(&CallbackRest_p->Children, Nh_WebIDL_allocateTerminalParseNode(&LocalParser.Tokens_p[1])); 
                    *Parser_p = Nh_WebIDL_advanceParser(LocalParser, 2);
                    NH_WEBIDL_END(Nh_WebIDL_initParseResult(CallbackRest_p))
                }

                Nh_WebIDL_freeParseNode(ArgumentList.Node_p);
            }

            Nh_WebIDL_freeParseNode(Type.Node_p);
        }
    }

NH_WEBIDL_END(Nh_WebIDL_initParseResult(NULL))
}

static Nh_WebIDL_ParseResult Nh_WebIDL_parseConstType(
    Nh_WebIDL_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    Nh_WebIDL_ParseResult PrimitiveType = Nh_WebIDL_parsePrimitiveType(Parser_p);
 
    if (PrimitiveType.Node_p != NULL) {
        Nh_WebIDL_ParseNode *ConstType_p = Nh_WebIDL_allocateNonTerminalParseNode("ConstType", 1);
        Nh_appendToList(&ConstType_p->Children, PrimitiveType.Node_p);
        NH_WEBIDL_END(Nh_WebIDL_initParseResult(ConstType_p))
    }

    if (Parser_p->unparsed > 0 && Parser_p->Tokens_p[0].type == NH_WEBIDL_TOKEN_IDENTIFIER) {
        Nh_WebIDL_ParseNode *ConstType_p = Nh_WebIDL_allocateNonTerminalParseNode("ConstType", 1);
        Nh_appendToList(&ConstType_p->Children, Nh_WebIDL_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
        NH_WEBIDL_END(Nh_WebIDL_initParseResult(ConstType_p))
    }

NH_WEBIDL_END(Nh_WebIDL_initParseResult(NULL))
}

static Nh_WebIDL_ParseResult Nh_WebIDL_parseConst(
    Nh_WebIDL_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    if (Parser_p->unparsed <= 0 || strcmp(Parser_p->Tokens_p[0].String.bytes_p, "const")) {
        NH_WEBIDL_END(Nh_WebIDL_initParseResult(NULL))
    }

    Nh_WebIDL_Parser LocalParser = Nh_WebIDL_advanceParser(*Parser_p, 1);
    Nh_WebIDL_ParseResult ConstType = Nh_WebIDL_parseConstType(&LocalParser);
 
    if (ConstType.Node_p != NULL) 
    {
        if (LocalParser.unparsed > 0 && LocalParser.Tokens_p[0].type == NH_WEBIDL_TOKEN_IDENTIFIER) 
        {
            Nh_WebIDL_Token *IdentifierToken_p = &LocalParser.Tokens_p[0];
            LocalParser = Nh_WebIDL_advanceParser(LocalParser, 1);   

            if (LocalParser.unparsed > 0 && LocalParser.Tokens_p[0].String.bytes_p[0] == '=') 
            {
                Nh_WebIDL_Token *EqualToken_p = &LocalParser.Tokens_p[0];
                LocalParser = Nh_WebIDL_advanceParser(LocalParser, 1);   

                Nh_WebIDL_ParseResult ConstValue = Nh_WebIDL_parseConstValue(&LocalParser);

                if (ConstValue.Node_p != NULL && LocalParser.unparsed > 0 && LocalParser.Tokens_p[0].String.bytes_p[0] == ';') 
                {
                    Nh_WebIDL_ParseNode *Const_p = Nh_WebIDL_allocateNonTerminalParseNode("Const", 6);
                    Nh_appendToList(&Const_p->Children, Nh_WebIDL_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
                    Nh_appendToList(&Const_p->Children, ConstType.Node_p);
                    Nh_appendToList(&Const_p->Children, Nh_WebIDL_allocateTerminalParseNode(IdentifierToken_p));
                    Nh_appendToList(&Const_p->Children, Nh_WebIDL_allocateTerminalParseNode(EqualToken_p));
                    Nh_appendToList(&Const_p->Children, ConstValue.Node_p);
                    Nh_appendToList(&Const_p->Children, Nh_WebIDL_allocateTerminalParseNode(&LocalParser.Tokens_p[0]));
                    *Parser_p = Nh_WebIDL_advanceParser(LocalParser, 1);
                    NH_WEBIDL_END(Nh_WebIDL_initParseResult(Const_p))
                }

                Nh_WebIDL_freeParseNode(ConstValue.Node_p);
            }
        }

        Nh_WebIDL_freeParseNode(ConstType.Node_p);
    }

NH_WEBIDL_END(Nh_WebIDL_initParseResult(NULL))
}

static Nh_WebIDL_ParseResult Nh_WebIDL_parseOperationNameKeyword(
    Nh_WebIDL_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    if (Parser_p->unparsed > 0 && !strcmp(Parser_p->Tokens_p[0].String.bytes_p, "includes")) 
    {
        Nh_WebIDL_ParseNode *OperationNameKeyword_p = Nh_WebIDL_allocateNonTerminalParseNode("OperationNameKeyword", 1);
        Nh_appendToList(&OperationNameKeyword_p->Children, Nh_WebIDL_allocateTerminalParseNode(&Parser_p->Tokens_p[0])); 
        *Parser_p = Nh_WebIDL_advanceParser(*Parser_p, 1);
        NH_WEBIDL_END(Nh_WebIDL_initParseResult(OperationNameKeyword_p))
    }

NH_WEBIDL_END(Nh_WebIDL_initParseResult(NULL))
}

static Nh_WebIDL_ParseResult Nh_WebIDL_parseOperationName(
    Nh_WebIDL_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    Nh_WebIDL_ParseResult OperationNameKeyword = Nh_WebIDL_parseOperationNameKeyword(Parser_p);
    if (OperationNameKeyword.Node_p != NULL) {
        Nh_WebIDL_ParseNode *OperationName_p = Nh_WebIDL_allocateNonTerminalParseNode("OperationName", 1);
        Nh_appendToList(&OperationName_p->Children, OperationNameKeyword.Node_p);
        NH_WEBIDL_END(Nh_WebIDL_initParseResult(OperationName_p))
    }

    if (Parser_p->unparsed > 0 && Parser_p->Tokens_p[0].type == NH_WEBIDL_TOKEN_IDENTIFIER) {
        Nh_WebIDL_ParseNode *OperationName_p = Nh_WebIDL_allocateNonTerminalParseNode("OperationName", 1);
        Nh_appendToList(&OperationName_p->Children, Nh_WebIDL_allocateTerminalParseNode(&Parser_p->Tokens_p[0])); 
        *Parser_p = Nh_WebIDL_advanceParser(*Parser_p, 1);
        NH_WEBIDL_END(Nh_WebIDL_initParseResult(OperationName_p))
    }

NH_WEBIDL_END(Nh_WebIDL_initParseResult(NULL))
}

static Nh_WebIDL_ParseResult Nh_WebIDL_parseOptionalOperationName(
    Nh_WebIDL_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    Nh_WebIDL_ParseNode *OptionalOperationName_p = Nh_WebIDL_allocateNonTerminalParseNode("OptionalOperationName", 1);

    Nh_WebIDL_ParseResult OperationName = Nh_WebIDL_parseOperationName(Parser_p);
    if (OperationName.Node_p != NULL) {
        Nh_appendToList(&OptionalOperationName_p->Children, OperationName.Node_p);
    }

NH_WEBIDL_END(Nh_WebIDL_initParseResult(OptionalOperationName_p))
}

static Nh_WebIDL_ParseResult Nh_WebIDL_parseOperationRest(
    Nh_WebIDL_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    Nh_WebIDL_Parser LocalParser = *Parser_p;
    Nh_WebIDL_ParseResult OptionalOperationName = Nh_WebIDL_parseOptionalOperationName(&LocalParser);

    if (LocalParser.unparsed > 0 && LocalParser.Tokens_p[0].String.bytes_p[0] == '(') 
    {
        Nh_WebIDL_Token *LeftBracket_p = &LocalParser.Tokens_p[0];
        LocalParser = Nh_WebIDL_advanceParser(LocalParser, 1);
        Nh_WebIDL_ParseResult ArgumentList = Nh_WebIDL_parseArgumentList(&LocalParser);

        if (LocalParser.unparsed > 1 
        &&  LocalParser.Tokens_p[0].String.bytes_p[0] == ')'
        &&  LocalParser.Tokens_p[1].String.bytes_p[0] == ';')
        {
            Nh_WebIDL_ParseNode *OperationRest_p = Nh_WebIDL_allocateNonTerminalParseNode("OperationRest", 5);
            Nh_appendToList(&OperationRest_p->Children, OptionalOperationName.Node_p); 
            Nh_appendToList(&OperationRest_p->Children, Nh_WebIDL_allocateTerminalParseNode(LeftBracket_p));
            Nh_appendToList(&OperationRest_p->Children, ArgumentList.Node_p);
            Nh_appendToList(&OperationRest_p->Children, Nh_WebIDL_allocateTerminalParseNode(&LocalParser.Tokens_p[0]));
            Nh_appendToList(&OperationRest_p->Children, Nh_WebIDL_allocateTerminalParseNode(&LocalParser.Tokens_p[1]));
            *Parser_p = Nh_WebIDL_advanceParser(LocalParser, 2);
            NH_WEBIDL_END(Nh_WebIDL_initParseResult(OperationRest_p))
        }
    }

NH_WEBIDL_END(Nh_WebIDL_initParseResult(NULL))
}

static Nh_WebIDL_ParseResult Nh_WebIDL_parseRegularOperation(
    Nh_WebIDL_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    Nh_WebIDL_Parser LocalParser = *Parser_p;
    Nh_WebIDL_ParseResult Type = Nh_WebIDL_parseType(&LocalParser);

    if (Type.Node_p != NULL) 
    {
        Nh_WebIDL_ParseResult OperationRest = Nh_WebIDL_parseOperationRest(&LocalParser);

        if (OperationRest.Node_p != NULL) 
        {
            Nh_WebIDL_ParseNode *RegularOperation_p = Nh_WebIDL_allocateNonTerminalParseNode("RegularOperation", 2);
            Nh_appendToList(&RegularOperation_p->Children, Type.Node_p);
            Nh_appendToList(&RegularOperation_p->Children, OperationRest.Node_p);
            *Parser_p = LocalParser;
            NH_WEBIDL_END(Nh_WebIDL_initParseResult(RegularOperation_p))
        }

        Nh_WebIDL_freeParseNode(Type.Node_p);
    }

NH_WEBIDL_END(Nh_WebIDL_initParseResult(NULL))
}

static Nh_WebIDL_ParseResult Nh_WebIDL_parseCallbackInterfaceMember(
    Nh_WebIDL_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    Nh_WebIDL_ParseResult Const = Nh_WebIDL_parseConst(Parser_p);
    if (Const.Node_p != NULL) 
    {
        Nh_WebIDL_ParseNode *CallbackInterfaceMember_p = Nh_WebIDL_allocateNonTerminalParseNode("CallbackInterfaceMember", 1);
        Nh_appendToList(&CallbackInterfaceMember_p->Children, Const.Node_p);
        NH_WEBIDL_END(Nh_WebIDL_initParseResult(CallbackInterfaceMember_p))
    }

    Nh_WebIDL_ParseResult RegularOperation = Nh_WebIDL_parseRegularOperation(Parser_p);
    if (RegularOperation.Node_p != NULL) 
    {
        Nh_WebIDL_ParseNode *CallbackInterfaceMember_p = Nh_WebIDL_allocateNonTerminalParseNode("CallbackInterfaceMember", 1);
        Nh_appendToList(&CallbackInterfaceMember_p->Children, RegularOperation.Node_p);
        NH_WEBIDL_END(Nh_WebIDL_initParseResult(CallbackInterfaceMember_p))
    }

NH_WEBIDL_END(Nh_WebIDL_initParseResult(NULL))
}

static Nh_WebIDL_ParseResult Nh_WebIDL_parseCallbackInterfaceMembers(
    Nh_WebIDL_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    Nh_WebIDL_ParseNode *CallbackInterfaceMembers_p = Nh_WebIDL_allocateNonTerminalParseNode("CallbackInterfaceMembers", 1);

    Nh_WebIDL_ParseResult ExtendedAttributeList = Nh_WebIDL_parseExtendedAttributeList(Parser_p);
    Nh_WebIDL_ParseResult CallbackInterfaceMember = Nh_WebIDL_parseCallbackInterfaceMember(Parser_p);

    if (CallbackInterfaceMember.Node_p != NULL) {
        Nh_WebIDL_ParseResult CallbackInterfaceMembers = Nh_WebIDL_parseCallbackInterfaceMembers(Parser_p);
        Nh_appendToList(&CallbackInterfaceMembers_p->Children, ExtendedAttributeList.Node_p);
        Nh_appendToList(&CallbackInterfaceMembers_p->Children, CallbackInterfaceMember.Node_p);
        Nh_appendToList(&CallbackInterfaceMembers_p->Children, CallbackInterfaceMembers.Node_p);
    }
    else {Nh_WebIDL_freeParseNode(ExtendedAttributeList.Node_p);}

NH_WEBIDL_END(Nh_WebIDL_initParseResult(CallbackInterfaceMembers_p))
}

static Nh_WebIDL_ParseResult Nh_WebIDL_parseCallbackRestOrInterface(
    Nh_WebIDL_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    Nh_WebIDL_ParseResult CallbackRest = Nh_WebIDL_parseCallbackRest(Parser_p);

    if (CallbackRest.Node_p != NULL)
    {
        Nh_WebIDL_ParseNode *CallbackRestOrInterface_p = Nh_WebIDL_allocateNonTerminalParseNode("CallbackRestOrInterface", 1);
        Nh_appendToList(&CallbackRestOrInterface_p->Children, CallbackRest.Node_p);
        NH_WEBIDL_END(Nh_WebIDL_initParseResult(CallbackRestOrInterface_p))
    }
    
    if (Parser_p->unparsed > 0 && !strcmp(Parser_p->Tokens_p[0].String.bytes_p, "interface")) 
    {
        Nh_WebIDL_Parser LocalParser = Nh_WebIDL_advanceParser(*Parser_p, 1);

        if (LocalParser.unparsed > 0 && LocalParser.Tokens_p[0].type == NH_WEBIDL_TOKEN_IDENTIFIER) 
        {
            Nh_WebIDL_Token *Identifier_p = &LocalParser.Tokens_p[0];
            LocalParser = Nh_WebIDL_advanceParser(LocalParser, 1);

            if (LocalParser.unparsed > 0 && LocalParser.Tokens_p[0].String.bytes_p[0] == '{') 
            {
                Nh_WebIDL_Token *LeftBracket_p = &LocalParser.Tokens_p[0];
                LocalParser = Nh_WebIDL_advanceParser(LocalParser, 1);

                Nh_WebIDL_ParseResult CallbackInterfaceMembers = Nh_WebIDL_parseCallbackInterfaceMembers(&LocalParser);

                if (LocalParser.unparsed > 1 
                &&  LocalParser.Tokens_p[0].String.bytes_p[0] == '}'
                &&  LocalParser.Tokens_p[1].String.bytes_p[0] == ';') 
                {
                    Nh_WebIDL_ParseNode *CallbackRestOrInterface_p = Nh_WebIDL_allocateNonTerminalParseNode("CallbackRestOrInterface", 6);
                    Nh_appendToList(&CallbackRestOrInterface_p->Children, Nh_WebIDL_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
                    Nh_appendToList(&CallbackRestOrInterface_p->Children, Nh_WebIDL_allocateTerminalParseNode(Identifier_p));
                    Nh_appendToList(&CallbackRestOrInterface_p->Children, Nh_WebIDL_allocateTerminalParseNode(LeftBracket_p));
                    Nh_appendToList(&CallbackRestOrInterface_p->Children, CallbackInterfaceMembers.Node_p);
                    Nh_appendToList(&CallbackRestOrInterface_p->Children, Nh_WebIDL_allocateTerminalParseNode(&LocalParser.Tokens_p[0]));
                    Nh_appendToList(&CallbackRestOrInterface_p->Children, Nh_WebIDL_allocateTerminalParseNode(&LocalParser.Tokens_p[1]));
                    *Parser_p = Nh_WebIDL_advanceParser(LocalParser, 2);
                    NH_WEBIDL_END(Nh_WebIDL_initParseResult(CallbackRestOrInterface_p))
                }
                else {Nh_WebIDL_freeParseNode(CallbackInterfaceMembers.Node_p);} 
            }
        }
    }

NH_WEBIDL_END(Nh_WebIDL_initParseResult(NULL))
}

static Nh_WebIDL_ParseResult Nh_WebIDL_parseNON_STANDARD_Specifier(
    Nh_WebIDL_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    Nh_WebIDL_ParseNode *Specifier_p = Nh_WebIDL_allocateNonTerminalParseNode("NON_STANDARD_Specifier", 2);

    if (Parser_p->unparsed > 1 && Parser_p->Tokens_p[0].String.bytes_p[0] == '@' && Parser_p->Tokens_p[1].type == NH_WEBIDL_TOKEN_IDENTIFIER)
    {
        Nh_appendToList(&Specifier_p->Children, Nh_WebIDL_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
        Nh_appendToList(&Specifier_p->Children, Nh_WebIDL_allocateTerminalParseNode(&Parser_p->Tokens_p[1]));

        *Parser_p = Nh_WebIDL_advanceParser(*Parser_p, 2);
    }

NH_WEBIDL_END(Nh_WebIDL_initParseResult(Specifier_p))
}

static Nh_WebIDL_ParseResult Nh_WebIDL_parseInheritance(
    Nh_WebIDL_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    Nh_WebIDL_ParseNode *Inheritance_p = Nh_WebIDL_allocateNonTerminalParseNode("Inheritance", 3);

    if (Parser_p->unparsed > 1 && Parser_p->Tokens_p[0].String.bytes_p[0] == ':' && Parser_p->Tokens_p[1].type == NH_WEBIDL_TOKEN_IDENTIFIER)
    {
        Nh_appendToList(&Inheritance_p->Children, Nh_WebIDL_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
        Nh_appendToList(&Inheritance_p->Children, Nh_WebIDL_allocateTerminalParseNode(&Parser_p->Tokens_p[1]));

        *Parser_p = Nh_WebIDL_advanceParser(*Parser_p, 2);

        Nh_WebIDL_ParseResult Specifier = Nh_WebIDL_parseNON_STANDARD_Specifier(Parser_p);
        Nh_appendToList(&Inheritance_p->Children, Specifier.Node_p);
    }

NH_WEBIDL_END(Nh_WebIDL_initParseResult(Inheritance_p))
}

static Nh_WebIDL_ParseResult Nh_WebIDL_parseSpecial(
    Nh_WebIDL_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    if (Parser_p->unparsed == 0) {NH_WEBIDL_END(Nh_WebIDL_initParseResult(NULL))}

    if (!strcmp(Parser_p->Tokens_p[0].String.bytes_p, "getter")
    ||  !strcmp(Parser_p->Tokens_p[0].String.bytes_p, "setter")
    ||  !strcmp(Parser_p->Tokens_p[0].String.bytes_p, "deleter"))
    {
        Nh_WebIDL_ParseNode *Special_p = Nh_WebIDL_allocateNonTerminalParseNode("Special", 1);
        Nh_appendToList(&Special_p->Children, Nh_WebIDL_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
        *Parser_p = Nh_WebIDL_advanceParser(*Parser_p, 1);
        NH_WEBIDL_END(Nh_WebIDL_initParseResult(Special_p))
    }

NH_WEBIDL_END(Nh_WebIDL_initParseResult(NULL))
}

static Nh_WebIDL_ParseResult Nh_WebIDL_parseSpecialOperation(
    Nh_WebIDL_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    Nh_WebIDL_ParseResult Special = Nh_WebIDL_parseSpecial(Parser_p);
    if (Special.Node_p != NULL) 
    {
        Nh_WebIDL_ParseResult RegularOperation = Nh_WebIDL_parseRegularOperation(Parser_p);
        if (RegularOperation.Node_p != NULL)
        {
            Nh_WebIDL_ParseNode *SpecialOperation_p = Nh_WebIDL_allocateNonTerminalParseNode("SpecialOperation", 2);
            Nh_appendToList(&SpecialOperation_p->Children, Special.Node_p);
            Nh_appendToList(&SpecialOperation_p->Children, RegularOperation.Node_p);
            NH_WEBIDL_END(Nh_WebIDL_initParseResult(SpecialOperation_p))
        }
        Nh_WebIDL_freeParseNode(Special.Node_p);
    }

NH_WEBIDL_END(Nh_WebIDL_initParseResult(NULL))
}

static Nh_WebIDL_ParseResult Nh_WebIDL_parseOperation(
    Nh_WebIDL_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    Nh_WebIDL_ParseResult RegularOperation = Nh_WebIDL_parseRegularOperation(Parser_p);
    if (RegularOperation.Node_p != NULL) 
    {
        Nh_WebIDL_ParseNode *Operation_p = Nh_WebIDL_allocateNonTerminalParseNode("Operation", 1);
        Nh_appendToList(&Operation_p->Children, RegularOperation.Node_p);
        NH_WEBIDL_END(Nh_WebIDL_initParseResult(Operation_p))
    }

    Nh_WebIDL_ParseResult SpecialOperation = Nh_WebIDL_parseSpecialOperation(Parser_p);
    if (SpecialOperation.Node_p != NULL) 
    {
        Nh_WebIDL_ParseNode *Operation_p = Nh_WebIDL_allocateNonTerminalParseNode("Operation", 1);
        Nh_appendToList(&Operation_p->Children, SpecialOperation.Node_p);
        NH_WEBIDL_END(Nh_WebIDL_initParseResult(Operation_p))
    }

NH_WEBIDL_END(Nh_WebIDL_initParseResult(NULL))
}

static Nh_WebIDL_ParseResult Nh_WebIDL_parseOptionalReadOnly(
    Nh_WebIDL_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    Nh_WebIDL_ParseNode *OptionalReadOnly_p = Nh_WebIDL_allocateNonTerminalParseNode("OptionalReadOnly", 1);

    if (!strcmp(Parser_p->Tokens_p[0].String.bytes_p, "readonly"))
    {
        Nh_appendToList(&OptionalReadOnly_p->Children, Nh_WebIDL_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
        *Parser_p = Nh_WebIDL_advanceParser(*Parser_p, 1);
    }

NH_WEBIDL_END(Nh_WebIDL_initParseResult(OptionalReadOnly_p))
}

static Nh_WebIDL_ParseResult Nh_WebIDL_parseAttributeNameKeyword(
    Nh_WebIDL_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    if (Parser_p->unparsed <= 0) {NH_WEBIDL_END(Nh_WebIDL_initParseResult(NULL))}

    if (!strcmp(Parser_p->Tokens_p[0].String.bytes_p, "async")
    ||  !strcmp(Parser_p->Tokens_p[0].String.bytes_p, "required"))
    {
        Nh_WebIDL_ParseNode *AttributeNameKeyword_p = Nh_WebIDL_allocateNonTerminalParseNode("AttributeNameKeyword", 1);
        Nh_appendToList(&AttributeNameKeyword_p->Children, Nh_WebIDL_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
        *Parser_p = Nh_WebIDL_advanceParser(*Parser_p, 1);
        NH_WEBIDL_END(Nh_WebIDL_initParseResult(AttributeNameKeyword_p))
    }
    
NH_WEBIDL_END(Nh_WebIDL_initParseResult(NULL))
}

static Nh_WebIDL_ParseResult Nh_WebIDL_parseAttributeName(
    Nh_WebIDL_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    Nh_WebIDL_ParseResult AttributeNameKeyword = Nh_WebIDL_parseAttributeNameKeyword(Parser_p);
    if (AttributeNameKeyword.Node_p != NULL) 
    {
        Nh_WebIDL_ParseNode *AttributeName_p = Nh_WebIDL_allocateNonTerminalParseNode("AttributeName", 1);
        Nh_appendToList(&AttributeName_p->Children, AttributeNameKeyword.Node_p);
        NH_WEBIDL_END(Nh_WebIDL_initParseResult(AttributeName_p))
    }

    if (Parser_p->unparsed > 0 && Parser_p->Tokens_p[0].type == NH_WEBIDL_TOKEN_IDENTIFIER)
    {
        Nh_WebIDL_ParseNode *AttributeName_p = Nh_WebIDL_allocateNonTerminalParseNode("AttributeName", 1);
        Nh_appendToList(&AttributeName_p->Children, Nh_WebIDL_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
        *Parser_p = Nh_WebIDL_advanceParser(*Parser_p, 1);
        NH_WEBIDL_END(Nh_WebIDL_initParseResult(AttributeName_p))
    }

NH_WEBIDL_END(Nh_WebIDL_initParseResult(NULL))
}

static Nh_WebIDL_ParseResult Nh_WebIDL_parseAttributeRest(
    Nh_WebIDL_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    if (!strcmp(Parser_p->Tokens_p[0].String.bytes_p, "attribute"))
    {
        Nh_WebIDL_Parser LocalParser = Nh_WebIDL_advanceParser(*Parser_p, 1);
        Nh_WebIDL_ParseResult TypeWithExtendedAttributes = Nh_WebIDL_parseTypeWithExtendedAttributes(&LocalParser);

        if (TypeWithExtendedAttributes.Node_p != NULL)
        {
            Nh_WebIDL_ParseResult AttributeName = Nh_WebIDL_parseAttributeName(&LocalParser);
            if (AttributeName.Node_p != NULL && LocalParser.unparsed > 0 && LocalParser.Tokens_p[0].String.bytes_p[0] == ';')
            {
                Nh_WebIDL_ParseNode *AttributeRest_p = Nh_WebIDL_allocateNonTerminalParseNode("AttributeRest", 4);
                Nh_appendToList(&AttributeRest_p->Children, Nh_WebIDL_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
                Nh_appendToList(&AttributeRest_p->Children, TypeWithExtendedAttributes.Node_p);
                Nh_appendToList(&AttributeRest_p->Children, AttributeName.Node_p);
                Nh_appendToList(&AttributeRest_p->Children, Nh_WebIDL_allocateTerminalParseNode(&LocalParser.Tokens_p[0]));
                *Parser_p = Nh_WebIDL_advanceParser(LocalParser, 1);
                NH_WEBIDL_END(Nh_WebIDL_initParseResult(AttributeRest_p))
            }
        }

        Nh_WebIDL_freeParseNode(TypeWithExtendedAttributes.Node_p);
    }

NH_WEBIDL_END(Nh_WebIDL_initParseResult(NULL))
}

static Nh_WebIDL_ParseResult Nh_WebIDL_parseStringifierRest(
    Nh_WebIDL_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    if (Parser_p->Tokens_p[0].String.bytes_p[0] == ';')
    {
        Nh_WebIDL_ParseNode *StringifierRest_p = Nh_WebIDL_allocateNonTerminalParseNode("StringifierRest", 1);
        Nh_appendToList(&StringifierRest_p->Children, Nh_WebIDL_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
        *Parser_p = Nh_WebIDL_advanceParser(*Parser_p, 1);
        NH_WEBIDL_END(Nh_WebIDL_initParseResult(StringifierRest_p))
    }

    Nh_WebIDL_ParseResult RegularOperation = Nh_WebIDL_parseRegularOperation(Parser_p);

    if (RegularOperation.Node_p != NULL) 
    {
        Nh_WebIDL_ParseNode *StringifierRest_p = Nh_WebIDL_allocateNonTerminalParseNode("StringifierRest", 1);
        Nh_appendToList(&StringifierRest_p->Children, RegularOperation.Node_p);
        NH_WEBIDL_END(Nh_WebIDL_initParseResult(StringifierRest_p))
    }

    Nh_WebIDL_Parser LocalParser = *Parser_p;
    Nh_WebIDL_ParseResult OptionalReadOnly = Nh_WebIDL_parseOptionalReadOnly(&LocalParser);
    Nh_WebIDL_ParseResult AttributeRest = Nh_WebIDL_parseAttributeRest(&LocalParser);

    if (AttributeRest.Node_p != NULL) 
    {
        Nh_WebIDL_ParseNode *StringifierRest_p = Nh_WebIDL_allocateNonTerminalParseNode("StringifierRest", 2);
        Nh_appendToList(&StringifierRest_p->Children, OptionalReadOnly.Node_p);
        Nh_appendToList(&StringifierRest_p->Children, AttributeRest.Node_p);
        *Parser_p = LocalParser;
        NH_WEBIDL_END(Nh_WebIDL_initParseResult(StringifierRest_p))
    }
    else {Nh_WebIDL_freeParseNode(OptionalReadOnly.Node_p);}

NH_WEBIDL_END(Nh_WebIDL_initParseResult(NULL))
}

static Nh_WebIDL_ParseResult Nh_WebIDL_parseStringifier(
    Nh_WebIDL_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    if (Parser_p->unparsed == 0) {NH_WEBIDL_END(Nh_WebIDL_initParseResult(NULL))}

    if (!strcmp(Parser_p->Tokens_p[0].String.bytes_p, "stringifier"))
    {
        Nh_WebIDL_Parser LocalParser = Nh_WebIDL_advanceParser(*Parser_p, 1);
        Nh_WebIDL_ParseResult StringifierRest = Nh_WebIDL_parseStringifierRest(&LocalParser);

        if (StringifierRest.Node_p != NULL)
        {
            Nh_WebIDL_ParseNode *Stringifier_p = Nh_WebIDL_allocateNonTerminalParseNode("Stringifier", 2);
            Nh_appendToList(&Stringifier_p->Children, Nh_WebIDL_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
            Nh_appendToList(&Stringifier_p->Children, StringifierRest.Node_p);
            *Parser_p = LocalParser;
            NH_WEBIDL_END(Nh_WebIDL_initParseResult(Stringifier_p))
        }
    }

NH_WEBIDL_END(Nh_WebIDL_initParseResult(NULL))
}

static Nh_WebIDL_ParseResult Nh_WebIDL_parseStaticMemberRest(
    Nh_WebIDL_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    Nh_WebIDL_ParseResult RegularOperation = Nh_WebIDL_parseRegularOperation(Parser_p);

    if (RegularOperation.Node_p != NULL) 
    {
        Nh_WebIDL_ParseNode *StaticMemberRest_p = Nh_WebIDL_allocateNonTerminalParseNode("StaticMemberRest", 1);
        Nh_appendToList(&StaticMemberRest_p->Children, RegularOperation.Node_p);
        NH_WEBIDL_END(Nh_WebIDL_initParseResult(StaticMemberRest_p))
    }

    Nh_WebIDL_Parser LocalParser = *Parser_p;
    Nh_WebIDL_ParseResult OptionalReadOnly = Nh_WebIDL_parseOptionalReadOnly(&LocalParser);
    Nh_WebIDL_ParseResult AttributeRest = Nh_WebIDL_parseAttributeRest(&LocalParser);

    if (AttributeRest.Node_p != NULL) 
    {
        Nh_WebIDL_ParseNode *StaticMemberRest_p = Nh_WebIDL_allocateNonTerminalParseNode("StaticMemberRest", 2);
        Nh_appendToList(&StaticMemberRest_p->Children, OptionalReadOnly.Node_p);
        Nh_appendToList(&StaticMemberRest_p->Children, AttributeRest.Node_p);
        *Parser_p = LocalParser;
        NH_WEBIDL_END(Nh_WebIDL_initParseResult(StaticMemberRest_p))
    }
    else {Nh_WebIDL_freeParseNode(OptionalReadOnly.Node_p);}

NH_WEBIDL_END(Nh_WebIDL_initParseResult(NULL))
}

static Nh_WebIDL_ParseResult Nh_WebIDL_parseStaticMember(
    Nh_WebIDL_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    if (Parser_p->unparsed == 0) {NH_WEBIDL_END(Nh_WebIDL_initParseResult(NULL))}

    if (!strcmp(Parser_p->Tokens_p[0].String.bytes_p, "static"))
    {
        Nh_WebIDL_Parser LocalParser = Nh_WebIDL_advanceParser(*Parser_p, 1);
        Nh_WebIDL_ParseResult StaticMemberRest = Nh_WebIDL_parseStaticMemberRest(&LocalParser);

        if (StaticMemberRest.Node_p != NULL)
        {
            Nh_WebIDL_ParseNode *StaticMember_p = Nh_WebIDL_allocateNonTerminalParseNode("StaticMember", 2);
            Nh_appendToList(&StaticMember_p->Children, Nh_WebIDL_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
            Nh_appendToList(&StaticMember_p->Children, StaticMemberRest.Node_p);
            *Parser_p = LocalParser;
            NH_WEBIDL_END(Nh_WebIDL_initParseResult(StaticMember_p))
        }
    }

NH_WEBIDL_END(Nh_WebIDL_initParseResult(NULL))
}

static Nh_WebIDL_ParseResult Nh_WebIDL_parseOptionalType(
    Nh_WebIDL_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    Nh_WebIDL_ParseNode *OptionalType_p = Nh_WebIDL_allocateNonTerminalParseNode("OptionalType", 2);

    if (Parser_p->Tokens_p[0].String.bytes_p[0] == ',')
    {
        Nh_WebIDL_Parser LocalParser = Nh_WebIDL_advanceParser(*Parser_p, 1);
        Nh_WebIDL_ParseResult TypeWithExtendedAttributes = Nh_WebIDL_parseTypeWithExtendedAttributes(&LocalParser);

        if (TypeWithExtendedAttributes.Node_p != NULL)
        {
            Nh_appendToList(&OptionalType_p->Children, Nh_WebIDL_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
            Nh_appendToList(&OptionalType_p->Children, TypeWithExtendedAttributes.Node_p);
            *Parser_p = LocalParser;
        }
    }

NH_WEBIDL_END(Nh_WebIDL_initParseResult(OptionalType_p))
}

static Nh_WebIDL_ParseResult Nh_WebIDL_parseIterable(
    Nh_WebIDL_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    if (Parser_p->unparsed <= 4) {NH_WEBIDL_END(Nh_WebIDL_initParseResult(NULL))}

    if (!strcmp(Parser_p->Tokens_p[0].String.bytes_p, "iterable") && Parser_p->Tokens_p[1].String.bytes_p[0] == '<')
    {
        Nh_WebIDL_Parser LocalParser = Nh_WebIDL_advanceParser(*Parser_p, 2);
        Nh_WebIDL_ParseResult TypeWithExtendedAttributes = Nh_WebIDL_parseTypeWithExtendedAttributes(&LocalParser);

        if (TypeWithExtendedAttributes.Node_p != NULL)
        {
            Nh_WebIDL_ParseResult OptionalType = Nh_WebIDL_parseOptionalType(&LocalParser);

            if (LocalParser.unparsed >= 2 && LocalParser.Tokens_p[0].String.bytes_p[0] == '>' && LocalParser.Tokens_p[1].String.bytes_p[0] == ';')
            {
                Nh_WebIDL_ParseNode *Iterable_p = Nh_WebIDL_allocateNonTerminalParseNode("Iterable", 6);
                Nh_appendToList(&Iterable_p->Children, Nh_WebIDL_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
                Nh_appendToList(&Iterable_p->Children, Nh_WebIDL_allocateTerminalParseNode(&Parser_p->Tokens_p[1]));
                Nh_appendToList(&Iterable_p->Children, TypeWithExtendedAttributes.Node_p);
                Nh_appendToList(&Iterable_p->Children, OptionalType.Node_p);
                Nh_appendToList(&Iterable_p->Children, Nh_WebIDL_allocateTerminalParseNode(&LocalParser.Tokens_p[0]));
                Nh_appendToList(&Iterable_p->Children, Nh_WebIDL_allocateTerminalParseNode(&LocalParser.Tokens_p[1]));
                *Parser_p = Nh_WebIDL_advanceParser(LocalParser, 2);
                NH_WEBIDL_END(Nh_WebIDL_initParseResult(Iterable_p))
            }
        }
    }

NH_WEBIDL_END(Nh_WebIDL_initParseResult(NULL))
}

static Nh_WebIDL_ParseResult Nh_WebIDL_parseOptionalArgumentList(
    Nh_WebIDL_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    Nh_WebIDL_ParseNode *OptionalArgumentList_p = Nh_WebIDL_allocateNonTerminalParseNode("OptionalArgumentList", 3);

    if (Parser_p->unparsed > 0 && Parser_p->Tokens_p[0].String.bytes_p[0] == '(')
    {
        Nh_WebIDL_Parser LocalParser = Nh_WebIDL_advanceParser(*Parser_p, 1);
        Nh_WebIDL_ParseResult ArgumentList = Nh_WebIDL_parseArgumentList(&LocalParser);

        if (LocalParser.unparsed > 0 && LocalParser.Tokens_p[0].String.bytes_p[0] == ')')
        {
            Nh_appendToList(&OptionalArgumentList_p->Children, Nh_WebIDL_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
            Nh_appendToList(&OptionalArgumentList_p->Children, ArgumentList.Node_p);
            Nh_appendToList(&OptionalArgumentList_p->Children, Nh_WebIDL_allocateTerminalParseNode(&LocalParser.Tokens_p[0]));
            *Parser_p = Nh_WebIDL_advanceParser(LocalParser, 1);
        }
    }

NH_WEBIDL_END(Nh_WebIDL_initParseResult(OptionalArgumentList_p))
}

static Nh_WebIDL_ParseResult Nh_WebIDL_parseAsyncIterable(
    Nh_WebIDL_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    if (Parser_p->unparsed <= 5) {NH_WEBIDL_END(Nh_WebIDL_initParseResult(NULL))}

    if (!strcmp(Parser_p->Tokens_p[0].String.bytes_p, "async") && !strcmp(Parser_p->Tokens_p[1].String.bytes_p, "iterable") && Parser_p->Tokens_p[2].String.bytes_p[0] == '<')
    {
        Nh_WebIDL_Parser LocalParser = Nh_WebIDL_advanceParser(*Parser_p, 3);
        Nh_WebIDL_ParseResult TypeWithExtendedAttributes = Nh_WebIDL_parseTypeWithExtendedAttributes(&LocalParser);

        if (TypeWithExtendedAttributes.Node_p != NULL)
        {
            Nh_WebIDL_ParseResult OptionalType = Nh_WebIDL_parseOptionalType(&LocalParser);

            if (LocalParser.unparsed > 2 && LocalParser.Tokens_p[0].String.bytes_p[0] == '>')
            {
                Nh_WebIDL_Token *Right_p = &LocalParser.Tokens_p[0];
                Nh_WebIDL_ParseResult OptionalArgumentList = Nh_WebIDL_parseOptionalArgumentList(&LocalParser);

                if (LocalParser.unparsed > 0 && LocalParser.Tokens_p[0].String.bytes_p[0] == ';')
                {
                    Nh_WebIDL_ParseNode *AsyncIterable_p = Nh_WebIDL_allocateNonTerminalParseNode("AsyncIterable", 8);
                    Nh_appendToList(&AsyncIterable_p->Children, Nh_WebIDL_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
                    Nh_appendToList(&AsyncIterable_p->Children, Nh_WebIDL_allocateTerminalParseNode(&Parser_p->Tokens_p[1]));
                    Nh_appendToList(&AsyncIterable_p->Children, Nh_WebIDL_allocateTerminalParseNode(&Parser_p->Tokens_p[2]));
                    Nh_appendToList(&AsyncIterable_p->Children, TypeWithExtendedAttributes.Node_p);
                    Nh_appendToList(&AsyncIterable_p->Children, OptionalType.Node_p);
                    Nh_appendToList(&AsyncIterable_p->Children, Nh_WebIDL_allocateTerminalParseNode(Right_p));
                    Nh_appendToList(&AsyncIterable_p->Children, OptionalArgumentList.Node_p);
                    Nh_appendToList(&AsyncIterable_p->Children, Nh_WebIDL_allocateTerminalParseNode(&LocalParser.Tokens_p[0]));
                    *Parser_p = Nh_WebIDL_advanceParser(LocalParser, 1);
                    NH_WEBIDL_END(Nh_WebIDL_initParseResult(AsyncIterable_p))
                }
                Nh_WebIDL_freeParseNode(OptionalArgumentList.Node_p);
            }
            Nh_WebIDL_freeParseNode(OptionalType.Node_p);
        }
        Nh_WebIDL_freeParseNode(TypeWithExtendedAttributes.Node_p);
    }

NH_WEBIDL_END(Nh_WebIDL_initParseResult(NULL))
}

static Nh_WebIDL_ParseResult Nh_WebIDL_parseMaplikeRest(
    Nh_WebIDL_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    if (Parser_p->unparsed <= 4) {NH_WEBIDL_END(Nh_WebIDL_initParseResult(NULL))}

    if (!strcmp(Parser_p->Tokens_p[0].String.bytes_p, "maplike") && Parser_p->Tokens_p[1].String.bytes_p[0] == '<')
    {
        Nh_WebIDL_Parser LocalParser = Nh_WebIDL_advanceParser(*Parser_p, 2);
        Nh_WebIDL_ParseResult TypeWithExtendedAttributes1 = Nh_WebIDL_parseTypeWithExtendedAttributes(&LocalParser);

        if (TypeWithExtendedAttributes1.Node_p != NULL && LocalParser.unparsed > 2 && LocalParser.Tokens_p[0].String.bytes_p[0] == ',')
        {
            Nh_WebIDL_Token *Comma_p = &LocalParser.Tokens_p[0];
            LocalParser = Nh_WebIDL_advanceParser(LocalParser, 1);

            Nh_WebIDL_ParseResult TypeWithExtendedAttributes2 = Nh_WebIDL_parseTypeWithExtendedAttributes(&LocalParser);
    
            if (TypeWithExtendedAttributes2.Node_p != NULL && LocalParser.unparsed > 1 && LocalParser.Tokens_p[0].String.bytes_p[0] == '>' && LocalParser.Tokens_p[1].String.bytes_p[0] == ';')
            {
                Nh_WebIDL_ParseNode *MaplikeRest_p = Nh_WebIDL_allocateNonTerminalParseNode("MaplikeRest", 7);
                Nh_appendToList(&MaplikeRest_p->Children, Nh_WebIDL_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
                Nh_appendToList(&MaplikeRest_p->Children, Nh_WebIDL_allocateTerminalParseNode(&Parser_p->Tokens_p[1]));
                Nh_appendToList(&MaplikeRest_p->Children, TypeWithExtendedAttributes1.Node_p);
                Nh_appendToList(&MaplikeRest_p->Children, Nh_WebIDL_allocateTerminalParseNode(Comma_p));
                Nh_appendToList(&MaplikeRest_p->Children, TypeWithExtendedAttributes2.Node_p);
                Nh_appendToList(&MaplikeRest_p->Children, Nh_WebIDL_allocateTerminalParseNode(&LocalParser.Tokens_p[0]));
                Nh_appendToList(&MaplikeRest_p->Children, Nh_WebIDL_allocateTerminalParseNode(&LocalParser.Tokens_p[1]));
                *Parser_p = Nh_WebIDL_advanceParser(LocalParser, 2);
                NH_WEBIDL_END(Nh_WebIDL_initParseResult(MaplikeRest_p))
            }
            Nh_WebIDL_freeParseNode(TypeWithExtendedAttributes2.Node_p);
        }
        Nh_WebIDL_freeParseNode(TypeWithExtendedAttributes1.Node_p);
    }

NH_WEBIDL_END(Nh_WebIDL_initParseResult(NULL))
}

static Nh_WebIDL_ParseResult Nh_WebIDL_parseSetlikeRest(
    Nh_WebIDL_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    if (Parser_p->unparsed <= 4) {NH_WEBIDL_END(Nh_WebIDL_initParseResult(NULL))}

    if (!strcmp(Parser_p->Tokens_p[0].String.bytes_p, "setlike") && Parser_p->Tokens_p[1].String.bytes_p[0] == '<')
    {
        Nh_WebIDL_Parser LocalParser = Nh_WebIDL_advanceParser(*Parser_p, 2);
        Nh_WebIDL_ParseResult TypeWithExtendedAttributes = Nh_WebIDL_parseTypeWithExtendedAttributes(&LocalParser);

        if (TypeWithExtendedAttributes.Node_p != NULL && LocalParser.unparsed > 2 && LocalParser.Tokens_p[0].String.bytes_p[0] == '>' && LocalParser.Tokens_p[0].String.bytes_p[0] == ';')
        {
            Nh_WebIDL_ParseNode *SetlikeRest_p = Nh_WebIDL_allocateNonTerminalParseNode("SetlikeRest", 5);
            Nh_appendToList(&SetlikeRest_p->Children, Nh_WebIDL_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
            Nh_appendToList(&SetlikeRest_p->Children, Nh_WebIDL_allocateTerminalParseNode(&Parser_p->Tokens_p[1]));
            Nh_appendToList(&SetlikeRest_p->Children, TypeWithExtendedAttributes.Node_p);
            Nh_appendToList(&SetlikeRest_p->Children, Nh_WebIDL_allocateTerminalParseNode(&LocalParser.Tokens_p[0]));
            Nh_appendToList(&SetlikeRest_p->Children, Nh_WebIDL_allocateTerminalParseNode(&LocalParser.Tokens_p[1]));
            *Parser_p = Nh_WebIDL_advanceParser(LocalParser, 2);
            NH_WEBIDL_END(Nh_WebIDL_initParseResult(SetlikeRest_p))
        }

        Nh_WebIDL_freeParseNode(TypeWithExtendedAttributes.Node_p);
    }

NH_WEBIDL_END(Nh_WebIDL_initParseResult(NULL))
}

static Nh_WebIDL_ParseResult Nh_WebIDL_parseReadOnlyMemberRest(
    Nh_WebIDL_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    Nh_WebIDL_ParseResult AttributeRest = Nh_WebIDL_parseAttributeRest(Parser_p);
    if (AttributeRest.Node_p != NULL)
    {
        Nh_WebIDL_ParseNode *ReadOnlyMemberRest_p = Nh_WebIDL_allocateNonTerminalParseNode("ReadOnlyMemberRest", 1);
        Nh_appendToList(&ReadOnlyMemberRest_p->Children, AttributeRest.Node_p);
        NH_WEBIDL_END(Nh_WebIDL_initParseResult(ReadOnlyMemberRest_p))
    }

    Nh_WebIDL_ParseResult MaplikeRest = Nh_WebIDL_parseMaplikeRest(Parser_p);
    if (MaplikeRest.Node_p != NULL)
    {
        Nh_WebIDL_ParseNode *ReadOnlyMemberRest_p = Nh_WebIDL_allocateNonTerminalParseNode("ReadOnlyMemberRest", 1);
        Nh_appendToList(&ReadOnlyMemberRest_p->Children, MaplikeRest.Node_p);
        NH_WEBIDL_END(Nh_WebIDL_initParseResult(ReadOnlyMemberRest_p))
    }

    Nh_WebIDL_ParseResult SetlikeRest = Nh_WebIDL_parseSetlikeRest(Parser_p);
    if (SetlikeRest.Node_p != NULL)
    {
        Nh_WebIDL_ParseNode *ReadOnlyMemberRest_p = Nh_WebIDL_allocateNonTerminalParseNode("ReadOnlyMemberRest", 1);
        Nh_appendToList(&ReadOnlyMemberRest_p->Children, SetlikeRest.Node_p);
        NH_WEBIDL_END(Nh_WebIDL_initParseResult(ReadOnlyMemberRest_p))
    }

NH_WEBIDL_END(Nh_WebIDL_initParseResult(NULL))
}

static Nh_WebIDL_ParseResult Nh_WebIDL_parseReadOnlyMember(
    Nh_WebIDL_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    if (Parser_p->unparsed <= 2) {NH_WEBIDL_END(Nh_WebIDL_initParseResult(NULL))}

    if (!strcmp(Parser_p->Tokens_p[0].String.bytes_p, "readonly"))
    {
        Nh_WebIDL_Parser LocalParser = Nh_WebIDL_advanceParser(*Parser_p, 1);
        Nh_WebIDL_ParseResult ReadOnlyMemberRest = Nh_WebIDL_parseReadOnlyMemberRest(&LocalParser);

        if (ReadOnlyMemberRest.Node_p != NULL)
        {
            Nh_WebIDL_ParseNode *ReadOnlyMember_p = Nh_WebIDL_allocateNonTerminalParseNode("ReadOnlyMember", 2);
            Nh_appendToList(&ReadOnlyMember_p->Children, Nh_WebIDL_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
            Nh_appendToList(&ReadOnlyMember_p->Children, ReadOnlyMemberRest.Node_p);
            *Parser_p = LocalParser;
            NH_WEBIDL_END(Nh_WebIDL_initParseResult(ReadOnlyMember_p))
        }
    }

NH_WEBIDL_END(Nh_WebIDL_initParseResult(NULL))
}

static Nh_WebIDL_ParseResult Nh_WebIDL_parseInheritAttribute(
    Nh_WebIDL_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    if (Parser_p->unparsed <= 2) {NH_WEBIDL_END(Nh_WebIDL_initParseResult(NULL))}

    if (!strcmp(Parser_p->Tokens_p[0].String.bytes_p, "inherit"))
    {
        Nh_WebIDL_Parser LocalParser = Nh_WebIDL_advanceParser(*Parser_p, 1);
        Nh_WebIDL_ParseResult AttributeRest = Nh_WebIDL_parseAttributeRest(&LocalParser);

        if (AttributeRest.Node_p != NULL)
        {
            Nh_WebIDL_ParseNode *InheritAttribute_p = Nh_WebIDL_allocateNonTerminalParseNode("InheritAttribute", 2);
            Nh_appendToList(&InheritAttribute_p->Children, Nh_WebIDL_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
            Nh_appendToList(&InheritAttribute_p->Children, AttributeRest.Node_p);
            *Parser_p = LocalParser;
            NH_WEBIDL_END(Nh_WebIDL_initParseResult(InheritAttribute_p))
        }
    }

NH_WEBIDL_END(Nh_WebIDL_initParseResult(NULL))
}

static Nh_WebIDL_ParseResult Nh_WebIDL_parseReadWriteAttribute(
    Nh_WebIDL_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    Nh_WebIDL_ParseResult AttributeRest = Nh_WebIDL_parseAttributeRest(Parser_p);

    if (AttributeRest.Node_p != NULL)
    {
        Nh_WebIDL_ParseNode *ReadWriteAttribute_p = Nh_WebIDL_allocateNonTerminalParseNode("ReadWriteAttribute", 1);
        Nh_appendToList(&ReadWriteAttribute_p->Children, AttributeRest.Node_p);
        NH_WEBIDL_END(Nh_WebIDL_initParseResult(ReadWriteAttribute_p))
    }

NH_WEBIDL_END(Nh_WebIDL_initParseResult(NULL))
}

static Nh_WebIDL_ParseResult Nh_WebIDL_parseReadWriteSetlike(
    Nh_WebIDL_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    Nh_WebIDL_ParseResult SetlikeRest = Nh_WebIDL_parseSetlikeRest(Parser_p);

    if (SetlikeRest.Node_p != NULL)
    {
        Nh_WebIDL_ParseNode *ReadWriteSetlike_p = Nh_WebIDL_allocateNonTerminalParseNode("ReadWriteSetlike", 1);
        Nh_appendToList(&ReadWriteSetlike_p->Children, SetlikeRest.Node_p);
        NH_WEBIDL_END(Nh_WebIDL_initParseResult(ReadWriteSetlike_p))
    }

NH_WEBIDL_END(Nh_WebIDL_initParseResult(NULL))
}

static Nh_WebIDL_ParseResult Nh_WebIDL_parseReadWriteMaplike(
    Nh_WebIDL_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    Nh_WebIDL_ParseResult MaplikeRest = Nh_WebIDL_parseMaplikeRest(Parser_p);

    if (MaplikeRest.Node_p != NULL)
    {
        Nh_WebIDL_ParseNode *ReadWriteMaplike_p = Nh_WebIDL_allocateNonTerminalParseNode("ReadWriteMaplike", 1);
        Nh_appendToList(&ReadWriteMaplike_p->Children, MaplikeRest.Node_p);
        NH_WEBIDL_END(Nh_WebIDL_initParseResult(ReadWriteMaplike_p))
    }

NH_WEBIDL_END(Nh_WebIDL_initParseResult(NULL))
}

static Nh_WebIDL_ParseResult Nh_WebIDL_parsePartialInterfaceMember(
    Nh_WebIDL_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    Nh_WebIDL_ParseResult Const = Nh_WebIDL_parseConst(Parser_p);
    if (Const.Node_p != NULL) 
    {
        Nh_WebIDL_ParseNode *PartialInterfaceMember_p = Nh_WebIDL_allocateNonTerminalParseNode("PartialInterfaceMember", 1);
        Nh_appendToList(&PartialInterfaceMember_p->Children, Const.Node_p);
        NH_WEBIDL_END(Nh_WebIDL_initParseResult(PartialInterfaceMember_p))
    }

    Nh_WebIDL_ParseResult Operation = Nh_WebIDL_parseOperation(Parser_p);
    if (Operation.Node_p != NULL) 
    {
        Nh_WebIDL_ParseNode *PartialInterfaceMember_p = Nh_WebIDL_allocateNonTerminalParseNode("PartialInterfaceMember", 1);
        Nh_appendToList(&PartialInterfaceMember_p->Children, Operation.Node_p);
        NH_WEBIDL_END(Nh_WebIDL_initParseResult(PartialInterfaceMember_p))
    }

    Nh_WebIDL_ParseResult Stringifier = Nh_WebIDL_parseStringifier(Parser_p);
    if (Stringifier.Node_p != NULL) 
    {
        Nh_WebIDL_ParseNode *PartialInterfaceMember_p = Nh_WebIDL_allocateNonTerminalParseNode("PartialInterfaceMember", 1);
        Nh_appendToList(&PartialInterfaceMember_p->Children, Stringifier.Node_p);
        NH_WEBIDL_END(Nh_WebIDL_initParseResult(PartialInterfaceMember_p))
    }

    Nh_WebIDL_ParseResult StaticMember = Nh_WebIDL_parseStaticMember(Parser_p);
    if (Stringifier.Node_p != NULL) 
    {
        Nh_WebIDL_ParseNode *PartialInterfaceMember_p = Nh_WebIDL_allocateNonTerminalParseNode("PartialInterfaceMember", 1);
        Nh_appendToList(&PartialInterfaceMember_p->Children, StaticMember.Node_p);
        NH_WEBIDL_END(Nh_WebIDL_initParseResult(PartialInterfaceMember_p))
    }

    Nh_WebIDL_ParseResult Iterable = Nh_WebIDL_parseIterable(Parser_p);
    if (Iterable.Node_p != NULL) 
    {
        Nh_WebIDL_ParseNode *PartialInterfaceMember_p = Nh_WebIDL_allocateNonTerminalParseNode("PartialInterfaceMember", 1);
        Nh_appendToList(&PartialInterfaceMember_p->Children, Iterable.Node_p);
        NH_WEBIDL_END(Nh_WebIDL_initParseResult(PartialInterfaceMember_p))
    }

    Nh_WebIDL_ParseResult AsyncIterable = Nh_WebIDL_parseAsyncIterable(Parser_p);
    if (AsyncIterable.Node_p != NULL) 
    {
        Nh_WebIDL_ParseNode *PartialInterfaceMember_p = Nh_WebIDL_allocateNonTerminalParseNode("PartialInterfaceMember", 1);
        Nh_appendToList(&PartialInterfaceMember_p->Children, AsyncIterable.Node_p);
        NH_WEBIDL_END(Nh_WebIDL_initParseResult(PartialInterfaceMember_p))
    }

    Nh_WebIDL_ParseResult ReadOnlyMember = Nh_WebIDL_parseReadOnlyMember(Parser_p);
    if (ReadOnlyMember.Node_p != NULL) 
    {
        Nh_WebIDL_ParseNode *PartialInterfaceMember_p = Nh_WebIDL_allocateNonTerminalParseNode("PartialInterfaceMember", 1);
        Nh_appendToList(&PartialInterfaceMember_p->Children, ReadOnlyMember.Node_p);
        NH_WEBIDL_END(Nh_WebIDL_initParseResult(PartialInterfaceMember_p))
    }

    Nh_WebIDL_ParseResult ReadWriteAttribute = Nh_WebIDL_parseReadWriteAttribute(Parser_p);
    if (ReadWriteAttribute.Node_p != NULL) 
    {
        Nh_WebIDL_ParseNode *PartialInterfaceMember_p = Nh_WebIDL_allocateNonTerminalParseNode("PartialInterfaceMember", 1);
        Nh_appendToList(&PartialInterfaceMember_p->Children, ReadWriteAttribute.Node_p);
        NH_WEBIDL_END(Nh_WebIDL_initParseResult(PartialInterfaceMember_p))
    }

    Nh_WebIDL_ParseResult ReadWriteMaplike = Nh_WebIDL_parseReadWriteMaplike(Parser_p);
    if (ReadWriteMaplike.Node_p != NULL) 
    {
        Nh_WebIDL_ParseNode *PartialInterfaceMember_p = Nh_WebIDL_allocateNonTerminalParseNode("PartialInterfaceMember", 1);
        Nh_appendToList(&PartialInterfaceMember_p->Children, ReadWriteMaplike.Node_p);
        NH_WEBIDL_END(Nh_WebIDL_initParseResult(PartialInterfaceMember_p))
    }

    Nh_WebIDL_ParseResult ReadWriteSetlike = Nh_WebIDL_parseReadWriteSetlike(Parser_p);
    if (ReadWriteSetlike.Node_p != NULL) 
    {
        Nh_WebIDL_ParseNode *PartialInterfaceMember_p = Nh_WebIDL_allocateNonTerminalParseNode("PartialInterfaceMember", 1);
        Nh_appendToList(&PartialInterfaceMember_p->Children, ReadWriteSetlike.Node_p);
        NH_WEBIDL_END(Nh_WebIDL_initParseResult(PartialInterfaceMember_p))
    }

    Nh_WebIDL_ParseResult InheritAttribute = Nh_WebIDL_parseInheritAttribute(Parser_p);
    if (InheritAttribute.Node_p != NULL) 
    {
        Nh_WebIDL_ParseNode *PartialInterfaceMember_p = Nh_WebIDL_allocateNonTerminalParseNode("PartialInterfaceMember", 1);
        Nh_appendToList(&PartialInterfaceMember_p->Children, InheritAttribute.Node_p);
        NH_WEBIDL_END(Nh_WebIDL_initParseResult(PartialInterfaceMember_p))
    }

NH_WEBIDL_END(Nh_WebIDL_initParseResult(NULL))
}

static Nh_WebIDL_ParseResult Nh_WebIDL_parseConstructor(
    Nh_WebIDL_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    if (Parser_p->unparsed <= 3) {NH_WEBIDL_END(Nh_WebIDL_initParseResult(NULL))}

    if (!strcmp(Parser_p->Tokens_p[0].String.bytes_p, "constructor") && Parser_p->Tokens_p[1].String.bytes_p[0] == '(')
    {
        Nh_WebIDL_Parser LocalParser = Nh_WebIDL_advanceParser(*Parser_p, 2);
        Nh_WebIDL_ParseResult ArgumentList = Nh_WebIDL_parseArgumentList(&LocalParser);

        if (LocalParser.unparsed > 1 && LocalParser.Tokens_p[0].String.bytes_p[0] == ')' && LocalParser.Tokens_p[1].String.bytes_p[0] == ';')
        {
            Nh_WebIDL_ParseNode *Constructor_p = Nh_WebIDL_allocateNonTerminalParseNode("Constructor", 5);
            Nh_appendToList(&Constructor_p->Children, Nh_WebIDL_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
            Nh_appendToList(&Constructor_p->Children, Nh_WebIDL_allocateTerminalParseNode(&Parser_p->Tokens_p[1]));
            Nh_appendToList(&Constructor_p->Children, ArgumentList.Node_p);
            Nh_appendToList(&Constructor_p->Children, Nh_WebIDL_allocateTerminalParseNode(&LocalParser.Tokens_p[0]));
            Nh_appendToList(&Constructor_p->Children, Nh_WebIDL_allocateTerminalParseNode(&LocalParser.Tokens_p[1]));
            *Parser_p = Nh_WebIDL_advanceParser(LocalParser, 2);
            NH_WEBIDL_END(Nh_WebIDL_initParseResult(Constructor_p))
        }

        Nh_WebIDL_freeParseNode(ArgumentList.Node_p);
    }

NH_WEBIDL_END(Nh_WebIDL_initParseResult(NULL))
}

static Nh_WebIDL_ParseResult Nh_WebIDL_parseInterfaceMember(
    Nh_WebIDL_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    Nh_WebIDL_ParseResult PartialInterfaceMember = Nh_WebIDL_parsePartialInterfaceMember(Parser_p);
    if (PartialInterfaceMember.Node_p != NULL)  
    {
        Nh_WebIDL_ParseNode *InterfaceMember_p = Nh_WebIDL_allocateNonTerminalParseNode("InterfaceMember", 1);
        Nh_appendToList(&InterfaceMember_p->Children, PartialInterfaceMember.Node_p);
        NH_WEBIDL_END(Nh_WebIDL_initParseResult(InterfaceMember_p))
    }

    Nh_WebIDL_ParseResult Constructor = Nh_WebIDL_parseConstructor(Parser_p);
    if (Constructor.Node_p != NULL)  
    {
        Nh_WebIDL_ParseNode *InterfaceMember_p = Nh_WebIDL_allocateNonTerminalParseNode("InterfaceMember", 1);
        Nh_appendToList(&InterfaceMember_p->Children, Constructor.Node_p);
        NH_WEBIDL_END(Nh_WebIDL_initParseResult(InterfaceMember_p))
    }

NH_WEBIDL_END(Nh_WebIDL_initParseResult(NULL))
}

static Nh_WebIDL_ParseResult Nh_WebIDL_parseInterfaceMembers(
    Nh_WebIDL_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    Nh_WebIDL_ParseNode *InterfaceMembers_p = Nh_WebIDL_allocateNonTerminalParseNode("InterfaceMembers", 3);
    Nh_WebIDL_ParseResult ExtendedAttributeList = Nh_WebIDL_parseExtendedAttributeList(Parser_p);
    Nh_WebIDL_ParseResult InterfaceMember = Nh_WebIDL_parseInterfaceMember(Parser_p);

    if (InterfaceMember.Node_p != NULL) {
        Nh_WebIDL_ParseResult InterfaceMembers = Nh_WebIDL_parseInterfaceMembers(Parser_p);
        Nh_appendToList(&InterfaceMembers_p->Children, ExtendedAttributeList.Node_p);
        Nh_appendToList(&InterfaceMembers_p->Children, InterfaceMember.Node_p);
        Nh_appendToList(&InterfaceMembers_p->Children, InterfaceMembers.Node_p);
    }
    else {Nh_WebIDL_freeParseNode(ExtendedAttributeList.Node_p);}

NH_WEBIDL_END(Nh_WebIDL_initParseResult(InterfaceMembers_p))
}

static Nh_WebIDL_ParseResult Nh_WebIDL_parseInterfaceRest(
    Nh_WebIDL_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    if (Parser_p->unparsed > 0 && Parser_p->Tokens_p[0].type == NH_WEBIDL_TOKEN_IDENTIFIER) 
    {
        Nh_WebIDL_Parser LocalParser = Nh_WebIDL_advanceParser(*Parser_p, 1);
        Nh_WebIDL_ParseResult Inheritance = Nh_WebIDL_parseInheritance(&LocalParser); 

        if (LocalParser.unparsed > 0 && LocalParser.Tokens_p[0].String.bytes_p[0] == '{') 
        {
            Nh_WebIDL_Token *LeftBracket_p = &LocalParser.Tokens_p[0];
            LocalParser = Nh_WebIDL_advanceParser(LocalParser, 1);

            Nh_WebIDL_ParseResult InterfaceMembers = Nh_WebIDL_parseInterfaceMembers(&LocalParser);

            if (LocalParser.unparsed > 1 
            &&  LocalParser.Tokens_p[0].String.bytes_p[0] == '}'
            &&  LocalParser.Tokens_p[1].String.bytes_p[0] == ';') 
            {
                Nh_WebIDL_ParseNode *InterfaceRest_p = Nh_WebIDL_allocateNonTerminalParseNode("InterfaceRest", 6);
                Nh_appendToList(&InterfaceRest_p->Children, Nh_WebIDL_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
                Nh_appendToList(&InterfaceRest_p->Children, Inheritance.Node_p);
                Nh_appendToList(&InterfaceRest_p->Children, Nh_WebIDL_allocateTerminalParseNode(LeftBracket_p));
                Nh_appendToList(&InterfaceRest_p->Children, InterfaceMembers.Node_p);
                Nh_appendToList(&InterfaceRest_p->Children, Nh_WebIDL_allocateTerminalParseNode(&LocalParser.Tokens_p[0]));
                Nh_appendToList(&InterfaceRest_p->Children, Nh_WebIDL_allocateTerminalParseNode(&LocalParser.Tokens_p[1]));
                *Parser_p = Nh_WebIDL_advanceParser(LocalParser, 2);
                NH_WEBIDL_END(Nh_WebIDL_initParseResult(InterfaceRest_p))
            }
            else {Nh_WebIDL_freeParseNode(InterfaceMembers.Node_p);} 
        }
        Nh_WebIDL_freeParseNode(Inheritance.Node_p);
    }

NH_WEBIDL_END(Nh_WebIDL_initParseResult(NULL))
}

static Nh_WebIDL_ParseResult Nh_WebIDL_parseMixinMember(
    Nh_WebIDL_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    Nh_WebIDL_ParseResult Const = Nh_WebIDL_parseConst(Parser_p);
    if (Const.Node_p != NULL)  
    {
        Nh_WebIDL_ParseNode *MixinMember_p = Nh_WebIDL_allocateNonTerminalParseNode("MixinMember", 1);
        Nh_appendToList(&MixinMember_p->Children, Const.Node_p);
        NH_WEBIDL_END(Nh_WebIDL_initParseResult(MixinMember_p))
    }

    Nh_WebIDL_ParseResult RegularOperation = Nh_WebIDL_parseRegularOperation(Parser_p);
    if (RegularOperation.Node_p != NULL)  
    {
        Nh_WebIDL_ParseNode *MixinMember_p = Nh_WebIDL_allocateNonTerminalParseNode("MixinMember", 1);
        Nh_appendToList(&MixinMember_p->Children, RegularOperation.Node_p);
        NH_WEBIDL_END(Nh_WebIDL_initParseResult(MixinMember_p))
    }

    Nh_WebIDL_ParseResult Stringifier = Nh_WebIDL_parseStringifier(Parser_p);
    if (Stringifier.Node_p != NULL)  
    {
        Nh_WebIDL_ParseNode *MixinMember_p = Nh_WebIDL_allocateNonTerminalParseNode("MixinMember", 1);
        Nh_appendToList(&MixinMember_p->Children, Stringifier.Node_p);
        NH_WEBIDL_END(Nh_WebIDL_initParseResult(MixinMember_p))
    }

    Nh_WebIDL_Parser LocalParser = *Parser_p;
    Nh_WebIDL_ParseResult OptionalReadOnly = Nh_WebIDL_parseOptionalReadOnly(&LocalParser);
    Nh_WebIDL_ParseResult AttributeRest = Nh_WebIDL_parseAttributeRest(&LocalParser);
    if (AttributeRest.Node_p != NULL)  
    {
        Nh_WebIDL_ParseNode *MixinMember_p = Nh_WebIDL_allocateNonTerminalParseNode("MixinMember", 2);
        Nh_appendToList(&MixinMember_p->Children, OptionalReadOnly.Node_p);
        Nh_appendToList(&MixinMember_p->Children, AttributeRest.Node_p);
        *Parser_p = LocalParser;
        NH_WEBIDL_END(Nh_WebIDL_initParseResult(MixinMember_p))
    }
    else {Nh_WebIDL_freeParseNode(OptionalReadOnly.Node_p);}

NH_WEBIDL_END(Nh_WebIDL_initParseResult(NULL))
}

static Nh_WebIDL_ParseResult Nh_WebIDL_parseMixinMembers(
    Nh_WebIDL_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    Nh_WebIDL_ParseNode *MixinMembers_p = Nh_WebIDL_allocateNonTerminalParseNode("MixinMembers", 3);
    Nh_WebIDL_ParseResult ExtendedAttributeList = Nh_WebIDL_parseExtendedAttributeList(Parser_p);
    Nh_WebIDL_ParseResult MixinMember = Nh_WebIDL_parseMixinMember(Parser_p);

    if (MixinMember.Node_p != NULL) {
        Nh_WebIDL_ParseResult MixinMembers = Nh_WebIDL_parseMixinMembers(Parser_p);
        Nh_appendToList(&MixinMembers_p->Children, ExtendedAttributeList.Node_p);
        Nh_appendToList(&MixinMembers_p->Children, MixinMember.Node_p);
        Nh_appendToList(&MixinMembers_p->Children, MixinMembers.Node_p);
    }
    else {Nh_WebIDL_freeParseNode(ExtendedAttributeList.Node_p);}

NH_WEBIDL_END(Nh_WebIDL_initParseResult(MixinMembers_p))
}

static Nh_WebIDL_ParseResult Nh_WebIDL_parseMixinRest(
    Nh_WebIDL_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    if (Parser_p->unparsed > 2 && !strcmp(Parser_p->Tokens_p[0].String.bytes_p, "mixin") && Parser_p->Tokens_p[1].type == NH_WEBIDL_TOKEN_IDENTIFIER && Parser_p->Tokens_p[2].String.bytes_p[0] == '{') 
    {
        Nh_WebIDL_Parser LocalParser = Nh_WebIDL_advanceParser(*Parser_p, 3);
        Nh_WebIDL_ParseResult MixinMembers = Nh_WebIDL_parseMixinMembers(&LocalParser); 

        if (LocalParser.unparsed > 1 && LocalParser.Tokens_p[0].String.bytes_p[0] == '}' && LocalParser.Tokens_p[1].String.bytes_p[0] == ';') 
        {
            Nh_WebIDL_ParseNode *MixinRest_p = Nh_WebIDL_allocateNonTerminalParseNode("MixinRest", 6);
            Nh_appendToList(&MixinRest_p->Children, Nh_WebIDL_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
            Nh_appendToList(&MixinRest_p->Children, Nh_WebIDL_allocateTerminalParseNode(&Parser_p->Tokens_p[1]));
            Nh_appendToList(&MixinRest_p->Children, Nh_WebIDL_allocateTerminalParseNode(&Parser_p->Tokens_p[2]));
            Nh_appendToList(&MixinRest_p->Children, MixinMembers.Node_p);
            Nh_appendToList(&MixinRest_p->Children, Nh_WebIDL_allocateTerminalParseNode(&LocalParser.Tokens_p[0]));
            Nh_appendToList(&MixinRest_p->Children, Nh_WebIDL_allocateTerminalParseNode(&LocalParser.Tokens_p[1]));
            *Parser_p = Nh_WebIDL_advanceParser(LocalParser, 2);
            NH_WEBIDL_END(Nh_WebIDL_initParseResult(MixinRest_p))
        }

        Nh_WebIDL_freeParseNode(MixinMembers.Node_p);
    }

NH_WEBIDL_END(Nh_WebIDL_initParseResult(NULL))
}

static Nh_WebIDL_ParseResult Nh_WebIDL_parseInterfaceOrMixin(
    Nh_WebIDL_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    Nh_WebIDL_ParseResult InterfaceRest = Nh_WebIDL_parseInterfaceRest(Parser_p);

    if (InterfaceRest.Node_p != NULL) 
    {
        Nh_WebIDL_ParseNode *InterfaceOrMixin_p = Nh_WebIDL_allocateNonTerminalParseNode("InterfaceOrMixin", 1);
        Nh_appendToList(&InterfaceOrMixin_p->Children, InterfaceRest.Node_p);
        NH_WEBIDL_END(Nh_WebIDL_initParseResult(InterfaceOrMixin_p))
    }
    
    Nh_WebIDL_ParseResult MixinRest = Nh_WebIDL_parseMixinRest(Parser_p);

    if (MixinRest.Node_p != NULL) 
    {
        Nh_WebIDL_ParseNode *InterfaceOrMixin_p = Nh_WebIDL_allocateNonTerminalParseNode("InterfaceOrMixin", 1);
        Nh_appendToList(&InterfaceOrMixin_p->Children, MixinRest.Node_p);
        NH_WEBIDL_END(Nh_WebIDL_initParseResult(InterfaceOrMixin_p))
    }

NH_WEBIDL_END(Nh_WebIDL_initParseResult(NULL))
}

static Nh_WebIDL_ParseResult Nh_WebIDL_parseCallbackOrInterfaceOrMixin(
    Nh_WebIDL_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    if (Parser_p->unparsed > 0 && !strcmp("callback", Parser_p->Tokens_p[0].String.bytes_p)) 
    {
        Nh_WebIDL_Parser LocalParser = Nh_WebIDL_advanceParser(*Parser_p, 1);
        Nh_WebIDL_ParseResult CallbackRestOrInterface = Nh_WebIDL_parseCallbackRestOrInterface(&LocalParser);

        if (CallbackRestOrInterface.Node_p != NULL) 
        {
            Nh_WebIDL_ParseNode *CallbackOrInterfaceOrMixin_p = Nh_WebIDL_allocateNonTerminalParseNode("CallbackOrInterfaceOrMixin", 2);
            Nh_appendToList(&CallbackOrInterfaceOrMixin_p->Children, Nh_WebIDL_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
            Nh_appendToList(&CallbackOrInterfaceOrMixin_p->Children, CallbackRestOrInterface.Node_p);
            *Parser_p = LocalParser;
            NH_WEBIDL_END(Nh_WebIDL_initParseResult(CallbackOrInterfaceOrMixin_p))
        }
    }

    if (Parser_p->unparsed > 0 && !strcmp("interface", Parser_p->Tokens_p[0].String.bytes_p)) 
    {
        Nh_WebIDL_Parser LocalParser = Nh_WebIDL_advanceParser(*Parser_p, 1);
        Nh_WebIDL_ParseResult InterfaceOrMixin = Nh_WebIDL_parseInterfaceOrMixin(&LocalParser);

        if (InterfaceOrMixin.Node_p != NULL) 
        {
            Nh_WebIDL_ParseNode *CallbackOrInterfaceOrMixin_p = Nh_WebIDL_allocateNonTerminalParseNode("CallbackOrInterfaceOrMixin", 2);
            Nh_appendToList(&CallbackOrInterfaceOrMixin_p->Children, Nh_WebIDL_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
            Nh_appendToList(&CallbackOrInterfaceOrMixin_p->Children, InterfaceOrMixin.Node_p);
            *Parser_p = LocalParser;
            NH_WEBIDL_END(Nh_WebIDL_initParseResult(CallbackOrInterfaceOrMixin_p))
        }
    }
    
NH_WEBIDL_END(Nh_WebIDL_initParseResult(NULL))
}

static Nh_WebIDL_ParseResult Nh_WebIDL_parseNamespaceMember(
    Nh_WebIDL_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    Nh_WebIDL_ParseResult RegularOperation = Nh_WebIDL_parseRegularOperation(Parser_p);
    if (RegularOperation.Node_p != NULL)  
    {
        Nh_WebIDL_ParseNode *NamespaceMember_p = Nh_WebIDL_allocateNonTerminalParseNode("NamespaceMember", 1);
        Nh_appendToList(&NamespaceMember_p->Children, RegularOperation.Node_p);
        NH_WEBIDL_END(Nh_WebIDL_initParseResult(NamespaceMember_p))
    }

    if (Parser_p->unparsed > 0 && !strcmp(Parser_p->Tokens_p[0].String.bytes_p, "readonly"))
    {
        Nh_WebIDL_Parser LocalParser = Nh_WebIDL_advanceParser(*Parser_p, 1);
        Nh_WebIDL_ParseResult AttributeRest = Nh_WebIDL_parseAttributeRest(&LocalParser);

        if (AttributeRest.Node_p != NULL)  
        {
            Nh_WebIDL_ParseNode *NamespaceMember_p = Nh_WebIDL_allocateNonTerminalParseNode("NamespaceMember", 2);
            Nh_appendToList(&NamespaceMember_p->Children, Nh_WebIDL_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
            Nh_appendToList(&NamespaceMember_p->Children, AttributeRest.Node_p);
            *Parser_p = LocalParser;
            NH_WEBIDL_END(Nh_WebIDL_initParseResult(NamespaceMember_p))
        }
    }

NH_WEBIDL_END(Nh_WebIDL_initParseResult(NULL))
}

static Nh_WebIDL_ParseResult Nh_WebIDL_parseNamespaceMembers(
    Nh_WebIDL_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    Nh_WebIDL_ParseNode *NamespaceMembers_p = Nh_WebIDL_allocateNonTerminalParseNode("NamespaceMembers", 3);
    Nh_WebIDL_ParseResult ExtendedAttributeList = Nh_WebIDL_parseExtendedAttributeList(Parser_p);
    Nh_WebIDL_ParseResult NamespaceMember = Nh_WebIDL_parseNamespaceMember(Parser_p);

    if (NamespaceMember.Node_p != NULL) {
        Nh_WebIDL_ParseResult NamespaceMembers = Nh_WebIDL_parseNamespaceMembers(Parser_p);
        Nh_appendToList(&NamespaceMembers_p->Children, ExtendedAttributeList.Node_p);
        Nh_appendToList(&NamespaceMembers_p->Children, NamespaceMember.Node_p);
        Nh_appendToList(&NamespaceMembers_p->Children, NamespaceMembers.Node_p);
    }
    else {Nh_WebIDL_freeParseNode(ExtendedAttributeList.Node_p);}

NH_WEBIDL_END(Nh_WebIDL_initParseResult(NamespaceMembers_p))
}

static Nh_WebIDL_ParseResult Nh_WebIDL_parseNamespace(
    Nh_WebIDL_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    if (Parser_p->unparsed > 2 && !strcmp(Parser_p->Tokens_p[0].String.bytes_p, "namespace") && Parser_p->Tokens_p[1].type == NH_WEBIDL_TOKEN_IDENTIFIER && Parser_p->Tokens_p[2].String.bytes_p[0] == '{') 
    {
        Nh_WebIDL_Parser LocalParser = Nh_WebIDL_advanceParser(*Parser_p, 3);
        Nh_WebIDL_ParseResult NamespaceMembers = Nh_WebIDL_parseNamespaceMembers(&LocalParser); 

        if (LocalParser.unparsed > 1 && LocalParser.Tokens_p[0].String.bytes_p[0] == '}' && LocalParser.Tokens_p[1].String.bytes_p[0] == ';') 
        {
            Nh_WebIDL_ParseNode *Namespace_p = Nh_WebIDL_allocateNonTerminalParseNode("Namespace", 6);
            Nh_appendToList(&Namespace_p->Children, Nh_WebIDL_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
            Nh_appendToList(&Namespace_p->Children, Nh_WebIDL_allocateTerminalParseNode(&Parser_p->Tokens_p[1]));
            Nh_appendToList(&Namespace_p->Children, Nh_WebIDL_allocateTerminalParseNode(&Parser_p->Tokens_p[2]));
            Nh_appendToList(&Namespace_p->Children, NamespaceMembers.Node_p);
            Nh_appendToList(&Namespace_p->Children, Nh_WebIDL_allocateTerminalParseNode(&LocalParser.Tokens_p[0]));
            Nh_appendToList(&Namespace_p->Children, Nh_WebIDL_allocateTerminalParseNode(&LocalParser.Tokens_p[1]));
            *Parser_p = Nh_WebIDL_advanceParser(LocalParser, 2);
            NH_WEBIDL_END(Nh_WebIDL_initParseResult(Namespace_p))
        }

        Nh_WebIDL_freeParseNode(NamespaceMembers.Node_p);
    }

NH_WEBIDL_END(Nh_WebIDL_initParseResult(NULL))
}

static Nh_WebIDL_ParseResult Nh_WebIDL_parsePartialInterfaceMembers(
    Nh_WebIDL_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    Nh_WebIDL_ParseNode *PartialInterfaceMembers_p = Nh_WebIDL_allocateNonTerminalParseNode("PartialInterfaceMembers", 3);
    Nh_WebIDL_ParseResult ExtendedAttributeList = Nh_WebIDL_parseExtendedAttributeList(Parser_p);
    Nh_WebIDL_ParseResult PartialInterfaceMember = Nh_WebIDL_parsePartialInterfaceMember(Parser_p);

    if (PartialInterfaceMember.Node_p != NULL) {
        Nh_WebIDL_ParseResult PartialInterfaceMembers = Nh_WebIDL_parsePartialInterfaceMembers(Parser_p);
        Nh_appendToList(&PartialInterfaceMembers_p->Children, ExtendedAttributeList.Node_p);
        Nh_appendToList(&PartialInterfaceMembers_p->Children, PartialInterfaceMember.Node_p);
        Nh_appendToList(&PartialInterfaceMembers_p->Children, PartialInterfaceMembers.Node_p);
    }
    else {Nh_WebIDL_freeParseNode(ExtendedAttributeList.Node_p);}

NH_WEBIDL_END(Nh_WebIDL_initParseResult(PartialInterfaceMembers_p))
}

static Nh_WebIDL_ParseResult Nh_WebIDL_parsePartialInterfaceRest(
    Nh_WebIDL_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    if (Parser_p->unparsed > 1 && Parser_p->Tokens_p[0].type == NH_WEBIDL_TOKEN_IDENTIFIER && Parser_p->Tokens_p[1].String.bytes_p[0] == '{') 
    {
        Nh_WebIDL_Parser LocalParser = Nh_WebIDL_advanceParser(*Parser_p, 2);
        Nh_WebIDL_ParseResult PartialInterfaceMembers = Nh_WebIDL_parsePartialInterfaceMembers(&LocalParser); 

        if (LocalParser.unparsed > 1 && LocalParser.Tokens_p[0].String.bytes_p[0] == '}' && LocalParser.Tokens_p[1].String.bytes_p[0] == ';') 
        {
            Nh_WebIDL_ParseNode *PartialInterfaceRest_p = Nh_WebIDL_allocateNonTerminalParseNode("PartialInterfaceRest", 5);
            Nh_appendToList(&PartialInterfaceRest_p->Children, Nh_WebIDL_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
            Nh_appendToList(&PartialInterfaceRest_p->Children, Nh_WebIDL_allocateTerminalParseNode(&Parser_p->Tokens_p[1]));
            Nh_appendToList(&PartialInterfaceRest_p->Children, PartialInterfaceMembers.Node_p);
            Nh_appendToList(&PartialInterfaceRest_p->Children, Nh_WebIDL_allocateTerminalParseNode(&LocalParser.Tokens_p[0]));
            Nh_appendToList(&PartialInterfaceRest_p->Children, Nh_WebIDL_allocateTerminalParseNode(&LocalParser.Tokens_p[1]));
            *Parser_p = Nh_WebIDL_advanceParser(LocalParser, 2);
            NH_WEBIDL_END(Nh_WebIDL_initParseResult(PartialInterfaceRest_p))
        }

        Nh_WebIDL_freeParseNode(PartialInterfaceMembers.Node_p);
    }

NH_WEBIDL_END(Nh_WebIDL_initParseResult(NULL))
}

static Nh_WebIDL_ParseResult Nh_WebIDL_parsePartialInterfaceOrPartialMixin(
    Nh_WebIDL_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    Nh_WebIDL_ParseResult PartialInterfaceRest = Nh_WebIDL_parsePartialInterfaceRest(Parser_p);

    if (PartialInterfaceRest.Node_p != NULL) 
    {
        Nh_WebIDL_ParseNode *PartialInterfaceOrPartialMixin_p = Nh_WebIDL_allocateNonTerminalParseNode("PartialInterfaceOrPartialMixin", 1);
        Nh_appendToList(&PartialInterfaceOrPartialMixin_p->Children, PartialInterfaceRest.Node_p);
        NH_WEBIDL_END(Nh_WebIDL_initParseResult(PartialInterfaceOrPartialMixin_p))
    }
    
    Nh_WebIDL_ParseResult MixinRest = Nh_WebIDL_parseMixinRest(Parser_p);

    if (MixinRest.Node_p != NULL) 
    {
        Nh_WebIDL_ParseNode *PartialInterfaceOrPartialMixin_p = Nh_WebIDL_allocateNonTerminalParseNode("PartialInterfaceOrPartialMixin", 1);
        Nh_appendToList(&PartialInterfaceOrPartialMixin_p->Children, MixinRest.Node_p);
        NH_WEBIDL_END(Nh_WebIDL_initParseResult(PartialInterfaceOrPartialMixin_p))
    }

NH_WEBIDL_END(Nh_WebIDL_initParseResult(NULL))
}

static Nh_WebIDL_ParseResult Nh_WebIDL_parseDictionaryMemberRest(
    Nh_WebIDL_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    if (Parser_p->unparsed > 0 && !strcmp(Parser_p->Tokens_p[0].String.bytes_p, "required")) 
    {
        Nh_WebIDL_Parser LocalParser = Nh_WebIDL_advanceParser(*Parser_p, 1);
        Nh_WebIDL_ParseResult TypeWithExtendedAttributes = Nh_WebIDL_parseTypeWithExtendedAttributes(&LocalParser); 

        if (TypeWithExtendedAttributes.Node_p != NULL && LocalParser.unparsed > 1 && LocalParser.Tokens_p[0].type == NH_WEBIDL_TOKEN_IDENTIFIER && LocalParser.Tokens_p[1].String.bytes_p[0] == ';') 
        {
            Nh_WebIDL_ParseNode *DictionaryMemberRest_p = Nh_WebIDL_allocateNonTerminalParseNode("DictionaryMemberRest", 4);
            Nh_appendToList(&DictionaryMemberRest_p->Children, Nh_WebIDL_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
            Nh_appendToList(&DictionaryMemberRest_p->Children, TypeWithExtendedAttributes.Node_p);
            Nh_appendToList(&DictionaryMemberRest_p->Children, Nh_WebIDL_allocateTerminalParseNode(&LocalParser.Tokens_p[0]));
            Nh_appendToList(&DictionaryMemberRest_p->Children, Nh_WebIDL_allocateTerminalParseNode(&LocalParser.Tokens_p[1]));
            *Parser_p = Nh_WebIDL_advanceParser(LocalParser, 2);
            NH_WEBIDL_END(Nh_WebIDL_initParseResult(DictionaryMemberRest_p))
        }

        Nh_WebIDL_freeParseNode(TypeWithExtendedAttributes.Node_p);
    }
    
    Nh_WebIDL_Parser LocalParser = *Parser_p;
    Nh_WebIDL_ParseResult Type = Nh_WebIDL_parseType(&LocalParser); 

    if (Type.Node_p != NULL && LocalParser.unparsed > 1 && LocalParser.Tokens_p[0].type == NH_WEBIDL_TOKEN_IDENTIFIER) 
    {
        Nh_WebIDL_Token *Identifier_p = &LocalParser.Tokens_p[0];
        LocalParser = Nh_WebIDL_advanceParser(LocalParser, 1);
        Nh_WebIDL_ParseResult Default = Nh_WebIDL_parseDefault(&LocalParser); 

        if (LocalParser.unparsed > 0 && LocalParser.Tokens_p[0].String.bytes_p[0] == ';')
        {
            Nh_WebIDL_ParseNode *DictionaryMemberRest_p = Nh_WebIDL_allocateNonTerminalParseNode("DictionaryMemberRest", 4);
            Nh_appendToList(&DictionaryMemberRest_p->Children, Type.Node_p);
            Nh_appendToList(&DictionaryMemberRest_p->Children, Nh_WebIDL_allocateTerminalParseNode(Identifier_p));
            Nh_appendToList(&DictionaryMemberRest_p->Children, Default.Node_p);
            Nh_appendToList(&DictionaryMemberRest_p->Children, Nh_WebIDL_allocateTerminalParseNode(&LocalParser.Tokens_p[0]));
            *Parser_p = Nh_WebIDL_advanceParser(LocalParser, 1);
            NH_WEBIDL_END(Nh_WebIDL_initParseResult(DictionaryMemberRest_p))
        }

        Nh_WebIDL_freeParseNode(Default.Node_p);
        Nh_WebIDL_freeParseNode(Type.Node_p);
    }

NH_WEBIDL_END(Nh_WebIDL_initParseResult(NULL))
}

static Nh_WebIDL_ParseResult Nh_WebIDL_parseDictionaryMember(
    Nh_WebIDL_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    Nh_WebIDL_Parser LocalParser = *Parser_p;
    Nh_WebIDL_ParseResult ExtendedAttributeList = Nh_WebIDL_parseExtendedAttributeList(&LocalParser);
    Nh_WebIDL_ParseResult DictionaryMemberRest = Nh_WebIDL_parseDictionaryMemberRest(&LocalParser);

    if (DictionaryMemberRest.Node_p != NULL) {
        Nh_WebIDL_ParseNode *DictionaryMember_p = Nh_WebIDL_allocateNonTerminalParseNode("DictionaryMember", 2);
        Nh_appendToList(&DictionaryMember_p->Children, ExtendedAttributeList.Node_p);
        Nh_appendToList(&DictionaryMember_p->Children, DictionaryMemberRest.Node_p);
        *Parser_p = LocalParser;
        NH_WEBIDL_END(Nh_WebIDL_initParseResult(DictionaryMember_p))
    }
    else {Nh_WebIDL_freeParseNode(ExtendedAttributeList.Node_p);}

NH_WEBIDL_END(Nh_WebIDL_initParseResult(NULL))
}

static Nh_WebIDL_ParseResult Nh_WebIDL_parseDictionaryMembers(
    Nh_WebIDL_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    Nh_WebIDL_ParseNode *DictionaryMembers_p = Nh_WebIDL_allocateNonTerminalParseNode("DictionaryMembers", 2);
    Nh_WebIDL_ParseResult DictionaryMember = Nh_WebIDL_parseDictionaryMember(Parser_p);

    if (DictionaryMember.Node_p != NULL) {
        Nh_WebIDL_ParseResult DictionaryMembers = Nh_WebIDL_parseDictionaryMembers(Parser_p);
        Nh_appendToList(&DictionaryMembers_p->Children, DictionaryMember.Node_p);
        Nh_appendToList(&DictionaryMembers_p->Children, DictionaryMembers.Node_p);
    }

NH_WEBIDL_END(Nh_WebIDL_initParseResult(DictionaryMembers_p))
}

static Nh_WebIDL_ParseResult Nh_WebIDL_parsePartialDictionary(
    Nh_WebIDL_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    if (Parser_p->unparsed > 2 && !strcmp(Parser_p->Tokens_p[0].String.bytes_p, "dictionary") && Parser_p->Tokens_p[1].type == NH_WEBIDL_TOKEN_IDENTIFIER && Parser_p->Tokens_p[2].String.bytes_p[0] == '{') 
    {
        Nh_WebIDL_Parser LocalParser = Nh_WebIDL_advanceParser(*Parser_p, 3);
        Nh_WebIDL_ParseResult DictionaryMembers = Nh_WebIDL_parseDictionaryMembers(&LocalParser); 

        if (LocalParser.unparsed > 1 && LocalParser.Tokens_p[0].String.bytes_p[0] == '}' && LocalParser.Tokens_p[1].String.bytes_p[0] == ';') 
        {
            Nh_WebIDL_ParseNode *PartialDictionary_p = Nh_WebIDL_allocateNonTerminalParseNode("PartialDictionary", 6);
            Nh_appendToList(&PartialDictionary_p->Children, Nh_WebIDL_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
            Nh_appendToList(&PartialDictionary_p->Children, Nh_WebIDL_allocateTerminalParseNode(&Parser_p->Tokens_p[1]));
            Nh_appendToList(&PartialDictionary_p->Children, Nh_WebIDL_allocateTerminalParseNode(&Parser_p->Tokens_p[2]));
            Nh_appendToList(&PartialDictionary_p->Children, DictionaryMembers.Node_p);
            Nh_appendToList(&PartialDictionary_p->Children, Nh_WebIDL_allocateTerminalParseNode(&LocalParser.Tokens_p[0]));
            Nh_appendToList(&PartialDictionary_p->Children, Nh_WebIDL_allocateTerminalParseNode(&LocalParser.Tokens_p[1]));
            *Parser_p = Nh_WebIDL_advanceParser(LocalParser, 2);
            NH_WEBIDL_END(Nh_WebIDL_initParseResult(PartialDictionary_p))
        }

        Nh_WebIDL_freeParseNode(DictionaryMembers.Node_p);
    }

NH_WEBIDL_END(Nh_WebIDL_initParseResult(NULL))
}

static Nh_WebIDL_ParseResult Nh_WebIDL_parsePartialDefinition(
    Nh_WebIDL_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    if (Parser_p->unparsed > 0 && !strcmp(Parser_p->Tokens_p[0].String.bytes_p, "interface")) 
    {
        Nh_WebIDL_Parser LocalParser = Nh_WebIDL_advanceParser(*Parser_p, 1);
        Nh_WebIDL_ParseResult PartialInterfaceOrPartialMixin = Nh_WebIDL_parsePartialInterfaceOrPartialMixin(&LocalParser); 

        if (PartialInterfaceOrPartialMixin.Node_p != NULL) 
        {
            Nh_WebIDL_ParseNode *PartialDefinition_p = Nh_WebIDL_allocateNonTerminalParseNode("PartialDefinition", 2);
            Nh_appendToList(&PartialDefinition_p->Children, Nh_WebIDL_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
            Nh_appendToList(&PartialDefinition_p->Children, PartialInterfaceOrPartialMixin.Node_p);
            *Parser_p = LocalParser;
            NH_WEBIDL_END(Nh_WebIDL_initParseResult(PartialDefinition_p))
        }
    }

    Nh_WebIDL_ParseResult PartialDictionary = Nh_WebIDL_parsePartialDictionary(Parser_p);

    if (PartialDictionary.Node_p != NULL) 
    {
        Nh_WebIDL_ParseNode *PartialDefinition_p = Nh_WebIDL_allocateNonTerminalParseNode("PartialDefinition", 1);
        Nh_appendToList(&PartialDefinition_p->Children, PartialDictionary.Node_p);
        NH_WEBIDL_END(Nh_WebIDL_initParseResult(PartialDefinition_p))
    }

    Nh_WebIDL_ParseResult Namespace = Nh_WebIDL_parseNamespace(Parser_p);

    if (Namespace.Node_p != NULL) 
    {
        Nh_WebIDL_ParseNode *PartialDefinition_p = Nh_WebIDL_allocateNonTerminalParseNode("PartialDefinition", 1);
        Nh_appendToList(&PartialDefinition_p->Children, Namespace.Node_p);
        NH_WEBIDL_END(Nh_WebIDL_initParseResult(PartialDefinition_p))
    }

NH_WEBIDL_END(Nh_WebIDL_initParseResult(NULL))
}

static Nh_WebIDL_ParseResult Nh_WebIDL_parsePartial(
    Nh_WebIDL_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    if (Parser_p->unparsed > 0 && !strcmp(Parser_p->Tokens_p[0].String.bytes_p, "partial")) 
    {
        Nh_WebIDL_Parser LocalParser = Nh_WebIDL_advanceParser(*Parser_p, 1);
        Nh_WebIDL_ParseResult PartialDefinition = Nh_WebIDL_parsePartialDefinition(&LocalParser); 

        if (PartialDefinition.Node_p != NULL) 
        {
            Nh_WebIDL_ParseNode *Partial_p = Nh_WebIDL_allocateNonTerminalParseNode("Partial", 2);
            Nh_appendToList(&Partial_p->Children, Nh_WebIDL_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
            Nh_appendToList(&Partial_p->Children, PartialDefinition.Node_p);
            *Parser_p = LocalParser;
            NH_WEBIDL_END(Nh_WebIDL_initParseResult(Partial_p))
        }
    }

NH_WEBIDL_END(Nh_WebIDL_initParseResult(NULL))
}

static Nh_WebIDL_ParseResult Nh_WebIDL_parseDictionary(
    Nh_WebIDL_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    if (Parser_p->unparsed > 2 && !strcmp(Parser_p->Tokens_p[0].String.bytes_p, "dictionary") && Parser_p->Tokens_p[1].type == NH_WEBIDL_TOKEN_IDENTIFIER) 
    {
        Nh_WebIDL_Parser LocalParser = Nh_WebIDL_advanceParser(*Parser_p, 2);
        Nh_WebIDL_ParseResult Inheritance = Nh_WebIDL_parseInheritance(&LocalParser); 

        if (LocalParser.unparsed > 2 && LocalParser.Tokens_p[0].String.bytes_p[0] == '{')
        {
            Nh_WebIDL_Token *OpenBracket_p = &LocalParser.Tokens_p[0];
            LocalParser = Nh_WebIDL_advanceParser(LocalParser, 1);
            Nh_WebIDL_ParseResult DictionaryMembers = Nh_WebIDL_parseDictionaryMembers(&LocalParser); 

            if (LocalParser.unparsed > 1 && LocalParser.Tokens_p[0].String.bytes_p[0] == '}' && LocalParser.Tokens_p[1].String.bytes_p[0] == ';') 
            {
                Nh_WebIDL_ParseNode *Dictionary_p = Nh_WebIDL_allocateNonTerminalParseNode("Dictionary", 7);
                Nh_appendToList(&Dictionary_p->Children, Nh_WebIDL_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
                Nh_appendToList(&Dictionary_p->Children, Nh_WebIDL_allocateTerminalParseNode(&Parser_p->Tokens_p[1]));
                Nh_appendToList(&Dictionary_p->Children, Inheritance.Node_p);
                Nh_appendToList(&Dictionary_p->Children, Nh_WebIDL_allocateTerminalParseNode(OpenBracket_p));
                Nh_appendToList(&Dictionary_p->Children, DictionaryMembers.Node_p);
                Nh_appendToList(&Dictionary_p->Children, Nh_WebIDL_allocateTerminalParseNode(&LocalParser.Tokens_p[0]));
                Nh_appendToList(&Dictionary_p->Children, Nh_WebIDL_allocateTerminalParseNode(&LocalParser.Tokens_p[1]));
                *Parser_p = Nh_WebIDL_advanceParser(LocalParser, 2);
                NH_WEBIDL_END(Nh_WebIDL_initParseResult(Dictionary_p))
            }
            Nh_WebIDL_freeParseNode(DictionaryMembers.Node_p);
        }
        Nh_WebIDL_freeParseNode(Inheritance.Node_p);
    }

NH_WEBIDL_END(Nh_WebIDL_initParseResult(NULL))
}

static Nh_WebIDL_ParseResult Nh_WebIDL_parseEnumValueListComma(
    Nh_WebIDL_Parser *Parser_p
);

static Nh_WebIDL_ParseResult Nh_WebIDL_parseEnumValueListString(
    Nh_WebIDL_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    Nh_WebIDL_ParseNode *EnumValueListString_p = Nh_WebIDL_allocateNonTerminalParseNode("EnumValueListString", 2);

    if (Parser_p->unparsed > 0 && Parser_p->Tokens_p[0].type == NH_WEBIDL_TOKEN_STRING) 
    {
        Nh_WebIDL_Parser LocalParser = Nh_WebIDL_advanceParser(*Parser_p, 1);
        Nh_WebIDL_ParseResult EnumValueListComma = Nh_WebIDL_parseEnumValueListComma(&LocalParser);
        Nh_appendToList(&EnumValueListString_p->Children, Nh_WebIDL_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
        Nh_appendToList(&EnumValueListString_p->Children, EnumValueListComma.Node_p);
        *Parser_p = LocalParser;
    }

NH_WEBIDL_END(Nh_WebIDL_initParseResult(EnumValueListString_p))
}

static Nh_WebIDL_ParseResult Nh_WebIDL_parseEnumValueListComma(
    Nh_WebIDL_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    Nh_WebIDL_ParseNode *EnumValueListComma_p = Nh_WebIDL_allocateNonTerminalParseNode("EnumValueListComma", 2);

    if (Parser_p->unparsed > 0 && Parser_p->Tokens_p[0].String.bytes_p[0] == ',') 
    {
        Nh_WebIDL_Parser LocalParser = Nh_WebIDL_advanceParser(*Parser_p, 1);
        Nh_WebIDL_ParseResult EnumValueListString = Nh_WebIDL_parseEnumValueListString(&LocalParser);
        Nh_appendToList(&EnumValueListComma_p->Children, Nh_WebIDL_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
        Nh_appendToList(&EnumValueListComma_p->Children, EnumValueListString.Node_p);
        *Parser_p = LocalParser;
    }

NH_WEBIDL_END(Nh_WebIDL_initParseResult(EnumValueListComma_p))
}

static Nh_WebIDL_ParseResult Nh_WebIDL_parseEnumValueList(
    Nh_WebIDL_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    if (Parser_p->unparsed > 0 && Parser_p->Tokens_p[0].type == NH_WEBIDL_TOKEN_STRING) 
    {
        Nh_WebIDL_Parser LocalParser = Nh_WebIDL_advanceParser(*Parser_p, 1);
        Nh_WebIDL_ParseResult EnumValueListComma = Nh_WebIDL_parseEnumValueListComma(&LocalParser);

        Nh_WebIDL_ParseNode *EnumValueList_p = Nh_WebIDL_allocateNonTerminalParseNode("EnumValueList", 2);
        Nh_appendToList(&EnumValueList_p->Children, Nh_WebIDL_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
        Nh_appendToList(&EnumValueList_p->Children, EnumValueListComma.Node_p);

        *Parser_p = LocalParser;

        NH_WEBIDL_END(Nh_WebIDL_initParseResult(EnumValueList_p))
    }

NH_WEBIDL_END(Nh_WebIDL_initParseResult(NULL))
}

static Nh_WebIDL_ParseResult Nh_WebIDL_parseEnum(
    Nh_WebIDL_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    if (Parser_p->unparsed > 2 && !strcmp(Parser_p->Tokens_p[0].String.bytes_p, "enum") && Parser_p->Tokens_p[1].type == NH_WEBIDL_TOKEN_IDENTIFIER && Parser_p->Tokens_p[2].String.bytes_p[0] == '{') 
    {
        Nh_WebIDL_Parser LocalParser = Nh_WebIDL_advanceParser(*Parser_p, 3);
        Nh_WebIDL_ParseResult EnumValueList = Nh_WebIDL_parseEnumValueList(&LocalParser); 

        if (EnumValueList.Node_p != NULL && LocalParser.unparsed > 1 && LocalParser.Tokens_p[0].String.bytes_p[0] == '}' && LocalParser.Tokens_p[1].String.bytes_p[0] == ';') 
        {
            Nh_WebIDL_ParseNode *Enum_p = Nh_WebIDL_allocateNonTerminalParseNode("Enum", 6);
            Nh_appendToList(&Enum_p->Children, Nh_WebIDL_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
            Nh_appendToList(&Enum_p->Children, Nh_WebIDL_allocateTerminalParseNode(&Parser_p->Tokens_p[1]));
            Nh_appendToList(&Enum_p->Children, Nh_WebIDL_allocateTerminalParseNode(&Parser_p->Tokens_p[1]));
            Nh_appendToList(&Enum_p->Children, EnumValueList.Node_p);
            Nh_appendToList(&Enum_p->Children, Nh_WebIDL_allocateTerminalParseNode(&LocalParser.Tokens_p[0]));
            Nh_appendToList(&Enum_p->Children, Nh_WebIDL_allocateTerminalParseNode(&LocalParser.Tokens_p[1]));
            *Parser_p = Nh_WebIDL_advanceParser(LocalParser, 2);
            NH_WEBIDL_END(Nh_WebIDL_initParseResult(Enum_p))
        }
    }

NH_WEBIDL_END(Nh_WebIDL_initParseResult(NULL))
}

static Nh_WebIDL_ParseResult Nh_WebIDL_parseTypedef(
    Nh_WebIDL_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    if (Parser_p->unparsed > 0 && !strcmp(Parser_p->Tokens_p[0].String.bytes_p, "typedef")) 
    {
        Nh_WebIDL_Parser LocalParser = Nh_WebIDL_advanceParser(*Parser_p, 1);
        Nh_WebIDL_ParseResult TypeWithExtendedAttributes = Nh_WebIDL_parseTypeWithExtendedAttributes(&LocalParser); 

        if (TypeWithExtendedAttributes.Node_p != NULL && LocalParser.unparsed > 1 && LocalParser.Tokens_p[0].type == NH_WEBIDL_TOKEN_IDENTIFIER && LocalParser.Tokens_p[1].String.bytes_p[0] == ';') 
        {
            Nh_WebIDL_ParseNode *Typedef_p = Nh_WebIDL_allocateNonTerminalParseNode("Typedef", 4);
            Nh_appendToList(&Typedef_p->Children, Nh_WebIDL_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
            Nh_appendToList(&Typedef_p->Children, TypeWithExtendedAttributes.Node_p);
            Nh_appendToList(&Typedef_p->Children, Nh_WebIDL_allocateTerminalParseNode(&LocalParser.Tokens_p[0]));
            Nh_appendToList(&Typedef_p->Children, Nh_WebIDL_allocateTerminalParseNode(&LocalParser.Tokens_p[1]));
            *Parser_p = Nh_WebIDL_advanceParser(LocalParser, 2);
            NH_WEBIDL_END(Nh_WebIDL_initParseResult(Typedef_p))
        }

        Nh_WebIDL_freeParseNode(TypeWithExtendedAttributes.Node_p);
    }

NH_WEBIDL_END(Nh_WebIDL_initParseResult(NULL))
}

static Nh_WebIDL_ParseResult Nh_WebIDL_parseIncludesStatement(
    Nh_WebIDL_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    if (Parser_p->unparsed > 3 
    &&  Parser_p->Tokens_p[0].type == NH_WEBIDL_TOKEN_IDENTIFIER 
    &&  !strcmp(Parser_p->Tokens_p[1].String.bytes_p, "includes")
    &&  Parser_p->Tokens_p[2].type == NH_WEBIDL_TOKEN_IDENTIFIER 
    &&  Parser_p->Tokens_p[3].String.bytes_p[0] == ';')
    {
        Nh_WebIDL_ParseNode *IncludesStatement_p = Nh_WebIDL_allocateNonTerminalParseNode("IncludesStatement", 4);
        Nh_appendToList(&IncludesStatement_p->Children, Nh_WebIDL_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
        Nh_appendToList(&IncludesStatement_p->Children, Nh_WebIDL_allocateTerminalParseNode(&Parser_p->Tokens_p[1]));
        Nh_appendToList(&IncludesStatement_p->Children, Nh_WebIDL_allocateTerminalParseNode(&Parser_p->Tokens_p[2]));
        Nh_appendToList(&IncludesStatement_p->Children, Nh_WebIDL_allocateTerminalParseNode(&Parser_p->Tokens_p[3]));
        *Parser_p = Nh_WebIDL_advanceParser(*Parser_p, 4);
        NH_WEBIDL_END(Nh_WebIDL_initParseResult(IncludesStatement_p))
    }

NH_WEBIDL_END(Nh_WebIDL_initParseResult(NULL))
}

static Nh_WebIDL_ParseResult Nh_WebIDL_parseDefinition(
    Nh_WebIDL_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    Nh_WebIDL_ParseResult CallbackOrInterfaceOrMixin = Nh_WebIDL_parseCallbackOrInterfaceOrMixin(Parser_p);

    if (CallbackOrInterfaceOrMixin.Node_p != NULL) 
    {
        Nh_WebIDL_ParseNode *Definition_p = Nh_WebIDL_allocateNonTerminalParseNode("Definition", 1);
        Nh_appendToList(&Definition_p->Children, CallbackOrInterfaceOrMixin.Node_p);
        NH_WEBIDL_END(Nh_WebIDL_initParseResult(Definition_p))
    }

    Nh_WebIDL_ParseResult Namespace = Nh_WebIDL_parseNamespace(Parser_p);

    if (Namespace.Node_p != NULL) 
    {
        Nh_WebIDL_ParseNode *Definition_p = Nh_WebIDL_allocateNonTerminalParseNode("Definition", 1);
        Nh_appendToList(&Definition_p->Children, Namespace.Node_p);
        NH_WEBIDL_END(Nh_WebIDL_initParseResult(Definition_p))
    }

    Nh_WebIDL_ParseResult Partial = Nh_WebIDL_parsePartial(Parser_p);

    if (Partial.Node_p != NULL) 
    {
        Nh_WebIDL_ParseNode *Definition_p = Nh_WebIDL_allocateNonTerminalParseNode("Definition", 1);
        Nh_appendToList(&Definition_p->Children, Partial.Node_p);
        NH_WEBIDL_END(Nh_WebIDL_initParseResult(Definition_p))
    }

    Nh_WebIDL_ParseResult Dictionary = Nh_WebIDL_parseDictionary(Parser_p);

    if (Dictionary.Node_p != NULL) 
    {
        Nh_WebIDL_ParseNode *Definition_p = Nh_WebIDL_allocateNonTerminalParseNode("Definition", 1);
        Nh_appendToList(&Definition_p->Children, Dictionary.Node_p);
        NH_WEBIDL_END(Nh_WebIDL_initParseResult(Definition_p))
    }

    Nh_WebIDL_ParseResult Enum = Nh_WebIDL_parseEnum(Parser_p);

    if (Enum.Node_p != NULL) 
    {
        Nh_WebIDL_ParseNode *Definition_p = Nh_WebIDL_allocateNonTerminalParseNode("Definition", 1);
        Nh_appendToList(&Definition_p->Children, Enum.Node_p);
        NH_WEBIDL_END(Nh_WebIDL_initParseResult(Definition_p))
    }

    Nh_WebIDL_ParseResult Typedef = Nh_WebIDL_parseTypedef(Parser_p);

    if (Typedef.Node_p != NULL) 
    {
        Nh_WebIDL_ParseNode *Definition_p = Nh_WebIDL_allocateNonTerminalParseNode("Definition", 1);
        Nh_appendToList(&Definition_p->Children, Typedef.Node_p);
        NH_WEBIDL_END(Nh_WebIDL_initParseResult(Definition_p))
    }

    Nh_WebIDL_ParseResult IncludesStatement = Nh_WebIDL_parseIncludesStatement(Parser_p);

    if (IncludesStatement.Node_p != NULL) 
    {
        Nh_WebIDL_ParseNode *Definition_p = Nh_WebIDL_allocateNonTerminalParseNode("Definition", 1);
        Nh_appendToList(&Definition_p->Children, IncludesStatement.Node_p);
        NH_WEBIDL_END(Nh_WebIDL_initParseResult(Definition_p))
    }

NH_WEBIDL_END(Nh_WebIDL_initParseResult(NULL))
}

static Nh_WebIDL_ParseResult Nh_WebIDL_parseDefinitions(
    Nh_WebIDL_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    Nh_WebIDL_ParseNode *Definitions_p = Nh_WebIDL_allocateNonTerminalParseNode("Definitions", 3);

    Nh_WebIDL_ParseResult ExtendedAttributeList = Nh_WebIDL_parseExtendedAttributeList(Parser_p);
    Nh_WebIDL_ParseResult Definition = Nh_WebIDL_parseDefinition(Parser_p);

    if (Definition.Node_p != NULL) 
    {
        Nh_appendToList(&Definitions_p->Children, ExtendedAttributeList.Node_p);
        Nh_appendToList(&Definitions_p->Children, Definition.Node_p);
        Nh_WebIDL_ParseResult Definitions = Nh_WebIDL_parseDefinitions(Parser_p);
        Nh_appendToList(&Definitions_p->Children, Definitions.Node_p);
    }
    else {Nh_WebIDL_freeParseNode(ExtendedAttributeList.Node_p);}

NH_WEBIDL_END(Nh_WebIDL_initParseResult(Definitions_p))
}

// PARSE ===========================================================================================

Nh_WebIDL_FragmentParseResult Nh_WebIDL_parse(
    NH_BYTE *logName_p, NH_BYTE *fragment_p)
{
NH_WEBIDL_BEGIN()

    Nh_Array Tokens = Nh_WebIDL_tokenizeFragment(logName_p, fragment_p);

    Nh_WebIDL_Parser Parser;
    Parser.Tokens_p = (Nh_WebIDL_Token*)Tokens.bytes_p;
    Parser.unparsed = Tokens.length;

    Nh_WebIDL_ParseResult Definitions = Nh_WebIDL_parseDefinitions(&Parser);

    Nh_WebIDL_FragmentParseResult Result;
    Result.Root_p = Definitions.Node_p;
    Result.SyntaxErrors = Definitions.SyntaxErrors;
    Result.Tokens = Tokens;

    Nh_WebIDL_logParseTree(logName_p, Result.Root_p);

NH_WEBIDL_END(Result)
}

// UNPARSE =========================================================================================

static void Nh_WebIDL_unparseParseNode(
    Nh_WebIDL_ParseNode *Node_p)
{
NH_WEBIDL_BEGIN()

    for (int i = 0; i < Node_p->Children.size; ++i) {
        Nh_WebIDL_unparseParseNode(Node_p->Children.pp[i]);
    }

    Nh_freeList(&Node_p->Children, NH_FALSE);
    Nh_free(Node_p);

NH_WEBIDL_SILENT_END()
}

void Nh_WebIDL_unparse(
    Nh_WebIDL_FragmentParseResult ParseResult)
{
NH_WEBIDL_BEGIN()

    Nh_WebIDL_freeTokens(ParseResult.Tokens);
    Nh_WebIDL_unparseParseNode(ParseResult.Root_p);

NH_WEBIDL_SILENT_END()
}

// INIT ============================================================================================

Nh_WebIDL_FragmentParseResult Nh_WebIDL_initFragmentParseResult()
{
NH_WEBIDL_BEGIN()

    Nh_WebIDL_FragmentParseResult Result;
    Result.Root_p = NULL;
    Result.SyntaxErrors = Nh_initList(8);
    Result.Tokens = Nh_initArray(sizeof(Nh_WebIDL_Token), 64);

NH_WEBIDL_END(Result)
}

// GET =============================================================================================

NH_WEBIDL_RESULT Nh_WebIDL_getParseNodes(
    Nh_WebIDL_ParseNode *Node_p, NH_WEBIDL_PARSE_NODE type, Nh_List *Nodes_p)
{
NH_WEBIDL_BEGIN()

    if (Node_p->type == type) {
        Nh_appendToList(Nodes_p, Node_p);
    }

    for (int i = 0; i < Node_p->Children.size; ++i) {
        Nh_WebIDL_getParseNodes(Node_p->Children.pp[i], type, Nodes_p);
    }

NH_WEBIDL_DIAGNOSTIC_END(NH_WEBIDL_SUCCESS)
}


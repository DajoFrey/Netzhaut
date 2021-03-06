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

#include NH_WEB_DEFAULT_CHECK
#include NH_WEB_FLOW

#include "../../NhCore/Memory.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// DATA ============================================================================================

const NH_BYTE *NH_WEB_PARSE_NODE_NAMES_PP[] = {
    "Definitions",
    "Definition",
    "ArgumentNameKeyword",
    "CallbackOrClassOrMixin",
    "ClassOrMixin",
    "ClassRest",
    "Partial",
    "PartialDefinition",
    "PartialClassOrPartialMixin",
    "PartialClassRest",
    "ClassMembers",
    "ClassMember",
    "PartialClassMembers",
    "PartialClassMember",
    "Inheritance",
    "MixinRest",
    "MixinMembers",
    "MixinMember",
    "IncludesStatement",
    "CallbackRestOrClass",
    "CallbackClassMembers",
    "CallbackClassMember",
    "Const",
    "ConstValue",
    "BooleanLiteral",
    "FloatLiteral",
    "ConstType",
    "ReadOnlyMember",
    "ReadOnlyMemberRest",
    "ReadWriteAttribute",
    "InheritAttribute",
    "PrivateAttribute",
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
};

size_t NH_WEB_PARSE_NODE_NAMES_PP_COUNT = sizeof(NH_WEB_PARSE_NODE_NAMES_PP) / sizeof(NH_WEB_PARSE_NODE_NAMES_PP[0]);

// DECLARE =========================================================================================

typedef struct Nh_Web_ParseResult {
    Nh_Web_ParseNode *Node_p;
    Nh_List SyntaxErrors;
} Nh_Web_ParseResult;

typedef struct Nh_Web_Parser {
    Nh_Web_Token *Tokens_p;
    int unparsed;
} Nh_Web_Parser;

static Nh_Web_ParseResult Nh_Web_parseExtendedAttribute(
    Nh_Web_Parser *Parser_p
);
static Nh_Web_ParseResult Nh_Web_parseType(
    Nh_Web_Parser *Parser_p
);
static Nh_Web_ParseResult Nh_Web_parseTypeWithExtendedAttributes(
    Nh_Web_Parser *Parser_p
);

// HELPER ==========================================================================================

static Nh_Web_ParseResult Nh_Web_initParseResult(
    Nh_Web_ParseNode *Node_p)
{
NH_WEB_BEGIN()

    Nh_Web_ParseResult Result;
    Result.Node_p = Node_p;
    Result.SyntaxErrors = Nh_initList(8);

NH_WEB_END(Result)
}

static Nh_Web_ParseNode *Nh_Web_allocateNonTerminalParseNode(
    const NH_BYTE *name_p, int chunkSize)
{
NH_WEB_BEGIN()

    unsigned int *index_p = Nh_getFromHashMap(&NH_WEB_INDEXMAP.ParseNodeNames, (NH_BYTE*)name_p);
    if (index_p == NULL) {NH_WEB_END(NULL)}

    Nh_Web_ParseNode *Node_p = Nh_allocate(sizeof(Nh_Web_ParseNode));
    Node_p->Children = Nh_initList(chunkSize);
    Node_p->Token_p  = NULL;
    Node_p->type     = *index_p;

NH_WEB_END(Node_p)
}

static Nh_Web_ParseNode *Nh_Web_allocateTerminalParseNode(
    Nh_Web_Token *Token_p)
{
NH_WEB_BEGIN()

    Nh_Web_ParseNode *Node_p = Nh_allocate(sizeof(Nh_Web_ParseNode));
    Node_p->Children = Nh_initList(1);
    Node_p->Token_p  = Token_p;
    Node_p->type     = -1;

NH_WEB_END(Node_p)
}

static void Nh_Web_freeParseNode(
    Nh_Web_ParseNode *Node_p)
{
NH_WEB_BEGIN()

    // TODO

NH_WEB_SILENT_END()
}

static Nh_Web_Parser Nh_Web_advanceParser(
    Nh_Web_Parser Parser, int steps)
{
NH_WEB_BEGIN()

    Parser.unparsed -= steps;
    Parser.Tokens_p  = Parser.Tokens_p + steps;
 
NH_WEB_END(Parser)
}

// PARSER ==========================================================================================

static Nh_Web_ParseResult Nh_Web_parseOther(
    Nh_Web_Parser *Parser_p)
{
NH_WEB_BEGIN()

    if (Parser_p->unparsed == 0 
    ||  Parser_p->Tokens_p[0].String.bytes_p[0] == ','
    ||  Parser_p->Tokens_p[0].String.bytes_p[0] == '('
    ||  Parser_p->Tokens_p[0].String.bytes_p[0] == '['
    ||  Parser_p->Tokens_p[0].String.bytes_p[0] == '{'
    ||  Parser_p->Tokens_p[0].String.bytes_p[0] == ')'
    ||  Parser_p->Tokens_p[0].String.bytes_p[0] == ']'
    ||  Parser_p->Tokens_p[0].String.bytes_p[0] == '}') 
    {
        NH_WEB_END(Nh_Web_initParseResult(NULL))   
    }

    Nh_Web_ParseNode *Other_p = Nh_Web_allocateNonTerminalParseNode("Other", 1);
    Nh_Web_ParseNode *Child_p = Nh_Web_allocateTerminalParseNode(&Parser_p->Tokens_p[0]);
    Nh_appendToList(&Other_p->Children, Child_p);
    *Parser_p = Nh_Web_advanceParser(*Parser_p, 1);

NH_WEB_END(Nh_Web_initParseResult(Other_p))
}

static Nh_Web_ParseResult Nh_Web_parseOtherOrComma(
    Nh_Web_Parser *Parser_p)
{
NH_WEB_BEGIN()

    Nh_Web_ParseResult Result = Nh_Web_parseOther(Parser_p);

    if (Result.Node_p != NULL) {
        Nh_Web_ParseNode *OtherOrComma_p = Nh_Web_allocateNonTerminalParseNode("OtherOrComma", 1);
        Nh_appendToList(&OtherOrComma_p->Children, Result.Node_p);
        NH_WEB_END(Nh_Web_initParseResult(OtherOrComma_p))
    } 
    else if (Parser_p->unparsed > 0 && Parser_p->Tokens_p[0].String.bytes_p[0] == ',') 
    {
        Nh_Web_ParseNode *OtherOrComma_p = Nh_Web_allocateNonTerminalParseNode("OtherOrComma", 1);
        Nh_Web_ParseNode *Child_p = Nh_Web_allocateTerminalParseNode(&Parser_p->Tokens_p[0]);
        Nh_appendToList(&OtherOrComma_p->Children, Child_p);
        *Parser_p = Nh_Web_advanceParser(*Parser_p, 1);
        NH_WEB_END(Nh_Web_initParseResult(OtherOrComma_p))
    }

NH_WEB_END(Nh_Web_initParseResult(NULL))
}

static Nh_Web_ParseResult Nh_Web_parseExtendedAttributeInner(
    Nh_Web_Parser *Parser_p)
{
NH_WEB_BEGIN()

    NH_BYTE openBrackets_p[3] = "([{";
    NH_BYTE closeBrackets_p[3] = ")]}";

    for (int i = 0; i < 3; ++i) 
    {
        if (Parser_p->unparsed > 0 && Parser_p->Tokens_p[0].String.bytes_p[0] == openBrackets_p[i]) 
        {
            Nh_Web_Parser LocalParser = *Parser_p;
            Nh_Web_ParseResult Result1 = Nh_Web_parseExtendedAttributeInner(&LocalParser);
    
            if (LocalParser.unparsed > 0 && LocalParser.Tokens_p[0].String.bytes_p[0] == closeBrackets_p[i]) 
            {
                Nh_Web_ParseResult Result2 = Nh_Web_parseExtendedAttributeInner(&LocalParser);
                Nh_Web_ParseNode *ExtendedAttributeInner_p = Nh_Web_allocateNonTerminalParseNode("ExtendedAttributeInner", 4);
                Nh_appendToList(&ExtendedAttributeInner_p->Children, Nh_Web_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
                Nh_appendToList(&ExtendedAttributeInner_p->Children, Result1.Node_p);
                Nh_appendToList(&ExtendedAttributeInner_p->Children, Nh_Web_allocateTerminalParseNode(&LocalParser.Tokens_p[0]));
                Nh_appendToList(&ExtendedAttributeInner_p->Children, Result2.Node_p);
                *Parser_p = Nh_Web_advanceParser(LocalParser, 1);
                NH_WEB_END(Nh_Web_initParseResult(ExtendedAttributeInner_p))
            }
            else {Nh_Web_freeParseNode(Result1.Node_p);}
        }
    }

    Nh_Web_ParseNode *ExtendedAttributeInner_p = Nh_Web_allocateNonTerminalParseNode("ExtendedAttributeInner", 1);
    Nh_Web_ParseResult Result = Nh_Web_parseOtherOrComma(Parser_p);

    if (Result.Node_p != NULL) {
        Nh_appendToList(&ExtendedAttributeInner_p->Children, Result.Node_p);
        Nh_Web_ParseResult Result = Nh_Web_parseExtendedAttributeInner(Parser_p);
        Nh_appendToList(&ExtendedAttributeInner_p->Children, Result.Node_p);
    } 

NH_WEB_END(Nh_Web_initParseResult(ExtendedAttributeInner_p))
}

static Nh_Web_ParseResult Nh_Web_parseExtendedAttributeRest(
    Nh_Web_Parser *Parser_p)
{
NH_WEB_BEGIN()

    Nh_Web_ParseResult Result = Nh_Web_parseExtendedAttribute(Parser_p);
    Nh_Web_ParseNode *ExtendedAttributeRest_p = Nh_Web_allocateNonTerminalParseNode("ExtendedAttributeRest", 1);

    if (Result.Node_p != NULL) {
        Nh_appendToList(&ExtendedAttributeRest_p->Children, Result.Node_p);
    } 

NH_WEB_END(Nh_Web_initParseResult(ExtendedAttributeRest_p))
}

static Nh_Web_ParseResult Nh_Web_parseExtendedAttribute(
    Nh_Web_Parser *Parser_p)
{
NH_WEB_BEGIN()

    NH_BYTE openBrackets_p[3] = "([{";
    NH_BYTE closeBrackets_p[3] = ")]}";

    for (int i = 0; i < 3; ++i) 
    {
        if (Parser_p->unparsed > 0 && Parser_p->Tokens_p[0].String.bytes_p[0] == openBrackets_p[i]) 
        {
            Nh_Web_Parser LocalParser = Nh_Web_advanceParser(*Parser_p, 1);
            Nh_Web_ParseResult ExtendedAttributeInner = Nh_Web_parseExtendedAttributeInner(&LocalParser);
    
            if (LocalParser.unparsed > 0 && LocalParser.Tokens_p[0].String.bytes_p[0] == closeBrackets_p[i]) 
            {
                Nh_Web_ParseResult ExtendedAttributeRest = Nh_Web_parseExtendedAttributeRest(&LocalParser);
                Nh_Web_ParseNode *ExtendedAttribute_p = Nh_Web_allocateNonTerminalParseNode("ExtendedAttribute", 4);

                Nh_appendToList(&ExtendedAttribute_p->Children, Nh_Web_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
                Nh_appendToList(&ExtendedAttribute_p->Children, ExtendedAttributeInner.Node_p);
                Nh_appendToList(&ExtendedAttribute_p->Children, Nh_Web_allocateTerminalParseNode(&LocalParser.Tokens_p[0]));
                Nh_appendToList(&ExtendedAttribute_p->Children, ExtendedAttributeRest.Node_p);

                *Parser_p = Nh_Web_advanceParser(LocalParser, 1);
                NH_WEB_END(Nh_Web_initParseResult(ExtendedAttribute_p))
            }
            else {Nh_Web_freeParseNode(ExtendedAttributeInner.Node_p);}
        }
    }

    Nh_Web_ParseResult Other = Nh_Web_parseOther(Parser_p);

    if (Other.Node_p != NULL) 
    {
        Nh_Web_ParseResult ExtendedAttributeRest = Nh_Web_parseExtendedAttributeRest(Parser_p);
        Nh_Web_ParseNode *ExtendedAttribute_p = Nh_Web_allocateNonTerminalParseNode("ExtendedAttribute", 2);
        Nh_appendToList(&ExtendedAttribute_p->Children, Other.Node_p);
        Nh_appendToList(&ExtendedAttribute_p->Children, ExtendedAttributeRest.Node_p);
        NH_WEB_END(Nh_Web_initParseResult(ExtendedAttribute_p))
    } 

NH_WEB_END(Nh_Web_initParseResult(NULL))
}

static Nh_Web_ParseResult Nh_Web_parseExtendedAttributes(
    Nh_Web_Parser *Parser_p)
{
NH_WEB_BEGIN()

    Nh_Web_ParseNode *ExtendedAttributes_p = Nh_Web_allocateNonTerminalParseNode("ExtendedAttributes", 2);

    if (Parser_p->unparsed > 0 && Parser_p->Tokens_p[0].String.bytes_p[0] == ',') 
    {
        Nh_Web_Parser LocalParser = Nh_Web_advanceParser(*Parser_p, 1);
        Nh_Web_ParseResult ExtendedAttribute = Nh_Web_parseExtendedAttribute(&LocalParser);

        if (ExtendedAttribute.Node_p != NULL) 
        {
            Nh_Web_ParseResult ExtendedAttributes = Nh_Web_parseExtendedAttributes(&LocalParser);
            Nh_appendToList(&ExtendedAttributes_p->Children, Nh_Web_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
            Nh_appendToList(&ExtendedAttributes_p->Children, ExtendedAttribute.Node_p);
            Nh_appendToList(&ExtendedAttributes_p->Children, ExtendedAttributes.Node_p);
            *Parser_p = LocalParser;
        }
    }

NH_WEB_END(Nh_Web_initParseResult(ExtendedAttributes_p))
}

static Nh_Web_ParseResult Nh_Web_parseExtendedAttributeList(
    Nh_Web_Parser *Parser_p)
{
NH_WEB_BEGIN()

    Nh_Web_ParseNode *ExtendedAttributeList_p = Nh_Web_allocateNonTerminalParseNode("ExtendedAttributeList", 1);

    if (Parser_p->unparsed > 0 && Parser_p->Tokens_p[0].String.bytes_p[0] == '[') 
    {
        Nh_Web_Parser LocalParser = Nh_Web_advanceParser(*Parser_p, 1);
        Nh_Web_ParseResult ExtendedAttribute = Nh_Web_parseExtendedAttribute(&LocalParser);

        if (ExtendedAttribute.Node_p != NULL) 
        {
            Nh_Web_ParseResult ExtendedAttributes = Nh_Web_parseExtendedAttributes(&LocalParser);

            if (LocalParser.unparsed > 0 && LocalParser.Tokens_p[0].String.bytes_p[0] == ']') 
            {
                Nh_appendToList(&ExtendedAttributeList_p->Children, Nh_Web_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
                Nh_appendToList(&ExtendedAttributeList_p->Children, ExtendedAttribute.Node_p);
                Nh_appendToList(&ExtendedAttributeList_p->Children, ExtendedAttributes.Node_p);
                Nh_appendToList(&ExtendedAttributeList_p->Children, Nh_Web_allocateTerminalParseNode(&LocalParser.Tokens_p[0]));
                *Parser_p = Nh_Web_advanceParser(LocalParser, 1);
            }
            else {
                Nh_Web_freeParseNode(ExtendedAttributes.Node_p);
                Nh_Web_freeParseNode(ExtendedAttribute.Node_p);
            }
        }
    }

NH_WEB_END(Nh_Web_initParseResult(ExtendedAttributeList_p))
}

static Nh_Web_ParseResult Nh_Web_parseOptionalLong(
    Nh_Web_Parser *Parser_p)
{
NH_WEB_BEGIN()

    Nh_Web_ParseNode *OptionalLong_p = Nh_Web_allocateNonTerminalParseNode("OptionalLong", 1);

    if (!strcmp(Parser_p->Tokens_p[0].String.bytes_p, "long")) 
    {
        Nh_appendToList(&OptionalLong_p->Children, Nh_Web_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
        *Parser_p = Nh_Web_advanceParser(*Parser_p, 1);
    }

NH_WEB_END(Nh_Web_initParseResult(OptionalLong_p))
}

static Nh_Web_ParseResult Nh_Web_parseIntegerType(
    Nh_Web_Parser *Parser_p)
{
NH_WEB_BEGIN()

    if (!strcmp(Parser_p->Tokens_p[0].String.bytes_p, "short"))
    {
        Nh_Web_ParseNode *IntegerType_p = Nh_Web_allocateNonTerminalParseNode("IntegerType", 1);
        Nh_appendToList(&IntegerType_p->Children, Nh_Web_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
        *Parser_p = Nh_Web_advanceParser(*Parser_p, 1);
        NH_WEB_END(Nh_Web_initParseResult(IntegerType_p))
    }

    if (!strcmp(Parser_p->Tokens_p[0].String.bytes_p, "long")) 
    {
        Nh_Web_ParseNode *IntegerType_p = Nh_Web_allocateNonTerminalParseNode("IntegerType", 1);
        Nh_appendToList(&IntegerType_p->Children, Nh_Web_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
        *Parser_p = Nh_Web_advanceParser(*Parser_p, 1);
        Nh_Web_ParseResult OptionalLong = Nh_Web_parseOptionalLong(Parser_p);
        Nh_appendToList(&IntegerType_p->Children, OptionalLong.Node_p);
        NH_WEB_END(Nh_Web_initParseResult(IntegerType_p))
    }

NH_WEB_END(Nh_Web_initParseResult(NULL))
}

static Nh_Web_ParseResult Nh_Web_parseUnsignedIntegerType(
    Nh_Web_Parser *Parser_p)
{
NH_WEB_BEGIN()

    if (!strcmp(Parser_p->Tokens_p[0].String.bytes_p, "unsigned"))
    {
        Nh_Web_Parser LocalParser = Nh_Web_advanceParser(*Parser_p, 1);
        Nh_Web_ParseResult IntegerType = Nh_Web_parseIntegerType(&LocalParser);

        if (IntegerType.Node_p != NULL) 
        {
            Nh_Web_ParseNode *UnsignedIntegerType_p = Nh_Web_allocateNonTerminalParseNode("UnsignedIntegerType", 1);
            Nh_Web_ParseNode *Unsigned_p = Nh_Web_allocateTerminalParseNode(&Parser_p->Tokens_p[0]);
            Nh_appendToList(&UnsignedIntegerType_p->Children, Unsigned_p); 
            Nh_appendToList(&UnsignedIntegerType_p->Children, IntegerType.Node_p); 
            *Parser_p = LocalParser;
            NH_WEB_END(Nh_Web_initParseResult(UnsignedIntegerType_p))
        }
    }

    Nh_Web_ParseResult IntegerType = Nh_Web_parseIntegerType(Parser_p);
    if (IntegerType.Node_p != NULL) 
    {
        Nh_Web_ParseNode *UnsignedIntegerType_p = Nh_Web_allocateNonTerminalParseNode("UnsignedIntegerType", 1);
        Nh_appendToList(&UnsignedIntegerType_p->Children, IntegerType.Node_p);
        NH_WEB_END(Nh_Web_initParseResult(UnsignedIntegerType_p))
    }

NH_WEB_END(Nh_Web_initParseResult(NULL))
}

static Nh_Web_ParseResult Nh_Web_parseFloatType(
    Nh_Web_Parser *Parser_p)
{
NH_WEB_BEGIN()

    if (!strcmp(Parser_p->Tokens_p[0].String.bytes_p, "float")
    ||  !strcmp(Parser_p->Tokens_p[0].String.bytes_p, "double"))
    {
        Nh_Web_ParseNode *FloatType_p = Nh_Web_allocateNonTerminalParseNode("FloatType", 1);
        Nh_Web_ParseNode *Terminal_p = Nh_Web_allocateTerminalParseNode(&Parser_p->Tokens_p[0]);
        Nh_appendToList(&FloatType_p->Children, Terminal_p);
        *Parser_p = Nh_Web_advanceParser(*Parser_p, 1);
        NH_WEB_END(Nh_Web_initParseResult(FloatType_p))
    }

NH_WEB_END(Nh_Web_initParseResult(NULL))
}

static Nh_Web_ParseResult Nh_Web_parseUnrestrictedFloatType(
    Nh_Web_Parser *Parser_p)
{
NH_WEB_BEGIN()

    if (!strcmp(Parser_p->Tokens_p[0].String.bytes_p, "unrestricted"))
    {
        Nh_Web_Parser LocalParser = Nh_Web_advanceParser(*Parser_p, 1);
        Nh_Web_ParseResult FloatType = Nh_Web_parseFloatType(&LocalParser);

        if (FloatType.Node_p != NULL) 
        {
            Nh_Web_ParseNode *UnrestrictedFloatType_p = Nh_Web_allocateNonTerminalParseNode("UnrestrictedFloatType", 1);
            Nh_Web_ParseNode *Unrestricted_p = Nh_Web_allocateTerminalParseNode(&Parser_p->Tokens_p[0]);
            Nh_appendToList(&UnrestrictedFloatType_p->Children, Unrestricted_p); 
            Nh_appendToList(&UnrestrictedFloatType_p->Children, FloatType.Node_p); 
            *Parser_p = LocalParser;
            NH_WEB_END(Nh_Web_initParseResult(UnrestrictedFloatType_p))
        }
    }

    Nh_Web_ParseResult FloatType = Nh_Web_parseFloatType(Parser_p);
    if (FloatType.Node_p != NULL) 
    {
        Nh_Web_ParseNode *UnrestrictedFloatType_p = Nh_Web_allocateNonTerminalParseNode("UnrestrictedFloatType", 1);
        Nh_appendToList(&UnrestrictedFloatType_p->Children, FloatType.Node_p);
        NH_WEB_END(Nh_Web_initParseResult(UnrestrictedFloatType_p))
    }

NH_WEB_END(Nh_Web_initParseResult(NULL))
}

static Nh_Web_ParseResult Nh_Web_parsePrimitiveType(
    Nh_Web_Parser *Parser_p)
{
NH_WEB_BEGIN()

    if (Parser_p->unparsed == 0) {NH_WEB_END(Nh_Web_initParseResult(NULL))}

    if (!strcmp(Parser_p->Tokens_p[0].String.bytes_p, "undefined")
    ||  !strcmp(Parser_p->Tokens_p[0].String.bytes_p, "boolean")
    ||  !strcmp(Parser_p->Tokens_p[0].String.bytes_p, "byte")
    ||  !strcmp(Parser_p->Tokens_p[0].String.bytes_p, "octet")
    ||  !strcmp(Parser_p->Tokens_p[0].String.bytes_p, "bigint")) 
    {
        Nh_Web_ParseNode *PrimitiveType_p = Nh_Web_allocateNonTerminalParseNode("PrimitiveType", 1);
        Nh_appendToList(&PrimitiveType_p->Children, Nh_Web_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
        *Parser_p = Nh_Web_advanceParser(*Parser_p, 1); 
        NH_WEB_END(Nh_Web_initParseResult(PrimitiveType_p))
    }

    Nh_Web_ParseResult UnsignedIntegerType = Nh_Web_parseUnsignedIntegerType(Parser_p);
    if (UnsignedIntegerType.Node_p != NULL) 
    {
        Nh_Web_ParseNode *PrimitiveType_p = Nh_Web_allocateNonTerminalParseNode("PrimitiveType", 1);
        Nh_appendToList(&PrimitiveType_p->Children, UnsignedIntegerType.Node_p);
        NH_WEB_END(Nh_Web_initParseResult(PrimitiveType_p))
    }

    Nh_Web_ParseResult UnrestrictedFloatType = Nh_Web_parseUnrestrictedFloatType(Parser_p);
    if (UnrestrictedFloatType.Node_p != NULL) 
    {
        Nh_Web_ParseNode *PrimitiveType_p = Nh_Web_allocateNonTerminalParseNode("PrimitiveType", 1);
        Nh_appendToList(&PrimitiveType_p->Children, UnrestrictedFloatType.Node_p);
        NH_WEB_END(Nh_Web_initParseResult(PrimitiveType_p))
    }

NH_WEB_END(Nh_Web_initParseResult(NULL))
}

static Nh_Web_ParseResult Nh_Web_parseNull(
    Nh_Web_Parser *Parser_p)
{
NH_WEB_BEGIN()

    Nh_Web_ParseNode *Null_p = Nh_Web_allocateNonTerminalParseNode("Null", 1);

    if (Parser_p->Tokens_p[0].String.bytes_p[0] == '?') {
        Nh_appendToList(&Null_p->Children, Nh_Web_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
        *Parser_p = Nh_Web_advanceParser(*Parser_p, 1);
    }

NH_WEB_END(Nh_Web_initParseResult(Null_p))
}

static Nh_Web_ParseResult Nh_Web_parseStringType(
    Nh_Web_Parser *Parser_p)
{
NH_WEB_BEGIN()

    if (!strcmp(Parser_p->Tokens_p[0].String.bytes_p, "ByteString") 
    ||  !strcmp(Parser_p->Tokens_p[0].String.bytes_p, "DOMString") 
    ||  !strcmp(Parser_p->Tokens_p[0].String.bytes_p, "USVString")) 
    {
        Nh_Web_ParseNode *StringType_p = Nh_Web_allocateNonTerminalParseNode("StringType", 1);
        Nh_appendToList(&StringType_p->Children, Nh_Web_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
        *Parser_p = Nh_Web_advanceParser(*Parser_p, 1);
        NH_WEB_END(Nh_Web_initParseResult(StringType_p))
    }

NH_WEB_END(Nh_Web_initParseResult(NULL))
}

static Nh_Web_ParseResult Nh_Web_parseBufferRelatedType(
    Nh_Web_Parser *Parser_p)
{
NH_WEB_BEGIN()

    if (Parser_p->unparsed == 0) {NH_WEB_END(Nh_Web_initParseResult(NULL))}
    
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
        Nh_Web_ParseNode *BufferRelatedType_p = Nh_Web_allocateNonTerminalParseNode("BufferRelatedType", 1);
        Nh_appendToList(&BufferRelatedType_p->Children, Nh_Web_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
        *Parser_p = Nh_Web_advanceParser(*Parser_p, 1);
        NH_WEB_END(Nh_Web_initParseResult(BufferRelatedType_p))
    }

NH_WEB_END(Nh_Web_initParseResult(NULL))
}

static Nh_Web_ParseResult Nh_Web_parseRecordType(
    Nh_Web_Parser *Parser_p)
{
NH_WEB_BEGIN()

    if (Parser_p->unparsed <= 4) {NH_WEB_END(Nh_Web_initParseResult(NULL))}

    if (!strcmp(Parser_p->Tokens_p[0].String.bytes_p, "record") && Parser_p->Tokens_p[1].String.bytes_p[0] == '<')
    {
        Nh_Web_Parser LocalParser = Nh_Web_advanceParser(*Parser_p, 2);
        Nh_Web_ParseResult StringType = Nh_Web_parseStringType(&LocalParser);

        if (StringType.Node_p != NULL && LocalParser.unparsed > 2 && LocalParser.Tokens_p[0].String.bytes_p[0] == ',')
        {
            Nh_Web_Token *Comma_p = &LocalParser.Tokens_p[0];
            LocalParser = Nh_Web_advanceParser(LocalParser, 1);
            Nh_Web_ParseResult TypeWithExtendedAttributes = Nh_Web_parseTypeWithExtendedAttributes(&LocalParser);

            if (TypeWithExtendedAttributes.Node_p != NULL && LocalParser.unparsed >= 1 && LocalParser.Tokens_p[0].String.bytes_p[0] == '>')
            {
                Nh_Web_ParseNode *RecordType_p = Nh_Web_allocateNonTerminalParseNode("RecordType", 6);
                Nh_appendToList(&RecordType_p->Children, Nh_Web_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
                Nh_appendToList(&RecordType_p->Children, Nh_Web_allocateTerminalParseNode(&Parser_p->Tokens_p[1]));
                Nh_appendToList(&RecordType_p->Children, StringType.Node_p);
                Nh_appendToList(&RecordType_p->Children, Nh_Web_allocateTerminalParseNode(Comma_p));
                Nh_appendToList(&RecordType_p->Children, TypeWithExtendedAttributes.Node_p);
                Nh_appendToList(&RecordType_p->Children, Nh_Web_allocateTerminalParseNode(&LocalParser.Tokens_p[0]));
                *Parser_p = Nh_Web_advanceParser(LocalParser, 1);
                NH_WEB_END(Nh_Web_initParseResult(RecordType_p))
            }
        }
    }

NH_WEB_END(Nh_Web_initParseResult(NULL))
}

static Nh_Web_ParseResult Nh_Web_parseDistinguishableType(
    Nh_Web_Parser *Parser_p)
{
NH_WEB_BEGIN()

    Nh_Web_ParseResult PrimitiveType = Nh_Web_parsePrimitiveType(Parser_p);
    if (PrimitiveType.Node_p != NULL) 
    {
        Nh_Web_ParseNode *DistinguishableType_p = Nh_Web_allocateNonTerminalParseNode("DistinguishableType", 1);
        Nh_appendToList(&DistinguishableType_p->Children, PrimitiveType.Node_p);
        Nh_Web_ParseResult Null = Nh_Web_parseNull(Parser_p);
        Nh_appendToList(&DistinguishableType_p->Children, Null.Node_p);
        NH_WEB_END(Nh_Web_initParseResult(DistinguishableType_p))
    } 
    
    Nh_Web_ParseResult StringType = Nh_Web_parseStringType(Parser_p);
    if (StringType.Node_p != NULL) 
    {
        Nh_Web_ParseNode *DistinguishableType_p = Nh_Web_allocateNonTerminalParseNode("DistinguishableType", 1);
        Nh_appendToList(&DistinguishableType_p->Children, StringType.Node_p);
        Nh_Web_ParseResult Null = Nh_Web_parseNull(Parser_p);
        Nh_appendToList(&DistinguishableType_p->Children, Null.Node_p);
        NH_WEB_END(Nh_Web_initParseResult(DistinguishableType_p))
    } 

    Nh_Web_ParseResult BufferRelatedType = Nh_Web_parseStringType(Parser_p);
    if (BufferRelatedType.Node_p != NULL) 
    {
        Nh_Web_ParseNode *DistinguishableType_p = Nh_Web_allocateNonTerminalParseNode("DistinguishableType", 1);
        Nh_appendToList(&DistinguishableType_p->Children, BufferRelatedType.Node_p);
        Nh_Web_ParseResult Null = Nh_Web_parseNull(Parser_p);
        Nh_appendToList(&DistinguishableType_p->Children, Null.Node_p);
        NH_WEB_END(Nh_Web_initParseResult(DistinguishableType_p))
    } 

    Nh_Web_ParseResult RecordType = Nh_Web_parseRecordType(Parser_p);
    if (RecordType.Node_p != NULL) 
    {
        Nh_Web_ParseNode *DistinguishableType_p = Nh_Web_allocateNonTerminalParseNode("DistinguishableType", 1);
        Nh_appendToList(&DistinguishableType_p->Children, RecordType.Node_p);
        Nh_Web_ParseResult Null = Nh_Web_parseNull(Parser_p);
        Nh_appendToList(&DistinguishableType_p->Children, Null.Node_p);
        NH_WEB_END(Nh_Web_initParseResult(DistinguishableType_p))
    } 

    if (Parser_p->unparsed > 3)
    {
        if (!strcmp(Parser_p->Tokens_p[0].String.bytes_p, "sequence") && Parser_p->Tokens_p[1].String.bytes_p[0] == '<'
        ||  !strcmp(Parser_p->Tokens_p[0].String.bytes_p, "FrozenArray") && Parser_p->Tokens_p[1].String.bytes_p[0] == '<'
        ||  !strcmp(Parser_p->Tokens_p[0].String.bytes_p, "ObservableArray") && Parser_p->Tokens_p[1].String.bytes_p[0] == '<')
        {
            Nh_Web_Parser LocalParser = Nh_Web_advanceParser(*Parser_p, 2);
            Nh_Web_ParseResult TypeWithExtendedAttributes = Nh_Web_parseTypeWithExtendedAttributes(&LocalParser);

            if (TypeWithExtendedAttributes.Node_p != NULL && LocalParser.unparsed > 0 && LocalParser.Tokens_p[0].String.bytes_p[0] == '>')
            {
                Nh_Web_ParseNode *DistinguishableType_p = Nh_Web_allocateNonTerminalParseNode("DistinguishableType", 5);
                Nh_appendToList(&DistinguishableType_p->Children, Nh_Web_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
                Nh_appendToList(&DistinguishableType_p->Children, Nh_Web_allocateTerminalParseNode(&Parser_p->Tokens_p[1]));
                Nh_appendToList(&DistinguishableType_p->Children, TypeWithExtendedAttributes.Node_p);
                Nh_appendToList(&DistinguishableType_p->Children, Nh_Web_allocateTerminalParseNode(&LocalParser.Tokens_p[0]));
                *Parser_p = Nh_Web_advanceParser(LocalParser, 1);
                Nh_Web_ParseResult Null = Nh_Web_parseNull(Parser_p);
                Nh_appendToList(&DistinguishableType_p->Children, Null.Node_p);
                NH_WEB_END(Nh_Web_initParseResult(DistinguishableType_p))
            }
        }
    }

    if (Parser_p->unparsed > 0 && Parser_p->Tokens_p[0].type == NH_WEB_TOKEN_IDENTIFIER) 
    {
        Nh_Web_ParseNode *DistinguishableType_p = Nh_Web_allocateNonTerminalParseNode("DistinguishableType", 1);
        Nh_appendToList(&DistinguishableType_p->Children, Nh_Web_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
        *Parser_p = Nh_Web_advanceParser(*Parser_p, 1);
        Nh_Web_ParseResult Null = Nh_Web_parseNull(Parser_p);
        Nh_appendToList(&DistinguishableType_p->Children, Null.Node_p);
        NH_WEB_END(Nh_Web_initParseResult(DistinguishableType_p))
    }

NH_WEB_END(Nh_Web_initParseResult(NULL))
}

static Nh_Web_ParseResult Nh_Web_parsePromiseType(
    Nh_Web_Parser *Parser_p)
{
NH_WEB_BEGIN()

    if (Parser_p->unparsed > 3 && !strcmp(Parser_p->Tokens_p[0].String.bytes_p, "Promise") && Parser_p->Tokens_p[1].String.bytes_p[0] == '<') 
    {
        Nh_Web_Parser LocalParser = Nh_Web_advanceParser(*Parser_p, 2);
        Nh_Web_ParseResult Type = Nh_Web_parseType(&LocalParser);

        if (Type.Node_p != NULL && LocalParser.unparsed > 0 && LocalParser.Tokens_p[0].String.bytes_p[0] == '>')
        {
            Nh_Web_ParseNode *PromiseType_p = Nh_Web_allocateNonTerminalParseNode("PromiseType", 4);
            Nh_appendToList(&PromiseType_p->Children, Nh_Web_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
            Nh_appendToList(&PromiseType_p->Children, Nh_Web_allocateTerminalParseNode(&Parser_p->Tokens_p[1]));
            Nh_appendToList(&PromiseType_p->Children, Type.Node_p);
            Nh_appendToList(&PromiseType_p->Children, Nh_Web_allocateTerminalParseNode(&LocalParser.Tokens_p[0]));
            *Parser_p = Nh_Web_advanceParser(LocalParser, 1);
            NH_WEB_END(Nh_Web_initParseResult(PromiseType_p))
        }
    }

NH_WEB_END(Nh_Web_initParseResult(NULL))
}

static Nh_Web_ParseResult Nh_Web_parseWildcardType(
    Nh_Web_Parser *Parser_p)
{
NH_WEB_BEGIN()

    if (Parser_p->unparsed > 0 && Parser_p->Tokens_p[0].String.bytes_p[0] == '*') 
    {
        Nh_Web_ParseNode *WildcardType_p = Nh_Web_allocateNonTerminalParseNode("WildcardType", 1);
        Nh_appendToList(&WildcardType_p->Children, Nh_Web_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
        *Parser_p = Nh_Web_advanceParser(*Parser_p, 1);
        NH_WEB_END(Nh_Web_initParseResult(WildcardType_p))
    }

NH_WEB_END(Nh_Web_initParseResult(NULL))
}

static Nh_Web_ParseResult Nh_Web_parseSingleType(
    Nh_Web_Parser *Parser_p)
{
NH_WEB_BEGIN()

    Nh_Web_ParseResult DistinguishableType = Nh_Web_parseDistinguishableType(Parser_p);
    if (DistinguishableType.Node_p != NULL) 
    {
        Nh_Web_ParseNode *SingleType_p = Nh_Web_allocateNonTerminalParseNode("SingleType", 1);
        Nh_appendToList(&SingleType_p->Children, DistinguishableType.Node_p);
        NH_WEB_END(Nh_Web_initParseResult(SingleType_p))
    }
 
    if (Parser_p->unparsed > 0 && (!strcmp(Parser_p->Tokens_p[0].String.bytes_p, "any") || Parser_p->Tokens_p[0].String.bytes_p[0] == '*'))
    {
        Nh_Web_ParseNode *SingleType_p = Nh_Web_allocateNonTerminalParseNode("SingleType", 1);
        Nh_appendToList(&SingleType_p->Children, Nh_Web_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
        *Parser_p = Nh_Web_advanceParser(*Parser_p, 1);
        NH_WEB_END(Nh_Web_initParseResult(SingleType_p))
    }

    Nh_Web_ParseResult PromiseType = Nh_Web_parsePromiseType(Parser_p);
    if (PromiseType.Node_p != NULL) 
    {
        Nh_Web_ParseNode *SingleType_p = Nh_Web_allocateNonTerminalParseNode("SingleType", 1);
        Nh_appendToList(&SingleType_p->Children, PromiseType.Node_p);
        NH_WEB_END(Nh_Web_initParseResult(SingleType_p))
    } 

NH_WEB_END(Nh_Web_initParseResult(NULL))
}

static Nh_Web_ParseResult Nh_Web_parseUnionType(
    Nh_Web_Parser *Parser_p
);

static Nh_Web_ParseResult Nh_Web_parseUnionMemberType(
    Nh_Web_Parser *Parser_p)
{
NH_WEB_BEGIN()

    Nh_Web_ParseResult ExtendedAttributeList = Nh_Web_parseExtendedAttributeList(Parser_p);
    Nh_Web_ParseResult DistinguishableType = Nh_Web_parseDistinguishableType(Parser_p);

    if (DistinguishableType.Node_p != NULL) 
    {
        Nh_Web_ParseNode *UnionMemberType_p = Nh_Web_allocateNonTerminalParseNode("UnionMemberType", 2);
        Nh_appendToList(&UnionMemberType_p->Children, ExtendedAttributeList.Node_p);
        Nh_appendToList(&UnionMemberType_p->Children, DistinguishableType.Node_p);
        NH_WEB_END(Nh_Web_initParseResult(UnionMemberType_p))
    }
    else {Nh_Web_freeParseNode(DistinguishableType.Node_p);}

    Nh_Web_ParseResult UnionType = Nh_Web_parseUnionType(Parser_p);

    if (UnionType.Node_p != NULL)
    {
        Nh_Web_ParseResult Null = Nh_Web_parseNull(Parser_p);
        Nh_Web_ParseNode *UnionMemberType_p = Nh_Web_allocateNonTerminalParseNode("UnionMemberType", 2);
        Nh_appendToList(&UnionMemberType_p->Children, UnionType.Node_p);
        Nh_appendToList(&UnionMemberType_p->Children, Null.Node_p);
        NH_WEB_END(Nh_Web_initParseResult(UnionMemberType_p))
    }

NH_WEB_END(Nh_Web_initParseResult(NULL))
}

static Nh_Web_ParseResult Nh_Web_parseUnionMemberTypes(
    Nh_Web_Parser *Parser_p)
{
NH_WEB_BEGIN()

    Nh_Web_ParseNode *UnionMemberTypes_p = Nh_Web_allocateNonTerminalParseNode("UnionMemberTypes", 3);

    if (Parser_p->unparsed > 0 && !strcmp(Parser_p->Tokens_p[0].String.bytes_p, "or")) 
    {
        Nh_Web_Parser LocalParser = Nh_Web_advanceParser(*Parser_p, 1);
        Nh_Web_ParseResult UnionMemberType = Nh_Web_parseUnionMemberType(&LocalParser);

        if (UnionMemberType.Node_p != NULL) 
        {
            Nh_Web_ParseResult UnionMemberTypes = Nh_Web_parseUnionMemberTypes(&LocalParser);
            Nh_appendToList(&UnionMemberTypes_p->Children, Nh_Web_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
            Nh_appendToList(&UnionMemberTypes_p->Children, UnionMemberType.Node_p);
            Nh_appendToList(&UnionMemberTypes_p->Children, UnionMemberTypes.Node_p);
            *Parser_p = LocalParser;
        }
    }

NH_WEB_END(Nh_Web_initParseResult(UnionMemberTypes_p))
}

static Nh_Web_ParseResult Nh_Web_parseUnionType(
    Nh_Web_Parser *Parser_p)
{
NH_WEB_BEGIN()

    if (Parser_p->unparsed > 0 && Parser_p->Tokens_p[0].String.bytes_p[0] == '(') 
    {
        Nh_Web_Parser LocalParser = Nh_Web_advanceParser(*Parser_p, 1);
        Nh_Web_ParseResult UnionMemberType1 = Nh_Web_parseUnionMemberType(&LocalParser);

        if (UnionMemberType1.Node_p != NULL && LocalParser.unparsed > 0 && !strcmp(LocalParser.Tokens_p[0].String.bytes_p, "or")) 
        {
            Nh_Web_Token *OrToken_p = &LocalParser.Tokens_p[0];
            LocalParser = Nh_Web_advanceParser(LocalParser, 1);
            Nh_Web_ParseResult UnionMemberType2 = Nh_Web_parseUnionMemberType(&LocalParser);

            if (UnionMemberType2.Node_p != NULL) 
            {
                Nh_Web_ParseResult UnionMemberTypes = Nh_Web_parseUnionMemberTypes(&LocalParser);
                if (LocalParser.unparsed > 0 && LocalParser.Tokens_p[0].String.bytes_p[0] == ')') 
                {
                    Nh_Web_ParseNode *UnionType_p = Nh_Web_allocateNonTerminalParseNode("UnionType", 6);
                    Nh_appendToList(&UnionType_p->Children, Nh_Web_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
                    Nh_appendToList(&UnionType_p->Children, UnionMemberType1.Node_p);
                    Nh_appendToList(&UnionType_p->Children, Nh_Web_allocateTerminalParseNode(OrToken_p));
                    Nh_appendToList(&UnionType_p->Children, UnionMemberType2.Node_p);
                    Nh_appendToList(&UnionType_p->Children, UnionMemberTypes.Node_p);
                    Nh_appendToList(&UnionType_p->Children, Nh_Web_allocateTerminalParseNode(&LocalParser.Tokens_p[0]));
                    *Parser_p = Nh_Web_advanceParser(LocalParser, 1);
                    NH_WEB_END(Nh_Web_initParseResult(UnionType_p))
                }
            }
        }
    }

NH_WEB_END(Nh_Web_initParseResult(NULL))
}

static Nh_Web_ParseResult Nh_Web_parseType(
    Nh_Web_Parser *Parser_p)
{
NH_WEB_BEGIN()

    Nh_Web_ParseResult SingleType = Nh_Web_parseSingleType(Parser_p);
    if (SingleType.Node_p != NULL) 
    {
        Nh_Web_ParseNode *Type_p = Nh_Web_allocateNonTerminalParseNode("Type", 1);
        Nh_appendToList(&Type_p->Children, SingleType.Node_p);
        NH_WEB_END(Nh_Web_initParseResult(Type_p))
    } 
   
    Nh_Web_ParseResult UnionType = Nh_Web_parseUnionType(Parser_p);
    if (UnionType.Node_p != NULL) 
    {
        Nh_Web_ParseResult Null = Nh_Web_parseNull(Parser_p);
        Nh_Web_ParseNode *Type_p = Nh_Web_allocateNonTerminalParseNode("Type", 1);
        Nh_appendToList(&Type_p->Children, UnionType.Node_p);
        Nh_appendToList(&Type_p->Children, Null.Node_p);
        NH_WEB_END(Nh_Web_initParseResult(Type_p))
    } 

NH_WEB_END(Nh_Web_initParseResult(NULL))
}

static Nh_Web_ParseResult Nh_Web_parseTypeWithExtendedAttributes(
    Nh_Web_Parser *Parser_p)
{
NH_WEB_BEGIN()

    Nh_Web_Parser LocalParser = *Parser_p;

    Nh_Web_ParseResult ExtendedAttributeList = Nh_Web_parseExtendedAttributeList(&LocalParser);
    Nh_Web_ParseResult Type = Nh_Web_parseType(&LocalParser);

    if (Type.Node_p != NULL) 
    {
        Nh_Web_ParseNode *TypeWithExtendedAttributes_p = Nh_Web_allocateNonTerminalParseNode("TypeWithExtendedAttributes", 2);
        Nh_appendToList(&TypeWithExtendedAttributes_p->Children, ExtendedAttributeList.Node_p);
        Nh_appendToList(&TypeWithExtendedAttributes_p->Children, Type.Node_p);
        *Parser_p = LocalParser;
        NH_WEB_END(Nh_Web_initParseResult(TypeWithExtendedAttributes_p))
    }

    Nh_Web_freeParseNode(ExtendedAttributeList.Node_p);

NH_WEB_END(Nh_Web_initParseResult(NULL))
}

static Nh_Web_ParseResult Nh_Web_parseArgumentNameKeyword(
    Nh_Web_Parser *Parser_p)
{
NH_WEB_BEGIN()

    if (Parser_p->unparsed == 0) {NH_WEB_END(Nh_Web_initParseResult(NULL))}

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
    ||  !strcmp(Parser_p->Tokens_p[0].String.bytes_p, "class")
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
        Nh_Web_ParseNode *ArgumentNameKeyword_p = Nh_Web_allocateNonTerminalParseNode("ArgumentNameKeyword", 1);
        Nh_appendToList(&ArgumentNameKeyword_p->Children, Nh_Web_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
        *Parser_p = Nh_Web_advanceParser(*Parser_p, 1);
        NH_WEB_END(Nh_Web_initParseResult(ArgumentNameKeyword_p))
    }

NH_WEB_END(Nh_Web_initParseResult(NULL))
}

static Nh_Web_ParseResult Nh_Web_parseArgumentName(
    Nh_Web_Parser *Parser_p)
{
NH_WEB_BEGIN()

    Nh_Web_ParseResult ArgumentNameKeyword = Nh_Web_parseArgumentNameKeyword(Parser_p);

    if (ArgumentNameKeyword.Node_p != NULL) 
    {
        Nh_Web_ParseNode *ArgumentName_p = Nh_Web_allocateNonTerminalParseNode("ArgumentName", 1);
        Nh_appendToList(&ArgumentName_p->Children, ArgumentNameKeyword.Node_p);
        NH_WEB_END(Nh_Web_initParseResult(ArgumentName_p))
    }

    if (Parser_p->unparsed > 0 && Parser_p->Tokens_p[0].type == NH_WEB_TOKEN_IDENTIFIER) 
    {
        Nh_Web_ParseNode *ArgumentName_p = Nh_Web_allocateNonTerminalParseNode("ArgumentName", 1);
        Nh_appendToList(&ArgumentName_p->Children, Nh_Web_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
        *Parser_p = Nh_Web_advanceParser(*Parser_p, 1);
        NH_WEB_END(Nh_Web_initParseResult(ArgumentName_p))
    }

NH_WEB_END(Nh_Web_initParseResult(NULL))
}

static Nh_Web_ParseResult Nh_Web_parseBooleanLiteral(
    Nh_Web_Parser *Parser_p)
{
NH_WEB_BEGIN()

    if (Parser_p->unparsed == 0) {NH_WEB_END(Nh_Web_initParseResult(NULL))}

    if (!strcmp(Parser_p->Tokens_p[0].String.bytes_p, "true")
    ||  !strcmp(Parser_p->Tokens_p[0].String.bytes_p, "false"))
    {
        Nh_Web_ParseNode *BooleanLiteral_p = Nh_Web_allocateNonTerminalParseNode("BooleanLiteral", 1);
        Nh_appendToList(&BooleanLiteral_p->Children, Nh_Web_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
        *Parser_p = Nh_Web_advanceParser(*Parser_p, 1);
        NH_WEB_END(Nh_Web_initParseResult(BooleanLiteral_p))
    }

NH_WEB_END(Nh_Web_initParseResult(NULL))
}

static Nh_Web_ParseResult Nh_Web_parseFloatLiteral(
    Nh_Web_Parser *Parser_p)
{
NH_WEB_BEGIN()

    if (Parser_p->unparsed == 0) {NH_WEB_END(Nh_Web_initParseResult(NULL))}

    if (!strcmp(Parser_p->Tokens_p[0].String.bytes_p, "-Infinity")
    ||  !strcmp(Parser_p->Tokens_p[0].String.bytes_p, "Infinity")
    ||  !strcmp(Parser_p->Tokens_p[0].String.bytes_p, "NaN")
    ||  Nh_Web_isNumericToken(&Parser_p->Tokens_p[0]))
    {
        Nh_Web_ParseNode *FloatLiteral_p = Nh_Web_allocateNonTerminalParseNode("FloatLiteral", 1);
        Nh_appendToList(&FloatLiteral_p->Children, Nh_Web_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
        *Parser_p = Nh_Web_advanceParser(*Parser_p, 1);
        NH_WEB_END(Nh_Web_initParseResult(FloatLiteral_p))
    }

NH_WEB_END(Nh_Web_initParseResult(NULL))
}

static Nh_Web_ParseResult Nh_Web_parseConstValue(
    Nh_Web_Parser *Parser_p)
{
NH_WEB_BEGIN()

    if (Parser_p->unparsed == 0) {NH_WEB_END(Nh_Web_initParseResult(NULL))}

    Nh_Web_ParseResult BooleanLiteral = Nh_Web_parseBooleanLiteral(Parser_p);
    if (BooleanLiteral.Node_p != NULL) {
        Nh_Web_ParseNode *ConstValue_p = Nh_Web_allocateNonTerminalParseNode("ConstValue", 1);
        Nh_appendToList(&ConstValue_p->Children, BooleanLiteral.Node_p);
        NH_WEB_END(Nh_Web_initParseResult(ConstValue_p))
    }

    Nh_Web_ParseResult FloatLiteral = Nh_Web_parseFloatLiteral(Parser_p);
    if (FloatLiteral.Node_p != NULL) {
        Nh_Web_ParseNode *ConstValue_p = Nh_Web_allocateNonTerminalParseNode("ConstValue", 1);
        Nh_appendToList(&ConstValue_p->Children, FloatLiteral.Node_p);
        NH_WEB_END(Nh_Web_initParseResult(ConstValue_p))
    }

    if (Nh_Web_isNumericToken(&Parser_p->Tokens_p[0])) 
    {
        Nh_Web_ParseNode *ConstValue_p = Nh_Web_allocateNonTerminalParseNode("ConstValue", 1);
        Nh_appendToList(&ConstValue_p->Children, Nh_Web_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
        *Parser_p = Nh_Web_advanceParser(*Parser_p, 1);
        NH_WEB_END(Nh_Web_initParseResult(ConstValue_p))
    }

NH_WEB_END(Nh_Web_initParseResult(NULL))
}

static Nh_Web_ParseResult Nh_Web_parseDefaultValue(
    Nh_Web_Parser *Parser_p)
{
NH_WEB_BEGIN()

    if (Parser_p->unparsed == 0) {NH_WEB_END(Nh_Web_initParseResult(NULL))}

    Nh_Web_ParseResult ConstValue = Nh_Web_parseConstValue(Parser_p);
    if (ConstValue.Node_p != NULL) 
    {
        Nh_Web_ParseNode *DefaultValue_p = Nh_Web_allocateNonTerminalParseNode("DefaultValue", 1);
        Nh_appendToList(&DefaultValue_p->Children, ConstValue.Node_p);
        NH_WEB_END(Nh_Web_initParseResult(DefaultValue_p))
    }

    if (!strcmp(Parser_p->Tokens_p[0].String.bytes_p, "null")) 
    {
        Nh_Web_ParseNode *DefaultValue_p = Nh_Web_allocateNonTerminalParseNode("DefaultValue", 1);
        Nh_appendToList(&DefaultValue_p->Children, Nh_Web_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
        *Parser_p = Nh_Web_advanceParser(*Parser_p, 1);
        NH_WEB_END(Nh_Web_initParseResult(DefaultValue_p))
    }

    if (Parser_p->Tokens_p[0].type == NH_WEB_TOKEN_STRING) 
    {
        Nh_Web_ParseNode *DefaultValue_p = Nh_Web_allocateNonTerminalParseNode("DefaultValue", 1);
        Nh_appendToList(&DefaultValue_p->Children, Nh_Web_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
        *Parser_p = Nh_Web_advanceParser(*Parser_p, 1);
        NH_WEB_END(Nh_Web_initParseResult(DefaultValue_p))
    }

    if (Parser_p->unparsed <= 1) {NH_WEB_END(Nh_Web_initParseResult(NULL))}

    if ((Parser_p->Tokens_p[0].String.bytes_p[0] == '[' && Parser_p->Tokens_p[1].String.bytes_p[0] == ']')
    ||  (Parser_p->Tokens_p[0].String.bytes_p[0] == '{' && Parser_p->Tokens_p[1].String.bytes_p[0] == '}'))
    {
        Nh_Web_ParseNode *DefaultValue_p = Nh_Web_allocateNonTerminalParseNode("DefaultValue", 1);
        Nh_appendToList(&DefaultValue_p->Children, Nh_Web_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
        Nh_appendToList(&DefaultValue_p->Children, Nh_Web_allocateTerminalParseNode(&Parser_p->Tokens_p[1]));
        *Parser_p = Nh_Web_advanceParser(*Parser_p, 2);
        NH_WEB_END(Nh_Web_initParseResult(DefaultValue_p))
    }

NH_WEB_END(Nh_Web_initParseResult(NULL))
}

static Nh_Web_ParseResult Nh_Web_parseDefault(
    Nh_Web_Parser *Parser_p)
{
NH_WEB_BEGIN()

    Nh_Web_ParseNode *Default_p = Nh_Web_allocateNonTerminalParseNode("Default", 1);

    if (Parser_p->unparsed > 0 && Parser_p->Tokens_p[0].String.bytes_p[0] == '=') 
    {
        Nh_Web_Parser LocalParser = Nh_Web_advanceParser(*Parser_p, 1);
        Nh_Web_ParseResult DefaultValue = Nh_Web_parseDefaultValue(&LocalParser);

        if (DefaultValue.Node_p != NULL) 
        {
            Nh_appendToList(&Default_p->Children, Nh_Web_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
            Nh_appendToList(&Default_p->Children, DefaultValue.Node_p);
            *Parser_p = LocalParser;    
        }
    } 

NH_WEB_END(Nh_Web_initParseResult(Default_p))
}

static Nh_Web_ParseResult Nh_Web_parseEllipsis(
    Nh_Web_Parser *Parser_p)
{
NH_WEB_BEGIN()

    Nh_Web_ParseNode *Ellipsis_p = Nh_Web_allocateNonTerminalParseNode("Ellipsis", 1);

    if (Parser_p->unparsed > 0 && !strcmp(Parser_p->Tokens_p[0].String.bytes_p, "...")) 
    {
        Nh_Web_ParseNode *Terminal_p = Nh_Web_allocateTerminalParseNode(&Parser_p->Tokens_p[0]);
        Nh_appendToList(&Ellipsis_p->Children, Terminal_p);
        *Parser_p = Nh_Web_advanceParser(*Parser_p, 1);
    }

NH_WEB_END(Nh_Web_initParseResult(Ellipsis_p))
}

static Nh_Web_ParseResult Nh_Web_parseArgumentRest(
    Nh_Web_Parser *Parser_p)
{
NH_WEB_BEGIN()

    if (Parser_p->unparsed > 0 && !strcmp("optional", Parser_p->Tokens_p[0].String.bytes_p)) 
    {
        Nh_Web_Parser LocalParser = Nh_Web_advanceParser(*Parser_p, 1); 
        Nh_Web_ParseResult TypeWithExtendedAttributes = Nh_Web_parseTypeWithExtendedAttributes(&LocalParser);

        if (TypeWithExtendedAttributes.Node_p != NULL) 
        {
            Nh_Web_ParseResult ArgumentName = Nh_Web_parseArgumentName(&LocalParser);

            if (ArgumentName.Node_p != NULL) 
            {
                Nh_Web_ParseResult Default = Nh_Web_parseDefault(&LocalParser);
                Nh_Web_ParseNode *ArgumentRest_p = Nh_Web_allocateNonTerminalParseNode("ArgumentRest", 4);
                Nh_appendToList(&ArgumentRest_p->Children, Nh_Web_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
                Nh_appendToList(&ArgumentRest_p->Children, TypeWithExtendedAttributes.Node_p);
                Nh_appendToList(&ArgumentRest_p->Children, ArgumentName.Node_p);
                Nh_appendToList(&ArgumentRest_p->Children, Default.Node_p);
                *Parser_p = LocalParser;
                NH_WEB_END(Nh_Web_initParseResult(ArgumentRest_p))
            }
            Nh_Web_freeParseNode(TypeWithExtendedAttributes.Node_p);
        }
        NH_WEB_END(Nh_Web_initParseResult(NULL))
    }

    Nh_Web_Parser LocalParser = *Parser_p;
    Nh_Web_ParseResult Type = Nh_Web_parseType(&LocalParser);

    if (Type.Node_p != NULL) 
    {
        Nh_Web_ParseResult Ellipsis = Nh_Web_parseEllipsis(&LocalParser);
        Nh_Web_ParseResult ArgumentName = Nh_Web_parseArgumentName(&LocalParser);

        if (ArgumentName.Node_p != NULL) 
        {
            Nh_Web_ParseNode *ArgumentRest_p = Nh_Web_allocateNonTerminalParseNode("ArgumentRest", 3);
            Nh_appendToList(&ArgumentRest_p->Children, Type.Node_p);
            Nh_appendToList(&ArgumentRest_p->Children, Ellipsis.Node_p);
            Nh_appendToList(&ArgumentRest_p->Children, ArgumentName.Node_p);
            *Parser_p = LocalParser;
            NH_WEB_END(Nh_Web_initParseResult(ArgumentRest_p))
        }

        Nh_Web_freeParseNode(Type.Node_p);
        Nh_Web_freeParseNode(Ellipsis.Node_p);
    }

NH_WEB_END(Nh_Web_initParseResult(NULL))
}

static Nh_Web_ParseResult Nh_Web_parseArgument(
    Nh_Web_Parser *Parser_p)
{
NH_WEB_BEGIN()

    Nh_Web_Parser LocalParser = *Parser_p;

    Nh_Web_ParseResult ExtendedAttributeList = Nh_Web_parseExtendedAttributeList(&LocalParser);
    Nh_Web_ParseResult ArgumentRest = Nh_Web_parseArgumentRest(&LocalParser);

    if (ArgumentRest.Node_p != NULL) 
    {
        Nh_Web_ParseNode *Argument_p = Nh_Web_allocateNonTerminalParseNode("Argument", 2);
        Nh_appendToList(&Argument_p->Children, ExtendedAttributeList.Node_p);
        Nh_appendToList(&Argument_p->Children, ArgumentRest.Node_p);
        *Parser_p = LocalParser;
        NH_WEB_END(Nh_Web_initParseResult(Argument_p))
    }

NH_WEB_END(Nh_Web_initParseResult(NULL))
}

static Nh_Web_ParseResult Nh_Web_parseArguments(
    Nh_Web_Parser *Parser_p)
{
NH_WEB_BEGIN()

    Nh_Web_ParseNode *Arguments_p = Nh_Web_allocateNonTerminalParseNode("Arguments", 3);

    if (Parser_p->unparsed > 0 && Parser_p->Tokens_p[0].String.bytes_p[0] == ',') 
    {
        Nh_Web_Parser LocalParser = Nh_Web_advanceParser(*Parser_p, 1);
        Nh_Web_ParseResult Argument = Nh_Web_parseArgument(&LocalParser);

        if (Argument.Node_p != NULL) 
        {
            Nh_Web_ParseResult Arguments = Nh_Web_parseArguments(&LocalParser);
            Nh_appendToList(&Arguments_p->Children, Nh_Web_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
            Nh_appendToList(&Arguments_p->Children, Argument.Node_p);
            Nh_appendToList(&Arguments_p->Children, Arguments.Node_p);
            *Parser_p = LocalParser;
        }
    }

NH_WEB_END(Nh_Web_initParseResult(Arguments_p))
}

static Nh_Web_ParseResult Nh_Web_parseArgumentList(
    Nh_Web_Parser *Parser_p)
{
NH_WEB_BEGIN()

    Nh_Web_ParseNode *ArgumentList_p = Nh_Web_allocateNonTerminalParseNode("ArgumentList", 2);

    Nh_Web_ParseResult Argument = Nh_Web_parseArgument(Parser_p);
    if (Argument.Node_p != NULL) 
    {
        Nh_Web_ParseResult Arguments = Nh_Web_parseArguments(Parser_p);
        Nh_appendToList(&ArgumentList_p->Children, Argument.Node_p);
        Nh_appendToList(&ArgumentList_p->Children, Arguments.Node_p);
    } 
   
NH_WEB_END(Nh_Web_initParseResult(ArgumentList_p))
}

static Nh_Web_ParseResult Nh_Web_parseCallbackRest(
    Nh_Web_Parser *Parser_p)
{
NH_WEB_BEGIN()

    if (Parser_p->unparsed > 0 && Parser_p->Tokens_p[0].type == NH_WEB_TOKEN_IDENTIFIER) 
    {
        Nh_Web_Parser LocalParser = Nh_Web_advanceParser(*Parser_p, 1);

        if (LocalParser.unparsed > 0 && LocalParser.Tokens_p[0].String.bytes_p[0] == '=') 
        {
            Nh_Web_Token *Equal_p = &LocalParser.Tokens_p[0];
            LocalParser = Nh_Web_advanceParser(LocalParser, 1);
            Nh_Web_ParseResult Type = Nh_Web_parseType(&LocalParser);

            if (Type.Node_p != NULL && LocalParser.unparsed > 0 && LocalParser.Tokens_p[0].String.bytes_p[0] == '(') 
            {
                Nh_Web_Token *LeftBracket_p = &LocalParser.Tokens_p[0];
                LocalParser = Nh_Web_advanceParser(LocalParser, 1);
                Nh_Web_ParseResult ArgumentList = Nh_Web_parseArgumentList(&LocalParser);

                if (LocalParser.unparsed > 1 
                &&  LocalParser.Tokens_p[0].String.bytes_p[0] == ')'
                &&  LocalParser.Tokens_p[1].String.bytes_p[0] == ';')
                {
                    Nh_Web_ParseNode *CallbackRest_p = Nh_Web_allocateNonTerminalParseNode("CallbackRest", 1);
                    Nh_appendToList(&CallbackRest_p->Children, Nh_Web_allocateTerminalParseNode(&Parser_p->Tokens_p[0])); 
                    Nh_appendToList(&CallbackRest_p->Children, Nh_Web_allocateTerminalParseNode(Equal_p)); 
                    Nh_appendToList(&CallbackRest_p->Children, Type.Node_p); 
                    Nh_appendToList(&CallbackRest_p->Children, Nh_Web_allocateTerminalParseNode(LeftBracket_p)); 
                    Nh_appendToList(&CallbackRest_p->Children, ArgumentList.Node_p); 
                    Nh_appendToList(&CallbackRest_p->Children, Nh_Web_allocateTerminalParseNode(&LocalParser.Tokens_p[0])); 
                    Nh_appendToList(&CallbackRest_p->Children, Nh_Web_allocateTerminalParseNode(&LocalParser.Tokens_p[1])); 
                    *Parser_p = Nh_Web_advanceParser(LocalParser, 2);
                    NH_WEB_END(Nh_Web_initParseResult(CallbackRest_p))
                }

                Nh_Web_freeParseNode(ArgumentList.Node_p);
            }

            Nh_Web_freeParseNode(Type.Node_p);
        }
    }

NH_WEB_END(Nh_Web_initParseResult(NULL))
}

static Nh_Web_ParseResult Nh_Web_parseConstType(
    Nh_Web_Parser *Parser_p)
{
NH_WEB_BEGIN()

    Nh_Web_ParseResult PrimitiveType = Nh_Web_parsePrimitiveType(Parser_p);
 
    if (PrimitiveType.Node_p != NULL) {
        Nh_Web_ParseNode *ConstType_p = Nh_Web_allocateNonTerminalParseNode("ConstType", 1);
        Nh_appendToList(&ConstType_p->Children, PrimitiveType.Node_p);
        NH_WEB_END(Nh_Web_initParseResult(ConstType_p))
    }

    if (Parser_p->unparsed > 0 && Parser_p->Tokens_p[0].type == NH_WEB_TOKEN_IDENTIFIER) {
        Nh_Web_ParseNode *ConstType_p = Nh_Web_allocateNonTerminalParseNode("ConstType", 1);
        Nh_appendToList(&ConstType_p->Children, Nh_Web_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
        NH_WEB_END(Nh_Web_initParseResult(ConstType_p))
    }

NH_WEB_END(Nh_Web_initParseResult(NULL))
}

static Nh_Web_ParseResult Nh_Web_parseConst(
    Nh_Web_Parser *Parser_p)
{
NH_WEB_BEGIN()

    if (Parser_p->unparsed <= 0 || strcmp(Parser_p->Tokens_p[0].String.bytes_p, "const")) {
        NH_WEB_END(Nh_Web_initParseResult(NULL))
    }

    Nh_Web_Parser LocalParser = Nh_Web_advanceParser(*Parser_p, 1);
    Nh_Web_ParseResult ConstType = Nh_Web_parseConstType(&LocalParser);
 
    if (ConstType.Node_p != NULL) 
    {
        if (LocalParser.unparsed > 0 && LocalParser.Tokens_p[0].type == NH_WEB_TOKEN_IDENTIFIER) 
        {
            Nh_Web_Token *IdentifierToken_p = &LocalParser.Tokens_p[0];
            LocalParser = Nh_Web_advanceParser(LocalParser, 1);   

            if (LocalParser.unparsed > 0 && LocalParser.Tokens_p[0].String.bytes_p[0] == '=') 
            {
                Nh_Web_Token *EqualToken_p = &LocalParser.Tokens_p[0];
                LocalParser = Nh_Web_advanceParser(LocalParser, 1);   

                Nh_Web_ParseResult ConstValue = Nh_Web_parseConstValue(&LocalParser);

                if (ConstValue.Node_p != NULL && LocalParser.unparsed > 0 && LocalParser.Tokens_p[0].String.bytes_p[0] == ';') 
                {
                    Nh_Web_ParseNode *Const_p = Nh_Web_allocateNonTerminalParseNode("Const", 6);
                    Nh_appendToList(&Const_p->Children, Nh_Web_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
                    Nh_appendToList(&Const_p->Children, ConstType.Node_p);
                    Nh_appendToList(&Const_p->Children, Nh_Web_allocateTerminalParseNode(IdentifierToken_p));
                    Nh_appendToList(&Const_p->Children, Nh_Web_allocateTerminalParseNode(EqualToken_p));
                    Nh_appendToList(&Const_p->Children, ConstValue.Node_p);
                    Nh_appendToList(&Const_p->Children, Nh_Web_allocateTerminalParseNode(&LocalParser.Tokens_p[0]));
                    *Parser_p = Nh_Web_advanceParser(LocalParser, 1);
                    NH_WEB_END(Nh_Web_initParseResult(Const_p))
                }

                Nh_Web_freeParseNode(ConstValue.Node_p);
            }
        }

        Nh_Web_freeParseNode(ConstType.Node_p);
    }

NH_WEB_END(Nh_Web_initParseResult(NULL))
}

static Nh_Web_ParseResult Nh_Web_parseOperationNameKeyword(
    Nh_Web_Parser *Parser_p)
{
NH_WEB_BEGIN()

    if (Parser_p->unparsed > 0 && !strcmp(Parser_p->Tokens_p[0].String.bytes_p, "includes")) 
    {
        Nh_Web_ParseNode *OperationNameKeyword_p = Nh_Web_allocateNonTerminalParseNode("OperationNameKeyword", 1);
        Nh_appendToList(&OperationNameKeyword_p->Children, Nh_Web_allocateTerminalParseNode(&Parser_p->Tokens_p[0])); 
        *Parser_p = Nh_Web_advanceParser(*Parser_p, 1);
        NH_WEB_END(Nh_Web_initParseResult(OperationNameKeyword_p))
    }

NH_WEB_END(Nh_Web_initParseResult(NULL))
}

static Nh_Web_ParseResult Nh_Web_parseOperationName(
    Nh_Web_Parser *Parser_p)
{
NH_WEB_BEGIN()

    Nh_Web_ParseResult OperationNameKeyword = Nh_Web_parseOperationNameKeyword(Parser_p);
    if (OperationNameKeyword.Node_p != NULL) {
        Nh_Web_ParseNode *OperationName_p = Nh_Web_allocateNonTerminalParseNode("OperationName", 1);
        Nh_appendToList(&OperationName_p->Children, OperationNameKeyword.Node_p);
        NH_WEB_END(Nh_Web_initParseResult(OperationName_p))
    }

    if (Parser_p->unparsed > 0 && Parser_p->Tokens_p[0].type == NH_WEB_TOKEN_IDENTIFIER) {
        Nh_Web_ParseNode *OperationName_p = Nh_Web_allocateNonTerminalParseNode("OperationName", 1);
        Nh_appendToList(&OperationName_p->Children, Nh_Web_allocateTerminalParseNode(&Parser_p->Tokens_p[0])); 
        *Parser_p = Nh_Web_advanceParser(*Parser_p, 1);
        NH_WEB_END(Nh_Web_initParseResult(OperationName_p))
    }

NH_WEB_END(Nh_Web_initParseResult(NULL))
}

static Nh_Web_ParseResult Nh_Web_parseOptionalOperationName(
    Nh_Web_Parser *Parser_p)
{
NH_WEB_BEGIN()

    Nh_Web_ParseNode *OptionalOperationName_p = Nh_Web_allocateNonTerminalParseNode("OptionalOperationName", 1);

    Nh_Web_ParseResult OperationName = Nh_Web_parseOperationName(Parser_p);
    if (OperationName.Node_p != NULL) {
        Nh_appendToList(&OptionalOperationName_p->Children, OperationName.Node_p);
    }

NH_WEB_END(Nh_Web_initParseResult(OptionalOperationName_p))
}

static Nh_Web_ParseResult Nh_Web_parseOperationRest(
    Nh_Web_Parser *Parser_p)
{
NH_WEB_BEGIN()

    Nh_Web_Parser LocalParser = *Parser_p;
    Nh_Web_ParseResult OptionalOperationName = Nh_Web_parseOptionalOperationName(&LocalParser);

    if (LocalParser.unparsed > 0 && LocalParser.Tokens_p[0].String.bytes_p[0] == '(') 
    {
        Nh_Web_Token *LeftBracket_p = &LocalParser.Tokens_p[0];
        LocalParser = Nh_Web_advanceParser(LocalParser, 1);
        Nh_Web_ParseResult ArgumentList = Nh_Web_parseArgumentList(&LocalParser);

        if (LocalParser.unparsed > 1 
        &&  LocalParser.Tokens_p[0].String.bytes_p[0] == ')'
        &&  LocalParser.Tokens_p[1].String.bytes_p[0] == ';')
        {
            Nh_Web_ParseNode *OperationRest_p = Nh_Web_allocateNonTerminalParseNode("OperationRest", 5);
            Nh_appendToList(&OperationRest_p->Children, OptionalOperationName.Node_p); 
            Nh_appendToList(&OperationRest_p->Children, Nh_Web_allocateTerminalParseNode(LeftBracket_p));
            Nh_appendToList(&OperationRest_p->Children, ArgumentList.Node_p);
            Nh_appendToList(&OperationRest_p->Children, Nh_Web_allocateTerminalParseNode(&LocalParser.Tokens_p[0]));
            Nh_appendToList(&OperationRest_p->Children, Nh_Web_allocateTerminalParseNode(&LocalParser.Tokens_p[1]));
            *Parser_p = Nh_Web_advanceParser(LocalParser, 2);
            NH_WEB_END(Nh_Web_initParseResult(OperationRest_p))
        }
    }

NH_WEB_END(Nh_Web_initParseResult(NULL))
}

static Nh_Web_ParseResult Nh_Web_parseRegularOperation(
    Nh_Web_Parser *Parser_p)
{
NH_WEB_BEGIN()

    Nh_Web_Parser LocalParser = *Parser_p;
    Nh_Web_ParseResult Type = Nh_Web_parseType(&LocalParser);

    if (Type.Node_p != NULL) 
    {
        Nh_Web_ParseResult OperationRest = Nh_Web_parseOperationRest(&LocalParser);

        if (OperationRest.Node_p != NULL) 
        {
            Nh_Web_ParseNode *RegularOperation_p = Nh_Web_allocateNonTerminalParseNode("RegularOperation", 2);
            Nh_appendToList(&RegularOperation_p->Children, Type.Node_p);
            Nh_appendToList(&RegularOperation_p->Children, OperationRest.Node_p);
            *Parser_p = LocalParser;
            NH_WEB_END(Nh_Web_initParseResult(RegularOperation_p))
        }

        Nh_Web_freeParseNode(Type.Node_p);
    }

NH_WEB_END(Nh_Web_initParseResult(NULL))
}

static Nh_Web_ParseResult Nh_Web_parseCallbackClassMember(
    Nh_Web_Parser *Parser_p)
{
NH_WEB_BEGIN()

    Nh_Web_ParseResult Const = Nh_Web_parseConst(Parser_p);
    if (Const.Node_p != NULL) 
    {
        Nh_Web_ParseNode *CallbackClassMember_p = Nh_Web_allocateNonTerminalParseNode("CallbackClassMember", 1);
        Nh_appendToList(&CallbackClassMember_p->Children, Const.Node_p);
        NH_WEB_END(Nh_Web_initParseResult(CallbackClassMember_p))
    }

    Nh_Web_ParseResult RegularOperation = Nh_Web_parseRegularOperation(Parser_p);
    if (RegularOperation.Node_p != NULL) 
    {
        Nh_Web_ParseNode *CallbackClassMember_p = Nh_Web_allocateNonTerminalParseNode("CallbackClassMember", 1);
        Nh_appendToList(&CallbackClassMember_p->Children, RegularOperation.Node_p);
        NH_WEB_END(Nh_Web_initParseResult(CallbackClassMember_p))
    }

NH_WEB_END(Nh_Web_initParseResult(NULL))
}

static Nh_Web_ParseResult Nh_Web_parseCallbackClassMembers(
    Nh_Web_Parser *Parser_p)
{
NH_WEB_BEGIN()

    Nh_Web_ParseNode *CallbackClassMembers_p = Nh_Web_allocateNonTerminalParseNode("CallbackClassMembers", 1);

    Nh_Web_ParseResult ExtendedAttributeList = Nh_Web_parseExtendedAttributeList(Parser_p);
    Nh_Web_ParseResult CallbackClassMember = Nh_Web_parseCallbackClassMember(Parser_p);

    if (CallbackClassMember.Node_p != NULL) {
        Nh_Web_ParseResult CallbackClassMembers = Nh_Web_parseCallbackClassMembers(Parser_p);
        Nh_appendToList(&CallbackClassMembers_p->Children, ExtendedAttributeList.Node_p);
        Nh_appendToList(&CallbackClassMembers_p->Children, CallbackClassMember.Node_p);
        Nh_appendToList(&CallbackClassMembers_p->Children, CallbackClassMembers.Node_p);
    }
    else {Nh_Web_freeParseNode(ExtendedAttributeList.Node_p);}

NH_WEB_END(Nh_Web_initParseResult(CallbackClassMembers_p))
}

static Nh_Web_ParseResult Nh_Web_parseCallbackRestOrClass(
    Nh_Web_Parser *Parser_p)
{
NH_WEB_BEGIN()

    Nh_Web_ParseResult CallbackRest = Nh_Web_parseCallbackRest(Parser_p);

    if (CallbackRest.Node_p != NULL)
    {
        Nh_Web_ParseNode *CallbackRestOrClass_p = Nh_Web_allocateNonTerminalParseNode("CallbackRestOrClass", 1);
        Nh_appendToList(&CallbackRestOrClass_p->Children, CallbackRest.Node_p);
        NH_WEB_END(Nh_Web_initParseResult(CallbackRestOrClass_p))
    }
    
    if (Parser_p->unparsed > 0 && !strcmp(Parser_p->Tokens_p[0].String.bytes_p, "class")) 
    {
        Nh_Web_Parser LocalParser = Nh_Web_advanceParser(*Parser_p, 1);

        if (LocalParser.unparsed > 0 && LocalParser.Tokens_p[0].type == NH_WEB_TOKEN_IDENTIFIER) 
        {
            Nh_Web_Token *Identifier_p = &LocalParser.Tokens_p[0];
            LocalParser = Nh_Web_advanceParser(LocalParser, 1);

            if (LocalParser.unparsed > 0 && LocalParser.Tokens_p[0].String.bytes_p[0] == '{') 
            {
                Nh_Web_Token *LeftBracket_p = &LocalParser.Tokens_p[0];
                LocalParser = Nh_Web_advanceParser(LocalParser, 1);

                Nh_Web_ParseResult CallbackClassMembers = Nh_Web_parseCallbackClassMembers(&LocalParser);

                if (LocalParser.unparsed > 1 
                &&  LocalParser.Tokens_p[0].String.bytes_p[0] == '}'
                &&  LocalParser.Tokens_p[1].String.bytes_p[0] == ';') 
                {
                    Nh_Web_ParseNode *CallbackRestOrClass_p = Nh_Web_allocateNonTerminalParseNode("CallbackRestOrClass", 6);
                    Nh_appendToList(&CallbackRestOrClass_p->Children, Nh_Web_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
                    Nh_appendToList(&CallbackRestOrClass_p->Children, Nh_Web_allocateTerminalParseNode(Identifier_p));
                    Nh_appendToList(&CallbackRestOrClass_p->Children, Nh_Web_allocateTerminalParseNode(LeftBracket_p));
                    Nh_appendToList(&CallbackRestOrClass_p->Children, CallbackClassMembers.Node_p);
                    Nh_appendToList(&CallbackRestOrClass_p->Children, Nh_Web_allocateTerminalParseNode(&LocalParser.Tokens_p[0]));
                    Nh_appendToList(&CallbackRestOrClass_p->Children, Nh_Web_allocateTerminalParseNode(&LocalParser.Tokens_p[1]));
                    *Parser_p = Nh_Web_advanceParser(LocalParser, 2);
                    NH_WEB_END(Nh_Web_initParseResult(CallbackRestOrClass_p))
                }
                else {Nh_Web_freeParseNode(CallbackClassMembers.Node_p);} 
            }
        }
    }

NH_WEB_END(Nh_Web_initParseResult(NULL))
}

static Nh_Web_ParseResult Nh_Web_parseInheritance(
    Nh_Web_Parser *Parser_p)
{
NH_WEB_BEGIN()

    Nh_Web_ParseNode *Inheritance_p = Nh_Web_allocateNonTerminalParseNode("Inheritance", 2);

    if (Parser_p->unparsed > 1 && Parser_p->Tokens_p[0].String.bytes_p[0] == ':' && Parser_p->Tokens_p[1].type == NH_WEB_TOKEN_IDENTIFIER)
    {
        Nh_appendToList(&Inheritance_p->Children, Nh_Web_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
        Nh_appendToList(&Inheritance_p->Children, Nh_Web_allocateTerminalParseNode(&Parser_p->Tokens_p[1]));
        *Parser_p = Nh_Web_advanceParser(*Parser_p, 2);
    }

NH_WEB_END(Nh_Web_initParseResult(Inheritance_p))
}

static Nh_Web_ParseResult Nh_Web_parseSpecial(
    Nh_Web_Parser *Parser_p)
{
NH_WEB_BEGIN()

    if (Parser_p->unparsed == 0) {NH_WEB_END(Nh_Web_initParseResult(NULL))}

    if (!strcmp(Parser_p->Tokens_p[0].String.bytes_p, "getter")
    ||  !strcmp(Parser_p->Tokens_p[0].String.bytes_p, "setter")
    ||  !strcmp(Parser_p->Tokens_p[0].String.bytes_p, "deleter"))
    {
        Nh_Web_ParseNode *Special_p = Nh_Web_allocateNonTerminalParseNode("Special", 1);
        Nh_appendToList(&Special_p->Children, Nh_Web_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
        *Parser_p = Nh_Web_advanceParser(*Parser_p, 1);
        NH_WEB_END(Nh_Web_initParseResult(Special_p))
    }

NH_WEB_END(Nh_Web_initParseResult(NULL))
}

static Nh_Web_ParseResult Nh_Web_parseSpecialOperation(
    Nh_Web_Parser *Parser_p)
{
NH_WEB_BEGIN()

    Nh_Web_ParseResult Special = Nh_Web_parseSpecial(Parser_p);
    if (Special.Node_p != NULL) 
    {
        Nh_Web_ParseResult RegularOperation = Nh_Web_parseRegularOperation(Parser_p);
        if (RegularOperation.Node_p != NULL)
        {
            Nh_Web_ParseNode *SpecialOperation_p = Nh_Web_allocateNonTerminalParseNode("SpecialOperation", 2);
            Nh_appendToList(&SpecialOperation_p->Children, Special.Node_p);
            Nh_appendToList(&SpecialOperation_p->Children, RegularOperation.Node_p);
            NH_WEB_END(Nh_Web_initParseResult(SpecialOperation_p))
        }
        Nh_Web_freeParseNode(Special.Node_p);
    }

NH_WEB_END(Nh_Web_initParseResult(NULL))
}

static Nh_Web_ParseResult Nh_Web_parseOperation(
    Nh_Web_Parser *Parser_p)
{
NH_WEB_BEGIN()

    Nh_Web_ParseResult RegularOperation = Nh_Web_parseRegularOperation(Parser_p);
    if (RegularOperation.Node_p != NULL) 
    {
        Nh_Web_ParseNode *Operation_p = Nh_Web_allocateNonTerminalParseNode("Operation", 1);
        Nh_appendToList(&Operation_p->Children, RegularOperation.Node_p);
        NH_WEB_END(Nh_Web_initParseResult(Operation_p))
    }

    Nh_Web_ParseResult SpecialOperation = Nh_Web_parseSpecialOperation(Parser_p);
    if (SpecialOperation.Node_p != NULL) 
    {
        Nh_Web_ParseNode *Operation_p = Nh_Web_allocateNonTerminalParseNode("Operation", 1);
        Nh_appendToList(&Operation_p->Children, SpecialOperation.Node_p);
        NH_WEB_END(Nh_Web_initParseResult(Operation_p))
    }

NH_WEB_END(Nh_Web_initParseResult(NULL))
}

static Nh_Web_ParseResult Nh_Web_parseOptionalReadOnly(
    Nh_Web_Parser *Parser_p)
{
NH_WEB_BEGIN()

    Nh_Web_ParseNode *OptionalReadOnly_p = Nh_Web_allocateNonTerminalParseNode("OptionalReadOnly", 1);

    if (!strcmp(Parser_p->Tokens_p[0].String.bytes_p, "readonly"))
    {
        Nh_appendToList(&OptionalReadOnly_p->Children, Nh_Web_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
        *Parser_p = Nh_Web_advanceParser(*Parser_p, 1);
    }

NH_WEB_END(Nh_Web_initParseResult(OptionalReadOnly_p))
}

static Nh_Web_ParseResult Nh_Web_parseAttributeNameKeyword(
    Nh_Web_Parser *Parser_p)
{
NH_WEB_BEGIN()

    if (Parser_p->unparsed <= 0) {NH_WEB_END(Nh_Web_initParseResult(NULL))}

    if (!strcmp(Parser_p->Tokens_p[0].String.bytes_p, "async")
    ||  !strcmp(Parser_p->Tokens_p[0].String.bytes_p, "required"))
    {
        Nh_Web_ParseNode *AttributeNameKeyword_p = Nh_Web_allocateNonTerminalParseNode("AttributeNameKeyword", 1);
        Nh_appendToList(&AttributeNameKeyword_p->Children, Nh_Web_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
        *Parser_p = Nh_Web_advanceParser(*Parser_p, 1);
        NH_WEB_END(Nh_Web_initParseResult(AttributeNameKeyword_p))
    }
    
NH_WEB_END(Nh_Web_initParseResult(NULL))
}

static Nh_Web_ParseResult Nh_Web_parseAttributeName(
    Nh_Web_Parser *Parser_p)
{
NH_WEB_BEGIN()

    Nh_Web_ParseResult AttributeNameKeyword = Nh_Web_parseAttributeNameKeyword(Parser_p);
    if (AttributeNameKeyword.Node_p != NULL) 
    {
        Nh_Web_ParseNode *AttributeName_p = Nh_Web_allocateNonTerminalParseNode("AttributeName", 1);
        Nh_appendToList(&AttributeName_p->Children, AttributeNameKeyword.Node_p);
        NH_WEB_END(Nh_Web_initParseResult(AttributeName_p))
    }

    if (Parser_p->unparsed > 0 && Parser_p->Tokens_p[0].type == NH_WEB_TOKEN_IDENTIFIER)
    {
        Nh_Web_ParseNode *AttributeName_p = Nh_Web_allocateNonTerminalParseNode("AttributeName", 1);
        Nh_appendToList(&AttributeName_p->Children, Nh_Web_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
        *Parser_p = Nh_Web_advanceParser(*Parser_p, 1);
        NH_WEB_END(Nh_Web_initParseResult(AttributeName_p))
    }

NH_WEB_END(Nh_Web_initParseResult(NULL))
}

static Nh_Web_ParseResult Nh_Web_parseAttributeRest(
    Nh_Web_Parser *Parser_p)
{
NH_WEB_BEGIN()

    if (!strcmp(Parser_p->Tokens_p[0].String.bytes_p, "attribute"))
    {
        Nh_Web_Parser LocalParser = Nh_Web_advanceParser(*Parser_p, 1);
        Nh_Web_ParseResult TypeWithExtendedAttributes = Nh_Web_parseTypeWithExtendedAttributes(&LocalParser);

        if (TypeWithExtendedAttributes.Node_p != NULL)
        {
            Nh_Web_ParseResult AttributeName = Nh_Web_parseAttributeName(&LocalParser);
            if (AttributeName.Node_p != NULL && LocalParser.unparsed > 0 && LocalParser.Tokens_p[0].String.bytes_p[0] == ';')
            {
                Nh_Web_ParseNode *AttributeRest_p = Nh_Web_allocateNonTerminalParseNode("AttributeRest", 4);
                Nh_appendToList(&AttributeRest_p->Children, Nh_Web_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
                Nh_appendToList(&AttributeRest_p->Children, TypeWithExtendedAttributes.Node_p);
                Nh_appendToList(&AttributeRest_p->Children, AttributeName.Node_p);
                Nh_appendToList(&AttributeRest_p->Children, Nh_Web_allocateTerminalParseNode(&LocalParser.Tokens_p[0]));
                *Parser_p = Nh_Web_advanceParser(LocalParser, 1);
                NH_WEB_END(Nh_Web_initParseResult(AttributeRest_p))
            }
        }

        Nh_Web_freeParseNode(TypeWithExtendedAttributes.Node_p);
    }

NH_WEB_END(Nh_Web_initParseResult(NULL))
}

static Nh_Web_ParseResult Nh_Web_parseStringifierRest(
    Nh_Web_Parser *Parser_p)
{
NH_WEB_BEGIN()

    if (Parser_p->Tokens_p[0].String.bytes_p[0] == ';')
    {
        Nh_Web_ParseNode *StringifierRest_p = Nh_Web_allocateNonTerminalParseNode("StringifierRest", 1);
        Nh_appendToList(&StringifierRest_p->Children, Nh_Web_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
        *Parser_p = Nh_Web_advanceParser(*Parser_p, 1);
        NH_WEB_END(Nh_Web_initParseResult(StringifierRest_p))
    }

    Nh_Web_ParseResult RegularOperation = Nh_Web_parseRegularOperation(Parser_p);

    if (RegularOperation.Node_p != NULL) 
    {
        Nh_Web_ParseNode *StringifierRest_p = Nh_Web_allocateNonTerminalParseNode("StringifierRest", 1);
        Nh_appendToList(&StringifierRest_p->Children, RegularOperation.Node_p);
        NH_WEB_END(Nh_Web_initParseResult(StringifierRest_p))
    }

    Nh_Web_Parser LocalParser = *Parser_p;
    Nh_Web_ParseResult OptionalReadOnly = Nh_Web_parseOptionalReadOnly(&LocalParser);
    Nh_Web_ParseResult AttributeRest = Nh_Web_parseAttributeRest(&LocalParser);

    if (AttributeRest.Node_p != NULL) 
    {
        Nh_Web_ParseNode *StringifierRest_p = Nh_Web_allocateNonTerminalParseNode("StringifierRest", 2);
        Nh_appendToList(&StringifierRest_p->Children, OptionalReadOnly.Node_p);
        Nh_appendToList(&StringifierRest_p->Children, AttributeRest.Node_p);
        *Parser_p = LocalParser;
        NH_WEB_END(Nh_Web_initParseResult(StringifierRest_p))
    }
    else {Nh_Web_freeParseNode(OptionalReadOnly.Node_p);}

NH_WEB_END(Nh_Web_initParseResult(NULL))
}

static Nh_Web_ParseResult Nh_Web_parseStringifier(
    Nh_Web_Parser *Parser_p)
{
NH_WEB_BEGIN()

    if (Parser_p->unparsed == 0) {NH_WEB_END(Nh_Web_initParseResult(NULL))}

    if (!strcmp(Parser_p->Tokens_p[0].String.bytes_p, "stringifier"))
    {
        Nh_Web_Parser LocalParser = Nh_Web_advanceParser(*Parser_p, 1);
        Nh_Web_ParseResult StringifierRest = Nh_Web_parseStringifierRest(&LocalParser);

        if (StringifierRest.Node_p != NULL)
        {
            Nh_Web_ParseNode *Stringifier_p = Nh_Web_allocateNonTerminalParseNode("Stringifier", 2);
            Nh_appendToList(&Stringifier_p->Children, Nh_Web_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
            Nh_appendToList(&Stringifier_p->Children, StringifierRest.Node_p);
            *Parser_p = LocalParser;
            NH_WEB_END(Nh_Web_initParseResult(Stringifier_p))
        }
    }

NH_WEB_END(Nh_Web_initParseResult(NULL))
}

static Nh_Web_ParseResult Nh_Web_parseStaticMemberRest(
    Nh_Web_Parser *Parser_p)
{
NH_WEB_BEGIN()

    Nh_Web_ParseResult RegularOperation = Nh_Web_parseRegularOperation(Parser_p);

    if (RegularOperation.Node_p != NULL) 
    {
        Nh_Web_ParseNode *StaticMemberRest_p = Nh_Web_allocateNonTerminalParseNode("StaticMemberRest", 1);
        Nh_appendToList(&StaticMemberRest_p->Children, RegularOperation.Node_p);
        NH_WEB_END(Nh_Web_initParseResult(StaticMemberRest_p))
    }

    Nh_Web_Parser LocalParser = *Parser_p;
    Nh_Web_ParseResult OptionalReadOnly = Nh_Web_parseOptionalReadOnly(&LocalParser);
    Nh_Web_ParseResult AttributeRest = Nh_Web_parseAttributeRest(&LocalParser);

    if (AttributeRest.Node_p != NULL) 
    {
        Nh_Web_ParseNode *StaticMemberRest_p = Nh_Web_allocateNonTerminalParseNode("StaticMemberRest", 2);
        Nh_appendToList(&StaticMemberRest_p->Children, OptionalReadOnly.Node_p);
        Nh_appendToList(&StaticMemberRest_p->Children, AttributeRest.Node_p);
        *Parser_p = LocalParser;
        NH_WEB_END(Nh_Web_initParseResult(StaticMemberRest_p))
    }
    else {Nh_Web_freeParseNode(OptionalReadOnly.Node_p);}

NH_WEB_END(Nh_Web_initParseResult(NULL))
}

static Nh_Web_ParseResult Nh_Web_parseStaticMember(
    Nh_Web_Parser *Parser_p)
{
NH_WEB_BEGIN()

    if (Parser_p->unparsed == 0) {NH_WEB_END(Nh_Web_initParseResult(NULL))}

    if (!strcmp(Parser_p->Tokens_p[0].String.bytes_p, "static"))
    {
        Nh_Web_Parser LocalParser = Nh_Web_advanceParser(*Parser_p, 1);
        Nh_Web_ParseResult StaticMemberRest = Nh_Web_parseStaticMemberRest(&LocalParser);

        if (StaticMemberRest.Node_p != NULL)
        {
            Nh_Web_ParseNode *StaticMember_p = Nh_Web_allocateNonTerminalParseNode("StaticMember", 2);
            Nh_appendToList(&StaticMember_p->Children, Nh_Web_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
            Nh_appendToList(&StaticMember_p->Children, StaticMemberRest.Node_p);
            *Parser_p = LocalParser;
            NH_WEB_END(Nh_Web_initParseResult(StaticMember_p))
        }
    }

NH_WEB_END(Nh_Web_initParseResult(NULL))
}

static Nh_Web_ParseResult Nh_Web_parseOptionalType(
    Nh_Web_Parser *Parser_p)
{
NH_WEB_BEGIN()

    Nh_Web_ParseNode *OptionalType_p = Nh_Web_allocateNonTerminalParseNode("OptionalType", 2);

    if (Parser_p->Tokens_p[0].String.bytes_p[0] == ',')
    {
        Nh_Web_Parser LocalParser = Nh_Web_advanceParser(*Parser_p, 1);
        Nh_Web_ParseResult TypeWithExtendedAttributes = Nh_Web_parseTypeWithExtendedAttributes(&LocalParser);

        if (TypeWithExtendedAttributes.Node_p != NULL)
        {
            Nh_appendToList(&OptionalType_p->Children, Nh_Web_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
            Nh_appendToList(&OptionalType_p->Children, TypeWithExtendedAttributes.Node_p);
            *Parser_p = LocalParser;
        }
    }

NH_WEB_END(Nh_Web_initParseResult(OptionalType_p))
}

static Nh_Web_ParseResult Nh_Web_parseIterable(
    Nh_Web_Parser *Parser_p)
{
NH_WEB_BEGIN()

    if (Parser_p->unparsed <= 4) {NH_WEB_END(Nh_Web_initParseResult(NULL))}

    if (!strcmp(Parser_p->Tokens_p[0].String.bytes_p, "iterable") && Parser_p->Tokens_p[1].String.bytes_p[0] == '<')
    {
        Nh_Web_Parser LocalParser = Nh_Web_advanceParser(*Parser_p, 2);
        Nh_Web_ParseResult TypeWithExtendedAttributes = Nh_Web_parseTypeWithExtendedAttributes(&LocalParser);

        if (TypeWithExtendedAttributes.Node_p != NULL)
        {
            Nh_Web_ParseResult OptionalType = Nh_Web_parseOptionalType(&LocalParser);

            if (LocalParser.unparsed >= 2 && LocalParser.Tokens_p[0].String.bytes_p[0] == '>' && LocalParser.Tokens_p[1].String.bytes_p[0] == ';')
            {
                Nh_Web_ParseNode *Iterable_p = Nh_Web_allocateNonTerminalParseNode("Iterable", 6);
                Nh_appendToList(&Iterable_p->Children, Nh_Web_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
                Nh_appendToList(&Iterable_p->Children, Nh_Web_allocateTerminalParseNode(&Parser_p->Tokens_p[1]));
                Nh_appendToList(&Iterable_p->Children, TypeWithExtendedAttributes.Node_p);
                Nh_appendToList(&Iterable_p->Children, OptionalType.Node_p);
                Nh_appendToList(&Iterable_p->Children, Nh_Web_allocateTerminalParseNode(&LocalParser.Tokens_p[0]));
                Nh_appendToList(&Iterable_p->Children, Nh_Web_allocateTerminalParseNode(&LocalParser.Tokens_p[1]));
                *Parser_p = Nh_Web_advanceParser(LocalParser, 2);
                NH_WEB_END(Nh_Web_initParseResult(Iterable_p))
            }
        }
    }

NH_WEB_END(Nh_Web_initParseResult(NULL))
}

static Nh_Web_ParseResult Nh_Web_parseOptionalArgumentList(
    Nh_Web_Parser *Parser_p)
{
NH_WEB_BEGIN()

    Nh_Web_ParseNode *OptionalArgumentList_p = Nh_Web_allocateNonTerminalParseNode("OptionalArgumentList", 3);

    if (Parser_p->unparsed > 0 && Parser_p->Tokens_p[0].String.bytes_p[0] == '(')
    {
        Nh_Web_Parser LocalParser = Nh_Web_advanceParser(*Parser_p, 1);
        Nh_Web_ParseResult ArgumentList = Nh_Web_parseArgumentList(&LocalParser);

        if (LocalParser.unparsed > 0 && LocalParser.Tokens_p[0].String.bytes_p[0] == ')')
        {
            Nh_appendToList(&OptionalArgumentList_p->Children, Nh_Web_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
            Nh_appendToList(&OptionalArgumentList_p->Children, ArgumentList.Node_p);
            Nh_appendToList(&OptionalArgumentList_p->Children, Nh_Web_allocateTerminalParseNode(&LocalParser.Tokens_p[0]));
            *Parser_p = Nh_Web_advanceParser(LocalParser, 1);
        }
    }

NH_WEB_END(Nh_Web_initParseResult(OptionalArgumentList_p))
}

static Nh_Web_ParseResult Nh_Web_parseAsyncIterable(
    Nh_Web_Parser *Parser_p)
{
NH_WEB_BEGIN()

    if (Parser_p->unparsed <= 5) {NH_WEB_END(Nh_Web_initParseResult(NULL))}

    if (!strcmp(Parser_p->Tokens_p[0].String.bytes_p, "async") && !strcmp(Parser_p->Tokens_p[1].String.bytes_p, "iterable") && Parser_p->Tokens_p[2].String.bytes_p[0] == '<')
    {
        Nh_Web_Parser LocalParser = Nh_Web_advanceParser(*Parser_p, 3);
        Nh_Web_ParseResult TypeWithExtendedAttributes = Nh_Web_parseTypeWithExtendedAttributes(&LocalParser);

        if (TypeWithExtendedAttributes.Node_p != NULL)
        {
            Nh_Web_ParseResult OptionalType = Nh_Web_parseOptionalType(&LocalParser);

            if (LocalParser.unparsed > 2 && LocalParser.Tokens_p[0].String.bytes_p[0] == '>')
            {
                Nh_Web_Token *Right_p = &LocalParser.Tokens_p[0];
                Nh_Web_ParseResult OptionalArgumentList = Nh_Web_parseOptionalArgumentList(&LocalParser);

                if (LocalParser.unparsed > 0 && LocalParser.Tokens_p[0].String.bytes_p[0] == ';')
                {
                    Nh_Web_ParseNode *AsyncIterable_p = Nh_Web_allocateNonTerminalParseNode("AsyncIterable", 8);
                    Nh_appendToList(&AsyncIterable_p->Children, Nh_Web_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
                    Nh_appendToList(&AsyncIterable_p->Children, Nh_Web_allocateTerminalParseNode(&Parser_p->Tokens_p[1]));
                    Nh_appendToList(&AsyncIterable_p->Children, Nh_Web_allocateTerminalParseNode(&Parser_p->Tokens_p[2]));
                    Nh_appendToList(&AsyncIterable_p->Children, TypeWithExtendedAttributes.Node_p);
                    Nh_appendToList(&AsyncIterable_p->Children, OptionalType.Node_p);
                    Nh_appendToList(&AsyncIterable_p->Children, Nh_Web_allocateTerminalParseNode(Right_p));
                    Nh_appendToList(&AsyncIterable_p->Children, OptionalArgumentList.Node_p);
                    Nh_appendToList(&AsyncIterable_p->Children, Nh_Web_allocateTerminalParseNode(&LocalParser.Tokens_p[0]));
                    *Parser_p = Nh_Web_advanceParser(LocalParser, 1);
                    NH_WEB_END(Nh_Web_initParseResult(AsyncIterable_p))
                }
                Nh_Web_freeParseNode(OptionalArgumentList.Node_p);
            }
            Nh_Web_freeParseNode(OptionalType.Node_p);
        }
        Nh_Web_freeParseNode(TypeWithExtendedAttributes.Node_p);
    }

NH_WEB_END(Nh_Web_initParseResult(NULL))
}

static Nh_Web_ParseResult Nh_Web_parseMaplikeRest(
    Nh_Web_Parser *Parser_p)
{
NH_WEB_BEGIN()

    if (Parser_p->unparsed <= 4) {NH_WEB_END(Nh_Web_initParseResult(NULL))}

    if (!strcmp(Parser_p->Tokens_p[0].String.bytes_p, "maplike") && Parser_p->Tokens_p[1].String.bytes_p[0] == '<')
    {
        Nh_Web_Parser LocalParser = Nh_Web_advanceParser(*Parser_p, 2);
        Nh_Web_ParseResult TypeWithExtendedAttributes1 = Nh_Web_parseTypeWithExtendedAttributes(&LocalParser);

        if (TypeWithExtendedAttributes1.Node_p != NULL && LocalParser.unparsed > 2 && LocalParser.Tokens_p[0].String.bytes_p[0] == ',')
        {
            Nh_Web_Token *Comma_p = &LocalParser.Tokens_p[0];
            LocalParser = Nh_Web_advanceParser(LocalParser, 1);

            Nh_Web_ParseResult TypeWithExtendedAttributes2 = Nh_Web_parseTypeWithExtendedAttributes(&LocalParser);
    
            if (TypeWithExtendedAttributes2.Node_p != NULL && LocalParser.unparsed > 1 && LocalParser.Tokens_p[0].String.bytes_p[0] == '>' && LocalParser.Tokens_p[1].String.bytes_p[0] == ';')
            {
                Nh_Web_ParseNode *MaplikeRest_p = Nh_Web_allocateNonTerminalParseNode("MaplikeRest", 7);
                Nh_appendToList(&MaplikeRest_p->Children, Nh_Web_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
                Nh_appendToList(&MaplikeRest_p->Children, Nh_Web_allocateTerminalParseNode(&Parser_p->Tokens_p[1]));
                Nh_appendToList(&MaplikeRest_p->Children, TypeWithExtendedAttributes1.Node_p);
                Nh_appendToList(&MaplikeRest_p->Children, Nh_Web_allocateTerminalParseNode(Comma_p));
                Nh_appendToList(&MaplikeRest_p->Children, TypeWithExtendedAttributes2.Node_p);
                Nh_appendToList(&MaplikeRest_p->Children, Nh_Web_allocateTerminalParseNode(&LocalParser.Tokens_p[0]));
                Nh_appendToList(&MaplikeRest_p->Children, Nh_Web_allocateTerminalParseNode(&LocalParser.Tokens_p[1]));
                *Parser_p = Nh_Web_advanceParser(LocalParser, 2);
                NH_WEB_END(Nh_Web_initParseResult(MaplikeRest_p))
            }
            Nh_Web_freeParseNode(TypeWithExtendedAttributes2.Node_p);
        }
        Nh_Web_freeParseNode(TypeWithExtendedAttributes1.Node_p);
    }

NH_WEB_END(Nh_Web_initParseResult(NULL))
}

static Nh_Web_ParseResult Nh_Web_parseSetlikeRest(
    Nh_Web_Parser *Parser_p)
{
NH_WEB_BEGIN()

    if (Parser_p->unparsed <= 4) {NH_WEB_END(Nh_Web_initParseResult(NULL))}

    if (!strcmp(Parser_p->Tokens_p[0].String.bytes_p, "setlike") && Parser_p->Tokens_p[1].String.bytes_p[0] == '<')
    {
        Nh_Web_Parser LocalParser = Nh_Web_advanceParser(*Parser_p, 2);
        Nh_Web_ParseResult TypeWithExtendedAttributes = Nh_Web_parseTypeWithExtendedAttributes(&LocalParser);

        if (TypeWithExtendedAttributes.Node_p != NULL && LocalParser.unparsed > 2 && LocalParser.Tokens_p[0].String.bytes_p[0] == '>' && LocalParser.Tokens_p[0].String.bytes_p[0] == ';')
        {
            Nh_Web_ParseNode *SetlikeRest_p = Nh_Web_allocateNonTerminalParseNode("SetlikeRest", 5);
            Nh_appendToList(&SetlikeRest_p->Children, Nh_Web_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
            Nh_appendToList(&SetlikeRest_p->Children, Nh_Web_allocateTerminalParseNode(&Parser_p->Tokens_p[1]));
            Nh_appendToList(&SetlikeRest_p->Children, TypeWithExtendedAttributes.Node_p);
            Nh_appendToList(&SetlikeRest_p->Children, Nh_Web_allocateTerminalParseNode(&LocalParser.Tokens_p[0]));
            Nh_appendToList(&SetlikeRest_p->Children, Nh_Web_allocateTerminalParseNode(&LocalParser.Tokens_p[1]));
            *Parser_p = Nh_Web_advanceParser(LocalParser, 2);
            NH_WEB_END(Nh_Web_initParseResult(SetlikeRest_p))
        }

        Nh_Web_freeParseNode(TypeWithExtendedAttributes.Node_p);
    }

NH_WEB_END(Nh_Web_initParseResult(NULL))
}

static Nh_Web_ParseResult Nh_Web_parseReadOnlyMemberRest(
    Nh_Web_Parser *Parser_p)
{
NH_WEB_BEGIN()

    Nh_Web_ParseResult AttributeRest = Nh_Web_parseAttributeRest(Parser_p);
    if (AttributeRest.Node_p != NULL)
    {
        Nh_Web_ParseNode *ReadOnlyMemberRest_p = Nh_Web_allocateNonTerminalParseNode("ReadOnlyMemberRest", 1);
        Nh_appendToList(&ReadOnlyMemberRest_p->Children, AttributeRest.Node_p);
        NH_WEB_END(Nh_Web_initParseResult(ReadOnlyMemberRest_p))
    }

    Nh_Web_ParseResult MaplikeRest = Nh_Web_parseMaplikeRest(Parser_p);
    if (MaplikeRest.Node_p != NULL)
    {
        Nh_Web_ParseNode *ReadOnlyMemberRest_p = Nh_Web_allocateNonTerminalParseNode("ReadOnlyMemberRest", 1);
        Nh_appendToList(&ReadOnlyMemberRest_p->Children, MaplikeRest.Node_p);
        NH_WEB_END(Nh_Web_initParseResult(ReadOnlyMemberRest_p))
    }

    Nh_Web_ParseResult SetlikeRest = Nh_Web_parseSetlikeRest(Parser_p);
    if (SetlikeRest.Node_p != NULL)
    {
        Nh_Web_ParseNode *ReadOnlyMemberRest_p = Nh_Web_allocateNonTerminalParseNode("ReadOnlyMemberRest", 1);
        Nh_appendToList(&ReadOnlyMemberRest_p->Children, SetlikeRest.Node_p);
        NH_WEB_END(Nh_Web_initParseResult(ReadOnlyMemberRest_p))
    }

NH_WEB_END(Nh_Web_initParseResult(NULL))
}

static Nh_Web_ParseResult Nh_Web_parseReadOnlyMember(
    Nh_Web_Parser *Parser_p)
{
NH_WEB_BEGIN()

    if (Parser_p->unparsed <= 2) {NH_WEB_END(Nh_Web_initParseResult(NULL))}

    if (!strcmp(Parser_p->Tokens_p[0].String.bytes_p, "readonly"))
    {
        Nh_Web_Parser LocalParser = Nh_Web_advanceParser(*Parser_p, 1);
        Nh_Web_ParseResult ReadOnlyMemberRest = Nh_Web_parseReadOnlyMemberRest(&LocalParser);

        if (ReadOnlyMemberRest.Node_p != NULL)
        {
            Nh_Web_ParseNode *ReadOnlyMember_p = Nh_Web_allocateNonTerminalParseNode("ReadOnlyMember", 2);
            Nh_appendToList(&ReadOnlyMember_p->Children, Nh_Web_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
            Nh_appendToList(&ReadOnlyMember_p->Children, ReadOnlyMemberRest.Node_p);
            *Parser_p = LocalParser;
            NH_WEB_END(Nh_Web_initParseResult(ReadOnlyMember_p))
        }
    }

NH_WEB_END(Nh_Web_initParseResult(NULL))
}

static Nh_Web_ParseResult Nh_Web_parseInheritAttribute(
    Nh_Web_Parser *Parser_p)
{
NH_WEB_BEGIN()

    if (Parser_p->unparsed <= 2) {NH_WEB_END(Nh_Web_initParseResult(NULL))}

    if (!strcmp(Parser_p->Tokens_p[0].String.bytes_p, "inherit"))
    {
        Nh_Web_Parser LocalParser = Nh_Web_advanceParser(*Parser_p, 1);
        Nh_Web_ParseResult AttributeRest = Nh_Web_parseAttributeRest(&LocalParser);

        if (AttributeRest.Node_p != NULL)
        {
            Nh_Web_ParseNode *InheritAttribute_p = Nh_Web_allocateNonTerminalParseNode("InheritAttribute", 2);
            Nh_appendToList(&InheritAttribute_p->Children, Nh_Web_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
            Nh_appendToList(&InheritAttribute_p->Children, AttributeRest.Node_p);
            *Parser_p = LocalParser;
            NH_WEB_END(Nh_Web_initParseResult(InheritAttribute_p))
        }
    }

NH_WEB_END(Nh_Web_initParseResult(NULL))
}

static Nh_Web_ParseResult Nh_Web_parsePrivateAttribute(
    Nh_Web_Parser *Parser_p)
{
NH_WEB_BEGIN()

    if (Parser_p->unparsed <= 2) {NH_WEB_END(Nh_Web_initParseResult(NULL))}

    if (!strcmp(Parser_p->Tokens_p[0].String.bytes_p, "private"))
    {
        Nh_Web_Parser LocalParser = Nh_Web_advanceParser(*Parser_p, 1);
        Nh_Web_ParseResult AttributeRest = Nh_Web_parseAttributeRest(&LocalParser);

        if (AttributeRest.Node_p != NULL)
        {
            Nh_Web_ParseNode *PrivateAttribute_p = Nh_Web_allocateNonTerminalParseNode("PrivateAttribute", 2);
            Nh_appendToList(&PrivateAttribute_p->Children, Nh_Web_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
            Nh_appendToList(&PrivateAttribute_p->Children, AttributeRest.Node_p);
            *Parser_p = LocalParser;
            NH_WEB_END(Nh_Web_initParseResult(PrivateAttribute_p))
        }
    }

NH_WEB_END(Nh_Web_initParseResult(NULL))
}

static Nh_Web_ParseResult Nh_Web_parseReadWriteAttribute(
    Nh_Web_Parser *Parser_p)
{
NH_WEB_BEGIN()

    Nh_Web_ParseResult AttributeRest = Nh_Web_parseAttributeRest(Parser_p);

    if (AttributeRest.Node_p != NULL)
    {
        Nh_Web_ParseNode *ReadWriteAttribute_p = Nh_Web_allocateNonTerminalParseNode("ReadWriteAttribute", 1);
        Nh_appendToList(&ReadWriteAttribute_p->Children, AttributeRest.Node_p);
        NH_WEB_END(Nh_Web_initParseResult(ReadWriteAttribute_p))
    }

NH_WEB_END(Nh_Web_initParseResult(NULL))
}

static Nh_Web_ParseResult Nh_Web_parseReadWriteSetlike(
    Nh_Web_Parser *Parser_p)
{
NH_WEB_BEGIN()

    Nh_Web_ParseResult SetlikeRest = Nh_Web_parseSetlikeRest(Parser_p);

    if (SetlikeRest.Node_p != NULL)
    {
        Nh_Web_ParseNode *ReadWriteSetlike_p = Nh_Web_allocateNonTerminalParseNode("ReadWriteSetlike", 1);
        Nh_appendToList(&ReadWriteSetlike_p->Children, SetlikeRest.Node_p);
        NH_WEB_END(Nh_Web_initParseResult(ReadWriteSetlike_p))
    }

NH_WEB_END(Nh_Web_initParseResult(NULL))
}

static Nh_Web_ParseResult Nh_Web_parseReadWriteMaplike(
    Nh_Web_Parser *Parser_p)
{
NH_WEB_BEGIN()

    Nh_Web_ParseResult MaplikeRest = Nh_Web_parseMaplikeRest(Parser_p);

    if (MaplikeRest.Node_p != NULL)
    {
        Nh_Web_ParseNode *ReadWriteMaplike_p = Nh_Web_allocateNonTerminalParseNode("ReadWriteMaplike", 1);
        Nh_appendToList(&ReadWriteMaplike_p->Children, MaplikeRest.Node_p);
        NH_WEB_END(Nh_Web_initParseResult(ReadWriteMaplike_p))
    }

NH_WEB_END(Nh_Web_initParseResult(NULL))
}

static Nh_Web_ParseResult Nh_Web_parsePartialClassMember(
    Nh_Web_Parser *Parser_p)
{
NH_WEB_BEGIN()

    Nh_Web_ParseResult Const = Nh_Web_parseConst(Parser_p);
    if (Const.Node_p != NULL) 
    {
        Nh_Web_ParseNode *PartialClassMember_p = Nh_Web_allocateNonTerminalParseNode("PartialClassMember", 1);
        Nh_appendToList(&PartialClassMember_p->Children, Const.Node_p);
        NH_WEB_END(Nh_Web_initParseResult(PartialClassMember_p))
    }

    Nh_Web_ParseResult Operation = Nh_Web_parseOperation(Parser_p);
    if (Operation.Node_p != NULL) 
    {
        Nh_Web_ParseNode *PartialClassMember_p = Nh_Web_allocateNonTerminalParseNode("PartialClassMember", 1);
        Nh_appendToList(&PartialClassMember_p->Children, Operation.Node_p);
        NH_WEB_END(Nh_Web_initParseResult(PartialClassMember_p))
    }

    Nh_Web_ParseResult Stringifier = Nh_Web_parseStringifier(Parser_p);
    if (Stringifier.Node_p != NULL) 
    {
        Nh_Web_ParseNode *PartialClassMember_p = Nh_Web_allocateNonTerminalParseNode("PartialClassMember", 1);
        Nh_appendToList(&PartialClassMember_p->Children, Stringifier.Node_p);
        NH_WEB_END(Nh_Web_initParseResult(PartialClassMember_p))
    }

    Nh_Web_ParseResult StaticMember = Nh_Web_parseStaticMember(Parser_p);
    if (Stringifier.Node_p != NULL) 
    {
        Nh_Web_ParseNode *PartialClassMember_p = Nh_Web_allocateNonTerminalParseNode("PartialClassMember", 1);
        Nh_appendToList(&PartialClassMember_p->Children, StaticMember.Node_p);
        NH_WEB_END(Nh_Web_initParseResult(PartialClassMember_p))
    }

    Nh_Web_ParseResult Iterable = Nh_Web_parseIterable(Parser_p);
    if (Iterable.Node_p != NULL) 
    {
        Nh_Web_ParseNode *PartialClassMember_p = Nh_Web_allocateNonTerminalParseNode("PartialClassMember", 1);
        Nh_appendToList(&PartialClassMember_p->Children, Iterable.Node_p);
        NH_WEB_END(Nh_Web_initParseResult(PartialClassMember_p))
    }

    Nh_Web_ParseResult AsyncIterable = Nh_Web_parseAsyncIterable(Parser_p);
    if (AsyncIterable.Node_p != NULL) 
    {
        Nh_Web_ParseNode *PartialClassMember_p = Nh_Web_allocateNonTerminalParseNode("PartialClassMember", 1);
        Nh_appendToList(&PartialClassMember_p->Children, AsyncIterable.Node_p);
        NH_WEB_END(Nh_Web_initParseResult(PartialClassMember_p))
    }

    Nh_Web_ParseResult ReadOnlyMember = Nh_Web_parseReadOnlyMember(Parser_p);
    if (ReadOnlyMember.Node_p != NULL) 
    {
        Nh_Web_ParseNode *PartialClassMember_p = Nh_Web_allocateNonTerminalParseNode("PartialClassMember", 1);
        Nh_appendToList(&PartialClassMember_p->Children, ReadOnlyMember.Node_p);
        NH_WEB_END(Nh_Web_initParseResult(PartialClassMember_p))
    }

    Nh_Web_ParseResult ReadWriteAttribute = Nh_Web_parseReadWriteAttribute(Parser_p);
    if (ReadWriteAttribute.Node_p != NULL) 
    {
        Nh_Web_ParseNode *PartialClassMember_p = Nh_Web_allocateNonTerminalParseNode("PartialClassMember", 1);
        Nh_appendToList(&PartialClassMember_p->Children, ReadWriteAttribute.Node_p);
        NH_WEB_END(Nh_Web_initParseResult(PartialClassMember_p))
    }

    Nh_Web_ParseResult ReadWriteMaplike = Nh_Web_parseReadWriteMaplike(Parser_p);
    if (ReadWriteMaplike.Node_p != NULL) 
    {
        Nh_Web_ParseNode *PartialClassMember_p = Nh_Web_allocateNonTerminalParseNode("PartialClassMember", 1);
        Nh_appendToList(&PartialClassMember_p->Children, ReadWriteMaplike.Node_p);
        NH_WEB_END(Nh_Web_initParseResult(PartialClassMember_p))
    }

    Nh_Web_ParseResult ReadWriteSetlike = Nh_Web_parseReadWriteSetlike(Parser_p);
    if (ReadWriteSetlike.Node_p != NULL) 
    {
        Nh_Web_ParseNode *PartialClassMember_p = Nh_Web_allocateNonTerminalParseNode("PartialClassMember", 1);
        Nh_appendToList(&PartialClassMember_p->Children, ReadWriteSetlike.Node_p);
        NH_WEB_END(Nh_Web_initParseResult(PartialClassMember_p))
    }

    Nh_Web_ParseResult InheritAttribute = Nh_Web_parseInheritAttribute(Parser_p);
    if (InheritAttribute.Node_p != NULL) 
    {
        Nh_Web_ParseNode *PartialClassMember_p = Nh_Web_allocateNonTerminalParseNode("PartialClassMember", 1);
        Nh_appendToList(&PartialClassMember_p->Children, InheritAttribute.Node_p);
        NH_WEB_END(Nh_Web_initParseResult(PartialClassMember_p))
    }

    Nh_Web_ParseResult PrivateAttribute = Nh_Web_parsePrivateAttribute(Parser_p);
    if (PrivateAttribute.Node_p != NULL) 
    {
        Nh_Web_ParseNode *PartialClassMember_p = Nh_Web_allocateNonTerminalParseNode("PartialClassMember", 1);
        Nh_appendToList(&PartialClassMember_p->Children, PrivateAttribute.Node_p);
        NH_WEB_END(Nh_Web_initParseResult(PartialClassMember_p))
    }

NH_WEB_END(Nh_Web_initParseResult(NULL))
}

static Nh_Web_ParseResult Nh_Web_parseConstructor(
    Nh_Web_Parser *Parser_p)
{
NH_WEB_BEGIN()

    if (Parser_p->unparsed <= 3) {NH_WEB_END(Nh_Web_initParseResult(NULL))}

    if (!strcmp(Parser_p->Tokens_p[0].String.bytes_p, "constructor") && Parser_p->Tokens_p[1].String.bytes_p[0] == '(')
    {
        Nh_Web_Parser LocalParser = Nh_Web_advanceParser(*Parser_p, 2);
        Nh_Web_ParseResult ArgumentList = Nh_Web_parseArgumentList(&LocalParser);

        if (LocalParser.unparsed > 1 && LocalParser.Tokens_p[0].String.bytes_p[0] == ')' && LocalParser.Tokens_p[1].String.bytes_p[0] == ';')
        {
            Nh_Web_ParseNode *Constructor_p = Nh_Web_allocateNonTerminalParseNode("Constructor", 5);
            Nh_appendToList(&Constructor_p->Children, Nh_Web_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
            Nh_appendToList(&Constructor_p->Children, Nh_Web_allocateTerminalParseNode(&Parser_p->Tokens_p[1]));
            Nh_appendToList(&Constructor_p->Children, ArgumentList.Node_p);
            Nh_appendToList(&Constructor_p->Children, Nh_Web_allocateTerminalParseNode(&LocalParser.Tokens_p[0]));
            Nh_appendToList(&Constructor_p->Children, Nh_Web_allocateTerminalParseNode(&LocalParser.Tokens_p[1]));
            *Parser_p = Nh_Web_advanceParser(LocalParser, 2);
            NH_WEB_END(Nh_Web_initParseResult(Constructor_p))
        }

        Nh_Web_freeParseNode(ArgumentList.Node_p);
    }

NH_WEB_END(Nh_Web_initParseResult(NULL))
}

static Nh_Web_ParseResult Nh_Web_parseClassMember(
    Nh_Web_Parser *Parser_p)
{
NH_WEB_BEGIN()

    Nh_Web_ParseResult PartialClassMember = Nh_Web_parsePartialClassMember(Parser_p);
    if (PartialClassMember.Node_p != NULL)  
    {
        Nh_Web_ParseNode *ClassMember_p = Nh_Web_allocateNonTerminalParseNode("ClassMember", 1);
        Nh_appendToList(&ClassMember_p->Children, PartialClassMember.Node_p);
        NH_WEB_END(Nh_Web_initParseResult(ClassMember_p))
    }

    Nh_Web_ParseResult Constructor = Nh_Web_parseConstructor(Parser_p);
    if (Constructor.Node_p != NULL)  
    {
        Nh_Web_ParseNode *ClassMember_p = Nh_Web_allocateNonTerminalParseNode("ClassMember", 1);
        Nh_appendToList(&ClassMember_p->Children, Constructor.Node_p);
        NH_WEB_END(Nh_Web_initParseResult(ClassMember_p))
    }

NH_WEB_END(Nh_Web_initParseResult(NULL))
}

static Nh_Web_ParseResult Nh_Web_parseClassMembers(
    Nh_Web_Parser *Parser_p)
{
NH_WEB_BEGIN()

    Nh_Web_ParseNode *ClassMembers_p = Nh_Web_allocateNonTerminalParseNode("ClassMembers", 3);
    Nh_Web_ParseResult ExtendedAttributeList = Nh_Web_parseExtendedAttributeList(Parser_p);
    Nh_Web_ParseResult ClassMember = Nh_Web_parseClassMember(Parser_p);

    if (ClassMember.Node_p != NULL) {
        Nh_Web_ParseResult ClassMembers = Nh_Web_parseClassMembers(Parser_p);
        Nh_appendToList(&ClassMembers_p->Children, ExtendedAttributeList.Node_p);
        Nh_appendToList(&ClassMembers_p->Children, ClassMember.Node_p);
        Nh_appendToList(&ClassMembers_p->Children, ClassMembers.Node_p);
    }
    else {Nh_Web_freeParseNode(ExtendedAttributeList.Node_p);}

NH_WEB_END(Nh_Web_initParseResult(ClassMembers_p))
}

static Nh_Web_ParseResult Nh_Web_parseClassRest(
    Nh_Web_Parser *Parser_p)
{
NH_WEB_BEGIN()

    if (Parser_p->unparsed > 0 && Parser_p->Tokens_p[0].type == NH_WEB_TOKEN_IDENTIFIER) 
    {
        Nh_Web_Parser LocalParser = Nh_Web_advanceParser(*Parser_p, 1);
        Nh_Web_ParseResult Inheritance = Nh_Web_parseInheritance(&LocalParser); 

        if (LocalParser.unparsed > 0 && LocalParser.Tokens_p[0].String.bytes_p[0] == '{') 
        {
            Nh_Web_Token *LeftBracket_p = &LocalParser.Tokens_p[0];
            LocalParser = Nh_Web_advanceParser(LocalParser, 1);

            Nh_Web_ParseResult ClassMembers = Nh_Web_parseClassMembers(&LocalParser);

            if (LocalParser.unparsed > 1 
            &&  LocalParser.Tokens_p[0].String.bytes_p[0] == '}'
            &&  LocalParser.Tokens_p[1].String.bytes_p[0] == ';') 
            {
                Nh_Web_ParseNode *ClassRest_p = Nh_Web_allocateNonTerminalParseNode("ClassRest", 6);
                Nh_appendToList(&ClassRest_p->Children, Nh_Web_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
                Nh_appendToList(&ClassRest_p->Children, Inheritance.Node_p);
                Nh_appendToList(&ClassRest_p->Children, Nh_Web_allocateTerminalParseNode(LeftBracket_p));
                Nh_appendToList(&ClassRest_p->Children, ClassMembers.Node_p);
                Nh_appendToList(&ClassRest_p->Children, Nh_Web_allocateTerminalParseNode(&LocalParser.Tokens_p[0]));
                Nh_appendToList(&ClassRest_p->Children, Nh_Web_allocateTerminalParseNode(&LocalParser.Tokens_p[1]));
                *Parser_p = Nh_Web_advanceParser(LocalParser, 2);
                NH_WEB_END(Nh_Web_initParseResult(ClassRest_p))
            }
            else {Nh_Web_freeParseNode(ClassMembers.Node_p);} 
        }
        Nh_Web_freeParseNode(Inheritance.Node_p);
    }

NH_WEB_END(Nh_Web_initParseResult(NULL))
}

static Nh_Web_ParseResult Nh_Web_parseMixinMember(
    Nh_Web_Parser *Parser_p)
{
NH_WEB_BEGIN()

    Nh_Web_ParseResult Const = Nh_Web_parseConst(Parser_p);
    if (Const.Node_p != NULL)  
    {
        Nh_Web_ParseNode *MixinMember_p = Nh_Web_allocateNonTerminalParseNode("MixinMember", 1);
        Nh_appendToList(&MixinMember_p->Children, Const.Node_p);
        NH_WEB_END(Nh_Web_initParseResult(MixinMember_p))
    }

    Nh_Web_ParseResult RegularOperation = Nh_Web_parseRegularOperation(Parser_p);
    if (RegularOperation.Node_p != NULL)  
    {
        Nh_Web_ParseNode *MixinMember_p = Nh_Web_allocateNonTerminalParseNode("MixinMember", 1);
        Nh_appendToList(&MixinMember_p->Children, RegularOperation.Node_p);
        NH_WEB_END(Nh_Web_initParseResult(MixinMember_p))
    }

    Nh_Web_ParseResult Stringifier = Nh_Web_parseStringifier(Parser_p);
    if (Stringifier.Node_p != NULL)  
    {
        Nh_Web_ParseNode *MixinMember_p = Nh_Web_allocateNonTerminalParseNode("MixinMember", 1);
        Nh_appendToList(&MixinMember_p->Children, Stringifier.Node_p);
        NH_WEB_END(Nh_Web_initParseResult(MixinMember_p))
    }

    Nh_Web_Parser LocalParser = *Parser_p;
    Nh_Web_ParseResult OptionalReadOnly = Nh_Web_parseOptionalReadOnly(&LocalParser);
    Nh_Web_ParseResult AttributeRest = Nh_Web_parseAttributeRest(&LocalParser);
    if (AttributeRest.Node_p != NULL)  
    {
        Nh_Web_ParseNode *MixinMember_p = Nh_Web_allocateNonTerminalParseNode("MixinMember", 2);
        Nh_appendToList(&MixinMember_p->Children, OptionalReadOnly.Node_p);
        Nh_appendToList(&MixinMember_p->Children, AttributeRest.Node_p);
        *Parser_p = LocalParser;
        NH_WEB_END(Nh_Web_initParseResult(MixinMember_p))
    }
    else {Nh_Web_freeParseNode(OptionalReadOnly.Node_p);}

NH_WEB_END(Nh_Web_initParseResult(NULL))
}

static Nh_Web_ParseResult Nh_Web_parseMixinMembers(
    Nh_Web_Parser *Parser_p)
{
NH_WEB_BEGIN()

    Nh_Web_ParseNode *MixinMembers_p = Nh_Web_allocateNonTerminalParseNode("MixinMembers", 3);
    Nh_Web_ParseResult ExtendedAttributeList = Nh_Web_parseExtendedAttributeList(Parser_p);
    Nh_Web_ParseResult MixinMember = Nh_Web_parseMixinMember(Parser_p);

    if (MixinMember.Node_p != NULL) {
        Nh_Web_ParseResult MixinMembers = Nh_Web_parseMixinMembers(Parser_p);
        Nh_appendToList(&MixinMembers_p->Children, ExtendedAttributeList.Node_p);
        Nh_appendToList(&MixinMembers_p->Children, MixinMember.Node_p);
        Nh_appendToList(&MixinMembers_p->Children, MixinMembers.Node_p);
    }
    else {Nh_Web_freeParseNode(ExtendedAttributeList.Node_p);}

NH_WEB_END(Nh_Web_initParseResult(MixinMembers_p))
}

static Nh_Web_ParseResult Nh_Web_parseMixinRest(
    Nh_Web_Parser *Parser_p)
{
NH_WEB_BEGIN()

    if (Parser_p->unparsed > 2 && !strcmp(Parser_p->Tokens_p[0].String.bytes_p, "mixin") && Parser_p->Tokens_p[1].type == NH_WEB_TOKEN_IDENTIFIER && Parser_p->Tokens_p[2].String.bytes_p[0] == '{') 
    {
        Nh_Web_Parser LocalParser = Nh_Web_advanceParser(*Parser_p, 3);
        Nh_Web_ParseResult MixinMembers = Nh_Web_parseMixinMembers(&LocalParser); 

        if (LocalParser.unparsed > 1 && LocalParser.Tokens_p[0].String.bytes_p[0] == '}' && LocalParser.Tokens_p[1].String.bytes_p[0] == ';') 
        {
            Nh_Web_ParseNode *MixinRest_p = Nh_Web_allocateNonTerminalParseNode("MixinRest", 6);
            Nh_appendToList(&MixinRest_p->Children, Nh_Web_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
            Nh_appendToList(&MixinRest_p->Children, Nh_Web_allocateTerminalParseNode(&Parser_p->Tokens_p[1]));
            Nh_appendToList(&MixinRest_p->Children, Nh_Web_allocateTerminalParseNode(&Parser_p->Tokens_p[2]));
            Nh_appendToList(&MixinRest_p->Children, MixinMembers.Node_p);
            Nh_appendToList(&MixinRest_p->Children, Nh_Web_allocateTerminalParseNode(&LocalParser.Tokens_p[0]));
            Nh_appendToList(&MixinRest_p->Children, Nh_Web_allocateTerminalParseNode(&LocalParser.Tokens_p[1]));
            *Parser_p = Nh_Web_advanceParser(LocalParser, 2);
            NH_WEB_END(Nh_Web_initParseResult(MixinRest_p))
        }

        Nh_Web_freeParseNode(MixinMembers.Node_p);
    }

NH_WEB_END(Nh_Web_initParseResult(NULL))
}

static Nh_Web_ParseResult Nh_Web_parseClassOrMixin(
    Nh_Web_Parser *Parser_p)
{
NH_WEB_BEGIN()

    Nh_Web_ParseResult ClassRest = Nh_Web_parseClassRest(Parser_p);

    if (ClassRest.Node_p != NULL) 
    {
        Nh_Web_ParseNode *ClassOrMixin_p = Nh_Web_allocateNonTerminalParseNode("ClassOrMixin", 1);
        Nh_appendToList(&ClassOrMixin_p->Children, ClassRest.Node_p);
        NH_WEB_END(Nh_Web_initParseResult(ClassOrMixin_p))
    }
    
    Nh_Web_ParseResult MixinRest = Nh_Web_parseMixinRest(Parser_p);

    if (MixinRest.Node_p != NULL) 
    {
        Nh_Web_ParseNode *ClassOrMixin_p = Nh_Web_allocateNonTerminalParseNode("ClassOrMixin", 1);
        Nh_appendToList(&ClassOrMixin_p->Children, MixinRest.Node_p);
        NH_WEB_END(Nh_Web_initParseResult(ClassOrMixin_p))
    }

NH_WEB_END(Nh_Web_initParseResult(NULL))
}

static Nh_Web_ParseResult Nh_Web_parseCallbackOrClassOrMixin(
    Nh_Web_Parser *Parser_p)
{
NH_WEB_BEGIN()

    if (Parser_p->unparsed > 0 && !strcmp("callback", Parser_p->Tokens_p[0].String.bytes_p)) 
    {
        Nh_Web_Parser LocalParser = Nh_Web_advanceParser(*Parser_p, 1);
        Nh_Web_ParseResult CallbackRestOrClass = Nh_Web_parseCallbackRestOrClass(&LocalParser);

        if (CallbackRestOrClass.Node_p != NULL) 
        {
            Nh_Web_ParseNode *CallbackOrClassOrMixin_p = Nh_Web_allocateNonTerminalParseNode("CallbackOrClassOrMixin", 2);
            Nh_appendToList(&CallbackOrClassOrMixin_p->Children, Nh_Web_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
            Nh_appendToList(&CallbackOrClassOrMixin_p->Children, CallbackRestOrClass.Node_p);
            *Parser_p = LocalParser;
            NH_WEB_END(Nh_Web_initParseResult(CallbackOrClassOrMixin_p))
        }
    }

    if (Parser_p->unparsed > 0 && !strcmp("class", Parser_p->Tokens_p[0].String.bytes_p)) 
    {
        Nh_Web_Parser LocalParser = Nh_Web_advanceParser(*Parser_p, 1);
        Nh_Web_ParseResult ClassOrMixin = Nh_Web_parseClassOrMixin(&LocalParser);

        if (ClassOrMixin.Node_p != NULL) 
        {
            Nh_Web_ParseNode *CallbackOrClassOrMixin_p = Nh_Web_allocateNonTerminalParseNode("CallbackOrClassOrMixin", 2);
            Nh_appendToList(&CallbackOrClassOrMixin_p->Children, Nh_Web_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
            Nh_appendToList(&CallbackOrClassOrMixin_p->Children, ClassOrMixin.Node_p);
            *Parser_p = LocalParser;
            NH_WEB_END(Nh_Web_initParseResult(CallbackOrClassOrMixin_p))
        }
    }
    
NH_WEB_END(Nh_Web_initParseResult(NULL))
}

static Nh_Web_ParseResult Nh_Web_parseNamespaceMember(
    Nh_Web_Parser *Parser_p)
{
NH_WEB_BEGIN()

    Nh_Web_ParseResult RegularOperation = Nh_Web_parseRegularOperation(Parser_p);
    if (RegularOperation.Node_p != NULL)  
    {
        Nh_Web_ParseNode *NamespaceMember_p = Nh_Web_allocateNonTerminalParseNode("NamespaceMember", 1);
        Nh_appendToList(&NamespaceMember_p->Children, RegularOperation.Node_p);
        NH_WEB_END(Nh_Web_initParseResult(NamespaceMember_p))
    }

    if (Parser_p->unparsed > 0 && !strcmp(Parser_p->Tokens_p[0].String.bytes_p, "readonly"))
    {
        Nh_Web_Parser LocalParser = Nh_Web_advanceParser(*Parser_p, 1);
        Nh_Web_ParseResult AttributeRest = Nh_Web_parseAttributeRest(&LocalParser);

        if (AttributeRest.Node_p != NULL)  
        {
            Nh_Web_ParseNode *NamespaceMember_p = Nh_Web_allocateNonTerminalParseNode("NamespaceMember", 2);
            Nh_appendToList(&NamespaceMember_p->Children, Nh_Web_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
            Nh_appendToList(&NamespaceMember_p->Children, AttributeRest.Node_p);
            *Parser_p = LocalParser;
            NH_WEB_END(Nh_Web_initParseResult(NamespaceMember_p))
        }
    }

NH_WEB_END(Nh_Web_initParseResult(NULL))
}

static Nh_Web_ParseResult Nh_Web_parseNamespaceMembers(
    Nh_Web_Parser *Parser_p)
{
NH_WEB_BEGIN()

    Nh_Web_ParseNode *NamespaceMembers_p = Nh_Web_allocateNonTerminalParseNode("NamespaceMembers", 3);
    Nh_Web_ParseResult ExtendedAttributeList = Nh_Web_parseExtendedAttributeList(Parser_p);
    Nh_Web_ParseResult NamespaceMember = Nh_Web_parseNamespaceMember(Parser_p);

    if (NamespaceMember.Node_p != NULL) {
        Nh_Web_ParseResult NamespaceMembers = Nh_Web_parseNamespaceMembers(Parser_p);
        Nh_appendToList(&NamespaceMembers_p->Children, ExtendedAttributeList.Node_p);
        Nh_appendToList(&NamespaceMembers_p->Children, NamespaceMember.Node_p);
        Nh_appendToList(&NamespaceMembers_p->Children, NamespaceMembers.Node_p);
    }
    else {Nh_Web_freeParseNode(ExtendedAttributeList.Node_p);}

NH_WEB_END(Nh_Web_initParseResult(NamespaceMembers_p))
}

static Nh_Web_ParseResult Nh_Web_parseNamespace(
    Nh_Web_Parser *Parser_p)
{
NH_WEB_BEGIN()

    if (Parser_p->unparsed > 2 && !strcmp(Parser_p->Tokens_p[0].String.bytes_p, "namespace") && Parser_p->Tokens_p[1].type == NH_WEB_TOKEN_IDENTIFIER && Parser_p->Tokens_p[2].String.bytes_p[0] == '{') 
    {
        Nh_Web_Parser LocalParser = Nh_Web_advanceParser(*Parser_p, 3);
        Nh_Web_ParseResult NamespaceMembers = Nh_Web_parseNamespaceMembers(&LocalParser); 

        if (LocalParser.unparsed > 1 && LocalParser.Tokens_p[0].String.bytes_p[0] == '}' && LocalParser.Tokens_p[1].String.bytes_p[0] == ';') 
        {
            Nh_Web_ParseNode *Namespace_p = Nh_Web_allocateNonTerminalParseNode("Namespace", 6);
            Nh_appendToList(&Namespace_p->Children, Nh_Web_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
            Nh_appendToList(&Namespace_p->Children, Nh_Web_allocateTerminalParseNode(&Parser_p->Tokens_p[1]));
            Nh_appendToList(&Namespace_p->Children, Nh_Web_allocateTerminalParseNode(&Parser_p->Tokens_p[2]));
            Nh_appendToList(&Namespace_p->Children, NamespaceMembers.Node_p);
            Nh_appendToList(&Namespace_p->Children, Nh_Web_allocateTerminalParseNode(&LocalParser.Tokens_p[0]));
            Nh_appendToList(&Namespace_p->Children, Nh_Web_allocateTerminalParseNode(&LocalParser.Tokens_p[1]));
            *Parser_p = Nh_Web_advanceParser(LocalParser, 2);
            NH_WEB_END(Nh_Web_initParseResult(Namespace_p))
        }

        Nh_Web_freeParseNode(NamespaceMembers.Node_p);
    }

NH_WEB_END(Nh_Web_initParseResult(NULL))
}

static Nh_Web_ParseResult Nh_Web_parsePartialClassMembers(
    Nh_Web_Parser *Parser_p)
{
NH_WEB_BEGIN()

    Nh_Web_ParseNode *PartialClassMembers_p = Nh_Web_allocateNonTerminalParseNode("PartialClassMembers", 3);
    Nh_Web_ParseResult ExtendedAttributeList = Nh_Web_parseExtendedAttributeList(Parser_p);
    Nh_Web_ParseResult PartialClassMember = Nh_Web_parsePartialClassMember(Parser_p);

    if (PartialClassMember.Node_p != NULL) {
        Nh_Web_ParseResult PartialClassMembers = Nh_Web_parsePartialClassMembers(Parser_p);
        Nh_appendToList(&PartialClassMembers_p->Children, ExtendedAttributeList.Node_p);
        Nh_appendToList(&PartialClassMembers_p->Children, PartialClassMember.Node_p);
        Nh_appendToList(&PartialClassMembers_p->Children, PartialClassMembers.Node_p);
    }
    else {Nh_Web_freeParseNode(ExtendedAttributeList.Node_p);}

NH_WEB_END(Nh_Web_initParseResult(PartialClassMembers_p))
}

static Nh_Web_ParseResult Nh_Web_parsePartialClassRest(
    Nh_Web_Parser *Parser_p)
{
NH_WEB_BEGIN()

    if (Parser_p->unparsed > 1 && Parser_p->Tokens_p[0].type == NH_WEB_TOKEN_IDENTIFIER && Parser_p->Tokens_p[1].String.bytes_p[0] == '{') 
    {
        Nh_Web_Parser LocalParser = Nh_Web_advanceParser(*Parser_p, 2);
        Nh_Web_ParseResult PartialClassMembers = Nh_Web_parsePartialClassMembers(&LocalParser); 

        if (LocalParser.unparsed > 1 && LocalParser.Tokens_p[0].String.bytes_p[0] == '}' && LocalParser.Tokens_p[1].String.bytes_p[0] == ';') 
        {
            Nh_Web_ParseNode *PartialClassRest_p = Nh_Web_allocateNonTerminalParseNode("PartialClassRest", 5);
            Nh_appendToList(&PartialClassRest_p->Children, Nh_Web_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
            Nh_appendToList(&PartialClassRest_p->Children, Nh_Web_allocateTerminalParseNode(&Parser_p->Tokens_p[1]));
            Nh_appendToList(&PartialClassRest_p->Children, PartialClassMembers.Node_p);
            Nh_appendToList(&PartialClassRest_p->Children, Nh_Web_allocateTerminalParseNode(&LocalParser.Tokens_p[0]));
            Nh_appendToList(&PartialClassRest_p->Children, Nh_Web_allocateTerminalParseNode(&LocalParser.Tokens_p[1]));
            *Parser_p = Nh_Web_advanceParser(LocalParser, 2);
            NH_WEB_END(Nh_Web_initParseResult(PartialClassRest_p))
        }

        Nh_Web_freeParseNode(PartialClassMembers.Node_p);
    }

NH_WEB_END(Nh_Web_initParseResult(NULL))
}

static Nh_Web_ParseResult Nh_Web_parsePartialClassOrPartialMixin(
    Nh_Web_Parser *Parser_p)
{
NH_WEB_BEGIN()

    Nh_Web_ParseResult PartialClassRest = Nh_Web_parsePartialClassRest(Parser_p);

    if (PartialClassRest.Node_p != NULL) 
    {
        Nh_Web_ParseNode *PartialClassOrPartialMixin_p = Nh_Web_allocateNonTerminalParseNode("PartialClassOrPartialMixin", 1);
        Nh_appendToList(&PartialClassOrPartialMixin_p->Children, PartialClassRest.Node_p);
        NH_WEB_END(Nh_Web_initParseResult(PartialClassOrPartialMixin_p))
    }
    
    Nh_Web_ParseResult MixinRest = Nh_Web_parseMixinRest(Parser_p);

    if (MixinRest.Node_p != NULL) 
    {
        Nh_Web_ParseNode *PartialClassOrPartialMixin_p = Nh_Web_allocateNonTerminalParseNode("PartialClassOrPartialMixin", 1);
        Nh_appendToList(&PartialClassOrPartialMixin_p->Children, MixinRest.Node_p);
        NH_WEB_END(Nh_Web_initParseResult(PartialClassOrPartialMixin_p))
    }

NH_WEB_END(Nh_Web_initParseResult(NULL))
}

static Nh_Web_ParseResult Nh_Web_parseDictionaryMemberRest(
    Nh_Web_Parser *Parser_p)
{
NH_WEB_BEGIN()

    if (Parser_p->unparsed > 0 && !strcmp(Parser_p->Tokens_p[0].String.bytes_p, "required")) 
    {
        Nh_Web_Parser LocalParser = Nh_Web_advanceParser(*Parser_p, 1);
        Nh_Web_ParseResult TypeWithExtendedAttributes = Nh_Web_parseTypeWithExtendedAttributes(&LocalParser); 

        if (TypeWithExtendedAttributes.Node_p != NULL && LocalParser.unparsed > 1 && LocalParser.Tokens_p[0].type == NH_WEB_TOKEN_IDENTIFIER && LocalParser.Tokens_p[1].String.bytes_p[0] == ';') 
        {
            Nh_Web_ParseNode *DictionaryMemberRest_p = Nh_Web_allocateNonTerminalParseNode("DictionaryMemberRest", 4);
            Nh_appendToList(&DictionaryMemberRest_p->Children, Nh_Web_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
            Nh_appendToList(&DictionaryMemberRest_p->Children, TypeWithExtendedAttributes.Node_p);
            Nh_appendToList(&DictionaryMemberRest_p->Children, Nh_Web_allocateTerminalParseNode(&LocalParser.Tokens_p[0]));
            Nh_appendToList(&DictionaryMemberRest_p->Children, Nh_Web_allocateTerminalParseNode(&LocalParser.Tokens_p[1]));
            *Parser_p = Nh_Web_advanceParser(LocalParser, 2);
            NH_WEB_END(Nh_Web_initParseResult(DictionaryMemberRest_p))
        }

        Nh_Web_freeParseNode(TypeWithExtendedAttributes.Node_p);
    }
    
    Nh_Web_Parser LocalParser = *Parser_p;
    Nh_Web_ParseResult Type = Nh_Web_parseType(&LocalParser); 

    if (Type.Node_p != NULL && LocalParser.unparsed > 1 && LocalParser.Tokens_p[0].type == NH_WEB_TOKEN_IDENTIFIER) 
    {
        Nh_Web_Token *Identifier_p = &LocalParser.Tokens_p[0];
        LocalParser = Nh_Web_advanceParser(LocalParser, 1);
        Nh_Web_ParseResult Default = Nh_Web_parseDefault(&LocalParser); 

        if (LocalParser.unparsed > 0 && LocalParser.Tokens_p[0].String.bytes_p[0] == ';')
        {
            Nh_Web_ParseNode *DictionaryMemberRest_p = Nh_Web_allocateNonTerminalParseNode("DictionaryMemberRest", 4);
            Nh_appendToList(&DictionaryMemberRest_p->Children, Type.Node_p);
            Nh_appendToList(&DictionaryMemberRest_p->Children, Nh_Web_allocateTerminalParseNode(Identifier_p));
            Nh_appendToList(&DictionaryMemberRest_p->Children, Default.Node_p);
            Nh_appendToList(&DictionaryMemberRest_p->Children, Nh_Web_allocateTerminalParseNode(&LocalParser.Tokens_p[0]));
            *Parser_p = Nh_Web_advanceParser(LocalParser, 1);
            NH_WEB_END(Nh_Web_initParseResult(DictionaryMemberRest_p))
        }

        Nh_Web_freeParseNode(Default.Node_p);
        Nh_Web_freeParseNode(Type.Node_p);
    }

NH_WEB_END(Nh_Web_initParseResult(NULL))
}

static Nh_Web_ParseResult Nh_Web_parseDictionaryMember(
    Nh_Web_Parser *Parser_p)
{
NH_WEB_BEGIN()

    Nh_Web_Parser LocalParser = *Parser_p;
    Nh_Web_ParseResult ExtendedAttributeList = Nh_Web_parseExtendedAttributeList(&LocalParser);
    Nh_Web_ParseResult DictionaryMemberRest = Nh_Web_parseDictionaryMemberRest(&LocalParser);

    if (DictionaryMemberRest.Node_p != NULL) {
        Nh_Web_ParseNode *DictionaryMember_p = Nh_Web_allocateNonTerminalParseNode("DictionaryMember", 2);
        Nh_appendToList(&DictionaryMember_p->Children, ExtendedAttributeList.Node_p);
        Nh_appendToList(&DictionaryMember_p->Children, DictionaryMemberRest.Node_p);
        *Parser_p = LocalParser;
        NH_WEB_END(Nh_Web_initParseResult(DictionaryMember_p))
    }
    else {Nh_Web_freeParseNode(ExtendedAttributeList.Node_p);}

NH_WEB_END(Nh_Web_initParseResult(NULL))
}

static Nh_Web_ParseResult Nh_Web_parseDictionaryMembers(
    Nh_Web_Parser *Parser_p)
{
NH_WEB_BEGIN()

    Nh_Web_ParseNode *DictionaryMembers_p = Nh_Web_allocateNonTerminalParseNode("DictionaryMembers", 2);
    Nh_Web_ParseResult DictionaryMember = Nh_Web_parseDictionaryMember(Parser_p);

    if (DictionaryMember.Node_p != NULL) {
        Nh_Web_ParseResult DictionaryMembers = Nh_Web_parseDictionaryMembers(Parser_p);
        Nh_appendToList(&DictionaryMembers_p->Children, DictionaryMember.Node_p);
        Nh_appendToList(&DictionaryMembers_p->Children, DictionaryMembers.Node_p);
    }

NH_WEB_END(Nh_Web_initParseResult(DictionaryMembers_p))
}

static Nh_Web_ParseResult Nh_Web_parsePartialDictionary(
    Nh_Web_Parser *Parser_p)
{
NH_WEB_BEGIN()

    if (Parser_p->unparsed > 2 && !strcmp(Parser_p->Tokens_p[0].String.bytes_p, "dictionary") && Parser_p->Tokens_p[1].type == NH_WEB_TOKEN_IDENTIFIER && Parser_p->Tokens_p[2].String.bytes_p[0] == '{') 
    {
        Nh_Web_Parser LocalParser = Nh_Web_advanceParser(*Parser_p, 3);
        Nh_Web_ParseResult DictionaryMembers = Nh_Web_parseDictionaryMembers(&LocalParser); 

        if (LocalParser.unparsed > 1 && LocalParser.Tokens_p[0].String.bytes_p[0] == '}' && LocalParser.Tokens_p[1].String.bytes_p[0] == ';') 
        {
            Nh_Web_ParseNode *PartialDictionary_p = Nh_Web_allocateNonTerminalParseNode("PartialDictionary", 6);
            Nh_appendToList(&PartialDictionary_p->Children, Nh_Web_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
            Nh_appendToList(&PartialDictionary_p->Children, Nh_Web_allocateTerminalParseNode(&Parser_p->Tokens_p[1]));
            Nh_appendToList(&PartialDictionary_p->Children, Nh_Web_allocateTerminalParseNode(&Parser_p->Tokens_p[2]));
            Nh_appendToList(&PartialDictionary_p->Children, DictionaryMembers.Node_p);
            Nh_appendToList(&PartialDictionary_p->Children, Nh_Web_allocateTerminalParseNode(&LocalParser.Tokens_p[0]));
            Nh_appendToList(&PartialDictionary_p->Children, Nh_Web_allocateTerminalParseNode(&LocalParser.Tokens_p[1]));
            *Parser_p = Nh_Web_advanceParser(LocalParser, 2);
            NH_WEB_END(Nh_Web_initParseResult(PartialDictionary_p))
        }

        Nh_Web_freeParseNode(DictionaryMembers.Node_p);
    }

NH_WEB_END(Nh_Web_initParseResult(NULL))
}

static Nh_Web_ParseResult Nh_Web_parsePartialDefinition(
    Nh_Web_Parser *Parser_p)
{
NH_WEB_BEGIN()

    if (Parser_p->unparsed > 0 && !strcmp(Parser_p->Tokens_p[0].String.bytes_p, "class")) 
    {
        Nh_Web_Parser LocalParser = Nh_Web_advanceParser(*Parser_p, 1);
        Nh_Web_ParseResult PartialClassOrPartialMixin = Nh_Web_parsePartialClassOrPartialMixin(&LocalParser); 

        if (PartialClassOrPartialMixin.Node_p != NULL) 
        {
            Nh_Web_ParseNode *PartialDefinition_p = Nh_Web_allocateNonTerminalParseNode("PartialDefinition", 2);
            Nh_appendToList(&PartialDefinition_p->Children, Nh_Web_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
            Nh_appendToList(&PartialDefinition_p->Children, PartialClassOrPartialMixin.Node_p);
            *Parser_p = LocalParser;
            NH_WEB_END(Nh_Web_initParseResult(PartialDefinition_p))
        }
    }

    Nh_Web_ParseResult PartialDictionary = Nh_Web_parsePartialDictionary(Parser_p);

    if (PartialDictionary.Node_p != NULL) 
    {
        Nh_Web_ParseNode *PartialDefinition_p = Nh_Web_allocateNonTerminalParseNode("PartialDefinition", 1);
        Nh_appendToList(&PartialDefinition_p->Children, PartialDictionary.Node_p);
        NH_WEB_END(Nh_Web_initParseResult(PartialDefinition_p))
    }

    Nh_Web_ParseResult Namespace = Nh_Web_parseNamespace(Parser_p);

    if (Namespace.Node_p != NULL) 
    {
        Nh_Web_ParseNode *PartialDefinition_p = Nh_Web_allocateNonTerminalParseNode("PartialDefinition", 1);
        Nh_appendToList(&PartialDefinition_p->Children, Namespace.Node_p);
        NH_WEB_END(Nh_Web_initParseResult(PartialDefinition_p))
    }

NH_WEB_END(Nh_Web_initParseResult(NULL))
}

static Nh_Web_ParseResult Nh_Web_parsePartial(
    Nh_Web_Parser *Parser_p)
{
NH_WEB_BEGIN()

    if (Parser_p->unparsed > 0 && !strcmp(Parser_p->Tokens_p[0].String.bytes_p, "partial")) 
    {
        Nh_Web_Parser LocalParser = Nh_Web_advanceParser(*Parser_p, 1);
        Nh_Web_ParseResult PartialDefinition = Nh_Web_parsePartialDefinition(&LocalParser); 

        if (PartialDefinition.Node_p != NULL) 
        {
            Nh_Web_ParseNode *Partial_p = Nh_Web_allocateNonTerminalParseNode("Partial", 2);
            Nh_appendToList(&Partial_p->Children, Nh_Web_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
            Nh_appendToList(&Partial_p->Children, PartialDefinition.Node_p);
            *Parser_p = LocalParser;
            NH_WEB_END(Nh_Web_initParseResult(Partial_p))
        }
    }

NH_WEB_END(Nh_Web_initParseResult(NULL))
}

static Nh_Web_ParseResult Nh_Web_parseDictionary(
    Nh_Web_Parser *Parser_p)
{
NH_WEB_BEGIN()

    if (Parser_p->unparsed > 2 && !strcmp(Parser_p->Tokens_p[0].String.bytes_p, "dictionary") && Parser_p->Tokens_p[1].type == NH_WEB_TOKEN_IDENTIFIER) 
    {
        Nh_Web_Parser LocalParser = Nh_Web_advanceParser(*Parser_p, 2);
        Nh_Web_ParseResult Inheritance = Nh_Web_parseInheritance(&LocalParser); 

        if (LocalParser.unparsed > 2 && LocalParser.Tokens_p[0].String.bytes_p[0] == '{')
        {
            Nh_Web_Token *OpenBracket_p = &LocalParser.Tokens_p[0];
            LocalParser = Nh_Web_advanceParser(LocalParser, 1);
            Nh_Web_ParseResult DictionaryMembers = Nh_Web_parseDictionaryMembers(&LocalParser); 

            if (LocalParser.unparsed > 1 && LocalParser.Tokens_p[0].String.bytes_p[0] == '}' && LocalParser.Tokens_p[1].String.bytes_p[0] == ';') 
            {
                Nh_Web_ParseNode *Dictionary_p = Nh_Web_allocateNonTerminalParseNode("Dictionary", 7);
                Nh_appendToList(&Dictionary_p->Children, Nh_Web_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
                Nh_appendToList(&Dictionary_p->Children, Nh_Web_allocateTerminalParseNode(&Parser_p->Tokens_p[1]));
                Nh_appendToList(&Dictionary_p->Children, Inheritance.Node_p);
                Nh_appendToList(&Dictionary_p->Children, Nh_Web_allocateTerminalParseNode(OpenBracket_p));
                Nh_appendToList(&Dictionary_p->Children, DictionaryMembers.Node_p);
                Nh_appendToList(&Dictionary_p->Children, Nh_Web_allocateTerminalParseNode(&LocalParser.Tokens_p[0]));
                Nh_appendToList(&Dictionary_p->Children, Nh_Web_allocateTerminalParseNode(&LocalParser.Tokens_p[1]));
                *Parser_p = Nh_Web_advanceParser(LocalParser, 2);
                NH_WEB_END(Nh_Web_initParseResult(Dictionary_p))
            }
            Nh_Web_freeParseNode(DictionaryMembers.Node_p);
        }
        Nh_Web_freeParseNode(Inheritance.Node_p);
    }

NH_WEB_END(Nh_Web_initParseResult(NULL))
}

static Nh_Web_ParseResult Nh_Web_parseEnumValueListComma(
    Nh_Web_Parser *Parser_p
);

static Nh_Web_ParseResult Nh_Web_parseEnumValueListString(
    Nh_Web_Parser *Parser_p)
{
NH_WEB_BEGIN()

    Nh_Web_ParseNode *EnumValueListString_p = Nh_Web_allocateNonTerminalParseNode("EnumValueListString", 2);

    if (Parser_p->unparsed > 0 && Parser_p->Tokens_p[0].type == NH_WEB_TOKEN_STRING) 
    {
        Nh_Web_Parser LocalParser = Nh_Web_advanceParser(*Parser_p, 1);
        Nh_Web_ParseResult EnumValueListComma = Nh_Web_parseEnumValueListComma(&LocalParser);
        Nh_appendToList(&EnumValueListString_p->Children, Nh_Web_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
        Nh_appendToList(&EnumValueListString_p->Children, EnumValueListComma.Node_p);
        *Parser_p = LocalParser;
    }

NH_WEB_END(Nh_Web_initParseResult(EnumValueListString_p))
}

static Nh_Web_ParseResult Nh_Web_parseEnumValueListComma(
    Nh_Web_Parser *Parser_p)
{
NH_WEB_BEGIN()

    Nh_Web_ParseNode *EnumValueListComma_p = Nh_Web_allocateNonTerminalParseNode("EnumValueListComma", 2);

    if (Parser_p->unparsed > 0 && Parser_p->Tokens_p[0].String.bytes_p[0] == ',') 
    {
        Nh_Web_Parser LocalParser = Nh_Web_advanceParser(*Parser_p, 1);
        Nh_Web_ParseResult EnumValueListString = Nh_Web_parseEnumValueListString(&LocalParser);
        Nh_appendToList(&EnumValueListComma_p->Children, Nh_Web_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
        Nh_appendToList(&EnumValueListComma_p->Children, EnumValueListString.Node_p);
        *Parser_p = LocalParser;
    }

NH_WEB_END(Nh_Web_initParseResult(EnumValueListComma_p))
}

static Nh_Web_ParseResult Nh_Web_parseEnumValueList(
    Nh_Web_Parser *Parser_p)
{
NH_WEB_BEGIN()

    if (Parser_p->unparsed > 0 && Parser_p->Tokens_p[0].type == NH_WEB_TOKEN_STRING) 
    {
        Nh_Web_Parser LocalParser = Nh_Web_advanceParser(*Parser_p, 1);
        Nh_Web_ParseResult EnumValueListComma = Nh_Web_parseEnumValueListComma(&LocalParser);

        Nh_Web_ParseNode *EnumValueList_p = Nh_Web_allocateNonTerminalParseNode("EnumValueList", 2);
        Nh_appendToList(&EnumValueList_p->Children, Nh_Web_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
        Nh_appendToList(&EnumValueList_p->Children, EnumValueListComma.Node_p);

        *Parser_p = LocalParser;

        NH_WEB_END(Nh_Web_initParseResult(EnumValueList_p))
    }

NH_WEB_END(Nh_Web_initParseResult(NULL))
}

static Nh_Web_ParseResult Nh_Web_parseEnum(
    Nh_Web_Parser *Parser_p)
{
NH_WEB_BEGIN()

    if (Parser_p->unparsed > 2 && !strcmp(Parser_p->Tokens_p[0].String.bytes_p, "enum") && Parser_p->Tokens_p[1].type == NH_WEB_TOKEN_IDENTIFIER && Parser_p->Tokens_p[2].String.bytes_p[0] == '{') 
    {
        Nh_Web_Parser LocalParser = Nh_Web_advanceParser(*Parser_p, 3);
        Nh_Web_ParseResult EnumValueList = Nh_Web_parseEnumValueList(&LocalParser); 

        if (EnumValueList.Node_p != NULL && LocalParser.unparsed > 1 && LocalParser.Tokens_p[0].String.bytes_p[0] == '}' && LocalParser.Tokens_p[1].String.bytes_p[0] == ';') 
        {
            Nh_Web_ParseNode *Enum_p = Nh_Web_allocateNonTerminalParseNode("Enum", 6);
            Nh_appendToList(&Enum_p->Children, Nh_Web_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
            Nh_appendToList(&Enum_p->Children, Nh_Web_allocateTerminalParseNode(&Parser_p->Tokens_p[1]));
            Nh_appendToList(&Enum_p->Children, Nh_Web_allocateTerminalParseNode(&Parser_p->Tokens_p[1]));
            Nh_appendToList(&Enum_p->Children, EnumValueList.Node_p);
            Nh_appendToList(&Enum_p->Children, Nh_Web_allocateTerminalParseNode(&LocalParser.Tokens_p[0]));
            Nh_appendToList(&Enum_p->Children, Nh_Web_allocateTerminalParseNode(&LocalParser.Tokens_p[1]));
            *Parser_p = Nh_Web_advanceParser(LocalParser, 2);
            NH_WEB_END(Nh_Web_initParseResult(Enum_p))
        }
    }

NH_WEB_END(Nh_Web_initParseResult(NULL))
}

static Nh_Web_ParseResult Nh_Web_parseTypedef(
    Nh_Web_Parser *Parser_p)
{
NH_WEB_BEGIN()

    if (Parser_p->unparsed > 0 && !strcmp(Parser_p->Tokens_p[0].String.bytes_p, "typedef")) 
    {
        Nh_Web_Parser LocalParser = Nh_Web_advanceParser(*Parser_p, 1);
        Nh_Web_ParseResult TypeWithExtendedAttributes = Nh_Web_parseTypeWithExtendedAttributes(&LocalParser); 

        if (TypeWithExtendedAttributes.Node_p != NULL && LocalParser.unparsed > 1 && LocalParser.Tokens_p[0].type == NH_WEB_TOKEN_IDENTIFIER && LocalParser.Tokens_p[1].String.bytes_p[0] == ';') 
        {
            Nh_Web_ParseNode *Typedef_p = Nh_Web_allocateNonTerminalParseNode("Typedef", 4);
            Nh_appendToList(&Typedef_p->Children, Nh_Web_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
            Nh_appendToList(&Typedef_p->Children, TypeWithExtendedAttributes.Node_p);
            Nh_appendToList(&Typedef_p->Children, Nh_Web_allocateTerminalParseNode(&LocalParser.Tokens_p[0]));
            Nh_appendToList(&Typedef_p->Children, Nh_Web_allocateTerminalParseNode(&LocalParser.Tokens_p[1]));
            *Parser_p = Nh_Web_advanceParser(LocalParser, 2);
            NH_WEB_END(Nh_Web_initParseResult(Typedef_p))
        }

        Nh_Web_freeParseNode(TypeWithExtendedAttributes.Node_p);
    }

NH_WEB_END(Nh_Web_initParseResult(NULL))
}

static Nh_Web_ParseResult Nh_Web_parseIncludesStatement(
    Nh_Web_Parser *Parser_p)
{
NH_WEB_BEGIN()

    if (Parser_p->unparsed > 3 
    &&  Parser_p->Tokens_p[0].type == NH_WEB_TOKEN_IDENTIFIER 
    &&  !strcmp(Parser_p->Tokens_p[1].String.bytes_p, "includes")
    &&  Parser_p->Tokens_p[2].type == NH_WEB_TOKEN_IDENTIFIER 
    &&  Parser_p->Tokens_p[3].String.bytes_p[0] == ';')
    {
        Nh_Web_ParseNode *IncludesStatement_p = Nh_Web_allocateNonTerminalParseNode("IncludesStatement", 4);
        Nh_appendToList(&IncludesStatement_p->Children, Nh_Web_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
        Nh_appendToList(&IncludesStatement_p->Children, Nh_Web_allocateTerminalParseNode(&Parser_p->Tokens_p[1]));
        Nh_appendToList(&IncludesStatement_p->Children, Nh_Web_allocateTerminalParseNode(&Parser_p->Tokens_p[2]));
        Nh_appendToList(&IncludesStatement_p->Children, Nh_Web_allocateTerminalParseNode(&Parser_p->Tokens_p[3]));
        *Parser_p = Nh_Web_advanceParser(*Parser_p, 4);
        NH_WEB_END(Nh_Web_initParseResult(IncludesStatement_p))
    }

NH_WEB_END(Nh_Web_initParseResult(NULL))
}

static Nh_Web_ParseResult Nh_Web_parseDefinition(
    Nh_Web_Parser *Parser_p)
{
NH_WEB_BEGIN()

    Nh_Web_ParseResult CallbackOrClassOrMixin = Nh_Web_parseCallbackOrClassOrMixin(Parser_p);

    if (CallbackOrClassOrMixin.Node_p != NULL) 
    {
        Nh_Web_ParseNode *Definition_p = Nh_Web_allocateNonTerminalParseNode("Definition", 1);
        Nh_appendToList(&Definition_p->Children, CallbackOrClassOrMixin.Node_p);
        NH_WEB_END(Nh_Web_initParseResult(Definition_p))
    }

    Nh_Web_ParseResult Namespace = Nh_Web_parseNamespace(Parser_p);

    if (Namespace.Node_p != NULL) 
    {
        Nh_Web_ParseNode *Definition_p = Nh_Web_allocateNonTerminalParseNode("Definition", 1);
        Nh_appendToList(&Definition_p->Children, Namespace.Node_p);
        NH_WEB_END(Nh_Web_initParseResult(Definition_p))
    }

    Nh_Web_ParseResult Partial = Nh_Web_parsePartial(Parser_p);

    if (Partial.Node_p != NULL) 
    {
        Nh_Web_ParseNode *Definition_p = Nh_Web_allocateNonTerminalParseNode("Definition", 1);
        Nh_appendToList(&Definition_p->Children, Partial.Node_p);
        NH_WEB_END(Nh_Web_initParseResult(Definition_p))
    }

    Nh_Web_ParseResult Dictionary = Nh_Web_parseDictionary(Parser_p);

    if (Dictionary.Node_p != NULL) 
    {
        Nh_Web_ParseNode *Definition_p = Nh_Web_allocateNonTerminalParseNode("Definition", 1);
        Nh_appendToList(&Definition_p->Children, Dictionary.Node_p);
        NH_WEB_END(Nh_Web_initParseResult(Definition_p))
    }

    Nh_Web_ParseResult Enum = Nh_Web_parseEnum(Parser_p);

    if (Enum.Node_p != NULL) 
    {
        Nh_Web_ParseNode *Definition_p = Nh_Web_allocateNonTerminalParseNode("Definition", 1);
        Nh_appendToList(&Definition_p->Children, Enum.Node_p);
        NH_WEB_END(Nh_Web_initParseResult(Definition_p))
    }

    Nh_Web_ParseResult Typedef = Nh_Web_parseTypedef(Parser_p);

    if (Typedef.Node_p != NULL) 
    {
        Nh_Web_ParseNode *Definition_p = Nh_Web_allocateNonTerminalParseNode("Definition", 1);
        Nh_appendToList(&Definition_p->Children, Typedef.Node_p);
        NH_WEB_END(Nh_Web_initParseResult(Definition_p))
    }

    Nh_Web_ParseResult IncludesStatement = Nh_Web_parseIncludesStatement(Parser_p);

    if (IncludesStatement.Node_p != NULL) 
    {
        Nh_Web_ParseNode *Definition_p = Nh_Web_allocateNonTerminalParseNode("Definition", 1);
        Nh_appendToList(&Definition_p->Children, IncludesStatement.Node_p);
        NH_WEB_END(Nh_Web_initParseResult(Definition_p))
    }

NH_WEB_END(Nh_Web_initParseResult(NULL))
}

static Nh_Web_ParseResult Nh_Web_parseDefinitions(
    Nh_Web_Parser *Parser_p)
{
NH_WEB_BEGIN()

    Nh_Web_ParseNode *Definitions_p = Nh_Web_allocateNonTerminalParseNode("Definitions", 3);

    Nh_Web_ParseResult ExtendedAttributeList = Nh_Web_parseExtendedAttributeList(Parser_p);
    Nh_Web_ParseResult Definition = Nh_Web_parseDefinition(Parser_p);

    if (Definition.Node_p != NULL) 
    {
        Nh_appendToList(&Definitions_p->Children, ExtendedAttributeList.Node_p);
        Nh_appendToList(&Definitions_p->Children, Definition.Node_p);
        Nh_Web_ParseResult Definitions = Nh_Web_parseDefinitions(Parser_p);
        Nh_appendToList(&Definitions_p->Children, Definitions.Node_p);
    }
    else {Nh_Web_freeParseNode(ExtendedAttributeList.Node_p);}

NH_WEB_END(Nh_Web_initParseResult(Definitions_p))
}

// PARSE ===========================================================================================

Nh_Web_FragmentParseResult Nh_Web_parse(
    NH_BYTE *logName_p, NH_BYTE *fragment_p)
{
NH_WEB_BEGIN()

    Nh_Array Tokens = Nh_Web_tokenizeFragment(logName_p, fragment_p);

    Nh_Web_Parser Parser;
    Parser.Tokens_p = (Nh_Web_Token*)Tokens.bytes_p;
    Parser.unparsed = Tokens.length;

    Nh_Web_ParseResult Definitions = Nh_Web_parseDefinitions(&Parser);

    Nh_Web_FragmentParseResult Result;
    Result.Root_p = Definitions.Node_p;
    Result.SyntaxErrors = Definitions.SyntaxErrors;
    Result.Tokens = Tokens;

    Nh_Web_logParseTree(logName_p, Result.Root_p);

NH_WEB_END(Result)
}

// UNPARSE =========================================================================================

static void Nh_Web_unparseParseNode(
    Nh_Web_ParseNode *Node_p)
{
NH_WEB_BEGIN()

    for (int i = 0; i < Node_p->Children.size; ++i) {
        Nh_Web_unparseParseNode(Node_p->Children.pp[i]);
    }

    Nh_freeList(&Node_p->Children, NH_FALSE);
    Nh_free(Node_p);

NH_WEB_SILENT_END()
}

void Nh_Web_unparse(
    Nh_Web_FragmentParseResult ParseResult)
{
NH_WEB_BEGIN()

    Nh_Web_freeTokens(ParseResult.Tokens);
    Nh_Web_unparseParseNode(ParseResult.Root_p);

NH_WEB_SILENT_END()
}

// INIT ============================================================================================

Nh_Web_FragmentParseResult Nh_Web_initFragmentParseResult()
{
NH_WEB_BEGIN()

    Nh_Web_FragmentParseResult Result;
    Result.Root_p = NULL;
    Result.SyntaxErrors = Nh_initList(8);
    Result.Tokens = Nh_initArray(sizeof(Nh_Web_Token), 64);

NH_WEB_END(Result)
}

// GET =============================================================================================

NH_WEB_RESULT Nh_Web_getParseNodes(
    Nh_Web_ParseNode *Node_p, NH_WEB_PARSE_NODE type, Nh_List *Nodes_p)
{
NH_WEB_BEGIN()

    if (Node_p->type == type) {
        Nh_appendToList(Nodes_p, Node_p);
    }

    for (int i = 0; i < Node_p->Children.size; ++i) {
        Nh_Web_getParseNodes(Node_p->Children.pp[i], type, Nodes_p);
    }

NH_WEB_DIAGNOSTIC_END(NH_WEB_SUCCESS)
}


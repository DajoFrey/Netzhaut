// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "Parser.h"
#include "Util.h"

#include "Common/Macro.h"
#include NH_WEBIDL_SERIALIZER_FLOW
#include NH_WEBIDL_SERIALIZER_DEFAULT_CHECK

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

// TYPES ===========================================================================================

const char *NH_WEBIDL_SERIALIZER_TYPES_PP[] = {
    "byte", 
    "octet", 
    "short", 
    "unsigned short", 
    "long", 
    "unsigned long", 
    "long long",
    "unsigned long long",
    "float", 
    "unrestricted float", 
    "double",
    "unrestricted double",
    "bigint",
    "boolean",
    "DOMString",
    "ByteString",
    "USVString",
    "Int8Array", 
    "Int16Array", 
    "Int32Array", 
    "Uint8Array", 
    "Uint16Array", 
    "Uint32Array", 
    "Uint8ClampedArray", 
    "Float32Array",
    "Float64Array",
    "ArrayBuffer",
    "DataView",
    "object",
};

// EXTENDED ATTRIBUTES =============================================================================

// TODO make better
static Nh_Array Nh_WebIDLSerializer_parseExtendedAttributes(
    char *member_p)
{
NH_WEBIDL_SERIALIZER_BEGIN()

    Nh_Array ExtendedAttributes = Nh_initArray(sizeof(Nh_WebIDLSerializer_ExtendedAttribute), 1);

    NH_BOOL list = NH_FALSE;
    while (member_p[0] == '[' || list) 
    {
        list = NH_FALSE;
        int end;
        for (end = 0; end < strlen(member_p) && member_p[end] != ']' && member_p[end] != ',' ; ++end);
        char replace = member_p[end];
        member_p[end] = '\0';
        char *extAttr_p = *member_p == '[' || *member_p == ',' ? member_p + 1 : member_p;
        
        if (!strcmp(extAttr_p, "AllowShared")) {

        }
        else if (!strcmp(extAttr_p, "Clamp")) {

        }
        else if (!strcmp(extAttr_p, "CrossOriginIsolated")) {

        }
        else if (!strcmp(extAttr_p, "Default")) {

        }
        else if (!strcmp(extAttr_p, "EnforceRange")) {

        }
        else if (!strcmp(extAttr_p, "Exposed")) {

        }
        else if (!strcmp(extAttr_p, "Global")) {

        }
        else if (!strcmp(extAttr_p, "NewObject")) {
            Nh_WebIDLSerializer_ExtendedAttribute *Attribute_p = Nh_getFromArray(&ExtendedAttributes, -1);
            Attribute_p->type = NH_WEBIDL_SERIALIZER_EXTENDED_ATTRIBUTE_NEW_OBJECT;
        }
        else if (!strcmp(extAttr_p, "PutForwards")) {

        }
        else if (!strcmp(extAttr_p, "Replaceable")) {

        }
        else if (!strcmp(extAttr_p, "SameObject")) {

        }
        else if (!strcmp(extAttr_p, "SecureContext")) {

        }
        else if (!strcmp(extAttr_p, "Unscopable")) {

        }

        member_p[end] = replace; 
        member_p = member_p + end + 1;
        while (*member_p == ' ') {member_p = member_p + 1;}
        if (replace == ',') {list = NH_TRUE;}
    }

NH_WEBIDL_SERIALIZER_END(ExtendedAttributes)
}

// INTERFACE =======================================================================================

//static char *Nh_WebIDLSerializer_parseType(
//    char *)
//{
//NH_WEBIDL_SERIALIZER_BEGIN()
//
//NH_WEBIDL_SERIALIZER_END()
//}

static Nh_WebIDLSerializer_Operation *Nh_WebIDLSerializer_parseOperation(
    char *member_p, Nh_WebIDLSerializer_Operation *Operation_p)
{
NH_WEBIDL_SERIALIZER_BEGIN()

    Operation_p->name_p = NULL;
    Operation_p->ExtendedAttributes = Nh_WebIDLSerializer_parseExtendedAttributes(member_p);

    char *name_p = member_p + (strlen(member_p) - 2);
    int depth = 1;
    while (depth > 0) {
        if (*name_p == ')') {depth++;}
        if (*name_p == '(') {depth--;}
        name_p = name_p - 1;
    }

    while (*name_p != ' ' && (name_p != member_p)) {name_p = name_p - 1;}
    Operation_p->name_p = Nh_WebIDLSerializer_allocateWord(*name_p == ' ' ? name_p + 1 : name_p, "(", 1);

NH_WEBIDL_SERIALIZER_DIAGNOSTIC_END(NH_WEBIDL_SERIALIZER_SUCCESS)
}

static NH_WEBIDL_SERIALIZER_RESULT Nh_WebIDLSerializer_parseAttribute(
    char *member_p, Nh_WebIDLSerializer_Attribute *Attribute_p)
{
NH_WEBIDL_SERIALIZER_BEGIN()

    Attribute_p->readonly = NH_FALSE;
    Attribute_p->stringifier = NH_FALSE;
    Attribute_p->_static = NH_FALSE;
    Attribute_p->name_p = NULL;
    
    if (strstr(member_p, "static ")) {
        Attribute_p->_static = NH_TRUE;
    }
    if (strstr(member_p, "readonly")) {
        Attribute_p->readonly = NH_TRUE;
    }
    if (strstr(member_p, "stringifier ")) {
        Attribute_p->stringifier = NH_TRUE;
    }

    member_p = Nh_WebIDLSerializer_nextValid(strstr(member_p, "attribute ") + 9);

    char *name_p = member_p + (strlen(member_p) - 1);
    while (*name_p != ' ') {name_p = name_p - 1;}
    Attribute_p->name_p = Nh_WebIDLSerializer_allocateWord(name_p + 1, "\0", 1);

NH_WEBIDL_SERIALIZER_DIAGNOSTIC_END(NH_WEBIDL_SERIALIZER_SUCCESS)
}

static Nh_WebIDLSerializer_Interface *Nh_WebIDLSerializer_parseInterfaceToken(
    Nh_WebIDLSerializer_InterfaceOrDictionaryToken *Token_p)
{
NH_WEBIDL_SERIALIZER_BEGIN()

    Nh_WebIDLSerializer_Interface *Interface_p = malloc(sizeof(Nh_WebIDLSerializer_Interface));

    Interface_p->name_p = Token_p->name_p;
    Interface_p->inherit_p = Token_p->inherit_p;
    Interface_p->Attributes = Nh_initArray(sizeof(Nh_WebIDLSerializer_Attribute), 4);
    Interface_p->Operations = Nh_initArray(sizeof(Nh_WebIDLSerializer_Operation), 4);

    for (int i = 0; i < Token_p->memberCount; ++i) 
    {
        char *member_p = Token_p->members_pp[i];

        if (strstr(member_p, "attribute ")) {
            Nh_WebIDLSerializer_parseAttribute(member_p, Nh_getFromArray(&Interface_p->Attributes, -1)); 
        }
        else if (strstr(member_p, "const ")) {
            // TODO
        }
        else if (strstr(member_p, "iterable<")) {
            // TODO
        }
        else {
            Nh_WebIDLSerializer_parseOperation(member_p, Nh_getFromArray(&Interface_p->Operations, -1));
        }
    }

NH_WEBIDL_SERIALIZER_END(Interface_p)
}

// PARSE ===========================================================================================

Nh_WebIDLSerializer_Fragment Nh_WebIDLSerializer_parseFragmentToken(
    Nh_WebIDLSerializer_FragmentToken Token)
{
NH_WEBIDL_SERIALIZER_BEGIN()

    Nh_WebIDLSerializer_Fragment Fragment;
    Fragment.definitionCount = Token.definitionCount;
    Fragment.Definitions_p = malloc(sizeof(Nh_WebIDLSerializer_Definition) * Fragment.definitionCount);

    for (int i = 0; i < Fragment.definitionCount; i++) 
    {
        Fragment.Definitions_p[i].data_p = NULL;
        Fragment.Definitions_p[i].type = -1;
 
        Nh_WebIDLSerializer_DefinitionToken *Definition_p = Token.Definitions_pp[i];

        switch (Definition_p->type)
        {
            case NH_WEBIDL_SERIALIZER_DEFINITION_CALLBACK           : break; 
            case NH_WEBIDL_SERIALIZER_DEFINITION_CALLBACK_INTERFACE : break;
            case NH_WEBIDL_SERIALIZER_DEFINITION_INTERFACE          : 
                Fragment.Definitions_p[i].type = NH_WEBIDL_SERIALIZER_DEFINITION_INTERFACE;
                Fragment.Definitions_p[i].data_p = Nh_WebIDLSerializer_parseInterfaceToken(Definition_p->data_p);
                break;
            case NH_WEBIDL_SERIALIZER_DEFINITION_INTERFACE_MIXIN    : break; 
            case NH_WEBIDL_SERIALIZER_DEFINITION_MIXIN              : break;
            case NH_WEBIDL_SERIALIZER_DEFINITION_NAMESPACE          : break;
            case NH_WEBIDL_SERIALIZER_DEFINITION_PARTIAL            : break;
            case NH_WEBIDL_SERIALIZER_DEFINITION_DICTIONARY         : break; 
            case NH_WEBIDL_SERIALIZER_DEFINITION_ENUM               : break;
            case NH_WEBIDL_SERIALIZER_DEFINITION_TYPEDEF            : break;
            case NH_WEBIDL_SERIALIZER_DEFINITION_INCLUDES_STATEMENT : break;
        }    
    }

NH_WEBIDL_SERIALIZER_END(Fragment)
}

// UNPARSE =========================================================================================

void Nh_WebIDLSerializer_unparseFragment(
    Nh_WebIDLSerializer_Fragment Fragment)
{
NH_WEBIDL_SERIALIZER_BEGIN()

    for (int i = 0; i < Fragment.definitionCount; i++) 
    {
        Nh_WebIDLSerializer_Definition Definition = Fragment.Definitions_p[i];
    
        switch (Definition.type)
        {
            case NH_WEBIDL_SERIALIZER_DEFINITION_CALLBACK           : break; 
            case NH_WEBIDL_SERIALIZER_DEFINITION_CALLBACK_INTERFACE : break;
            case NH_WEBIDL_SERIALIZER_DEFINITION_INTERFACE          : 
            {
                Nh_WebIDLSerializer_Interface *Interface_p = Definition.data_p;
                Nh_freeArray(&Interface_p->Attributes);
                Nh_freeArray(&Interface_p->Operations);
                break;
            }
            case NH_WEBIDL_SERIALIZER_DEFINITION_INTERFACE_MIXIN    : break; 
            case NH_WEBIDL_SERIALIZER_DEFINITION_MIXIN              : break;
            case NH_WEBIDL_SERIALIZER_DEFINITION_NAMESPACE          : break;
            case NH_WEBIDL_SERIALIZER_DEFINITION_PARTIAL            : break;
            case NH_WEBIDL_SERIALIZER_DEFINITION_DICTIONARY         : break; 
            case NH_WEBIDL_SERIALIZER_DEFINITION_ENUM               : break;
            case NH_WEBIDL_SERIALIZER_DEFINITION_TYPEDEF            : break;
            case NH_WEBIDL_SERIALIZER_DEFINITION_INCLUDES_STATEMENT : break;
        }    

        free(Definition.data_p);
    }

    free(Fragment.Definitions_p);

NH_WEBIDL_SERIALIZER_SILENT_END()
}

// STRINGIFY =======================================================================================

static void Nh_WebIDLSerializer_copyChars(
    char *from_p, char *to_p, int *length_p)
{
NH_WEBIDL_SERIALIZER_BEGIN()

    if (to_p != NULL) {
        int i = 0;
        for (int i = 0; strlen(from_p) > i; ++i) {to_p[(*length_p)++] = from_p[i];}
    }
    else {*length_p += strlen(from_p);}

NH_WEBIDL_SERIALIZER_SILENT_END()
}

void Nh_WebIDLSerializer_stringifyFragment(
    Nh_WebIDLSerializer_Fragment Fragment, char *str_p, int *length_p)
{
NH_WEBIDL_SERIALIZER_BEGIN()

    int length = 0;
    for (int i = 0; i < Fragment.definitionCount; ++i) 
    {
        Nh_WebIDLSerializer_Definition *Definition_p = &Fragment.Definitions_p[i];
        if (i > 0) {Nh_WebIDLSerializer_copyChars("\n", str_p, &length);}
        Nh_WebIDLSerializer_copyChars("DEFINITION\n", str_p, &length);

//        if (Definition_p->extendedAttributeList_p != NULL) {
//            Nh_WebIDLSerializer_copyChars("  EXTENDED ATTRIBUTE LIST\n    ", str_p, &length);
//            Nh_WebIDLSerializer_copyChars(Definition_p->extendedAttributeList_p, str_p, &length);
//            Nh_WebIDLSerializer_copyChars("\n", str_p, &length);
//        }

        switch (Definition_p->type)
        {
            case NH_WEBIDL_SERIALIZER_DEFINITION_INTERFACE  :
                Nh_WebIDLSerializer_copyChars("  INTERFACE\n", str_p, &length); break;
            case NH_WEBIDL_SERIALIZER_DEFINITION_CALLBACK  :
                Nh_WebIDLSerializer_copyChars("  CALLBACK\n", str_p, &length); break;
            case NH_WEBIDL_SERIALIZER_DEFINITION_INTERFACE_MIXIN  :
                Nh_WebIDLSerializer_copyChars("  INTERFACE MIXIN\n", str_p, &length); break;
            case NH_WEBIDL_SERIALIZER_DEFINITION_CALLBACK_INTERFACE  :
                Nh_WebIDLSerializer_copyChars("  CALLBACK INTERFACE\n", str_p, &length); break;
            case NH_WEBIDL_SERIALIZER_DEFINITION_DICTIONARY :
                Nh_WebIDLSerializer_copyChars("  DICTIONARY\n", str_p, &length); break;
            case NH_WEBIDL_SERIALIZER_DEFINITION_INCLUDES_STATEMENT :
                Nh_WebIDLSerializer_copyChars("  INCLUDES STATEMENT\n", str_p, &length); break;
            case NH_WEBIDL_SERIALIZER_DEFINITION_ENUM :
                Nh_WebIDLSerializer_copyChars("  ENUM\n", str_p, &length); break;
        }

//        switch (Definition_p->type)
//        {
//            case NH_WEBIDL_SERIALIZER_DEFINITION_INTERFACE  :
//            case NH_WEBIDL_SERIALIZER_DEFINITION_DICTIONARY :
//            {
//                Nh_WebIDLSerializer_InterfaceOrDictionaryToken *InterfaceOrDictionary_p = Definition_p->data_p;
//                if (InterfaceOrDictionary_p->name_p != NULL) {
//                    Nh_WebIDLSerializer_copyChars("    NAME ", str_p, &length);
//                    Nh_WebIDLSerializer_copyChars(InterfaceOrDictionary_p->name_p, str_p, &length);
//                }
//                if (InterfaceOrDictionary_p->inherit_p != NULL) {
//                    Nh_WebIDLSerializer_copyChars("\n", str_p, &length);
//                    Nh_WebIDLSerializer_copyChars("    INHERIT ", str_p, &length);
//                    Nh_WebIDLSerializer_copyChars(InterfaceOrDictionary_p->inherit_p, str_p, &length);
//                }
//                for (int j = 0; j < InterfaceOrDictionary_p->memberCount; ++j) {
//                    Nh_WebIDLSerializer_copyChars("\n", str_p, &length);
//                    Nh_WebIDLSerializer_copyChars("    MEMBER ", str_p, &length);
//                    Nh_WebIDLSerializer_copyChars(InterfaceOrDictionary_p->members_pp[j], str_p, &length);
//                }
//                break;
//            }
//            case NH_WEBIDL_SERIALIZER_DEFINITION_INTERFACE_MIXIN    :
//            case NH_WEBIDL_SERIALIZER_DEFINITION_CALLBACK_INTERFACE :
//            {
//                Nh_WebIDLSerializer_InterfaceMixinOrCallbackInterfaceToken *InterfaceMixin_p = Definition_p->data_p;
//                if (InterfaceMixin_p->name_p != NULL) {
//                    Nh_WebIDLSerializer_copyChars("    NAME ", str_p, &length);
//                    Nh_WebIDLSerializer_copyChars(InterfaceMixin_p->name_p, str_p, &length);
//                }
//                for (int j = 0; j < InterfaceMixin_p->memberCount; ++j) {
//                    Nh_WebIDLSerializer_copyChars("\n", str_p, &length);
//                    Nh_WebIDLSerializer_copyChars("    MEMBER ", str_p, &length);
//                    Nh_WebIDLSerializer_copyChars(InterfaceMixin_p->members_pp[j], str_p, &length);
//                }
//                break;
//            }
//            case NH_WEBIDL_SERIALIZER_DEFINITION_ENUM:
//            {
//                Nh_WebIDLSerializer_EnumToken *InterfaceMixin_p = Definition_p->data_p;
//                if (InterfaceMixin_p->name_p != NULL) {
//                    Nh_WebIDLSerializer_copyChars("    NAME ", str_p, &length);
//                    Nh_WebIDLSerializer_copyChars(InterfaceMixin_p->name_p, str_p, &length);
//                }
//                for (int j = 0; j < InterfaceMixin_p->valueCount; ++j) {
//                    Nh_WebIDLSerializer_copyChars("\n", str_p, &length);
//                    Nh_WebIDLSerializer_copyChars("    MEMBER ", str_p, &length);
//                    Nh_WebIDLSerializer_copyChars(InterfaceMixin_p->values_pp[j], str_p, &length);
//                }
//                break;
//            }
//            case NH_WEBIDL_SERIALIZER_DEFINITION_INCLUDES_STATEMENT:
//            {
//                Nh_WebIDLSerializer_IncludesStatementToken *IncludesStatement_p = Definition_p->data_p;
//                Nh_WebIDLSerializer_copyChars("    LEFT ", str_p, &length);
//                Nh_WebIDLSerializer_copyChars(IncludesStatement_p->left_p, str_p, &length);
//                Nh_WebIDLSerializer_copyChars("\n", str_p, &length);
//                Nh_WebIDLSerializer_copyChars("    RIGHT ", str_p, &length);
//                Nh_WebIDLSerializer_copyChars(IncludesStatement_p->right_p, str_p, &length);
//                break;
//            }
//            case NH_WEBIDL_SERIALIZER_DEFINITION_CALLBACK:
//            {
//                Nh_WebIDLSerializer_CallbackToken *InterfaceMixin_p = Definition_p->data_p;
//                if (InterfaceMixin_p->name_p != NULL) {
//                    Nh_WebIDLSerializer_copyChars("    NAME ", str_p, &length);
//                    Nh_WebIDLSerializer_copyChars(InterfaceMixin_p->name_p, str_p, &length);
//                }
//                if (InterfaceMixin_p->type_p != NULL) {
//                    Nh_WebIDLSerializer_copyChars("\n", str_p, &length);
//                    Nh_WebIDLSerializer_copyChars("    TYPE ", str_p, &length);
//                    Nh_WebIDLSerializer_copyChars(InterfaceMixin_p->type_p, str_p, &length);
//                }
//                for (int j = 0; j < InterfaceMixin_p->argumentCount; ++j) {
//                    Nh_WebIDLSerializer_copyChars("\n", str_p, &length);
//                    Nh_WebIDLSerializer_copyChars("    ARGUMENT ", str_p, &length);
//                    Nh_WebIDLSerializer_copyChars(InterfaceMixin_p->arguments_pp[j], str_p, &length);
//                }
//                break;
//            }
//        }
    }

    Nh_WebIDLSerializer_copyChars("\0", str_p, &length);
    if (length_p != NULL) {*length_p = length;}

NH_WEBIDL_SERIALIZER_SILENT_END()
}


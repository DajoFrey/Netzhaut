// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "Tokenizer.h"
#include "Util.h"

#include "Common/API.h"
#include "Common/Macro.h"
#include NH_WEBIDL_SERIALIZER_FLOW
#include NH_WEBIDL_SERIALIZER_DEFAULT_CHECK

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

// HELPER ==========================================================================================

static NH_BOOL Nh_WebIDLSerializer_isWord(
    NH_BYTE *current_p, const NH_BYTE *word_p)
{
NH_WEBIDL_SERIALIZER_BEGIN()

    for (int i = 0; i < strlen(word_p); ++i) {
        if (current_p[i] != word_p[i]) {NH_WEBIDL_SERIALIZER_END(NH_FALSE)}
    }

NH_WEBIDL_SERIALIZER_END(NH_TRUE)
}

// EXTENDED ATTRIBUTES LIST ========================================================================

static char *Nh_WebIDLSerializer_tokenizeExtendedAttributesList(
    char *current_p, Nh_WebIDLSerializer_DefinitionToken *Definition_p)
{
NH_WEBIDL_SERIALIZER_BEGIN()

    current_p = Nh_WebIDLSerializer_nextValid(current_p);

    if (current_p == NULL || *current_p != '[') {NH_WEBIDL_SERIALIZER_END(current_p)}

    int depth = 1, length = 1;
    char *tmp_p = current_p;

    while (depth > 0) 
    {
        tmp_p = current_p + length;    
        if (*tmp_p == ']') {depth--;}
        if (*tmp_p == '[') {depth++;}
        length++;
    }

    char replace = current_p[length];
    current_p[length] = '\0';    

    if (length > 0) {
        Definition_p->extendedAttributeList_p = malloc(sizeof(char) * (length + 1));
        strcpy(Definition_p->extendedAttributeList_p, current_p);
    }

    current_p[length] = replace;

NH_WEBIDL_SERIALIZER_END(current_p + length)
}

// MEMBERS =========================================================================================

static char *Nh_WebIDLSerializer_tokenizeMembers(
    char *current_p, char ***members_ppp, int *memberCount_p)
{
NH_WEBIDL_SERIALIZER_BEGIN()

    if (*current_p != '{') {NH_WEBIDL_SERIALIZER_END(NULL)}

    char *members_pp[1024] = {NULL};
    current_p = Nh_WebIDLSerializer_nextValid(current_p + 1);

    int i = 0;
    for (i = 0; i < 1024 && current_p != NULL && *current_p != '}'; ++i) 
    {
        members_pp[i] = Nh_WebIDLSerializer_allocateWord(current_p, ";", 2);
        current_p = Nh_WebIDLSerializer_nextValid(current_p + strlen(members_pp[i]) + 1);
    }

    *members_ppp = malloc(sizeof(char*) * i);
    *memberCount_p = i;
    for (int j = 0; j < i; ++j) {(*members_ppp)[j] = members_pp[j];}

    if (*current_p  != '}') {NH_WEBIDL_SERIALIZER_END(NULL)}

NH_WEBIDL_SERIALIZER_END(Nh_WebIDLSerializer_nextValid(current_p + 1) + 1)
}

// DEFINTION =======================================================================================

static char *Nh_WebIDLSerializer_tokenizeCallback(
    char *current_p, Nh_WebIDLSerializer_DefinitionToken *Definition_p)
{
NH_WEBIDL_SERIALIZER_BEGIN()

    Nh_WebIDLSerializer_CallbackToken *Interface_p = 
        malloc(sizeof(Nh_WebIDLSerializer_CallbackToken));

    Interface_p->name_p = NULL;
    Interface_p->type_p = NULL;
    Interface_p->argumentCount = 0;
    Interface_p->arguments_pp = NULL;

    current_p = Nh_WebIDLSerializer_nextValid(current_p);
    Interface_p->name_p = Nh_WebIDLSerializer_allocateWord(current_p, " =", 2);
    current_p = Nh_WebIDLSerializer_nextValid(current_p + strlen(Interface_p->name_p));

    while (*current_p != '=') {current_p = current_p + 1;}
    current_p = Nh_WebIDLSerializer_nextValid(current_p + 1);
    Interface_p->type_p = Nh_WebIDLSerializer_allocateWord(current_p, " (", 2);
 
    while (*current_p != '(') {current_p = current_p + 1;}
    current_p = Nh_WebIDLSerializer_nextValid(current_p + 1);

    char *arguments_pp[1024] = {NULL};
    int count = 0;

    while (current_p != NULL && *current_p != ')' && *current_p != ';') {
        arguments_pp[count] = Nh_WebIDLSerializer_allocateWord(current_p, ",)", 2);
        current_p = Nh_WebIDLSerializer_nextValid(current_p + strlen(arguments_pp[count++]) + 1);
    }

    if (count > 0) {
        Interface_p->argumentCount = count;
        Interface_p->arguments_pp = malloc(sizeof(char*) * count);
        for (int i = 0; i < count; ++i) {Interface_p->arguments_pp[i] = arguments_pp[i];}
    }

    Definition_p->data_p = Interface_p;
    while (*current_p != ';') {current_p = current_p + 1;}

NH_WEBIDL_SERIALIZER_END(Nh_WebIDLSerializer_nextValid(current_p + 1))
}


static char *Nh_WebIDLSerializer_tokenizeEnum(
    char *current_p, Nh_WebIDLSerializer_DefinitionToken *Definition_p)
{
NH_WEBIDL_SERIALIZER_BEGIN()

    Nh_WebIDLSerializer_EnumToken *Interface_p = 
        malloc(sizeof(Nh_WebIDLSerializer_EnumToken));

    Interface_p->name_p = NULL;
    Interface_p->valueCount = 0;
    Interface_p->values_pp = NULL;

    current_p = Nh_WebIDLSerializer_nextValid(current_p);
    Interface_p->name_p = Nh_WebIDLSerializer_allocateWord(current_p, " ", 1);

    while (*current_p != '{') {current_p = current_p + 1;}
    current_p = Nh_WebIDLSerializer_nextValid(current_p + 1);

    char *values_pp[1024] = {NULL};
    int count = 0;

    while (current_p != NULL && *current_p != '}') {
        if (*current_p == '"') {
            values_pp[count] = Nh_WebIDLSerializer_allocateWord(current_p + 1, "\"", 1);
            current_p = current_p + strlen(values_pp[count++]) + 2;
        }
        else {current_p = current_p + 1;}
    }

    if (count > 0) {
        Interface_p->valueCount = count;
        Interface_p->values_pp = malloc(sizeof(char*) * count);
        for (int i = 0; i < count; ++i) {Interface_p->values_pp[i] = values_pp[i];}
    }

    Definition_p->data_p = Interface_p;

    while (*current_p != ';') {current_p = current_p + 1;}

NH_WEBIDL_SERIALIZER_END(Nh_WebIDLSerializer_nextValid(current_p + 1))
}

static char *Nh_WebIDLSerializer_tokenizeIncludesStatement(
    char *current_p, Nh_WebIDLSerializer_DefinitionToken *Definition_p)
{
NH_WEBIDL_SERIALIZER_BEGIN()

    Nh_WebIDLSerializer_IncludesStatementToken *Interface_p = 
        malloc(sizeof(Nh_WebIDLSerializer_IncludesStatementToken));

    Interface_p->left_p = NULL;
    Interface_p->right_p = NULL;

    current_p = Nh_WebIDLSerializer_nextValid(current_p);
    Interface_p->left_p = Nh_WebIDLSerializer_allocateWord(current_p, " ", 1);
    current_p = Nh_WebIDLSerializer_nextValid(current_p + strlen(Interface_p->left_p) + 9);
    Interface_p->right_p = Nh_WebIDLSerializer_allocateWord(current_p, " ;", 2);
    current_p = Nh_WebIDLSerializer_nextValid(current_p + strlen(Interface_p->right_p));

    Definition_p->data_p = Interface_p;

    while (*current_p != ';') {current_p = current_p + 1;}

NH_WEBIDL_SERIALIZER_END(Nh_WebIDLSerializer_nextValid(current_p + 1))
}

static char *Nh_WebIDLSerializer_tokenizeInterfaceMixinOrCallbackInterface(
    char *current_p, Nh_WebIDLSerializer_DefinitionToken *Definition_p)
{
NH_WEBIDL_SERIALIZER_BEGIN()

    Nh_WebIDLSerializer_InterfaceMixinOrCallbackInterfaceToken *Interface_p = 
        malloc(sizeof(Nh_WebIDLSerializer_InterfaceMixinOrCallbackInterfaceToken));

    Interface_p->name_p = NULL;
    Interface_p->memberCount = 0;
    Interface_p->members_pp = NULL;

    current_p = Nh_WebIDLSerializer_nextValid(current_p);
    Interface_p->name_p = Nh_WebIDLSerializer_allocateWord(current_p, " {", 2);
    current_p = Nh_WebIDLSerializer_nextValid(current_p + strlen(Interface_p->name_p));

    if (*current_p == '{') {
        current_p = Nh_WebIDLSerializer_tokenizeMembers(current_p, &Interface_p->members_pp, &Interface_p->memberCount);
    }

    Definition_p->data_p = Interface_p;

NH_WEBIDL_SERIALIZER_END(current_p)
}

static char *Nh_WebIDLSerializer_tokenizeInterfaceOrDictionary(
    char *current_p, Nh_WebIDLSerializer_DefinitionToken *Definition_p)
{
NH_WEBIDL_SERIALIZER_BEGIN()

    Nh_WebIDLSerializer_InterfaceOrDictionaryToken *Interface_p = 
        malloc(sizeof(Nh_WebIDLSerializer_InterfaceOrDictionaryToken));

    Interface_p->name_p = NULL;
    Interface_p->inherit_p = NULL;
    Interface_p->memberCount = 0;
    Interface_p->members_pp = NULL;

    current_p = Nh_WebIDLSerializer_nextValid(current_p);
    Interface_p->name_p = Nh_WebIDLSerializer_allocateWord(current_p, " {", 2);
    current_p = Nh_WebIDLSerializer_nextValid(current_p + strlen(Interface_p->name_p));

    if (*current_p == ':') {
        current_p = Nh_WebIDLSerializer_nextValid(current_p + 1);
        Interface_p->inherit_p = Nh_WebIDLSerializer_allocateWord(current_p, " {", 2);
        current_p = Nh_WebIDLSerializer_nextValid(current_p + strlen(Interface_p->inherit_p));
    }

    if (*current_p == '{') {
        current_p = Nh_WebIDLSerializer_tokenizeMembers(current_p, &Interface_p->members_pp, &Interface_p->memberCount);
    }

    Definition_p->data_p = Interface_p;

NH_WEBIDL_SERIALIZER_END(current_p)
}

static char *Nh_WebIDLSerializer_tokenizeDefinition(
    char *current_p, Nh_WebIDLSerializer_DefinitionToken *Definition_p)
{
NH_WEBIDL_SERIALIZER_BEGIN()

    if (current_p == NULL) {NH_WEBIDL_SERIALIZER_END(NULL)}

    current_p = Nh_WebIDLSerializer_nextValid(current_p);

    if (Nh_WebIDLSerializer_isWord(current_p, "callback "))
    {
        // CallbackRestOrInterface ::
        //     CallbackRest
        //     interface identifier { CallbackInterfaceMembers } ;
        current_p = Nh_WebIDLSerializer_nextValid(current_p + 8);

        if (Nh_WebIDLSerializer_isWord(current_p, "interface ")) {
            // interface identifier { CallbackInterfaceMembers } ;
            Definition_p->type = NH_WEBIDL_SERIALIZER_DEFINITION_CALLBACK_INTERFACE;
            current_p = Nh_WebIDLSerializer_tokenizeInterfaceMixinOrCallbackInterface(current_p + 9, Definition_p);
            NH_WEBIDL_SERIALIZER_END(current_p)
        }
        else {
            // CallbackRest ::
            //     identifier = Type ( ArgumentList ) ;
            Definition_p->type = NH_WEBIDL_SERIALIZER_DEFINITION_CALLBACK;
            current_p = Nh_WebIDLSerializer_tokenizeCallback(current_p, Definition_p);
            NH_WEBIDL_SERIALIZER_END(current_p)
        }
    }
 
    if (Nh_WebIDLSerializer_isWord(current_p, "interface ")) 
    {
        // InterfaceOrMixin ::
        //     InterfaceRest
        //     MixinRest
        current_p = Nh_WebIDLSerializer_nextValid(current_p + 9);

        if (Nh_WebIDLSerializer_isWord(current_p, "mixin ")) {
            // MixinRest ::
            //     mixin identifier { MixinMembers } ;
            Definition_p->type = NH_WEBIDL_SERIALIZER_DEFINITION_INTERFACE_MIXIN;
            current_p = Nh_WebIDLSerializer_tokenizeInterfaceMixinOrCallbackInterface(current_p + 5, Definition_p);
            NH_WEBIDL_SERIALIZER_END(current_p)
        }
        else {
            // InterfaceRest ::
            //     identifier Inheritance { InterfaceMembers } ;
            Definition_p->type = NH_WEBIDL_SERIALIZER_DEFINITION_INTERFACE;
            current_p = Nh_WebIDLSerializer_tokenizeInterfaceOrDictionary(current_p, Definition_p);
            NH_WEBIDL_SERIALIZER_END(current_p)
        } 
    }

    if (Nh_WebIDLSerializer_isWord(current_p, "namespace "))
    {
        // Namespace ::
        //     namespace identifier { NamespaceMembers } ;
        exit(0);
    }

    if (Nh_WebIDLSerializer_isWord(current_p, "partial "))
    {
        // Partial ::
        //     partial PartialDefinition
        current_p = Nh_WebIDLSerializer_nextValid(current_p + 8);

        // InterfaceRest ::
        //     identifier Inheritance { InterfaceMembers } ;
        Definition_p->type = NH_WEBIDL_SERIALIZER_DEFINITION_INTERFACE;
        current_p = Nh_WebIDLSerializer_tokenizeInterfaceOrDictionary(current_p, Definition_p);
        NH_WEBIDL_SERIALIZER_END(current_p)

        exit(0);
    }

    if (Nh_WebIDLSerializer_isWord(current_p, "dictionary "))
    {
        // Dictionary ::
        //     dictionary identifier Inheritance { DictionaryMembers } ;
        Definition_p->type = NH_WEBIDL_SERIALIZER_DEFINITION_DICTIONARY;
        current_p = Nh_WebIDLSerializer_tokenizeInterfaceOrDictionary(current_p + 11, Definition_p);
        NH_WEBIDL_SERIALIZER_END(current_p)
    }

    if (Nh_WebIDLSerializer_isWord(current_p, "enum "))
    {
        // Enum ::
        //     enum identifier { EnumValueList } ;
        Definition_p->type = NH_WEBIDL_SERIALIZER_DEFINITION_ENUM;
        current_p = Nh_WebIDLSerializer_tokenizeEnum(current_p + 4, Definition_p);
        NH_WEBIDL_SERIALIZER_END(current_p)
    }

    if (Nh_WebIDLSerializer_isWord(current_p, "typedef "))
    {
        // Typedef ::
        //     typedef TypeWithExtendedAttributes identifier ;
        exit(0);
    }

    for (int i = 0; i < strlen(current_p); ++i) 
    {
        int length = Nh_WebIDLSerializer_getWordLength(current_p, " ", 1);
        if (Nh_WebIDLSerializer_isWord(Nh_WebIDLSerializer_nextValid(current_p + length), "includes "))
        {
            // IncludesStatement ::
            //     identifier includes identifier ;
            Definition_p->type = NH_WEBIDL_SERIALIZER_DEFINITION_INCLUDES_STATEMENT;
            current_p = Nh_WebIDLSerializer_tokenizeIncludesStatement(current_p, Definition_p);
            NH_WEBIDL_SERIALIZER_END(current_p)
        }         
    } 

NH_WEBIDL_SERIALIZER_END(NULL)
}

// TOKENIZE ========================================================================================

static void Nh_WebIDLSerializer_tokenizeDefinitions(
    char *current_p, Nh_WebIDLSerializer_FragmentToken *Fragment_p)
{
NH_WEBIDL_SERIALIZER_BEGIN()

    Nh_WebIDLSerializer_DefinitionToken *Definitions_pp[1024] = {NULL};

    int i = 0;
    for (i = 0; i < 1024 && current_p != NULL && *current_p != '\0';)
    {
        Nh_WebIDLSerializer_DefinitionToken *Definition_p = malloc(sizeof(Nh_WebIDLSerializer_DefinitionToken));

        Definition_p->extendedAttributeList_p = NULL;
        Definition_p->data_p = NULL;

        current_p = Nh_WebIDLSerializer_tokenizeExtendedAttributesList(current_p, Definition_p);
        current_p = Nh_WebIDLSerializer_tokenizeDefinition(current_p, Definition_p);

        if (Definition_p->data_p != NULL) {
            Definitions_pp[i++] = Definition_p;
        } 
        else {free(Definition_p);}
    }

    if (i > 0) {
        Fragment_p->Definitions_pp = malloc(sizeof(Nh_WebIDLSerializer_DefinitionToken*) * i);
        Fragment_p->definitionCount = i;
        for (int j = 0; j < i; ++j) {Fragment_p->Definitions_pp[j] = Definitions_pp[j];}
    }

NH_WEBIDL_SERIALIZER_SILENT_END()
}

Nh_WebIDLSerializer_FragmentToken Nh_WebIDLSerializer_tokenizeFile(
    const char *filepath_p)
{
NH_WEBIDL_SERIALIZER_BEGIN()

    Nh_WebIDLSerializer_FragmentToken Fragment;

    Fragment.definitionCount = 0;
    Fragment.Definitions_pp = NULL;

    char *current_p = Nh_WebIDLSerializer_getFileData(filepath_p, NULL);
    if (current_p == NULL) {NH_WEBIDL_SERIALIZER_END(Fragment)}

    Nh_WebIDLSerializer_tokenizeDefinitions(current_p, &Fragment);
    free(current_p);

NH_WEBIDL_SERIALIZER_END(Fragment)
}

// UNTOKENIZE ======================================================================================

void Nh_WebIDLSerializer_untokenize(
    Nh_WebIDLSerializer_FragmentToken Token)
{
NH_WEBIDL_SERIALIZER_BEGIN()

    for (int i = 0; i < Token.definitionCount; ++i) 
    {
        Nh_WebIDLSerializer_DefinitionToken *Definition_p = Token.Definitions_pp[i];

        switch (Definition_p->type)
        {
            case NH_WEBIDL_SERIALIZER_DEFINITION_CALLBACK           : break; 
            case NH_WEBIDL_SERIALIZER_DEFINITION_CALLBACK_INTERFACE : break;
            case NH_WEBIDL_SERIALIZER_DEFINITION_INTERFACE          : 
            case NH_WEBIDL_SERIALIZER_DEFINITION_DICTIONARY         : 
            {
                Nh_WebIDLSerializer_InterfaceOrDictionaryToken *Token_p = Definition_p->data_p; 
                free(Token_p->name_p);
                free(Token_p->inherit_p);
                for (int j = 0; j < Token_p->memberCount; ++j) {
                    free(Token_p->members_pp[j]);
                }
                break;
            }
            case NH_WEBIDL_SERIALIZER_DEFINITION_INTERFACE_MIXIN    : break;
            case NH_WEBIDL_SERIALIZER_DEFINITION_MIXIN              : break;
            case NH_WEBIDL_SERIALIZER_DEFINITION_NAMESPACE          : break;
            case NH_WEBIDL_SERIALIZER_DEFINITION_PARTIAL            : break;
                break;
            case NH_WEBIDL_SERIALIZER_DEFINITION_ENUM               : break;
            case NH_WEBIDL_SERIALIZER_DEFINITION_TYPEDEF            : break;
            case NH_WEBIDL_SERIALIZER_DEFINITION_INCLUDES_STATEMENT : break;
        }

        free(Definition_p->extendedAttributeList_p);
        free(Definition_p->data_p); 
        free(Definition_p);
    }

    free(Token.Definitions_pp);

NH_WEBIDL_SERIALIZER_SILENT_END()
}


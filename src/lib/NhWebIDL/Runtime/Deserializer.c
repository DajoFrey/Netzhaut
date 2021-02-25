// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "Deserializer.h"
#include "FragmentData.h"
#include "HashMap.h"

#include "../Common/Macro.h"
#include NH_WEBIDL_FLOW
#include NH_WEBIDL_DEFAULT_CHECK

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

// DATA ============================================================================================

static NH_BOOL deserialized = NH_FALSE;
static Nh_WebIDL_Fragment Fragments_p[NH_WEBIDL_FRAGMENTS_PP_COUNT];

// DESERIALIZE =====================================================================================

// TODO are name conflicts across specs possible??
static NH_WEBIDL_RESULT Nh_WebIDL_setInterfaceInheritance(
    Nh_WebIDL_Interface *Interface_p)
{
NH_WEBIDL_BEGIN()

    if (strlen(Interface_p->inherit_p) == 0) {NH_WEBIDL_DIAGNOSTIC_END(NH_WEBIDL_SUCCESS)}

    for (int i = 0; i < NH_WEBIDL_FRAGMENTS_PP_COUNT; ++i) {
        for (int j = 0; j < Fragments_p[i].definitionCount; ++j) 
        {
            if (Fragments_p[i].Definitions_p[j].type == NH_WEBIDL_DEFINITION_INTERFACE) 
            {
                Nh_WebIDL_Interface *Inherit_p = Fragments_p[i].Definitions_p[j].data_p;
                if (!strcmp(Inherit_p->name_p, Interface_p->inherit_p)) {
                    Interface_p->inherit_p = Inherit_p; 
                    NH_WEBIDL_DIAGNOSTIC_END(NH_WEBIDL_SUCCESS)
                }
            }
        }
    }

NH_WEBIDL_DIAGNOSTIC_END(NH_WEBIDL_ERROR_BAD_STATE)
}

static Nh_WebIDL_Fragment Nh_WebIDL_deserializeFragment(
    unsigned char *fragment_p)
{
NH_WEBIDL_BEGIN()

    Nh_WebIDL_Fragment Fragment;
    Fragment.definitionCount = *((*int)fragment_p);
    Fragment.Definitions_p = Nh_allocate(sizeof(Nh_WebIDL_Definition)) * Fragment.definitionCount;

    fragment_p += sizeof(int); 
    for (int i = 0; i < Fragment.definitionCount; ++i) 
    {
        Nh_WebIDL_Definition *Definition_p = &Fragment.Definitions_p[i];
        Definition_p->type = *((*NH_WEBIDL_DEFINITION)fragment_p);
        fragment_p += sizeof(NH_WEBIDL_DEFINITION);

        switch (Definition_p->type)
        {
            case NH_WEBIDL_DEFINITION_CALLBACK           : break; 
            case NH_WEBIDL_DEFINITION_CALLBACK_INTERFACE : break;
            case NH_WEBIDL_DEFINITION_INTERFACE          : 
            {
                Definition_p->data_p = Nh_allocate(sizeof(Nh_WebIDL_Interface));
                Nh_WebIDL_Interface *Interface_p = Definition_p->data_p;

                Interface_p->name_p = fragment_p;
                fragment_p += sizeof(char) * (strlen(Interface_p->name_p) + 1);

                Interface_p->inherit_p = fragment_p;
                fragment_p += sizeof(char) * (strlen(Interface_p->inherit_p) + 1);

                Interface_p->attributeCount = *((*int)fragment_p);
                Interface_p->Attributes_p = Nh_allocate(sizeof(Nh_WebIDL_Attribute) * Interface_p->attributeCount);
                fragment_p += sizeof(int);

                for (int j = 0; j < Interface_p->attributeCount; ++j) 
                {
                    Nh_WebIDL_Attribute *Attribute_p = &Interface_p->Attributes_p[j];
                    Attribute_p->_static = *((*NH_BOOL)fragment_p);
                    fragment_p += sizeof(NH_BOOL);
                    Attribute_p->readonly = *((*NH_BOOL)fragment_p);
                    fragment_p += sizeof(NH_BOOL);
                    Attribute_p->stringifier = *((*NH_BOOL)fragment_p);
                    fragment_p += sizeof(NH_BOOL);
                    Attribute_p->name_p = fragment_p;
                    fragment_p += sizeof(char) * (strlen(Attribute_p->name_p) + 1);
                }

                Interface_p->operationCount = *((*int)fragment_p);
                Interface_p->Operations_p = Nh_allocate(sizeof(Nh_WebIDL_Attribute) * Interface_p->attributeCount);
                fragment_p += sizeof(int);
               
                for (int j = 0; j < Interface_p->operationCount; ++j)
                {
                    Nh_WebIDL_Operation *Operation_p = &Interface_p->Operations_p[j];
                    Operation_p->name_p = fragment_p;
                    fragment_p += sizeof(char) * (strlen(Operation_p->name_p) + 1);
                }

                break;
            }
            case NH_WEBIDL_DEFINITION_INTERFACE_MIXIN    : break; 
            case NH_WEBIDL_DEFINITION_MIXIN              : break;
            case NH_WEBIDL_DEFINITION_NAMESPACE          : break;
            case NH_WEBIDL_DEFINITION_PARTIAL            : break;
            case NH_WEBIDL_DEFINITION_DICTIONARY         : break; 
            case NH_WEBIDL_DEFINITION_ENUM               : break;
            case NH_WEBIDL_DEFINITION_TYPEDEF            : break;
            case NH_WEBIDL_DEFINITION_INCLUDES_STATEMENT : break;
        }
    }

NH_WEBIDL_END(Fragment)
}

NH_WEBIDL_RESULT Nh_WebIDL_deserializeFragments()
{
NH_WEBIDL_BEGIN()

    if (deserialized) {NH_WEBIDL_DIAGNOSTIC_END(NH_WEBIDL_ERROR_BAD_STATE)}

    for (int i = 0; i < NH_WEBIDL_FRAGMENTS_PP_COUNT; ++i) {
        Fragments_p[i] = Nh_WebIDL_deserializeFragment(NH_WEBIDL_FRAGMENTS_PP[i]);
    }

    for (int i = 0; i < NH_WEBIDL_FRAGMENTS_PP_COUNT; ++i) {
        for (int j = 0; j < Fragments_p[i].definitionCount; ++j) 
        {
            Nh_WebIDL_Definition *Definition_p = &Fragments_p[i].Definitions_p[i];
            if (Definition_p->type == NH_WEBIDL_DEFINITION_INTERFACE) {
                NH_WEBIDL_CHECK(Nh_WebIDL_setInterfaceInheritance(Definition_p->data_p))
            }
        }
    }

    deserialized = NH_TRUE;

NH_WEBIDL_DIAGNOSTIC_END(NH_WEBIDL_SUCCESS)
}

// INTERFACE =======================================================================================

Nh_WebIDL_Interface *Nh_WebIDL_getInterface(
    char *spec_p, char *fragment_p)
{
NH_WEBIDL_BEGIN()

    if (deserialized) {NH_WEBIDL_END(NULL)}

    char fragmentName_p[256] = {'\0'};
    sprintf(fragmentName_p, "%s_%s", spec_p, fragment_p);

    unsigned int *index_p = NULL;
    hashmap_get(NH_WEBIDL_INDEXMAP.FragmentNames, fragmentName_p, (void**)(&index_p));

    if (index_p != NULL) 
    {
        Nh_WebIDL_Fragment *Fragment_p = &Fragments_p[*index_p];

        for (int i = 0; i < Fragment_p->definitionCount; ++i) {
            if (Fragment_p->Definitions_p[i].type == NH_WEBIDL_DEFINITION_INTERFACE) {
                Nh_WebIDL_Interface *Interface_p = Fragment_p->Definitions_p[i].data_p;
                if (!strcmp(Interface_p->name_p, fragment_p)) {
                    NH_WEBIDL_END(Interface_p)
                }
            }
        } 
    }

NH_WEBIDL_END(NULL)
}

Nh_WebIDL_Operation_f Nh_WebIDL_getOperation(
    char *spec_p, char *fragment_p, char *operation_p)
{
NH_WEBIDL_BEGIN()

//    if (deserialized) {NH_WEBIDL_END(NULL)}
//
//    char fragmentName_p[128] = {'\0'};
//    sprintf(fragmentName_p, "%s_%s", spec_p, fragment_p);
//
//    Nh_HashValue *HashValue_p = NULL;
//    hashmap_get(NH_WEBIDL_HASHMAP.FragmentNames, fragmentName_p, (void**)(&HashValue_p));
//
//    if (HashValue_p != NULL) 
//    {
//        Nh_WebIDL_Fragment *Fragment_p = &Fragments_p[HashValue_p->number];
//
//        for (int i = 0; i < Fragment_p->definitionCount; ++i) {
//            if (Fragment_p->Definitions_p[i].type == NH_WEBIDL_DEFINITION_INTERFACE) {
//                Nh_WebIDL_Interface *Interface_p = Fragment_p->Definitions_p[i].data_p;
//                if (!strcmp(Interface_p->name_p, fragment_p)) {
//                    NH_WEBIDL_END(Interface_p)
//                }
//            }
//        } 
//    }

NH_WEBIDL_END(NULL)
}


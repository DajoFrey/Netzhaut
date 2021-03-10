// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "Runtime.h"
#include "Definitions.h"
#include "Builtin.h"
#include "Object.h"

#include "../Common/Log.h"
#include "../Common/Macro.h"
#include NH_WEBIDL_FLOW
#include NH_WEBIDL_DEFAULT_CHECK

#include <string.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

// DATA ============================================================================================

Nh_WebIDL_Runtime NH_WEBIDL_RUNTIME;

// INIT ============================================================================================

NH_WEBIDL_RESULT Nh_WebIDL_initRuntime()
{
NH_WEBIDL_BEGIN()

    NH_WEBIDL_RUNTIME.Specifications = Nh_initArray(sizeof(Nh_WebIDL_Specification), 32);

    for (int i = 0; i < NH_WEBIDL_FRAGMENT_NAMES_PP_COUNT; ++i) 
    {
        NH_BYTE specificationName_p[255] = {'\0'};
        strcpy(specificationName_p, NH_WEBIDL_FRAGMENT_NAMES_PP[i]);
        NH_BYTE *fragmentName_p = strstr(specificationName_p, "_");
        if (!fragmentName_p) {NH_WEBIDL_END(NH_WEBIDL_ERROR_BAD_STATE)}
        *fragmentName_p = '\0';
        fragmentName_p = fragmentName_p + 1;
         
        Nh_WebIDL_FragmentParseResult ParseResult = Nh_WebIDL_parse(NH_WEBIDL_FRAGMENT_NAMES_PP[i], NH_WEBIDL_FRAGMENTS_PP[i]);
        Nh_WebIDL_load(specificationName_p, fragmentName_p, ParseResult);
    }

NH_WEBIDL_END(NH_WEBIDL_SUCCESS)
}

// SPECIFICATION ===================================================================================

Nh_WebIDL_Specification *Nh_WebIDL_getSpecification(
    NH_BYTE *specification_p)
{
NH_WEBIDL_BEGIN()

#include NH_WEBIDL_CUSTOM_CHECK

    Nh_WebIDL_Specification *Specification_p = NULL;

    for (int i = 0; i < NH_WEBIDL_RUNTIME.Specifications.length; ++i) {
        Specification_p = &((Nh_WebIDL_Specification*)NH_WEBIDL_RUNTIME.Specifications.bytes_p)[i];
        if (!strcmp(Specification_p->name_p, specification_p)) {break;}
        Specification_p = NULL;
    }

    if (!Specification_p) {
        Specification_p = Nh_incrementArray(&NH_WEBIDL_RUNTIME.Specifications);
        NH_WEBIDL_CHECK_MEM(NULL, Specification_p)
        Specification_p->Fragments = Nh_initArray(sizeof(Nh_WebIDL_Fragment), 64);
        strcpy(Specification_p->name_p, specification_p);
    }

#include NH_WEBIDL_DEFAULT_CHECK

NH_WEBIDL_END(Specification_p)
}

// FRAGMENT ========================================================================================

Nh_WebIDL_Fragment *Nh_WebIDL_getFragment(
    NH_BYTE *specificationName_p, NH_BYTE *fragmentName_p)
{
NH_WEBIDL_BEGIN()

#include NH_WEBIDL_CUSTOM_CHECK

    Nh_WebIDL_Specification *Specification_p = Nh_WebIDL_getSpecification(specificationName_p);

    NH_WEBIDL_CHECK_NULL(NULL, fragmentName_p)
    NH_WEBIDL_CHECK_NULL(NULL, Specification_p)

    Nh_WebIDL_Fragment *Fragment_p = NULL;
    for (int i = 0; i < Specification_p->Fragments.length; ++i) 
    {
        Fragment_p = &((Nh_WebIDL_Fragment*)Specification_p->Fragments.bytes_p)[i];
        if (!strcmp(Fragment_p->name_p, fragmentName_p)) {break;}
        Fragment_p = NULL;
    }

#include NH_WEBIDL_DEFAULT_CHECK

NH_WEBIDL_END(Fragment_p)
}

NH_WEBIDL_RESULT Nh_WebIDL_load(
    NH_BYTE *specification_p, NH_BYTE *fragmentName_p, Nh_WebIDL_FragmentParseResult ParseResult)
{
NH_WEBIDL_BEGIN()

    NH_WEBIDL_CHECK_NULL(specification_p)

    Nh_WebIDL_Specification *Specification_p = Nh_WebIDL_getSpecification(specification_p);
    if (!Specification_p) {NH_WEBIDL_DIAGNOSTIC_END(NH_WEBIDL_ERROR_BAD_STATE)}

    Nh_WebIDL_Fragment *Fragment_p = Nh_incrementArray(&Specification_p->Fragments);
    NH_WEBIDL_CHECK_MEM(Fragment_p)

    Fragment_p->Specification_p = Specification_p;
    Fragment_p->ParseResult = ParseResult;
    NH_WEBIDL_CHECK(Nh_WebIDL_createInterfaces(Fragment_p))
    strcpy(Fragment_p->name_p, fragmentName_p);

    Nh_WebIDL_logFragment(specification_p, Fragment_p);

NH_WEBIDL_DIAGNOSTIC_END(NH_WEBIDL_SUCCESS)
}

// GET =============================================================================================

Nh_WebIDL_Interface *Nh_WebIDL_getInterface(
    NH_BYTE *specification_p, NH_BYTE *interface_p)
{
NH_WEBIDL_BEGIN()

    Nh_WebIDL_Specification *Specification_p = Nh_WebIDL_getSpecification(specification_p);

    if (Specification_p) {
        for (int i = 0; i < Specification_p->Fragments.length; ++i) {
            Nh_WebIDL_Fragment *Fragment_p = &((Nh_WebIDL_Fragment*)Specification_p->Fragments.bytes_p)[i];
            if (Fragment_p) {
                Nh_WebIDL_Interface *Interface_p = Nh_WebIDL_getInterfaceFromFragment(Fragment_p, interface_p);
                if (Interface_p) {NH_WEBIDL_END(Interface_p)}
            }
        }
    }

NH_WEBIDL_END(NULL)
}

Nh_List Nh_WebIDL_getCompositeInterfaces( // TODO naive
    Nh_WebIDL_Interface *PartialInterface_p)
{
NH_WEBIDL_BEGIN()

    Nh_List Interfaces = Nh_initList(8);

    for (int j = 0; j < NH_WEBIDL_RUNTIME.Specifications.length; ++j)
    {
        Nh_WebIDL_Specification *Specification_p = &((Nh_WebIDL_Specification*)NH_WEBIDL_RUNTIME.Specifications.bytes_p)[j];
    
        if (Specification_p) {
            for (int i = 0; i < Specification_p->Fragments.length; ++i) {
                Nh_WebIDL_Fragment *Fragment_p = &((Nh_WebIDL_Fragment*)Specification_p->Fragments.bytes_p)[i];
                if (Fragment_p) {
                    Nh_WebIDL_Interface *Interface_p = Nh_WebIDL_getInterfaceFromFragment(Fragment_p, PartialInterface_p->name_p);
                    if (Interface_p) {Nh_appendToList(&Interfaces, Interface_p);}
                }
            }
        }
    }

NH_WEBIDL_END(Interfaces)
}


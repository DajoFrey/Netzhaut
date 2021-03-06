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
#include NH_WEB_FLOW
#include NH_WEB_DEFAULT_CHECK

#include <string.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

// DATA ============================================================================================

Nh_Web_Runtime NH_WEB_RUNTIME;

// INIT ============================================================================================

NH_WEB_RESULT Nh_Web_initRuntime()
{
NH_WEB_BEGIN()

    NH_WEB_RUNTIME.Specifications = Nh_initArray(sizeof(Nh_Web_Specification), 32);

    for (int i = 0; i < NH_WEB_FRAGMENT_NAMES_PP_COUNT; ++i) 
    {
        NH_BYTE specificationName_p[255] = {'\0'};
        strcpy(specificationName_p, NH_WEB_FRAGMENT_NAMES_PP[i]);
        NH_BYTE *fragmentName_p = strstr(specificationName_p, "_");
        if (!fragmentName_p) {NH_WEB_END(NH_WEB_ERROR_BAD_STATE)}
        *fragmentName_p = '\0';
        fragmentName_p = fragmentName_p + 1;
         
        Nh_Web_FragmentParseResult ParseResult = Nh_Web_parse(NH_WEB_FRAGMENT_NAMES_PP[i], NH_WEB_FRAGMENTS_PP[i]);
        Nh_Web_load(specificationName_p, fragmentName_p, ParseResult);
    }

    Nh_Web_createCompositeObject("HTML", "Document");

NH_WEB_END(NH_WEB_SUCCESS)
}

// SPECIFICATION ===================================================================================

Nh_Web_Specification *Nh_Web_getSpecification(
    NH_BYTE *specification_p)
{
NH_WEB_BEGIN()

#include NH_WEB_CUSTOM_CHECK

    Nh_Web_Specification *Specification_p = NULL;

    for (int i = 0; i < NH_WEB_RUNTIME.Specifications.length; ++i) {
        Specification_p = &((Nh_Web_Specification*)NH_WEB_RUNTIME.Specifications.bytes_p)[i];
        if (!strcmp(Specification_p->name_p, specification_p)) {break;}
        Specification_p = NULL;
    }

    if (!Specification_p) {
        Specification_p = Nh_getFromArray(&NH_WEB_RUNTIME.Specifications, -1);
        NH_WEB_CHECK_MEM(NULL, Specification_p)
        Specification_p->Fragments = Nh_initArray(sizeof(Nh_Web_Fragment), 64);
        strcpy(Specification_p->name_p, specification_p);
    }

#include NH_WEB_DEFAULT_CHECK

NH_WEB_END(Specification_p)
}

// FRAGMENT ========================================================================================

Nh_Web_Fragment *Nh_Web_getFragment(
    NH_BYTE *specificationName_p, NH_BYTE *fragmentName_p)
{
NH_WEB_BEGIN()

#include NH_WEB_CUSTOM_CHECK

    Nh_Web_Specification *Specification_p = Nh_Web_getSpecification(specificationName_p);

    NH_WEB_CHECK_NULL(NULL, fragmentName_p)
    NH_WEB_CHECK_NULL(NULL, Specification_p)

    Nh_Web_Fragment *Fragment_p = NULL;
    for (int i = 0; i < Specification_p->Fragments.length; ++i) 
    {
        Fragment_p = &((Nh_Web_Fragment*)Specification_p->Fragments.bytes_p)[i];
        if (!strcmp(Fragment_p->name_p, fragmentName_p)) {break;}
        Fragment_p = NULL;
    }

#include NH_WEB_DEFAULT_CHECK

NH_WEB_END(Fragment_p)
}

NH_WEB_RESULT Nh_Web_load(
    NH_BYTE *specification_p, NH_BYTE *fragmentName_p, Nh_Web_FragmentParseResult ParseResult)
{
NH_WEB_BEGIN()

    NH_WEB_CHECK_NULL(specification_p)

    Nh_Web_Specification *Specification_p = Nh_Web_getSpecification(specification_p);
    if (!Specification_p) {NH_WEB_DIAGNOSTIC_END(NH_WEB_ERROR_BAD_STATE)}

    Nh_Web_Fragment *Fragment_p = Nh_getFromArray(&Specification_p->Fragments, -1);
    NH_WEB_CHECK_MEM(Fragment_p)

    Fragment_p->Specification_p = Specification_p;
    Fragment_p->ParseResult = ParseResult;
    NH_WEB_CHECK(Nh_Web_createClasses(Fragment_p))
    strcpy(Fragment_p->name_p, fragmentName_p);

    Nh_Web_logFragment(specification_p, Fragment_p);

NH_WEB_DIAGNOSTIC_END(NH_WEB_SUCCESS)
}

// GET =============================================================================================

Nh_Web_Class *Nh_Web_getClass(
    NH_BYTE *specification_p, NH_BYTE *class_p)
{
NH_WEB_BEGIN()

    Nh_Web_Specification *Specification_p = Nh_Web_getSpecification(specification_p);

    if (Specification_p) {
        for (int i = 0; i < Specification_p->Fragments.length; ++i) {
            Nh_Web_Fragment *Fragment_p = &((Nh_Web_Fragment*)Specification_p->Fragments.bytes_p)[i];
            if (Fragment_p) {
                Nh_Web_Class *Class_p = Nh_Web_getClassFromFragment(Fragment_p, class_p);
                if (Class_p) {NH_WEB_END(Class_p)}
            }
        }
    }

NH_WEB_END(NULL)
}

Nh_List Nh_Web_getCompositeClasses( // TODO naive
    Nh_Web_Class *PartialClass_p)
{
NH_WEB_BEGIN()

    Nh_List Classes = Nh_initList(8);

    for (int j = 0; j < NH_WEB_RUNTIME.Specifications.length; ++j)
    {
        Nh_Web_Specification *Specification_p = &((Nh_Web_Specification*)NH_WEB_RUNTIME.Specifications.bytes_p)[j];
    
        if (Specification_p) {
            for (int i = 0; i < Specification_p->Fragments.length; ++i) {
                Nh_Web_Fragment *Fragment_p = &((Nh_Web_Fragment*)Specification_p->Fragments.bytes_p)[i];
                if (Fragment_p) {
                    Nh_Web_Class *Class_p = Nh_Web_getClassFromFragment(Fragment_p, PartialClass_p->name_p);
                    if (Class_p) {Nh_appendToList(&Classes, Class_p);}
                }
            }
        }
    }

NH_WEB_END(Classes)
}


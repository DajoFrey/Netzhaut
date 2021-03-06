#ifndef NH_WEB_RUNTIME_H
#define NH_WEB_RUNTIME_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "Parser.h"

#include "../Common/Result.h"
#include "../Common/API.h"

#include "../../NhCore/Array.h"

#endif

/** @addtogroup NhWebStructs
 *  @{
 */

    typedef struct Nh_Web_Specification {
        NH_BYTE name_p[255];
        Nh_Array Fragments;
    } Nh_Web_Specification;

    typedef struct Nh_Web_Fragment {
        NH_BYTE name_p[255];
        Nh_Web_Specification *Specification_p;
        Nh_Web_FragmentParseResult ParseResult;
        Nh_Array Classes;
    } Nh_Web_Fragment;

    typedef struct Nh_Web_Runtime {
        Nh_Array Specifications;
    } Nh_Web_Runtime;

/** @} */

/** @addtogroup NhWebVars
 *  @{
 */

    extern Nh_Web_Runtime NH_WEB_RUNTIME;

/** @} */

/** @addtogroup NhWebFunctions
 *  @{
 */

    NH_WEB_RESULT Nh_Web_initRuntime(
    );

    NH_WEB_RESULT Nh_Web_load(
        NH_BYTE *specification_p, NH_BYTE *fragmentName_p, Nh_Web_FragmentParseResult ParseResult
    );

    Nh_Web_Specification *Nh_Web_getSpecification(
        NH_BYTE *specification_p
    );

    Nh_Web_Fragment *Nh_Web_getFragment(
        NH_BYTE *specification_p, NH_BYTE *fragment_p
    );

    Nh_Web_Class *Nh_Web_getClass(
        NH_BYTE *specification_p, NH_BYTE *class_p
    );

    Nh_List Nh_Web_getCompositeClasses(
        Nh_Web_Class *PartialClass_p
    );

/** @} */

#endif

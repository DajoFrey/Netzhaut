#ifndef NH_WEBIDL_RUNTIME_H
#define NH_WEBIDL_RUNTIME_H

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

/** @addtogroup NhWebIDLStructs
 *  @{
 */

    typedef struct Nh_WebIDL_Fragment {
        NH_BYTE name_p[255];
        Nh_WebIDL_Specification *Specification_p;
        Nh_WebIDL_FragmentParseResult ParseResult;
        Nh_Array Interfaces;
    } Nh_WebIDL_Fragment;

    typedef struct Nh_WebIDL_Runtime {
        Nh_Array Specifications;
    } Nh_WebIDL_Runtime;

/** @} */

/** @addtogroup NhWebIDLVars
 *  @{
 */

    extern Nh_WebIDL_Runtime NH_WEBIDL_RUNTIME;

/** @} */

/** @addtogroup NhWebIDLFunctions
 *  @{
 */

    NH_WEBIDL_RESULT Nh_WebIDL_initRuntime(
    );

    NH_WEBIDL_RESULT Nh_WebIDL_load(
        NH_BYTE *specification_p, NH_BYTE *fragmentName_p, Nh_WebIDL_FragmentParseResult ParseResult
    );

    Nh_WebIDL_Specification *Nh_WebIDL_getSpecification(
        NH_BYTE *specification_p
    );

    Nh_WebIDL_Fragment *Nh_WebIDL_getFragment(
        NH_BYTE *specification_p, NH_BYTE *fragment_p
    );

    Nh_WebIDL_Interface *Nh_WebIDL_getInterface(
        NH_BYTE *specification_p, NH_BYTE *interface_p
    );

    Nh_List Nh_WebIDL_getCompositeInterfaces(
        Nh_WebIDL_Interface *PartialInterface_p
    );

/** @} */

#endif

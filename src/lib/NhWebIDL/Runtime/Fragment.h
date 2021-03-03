#ifndef NH_WEBIDL_FRAGMENT_H
#define NH_WEBIDL_FRAGMENT_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "Parser.h"
#include "../Common/Result.h"

#endif

/** @addtogroup NhWebIDLStructs
 *  @{
 */

    typedef struct Nh_WebIDL_Fragment {
        NH_BYTE qualifiedName_p[255];
        NH_BOOL builtin;
        Nh_WebIDL_FinalParseResult ParseResult;
    } Nh_WebIDL_Fragment;

/** @} */

/** @addtogroup NhWebIDLFunctions
 *  @{
 */

    NH_WEBIDL_RESULT Nh_WebIDL_createFragment(
        NH_BYTE *spec_p, NH_BYTE *name_p, Nh_WebIDL_Fragment *Fragment_p
    );

/** @} */

#endif

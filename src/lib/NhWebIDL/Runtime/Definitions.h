#ifndef NH_WEBIDL_DEFINITIONS_H
#define NH_WEBIDL_DEFINITIONS_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "Runtime.h"
#include "Parser.h"
#include "../Common/Result.h"

#endif

/** @addtogroup NhWebIDLStructs
 *  @{
 */

    typedef struct Nh_WebIDL_InterfaceMember {
        Nh_WebIDL_ParseNode *Node_p;
    } Nh_WebIDL_InterfaceMember;

/** @} */

/** @addtogroup NhWebIDLFunctions
 *  @{
 */

    NH_WEBIDL_RESULT Nh_WebIDL_createInterfaces(
        Nh_WebIDL_Fragment *Fragment_p
    );

    Nh_WebIDL_Interface *Nh_WebIDL_getInterfaceFromFragment(
        Nh_WebIDL_Fragment *Fragment_p, NH_BYTE *className_p
    );

/** @} */

#endif

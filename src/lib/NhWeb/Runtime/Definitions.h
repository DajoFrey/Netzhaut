#ifndef NH_WEB_DEFINITIONS_H
#define NH_WEB_DEFINITIONS_H

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

/** @addtogroup NhWebStructs
 *  @{
 */

    typedef struct Nh_Web_ClassMember {
        Nh_Web_ParseNode *Node_p;
    } Nh_Web_ClassMember;

    typedef struct Nh_Web_Class {
        NH_BOOL partial;
        NH_BYTE *name_p;
        Nh_Array Members;
        Nh_Web_Fragment *Fragment_p;
        Nh_Web_ParseNode *Inherit_p;
    } Nh_Web_Class;

/** @} */

/** @addtogroup NhWebFunctions
 *  @{
 */

    NH_WEB_RESULT Nh_Web_createClasses(
        Nh_Web_Fragment *Fragment_p
    );

    Nh_Web_Class *Nh_Web_getClassFromFragment(
        Nh_Web_Fragment *Fragment_p, NH_BYTE *className_p
    );

/** @} */

#endif

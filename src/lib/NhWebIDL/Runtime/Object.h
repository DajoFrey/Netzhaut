#ifndef NH_WEBIDL_OBJECT_H
#define NH_WEBIDL_OBJECT_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "../Common/Result.h"

#endif

/** @addtogroup NhWebIDLStructs
 *  @{
 */

    typedef struct Nh_WebIDL_Object {
//        Nh_WebIDL_Interface *Interface_p;
        Nh_WebIDL_Object *Inherit_p;
        Nh_List Attributes;
    } Nh_WebIDL_Object;

/** @} */

/** @addtogroup NhWebIDLFunctions
 *  @{
 */

    Nh_WebIDL_Object *Nh_WebIDL_createObject(
        NH_BYTE *spec_p, NH_BYTE *interface_p, ...
    );

//    Nh_WebIDL_Any Nh_WebIDL_callOperation(
//        Nh_WebIDL_Object *Object_p, NH_BYTE *name_p, ...
//    );
//
//    Nh_WebIDL_Any Nh_WebIDL_forceCallOperation(
//        Nh_WebIDL_Object *Object_p, NH_BYTE *name_p, ...
//    );
//
//    Nh_WebIDL_Any Nh_WebIDL_getAttribute(
//        Nh_WebIDL_Object *Object_p, NH_BYTE *name_p 
//    );

/** @} */

#endif

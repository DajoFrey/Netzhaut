#ifndef NH_WEBIDL_OBJECT_H
#define NH_WEBIDL_OBJECT_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "../Common/Result.h"
#include "../Common/API.h"

#endif

/** @addtogroup NhWebIDLFunctions
 *  @{
 */

    Nh_WebIDL_Object *Nh_WebIDL_createObject(
        NH_BYTE *specification_p, NH_BYTE *interface_p
    );

    Nh_WebIDL_Object *Nh_WebIDL_createObjectFromInterface(
       Nh_WebIDL_Interface *Interface_p 
    );

    Nh_WebIDL_Object *Nh_WebIDL_createCompositeObject(
        NH_BYTE *specification_p, NH_BYTE *interface_p, ...
    );

    Nh_WebIDL_Object *Nh_WebIDL_getPart(
        Nh_WebIDL_Object *Object_p, NH_BYTE *specification_p, ...
    ); 

    Nh_WebIDL_Object *Nh_WebIDL_getObject(
        Nh_WebIDL_Object *Object_p, NH_BYTE *specification_p, NH_BYTE *interface_p
    );

    void *Nh_WebIDL_get(
        Nh_WebIDL_Object *Object_p, NH_BYTE *attribute_p
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

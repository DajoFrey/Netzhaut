#ifndef NH_WEB_OBJECT_H
#define NH_WEB_OBJECT_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "../Common/Result.h"
#include "../Common/API.h"

#endif

/** @addtogroup NhWebFunctions
 *  @{
 */

    Nh_Web_Object *Nh_Web_createObject(
        NH_BYTE *specification_p, NH_BYTE *fragment_p, ... 
    );

    Nh_Web_CompositeObject *Nh_Web_createCompositeObject(
        NH_BYTE *specification_p, NH_BYTE *class_p, ...
    );

    Nh_Web_Object *Nh_Web_decompose(
        Nh_Web_CompositeObject *CompositeObject_p, NH_BYTE *specification_p, ... 
    );

//    Nh_Web_Any Nh_Web_callOperation(
//        Nh_Web_Object *Object_p, NH_BYTE *name_p, ...
//    );
//
//    Nh_Web_Any Nh_Web_forceCallOperation(
//        Nh_Web_Object *Object_p, NH_BYTE *name_p, ...
//    );
//
//    Nh_Web_Any Nh_Web_getAttribute(
//        Nh_Web_Object *Object_p, NH_BYTE *name_p 
//    );

/** @} */

#endif

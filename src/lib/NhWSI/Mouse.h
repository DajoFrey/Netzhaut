#ifndef NH_WSI_MOUSE_H
#define NH_WSI_MOUSE_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "Common/Result.h"
#include "Common/API.h"

#include "../NhCore/RingIterator.h"

#endif

/** @addtogroup IOEnums Enums
 *  \ingroup IO
 *  @{
 */

    typedef enum NH_WSI_MOUSE {
        NH_WSI_MOUSE_UNDEFINED,
        NH_WSI_MOUSE_LEFT,      
        NH_WSI_MOUSE_RIGHT,    
        NH_WSI_MOUSE_MIDDLE,    
        NH_WSI_MOUSE_SCROLL,    
        NH_WSI_MOUSE_MOVE,      
    } NH_WSI_MOUSE;

    typedef enum NH_WSI_MOUSE_CURSOR {
        NH_WSI_MOUSE_CURSOR_DEFAULT,
        NH_WSI_MOUSE_CURSOR_TEXT,
    } NH_WSI_MOUSE_CURSOR;

    typedef enum NH_WSI_MOUSE_FLAG {
        NH_WSI_MOUSE_FLAG_UNDEFINED = 1 << 0,
        NH_WSI_MOUSE_FLAG_LEFT      = 1 << 1,      
        NH_WSI_MOUSE_FLAG_RIGHT     = 1 << 2,    
        NH_WSI_MOUSE_FLAG_MIDDLE    = 1 << 3,    
        NH_WSI_MOUSE_FLAG_SCROLL    = 1 << 4,    
        NH_WSI_MOUSE_FLAG_MOVE      = 1 << 5,      
    } NH_WSI_MOUSE_FLAG;

/** @} */

/** @addtogroup IOStructs Structs
 *  \ingroup IO
 *  @{
 */  

    typedef struct Nh_WSI_MouseEvent {
        NH_WSI_MOUSE type; 
        int trigger;     
        Nh_PixelPosition Position;
    } Nh_WSI_MouseEvent;

    typedef struct Nh_WSI_Mouse {
        Nh_RingIteratorMark Mark;
        Nh_RingIterator Events;
        NH_WSI_MOUSE_FLAG state;
    } Nh_WSI_Mouse;

/** @} */

/** @addtogroup IOFunctions Functions
 *  \ingroup IO
 *  @{
 */

    NH_WSI_RESULT Nh_WSI_initMouse(
        Nh_WSI_Mouse *Mouse_p, int bufferSize
    );

    char *Nh_WSI_dumpMouse(
        Nh_WSI_MouseEvent *Event_p
    );

    NH_WSI_RESULT Nh_WSI_stringifyMouseState(
        NH_WSI_MOUSE_FLAG state, char *string_p
    );

/** @} */

#endif 

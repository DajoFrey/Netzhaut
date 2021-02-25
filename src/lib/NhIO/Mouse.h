#ifndef NH_IO_MOUSE_H
#define NH_IO_MOUSE_H

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

    typedef enum NH_IO_MOUSE {
        NH_IO_MOUSE_UNDEFINED,
        NH_IO_MOUSE_LEFT,      
        NH_IO_MOUSE_RIGHT,    
        NH_IO_MOUSE_MIDDLE,    
        NH_IO_MOUSE_SCROLL,    
        NH_IO_MOUSE_MOVE,      
    } NH_IO_MOUSE;

    typedef enum NH_IO_MOUSE_CURSOR {
        NH_IO_MOUSE_CURSOR_DEFAULT,
        NH_IO_MOUSE_CURSOR_TEXT,
    } NH_IO_MOUSE_CURSOR;

    typedef enum NH_IO_MOUSE_FLAG {
        NH_IO_MOUSE_FLAG_UNDEFINED = 1 << 0,
        NH_IO_MOUSE_FLAG_LEFT      = 1 << 1,      
        NH_IO_MOUSE_FLAG_RIGHT     = 1 << 2,    
        NH_IO_MOUSE_FLAG_MIDDLE    = 1 << 3,    
        NH_IO_MOUSE_FLAG_SCROLL    = 1 << 4,    
        NH_IO_MOUSE_FLAG_MOVE      = 1 << 5,      
    } NH_IO_MOUSE_FLAG;

/** @} */

/** @addtogroup IOStructs Structs
 *  \ingroup IO
 *  @{
 */  

    typedef struct Nh_IO_MouseEvent {
        NH_IO_MOUSE type; 
        int trigger;     
        Nh_PixelPosition Position;
    } Nh_IO_MouseEvent;

    typedef struct Nh_IO_Mouse {
        Nh_RingIteratorMark Mark;
        Nh_RingIterator Events;
        NH_IO_MOUSE_FLAG state;
    } Nh_IO_Mouse;

/** @} */

/** @addtogroup IOFunctions Functions
 *  \ingroup IO
 *  @{
 */

    NH_IO_RESULT Nh_IO_initMouse(
        Nh_IO_Mouse *Mouse_p, int bufferSize
    );

    char *Nh_IO_dumpMouse(
        Nh_IO_MouseEvent *Event_p
    );

    NH_IO_RESULT Nh_IO_stringifyMouseState(
        NH_IO_MOUSE_FLAG state, char *string_p
    );

/** @} */

#endif 

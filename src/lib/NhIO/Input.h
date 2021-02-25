#ifndef NH_IO_INPUT_H
#define NH_IO_INPUT_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "Mouse.h"
#include "Keyboard.h"

#include "Common/Result.h"
#include "Common/API.h"

#include "../NhCore/RingIterator.h"

#endif

/** @addtogroup IOEnums Enums
 *  \ingroup IO
 *  @{
 */

    typedef enum NH_IO_TRIGGER {
        NH_IO_TRIGGER_UNDEFINED,  
        NH_IO_TRIGGER_PRESS,      
        NH_IO_TRIGGER_RELEASE,    
        NH_IO_TRIGGER_UP,         
        NH_IO_TRIGGER_DOWN,       
        NH_IO_TRIGGER_MOVE,       
        NH_IO_TRIGGER_GET_FOCUS,  
        NH_IO_TRIGGER_LOOSE_FOCUS, 
    } NH_IO_TRIGGER;
   
/** @} */

/** @addtogroup IOStructs Structs
 *  \ingroup IO
 *  @{
 */

    typedef struct Nh_IO_ConfigureEvent {
        Nh_PixelPosition Position;
        Nh_PixelSize Size;
    } Nh_IO_ConfigureEvent;

    typedef struct Nh_IO_Configure {
        Nh_RingIterator Events;
        Nh_RingIteratorMark Mark;
    } Nh_IO_Configure;

    typedef struct Nh_IO_InputMark {
        Nh_RingIteratorMark Configure;
        Nh_RingIteratorMark Mouse;
        Nh_RingIteratorMark Keyboard;
    } Nh_IO_InputMark;

    typedef struct Nh_IO_Input {
        Nh_IO_Configure Configure;
        Nh_IO_Mouse Mouse;
        Nh_IO_Keyboard Keyboard;
    } Nh_IO_Input;

/** @} */

/** @addtogroup IODefines Defines
 *  \ingroup IO
 *  @{
 */

    typedef NH_IO_RESULT (*Nh_IO_initInput_f)(
        Nh_IO_Input *Input_p
    );

/** @} */


/** @addtogroup IOFunctions Functions
 *  \ingroup IO
 *  @{
 */

    NH_IO_RESULT Nh_IO_initInput(
        Nh_IO_Input *Input_p
    );

    Nh_IO_InputMark Nh_IO_initInputMark(
    );

    void Nh_IO_freeInput(
        Nh_IO_Input *Input_p
    );

    const char *Nh_IO_stringifyTrigger(
        NH_IO_TRIGGER trigger
    );

/** @} */

#endif 

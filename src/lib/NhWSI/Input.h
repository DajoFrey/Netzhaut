#ifndef NH_WSI_INPUT_H
#define NH_WSI_INPUT_H

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

    typedef enum NH_WSI_TRIGGER {
        NH_WSI_TRIGGER_UNDEFINED,  
        NH_WSI_TRIGGER_PRESS,      
        NH_WSI_TRIGGER_RELEASE,    
        NH_WSI_TRIGGER_UP,         
        NH_WSI_TRIGGER_DOWN,       
        NH_WSI_TRIGGER_MOVE,       
        NH_WSI_TRIGGER_GET_FOCUS,  
        NH_WSI_TRIGGER_LOOSE_FOCUS, 
    } NH_WSI_TRIGGER;
   
/** @} */

/** @addtogroup IOStructs Structs
 *  \ingroup IO
 *  @{
 */

    typedef struct Nh_WSI_ConfigureEvent {
        Nh_PixelPosition Position;
        Nh_PixelSize Size;
    } Nh_WSI_ConfigureEvent;

    typedef struct Nh_WSI_Configure {
        Nh_RingIterator Events;
        Nh_RingIteratorMark Mark;
    } Nh_WSI_Configure;

    typedef struct Nh_WSI_InputMark {
        Nh_RingIteratorMark Configure;
        Nh_RingIteratorMark Mouse;
        Nh_RingIteratorMark Keyboard;
    } Nh_WSI_InputMark;

    typedef struct Nh_WSI_Input {
        Nh_WSI_Configure Configure;
        Nh_WSI_Mouse Mouse;
        Nh_WSI_Keyboard Keyboard;
    } Nh_WSI_Input;

/** @} */

/** @addtogroup IODefines Defines
 *  \ingroup IO
 *  @{
 */

    typedef NH_WSI_RESULT (*Nh_WSI_initInput_f)(
        Nh_WSI_Input *Input_p
    );

/** @} */


/** @addtogroup IOFunctions Functions
 *  \ingroup IO
 *  @{
 */

    NH_WSI_RESULT Nh_WSI_initInput(
        Nh_WSI_Input *Input_p
    );

    Nh_WSI_InputMark Nh_WSI_initInputMark(
    );

    void Nh_WSI_freeInput(
        Nh_WSI_Input *Input_p
    );

    const char *Nh_WSI_stringifyTrigger(
        NH_WSI_TRIGGER trigger
    );

/** @} */

#endif 

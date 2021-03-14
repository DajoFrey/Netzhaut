#ifndef NH_WSI_KEYBOARD_H
#define NH_WSI_KEYBOARD_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "Common/Result.h"

#include "../NhCore/RingIterator.h"
#include "../NhCore/System.h"

#include <stdbool.h>

typedef struct Nh_Tab Nh_Tab;
typedef struct Nh_WSI_Input Nh_WSI_Input;

#endif

/** @addtogroup IOEnums Enums
 *  \ingroup IO
 *  @{
 */

    typedef enum NH_WSI_KEY {
        NH_WSI_KEY_UNDEFINED   ,
        
        // Modifiers
        NH_WSI_KEY_SHIFT_L     , 
        NH_WSI_KEY_SHIFT_R     ,
        NH_WSI_KEY_CONTROL_L   ,
        NH_WSI_KEY_CONTROL_R   ,
        NH_WSI_KEY_CAPS_LOCK   ,
        NH_WSI_KEY_SHIFT_LOCK  , 
        NH_WSI_KEY_META_L      , 
        NH_WSI_KEY_META_R      , 
        NH_WSI_KEY_ALT_L       , 
        NH_WSI_KEY_ALT_R       , 
        NH_WSI_KEY_SUPER_L     , 
        NH_WSI_KEY_SUPER_R     , 
        NH_WSI_KEY_HYPER_L     , 
        NH_WSI_KEY_HYPER_R     , 
    
        // Latin 1 (ISO/IEC 8859-1 = Unicode U+0020..U+00FF)
        NH_WSI_KEY_SPACE       , 
        NH_WSI_KEY_EXCLAM      , 
        NH_WSI_KEY_QUOTEDBL    , 
        NH_WSI_KEY_NUMBERSIGN  , 
        NH_WSI_KEY_DOLLAR      , 
        NH_WSI_KEY_PERCENT     , 
        NH_WSI_KEY_AMPERSAND   , 
        NH_WSI_KEY_APOSTROPHE  , 
        NH_WSI_KEY_PARENLEFT   , 
        NH_WSI_KEY_PARENRIGHT  , 
        NH_WSI_KEY_ASTERISK    , 
        NH_WSI_KEY_PLUS        , 
        NH_WSI_KEY_COMMA       , 
        NH_WSI_KEY_MINUS       , 
        NH_WSI_KEY_PERIOD      , 
        NH_WSI_KEY_SLASH       , 
        NH_WSI_KEY_0           , 
        NH_WSI_KEY_1           , 
        NH_WSI_KEY_2           , 
        NH_WSI_KEY_3           , 
        NH_WSI_KEY_4           , 
        NH_WSI_KEY_5           , 
        NH_WSI_KEY_6           , 
        NH_WSI_KEY_7           , 
        NH_WSI_KEY_8           , 
        NH_WSI_KEY_9           , 
        NH_WSI_KEY_COLON       , 
        NH_WSI_KEY_SEMICOLON   , 
        NH_WSI_KEY_LESS        , 
        NH_WSI_KEY_EQUAL       , 
        NH_WSI_KEY_GREATER     , 
        NH_WSI_KEY_QUESTION    , 
        NH_WSI_KEY_AT          , 
        NH_WSI_KEY_CAPITAL_A   ,
        NH_WSI_KEY_CAPITAL_B   ,
        NH_WSI_KEY_CAPITAL_C   ,
        NH_WSI_KEY_CAPITAL_D   ,
        NH_WSI_KEY_CAPITAL_E   ,
        NH_WSI_KEY_CAPITAL_F   ,
        NH_WSI_KEY_CAPITAL_G   ,
        NH_WSI_KEY_CAPITAL_H   ,
        NH_WSI_KEY_CAPITAL_I   ,
        NH_WSI_KEY_CAPITAL_J   ,
        NH_WSI_KEY_CAPITAL_K   ,
        NH_WSI_KEY_CAPITAL_L   ,
        NH_WSI_KEY_CAPITAL_M   ,
        NH_WSI_KEY_CAPITAL_N   ,
        NH_WSI_KEY_CAPITAL_O   ,
        NH_WSI_KEY_CAPITAL_P   ,
        NH_WSI_KEY_CAPITAL_Q   ,
        NH_WSI_KEY_CAPITAL_R   ,
        NH_WSI_KEY_CAPITAL_S   ,
        NH_WSI_KEY_CAPITAL_T   ,
        NH_WSI_KEY_CAPITAL_U   ,
        NH_WSI_KEY_CAPITAL_V   ,
        NH_WSI_KEY_CAPITAL_W   ,
        NH_WSI_KEY_CAPITAL_X   ,
        NH_WSI_KEY_CAPITAL_Y   ,
        NH_WSI_KEY_CAPITAL_Z   ,
        NH_WSI_KEY_BRACKETLEFT , 
        NH_WSI_KEY_BACKSLASH   , 
        NH_WSI_KEY_BRACKETRIGHT, 
        NH_WSI_KEY_ASCIICIRCUM , 
        NH_WSI_KEY_UNDERSCORE  , 
        NH_WSI_KEY_GRAVE       , 
        NH_WSI_KEY_SMALL_A     , 
        NH_WSI_KEY_SMALL_B     , 
        NH_WSI_KEY_SMALL_C     , 
        NH_WSI_KEY_SMALL_D     , 
        NH_WSI_KEY_SMALL_E     , 
        NH_WSI_KEY_SMALL_F     , 
        NH_WSI_KEY_SMALL_G     , 
        NH_WSI_KEY_SMALL_H     , 
        NH_WSI_KEY_SMALL_I     , 
        NH_WSI_KEY_SMALL_J     , 
        NH_WSI_KEY_SMALL_K     , 
        NH_WSI_KEY_SMALL_L     , 
        NH_WSI_KEY_SMALL_M     , 
        NH_WSI_KEY_SMALL_N     , 
        NH_WSI_KEY_SMALL_O     , 
        NH_WSI_KEY_SMALL_P     , 
        NH_WSI_KEY_SMALL_Q     , 
        NH_WSI_KEY_SMALL_R     , 
        NH_WSI_KEY_SMALL_S     , 
        NH_WSI_KEY_SMALL_T     , 
        NH_WSI_KEY_SMALL_U     , 
        NH_WSI_KEY_SMALL_V     , 
        NH_WSI_KEY_SMALL_W     , 
        NH_WSI_KEY_SMALL_X     , 
        NH_WSI_KEY_SMALL_Y     , 
        NH_WSI_KEY_SMALL_Z     , 
        NH_WSI_KEY_BRACELEFT   , 
        NH_WSI_KEY_BAR         , 
        NH_WSI_KEY_BRACERIGHT  , 
        NH_WSI_KEY_ASCIITILDE  ,
    
        NH_WSI_KEY_COUNT       , 
    } NH_WSI_KEY;

/** @} */

/** @addtogroup IOStructs Structs
 *  \ingroup IO
 *  @{
 */  

    typedef struct Nh_WSI_KeyboardEvent {
        NH_WSI_KEY key;    
        int trigger; 
    } Nh_WSI_KeyboardEvent;            

    typedef struct Nh_WSI_Keyboard {
        Nh_RingIteratorMark Mark;
        Nh_RingIterator Events;
        bool state_p[NH_WSI_KEY_COUNT];              
    } Nh_WSI_Keyboard;

/** @} */

/** @addtogroup IOFunctions Functions
 *  \ingroup IO
 *  @{
 */

    NH_WSI_RESULT Nh_WSI_initKeyboard(
        Nh_WSI_Keyboard *Keyboard_p, int bufferSize
    ); 

/** @} */

#endif 

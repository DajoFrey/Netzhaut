#ifndef NH_DOM_CHARACTER_DATA_H
#define NH_DOM_CHARACTER_DATA_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "Common/API.h"
#include "Common/Result.h"

#endif

/** @addtogroup NhDOMFunctions
 *  @{
 */

    NH_DOM_RESULT Nh_DOM_appendToCharacterData(
        Nh_WebIDL_Object *CharacterData_p, Nh_WebIDL_DOMString DOMString 
    );

/** @} */

#endif

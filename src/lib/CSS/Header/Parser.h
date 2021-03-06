#ifndef NH_CSS_PARSER_H
#define NH_CSS_PARSER_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "Sheet.h"

#include "../../NhCore/Netzhaut.h"

#endif

/** @addtogroup CSSFunctions Functions
 *  \ingroup CSS
 *  @{
 */

    NH_RESULT Nh_CSS_parseSheet(
        Nh_CSS_Sheet *Sheet_p
    );

    NH_RESULT Nh_CSS_parseSheets(
        Nh_LinkedList *Sheets_p
    );

/** @} */

#endif 

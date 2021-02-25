#ifndef DOXYGEN_SHOULD_SKIP_THIS

#ifndef NH_CSS_ASSOCIATE_H
#define NH_CSS_ASSOCIATE_H

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "Parser.h"

#include "../../Netzhaut/Netzhaut.h"
#include "../../HTML/Main/Header/Document.h"

#endif

/** @addtogroup CSSFunctions Functions
 *  \ingroup CSS
 *  @{
 */

    NH_RESULT Nh_CSS_associateSheets(
        Nh_HTML_Document *Document_p, Nh_HTML_Node *Node_p
    );

    NH_RESULT Nh_CSS_reassociateSheets(
        Nh_HTML_Document *Document_p, Nh_HTML_Node *Node_p
    );

/** @} */

#endif 

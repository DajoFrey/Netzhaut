#ifndef DOXYGEN_SHOULD_SKIP_THIS

#ifndef NH_HTML_PRE_ARRANGE_H
#define NH_HTML_PRE_ARRANGE_H

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "../../Main/Header/Document.h"

#endif

/** @addtogroup HTMLFunctions Functions
 *  \ingroup HTML
 *  @{
 */

    NH_BOOL Nh_HTML_respectFlow(
        Nh_HTML_Node *Node_p
    );

    NH_RESULT Nh_HTML_preArrange(
        Nh_HTML_Document *Document_p
    );

/** @} */

#endif

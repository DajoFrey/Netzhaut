#ifndef NH_HTML_DOCUMENT_H
#define NH_HTML_DOCUMENT_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "Input.h"
#include "Tree.h"
#include "Selection.h"
#include "Node.h" // for convenience

#include "../../../NhCore/URI.h"
#include "../../../NhCore/Content.h"
#include "../../../NhCore/Config.h"

#endif

/** @addtogroup HTMLStructs Structs
 *  \ingroup HTML
 *  @{
 */

    /**
     * Represents a HTML document. It holds the document tree and all other related resources.
     */
    typedef struct Nh_HTML_Document {

        Nh_Content *Content_p;
        Nh_HTML_Input Input;         /**<Holds information for input handling.*/
        Nh_HTML_Selection Selection;
        Nh_HTML_Tree Tree;           /**<The HTML document tree.*/ 
        Nh_LinkedList Sheets;        /**<Holds all associated CSS data in the form of @ref Nh_CSS_Sheet.*/

    } Nh_HTML_Document;

/** @} */

/** @addtogroup HTMLFunctions Functions
 *  \ingroup HTML
 *  @{
 */

    /**
     *
     */
    NH_RESULT Nh_HTML_createDocument(
        Nh_HTML_Document *Document_p, Nh_Content *Content_p
    );

    NH_RESULT Nh_HTML_updateDocument(
        Nh_HTML_Document *Document_p
    );

    NH_RESULT Nh_HTML_destroyDocument(
        Nh_HTML_Document *Document_p
    );

    char *Nh_HTML_getTitle(
        Nh_HTML_Document *Document_p
    );

    void Nh_HTML_getBase(
        Nh_HTML_Document *Document_p, char *set_p
    );

/** @} */

#endif

#ifndef NH_HTML_SELECTION_H
#define NH_HTML_SELECTION_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS
    
/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "Tree.h"
#include "Box.h"

#include "../../../NhCore/Netzhaut.h"
#include "../../../NhCore/LinkedList.h"

#include <stdint.h>

typedef struct Nh_HTML_Node Nh_HTML_Node;
typedef struct Nh_HTML_Document Nh_HTML_Document;

#endif

/** @addtogroup HTMLStructs Structs
 *  \ingroup HTML
 *  @{
 */

    typedef struct Nh_HTML_SelectionRange {
        Nh_HTML_Node *Begin_p;
        Nh_HTML_Node *End_p;
    } Nh_HTML_SelectionRange;

    typedef struct Nh_HTML_Selection {
        NH_BOOL update;
        Nh_LinkedList Ranges;
    } Nh_HTML_Selection;

/** @} */

/** @addtogroup HTMLFunctions Functions
 *  \ingroup HTML
 *  @{
 */

    NH_RESULT Nh_HTML_addSelectionRange(
        Nh_HTML_Document *Document_p, Nh_HTML_SelectionRange Range
    );

    NH_BOOL Nh_HTML_selected(
        Nh_HTML_Document *Document_p, Nh_HTML_Node *Node_p
    );

/** @} */

#endif 

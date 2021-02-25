#ifndef NH_HTML_TREE_H
#define NH_HTML_TREE_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "../../../NhCore/Netzhaut.h"
#include "../../../NhCore/List.h"

typedef struct Nh_HTML_Node Nh_HTML_Node;
typedef struct Nh_HTML_Document Nh_HTML_Document;

#endif

/** @addtogroup HTMLStructs Structs
 *  \ingroup HTML
 *  @{
 */

    /** 
     * HTML document tree.
     */
    typedef struct Nh_HTML_Tree { 
        Nh_HTML_Node *Root_p;    /**<A pointer to the root node.*/
        Nh_List All;
        Nh_List Render;
        Nh_List RenderNoChars;
    } Nh_HTML_Tree;

/** @} */

/** @addtogroup HTMLFunctions Functions
 *  \ingroup HTML
 *  @{
 */

    NH_RESULT Nh_HTML_initTree(
        Nh_HTML_Document *Document_p
    );

    NH_RESULT Nh_HTML_computeTreeGraphics(
        Nh_HTML_Document *Document_p
    );
    
    NH_RESULT Nh_HTML_resetTreeGraphics(
        Nh_HTML_Document *Document_p
    );
    
    NH_RESULT Nh_HTML_computeTree(
        Nh_HTML_Document *Document_p
    );
    
    NH_RESULT Nh_HTML_resetTree(
        Nh_HTML_Document *Document_p
    );

    NH_RESULT Nh_HTML_destroyTree(
        Nh_HTML_Document *Document_p
    );
    
    char *Nh_HTML_stringifyTree(
        Nh_HTML_Tree *Tree_p
    );

/** @} */

#endif


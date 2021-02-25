#ifndef NH_CSS_CURSOR_H
#define NH_CSS_CURSOR_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "GenericProperty.h"
#include "../../NhCore/Netzhaut.h"
#include "../../NhIO/Mouse.h"

typedef struct Nh_HTML_Node Nh_HTML_Node;

#endif

/** @addtogroup CSSEnums Enums
 *  \ingroup CSS
 *  @{
 */

    typedef enum NH_CSS_CURSOR {
        NH_CSS_CURSOR_AUTO,
        NH_CSS_CURSOR_DEFAULT,
        NH_CSS_CURSOR_NONE,
        NH_CSS_CURSOR_HELP,
        NH_CSS_CURSOR_POINTER,
        NH_CSS_CURSOR_PROGRESS,
        NH_CSS_CURSOR_WAIT,
        NH_CSS_CURSOR_CELL,
        NH_CSS_CURSOR_CROSSHAIR,
        NH_CSS_CURSOR_TEXT,
        NH_CSS_CURSOR_VERTICAL_TEXT,
        NH_CSS_CURSOR_ALIAS,
        NH_CSS_CURSOR_COPY,
        NH_CSS_CURSOR_MOVE,
        NH_CSS_CURSOR_NO_DROP,
        NH_CSS_CURSOR_NOT_ALLOWED,
        NH_CSS_CURSOR_GRAB,
        NH_CSS_CURSOR_GRABBING,
        NH_CSS_CURSOR_ALL_SCROLL,
        NH_CSS_CURSOR_COL_RESIZE,
        NH_CSS_CURSOR_ROW_RESIZE,
        NH_CSS_CURSOR_N_RESIZE,  
        NH_CSS_CURSOR_E_RESIZE, 
        NH_CSS_CURSOR_S_RESIZE, 
        NH_CSS_CURSOR_W_RESIZE, 
        NH_CSS_CURSOR_NE_RESIZE,
        NH_CSS_CURSOR_NW_RESIZE,
        NH_CSS_CURSOR_SE_RESIZE,
        NH_CSS_CURSOR_SW_RESIZE,
        NH_CSS_CURSOR_EW_RESIZE,
        NH_CSS_CURSOR_NS_RESIZE, 	
        NH_CSS_CURSOR_NESW_RESIZE, 	
        NH_CSS_CURSOR_NWSE_RESIZE, 	
        NH_CSS_CURSOR_ZOOM_IN,
        NH_CSS_CURSOR_ZOOM_OUT,
    } NH_CSS_CURSOR;
 
/** @} */

/** @addtogroup CSSFunctions Functions
 *  \ingroup CSS
 *  @{
 */

    NH_RESULT Nh_CSS_computeCursorProperty(
        Nh_HTML_Node *Node_p, Nh_CSS_GenericProperty **Properties_pp
    );

    NH_IO_MOUSE_CURSOR Nh_CSS_normalizeMouseCursor(
        Nh_HTML_Node *Node_p
    );

/** @} */

#endif 

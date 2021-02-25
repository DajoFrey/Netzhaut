#ifndef NH_CSS_PROPERTIES_H
#define NH_CSS_PROPERTIES_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "Background.h"
#include "Selector.h"
#include "Text.h"
#include "Border.h"
#include "List.h"
#include "Position.h"
#include "Animation.h"
#include "Cursor.h"

#include "../../Netzhaut/Netzhaut.h"

typedef struct Nh_Tab Nh_Tab;
typedef struct Nh_HTML_Node Nh_HTML_Node;

#endif

/** @addtogroup CSSEnums Enums
 *  \ingroup CSS
 *  @{
 */

    typedef enum NH_CSS_BACKFACE_VISIBILITY {
        NH_CSS_BACKFACE_VISIBILITY_VISIBLE, 	
        NH_CSS_BACKFACE_VISIBILITY_HIDDEN,  	
    } NH_CSS_BACKFACE_VISIBILITY;
    
/** @} */

/** @addtogroup CSSStructs Structs
 *  \ingroup CSS
 *  @{
 */

    typedef struct Nh_CSS_Properties {
    
        Nh_LinkedList Candidates;

        NH_CSS_Animation Animation; 
        NH_CSS_Background Background;  
        NH_CSS_Position Position;      
        NH_CSS_Border Border;
        Nh_CSS_PixelSides Margin;
        Nh_CSS_PixelSides Padding;          
        NH_CSS_Text Text;              
        NH_CSS_List List;              

        NH_CSS_CURSOR cursor;     
        NH_CSS_BACKFACE_VISIBILITY backfaceVisibility;
    
    } Nh_CSS_Properties;

/** @} */

/** @addtogroup CSSFunctions Functions
 *  \ingroup CSS
 *  @{
 */

    NH_RESULT Nh_CSS_initProperties(
        Nh_HTML_Node *Node_p
    );

    NH_RESULT Nh_CSS_computeNodeProperties(
        Nh_Content *Content_p, Nh_HTML_Node *Node_p, NH_BOOL *recomputeTree_p, NH_BOOL lazy
    );

    NH_RESULT Nh_CSS_computeProperties(
        Nh_HTML_Document *Document_p, NH_BOOL *recomputeTree_p, NH_BOOL lazy
    );

    void Nh_CSS_destroyProperties(
        Nh_CSS_Properties *Properties_p
    );

/** @} */

#endif 

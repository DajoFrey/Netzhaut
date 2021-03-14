#ifndef NH_CONTENT_H
#define NH_CONTENT_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "Tab.h"

#endif

/** @addtogroup NhCoreEnums
 *  @{
 */

    typedef enum NH_CONTENT {
        NH_CONTENT_UNDEFINED,
        NH_CONTENT_HTML_DOCUMENT,
    } NH_CONTENT;

/** @} */

/** @addtogroup NhCoreStructs
 *  @{
 */

    typedef struct Nh_Content {

        Nh_Tab *Tab_p;       /**<A pointer to the tab which holds the container.*/

        Nh_WSI_Input Input;
        Nh_URI URI;

        Nh_LinkedList Callbacks;
        Nh_LinkedList Textures;  

        NH_CONTENT type;
        void *data_p;

        Nh_PixelSize Size;    
        Nh_PixelPosition Position;    

        struct {
            NH_BOOL visible;        /**<Defines if the container is visible or not. Only one container per tab can be visible. Set this via Nh_showContent.*/
        } Flags;

        struct {
            float scale;
        } Settings;

    } Nh_Content;

/** @} */

/** @addtogroup NhCoreFunctions
 *  @{
 */

    NH_RESULT Nh_openContent(
        Nh_Tab *Tab_p, char *URI_p
    );

    NH_RESULT Nh_transitionContent(
        Nh_Content *Content_p, Nh_Tab *New_p
    );
    
    NH_RESULT Nh_destroyContent(
        Nh_Content *Content_p
    );
    
    NH_RESULT Nh_destroyContents(
        Nh_Tab *Tab_p
    );

    NH_RESULT Nh_updateContent(
        Nh_Content *Content_p
    );
    
    NH_RESULT Nh_resizeContent(
        Nh_Content *Content_p
    );

    NH_RESULT Nh_showContent(
        Nh_Content *Content_p
    );
    
    Nh_Content *Nh_getVisibleContent(
        Nh_Tab *Tab_p
    );
    
    char *Nh_getTitle(
        Nh_Content *Content_p
    );
    
    Nh_Content *Nh_getContent(
        Nh_Tab *Tab_p, int index
    );

    NH_RESULT Nh_addContent(
        Nh_Content *Content_p, Nh_Tab *Tab_p
    );
    
    NH_RESULT Nh_removeContent(
        Nh_Content *Content_p
    );

/** @} */

#endif

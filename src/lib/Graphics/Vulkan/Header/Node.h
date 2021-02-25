#ifndef NH_VK_NODE_H
#define NH_VK_NODE_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "../../../HTML/Main/Header/Document.h"
#include "../../../Netzhaut/Netzhaut.h"
#include "../../../CSS/Header/GenericProperty.h"

#endif

/** @addtogroup VulkanFunctions Functions
 *  \ingroup Vulkan
 *  @{
 */

    NH_RESULT Nh_Vk_createNode(
        Nh_Content *Content_p, Nh_HTML_Node *Node_p
    );

    NH_RESULT Nh_Vk_updateNodeProperty(
        Nh_Vk_GPU *GPU_p, Nh_HTML_Node *Node_p, Nh_CSS_GenericProperty *Property_p
    );

    void Nh_Vk_destroyNode(
        Nh_Vk_GPU *GPU_p, Nh_HTML_Node *Node_p
    );

/** @} */

#endif 

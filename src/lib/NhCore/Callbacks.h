#ifndef NH_CALLBACKS_H
#define NH_CALLBACKS_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "Tab.h"
#include "LinkedList.h"

#include "../Graphics/Vulkan/Header/Vulkan.h"

#endif

/** @addtogroup NhCoreEnums
 *  @{
 */

    typedef enum NH_CALLBACK {
        NH_CALLBACK_VULKAN,
    } NH_CALLBACK;

/** @} */

/** @addtogroup NhCoreTypedefs
 *  @{
 */

    typedef NH_RESULT (*Nh_VulkanCallback)(struct VolkDeviceTable, VkCommandBuffer*);

/** @} */

/** @addtogroup NhCoreFunctions
 *  @{
 */

    NH_RESULT Nh_setVulkanCallback(
        Nh_LinkedList *Callbacks_p, char *id_p, NH_RESULT (*function_p)(struct VolkDeviceTable, VkCommandBuffer*)
    );
    
    Nh_VulkanCallback Nh_getVulkanCallback(
        Nh_LinkedList *Callbacks_p, char *id_p
    );
    
    void Nh_deleteCallbacks(
        Nh_LinkedList *Callbacks_p
    );

/** @} */

#endif 

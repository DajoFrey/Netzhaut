#ifndef NH_VK_VULKAN_H
#define NH_VK_VULKAN_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#ifndef VOLK_VULKAN_H_PATH
    #define VOLK_VULKAN_H_PATH "vulkan.h"
#endif

#include "External/volk.h"

typedef struct Nh_Vk_Host Nh_Vk_Host;
typedef struct Nh_Vk_GPU Nh_Vk_GPU;

#endif

/** @addtogroup VulkanFunctions Functions
 *  \ingroup Vulkan
 *  @{
 */

    Nh_Vk_Host *Nh_Vk_getHost(
    );

    NH_RESULT Nh_Vk_initVulkan(
    );
    
    NH_RESULT Nh_Vk_freeVulkan(
    );

    Nh_Vk_GPU *Nh_Vk_getGPU(
        int i
    );

    int Nh_Vk_getGPUIndex(
        Nh_Vk_GPU *GPU_p
    );

    int Nh_Vk_getGPUCount(
    );

/** @} */

#endif 

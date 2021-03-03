#ifndef NH_VK_GPU_H
#define NH_VK_GPU_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "Pipeline.h"
#include "Driver.h"

#include "../../../NhCore/LinkedList.h"

#endif

/** @addtogroup VulkanStructs Structs
 *  \ingroup Vulkan
 *  @{
 */

    typedef struct Nh_Vk_GPU {
        char *name_p;                       
        Nh_Vk_Driver Driver;   
        Nh_Vk_Pipeline Pipelines_p[NH_VK_PIPELINE_COUNT];
    } Nh_Vk_GPU;

/** @} */

/** @addtogroup VulkanFunctions Functions
 *  \ingroup Vulkan
 *  @{
 */

    NH_RESULT Nh_Vk_initGPUs(
        Nh_LinkedList *GPUs_p, Nh_Vk_Host *Host_p
    );

    void Nh_Vk_freeGPUs(
        Nh_LinkedList *GPUs_p
    );

/** @} */

#endif

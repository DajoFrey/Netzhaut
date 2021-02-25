// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE ========================================================================================

#ifdef __unix__
#define _GNU_SOURCE
#endif

#include "../Header/Vulkan.h"
#include "../Header/Host.h"
#include "../Header/GPU.h"

#include "../../Main/Header/Macros.h"

#include "../../../Netzhaut/LinkedList.h"
#include "../../../Netzhaut/Config.h"

#include NH_FLOW
#include NH_DEFAULT_CHECK

#include <string.h>
#include <stdio.h>

// DECLARE =========================================================================================

typedef struct Nh_Vk_Vulkan {
    Nh_Vk_Host Host;
    Nh_LinkedList GPUs;
} Nh_Vk_Vulkan;

// DATA ============================================================================================

Nh_Vk_Vulkan Vulkan;

// VULKAN ==========================================================================================

NH_RESULT Nh_Vk_initVulkan()
{
NH_BEGIN()

    NH_INIT_LINKED_LIST(Vulkan.GPUs)

    if (Nh_Vk_createHost(NH_CONFIG.Flags.Log.vulkan) == NH_SUCCESS) {
        NH_CHECK(Nh_Vk_initGPUs(&Vulkan.GPUs, &Vulkan.Host))
        if (Vulkan.GPUs.count <= 0) {NH_DIAGNOSTIC_END(NH_VULKAN_ERROR_NO_DEVICE_SUITABLE)}
    } 
    else {NH_DIAGNOSTIC_END(NH_VULKAN_ERROR_HOST_CREATION_FAILED)}

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

NH_RESULT Nh_Vk_freeVulkan()
{
NH_BEGIN()

    Nh_Vk_freeGPUs(&Vulkan.GPUs);
    Nh_Vk_destroyHost();

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

Nh_Vk_Host *Nh_Vk_getHost()
{
NH_BEGIN()
NH_END(&Vulkan.Host)
}

Nh_Vk_GPU *Nh_Vk_getGPU(
    int i)
{
NH_BEGIN()
NH_END(Nh_getFromLinkedList(&Vulkan.GPUs, i))
}

int Nh_Vk_getGPUIndex(
    Nh_Vk_GPU *GPU_p)
{
NH_BEGIN()

    for (int i = 0; i < Vulkan.GPUs.count; ++i) {
        if (Nh_getFromLinkedList(&Vulkan.GPUs, i) == GPU_p) {NH_END(i)}
    }

NH_END(-1)
}

int Nh_Vk_getGPUCount()
{
NH_BEGIN()
NH_END(Vulkan.GPUs.count)
}


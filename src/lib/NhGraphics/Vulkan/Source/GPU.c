// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE ========================================================================================

#include "../Header/GPU.h"
#include "../Header/Host.h"
#include "../Header/Driver.h"
#include "../Header/Texture.h"

#include "../../Main/Header/Macros.h"

#include "../../../Netzhaut/Memory.h"
#include "../../../Netzhaut/API/Header/Vulkan.h"

#include NH_FLOW
#include NH_DEFAULT_CHECK

#include <string.h>
#include <stdio.h>

// DECLARE =========================================================================================

static NH_RESULT Nh_Vk_addDetectedGPUs(
    Nh_LinkedList *GPUs_p, Nh_Vk_Host *Host_p
);

// GPU =============================================================================================

NH_RESULT Nh_Vk_initGPUs(
    Nh_LinkedList *GPUs_p, Nh_Vk_Host *Host_p)
{
NH_BEGIN()

    NH_CHECK(Nh_Vk_addDetectedGPUs(GPUs_p, Host_p))

    for (int i = 0; i < GPUs_p->count; i++) 
    {
        Nh_Vk_GPU *GPU_p = Nh_getFromLinkedList(GPUs_p, i);
        NH_CHECK(Nh_Vk_createDriver(Host_p, GPU_p))
        NH_CHECK(Nh_Vk_createPipelines(GPU_p))
    }

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

void Nh_Vk_freeGPUs(
    Nh_LinkedList *GPUs_p)
{
NH_BEGIN()

    for (int i = 0; i < GPUs_p->count; i++) 
    {
        Nh_Vk_GPU *GPU_p = Nh_getFromLinkedList(GPUs_p, i);
        Nh_free(GPU_p->name_p);
        Nh_Vk_destroyPipelines(GPU_p);
        Nh_Vk_destroyDriver(&GPU_p->Driver);
    }

    Nh_destroyLinkedList(GPUs_p, true);

NH_SILENT_END()
}

// HELPER ==========================================================================================

static NH_RESULT Nh_Vk_addDetectedGPUs(
    Nh_LinkedList *GPUs_p, Nh_Vk_Host *Host_p)
{
NH_BEGIN()

    int detectedCount = 0;
    Host_p->Func.vkEnumeratePhysicalDevices(Host_p->Instance, &detectedCount, VK_NULL_HANDLE);
    if (detectedCount <= 0) {NH_DIAGNOSTIC_END(NH_SUCCESS)}
   
    VkPhysicalDevice *Physical_p = Nh_allocate(sizeof(VkPhysicalDevice) * detectedCount);
    NH_CHECK_MEM(Physical_p)

    Host_p->Func.vkEnumeratePhysicalDevices(Host_p->Instance, &detectedCount, Physical_p);

    for (int i = 0; i < detectedCount; i++) 
    {
        Nh_Vk_GPU *GPU_p = Nh_allocate(sizeof(Nh_Vk_GPU));
        NH_CHECK_MEM(GPU_p)

        VkPhysicalDeviceProperties Properties;
        Host_p->Func.vkGetPhysicalDeviceProperties(Physical_p[i], &Properties);

        GPU_p->name_p = Nh_allocate(sizeof(char) * (strlen(Properties.deviceName) + 1));
        NH_CHECK_MEM(GPU_p->name_p)

        strcpy(GPU_p->name_p, Properties.deviceName);

        NH_CHECK(Nh_appendToLinkedList(GPUs_p, GPU_p))
    }

    Nh_free(Physical_p);

NH_DIAGNOSTIC_END(NH_SUCCESS)
}


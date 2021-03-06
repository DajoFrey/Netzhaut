#ifndef NH_VK_DRIVER_H
#define NH_VK_DRIVER_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "../../../Netzhaut/Netzhaut.h"

typedef struct Nh_Vk_GPU Nh_Vk_GPU;

#endif

/** @addtogroup VulkanStructs Structs
 *  \ingroup Vulkan
 *  @{
 */

    typedef struct Nh_Vk_DriverInfo {
        uint32_t major;    
        uint32_t minor;    
        uint32_t patch;    
        uint32_t vendorId; 
        uint32_t deviceId; 
    } Nh_Vk_DriverInfo;

    typedef struct Nh_Vk_Driver {
        Nh_Vk_DriverInfo Info;                 
        VkPhysicalDevice PhysicalDevice;       
        VkDevice Device;                       
        struct VolkDeviceTable Func;           
        VkCommandPool *ComputeCommandPools_p;  
        VkCommandPool *GraphicsCommandPools_p; 
        VkDescriptorPool *DescriptorPool_p;    
        VkRenderPass *RenderPass_p;            
        VkQueue GraphicsQueue;                 
        VkQueue ComputeQueue;                  
        unsigned int descriptorPoolCount;      
        unsigned int renderPassCount;          
    } Nh_Vk_Driver;

/** @} */

/** @addtogroup VulkanFunctions Functions
 *  \ingroup Vulkan
 *  @{
 */

    int Nh_Vk_getDriver(
        Nh_Vk_Driver **Driver_pp
    );

    NH_RESULT Nh_Vk_createDriver(
        Nh_Vk_Host *Host_p, Nh_Vk_GPU *GPU_p
    );

    void Nh_Vk_destroyDriver(
        Nh_Vk_Driver *Driver_p
    );

/** @} */

#endif

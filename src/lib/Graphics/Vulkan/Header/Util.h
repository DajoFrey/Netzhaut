#ifndef NH_VK_UTIL_H
#define NH_VK_UTIL_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "../../../Netzhaut/Netzhaut.h"

#endif

/** @addtogroup VulkanEnums Enums
 *  \ingroup Vulkan
 *  @{
 */

    /**
     * Different types of a VkDeviceMemory handle. Must be specified when
     * creating a device memory handle.
     */
    typedef enum NH_VK_DEVICE_MEMORY {
        NH_VK_DEVICE_MEMORY_IMAGE, /**<Indicates VkImage memory.*/
        NH_VK_DEVICE_MEMORY_BUFFER /**<Indicates VkBuffer memory.*/
    } NH_VK_DEVICE_MEMORY;

/** @} */

/** @addtogroup VulkanStructs Structs
 *  \ingroup Vulkan
 *  @{
 */

    typedef struct NH_Vk_DeviceMemoryInfo{
        NH_VK_DEVICE_MEMORY deviceMemoryType;    
        VkMemoryPropertyFlags memoryProperties;
        void *bindSource_p;                    
    } NH_Vk_DeviceMemoryInfo;

/** @} */

/** @addtogroup VulkanFunctions Functions
 *  \ingroup Vulkan
 *  @{
 */

    NH_RESULT Nh_Vk_createBuffer(
        Nh_Vk_Host *Host_p, Nh_Vk_Driver *Driver_p, NH_Vk_BufferInfo *BufferInfo_p, 
        NH_Vk_Buffer *Buffer_p
    );
    
    NH_RESULT Nh_Vk_destroyBuffer(
        Nh_Vk_Driver *Driver_p, NH_Vk_Buffer *Buffer_p
    );

    NH_RESULT Nh_Vk_createDescriptorSet(
        Nh_Vk_Driver *Driver_p, VkDescriptorSetAllocateInfo *DescriptorSetAllocateInfo_p, 
        VkDescriptorSetLayoutCreateInfo *DescriptorSetLayoutInfo_p, NH_Vk_DescriptorSet *DescriptorSet_p
    );
    
    NH_RESULT Nh_Vk_destroyDescriptorSet(
        Nh_Vk_Driver *Driver_p, NH_Vk_DescriptorSet *DescriptorSet_p, VkDescriptorPool *DescriptorPool_p
    );
    
    NH_RESULT Nh_Vk_createDeviceMemory(
        Nh_Vk_Host *Host_p, Nh_Vk_Driver *Driver_p, NH_Vk_DeviceMemoryInfo *Info_p, 
        VkDeviceMemory *DeviceMemory_p
    );
    
    NH_RESULT Nh_Vk_bindDeviceMemory(
        Nh_Vk_Driver *Driver_p, VkDeviceMemory *DeviceMemory_p, NH_Vk_DeviceMemoryInfo *Info_p
    );
    
    NH_RESULT Nh_Vk_mapDeviceMemory(
        Nh_Vk_Driver *Driver_p, VkDeviceMemory *DeviceMemory_p, const void *data_p, 
        size_t bufferSize, size_t offset
    );
    
    NH_RESULT Nh_Vk_findMemoryType(
        Nh_Vk_Host *Host_p, Nh_Vk_Driver *Driver_p, int typeFilter, VkMemoryPropertyFlags properties, 
        uint32_t *result_p
    ); 

    NH_RESULT Nh_Vk_createPipeline(
        Nh_Vk_Driver *Driver_p, NH_VK_PIPELINE PipelineType, VkPipelineLayoutCreateInfo *PipelineLayoutCreateInfo_p, 
        void *pipelineInfo_p, Nh_Vk_Pipeline *Pipeline_p
    );
    
    void Nh_Vk_destroyPipeline(
        Nh_Vk_Driver *Driver_p, Nh_Vk_Pipeline *Pipeline_p
    );

    NH_RESULT Nh_Vk_createShaderModule(
        Nh_Vk_Driver *Driver_p, const uint32_t *code, size_t size, VkShaderModule *ShaderModule_p
    );

    NH_RESULT Nh_Vk_getResult(
        int result
    );

/** @} */

#endif

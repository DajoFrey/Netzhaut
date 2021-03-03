#ifndef NH_VK_TEXTURE_H
#define NH_VK_TEXTURE_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "../Header/GPU.h"
#include "../../../NhCore/Texture.h"

#endif

/** @addtogroup VulkanStructs Structs
 *  \ingroup Vulkan
 *  @{
 */

   typedef struct Nh_Vk_TextureInfo {
        unsigned int width;           
        unsigned int height;          
        int memoryProperties;         
        void *data_p;                 
        size_t size;                  
        VkImageLayout imageLayout;    
        VkFormat format;              
        VkImageUsageFlags usageFlags; 
    } Nh_Vk_TextureInfo;
    
   typedef struct Nh_Vk_Texture {
        int width;                                                  
        int height;                                
        VkImage Image;                             
        VkImageView ImageView;                     
        VkDeviceMemory DeviceMemory;
        VkDescriptorImageInfo DescriptorImageInfo; 
        VkSampler Sampler;                         
    } Nh_Vk_Texture;

/** @} */

/** @addtogroup VulkanFunctions Functions
 *  \ingroup Vulkan
 *  @{
 */

    NH_RESULT Nh_Vk_loadTexture(
        Nh_Vk_Driver *Driver_p, Nh_Vk_TextureInfo *TextureInfo_p, Nh_Vk_Texture *Texture_p, int threadId
    );
    
    NH_RESULT Nh_Vk_reloadTexture(
       Nh_Vk_Driver *Driver_p, Nh_Vk_TextureInfo *Info_p, Nh_Vk_Texture *Texture_p, int threadIndex
    );

    void Nh_Vk_destroyTexture(
        Nh_Vk_GPU *GPU_p, Nh_Vk_Texture *Texture_p
    );

    NH_RESULT Nh_Vk_hardLoadTexture(
        Nh_Vk_GPU *GPU_p, Nh_Texture *Texture_p
    );

    NH_RESULT Nh_Vk_hardReloadTexture(
        Nh_Vk_GPU *GPU_p, Nh_Texture *Texture_p
    );

/** @} */

#endif

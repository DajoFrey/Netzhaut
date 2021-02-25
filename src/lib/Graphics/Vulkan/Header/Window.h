#ifndef NH_VK_WINDOW_H
#define NH_VK_WINDOW_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "../../../Netzhaut/API/Header/Vulkan.h"
#include "../../../Netzhaut/Netzhaut.h"

#include "../../../Netzhaut/Window.h"

#endif

/** @addtogroup VulkanStructs Structs
 *  \ingroup Vulkan
 *  @{
 */

    typedef struct Nh_Vk_Surface {

        VkFramebuffer *Framebuffer_p;        
        VkSwapchainKHR SwapchainKHR;         
        VkExtent2D Extent2D;                 
        VkCommandBuffer *CommandBuffers_p; 

        struct {
            VkImageView ImageView;
            VkImage Image;
            VkDeviceMemory Memory;
        } DepthStencil;

        struct {
            VkImageView *ImageView_p;            
            VkImage *Image_p; 
        } Swapchain;                   

        struct {
            VkFence Fence;                       
            VkSemaphore Semaphore_p[2];         
        } Sync;

        unsigned int imageCount;         
        uint32_t currentImage;               

    } Nh_Vk_Surface;

/** @} */

/** @addtogroup VulkanFunctions Functions
 *  \ingroup Vulkan
 *  @{
 */

    NH_RESULT Nh_Vk_createWindowResources(
        Nh_Window *Window_p
    );
    
    NH_RESULT Nh_Vk_destroyWindowResources(
        Nh_Window *Window_p
    );

    NH_RESULT Nh_Vk_resize(
        Nh_Window *Window_p
    );

    NH_RESULT Nh_Vk_prepareRendering(
        Nh_Window *Window_p
    );

    NH_RESULT Nh_Vk_render(
        Nh_Window *Window_p
    );

/** @} */

#endif

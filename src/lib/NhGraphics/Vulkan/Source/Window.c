// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE ========================================================================================

#include "../Header/Window.h"
#include "../Header/Driver.h"
#include "../Header/Host.h"
#include "../Header/GPU.h"
#include "../Header/Util.h"
#include "../Header/Vulkan.h"

#include "../../Main/Header/Macros.h"

#include "../../../Netzhaut/Tab.h"
#include "../../../Netzhaut/Memory.h"
#include "../../../Netzhaut/Sync.h"
#include "../../../Netzhaut/LinkedList.h"
#include "../../../Netzhaut/Config.h"

#include NH_FLOW
#include NH_DEFAULT_CHECK

#include <limits.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// CREATE ==========================================================================================

static NH_RESULT Nh_Vk_getSurfaceKHR(
    Nh_Vk_Host *Host_p, Nh_IO_Window *Window_p, VkSurfaceKHR *SurfaceKHR_p)
{
NH_BEGIN()

#ifdef __unix__

    if (Window_p->type == NH_IO_TYPE_X11)
    {
        VkXlibSurfaceCreateInfoKHR surfaceInfo = 
        { 
            .sType  = VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR,
            .pNext  = VK_NULL_HANDLE,
            .dpy    = Window_p->X11.Display_p,
            .window = Window_p->X11.XWindow, 
        };
        Host_p->Func.vkCreateXlibSurfaceKHR(
            Host_p->Instance, &surfaceInfo, VK_NULL_HANDLE, SurfaceKHR_p
        ); 
    }
    else {NH_DIAGNOSTIC_END(NH_ERROR_BAD_STATE)}

#elif defined(_WIN32) || defined (WIN32)

    if (Window_p->type == NH_IO_TYPE_WIN32)
    {
        VkWin32SurfaceCreateInfoKHR surfaceInfo =
        {
            .sType     = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR,
            .pNext     = VK_NULL_HANDLE,
            .flags     = 0,
            .hinstance = Window_p->Win32.hinstance,
            .hwnd      = Window_p->Win32.hwnd, 
        };
        Host_p->Func.vkCreateWin32SurfaceKHR(
            Host_p->Instance, &surfaceInfo, VK_NULL_HANDLE, SurfaceKHR_p
        );
    }
    else {NH_DIAGNOSTIC_END(NH_ERROR_BAD_STATE)}
	
#endif
    
NH_DIAGNOSTIC_END(NH_SUCCESS)
}

static NH_RESULT Nh_Vk_chooseSurfaceFormat(
    VkSurfaceFormatKHR availableFormats[], VkSurfaceFormatKHR *format_p) 
{
NH_BEGIN();

    if (availableFormats[0].format == VK_FORMAT_UNDEFINED) { 
        VkSurfaceFormatKHR surfaceFormat = {VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR};
        *format_p = surfaceFormat;
    }

    for(int i = 0; i < sizeof(*availableFormats) / sizeof(availableFormats[0]); ++i) {
        if (availableFormats[i].format     == VK_FORMAT_B8G8R8A8_UNORM 
        &&  availableFormats[i].colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            *format_p = availableFormats[i];
	}
    }

    *format_p = availableFormats[0];
    
NH_DIAGNOSTIC_END(NH_SUCCESS);
}

static VkPresentModeKHR Nh_Vk_choosePresentMode(
    const VkPresentModeKHR *presentModes_p, int count) 
{
NH_BEGIN();

    for (int i = 0; i < count; ++i) {
        if (presentModes_p[i] == VK_PRESENT_MODE_IMMEDIATE_KHR) {NH_END(presentModes_p[i])}
    }
    
NH_END(-1);
}

static NH_RESULT Nh_Vk_createSwapchain(
    Nh_Vk_Driver *Driver_p, VkSurfaceKHR *SurfaceKHR_p, Nh_Vk_Surface *Surface_p, 
    uint32_t graphicsQueueFamily)
{
NH_BEGIN()
      
    Nh_Vk_Host *Host_p = Nh_Vk_getHost(); 
    VkPhysicalDevice *Device_p = &Driver_p->PhysicalDevice;

    // choose swap surface format
    uint32_t formatCount;
    Host_p->Func.vkGetPhysicalDeviceSurfaceFormatsKHR(
        *Device_p, *SurfaceKHR_p, &formatCount, VK_NULL_HANDLE
    ); 

    VkSurfaceFormatKHR *SurfaceFormats_p = Nh_allocate(sizeof(VkSurfaceFormatKHR) * formatCount);
    NH_CHECK_MEM(SurfaceFormats_p)

    Host_p->Func.vkGetPhysicalDeviceSurfaceFormatsKHR(
        *Device_p, *SurfaceKHR_p, &formatCount, SurfaceFormats_p
    ); 
    VkSurfaceFormatKHR SurfaceFormat;
    Nh_Vk_chooseSurfaceFormat(SurfaceFormats_p, &SurfaceFormat);

    // choose present mode
    uint32_t presentModeCount;
    Host_p->Func.vkGetPhysicalDeviceSurfacePresentModesKHR(
        *Device_p, *SurfaceKHR_p, &presentModeCount, VK_NULL_HANDLE
    ); 

    VkPresentModeKHR *presentModes_p = Nh_allocate(sizeof(VkPresentModeKHR) * presentModeCount);
    NH_CHECK_MEM(presentModes_p)

    Host_p->Func.vkGetPhysicalDeviceSurfacePresentModesKHR(
        *Device_p, *SurfaceKHR_p, &presentModeCount, presentModes_p
    ); 
    VkPresentModeKHR presentMode = Nh_Vk_choosePresentMode(presentModes_p, presentModeCount);
    if (presentMode == -1) {NH_DIAGNOSTIC_END(NH_ERROR_BAD_STATE)}

    // choose swapchain extent
    VkSurfaceCapabilitiesKHR surfaceCapabilities;
    Host_p->Func.vkGetPhysicalDeviceSurfaceCapabilitiesKHR(
        *Device_p, *SurfaceKHR_p, &surfaceCapabilities
    );
    VkExtent2D swapExtent = surfaceCapabilities.currentExtent;

    // create swapchain
    VkSwapchainCreateInfoKHR swapchainInfo = 
    {
        .sType                 = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
        .surface               = *SurfaceKHR_p,
        .minImageCount         = Surface_p->imageCount,
        .imageFormat           = SurfaceFormat.format,
        .imageColorSpace       = SurfaceFormat.colorSpace,
        .imageExtent           = swapExtent,
        .imageArrayLayers      = 1,
        .imageUsage            = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT 
                               | VK_IMAGE_USAGE_TRANSFER_SRC_BIT 
                               | VK_IMAGE_USAGE_TRANSFER_DST_BIT,
        .preTransform          = surfaceCapabilities.currentTransform,
        .compositeAlpha        = VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR,
        .presentMode           = presentMode,
        .clipped               = VK_TRUE,
        .queueFamilyIndexCount = 1,
        .pQueueFamilyIndices   = &graphicsQueueFamily
    };

    Driver_p->Func.vkCreateSwapchainKHR(Driver_p->Device, &swapchainInfo, VK_NULL_HANDLE, &Surface_p->SwapchainKHR);
    Surface_p->Extent2D = swapExtent;

    VkSwapchainKHR *VkSwapchainKHR_p = &Surface_p->SwapchainKHR;

    // create swapchain images
    int imageCount = 0;
    Driver_p->Func.vkGetSwapchainImagesKHR(Driver_p->Device, *VkSwapchainKHR_p, &imageCount, VK_NULL_HANDLE); 
   
    if (imageCount != Surface_p->imageCount) {NH_DIAGNOSTIC_END(NH_ERROR_BAD_STATE);}

    VkImage *Images_p = Nh_allocate(sizeof(VkImage) * imageCount);
    NH_CHECK_MEM(Images_p)

    Driver_p->Func.vkGetSwapchainImagesKHR(Driver_p->Device, *VkSwapchainKHR_p, &imageCount, Images_p); 

    // create swapchain image views
    VkImageViewCreateInfo ImageViewInfo = 
    {
        .sType                           = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
        .viewType                        = VK_IMAGE_VIEW_TYPE_2D,
        .format                          = SurfaceFormat.format,
        .components.r                    = VK_COMPONENT_SWIZZLE_IDENTITY,
        .components.g                    = VK_COMPONENT_SWIZZLE_IDENTITY,
        .components.b                    = VK_COMPONENT_SWIZZLE_IDENTITY,
        .components.a                    = VK_COMPONENT_SWIZZLE_IDENTITY,
        .subresourceRange.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT,
        .subresourceRange.baseMipLevel   = 0,
        .subresourceRange.levelCount     = 1,
        .subresourceRange.baseArrayLayer = 0,
        .subresourceRange.layerCount     = 1
    };

    for (int i = 0; i < imageCount; ++i)
    {
        VkImage *tmp_p = &Surface_p->Swapchain.Image_p[i];
        *tmp_p = Images_p[i];
        ImageViewInfo.image = *tmp_p;
        Driver_p->Func.vkCreateImageView(Driver_p->Device, &ImageViewInfo, VK_NULL_HANDLE, &Surface_p->Swapchain.ImageView_p[i]);
    }

    Nh_free(SurfaceFormats_p);
    Nh_free(presentModes_p);
    Nh_free(Images_p);

NH_DIAGNOSTIC_END(NH_SUCCESS)      
}

static NH_RESULT Nh_Vk_createDepthStencil(
    Nh_Vk_Host *Host_p, Nh_Vk_Driver *Driver_p, Nh_Vk_Surface *Surface_p) 
{
NH_BEGIN();

    // prepare
    VkPhysicalDevice *Device_p = &Driver_p->PhysicalDevice;
    VkExtent2D *Extent_p       = &Surface_p->Extent2D;

    VkFormat format = VK_FORMAT_D32_SFLOAT_S8_UINT; // TODO check if available
    VkImageCreateInfo ImageInfo = 
    {
        .sType         = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
        .imageType     = VK_IMAGE_TYPE_2D,
        .format        = format,
        .mipLevels     = 1,
        .arrayLayers   = 1,
        .samples       = VK_SAMPLE_COUNT_1_BIT,
        .tiling        = VK_IMAGE_TILING_OPTIMAL,
        .usage         = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
        .sharingMode   = VK_SHARING_MODE_EXCLUSIVE,
        .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED
    };

    VkExtent3D Extent;
    Extent.width  = Extent_p->width;
    Extent.height = Extent_p->height;
    Extent.depth  = 1;
    ImageInfo.extent = Extent;

    NH_CHECK_VULKAN(Driver_p->Func.vkCreateImage(Driver_p->Device, &ImageInfo, VK_NULL_HANDLE, &Surface_p->DepthStencil.Image))

    NH_Vk_DeviceMemoryInfo MemoryInfo =
    {
        .deviceMemoryType = NH_VK_DEVICE_MEMORY_IMAGE,
        .memoryProperties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
        .bindSource_p     = &Surface_p->DepthStencil.Image
    };

    NH_CHECK(Nh_Vk_createDeviceMemory(Host_p, Driver_p, &MemoryInfo, &Surface_p->DepthStencil.Memory))
    NH_CHECK(Nh_Vk_bindDeviceMemory(Driver_p, &Surface_p->DepthStencil.Memory, &MemoryInfo))

    // create image view
    VkImageViewCreateInfo ImageViewInfo = 
    {
        .sType                           = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
        .image                           = Surface_p->DepthStencil.Image,
        .viewType                        = VK_IMAGE_VIEW_TYPE_2D,
        .format                          = format,
        .subresourceRange.aspectMask     = VK_IMAGE_ASPECT_DEPTH_BIT 
                                         | VK_IMAGE_ASPECT_STENCIL_BIT,
        .subresourceRange.baseMipLevel   = 0,
        .subresourceRange.levelCount     = 1,
        .subresourceRange.baseArrayLayer = 0,
        .subresourceRange.layerCount     = 1
    };
    NH_CHECK_VULKAN(Driver_p->Func.vkCreateImageView(Driver_p->Device, &ImageViewInfo, VK_NULL_HANDLE, &Surface_p->DepthStencil.ImageView))

    // submit pipeline barrier 
    VkCommandBufferAllocateInfo CommandBufferInfo = 
    {
        .sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        .level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
        .commandPool        = Driver_p->GraphicsCommandPools_p[0], // TODO
        .commandBufferCount = 1
    };
    
    VkCommandBuffer CommandBuffer;    
    Driver_p->Func.vkAllocateCommandBuffers(Driver_p->Device, &CommandBufferInfo, &CommandBuffer); 

    VkCommandBufferBeginInfo CommandBufferBeginInfo = 
    {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
        .flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT
    };
    Driver_p->Func.vkBeginCommandBuffer(CommandBuffer, &CommandBufferBeginInfo);
    VkImageMemoryBarrier barrier = 
    {
        .sType                           = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
        .oldLayout                       = VK_IMAGE_LAYOUT_UNDEFINED,
        .newLayout                       = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
        .srcQueueFamilyIndex             = VK_QUEUE_FAMILY_IGNORED,
        .dstQueueFamilyIndex             = VK_QUEUE_FAMILY_IGNORED,
        .image                           = Surface_p->DepthStencil.Image,
        .subresourceRange.aspectMask     = VK_IMAGE_ASPECT_DEPTH_BIT 
                                         | VK_IMAGE_ASPECT_STENCIL_BIT,
        .subresourceRange.baseMipLevel   = 0,
        .subresourceRange.levelCount     = 1,
        .subresourceRange.baseArrayLayer = 0,
        .subresourceRange.layerCount     = 1,
        .srcAccessMask                   = 0,
        .dstAccessMask                   = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT 
                                         | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT
    };
    VkPipelineStageFlags sourceStage;
    VkPipelineStageFlags destinationStage;
    sourceStage      = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
    destinationStage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    Driver_p->Func.vkCmdPipelineBarrier(
        CommandBuffer, sourceStage, destinationStage, 0, 0, VK_NULL_HANDLE, 0, VK_NULL_HANDLE, 1, &barrier
    );
    Driver_p->Func.vkEndCommandBuffer(CommandBuffer);

    VkSubmitInfo submitInfo = 
    {
        .sType              = VK_STRUCTURE_TYPE_SUBMIT_INFO,
        .commandBufferCount = 1,
        .pCommandBuffers    = &CommandBuffer
    };
    
    Driver_p->Func.vkQueueSubmit(Driver_p->GraphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
    Driver_p->Func.vkQueueWaitIdle(Driver_p->GraphicsQueue);

    Driver_p->Func.vkFreeCommandBuffers(Driver_p->Device, Driver_p->GraphicsCommandPools_p[0], 1, &CommandBuffer);

NH_DIAGNOSTIC_END(NH_SUCCESS);
}

static NH_RESULT Nh_Vk_createFramebuffer(
    Nh_Vk_Driver *Driver_p, Nh_Vk_Surface *Surface_p)
{  
NH_BEGIN();
   
    VkFramebufferCreateInfo FramebufferInfo = 
    {
        .sType           = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
        .renderPass      = Driver_p->RenderPass_p[0],
        .flags           = 0,
        .attachmentCount = 2,
        .width           = Surface_p->Extent2D.width,
        .height          = Surface_p->Extent2D.height,
        .layers          = 1
    };

    for (size_t i = 0; i < Surface_p->imageCount; i++) 
    {
        VkImageView *Swapchain_p = &Surface_p->Swapchain.ImageView_p[i]; 
        VkImageView *DepthStencil_p = &Surface_p->DepthStencil.ImageView;

        VkImageView Attachments_p[2] = {*Swapchain_p, *DepthStencil_p};
        FramebufferInfo.pAttachments = Attachments_p;

        NH_CHECK_VULKAN(Driver_p->Func.vkCreateFramebuffer(Driver_p->Device, &FramebufferInfo, VK_NULL_HANDLE, &Surface_p->Framebuffer_p[i]))
    }

NH_DIAGNOSTIC_END(NH_SUCCESS);
}

static NH_RESULT Nh_Vk_createSurface(
    Nh_Vk_Surface *Surface_p, VkSurfaceKHR *SurfaceKHR_p, Nh_Vk_Driver *Driver_p)
{
NH_BEGIN()

    VkBool32 surfaceSupported = VK_FALSE;
    Nh_Vk_getHost()->Func.vkGetPhysicalDeviceSurfaceSupportKHR(
        Driver_p->PhysicalDevice, 0, *SurfaceKHR_p, &surfaceSupported
    ); 
    
    if (!surfaceSupported) {NH_DIAGNOSTIC_END(NH_ERROR_BAD_STATE);}

    VkSurfaceCapabilitiesKHR SurfaceCapabilities;
    Nh_Vk_getHost()->Func.vkGetPhysicalDeviceSurfaceCapabilitiesKHR(
        Driver_p->PhysicalDevice, *SurfaceKHR_p, &SurfaceCapabilities
    );
    
    Surface_p->imageCount = SurfaceCapabilities.minImageCount;
    Surface_p->Framebuffer_p = Nh_allocate(sizeof(VkFramebuffer) * Surface_p->imageCount);
    NH_CHECK_MEM(Surface_p->Framebuffer_p)

    Surface_p->CommandBuffers_p = Nh_allocate(sizeof(VkCommandBuffer) * Surface_p->imageCount);
    NH_CHECK_MEM(Surface_p->CommandBuffers_p)

    Surface_p->Swapchain.Image_p = Nh_allocate(sizeof(VkImage) * Surface_p->imageCount);
    NH_CHECK_MEM(Surface_p->Swapchain.Image_p) 

    Surface_p->Swapchain.ImageView_p = Nh_allocate(sizeof(VkImageView) * Surface_p->imageCount);
    NH_CHECK_MEM(Surface_p->Swapchain.ImageView_p)

    NH_CHECK(Nh_Vk_createSwapchain(Driver_p, SurfaceKHR_p, Surface_p, 0))
    
    // depth/stencil and framebuffer
    NH_CHECK(Nh_Vk_createDepthStencil(Nh_Vk_getHost(), Driver_p, Surface_p))
    NH_CHECK(Nh_Vk_createFramebuffer(Driver_p, Surface_p))

    // synchronisation resources
    VkFenceCreateInfo FenceInfo = {.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO};
    NH_CHECK_VULKAN(Driver_p->Func.vkCreateFence(Driver_p->Device, &FenceInfo, VK_NULL_HANDLE, &Surface_p->Sync.Fence))
    
    VkSemaphoreCreateInfo SemaphoreInfo = {.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO};
    NH_CHECK_VULKAN(Driver_p->Func.vkCreateSemaphore(Driver_p->Device, &SemaphoreInfo, VK_NULL_HANDLE, &Surface_p->Sync.Semaphore_p[0]))
    NH_CHECK_VULKAN(Driver_p->Func.vkCreateSemaphore(Driver_p->Device, &SemaphoreInfo, VK_NULL_HANDLE, &Surface_p->Sync.Semaphore_p[1]))

    // commandbuffers
    VkCommandBufferAllocateInfo AllocateInfo = {
        .sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        .commandPool        = Driver_p->GraphicsCommandPools_p[Nh_getThreadIndex()],
        .level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
        .commandBufferCount = Surface_p->imageCount
    };

    NH_CHECK_VULKAN(Driver_p->Func.vkAllocateCommandBuffers(Driver_p->Device, &AllocateInfo, Surface_p->CommandBuffers_p))

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

NH_RESULT Nh_Vk_createWindowResources(
    Nh_Window *Window_p)
{
NH_BEGIN()

    NH_INIT_LINKED_LIST(Window_p->Vulkan.Surfaces)
    NH_CHECK(Nh_Vk_getSurfaceKHR(Nh_Vk_getHost(), &Window_p->IO, &Window_p->Vulkan.SurfaceKHR))

    for (int i = 0; i < Nh_Vk_getGPUCount(); ++i) 
    { 
        Nh_Vk_Surface *Surface_p = Nh_allocate(sizeof(Nh_Vk_Surface));
        NH_CHECK_NULL(Surface_p)

        NH_CHECK(Nh_Vk_createSurface(Surface_p, &Window_p->Vulkan.SurfaceKHR, &Nh_Vk_getGPU(i)->Driver))
        NH_CHECK(Nh_appendToLinkedList(&Window_p->Vulkan.Surfaces, Surface_p))
    }

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

// DESTROY =========================================================================================

static NH_RESULT Nh_Vk_destroySurface(
    Nh_Vk_Surface *Surface_p, Nh_Vk_Driver *Driver_p)
{
NH_BEGIN()

    Driver_p->Func.vkDestroyFence(Driver_p->Device, Surface_p->Sync.Fence, VK_NULL_HANDLE);
    Driver_p->Func.vkDestroySemaphore(Driver_p->Device, Surface_p->Sync.Semaphore_p[0], VK_NULL_HANDLE);
    Driver_p->Func.vkDestroySemaphore(Driver_p->Device, Surface_p->Sync.Semaphore_p[1], VK_NULL_HANDLE);

    for (int i = 0; i < Surface_p->imageCount; i++) {
        Driver_p->Func.vkDestroyFramebuffer(Driver_p->Device, Surface_p->Framebuffer_p[i], VK_NULL_HANDLE);
        Driver_p->Func.vkDestroyImageView(Driver_p->Device, Surface_p->Swapchain.ImageView_p[i], VK_NULL_HANDLE);
    }

    Driver_p->Func.vkFreeMemory(Driver_p->Device, Surface_p->DepthStencil.Memory, VK_NULL_HANDLE);
    Driver_p->Func.vkDestroyImageView(Driver_p->Device, Surface_p->DepthStencil.ImageView, VK_NULL_HANDLE);
    Driver_p->Func.vkDestroyImage(Driver_p->Device, Surface_p->DepthStencil.Image, VK_NULL_HANDLE);

    Driver_p->Func.vkDestroySwapchainKHR(Driver_p->Device, Surface_p->SwapchainKHR, VK_NULL_HANDLE); // TODO

    Nh_free(Surface_p->Framebuffer_p);
    Nh_free(Surface_p->CommandBuffers_p);
    Nh_free(Surface_p->Swapchain.Image_p);
    Nh_free(Surface_p->Swapchain.ImageView_p);

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

NH_RESULT Nh_Vk_destroyWindowResources(
    Nh_Window *Window_p)
{
NH_BEGIN()

    for (int i = 0; i < Nh_Vk_getGPUCount(); ++i) { 
        NH_CHECK(Nh_Vk_destroySurface(Nh_getFromLinkedList(&Window_p->Vulkan.Surfaces, i), &Nh_Vk_getGPU(i)->Driver))
    }
    Nh_Vk_getHost()->Func.vkDestroySurfaceKHR(Nh_Vk_getHost()->Instance, Window_p->Vulkan.SurfaceKHR, VK_NULL_HANDLE);
    Nh_destroyLinkedList(&Window_p->Vulkan.Surfaces, true);

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

// RESIZE ==========================================================================================

NH_RESULT Nh_Vk_resize(
    Nh_Window *Window_p)
{
NH_BEGIN()

    NH_CHECK(Nh_Vk_destroyWindowResources(Window_p))
    NH_CHECK(Nh_Vk_createWindowResources(Window_p))

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

// PREPARE RENDERING ===============================================================================

NH_RESULT Nh_Vk_prepareRendering(
    Nh_Window *Window_p)
{
NH_BEGIN()

    Nh_LinkedList Acquired;
    NH_INIT_LINKED_LIST(Acquired)

    for (int i = 0; i < Window_p->Tabs.count; ++i)
    {
        Nh_Tab *Tab_p = Nh_getFromLinkedList(&Window_p->Tabs, i); // TODO Multi-GPU support
        Nh_Vk_GPU *GPU_p = Tab_p->GPU.Pointer;

        if (!Nh_inLinkedList(&Acquired, GPU_p)) 
        {
            Nh_Vk_Driver *Driver_p = &GPU_p->Driver;
            Nh_Vk_Surface *Surface_p = Nh_getFromLinkedList(&Window_p->Vulkan.Surfaces, Nh_Vk_getGPUIndex(GPU_p));
            NH_CHECK_NULL(Surface_p)

            VkResult result = Driver_p->Func.vkAcquireNextImageKHR(
                Driver_p->Device, Surface_p->SwapchainKHR, UINT64_MAX, Surface_p->Sync.Semaphore_p[0], 
                VK_NULL_HANDLE, &Surface_p->currentImage
            );
            NH_CHECK_VULKAN(result)

            NH_CHECK(Nh_appendToLinkedList(&Acquired, GPU_p))
        }
    }

    Nh_destroyLinkedList(&Acquired, false);

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

// RENDER ==========================================================================================

static NH_RESULT Nh_Vk_clearWindow(
    Nh_Window *Window_p, Nh_Vk_Driver *Driver_p, Nh_Vk_Surface *Surface_p)
{
NH_BEGIN()

    VkCommandBuffer *CommandBuffer_p = &Surface_p->CommandBuffers_p[Surface_p->currentImage];

    VkCommandBufferBeginInfo CommandBufferInfo = 
    {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
        .flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT
    };

    NH_CHECK_VULKAN(Driver_p->Func.vkBeginCommandBuffer(*CommandBuffer_p, &CommandBufferInfo))

    VkClearColorValue clearColorValue = {{
        Window_p->Settings.BackgroundColor.r, 
        Window_p->Settings.BackgroundColor.g, 
        Window_p->Settings.BackgroundColor.b, 
        Window_p->Settings.BackgroundColor.a
    }};

    VkImageSubresourceRange imageSubresourceRange =
    {
        .aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT,
        .baseMipLevel   = 0,
        .levelCount     = 1,
        .baseArrayLayer = 0,
        .layerCount     = 1
    };

    VkImageMemoryBarrier imageMemoryBarrier = {0};
    imageMemoryBarrier.sType            = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    imageMemoryBarrier.oldLayout        = VK_IMAGE_LAYOUT_UNDEFINED;
    imageMemoryBarrier.newLayout        = VK_IMAGE_LAYOUT_GENERAL;
    imageMemoryBarrier.image            = Surface_p->Swapchain.Image_p[Surface_p->currentImage];
    imageMemoryBarrier.subresourceRange = imageSubresourceRange;
    imageMemoryBarrier.srcAccessMask    = VK_ACCESS_SHADER_WRITE_BIT;
    imageMemoryBarrier.dstAccessMask    = VK_ACCESS_SHADER_WRITE_BIT;

    Driver_p->Func.vkCmdPipelineBarrier(
        *CommandBuffer_p, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
        0, 0, VK_NULL_HANDLE, 0, VK_NULL_HANDLE, 1, &imageMemoryBarrier
    );

    Driver_p->Func.vkCmdClearColorImage(
        *CommandBuffer_p, Surface_p->Swapchain.Image_p[Surface_p->currentImage],
        VK_IMAGE_LAYOUT_GENERAL, &clearColorValue, 1, &imageSubresourceRange
    );

    Driver_p->Func.vkEndCommandBuffer(*CommandBuffer_p);

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

NH_RESULT Nh_Vk_render( // TODO multi GPU rendering
    Nh_Window *Window_p)
{
NH_BEGIN()

#define FREE() Nh_free(Commands_p); Nh_free(SortedCommands_p); Nh_free(onTop_p); Nh_free(sortedIndices_p);

    Nh_Vk_GPU *GPU_p = Nh_getTab(Window_p, 0)->GPU.Pointer; // TODO Multi-GPU support

    Nh_Vk_Surface *Surface_p = Nh_getFromLinkedList(&Window_p->Vulkan.Surfaces, 0);
    VkSemaphore *Semaphores_pp[2] = {&Surface_p->Sync.Semaphore_p[0], &Surface_p->Sync.Semaphore_p[1]};

    Nh_LinkedList Render;
    NH_INIT_LINKED_LIST(Render)
    
    // find out which commandbuffers to render
    for (int i = 0; i < Window_p->Tabs.count; ++i) {
        if (Nh_getTab(Window_p, i)->Sync.render) {
            NH_CHECK(Nh_appendToLinkedList(&Render, Nh_getTab(Window_p, i)))
        }
    }

    VkCommandBuffer *Commands_p = Nh_allocate(sizeof(void*) * Render.count);
    NH_CHECK_MEM(Commands_p)

    VkCommandBuffer *SortedCommands_p = Nh_allocate(sizeof(void*) * (Render.count + 1));
    NH_CHECK_MEM(SortedCommands_p)

    int *sortedIndices_p = Nh_allocate(sizeof(int) * Render.count);
    NH_CHECK_MEM(sortedIndices_p)

    NH_BOOL *onTop_p = Nh_allocate(sizeof(NH_BOOL) * Render.count);
    NH_CHECK_MEM(onTop_p)

    // insert clear commandbuffer
    NH_CHECK(Nh_Vk_clearWindow(Window_p, &GPU_p->Driver, Surface_p))
    SortedCommands_p[0] = Surface_p->CommandBuffers_p[Surface_p->currentImage];

    // get commandbuffers
    for (int i = 0; i < Render.count; ++i) 
    {
        Nh_Tab *Tab_p = Nh_getFromLinkedList(&Render, i);
        Tab_p->Sync.submittedBuffers_p[Surface_p->currentImage]
            = Tab_p->Sync.newestBuffers_p[Surface_p->currentImage];

        Nh_syncUseCommandBuffer(Tab_p);

        Commands_p[i] = Tab_p->Vulkan.CommandBuffers_p[
            Tab_p->Sync.submittedBuffers_p[
                Surface_p->currentImage
            ]
        ];
    }

    // sort commandbuffers based on tab priority (from low to high priority)
    int max = 0;
    for (int i = 0; i < Render.count; ++i) {
        Nh_Tab *Tab_p = Nh_getFromLinkedList(&Render, i);
        if (Tab_p->Settings.priority > max) {max = Tab_p->Settings.priority;}
    }
    
    for (int i = 0; i < Render.count; ++i) 
    {
        int diff = max, newMax = -1;
        for (int j = 0; j < Render.count; ++j) 
        {
            Nh_Tab *Tab_p = Nh_getFromLinkedList(&Render, j);
            if ((Tab_p->Settings.priority - max) == 0) {sortedIndices_p[i] = j;}
            else if ((max - Tab_p->Settings.priority) <= diff) {
                diff = max - Tab_p->Settings.priority; 
                newMax = Tab_p->Settings.priority;
            }
        }
        max = newMax;
    }

    for (int i = 0; i < Render.count; ++i) {
        onTop_p[sortedIndices_p[i]] = Nh_getTab(Window_p, sortedIndices_p[i])->Flags.alwaysOnTop;
    }

    int count = 1;
    for (int i = 0; i < Render.count; ++i) {
        if (!onTop_p[sortedIndices_p[i]]){SortedCommands_p[count++] = Commands_p[sortedIndices_p[i]];}
    }
    for (int i = 0; i < Render.count; ++i) {
        if (onTop_p[sortedIndices_p[i]]){SortedCommands_p[count++] = Commands_p[sortedIndices_p[i]];}
    }

    // submit graphics
    VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    VkSubmitInfo submitInfo = 
    {
        .sType                = VK_STRUCTURE_TYPE_SUBMIT_INFO,
        .waitSemaphoreCount   = 1,
        .signalSemaphoreCount = 0,
        .pWaitSemaphores      = Semaphores_pp[0], 
        .pWaitDstStageMask    = waitStages,
        .pSignalSemaphores    = VK_NULL_HANDLE,
        .commandBufferCount   = count,
        .pCommandBuffers      = SortedCommands_p,
    };

    NH_CHECK_VULKAN(GPU_p->Driver.Func.vkQueueSubmit(GPU_p->Driver.GraphicsQueue, 1, &submitInfo, Surface_p->Sync.Fence))
    GPU_p->Driver.Func.vkWaitForFences(GPU_p->Driver.Device, 1, &Surface_p->Sync.Fence, VK_TRUE, UINT64_MAX);
    GPU_p->Driver.Func.vkResetFences(GPU_p->Driver.Device, 1, &Surface_p->Sync.Fence); 

    // present graphics
    VkPresentInfoKHR presentInfo =
    {
        .sType              = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
        .swapchainCount     = 1,
        .pSwapchains        = &Surface_p->SwapchainKHR,
        .pWaitSemaphores    = VK_NULL_HANDLE,
        .waitSemaphoreCount = 0,
        .pImageIndices      = &Surface_p->currentImage
    };
    
    NH_CHECK_VULKAN(GPU_p->Driver.Func.vkQueuePresentKHR(GPU_p->Driver.GraphicsQueue, &presentInfo))

    Nh_free(Commands_p); 
    Nh_free(SortedCommands_p); 
    Nh_free(onTop_p); 
    Nh_free(sortedIndices_p);
    Nh_destroyLinkedList(&Render, false);

#undef FREE

NH_DIAGNOSTIC_END(NH_SUCCESS)
}


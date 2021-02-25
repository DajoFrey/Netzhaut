// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "../Header/Record.h"
#include "../Header/Driver.h"
#include "../Header/Window.h"
#include "../Header/Buffer.h"
#include "../Header/Descriptor.h"
#include "../Header/GPU.h"
#include "../Header/Util.h"
#include "../Header/Vulkan.h"

#include "../../Main/Header/Macros.h"

#include "../../../Netzhaut/Sync.h"
#include "../../../Netzhaut/Callbacks.h"
#include "../../../Netzhaut/Memory.h"

#include "../../../HTML/Main/Header/Box.h"

#include NH_FLOW
#include NH_DEFAULT_CHECK
#include NH_HTML_UTILS
#include NH_CSS_UTILS

#include <string.h>

// DECLARE =========================================================================================

static inline void Nh_Vk_recordExternal(
    VkCommandBuffer *CommandBuffer_p, VkViewport Viewport, Nh_Vk_Driver *Driver_p, Nh_Content *Content_p, 
    Nh_Vk_Pipeline *Pipelines_p, Nh_HTML_Node *Node_p
);
static inline void Nh_Vk_recordInternal(
    VkCommandBuffer *CommandBuffer_p, VkViewport *Viewport_p, Nh_Vk_Driver *Driver_p, 
    Nh_Content *Content_p, Nh_Vk_Pipeline *Pipelines_p, Nh_HTML_Node *Node_p
);

// HELPER ==========================================================================================

static VkRect2D Nh_Vk_getDefaultScissor(
    Nh_Content *Content_p)
{
NH_BEGIN()

    VkOffset2D Offset = {
        .x = 0,
        .y = 0
    };
    VkExtent2D Extent = {
        .width  = Content_p->Tab_p->Size.width,
        .height = Content_p->Tab_p->Size.height,
    };
 
    VkRect2D Scissor = 
    { 
        .offset = Offset,
        .extent = Extent 
    };

NH_END(Scissor)
}

// RESOURCES =======================================================================================

NH_RESULT Nh_Vk_createRecordResources(
    Nh_Tab *Tab_p)
{
NH_BEGIN()

    Nh_Vk_GPU *GPU_p = Tab_p->GPU.Pointer;

    VkCommandBufferAllocateInfo AllocateInfo = {
        .sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        .commandPool        = GPU_p->Driver.GraphicsCommandPools_p[Nh_getThreadIndex()],  
        .level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
        .commandBufferCount = 6
    };

    NH_CHECK_VULKAN(GPU_p->Driver.Func.vkAllocateCommandBuffers(
        GPU_p->Driver.Device, &AllocateInfo, Tab_p->Vulkan.CommandBuffers_p
    ))

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

void Nh_Vk_destroyRecordResources(
    Nh_Tab *Tab_p)
{
NH_BEGIN()

    Nh_Vk_GPU *GPU_p = Tab_p->GPU.Pointer;
  
    GPU_p->Driver.Func.vkFreeCommandBuffers(
        GPU_p->Driver.Device, 
        GPU_p->Driver.GraphicsCommandPools_p[Nh_getThreadIndex()], 
        6,
        Tab_p->Vulkan.CommandBuffers_p
    );

NH_SILENT_END()
}

// RECORD ==========================================================================================

static NH_RESULT Nh_Vk_recordHTMLDocument(
    VkCommandBuffer *CommandBuffer_p, VkViewport Viewport, Nh_Vk_Driver *Driver_p, 
    Nh_HTML_Document *Document_p, Nh_Vk_Pipeline *Pipelines_p)
{
NH_BEGIN()

    bool *inFlow_p = Nh_allocate(sizeof(bool) * (Document_p->Tree.RenderNoChars.last + 1));
    NH_CHECK_MEM(inFlow_p);
    memset(inFlow_p, true, sizeof(bool) * (Document_p->Tree.RenderNoChars.last + 1));

    for (int i = 0; i <= Document_p->Tree.RenderNoChars.last; ++i)
    {
        Nh_HTML_Node *Node_p = Document_p->Tree.RenderNoChars.data_pp[i];
        if (!Nh_HTML_isInFlow(Node_p)) {inFlow_p[i] = false; continue;}

        Nh_Vk_recordExternal(CommandBuffer_p, Viewport, Driver_p, Document_p->Content_p, Pipelines_p, Node_p);
        Nh_Vk_recordInternal(CommandBuffer_p, &Viewport, Driver_p, Document_p->Content_p, Pipelines_p, Node_p);
    }

    for (int i = 0; i <= Document_p->Tree.RenderNoChars.last; ++i)
    {
        Nh_HTML_Node *Node_p = Document_p->Tree.RenderNoChars.data_pp[i];

        if (inFlow_p[i] == false)
        {
            Nh_Vk_recordExternal(CommandBuffer_p, Viewport, Driver_p, Document_p->Content_p, Pipelines_p, Node_p);
            Nh_Vk_recordInternal(CommandBuffer_p, &Viewport, Driver_p, Document_p->Content_p, Pipelines_p, Node_p);
        }
    }

    Nh_free(inFlow_p);

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

static NH_RESULT Nh_Vk_recordContent(
    VkCommandBuffer *CommandBuffer_p, Nh_Vk_Driver *Driver_p, Nh_Vk_Surface *Surface_p, Nh_Content *Content_p, 
    Nh_Vk_Pipeline *Pipelines_p, int imageIndex)
{
NH_BEGIN()

    VkClearColorValue clearColorValue = {{
        Content_p->Tab_p->Settings.BackgroundColor.r,
        Content_p->Tab_p->Settings.BackgroundColor.g,
        Content_p->Tab_p->Settings.BackgroundColor.b,
        Content_p->Tab_p->Settings.BackgroundColor.a
    }};
    VkClearDepthStencilValue depthStencilValue =
    {
        .depth   = 1.0f,
        .stencil = 0
    };
    VkClearValue clearValues[2];
    clearValues[0].color        = clearColorValue;
    clearValues[1].depthStencil = depthStencilValue;

    VkRenderPassBeginInfo RenderPassInfo =
    {
        .sType                    = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
        .renderPass               = Driver_p->RenderPass_p[0],
        .framebuffer              = Surface_p->Framebuffer_p[imageIndex],
        .renderArea.offset.x      = 0,
        .renderArea.offset.y      = 0,
        .renderArea.extent.width  = Surface_p->Extent2D.width,
        .renderArea.extent.height = Surface_p->Extent2D.height,
        .clearValueCount          = 2,
        .pClearValues             = clearValues
    };

    Driver_p->Func.vkCmdBeginRenderPass(*CommandBuffer_p, &RenderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

    VkViewport Viewport = 
    { 
        .x        = Content_p->Tab_p->Position.x, 
        .y        = Content_p->Tab_p->Position.y, 
        .width    = Content_p->Tab_p->Size.width, 
        .height   = Content_p->Tab_p->Size.height, 
        .minDepth = 0.0f, 
        .maxDepth = 1.0f 
    };

    VkRect2D Scissor = Nh_Vk_getDefaultScissor(Content_p); 

    Driver_p->Func.vkCmdSetViewport(*CommandBuffer_p, 0, 1, &Viewport);
    Driver_p->Func.vkCmdSetScissor(*CommandBuffer_p, 0, 1, &Scissor);

    VkClearAttachment ClearAttachment = {
        .aspectMask      = VK_IMAGE_ASPECT_COLOR_BIT,
        .colorAttachment = 0,
        .clearValue      = clearValues[0],
    };

    Scissor.offset.x = Content_p->Tab_p->Position.x;
    Scissor.offset.y = Content_p->Tab_p->Position.y;

    VkClearRect ClearRect = { 
        .rect           = Scissor,
        .baseArrayLayer = 0,
        .layerCount     = 1,
    };
    Driver_p->Func.vkCmdClearAttachments(*CommandBuffer_p, 1, &ClearAttachment, 1, &ClearRect);

    switch (Content_p->type)
    {
        case NH_CONTENT_HTML_DOCUMENT : 
            Nh_Vk_recordHTMLDocument(CommandBuffer_p, Viewport, Driver_p, Content_p->data_p, Pipelines_p);
            break;
        default : NH_DIAGNOSTIC_END(NH_ERROR_BAD_STATE)
    }    

    Driver_p->Func.vkCmdEndRenderPass(*CommandBuffer_p);

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

NH_RESULT Nh_Vk_record(
    Nh_Vk_GPU *GPU_p, Nh_Tab *Tab_p, int bufferIndex, int imageIndex)
{
NH_BEGIN()

    VkCommandBuffer *CommandBuffer_p = &Tab_p->Vulkan.CommandBuffers_p[bufferIndex];
    GPU_p->Driver.Func.vkResetCommandBuffer(*CommandBuffer_p, VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT);

    VkCommandBufferBeginInfo CommandBufferInfo = 
    {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
        .flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT
    };

    NH_CHECK_VULKAN(GPU_p->Driver.Func.vkBeginCommandBuffer(*CommandBuffer_p, &CommandBufferInfo))

    for (int i = 0; i < Tab_p->Contents.count; ++i) 
    {
        Nh_Content *Content_p = Nh_getFromLinkedList(&Tab_p->Contents, i);
        if (!Content_p->Flags.visible) {continue;}

        NH_CHECK(Nh_Vk_recordContent(
            CommandBuffer_p, &GPU_p->Driver, Nh_getFromLinkedList(&Tab_p->Window_p->Vulkan.Surfaces, Nh_Vk_getGPUIndex(GPU_p)), 
            Content_p, GPU_p->Pipelines_p, imageIndex
        ))
    }

    GPU_p->Driver.Func.vkEndCommandBuffer(*CommandBuffer_p);

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

// TEXTURE =========================================================================================

static NH_BOOL Nh_Vk_recordImage(
    Nh_Content *Content_p, Nh_HTML_Node *Node_p, Nh_Vk_Pipeline *Pipelines_p, Nh_Vk_Driver *Driver_p,
    VkCommandBuffer *CommandBuffer_p)
{
NH_BEGIN()

    if (Node_p->tag == NH_HTML_TAG_IMG && Nh_Gfx_textureLoaded(Content_p, Nh_HTML_getSrc(Node_p), NULL))
    {
        VkDeviceSize size_p[1] = {0};

        Driver_p->Func.vkCmdBindPipeline(
            *CommandBuffer_p, VK_PIPELINE_BIND_POINT_GRAPHICS, 
            Pipelines_p[NH_VK_PIPELINE_IMAGE].Pipeline
        );
        
        Driver_p->Func.vkCmdBindDescriptorSets(
            *CommandBuffer_p, VK_PIPELINE_BIND_POINT_GRAPHICS, 
            Pipelines_p[NH_VK_PIPELINE_IMAGE].PipelineLayout, 0, 1, 
            &Nh_Vk_getDescriptor(Node_p, NH_VK_DESCRIPTOR_IMAGE)->DescriptorSet,
            0, VK_NULL_HANDLE
        );

        Driver_p->Func.vkCmdBindVertexBuffers(
            *CommandBuffer_p, 0, 1, 
            &Nh_Vk_getBuffer(Node_p, NH_VK_BUFFER_TEXTURE)->Buffer, 
            size_p
        );
        
        Driver_p->Func.vkCmdDraw(*CommandBuffer_p, Nh_Vk_getBuffer(Node_p, NH_VK_BUFFER_TEXTURE)->size / 3, 1, 0, 0);

        NH_END(NH_TRUE)
    }

NH_END(NH_FALSE)
}

// BACKGROUND ======================================================================================

static void Nh_Vk_recordBackground(
    Nh_HTML_Node *Node_p, Nh_Vk_Pipeline *Pipelines_p, Nh_Vk_Driver *Driver_p,
    VkCommandBuffer *CommandBuffer_p)
{
NH_BEGIN()

    VkDeviceSize size_p[1] = {0};

    Driver_p->Func.vkCmdBindPipeline(
        *CommandBuffer_p, VK_PIPELINE_BIND_POINT_GRAPHICS, 
        Pipelines_p[NH_VK_PIPELINE_COLOR].Pipeline
    );
    
    Driver_p->Func.vkCmdBindDescriptorSets(
        *CommandBuffer_p, VK_PIPELINE_BIND_POINT_GRAPHICS, 
        Pipelines_p[NH_VK_PIPELINE_COLOR].PipelineLayout, 0, 1, 
        &Nh_Vk_getDescriptor(Node_p, NH_VK_DESCRIPTOR_BACKGROUND)->DescriptorSet,
        0, VK_NULL_HANDLE
    );
    
    Driver_p->Func.vkCmdBindVertexBuffers(
        *CommandBuffer_p, 0, 1, 
        &Nh_Vk_getBuffer(Node_p, NH_VK_BUFFER_BACKGROUND)->Buffer, 
        size_p
    );
    
    Driver_p->Func.vkCmdDraw(*CommandBuffer_p, Nh_Vk_getBuffer(Node_p, NH_VK_BUFFER_BACKGROUND)->size / 3, 1, 0, 0);

NH_SILENT_END()
}

// BACKGROUND IMAGE ================================================================================

static void Nh_Vk_recordBackgroundImage(
    Nh_Content *Content_p, Nh_HTML_Node *Node_p, Nh_Vk_Pipeline *Pipelines_p, Nh_Vk_Driver *Driver_p, 
    VkCommandBuffer *CommandBuffer_p, VkViewport *Viewport_p)
{
NH_BEGIN()

    for (int i = 0; i < Node_p->Properties.Background.Images.count; ++i)
    {
        NH_CSS_Image *Image_p = Nh_CSS_getImage(&Node_p->Properties.Background.Images, i);

        Nh_HTML_PixelBox Box = Nh_HTML_getBackgroundImageBox(Content_p, Node_p, Image_p, 0.0f);

        VkOffset2D Offset =
        {
            .x = Box.Position.x,
            .y = Box.Position.y,
        };
        VkExtent2D Extent = 
        {
            .width  = Box.Size.width,
            .height = Box.Size.height,
        };
        VkRect2D Scissor = 
        { 
            .offset = Offset,
            .extent = Extent,
        };

        Driver_p->Func.vkCmdSetScissor(*CommandBuffer_p, 0, 1, &Scissor);

        Driver_p->Func.vkCmdBindPipeline(
            *CommandBuffer_p, VK_PIPELINE_BIND_POINT_GRAPHICS, 
            Pipelines_p[NH_VK_PIPELINE_BACKGROUND_IMAGE].Pipeline
        );
        
        Driver_p->Func.vkCmdBindDescriptorSets(
            *CommandBuffer_p, VK_PIPELINE_BIND_POINT_GRAPHICS, 
            Pipelines_p[NH_VK_PIPELINE_BACKGROUND_IMAGE].PipelineLayout, 0, 1, 
            &Nh_Vk_getDescriptorFromIndex(Node_p, Image_p->descriptor)->DescriptorSet,
            0, VK_NULL_HANDLE
        );
       
        VkDeviceSize size_p[1] = {0};

        Driver_p->Func.vkCmdBindVertexBuffers(
            *CommandBuffer_p, 0, 1, 
            &Nh_Vk_getBufferFromIndex(Node_p, Image_p->buffer)->Buffer, 
            size_p
        );

        Driver_p->Func.vkCmdDraw(*CommandBuffer_p, Nh_Vk_getBufferFromIndex(Node_p, Image_p->buffer)->size / 3, 1, 0, 0);

        Scissor = Nh_Vk_getDefaultScissor(Content_p); 
        Driver_p->Func.vkCmdSetScissor(*CommandBuffer_p, 0, 1, &Scissor);
    }

NH_SILENT_END()
}

// TEXT ============================================================================================

static void Nh_Vk_recordText(
    Nh_HTML_Node *Node_p, Nh_Vk_Pipeline *Pipelines_p, Nh_Vk_Driver *Driver_p,
    VkCommandBuffer *CommandBuffer_p)
{
NH_BEGIN()

    if (Nh_HTML_isVisibleTextNode(Node_p))
    {
        Driver_p->Func.vkCmdBindPipeline(
            *CommandBuffer_p, VK_PIPELINE_BIND_POINT_GRAPHICS, 
            Pipelines_p[NH_VK_PIPELINE_TEXT_SDF].Pipeline
        );
        VkDeviceSize size_p[1] = {0};

        Driver_p->Func.vkCmdBindVertexBuffers(
            *CommandBuffer_p, 0, 1,
            &(Nh_Vk_getBuffer(Node_p, NH_VK_BUFFER_TEXT_SDF)->Buffer), 
            size_p
        );
        
        Driver_p->Func.vkCmdBindDescriptorSets(
            *CommandBuffer_p, VK_PIPELINE_BIND_POINT_GRAPHICS, 
            Pipelines_p[NH_VK_PIPELINE_TEXT_SDF].PipelineLayout, 
            0, 1, 
            &(Nh_Vk_getDescriptor(Node_p, NH_VK_DESCRIPTOR_TEXT_SDF)->DescriptorSet),
            0, VK_NULL_HANDLE
        );

        Driver_p->Func.vkCmdBindIndexBuffer(
            *CommandBuffer_p, 
            Nh_Vk_getBuffer(Node_p, NH_VK_BUFFER_TEXT_SDF_INDEX)->Buffer, 
            0, VK_INDEX_TYPE_UINT32
        );

        Driver_p->Func.vkCmdDrawIndexed(
            *CommandBuffer_p, strlen(Node_p->text_p) * 6, 1, 0, 0, 0
        );
    }

NH_SILENT_END()
}

// BORDER ==========================================================================================

static void Nh_Vk_recordBorder(
    Nh_HTML_Node *Node_p, Nh_Vk_Pipeline *Pipelines_p, Nh_Vk_Driver *Driver_p,
    VkCommandBuffer *CommandBuffer_p)
{
NH_BEGIN()

    if (NH_CSS_SHOW_TOP_BORDER(Node_p))
    {
        Driver_p->Func.vkCmdBindPipeline(
            *CommandBuffer_p, VK_PIPELINE_BIND_POINT_GRAPHICS, 
            Pipelines_p[NH_VK_PIPELINE_COLOR].Pipeline
        );
        VkDeviceSize size_p[1] = {0};
        
        Driver_p->Func.vkCmdBindDescriptorSets(
            *CommandBuffer_p, VK_PIPELINE_BIND_POINT_GRAPHICS, 
            Pipelines_p[NH_VK_PIPELINE_COLOR].PipelineLayout, 
            0, 1, 
            &Nh_Vk_getDescriptor(Node_p, NH_VK_DESCRIPTOR_TOP_BORDER)->DescriptorSet,
            0, VK_NULL_HANDLE
        );
        
        Driver_p->Func.vkCmdBindVertexBuffers(
            *CommandBuffer_p, 0, 1, 
            &Nh_Vk_getBuffer(Node_p, NH_VK_BUFFER_TOP_BORDER_VERTICES)->Buffer, 
            size_p
        );
        Driver_p->Func.vkCmdDraw(*CommandBuffer_p, Nh_Vk_getBuffer(Node_p, NH_VK_BUFFER_TOP_BORDER_VERTICES)->size / 3, 1, 0, 0);
    }
    if (NH_CSS_SHOW_RIGHT_BORDER(Node_p))
    {
         Driver_p->Func.vkCmdBindPipeline(
            *CommandBuffer_p, VK_PIPELINE_BIND_POINT_GRAPHICS, 
            Pipelines_p[NH_VK_PIPELINE_COLOR].Pipeline
        );
        VkDeviceSize size_p[1] = {0};
        
        Driver_p->Func.vkCmdBindDescriptorSets(
            *CommandBuffer_p, VK_PIPELINE_BIND_POINT_GRAPHICS, 
            Pipelines_p[NH_VK_PIPELINE_COLOR].PipelineLayout, 
            0, 1, 
            &Nh_Vk_getDescriptor(Node_p, NH_VK_DESCRIPTOR_RIGHT_BORDER)->DescriptorSet,
            0, VK_NULL_HANDLE
        );
        
        Driver_p->Func.vkCmdBindVertexBuffers(
            *CommandBuffer_p, 0, 1, 
            &Nh_Vk_getBuffer(Node_p, NH_VK_BUFFER_RIGHT_BORDER_VERTICES)->Buffer, 
            size_p
        );
        Driver_p->Func.vkCmdDraw(*CommandBuffer_p, Nh_Vk_getBuffer(Node_p, NH_VK_BUFFER_RIGHT_BORDER_VERTICES)->size / 3, 1, 0, 0);
   
    }
    if (NH_CSS_SHOW_BOTTOM_BORDER(Node_p))
    {
        Driver_p->Func.vkCmdBindPipeline(
            *CommandBuffer_p, VK_PIPELINE_BIND_POINT_GRAPHICS, 
            Pipelines_p[NH_VK_PIPELINE_COLOR].Pipeline
        );
        VkDeviceSize size_p[1] = {0};
        
        Driver_p->Func.vkCmdBindDescriptorSets(
            *CommandBuffer_p, VK_PIPELINE_BIND_POINT_GRAPHICS, 
            Pipelines_p[NH_VK_PIPELINE_COLOR].PipelineLayout, 
            0, 1, 
            &Nh_Vk_getDescriptor(Node_p, NH_VK_DESCRIPTOR_BOTTOM_BORDER)->DescriptorSet,
            0, VK_NULL_HANDLE
        );
        
        Driver_p->Func.vkCmdBindVertexBuffers(
            *CommandBuffer_p, 0, 1, 
            &Nh_Vk_getBuffer(Node_p, NH_VK_BUFFER_BOTTOM_BORDER_VERTICES)->Buffer, 
            size_p
        );
        Driver_p->Func.vkCmdDraw(*CommandBuffer_p, Nh_Vk_getBuffer(Node_p, NH_VK_BUFFER_BOTTOM_BORDER_VERTICES)->size / 3, 1, 0, 0);

    }
    if (NH_CSS_SHOW_LEFT_BORDER(Node_p))
    {
        Driver_p->Func.vkCmdBindPipeline(
            *CommandBuffer_p, VK_PIPELINE_BIND_POINT_GRAPHICS, 
            Pipelines_p[NH_VK_PIPELINE_COLOR].Pipeline
        );
        VkDeviceSize size_p[1] = {0};
        
        Driver_p->Func.vkCmdBindDescriptorSets(
            *CommandBuffer_p, VK_PIPELINE_BIND_POINT_GRAPHICS, 
            Pipelines_p[NH_VK_PIPELINE_COLOR].PipelineLayout, 
            0, 1, 
            &Nh_Vk_getDescriptor(Node_p, NH_VK_DESCRIPTOR_LEFT_BORDER)->DescriptorSet,
            0, VK_NULL_HANDLE
        );
        
        Driver_p->Func.vkCmdBindVertexBuffers(
            *CommandBuffer_p, 0, 1, 
            &Nh_Vk_getBuffer(Node_p, NH_VK_BUFFER_LEFT_BORDER_VERTICES)->Buffer, 
            size_p
        );
        Driver_p->Func.vkCmdDraw(*CommandBuffer_p, Nh_Vk_getBuffer(Node_p, NH_VK_BUFFER_LEFT_BORDER_VERTICES)->size / 3, 1, 0, 0);
    }

NH_SILENT_END()
}

// CANVAS RENDERING CONTEXT 2D =====================================================================

//static void Nh_Vk_recordCanvasRenderingContext2D(
//    Nh_Content *Content_p, Nh_HTML_Node *Node_p, Nh_Vk_Pipeline *Pipelines_p, Nh_Vk_Driver *Driver_p, 
//    VkCommandBuffer *CommandBuffer_p, NH_JS_CanvasRenderingContext2D *Context_p)
//{
//NH_BEGIN()
//
//    int count = 0;
//
//    for (int i = 0; i < Context_p->Paths.count; ++i)
//    {
//        NH_JS_Path2D *Path_p = Nh_getFromLinkedList(&Context_p->Paths, i);
// 
//        for (int j = 0; j < Path_p->Commands.count; ++j)
//        {
//            NH_JS_Path2DCommand *Command_p = Nh_getFromLinkedList(&Path_p->Commands, j);
//            switch (Command_p->type)
//            {
//                case NH_JS_PATH_2D_COMMAND_ARC :
//
//                    Driver_p->Func.vkCmdBindPipeline(
//                        *CommandBuffer_p, VK_PIPELINE_BIND_POINT_GRAPHICS, 
//                        Pipelines_p[NH_VK_PIPELINE_CANVAS_RENDERING_CONTEXT_2D_ARC].Pipeline
//                    );
//                    
//                    Driver_p->Func.vkCmdBindDescriptorSets(
//                        *CommandBuffer_p, VK_PIPELINE_BIND_POINT_GRAPHICS, 
//                        Pipelines_p[NH_VK_PIPELINE_CANVAS_RENDERING_CONTEXT_2D_ARC].PipelineLayout, 
//                        0, 1, 
//                        &((NH_Vk_DescriptorSet*)Nh_getFromLinkedList(&Context_p->Vulkan.Descriptors, count))->DescriptorSet,
//                        0, VK_NULL_HANDLE
//                    );
//
//                    VkDeviceSize size_p[1] = {0};
//                    Driver_p->Func.vkCmdBindVertexBuffers(
//                        *CommandBuffer_p, 0, 1, 
//                        &Nh_Vk_getBuffer(Node_p, NH_VK_BUFFER_TEXTURE)->Buffer, 
//                        size_p
//                    );
//                    Driver_p->Func.vkCmdDraw(*CommandBuffer_p, 6, 1, 0, 0);
//           
//                    break;
//            }
//            count++;
//        }
//    }
//
//NH_SILENT_END()
//}

// INTERNAL / EXTERNAL =============================================================================

static inline void Nh_Vk_recordExternal(
    VkCommandBuffer *CommandBuffer_p, VkViewport Viewport, Nh_Vk_Driver *Driver_p, Nh_Content *Content_p, 
    Nh_Vk_Pipeline *Pipelines_p, Nh_HTML_Node *Node_p)
{
NH_BEGIN()

    Nh_VulkanCallback Callback = Nh_getVulkanCallback(&Content_p->Callbacks, Nh_HTML_getId(Node_p));
    
    if (Callback != NULL) 
    {
//        Nh_Vk_recordBackground(Node_p, Pipelines_p, Driver_p, CommandBuffer_p);
//
//        Nh_HTML_ClipBox ContentBox;
//        Nh_CSS_getContentPixelBox(Content_p, Node_p, &ContentBox, NULL);
//
//        int offset_p[2], size_p[2];
//        Nh_CSS_getBoxOffsetRelToWindow(Content_p, &ContentBox, offset_p);
//        Nh_CSS_getBoxSize(Content_p, &ContentBox, offset_p);
//
//        VkViewport Viewport2 = 
//        { 
//            .x        = offset_p[0], 
//            .y        = offset_p[1], 
//            .width    = size_p[0], 
//            .height   = size_p[1], 
//            .minDepth = 0.0f, 
//            .maxDepth = 1.0f 
//        };
//        Driver_p->Func.vkCmdSetViewport(*CommandBuffer_p, 0, 1, &Viewport2);
//
//        Callback(Driver_p->Func, CommandBuffer_p);
//        
//        Driver_p->Func.vkCmdSetViewport(*CommandBuffer_p, 0, 1, &Viewport);
    }

NH_SILENT_END()
}

static inline void Nh_Vk_recordInternal(
    VkCommandBuffer *CommandBuffer_p, VkViewport *Viewport_p, Nh_Vk_Driver *Driver_p, 
    Nh_Content *Content_p, Nh_Vk_Pipeline *Pipelines_p, Nh_HTML_Node *Node_p)
{
NH_BEGIN()

    Nh_VulkanCallback Callback = Nh_getVulkanCallback(&Content_p->Callbacks, Nh_HTML_getId(Node_p));
    if (Callback != NULL) {NH_SILENT_END()}

    if (!Nh_Vk_recordImage(Content_p, Node_p, Pipelines_p, Driver_p, CommandBuffer_p)) {
        Nh_Vk_recordBackground(Node_p, Pipelines_p, Driver_p, CommandBuffer_p);
    }
//    Nh_Vk_recordBackgroundImage(Content_p, Node_p, Pipelines_p, Driver_p, CommandBuffer_p, Viewport_p);

    Nh_Vk_recordText(Node_p, Pipelines_p, Driver_p, CommandBuffer_p);
    Nh_Vk_recordBorder(Node_p, Pipelines_p, Driver_p, CommandBuffer_p);

//        for (int i = 0; i < Node_p->Injections.count; ++i) 
//        {
//            NH_JS_Injection *Injection_p = Nh_getFromLinkedList(&Node_p->Injections, i);
//            switch (Injection_p->type)
//            {
//                case NH_JS_INJECTION_CANVAS_RENDERING_CONTEXT_2D :
//                    Nh_Vk_recordCanvasRenderingContext2D(
//                        Content_p, Node_p, Pipelines_p, Driver_p, CommandBuffer_p, Injection_p->data_p
//                    );
//                    break;
//            }
//        }

NH_SILENT_END()
}


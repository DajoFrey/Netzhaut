// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "../Header/Node.h"
#include "../Header/Driver.h"
#include "../Header/Descriptor.h"
#include "../Header/Uniform.h"
#include "../Header/Pipeline.h"
#include "../Header/Buffer.h"
#include "../Header/Text.h"
#include "../Header/Texture.h"
#include "../Header/GPU.h"
#include "../Header/Util.h"

#include "../../Main/Header/Macros.h"

#include NH_FLOW
#include NH_DEFAULT_CHECK
#include NH_HTML_UTILS
#include NH_CSS_UTILS

// DECLARE =========================================================================================

//static NH_RESULT Nh_Vk_destroyCanvasRenderingContext2DResources(
//    Nh_Vk_Driver *Driver_p, NH_JS_CanvasRenderingContext2D *Context_p
//);

// CREATE ==========================================================================================

NH_RESULT Nh_Vk_createNode(
    Nh_Content *Content_p, Nh_HTML_Node *Node_p)
{
NH_BEGIN()

    if (Content_p == NULL || Node_p == NULL) {NH_DIAGNOSTIC_END(NH_SUCCESS)}

    Nh_Vk_GPU *GPU_p = Content_p->Tab_p->GPU.Pointer;

    if (Node_p->tag == NH_HTML_TAG_IMG && Nh_Gfx_textureLoaded(Content_p, Nh_HTML_getSrc(Node_p), NULL))
    {
        Nh_Texture *Texture_p = Nh_Gfx_getTextureFromLocation(Content_p, Nh_HTML_getSrc(Node_p), NULL);
        NH_CHECK(Nh_Vk_hardLoadTexture(GPU_p, Texture_p))

        NH_CHECK(Nh_Vk_createImageVertices(Content_p, &GPU_p->Driver, Node_p, 0.0f))
        NH_CHECK(Nh_Vk_createImageUniform(&GPU_p->Driver, Node_p, Texture_p))
        NH_CHECK(Nh_Vk_createImageDescriptor(GPU_p, Node_p, Texture_p))
    }
    else 
    {
        for (int i = 0; i < Node_p->Properties.Background.Images.count; ++i) 
        {
            NH_CSS_Image *Image_p = Nh_CSS_getImage(&Node_p->Properties.Background.Images, i);
            NH_CHECK(Nh_Vk_hardLoadTexture(GPU_p, Image_p->Texture_p))
            NH_CHECK(Nh_Vk_createBackgroundImageVertices(Content_p, &GPU_p->Driver, Node_p, Image_p))
            NH_CHECK(Nh_Vk_createBackgroundImageUniform(Content_p, &GPU_p->Driver, Node_p, Image_p))                
            NH_CHECK(Nh_Vk_createBackgroundImageDescriptor(GPU_p, Node_p, Image_p))
        }
        NH_CHECK(Nh_Vk_createBackgroundVertices(Content_p, &GPU_p->Driver, Node_p))
        NH_CHECK(Nh_Vk_createColorUniform(&GPU_p->Driver, Node_p, NH_VK_UNIFORM_BACKGROUND))
        NH_CHECK(Nh_Vk_createColorDescriptor(&GPU_p->Driver, Node_p, NH_VK_DESCRIPTOR_BACKGROUND))
    }

    if (NH_CSS_SHOW_TOP_BORDER(Node_p))
    {
        NH_CHECK(Nh_Vk_createBorderVertices(Content_p, &GPU_p->Driver, Node_p, NH_VK_BUFFER_TOP_BORDER_VERTICES))
        NH_CHECK(Nh_Vk_createColorUniform(&GPU_p->Driver, Node_p, NH_VK_UNIFORM_TOP_BORDER))
        NH_CHECK(Nh_Vk_createColorDescriptor(&GPU_p->Driver, Node_p, NH_VK_DESCRIPTOR_TOP_BORDER))
    }

    if (NH_CSS_SHOW_RIGHT_BORDER(Node_p))
    {
        NH_CHECK(Nh_Vk_createBorderVertices(Content_p, &GPU_p->Driver, Node_p, NH_VK_BUFFER_RIGHT_BORDER_VERTICES))
        NH_CHECK(Nh_Vk_createColorUniform(&GPU_p->Driver, Node_p, NH_VK_UNIFORM_RIGHT_BORDER))
        NH_CHECK(Nh_Vk_createColorDescriptor(&GPU_p->Driver, Node_p, NH_VK_DESCRIPTOR_RIGHT_BORDER))
    }

    if (NH_CSS_SHOW_BOTTOM_BORDER(Node_p))
    {
        NH_CHECK(Nh_Vk_createBorderVertices(Content_p, &GPU_p->Driver, Node_p, NH_VK_BUFFER_BOTTOM_BORDER_VERTICES))
        NH_CHECK(Nh_Vk_createColorUniform(&GPU_p->Driver, Node_p, NH_VK_UNIFORM_BOTTOM_BORDER))
        NH_CHECK(Nh_Vk_createColorDescriptor(&GPU_p->Driver, Node_p, NH_VK_DESCRIPTOR_BOTTOM_BORDER))
    }

    if (NH_CSS_SHOW_LEFT_BORDER(Node_p))
    {
        NH_CHECK(Nh_Vk_createBorderVertices(Content_p, &GPU_p->Driver, Node_p, NH_VK_BUFFER_LEFT_BORDER_VERTICES))
        NH_CHECK(Nh_Vk_createColorUniform(&GPU_p->Driver, Node_p, NH_VK_UNIFORM_LEFT_BORDER))
        NH_CHECK(Nh_Vk_createColorDescriptor(&GPU_p->Driver, Node_p, NH_VK_DESCRIPTOR_LEFT_BORDER))
    }

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

// UPDATE ==========================================================================================

NH_RESULT Nh_Vk_updateNodeProperty(
    Nh_Vk_GPU *GPU_p, Nh_HTML_Node *Node_p, Nh_CSS_GenericProperty *Property_p)
{
NH_BEGIN()

    if (GPU_p == NULL || Node_p == NULL) {NH_DIAGNOSTIC_END(NH_SUCCESS)}

    switch (Property_p->type)
    {
        case NH_CSS_PROPERTY_BACKGROUND_COLOR : 
 
            NH_CHECK(Nh_Vk_mapDeviceMemory(
                &GPU_p->Driver, 
                &Nh_Vk_getUniform(Node_p, NH_VK_UNIFORM_BACKGROUND)->DeviceMemory, 
                Node_p->Properties.Background.color_p, 
                sizeof(float) * 4,
                sizeof(float) * 16 
            ))
            break;
 
        case NH_CSS_PROPERTY_COLOR : 
 
            NH_CHECK(Nh_Vk_mapDeviceMemory(
                &GPU_p->Driver, 
                &Nh_Vk_getUniform(Node_p, NH_VK_UNIFORM_TEXT_SDF_FS)->DeviceMemory, 
                Node_p->Properties.Text.color_p, 
                sizeof(float) * 4,
                0
            ))
            break;
    }

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

// DESTROY =========================================================================================

void Nh_Vk_destroyNode(
    Nh_Vk_GPU *GPU_p, Nh_HTML_Node *Node_p)
{
NH_BEGIN()

    Nh_Vk_destroyBufferList(&GPU_p->Driver, Node_p);
    Nh_Vk_destroyUniformList(&GPU_p->Driver, Node_p);
    Nh_Vk_destroyDescriptorList(&GPU_p->Driver, Node_p);        

//    for (int i = 0; i < Node_p->Injections.count; ++i) {
//        NH_JS_Injection *Injection_p = Nh_getFromLinkedList(&Node_p->Injections, i);
//        switch (Injection_p->type)
//        {
//            case NH_JS_INJECTION_CANVAS_RENDERING_CONTEXT_2D :
//                Nh_Vk_destroyCanvasRenderingContext2DResources(&GPU_p->Driver, Injection_p->data_p);
//        }
//    }

NH_SILENT_END()
}

// CANVAS RENDERING CONTEXT 2D =====================================================================

//NH_RESULT Nh_Vk_createCanvasRenderingContext2DResources(
//    Nh_Content *Content_p, NH_JS_CanvasRenderingContext2D *Context_p)
//{
//NH_BEGIN()
//
//    int count = 0;
//    Nh_HTML_Node *Node_p = NULL;
//
//    for (int i = 0; i < Content_p->Document.Tree.All.count; ++i) {
//        Node_p = Nh_getFromLinkedList(&Content_p->Document.Tree.All, i);
//        if (Nh_JS_getInjection(Node_p, NH_JS_INJECTION_CANVAS_RENDERING_CONTEXT_2D) == Context_p) {break;}
//        Node_p = NULL;
//    }
//
//    NH_CHECK_NULL(Node_p)
//
//    Nh_GPU *GPU_p = &Content_p->Window_p->GPU;
//    Nh_Vk_Driver *Driver_p = &((Nh_Vk_GPU*)GPU_p->Pointer)->Driver;
//
//    for (int i = 0; i < Context_p->Paths.count; ++i)
//    {
//        NH_JS_Path2D *Path_p = Nh_getFromLinkedList(&Context_p->Paths, i);
//        for (int j = 0; j < Path_p->Commands.count; ++j)
//        {
//            if (count++ < Context_p->Vulkan.Uniforms.count) {continue;} // skip already handled commands
//            if (count == 1) {NH_CHECK(Nh_Vk_createImageVertices(Content_p, Driver_p, Node_p, 0.001f))}
//
//            NH_JS_Path2DCommand *Command_p = Nh_getFromLinkedList(&Path_p->Commands, j);
//            switch (Command_p->type)
//            {   
//                case NH_JS_PATH_2D_COMMAND_ARC :
//
//                    NH_CHECK(Nh_Vk_createCanvasRenderingContext2DArcUniform(
//                        Content_p, Driver_p, Node_p, Context_p, Command_p->data_p
//                    ))
//                    NH_CHECK(Nh_Vk_createCanvasRenderingContext2DDescriptor(
//                        Content_p, Driver_p, Context_p, Nh_getFromLinkedList(&Context_p->Vulkan.Uniforms, j), 29 * sizeof(float)
//                    ))
//                    break;
//
//                default : break;
//            }
//        }
//    }
//
//NH_DIAGNOSTIC_END(NH_SUCCESS)
//}
//
//static NH_RESULT Nh_Vk_destroyCanvasRenderingContext2DResources(
//    Nh_Vk_Driver *Driver_p, NH_JS_CanvasRenderingContext2D *Context_p)
//{
//NH_BEGIN()
//
//    for (int i = 0; i < Context_p->Vulkan.Descriptors.count; ++i)
//    {
//        NH_Vk_DescriptorSet *Descriptor_p = Nh_getFromLinkedList(&Context_p->Vulkan.Descriptors, i);
//        Nh_Vk_destroyDescriptorSet(Driver_p, Descriptor_p, &Driver_p->DescriptorPool_p[0]);
//    }
//    Nh_destroyLinkedList(&Context_p->Vulkan.Descriptors, true);
//
//    for (int i = 0; i < Context_p->Vulkan.Uniforms.count; ++i)
//    {
//        NH_Vk_Buffer *Buffer_p = Nh_getFromLinkedList(&Context_p->Vulkan.Uniforms, i);
//        Nh_Vk_destroyBuffer(Driver_p, Buffer_p);
//    }
//    Nh_destroyLinkedList(&Context_p->Vulkan.Uniforms, true);
//
//NH_DIAGNOSTIC_END(NH_SUCCESS)
//}
//

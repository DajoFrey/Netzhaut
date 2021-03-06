#ifndef NH_VK_BUFFER_H
#define NH_VK_BUFFER_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "../Header/Driver.h"

#include "../../../HTML/Main/Header/Document.h"
#include "../../../Netzhaut/Netzhaut.h"
#include "../../../CSS/Header/Image.h"

#endif

/** @addtogroup VulkanEnums Enums
 *  \ingroup Vulkan
 *  @{
 */

   typedef struct NH_Vk_BufferInfo {
        VkBufferCreateInfo *Info_p;
        void *data_p;              
        NH_BOOL mapMemory;           
        int memoryProperties;    
        NH_BOOL createInfo;          
    } NH_Vk_BufferInfo;
    
   typedef struct NH_Vk_Buffer {
        int type;                                    
        int size;
        VkBuffer Buffer;                       
        VkDeviceMemory DeviceMemory;                 
        VkDescriptorBufferInfo DescriptorBufferInfo; 
    } NH_Vk_Buffer;

/** @} */

/** @addtogroup VulkanEnums Enums
 *  \ingroup Vulkan
 *  @{
 */

    typedef enum NH_VK_BUFFER {
        NH_VK_BUFFER_TEXT_SDF,               
        NH_VK_BUFFER_TEXT_SDF_INDEX,         
        NH_VK_BUFFER_BACKGROUND,   
        NH_VK_BUFFER_TOP_BORDER_VERTICES,    
        NH_VK_BUFFER_RIGHT_BORDER_VERTICES,  
        NH_VK_BUFFER_BOTTOM_BORDER_VERTICES, 
        NH_VK_BUFFER_LEFT_BORDER_VERTICES,   
        NH_VK_BUFFER_LIST_ITEM_MARKER,
        NH_VK_BUFFER_BACKGROUND_IMAGE,       
        NH_VK_BUFFER_TEXTURE,            
        NH_VK_BUFFER_COUNT,                  
    } NH_VK_BUFFER;

/** @} */

/** @addtogroup VulkanFunctions Functions
 *  \ingroup Vulkan
 *  @{
 */

    NH_RESULT Nh_Vk_createBackgroundVertices(
        Nh_Content *Content_p, Nh_Vk_Driver *Driver_p, Nh_HTML_Node *Node_p
    );
    NH_RESULT Nh_Vk_updateBackgroundVertices(
        Nh_Content *Content_p, Nh_Vk_Driver *Driver_p, Nh_HTML_Node *Node_p
    );

    NH_RESULT Nh_Vk_createBackgroundImageVertices(
        Nh_Content *Content_p, Nh_Vk_Driver *Driver_p, Nh_HTML_Node *Node_p, NH_CSS_Image *Image_p
    );
    NH_RESULT Nh_Vk_updateBackgroundImageVertices(
        Nh_Content *Content_p, Nh_Vk_Driver *Driver_p, Nh_HTML_Node *Node_p, NH_CSS_Image *Image_p
    );

    NH_RESULT Nh_Vk_createBorderVertices(
        Nh_Content *Content_p, Nh_Vk_Driver *Driver_p, Nh_HTML_Node *Node_p, NH_VK_BUFFER type
    );
    NH_RESULT Nh_Vk_updateBorderVertices(
        Nh_Content *Content_p, Nh_Vk_Driver *Driver_p, Nh_HTML_Node *Node_p, NH_VK_BUFFER type
    );

    NH_RESULT Nh_Vk_createImageVertices(
        Nh_Content *Content_p, Nh_Vk_Driver *Driver_p, Nh_HTML_Node *Node_p, float subtractFromZ
    );
    NH_RESULT Nh_Vk_updateImageVertices(
        Nh_Content *Content_p, Nh_Vk_Driver *Driver_p, Nh_HTML_Node *Node_p, float subtractFromZ
    );
    
    void Nh_Vk_destroyBufferList(
        Nh_Vk_Driver *Driver_p, Nh_HTML_Node *Node_p
    );
    void Nh_Vk_destroyBufferFromType(
        Nh_Vk_Driver *Driver_p, Nh_HTML_Node *Node_p, NH_VK_BUFFER type
    );
 
    NH_Vk_Buffer *Nh_Vk_getBuffer(
        Nh_HTML_Node *Node_p, NH_VK_BUFFER type
    );
    NH_Vk_Buffer *Nh_Vk_getBufferFromIndex(
        Nh_HTML_Node *Node_p, int index
    );

/** @} */

#endif 

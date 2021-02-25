// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "../Header/Record.h"
#include "../Header/Macros.h"

#include "../../Vulkan/Header/Record.h"
#include "../../Vulkan/Header/Node.h"
#include "../../Vulkan/Header/GPU.h"
#include "../../Vulkan/Header/Window.h"
#include "../../Vulkan/Header/Vulkan.h"

#include NH_FLOW
#include NH_DEFAULT_CHECK
#include NH_HTML_UTILS
#include NH_CSS_UTILS

#include "../../../Netzhaut/Sync.h"

// TAB =============================================================================================

NH_RESULT Nh_Gfx_createRecordResources(
    Nh_Tab *Tab_p)
{
NH_BEGIN()

    switch (Tab_p->GPU.API)
    {
        case NH_API_VULKAN : NH_CHECK(Nh_Vk_createRecordResources(Tab_p)) break;
    }

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

NH_RESULT Nh_Gfx_record(
    Nh_Tab *Tab_p)
{
NH_BEGIN()

    Nh_syncRecordCommandBufferBegin(Tab_p);

    int imageCount = 0;

    switch (Tab_p->GPU.API)
    {
        case NH_API_VULKAN : 
        {
            Nh_Vk_Surface *Surface_p = Nh_getFromLinkedList(
                &Tab_p->Window_p->Vulkan.Surfaces, Nh_Vk_getGPUIndex(Tab_p->GPU.Pointer)
            );
            NH_CHECK_NULL(Surface_p)
            imageCount = Surface_p->imageCount; 
            break;
        }
    }

    for (int i = 0; i < imageCount; ++i)
    {
        int bufferIndex = 2 * i;

        if (Tab_p->Window_p->Sync.rendering) {
            bufferIndex += Tab_p->Sync.submittedBuffers_p[i] == bufferIndex ? 1 : 0;
        } 
        else {bufferIndex += Tab_p->Sync.newestBuffers_p[i] == bufferIndex ? 1 : 0;}
 
        switch (Tab_p->GPU.API)
        {
            case NH_API_VULKAN :
            { 
                NH_CHECK(Nh_Vk_record(Tab_p->GPU.Pointer, Tab_p, bufferIndex, i))
                break;
            }
            case NH_API_DIRECTX :
            { 
                break;
            }
        }

        Tab_p->Sync.newestBuffers_p[i] = bufferIndex;
    }

    Nh_syncRecordCommandBufferEnd(Tab_p);

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

void Nh_Gfx_destroyRecordResources(
    Nh_Tab *Tab_p)
{
NH_BEGIN()

    switch (Tab_p->GPU.API)
    {
        case NH_API_VULKAN : Nh_Vk_destroyRecordResources(Tab_p); break;
    }

NH_SILENT_END()
}


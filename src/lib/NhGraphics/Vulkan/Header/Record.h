#ifndef NH_VK_RECORD_H
#define NH_VK_RECORD_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "../../../Netzhaut/Netzhaut.h"
#include "../../../Netzhaut/System.h"
#include "../../../Netzhaut/Tab.h"

#endif

/** @addtogroup VulkanFunctions Functions
 *  \ingroup Vulkan
 *  @{
 */

    NH_RESULT Nh_Vk_createRecordResources(
        Nh_Tab *Tab_p
    );
   
    void Nh_Vk_destroyRecordResources(
        Nh_Tab *Tab_p
    );

    NH_RESULT Nh_Vk_record(
        Nh_Vk_GPU *GPU_p, Nh_Tab *Tab_p, int bufferIndex, int imageIndex
    );

/** @} */

#endif

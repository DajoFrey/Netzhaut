// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "Callbacks.h"
#include "Memory.h"
#include "Config.h"
#include "String.h"

#include "Common/Macro.h"
#include NH_FLOW
#include NH_DEFAULT_CHECK

#include <string.h>
#include <stdio.h>

// DECLARE =========================================================================================

typedef struct Nh_Callback {
    NH_CALLBACK type;
    char *id_p;
    void *function_p;
} Nh_Callback;

// ADD =============================================================================================

//static NH_RESULT Nh_addCallback(
//    Nh_LinkedList *Callbacks_p, char *id_p, void *function_p, NH_CALLBACK type)
//{
//NH_BEGIN()
//
//    NH_CHECK_NULL(Callbacks_p)
//    NH_CHECK_NULL(function_p)
//
//    Nh_Callback *Callback_p = Nh_allocate(sizeof(Nh_Callback));
//    NH_CHECK_MEM(Callback_p)
//    Callback_p->type       = type;
//    Callback_p->function_p = function_p;
//    Callback_p->id_p       = NULL;
//   
//    if (id_p != NULL) { 
//        Callback_p->id_p = Nh_allocateBytes(id_p);
//        NH_CHECK_MEM(Callback_p->id_p)
//    }
//
//    NH_CHECK(Nh_appendToLinkedList(Callbacks_p, Callback_p))
//
//NH_DIAGNOSTIC_END(NH_SUCCESS)
//}

// VULKAN ==========================================================================================

NH_RESULT Nh_setVulkanCallback(
    Nh_LinkedList *Callbacks_p, char *id_p, NH_RESULT (*function_p)(struct VolkDeviceTable, VkCommandBuffer*))
{
//NH_BEGIN()
//
//    NH_CHECK_NULL(Callbacks_p)
//    NH_CHECK_NULL(id_p)
//    NH_CHECK_NULL(function_p)
//
//NH_END(Nh_addCallback(Callbacks_p, id_p, function_p, NH_CALLBACK_VULKAN))
}

Nh_VulkanCallback Nh_getVulkanCallback(
    Nh_LinkedList *Callbacks_p, char *id_p)
{
NH_BEGIN()

    for (int i = 0; i < Callbacks_p->count; ++i) {
        Nh_Callback *Callback_p = Nh_getFromLinkedList(Callbacks_p, i);
        if (Callback_p->type == NH_CALLBACK_VULKAN && !strcmp(id_p, Callback_p->id_p)) {
            NH_END(Callback_p->function_p)
        }
    }

NH_END(NULL)
}

// DELETE ==========================================================================================

void Nh_deleteCallbacks(
    Nh_LinkedList *Callbacks_p)
{
NH_BEGIN()

    for (int i = 0; i < Callbacks_p->count; ++i) {
        Nh_Callback *Callback_p = Nh_getFromLinkedList(Callbacks_p, i);
        Nh_free(Callback_p->id_p);
    }
    Nh_destroyLinkedList(Callbacks_p, true);

NH_END()
}


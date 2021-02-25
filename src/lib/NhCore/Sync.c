// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE ========================================================================================

#include "Sync.h"
#include "Time.h"

#include "Common/Macro.h"
#include NH_FLOW
#include NH_DEFAULT_CHECK

#include <string.h>
#include <limits.h>

// DECLARE =========================================================================================

static void Nh_forceAtomicRecording(
    Nh_Window *Window_p
);

static NH_BOOL Nh_doSync(
    int ownerId
);

// COMMANDBUFFER ===================================================================================

void Nh_syncRecordCommandBufferBegin(
    Nh_Tab *Tab_p)
{
NH_BEGIN()

    Tab_p->Sync.atomic = Tab_p->Sync.atomicRecording;

NH_SILENT_END()
}

void Nh_syncRecordCommandBufferEnd(
    Nh_Tab *Tab_p)
{
NH_BEGIN()

    Tab_p->Sync.render = NH_TRUE;

//    // sync with window
//    if (Nh_doSync(Tab_p->Window_p->Internal.ownerId) && Tab_p->Sync.atomic) 
//    {
//        Tab_p->Sync.atomicRecording = NH_FALSE;
//        Tab_p->Sync.atomic = NH_FALSE;
//        while (Tab_p->Window_p->Sync.resize) {} // wait in case of resize
//    }

NH_SILENT_END()
}

void Nh_syncUseCommandBuffer(
    Nh_Tab *Tab_p)
{
NH_BEGIN()

//    // sync with tab (primarily command buffer creation)
//    if (Nh_doSync(Tab_p->Sync.ownerId)) {
//        Tab_p->Sync.atomicRecording = NH_TRUE;
//        while (Tab_p->Sync.atomicRecording && Tab_p->Sync.render) {};
//    }

NH_SILENT_END()
}

// RENDER ==========================================================================================

void Nh_syncRenderBegin(
    Nh_Window *Window_p)
{
NH_BEGIN()

    Window_p->Sync.rendering = NH_TRUE;
    Window_p->Internal.atomic = Window_p->Sync.atomicRender;

NH_SILENT_END()
}

void Nh_syncRenderEnd(
    Nh_Window *Window_p)
{
NH_BEGIN()

    if (Window_p->Internal.atomic) {
        Window_p->Internal.atomic = NH_FALSE;
        Window_p->Sync.atomicRender = NH_FALSE;
    }
    Window_p->Sync.rendering = NH_FALSE;

NH_SILENT_END()
}

// RESIZE ==========================================================================================

void Nh_syncResizeBegin(
    Nh_Window *Window_p)
{
NH_BEGIN()

    Window_p->Sync.resize = NH_TRUE;
    Nh_forceAtomicRecording(Window_p);

NH_SILENT_END()
}

void Nh_syncResizeEnd(
    Nh_Window *Window_p)
{
NH_BEGIN()

    Window_p->Sync.resize = NH_FALSE;
    Nh_forceAtomicRecording(Window_p);
    
    Window_p->Internal.getNextImage = NH_FALSE;
    Window_p->Internal.skipRender = NH_TRUE;

NH_SILENT_END()
}

void Nh_syncWaitForWindow(
    Nh_Window *Window_p)
{
NH_BEGIN()

//    if (Nh_doSync(Window_p->Internal.ownerId)) {
//        while (Window_p->Sync.ready) {}
//    }

NH_SILENT_END()
}

// HELPER ==========================================================================================

static NH_BOOL Nh_doSync(
    int ownerId)
{
NH_BEGIN()

//    Nh_Workload *Owner_p  = &Nh_getThreadPool()->Threads_p[ownerId];
//    Nh_Workload *Exec_p   = Nh_getThread();
//    Nh_Workload *Kernel_p = Nh_getKernelThread();
//
//    if (Owner_p->Info.owner == Kernel_p->id && Exec_p->Info.owner == Kernel_p->id) {NH_DIAGNOSTIC_END(NH_FALSE)}
//    if (Owner_p == Kernel_p && Exec_p->Info.owner == Kernel_p->id) {NH_DIAGNOSTIC_END(NH_FALSE)}
//    if (Owner_p->Info.owner == Kernel_p->id && Exec_p == Kernel_p) {NH_DIAGNOSTIC_END(NH_FALSE)}

NH_DIAGNOSTIC_END(NH_TRUE)
}

static void Nh_forceAtomicRecording(
    Nh_Window *Window_p)
{
NH_BEGIN()

    for (int i = 0; i < Window_p->Tabs.count; ++i) 
    {
        Nh_Tab *Tab_p = Nh_getTab(Window_p, i);
        
//        if (Nh_doSync(Tab_p->Sync.ownerId)) 
//        {
//            Tab_p->Sync.atomicRecording = NH_TRUE;
//            while (Tab_p->Sync.atomicRecording) {}
//        }
    }

NH_SILENT_END()
}


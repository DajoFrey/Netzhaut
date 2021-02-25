#ifndef NH_SYSTEM_H
#define NH_SYSTEM_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "String.h"
#include "Thread.h"
#include "Time.h"

#include "Common/Result.h"
#include "Common/API.h"

#include <stdlib.h>

#endif

/** @addtogroup NhCoreMacros
 *  @{
 */

    #define NH_API_UNDEFINED 666
    #define NH_HOSTNAME_LENGTH 255

/** @} */

/** @addtogroup NhCoreEnums
 *  @{
 */

    typedef enum NH_API {
        NH_API_VULKAN,
        NH_API_DIRECTX,
        NH_API_COUNT,
    } NH_API;
    
/** @} */

/** @addtogroup NhCoreStructs
 *  @{
 */

    typedef struct Nh_CPU {
        char name_p[255];
        int cores;       
        NH_BOOL ht;         
    } Nh_CPU;
    
    typedef struct Nh_RAM {
         unsigned long total;
         unsigned long free; 
    } Nh_RAM;

    typedef struct Nh_GPU {
        int API;
        void *Pointer;
    } Nh_GPU;
    
    typedef struct Nh_SystemUpdater {
        Nh_SystemTime LastUpdate;
        double updateIntervalInSeconds;
    } Nh_SystemUpdater;

    typedef struct Nh_System {
        Nh_SystemUpdater Updater;
        NH_BYTE hostname_p[NH_HOSTNAME_LENGTH];
        long uptime;
        Nh_LocalTime LocalTime;
        NH_BOOL littleEndian;
        NH_BOOL available_p[NH_API_COUNT];
        Nh_RAM RAM;                     
        Nh_CPU CPU; 
    } Nh_System;

/** @} */

/** @addtogroup NhCoreVars
 *  @{
 */

    extern Nh_System NH_SYSTEM;
    
/** @} */

/** @addtogroup NhCoreFunctions
 *  @{
 */

    NH_RESULT Nh_initSystem(
    );
    
    NH_RESULT Nh_freeSystem(
    );

    NH_SIGNAL Nh_runSystemUpdater(
        void *args_p
    );

/** @} */

#endif 

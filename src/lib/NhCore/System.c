// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE ========================================================================================

#ifdef __unix__
#define _GNU_SOURCE
#endif

#include "System.h"
#include "Memory.h"
#include "Config.h"
#include "String.h"

//#include "../../Graphics/Vulkan/Header/Vulkan.h"

#include "Common/Log.h"
#include "Common/Macro.h"
#include NH_FLOW
#include NH_DEFAULT_CHECK

#include <unistd.h>
#include <string.h>
#include <stdio.h>

#ifdef __unix__
    #include <sys/sysinfo.h>
#endif

// DATA ===========================================================================================

Nh_System NH_SYSTEM;

// CPU =============================================================================================

static NH_RESULT getCPUAttr(
    char *src, char *dest) 
{
NH_BEGIN()

    if (src != NULL) {
        char delim[] = ":";
        strtok(src, delim);
        char *src2 = strtok(NULL, delim);
        char delim2[] = "\n";
        char *src3 = strtok(src2, delim2); 
        strcpy(dest, src3 + 1);
    }
    else {NH_DIAGNOSTIC_END(NH_ERROR_BAD_STATE)}

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

static NH_RESULT Nh_getCPUInfo()
{
NH_BEGIN()

    char name_p[255];
    int cores = 0;
    NH_BOOL ht   = NH_FALSE;

#ifdef __unix__

    FILE *cpuinfo = fopen("/proc/cpuinfo", "rb");
    char *arg = 0;
    size_t size = 0;
    while(getdelim(&arg, &size, 0, cpuinfo) != -1) 
    {
        char arg2[size], arg3[size];
        strcpy(arg2, arg);
        strcpy(arg3, arg);
    
    // name
        char *ptr = strstr(arg, "model name");
        NH_CHECK(getCPUAttr(ptr, name_p))

    // cores
        char *ptr2 = strstr(arg2, "cpu cores");
        char tmp[8];
        NH_CHECK(getCPUAttr(ptr2, tmp))
        cores = atoi(tmp);

    // ht
        char *ptr3 = strstr(arg3, "flags");
        char flags[2048];
        
        NH_CHECK(getCPUAttr(ptr3, flags))
            
        ht = strstr(flags, "ht") != NULL;
    }
    free(arg);
    fclose(cpuinfo);

#elif defined(_WIN32) || defined (WIN32)

#endif

    NH_SYSTEM.CPU.cores = cores;
    NH_SYSTEM.CPU.ht    = ht;
    strcpy(NH_SYSTEM.CPU.name_p, name_p);

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

// OTHER ===========================================================================================

static NH_BOOL Nh_littleEndian()
{
NH_BEGIN()

    unsigned int x = 1;

NH_END((int)(((char *)&x)[0] == 1))
}

static NH_RESULT Nh_getOtherInfo()
{
NH_BEGIN()

    unsigned long total = 0;
    unsigned long free  = 0;
    long uptime = 0;

#ifdef __unix__

    struct sysinfo info;
    sysinfo(&info);
    uptime = info.uptime;
    total  = info.totalram;
    free   = info.freeram;

#elif defined(_WIN32) || defined (WIN32)

#endif

    NH_SYSTEM.LocalTime    = Nh_getLocalTime(); 
    NH_SYSTEM.uptime       = uptime;
    NH_SYSTEM.littleEndian = Nh_littleEndian();
    NH_SYSTEM.RAM.total    = total;
    NH_SYSTEM.RAM.free     = free;

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

// GPU =============================================================================================

static NH_RESULT Nh_initGPUs()
{
NH_BEGIN()

//    if (Nh_Vk_initVulkan() == NH_SUCCESS) {
//        System.available_p[NH_API_VULKAN] = NH_TRUE;
//    }
   
NH_DIAGNOSTIC_END(NH_SUCCESS)
}

// SYSTEM ==========================================================================================

static void Nh_logSystem()
{
NH_BEGIN()

    Nh_DataStorageUnits Total = Nh_humanizeBytes(NH_SYSTEM.RAM.total);
    Nh_DataStorageUnits Free = Nh_humanizeBytes(NH_SYSTEM.RAM.free);
    Nh_GeneralTime Uptime = Nh_humanizeSeconds(NH_SYSTEM.uptime);

    char log_p[255] = {'\0'};
    sprintf(log_p, "LOCAL TIME %lu-%lu-%lu %lu:%lu:%lu", 
        NH_SYSTEM.LocalTime.years, NH_SYSTEM.LocalTime.months, NH_SYSTEM.LocalTime.days, NH_SYSTEM.LocalTime.hours, NH_SYSTEM.LocalTime.minutes, NH_SYSTEM.LocalTime.seconds
    );
    Nh_logSystemInfo(log_p, 2);

    memset(log_p, 0, 255); 
    sprintf(log_p, "UPTIME     %lu weeks, %lu days, %lu hours, %lu minutes, %lu seconds", 
        Uptime.weeks, Uptime.days, Uptime.hours, Uptime.minutes, Uptime.seconds
    );
    Nh_logSystemInfo(log_p, 3);

    memset(log_p, 0, 255); 
    sprintf(log_p, "RAM        total:%lugb %lumb, free:%lugb %lumb", 
        Total.gigabytes, Total.megabytes, Free.gigabytes, Free.megabytes
    );
    Nh_logSystemInfo(log_p, 4);

NH_SILENT_END()
}

static NH_RESULT Nh_updateSystem()
{
NH_BEGIN()

    NH_CHECK(Nh_getCPUInfo())
    NH_CHECK(Nh_getOtherInfo())

    Nh_logSystem();

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

NH_RESULT Nh_initSystem()
{
NH_BEGIN()

    for (int i = 0; i < NH_API_COUNT; ++i) {NH_SYSTEM.available_p[i] = NH_FALSE;}

    NH_CHECK(Nh_initGPUs())
    NH_CHECK(Nh_updateSystem())

    NH_SYSTEM.Updater.updateIntervalInSeconds = 1;
    NH_SYSTEM.Updater.LastUpdate = Nh_getSystemTime();

    memset(NH_SYSTEM.hostname_p, 0, NH_HOSTNAME_LENGTH);
#ifdef __unix__
    gethostname(NH_SYSTEM.hostname_p, NH_HOSTNAME_LENGTH);
#endif

    char log_p[255] = {'\0'};
    sprintf(log_p, "HOST NAME  %s", NH_SYSTEM.hostname_p);
    Nh_logSystemInfo(log_p, 0);

    memset(log_p, 0, 255); 
    sprintf(log_p, NH_SYSTEM.littleEndian ? "ENDIANNESS Little-Endian (LE)" : "ENDIANNESS Big-Endian (BE)");
    Nh_logSystemInfo(log_p, 1);

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

NH_RESULT Nh_freeSystem()
{
NH_BEGIN()

//    Nh_Vk_freeVulkan();

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

// UPDATER =========================================================================================

NH_SIGNAL Nh_runSystemUpdater(
    void *args_p)
{
NH_BEGIN()

    if (Nh_getSystemTimeDiffInSeconds(NH_SYSTEM.Updater.LastUpdate, Nh_getSystemTime()) < NH_SYSTEM.Updater.updateIntervalInSeconds) {
        NH_END(NH_SIGNAL_OK)
    }
    NH_CHECK(Nh_updateSystem())
    NH_SYSTEM.Updater.LastUpdate = Nh_getSystemTime();

NH_END(NH_SIGNAL_OK)
}


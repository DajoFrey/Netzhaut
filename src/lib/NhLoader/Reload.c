// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#define _GNU_SOURCE

#include "Reload.h"
#include "Loader.h"

#include "Common/Macro.h"
#include NH_LOADER_DEFAULT_CHECK
#include NH_LOADER_FLOW

#include <link.h>
#include <dlfcn.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>

// MODIFIED ========================================================================================

NH_BYTE *Nh_Loader_lastModified(
    void *lib_p)
{
NH_LOADER_BEGIN()

#ifdef __unix__

    struct link_map *info_p;
    dlinfo(lib_p, RTLD_DI_LINKMAP, &info_p);

    struct stat attrib;
    stat(info_p->l_name, &attrib);
    
    struct tm *tm;
    NH_BYTE *time_p = malloc(sizeof(NH_BYTE) * 127);
    if (time_p == NULL) {NH_LOADER_END(NULL)}
    memset(time_p, '\0', 127);

    /* convert time_t to broken-down time representation */
    tm = localtime(&attrib.st_mtime);
    /* format time days.month.year hour:minute:seconds */
    strftime(time_p, sizeof(NH_BYTE) * 127, "%d.%m.%Y %H:%M:%S", tm);

#endif

NH_LOADER_END(time_p)
}

NH_BYTE *Nh_Loader_getModifiedModuleName()
{
NH_LOADER_BEGIN()

    NH_BYTE *name_p = NULL;

    for (int i = 0; i < NH_LOADER_MAX_MODULES; ++i) 
    {
        if (NH_LOADER.Modules_p[i].name_p != NULL) 
        {
            NH_BYTE *lastModified_p = Nh_Loader_lastModified(NH_LOADER.Modules_p[i].lib_p);
            if (strcmp(NH_LOADER.Modules_p[i].lastModified_p, lastModified_p)) {
                name_p = malloc(sizeof(NH_LOADER.Modules_p[i].name_p) + 1);
                strcpy(name_p, NH_LOADER.Modules_p[i].name_p);
            }
            free(lastModified_p);
            if (name_p != NULL) {break;}
        }
    }

NH_LOADER_END(name_p)
}

//// INIT UPDATER ====================================================================================
//
//typedef struct Nh_Loader_ModuleUpdater {
//    double intervalInSeconds;
//    Nh_SystemTime LastUpdate;
//    NH_BOOL offline;
//} Nh_Loader_ModuleUpdater;
//
//void *Nh_initModuleUpdater(
//    void *args_p) 
//{
//NH_LOADER_BEGIN()
//
//    Nh_Loader_ModuleUpdater *Updater_p = Nh_allocate(sizeof(Nh_Loader_ModuleUpdater));
//    Updater_p->intervalInSeconds = 2.0;
//    Updater_p->LastUpdate = Nh_getSystemTime();
//    Updater_p->offline = NH_TRUE;
//
//NH_LOADER_END(Updater_p)
//NH_LOADER_END(NULL)
//}
//
//// RUN UPDATER =====================================================================================
//
//typedef NH_RESULT (*Nh_startProcess_f)(
//    int argc, NH_BYTE **argv_pp
//); 
//
//// TODO multithreading
//static Nh_List Nh_killLibDependentForks(
//    NH_BYTE *libName_p)
//{
//NH_LOADER_BEGIN()
//
//    Nh_List Functions = Nh_initList(1);
//
//    for (int i = 0; i < NH_MAX_FORKS; ++i) 
//    {
//        Nh_Process *Proc_p = &NH_PROCESS_POOL.Forks_p[i];
//        if (Proc_p->id != 0) 
//        {
//            NH_BYTE *libName2_p = Nh_writeToProcess(Proc_p, "NH_IPC_ID", 10, NH_TRUE);
//            if (libName2_p != NULL && !strcmp(libName_p, libName2_p)) {
//                NH_BYTE *funcName_p = Nh_writeToProcess(Proc_p, "NH_IPC_UPDATE", 14, NH_TRUE);
//                if (funcName_p != NULL) {
//                    Nh_appendToList(&Functions, funcName_p);
//                    // wait until fork has terminated
//                    while (Proc_p->id != 0) {
//                        Nh_checkForks();
//                    }
//                }
//            }
//            free(libName2_p);
//        }
//    }
//
//NH_LOADER_END(Functions)
//}
//
//NH_SIGNAL Nh_runModuleUpdater(
//    void *args_p)
//{
//NH_LOADER_BEGIN()
//
//    Nh_Loader_ModuleUpdater *Updater_p = args_p;
//    if (Nh_getSystemTimeDiffInSeconds(Updater_p->LastUpdate, Nh_getSystemTime()) < Updater_p->intervalInSeconds) {
//        NH_LOADER_END(NH_SIGNAL_OK)
//    }
//
//    for (NH_BYTE *libName_p; libName_p = Nh_getModifiedLib();)
//    {
//        Nh_List Functions = Nh_killLibDependentForks(libName_p);
//
//        NH_LOADER_CHECK(Nh_unload(libName_p))
//        NH_LOADER_CHECK(Nh_load(libName_p, NH_FALSE))
//
//        for (int i = 0; i < Functions.size; ++i) {
//            NH_LOADER_CHECK(((Nh_startProcess_f)Nh_loadFunction(libName_p, Functions.handles_pp[i]))(0, NULL))
//        }
//    
//        Nh_freeList(&Functions, NH_TRUE);
//        Nh_free(libName_p);
//    }
//
//    Updater_p->LastUpdate = Nh_getSystemTime();
//
//NH_LOADER_END(NH_SIGNAL_OK)
//}
//

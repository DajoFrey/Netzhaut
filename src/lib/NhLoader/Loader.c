// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#define _GNU_SOURCE

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

// DECLARE =========================================================================================

#define NH_LOADER_MAX_LIBS 1024

typedef struct Nh_Loader_Lib {
    NH_BYTE *name_p;
    int major;
    void *handle_p;
    NH_BYTE *lastModified_p;
} Nh_Loader_Lib;

// DATA ============================================================================================

Nh_Loader NH_LOADER;
static Nh_Loader_Lib Libs_p[NH_LOADER_MAX_LIBS] = {NULL};

// GET =============================================================================================

Nh_Loader *Nh_Loader_getLoader()
{
NH_LOADER_BEGIN()
NH_LOADER_END(&NH_LOADER)
}

// LOAD ============================================================================================

static NH_BYTE *Nh_Loader_lastModified(
    void *lib_p)
{
NH_LOADER_BEGIN()

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

NH_LOADER_END(time_p)
}

static NH_LOADER_RESULT Nh_Loader_sleepMs(
    int milliseconds)
{
NH_LOADER_BEGIN()

#ifdef WIN32
    Sleep(milliseconds);
#elif _POSIX_C_SOURCE >= 199309L
    struct timespec ts;
    ts.tv_sec = milliseconds / 1000;
    ts.tv_nsec = (milliseconds % 1000) * 1000000;
    nanosleep(&ts, NULL);
#else
    usleep(milliseconds * 1000);
#endif

NH_LOADER_DIAGNOSTIC_END(NH_LOADER_SUCCESS)
}

static void *Nh_Loader_getLibraryHandle(
    const NH_BYTE *name_p, int major, NH_BOOL install)
{
NH_LOADER_BEGIN()

    NH_BYTE *error_p;
    dlerror();

    NH_BYTE libName_p[255] = {'\0'};
    sprintf(libName_p, "lib%s.so.%d", name_p, major);

    void *dl_p = dlopen(libName_p, RTLD_NOW | RTLD_GLOBAL);

    int try = 0;
    while (dl_p == NULL && try++ < 100) {
        if ((error_p = dlerror()) != NULL) { 
            fprintf(stderr, "%s\n", error_p);
        }
        Nh_Loader_sleepMs(50);
        dl_p = dlopen(libName_p, RTLD_NOW | RTLD_GLOBAL);
    }

    if (dl_p == NULL && install) {
        // TODO install lib
        exit(0);
    }

NH_LOADER_END(dl_p)
}

static Nh_Loader_Lib *Nh_Loader_getLibrary(
    const NH_BYTE *name_p, int major)
{
NH_LOADER_BEGIN()

    for (int i = 0; i < NH_LOADER_MAX_LIBS; ++i) {
        if (Libs_p[i].name_p != NULL && !strcmp(name_p, Libs_p[i].name_p) && Libs_p[i].major == major) {
            NH_LOADER_END(&Libs_p[i])
        }
    }

NH_LOADER_END(NULL)
}

NH_LOADER_RESULT Nh_Loader_load(
    const NH_BYTE *name_p, int major, NH_BOOL install)
{
NH_LOADER_BEGIN()

    if (Nh_Loader_isLoaded(name_p, major)) {NH_LOADER_DIAGNOSTIC_END(NH_LOADER_SUCCESS)}

    void *lib_p = Nh_Loader_getLibraryHandle(name_p, major, install);
    NH_LOADER_CHECK_NULL(lib_p)

    Nh_Loader_Lib *Lib_p = NULL;
    for (int i = 0; i < NH_LOADER_MAX_LIBS; ++i) {
        if (Libs_p[i].name_p == NULL && Libs_p[i].handle_p == NULL) {Lib_p = &Libs_p[i]; break;}
    }
    NH_LOADER_CHECK_NULL(Lib_p)

    Lib_p->handle_p = lib_p;

    Lib_p->lastModified_p = Nh_Loader_lastModified(lib_p);
    NH_LOADER_CHECK_MEM(Lib_p->lastModified_p)

    Lib_p->name_p = malloc(sizeof(NH_BYTE) * (strlen(name_p) + 1));
    NH_LOADER_CHECK_MEM(Lib_p->name_p)

    strcpy(Lib_p->name_p, name_p);

NH_LOADER_DIAGNOSTIC_END(NH_LOADER_SUCCESS)
}

// UNLOAD ==========================================================================================

NH_LOADER_RESULT Nh_Loader_unload(
    const NH_BYTE *name_p, int major)
{
NH_LOADER_BEGIN()

    Nh_Loader_Lib *Lib_p = Nh_Loader_getLibrary(name_p, major);
    NH_LOADER_CHECK_NULL(Lib_p)

    if (dlclose(Lib_p->handle_p)) {
        NH_LOADER_DIAGNOSTIC_END(NH_LOADER_ERROR_BAD_STATE)
    }

    Lib_p->handle_p = NULL;
    Lib_p->major = 0;

    if (Lib_p->name_p != NULL) {
        free(Lib_p->name_p);
        Lib_p->name_p = NULL;
    }
    if (Lib_p->lastModified_p != NULL) {
        free(Lib_p->lastModified_p);
        Lib_p->lastModified_p = NULL;
    }

NH_LOADER_DIAGNOSTIC_END(NH_LOADER_SUCCESS)
}

// IS LOADED =======================================================================================

NH_BOOL Nh_Loader_isLoaded(
    const NH_BYTE *name_p, int major)
{
NH_LOADER_BEGIN()
NH_LOADER_END(Nh_Loader_getLibrary(name_p, major) != NULL ? NH_TRUE : NH_FALSE)
}

// LOAD FUNCTION ===================================================================================

void *Nh_Loader_loadFunction(
    const NH_BYTE *libName_p, int major, const NH_BYTE *functionName_p)
{
NH_LOADER_BEGIN()

    Nh_Loader_Lib *Library_p = Nh_Loader_getLibrary(libName_p, major);
    if (Library_p == NULL) {NH_LOADER_END(NULL)}

    NH_BYTE *error_p;
    dlerror(); // clear any existing error

    void *function_p = dlsym(Library_p->handle_p, functionName_p); 
    if ((error_p = dlerror()) != NULL) {NH_LOADER_END(NULL)}

NH_LOADER_END(function_p)
}

// GET MODIFIED ====================================================================================

NH_BYTE *Nh_Loader_getModifiedLib()
{
NH_LOADER_BEGIN()

    NH_BYTE *name_p = NULL;

    for (int i = 0; i < NH_LOADER_MAX_LIBS; ++i) 
    {
        if (Libs_p[i].name_p != NULL) 
        {
            NH_BYTE *lastModified_p = Nh_Loader_lastModified(Libs_p[i].handle_p);
            if (strcmp(Libs_p[i].lastModified_p, lastModified_p)) {
                name_p = malloc(sizeof(Libs_p[i].name_p) + 1);
                strcpy(name_p, Libs_p[i].name_p);
            }
            free(lastModified_p);
            if (name_p != NULL) {break;}
        }
    }

NH_LOADER_END(name_p)
}

//// INIT UPDATER ====================================================================================
//
//typedef struct Nh_Loader_LibUpdater {
//    double intervalInSeconds;
//    Nh_SystemTime LastUpdate;
//    NH_BOOL offline;
//} Nh_Loader_LibUpdater;
//
//void *Nh_initLibraryUpdater(
//    void *args_p) 
//{
//NH_LOADER_BEGIN()
//
//    Nh_Loader_LibUpdater *Updater_p = Nh_allocate(sizeof(Nh_Loader_LibUpdater));
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
//NH_SIGNAL Nh_runLibraryUpdater(
//    void *args_p)
//{
//NH_LOADER_BEGIN()
//
//    Nh_Loader_LibUpdater *Updater_p = args_p;
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

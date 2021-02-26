// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#define _GNU_SOURCE

#include "Library.h"
#include "Loader.h"
#include "Repository.h"

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
#include <unistd.h>
#include <sys/stat.h>

// INSTALL =========================================================================================

static NH_BOOL Nh_Loader_fileExists(
    NH_BYTE *filename_p)
{
NH_LOADER_BEGIN()

    NH_BOOL fileExists = NH_FALSE;

#ifdef __unix__
    fileExists = access(filename_p, F_OK) != -1 ? NH_TRUE : NH_FALSE;
#endif

NH_LOADER_END(fileExists)
}

NH_LOADER_RESULT Nh_Loader_installLibrary(
    NH_BYTE *libName_p)
{
NH_LOADER_BEGIN()

    if (!NH_LOADER.install) {NH_LOADER_DIAGNOSTIC_END(NH_LOADER_ERROR_BAD_STATE)}

    if (!Nh_Loader_fileExists("Netzhaut-master")) {
        NH_LOADER_CHECK(Nh_Loader_downloadNetzhaut())
        NH_LOADER_CHECK(Nh_Loader_buildNhInstaller())
    }

#ifdef __unix__

    NH_BYTE command_p[2048] = {'\0'};
    memset(command_p, '\0', sizeof(NH_BYTE) * 2048);

    switch (NH_LOADER.scope)
    {
        case NH_LOADER_SCOPE_LOCAL :
            sprintf(command_p, "./Netzhaut-master/bin/installer/NhInstaller -l %s", libName_p);
            break;
        case NH_LOADER_SCOPE_LOCAL_SYSTEM :
            sprintf(command_p, "./Netzhaut-master/bin/installer/NhInstaller -li %s", libName_p);
            break;
        case NH_LOADER_SCOPE_SYSTEM :
            NH_LOADER_END(NH_LOADER_ERROR_BAD_STATE)
            break;
    }

    int status = system(command_p);
    if (WEXITSTATUS(status) || WIFSIGNALED(status)) {NH_LOADER_DIAGNOSTIC_END(NH_LOADER_ERROR_BAD_STATE)}

#endif

NH_LOADER_DIAGNOSTIC_END(NH_LOADER_SUCCESS)
}

// OPEN ============================================================================================

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
    NH_BYTE *libPath_p)
{
NH_LOADER_BEGIN()

    NH_BYTE *error_p;
    dlerror();

    void *dl_p = dlopen(libPath_p, RTLD_NOW | RTLD_GLOBAL);

    int try = 0;
    while (dl_p == NULL && try++ < 100) {
        if ((error_p = dlerror()) != NULL) { 
            fprintf(stderr, "%s\n", error_p);
        }
        Nh_Loader_sleepMs(50);
        dl_p = dlopen(libPath_p, RTLD_NOW | RTLD_GLOBAL);
    }

    if (dl_p == NULL) {
        exit(0);
    }

NH_LOADER_END(dl_p)
}

void *Nh_Loader_loadLibrary(
    NH_BYTE *libName_p, int major)
{
NH_LOADER_BEGIN()

#ifdef __unix__

    void *lib_p = NULL;

    switch (NH_LOADER.scope)
    {
        case NH_LOADER_SCOPE_LOCAL :
        {
            NH_BYTE exeDir_p[2048] = {'\0'};
            if (Nh_Loader_getExeDir(exeDir_p, 2048) != NH_LOADER_SUCCESS) {NH_LOADER_END(NULL)}
            NH_BYTE libPath_p[2048] = {'\0'};
            sprintf(libPath_p, "%s/Netzhaut-master/lib/lib%s.so.%d", exeDir_p, libName_p, major);
            lib_p = Nh_Loader_getLibraryHandle(libPath_p);
        }
        case NH_LOADER_SCOPE_LOCAL_SYSTEM :
        {
            NH_BYTE libPath_p[255] = {'\0'};
            sprintf(libPath_p, "/usr/local/lib/lib%s.so.%d", libName_p, major);
            lib_p = Nh_Loader_getLibraryHandle(libPath_p);
        }
        case NH_LOADER_SCOPE_SYSTEM :
        {
            NH_BYTE libPath_p[255] = {'\0'};
            sprintf(libPath_p, "lib%s.so.%d", libName_p, major);
            lib_p = Nh_Loader_getLibraryHandle(libPath_p);
        }
    }

#endif

    if (lib_p == NULL && NH_LOADER.install) {
        if (Nh_Loader_installLibrary(libName_p) != NH_LOADER_SUCCESS) {NH_LOADER_END(NULL)}
        NH_LOADER_END(Nh_Loader_loadLibrary(libName_p, major))
    }

NH_LOADER_END(lib_p)
}

// CLOSE ===========================================================================================

NH_LOADER_RESULT Nh_Loader_unloadLibrary(
    void *lib_p)
{
NH_LOADER_BEGIN()

#ifdef __unix__

    if (dlclose(lib_p)) {
        NH_LOADER_DIAGNOSTIC_END(NH_LOADER_ERROR_BAD_STATE)
    }

#endif

NH_LOADER_DIAGNOSTIC_END(NH_LOADER_SUCCESS)
}


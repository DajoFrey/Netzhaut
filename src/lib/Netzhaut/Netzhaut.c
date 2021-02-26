// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "Netzhaut.h"

#include "../NhLoader/Loader.h"
#include "../NhCore/Netzhaut.h"

#include <unistd.h>
#include <dlfcn.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// DATA ============================================================================================

Nh_Loader *NH_LOADER_P = NULL;
static void *loader_p  = NULL;

// LOADER ==========================================================================================

static void *Nh_openLoaderLib(
    NH_BYTE *path_p)
{
#ifdef __unix__

    NH_BYTE *error_p;
    dlerror();

    void *dl_p = dlopen(path_p, RTLD_NOW | RTLD_GLOBAL);

    if ((error_p = dlerror()) != NULL) { 
        fprintf(stderr, "%s\n", error_p);
        return NULL;
    }

    return dl_p;

#endif
}

static NH_RESULT Nh_getExeDir(
    NH_BYTE *buffer_p, size_t size)
{
#ifdef __unix__

    if (readlink("/proc/self/exe", buffer_p, 1024) == -1 
    &&  readlink("/proc/curproc/file", buffer_p, 1024) == -1
    &&  readlink("/proc/self/path/a.out", buffer_p, 1024) == -1) {return NH_ERROR_BAD_STATE;}

    int i;
    for (i = strlen(buffer_p); i > -1 && buffer_p[i] != '/'; --i) {}
    if (i == -1) {return NH_ERROR_BAD_STATE;}

    buffer_p[i] = '\0'; // remove exe name
    return NH_SUCCESS;

#endif
}

static void *Nh_openLoader(
    NH_LOADER_SCOPE loaderScope)
{
#ifdef __unix__

    switch (loaderScope)
    {
        case NH_LOADER_SCOPE_LOCAL :
        {
            NH_BYTE exeDir_p[2048] = {'\0'};
            if (Nh_getExeDir(exeDir_p, 2048) != NH_SUCCESS) {return NULL;}
            NH_BYTE loaderPath_p[2048] = {'\0'};
            sprintf(loaderPath_p, "%s/Netzhaut-master/lib/libNhLoader.so.0", exeDir_p);
            return Nh_openLoaderLib(loaderPath_p);
        }
        case NH_LOADER_SCOPE_LOCAL_SYSTEM :
            return Nh_openLoaderLib("/usr/local/lib/libNhLoader.so.0");
        case NH_LOADER_SCOPE_SYSTEM :
            return Nh_openLoaderLib("libNhLoader.so.0");
    }

#endif

    return NULL;
}

static void *Nh_loadLoaderFunction(
    const NH_BYTE *functionName_p)
{
#ifdef __unix__

    NH_BYTE *error_p;
    dlerror(); // clear any existing error

    void *function_p = dlsym(loader_p, functionName_p); 
    if ((error_p = dlerror()) != NULL) {
        fprintf(stderr, "%s\n", error_p);
        return NULL;
    }

    return function_p;

#endif
}

// INIT LOADER =====================================================================================

NH_RESULT Nh_initLoader(
     NH_LOADER_SCOPE loaderScope, NH_BYTE *loaderPath_p, NH_BOOL install)
{
    loader_p = Nh_openLoader(loaderScope);

    NH_BOOL fallback = !loader_p && loaderPath_p != NULL;
    if (fallback) {loader_p = Nh_openLoaderLib(loaderPath_p);}

    if (!loader_p) {return NH_ERROR_BAD_STATE;}

    Nh_Loader_initLoader_f initLoader_f = Nh_loadLoaderFunction("Nh_Loader_initLoader");
    NH_LOADER_P = !initLoader_f ? NULL : initLoader_f(loaderScope, fallback, install);

    if (!NH_LOADER_P) {return NH_ERROR_BAD_STATE;}

    return NH_SUCCESS;
}
 
// INIT ============================================================================================

NH_RESULT Nh_init()
{
    Nh_init_f init_f = !NH_LOADER_P ? NULL : NH_LOADER_P->loadFunction_f("NhCore", 0, "Nh_init");
    return init_f ? init_f() : NH_ERROR_BAD_STATE;
}

// RUN =============================================================================================

unsigned int Nh_run()
{
    Nh_run_f run_f = !NH_LOADER_P ? NULL : NH_LOADER_P->loadFunction_f("NhCore", 0, "Nh_run");
    return run_f ? run_f() : 0;
}

// IS RUNNING ======================================================================================

NH_BOOL Nh_keepRunning()
{
    Nh_keepRunning_f keepRunning_f = !NH_LOADER_P ? NULL : NH_LOADER_P->loadFunction_f("NhCore", 0, "Nh_keepRunning");
    return keepRunning_f ? keepRunning_f() : NH_FALSE;
}

// RELEASE =========================================================================================

NH_RESULT Nh_release()
{
    Nh_release_f release_f = !NH_LOADER_P ? NULL : NH_LOADER_P->loadFunction_f("NhCore", 0, "Nh_release");
    return release_f ? release_f() : NH_ERROR_BAD_STATE;
}


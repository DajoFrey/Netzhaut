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

static NH_BOOL init = NH_FALSE;
static void *loader_p = NULL;

void *NH_LOADER_P = NULL;

// GET REPOSITORY ==================================================================================

static NH_BOOL Nh_canRunCommand(
    const NH_BYTE *cmd) 
{
#ifdef __unix__

    if(strchr(cmd, '/')) {
        return access(cmd, X_OK)==0;
    }
    const NH_BYTE *path = getenv("PATH");
    if(!path) {return NH_FALSE;}
    NH_BYTE *buf = malloc(strlen(path)+strlen(cmd)+3);
    if(!buf) {return NH_FALSE;}
    for(; *path; ++path) {
        NH_BYTE *p = buf;
        for(; *path && *path!=':'; ++path,++p) {
            *p = *path;
        }
        if(p==buf) *p++='.';
        if(p[-1]!='/') *p++='/';
        strcpy(p, cmd);
        if(access(buf, X_OK)==0) {
            free(buf);
            return NH_TRUE;
        }
        if(!*path) break;
    }
    free(buf);

    return NH_FALSE;

#endif
}

static NH_RESULT Nh_getExeDir(
    NH_BYTE *buffer_p, size_t size)
{
    if (readlink("/proc/self/exe", buffer_p, 1024) == -1 
    &&  readlink("/proc/curproc/file", buffer_p, 1024) == -1
    &&  readlink("/proc/self/path/a.out", buffer_p, 1024) == -1) {return NH_ERROR_BAD_STATE;}

    int i;
    for (i = strlen(buffer_p); i > -1 && buffer_p[i] != '/'; --i) {}
    if (i == -1) {return NH_ERROR_BAD_STATE;}

    buffer_p[i] = '\0'; // remove exe name
    return NH_SUCCESS;
}

static NH_RESULT Nh_downloadProjectFromGithub(
    const NH_BYTE *owner_p, const NH_BYTE *repo_p)
{
    if (!Nh_canRunCommand("wget")) {return NH_ERROR_BAD_STATE;}
    if (!Nh_canRunCommand("unzip")) {return NH_ERROR_BAD_STATE;}

    NH_BYTE exeDir_p[2048] = {'\0'};
    if (Nh_getExeDir(exeDir_p, 2048) != NH_SUCCESS) {return NH_ERROR_BAD_STATE;}

    NH_BYTE zip_p[2048] = {'\0'};
    sprintf(zip_p, "%s%s", exeDir_p, "Netzhaut.zip");

    NH_BYTE command_p[2048] = {'\0'};
    sprintf(command_p, "wget -O %s https://github.com/%s/%s/archive/master.zip", zip_p, owner_p, repo_p);
    int status = system(command_p);
    if (WEXITSTATUS(status) || WIFSIGNALED(status)) {return NH_ERROR_BAD_STATE;}

    memset(command_p, '\0', sizeof(NH_BYTE) * 2048);
    sprintf(command_p, "unzip -o %s -d %s", zip_p, exeDir_p);
    status = system(command_p);
    if (WEXITSTATUS(status) || WIFSIGNALED(status)) {return NH_ERROR_BAD_STATE;}

    remove(zip_p);

    return NH_SUCCESS;
}

static NH_RESULT Nh_buildInstaller()
{
#ifdef __unix__

    NH_BYTE command_p[2048] = {'\0'};
    memset(command_p, '\0', sizeof(NH_BYTE) * 2048);
    sprintf(command_p, "./Netzhaut-master/data/installer/build.sh");
    int status = system(command_p);
    if (WEXITSTATUS(status) || WIFSIGNALED(status)) {return NH_ERROR_BAD_STATE;}
    return NH_SUCCESS;

#endif
}

static NH_RESULT Nh_buildLoader()
{
#ifdef __unix__

    NH_BYTE command_p[2048] = {'\0'};
    memset(command_p, '\0', sizeof(NH_BYTE) * 2048);
    sprintf(command_p, "./Netzhaut-master/bin/installer/NhInstaller -l loader");
    int status = system(command_p);
    if (WEXITSTATUS(status) || WIFSIGNALED(status)) {return NH_ERROR_BAD_STATE;}
    return NH_SUCCESS;

#endif
}

// HELPER ==========================================================================================

static void *Nh_openLoader(
    NH_BYTE *path_p)
{
#ifdef __unix__

    NH_BYTE *error_p;
    dlerror();

    void *dl_p = dlopen("libNhLoader.so.0", RTLD_NOW | RTLD_GLOBAL);

    if ((error_p = dlerror()) != NULL) { 
        fprintf(stderr, "%s\n", error_p);
        return NULL;
    }

    return dl_p;

#endif
}

static void *Nh_loadLoaderFunction(
    const NH_BYTE *functionName_p)
{
    NH_BYTE *error_p;
    dlerror(); // clear any existing error

    void *function_p = dlsym(loader_p, functionName_p); 
    if ((error_p = dlerror()) != NULL) {
        fprintf(stderr, "%s\n", error_p);
        return NULL;
    }

    return function_p;
}

// INIT ============================================================================================

NH_RESULT Nh_init(
    NH_LOADER_BEHAVIOR loaderBehavior)
{
    if (init == NH_TRUE) {return NH_ERROR_BAD_STATE;}

    loader_p = Nh_openLoader("libNhLoader.so.0");
    if (!loader_p) {
        if (loaderBehavior > NH_LOADER_BEHAVIOR_NO_INSTALL) {
            if (Nh_downloadProjectFromGithub("DajoFrey", "Netzhaut") != NH_SUCCESS) {
                return NH_ERROR_BAD_STATE;
            }
            if (Nh_buildInstaller() != NH_SUCCESS) {
                return NH_ERROR_BAD_STATE;
            }
            if (Nh_buildLoader() != NH_SUCCESS) {
                return NH_ERROR_BAD_STATE;
            }
            exit(0);
//            Nh_installLoader(loaderBehavior, ) == NH_SUCCESS) {
//                loader_p = 
//            }
        }
    }
    if (!loader_p) {return NH_ERROR_BAD_STATE;}

    NH_LOADER_P = ((Nh_Loader_getLoader_f)Nh_loadLoaderFunction("Nh_Loader_getLoader"))();
    Nh_Loader *Loader_p = NH_LOADER_P;

    Loader_p->load_f = Nh_loadLoaderFunction("Nh_Loader_load"); 
    if (!Loader_p->load_f) {return NH_ERROR_BAD_STATE;}

    Loader_p->unload_f = Nh_loadLoaderFunction("Nh_Loader_unload");
    if (!Loader_p->unload_f) {return NH_ERROR_BAD_STATE;}

    Loader_p->isLoaded_f = Nh_loadLoaderFunction("Nh_Loader_isLoaded");
    if (!Loader_p->isLoaded_f) {return NH_ERROR_BAD_STATE;}

    Loader_p->loadFunction_f = Nh_loadLoaderFunction("Nh_Loader_loadFunction");
    if (!Loader_p->loadFunction_f) {return NH_ERROR_BAD_STATE;}

    Loader_p->getModifiedLib_f = Nh_loadLoaderFunction("Nh_Loader_getModifiedLib");
    if (!Loader_p->getModifiedLib_f) {return NH_ERROR_BAD_STATE;}

    if (Loader_p->load_f("NhCore", 0, NH_TRUE) != NH_SUCCESS) {return NH_ERROR_BAD_STATE;}

    if (((Nh_init_f)Loader_p->loadFunction_f("NhCore", 0, "Nh_init"))() != NH_SUCCESS) {
        return NH_ERROR_BAD_STATE;
    }

    init = NH_TRUE;

    return NH_SUCCESS;
}

// RUN =============================================================================================

unsigned int Nh_run()
{
    if (init == NH_FALSE) {return 0;}
    return ((Nh_run_f)((Nh_Loader*)NH_LOADER_P)->loadFunction_f("NhCore", 0, "Nh_run"))();
}

// IS RUNNING ======================================================================================

NH_BOOL Nh_keepRunning()
{
    if (init == NH_FALSE) {return NH_FALSE;}
    return ((Nh_keepRunning_f)((Nh_Loader*)NH_LOADER_P)->loadFunction_f("NhCore", 0, "Nh_keepRunning"))();
}

// RELEASE =========================================================================================

NH_RESULT Nh_release()
{
    if (init == NH_FALSE) {return NH_USAGE_ERROR_INIT_MISSING;}
    return ((Nh_release_f)((Nh_Loader*)NH_LOADER_P)->loadFunction_f("NhCore", 0, "Nh_release"))();
}


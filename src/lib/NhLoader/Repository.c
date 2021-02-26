// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#define _GNU_SOURCE

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
#include <unistd.h>
#include <sys/stat.h>

// DOWNLOAD REPOSITORY =============================================================================

static NH_BOOL Nh_Loader_canRunCommand(
    const NH_BYTE *cmd) 
{
NH_LOADER_BEGIN()

#ifdef __unix__

    if(strchr(cmd, '/')) {
        NH_LOADER_END(access(cmd, X_OK)==0);
    }
    const NH_BYTE *path = getenv("PATH");
    if(!path) {NH_LOADER_END(NH_FALSE)}
    NH_BYTE *buf = malloc(strlen(path)+strlen(cmd)+3);
    if(!buf) {NH_LOADER_END(NH_FALSE)}
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
            NH_LOADER_END(NH_TRUE)
        }
        if(!*path) break;
    }
    free(buf);

#endif

NH_LOADER_END(NH_FALSE)
}

NH_LOADER_RESULT Nh_Loader_getExeDir(
    NH_BYTE *buffer_p, unsigned int size)
{
NH_LOADER_BEGIN()

#ifdef __unix__

    if (readlink("/proc/self/exe", buffer_p, 1024) == -1 
    &&  readlink("/proc/curproc/file", buffer_p, 1024) == -1
    &&  readlink("/proc/self/path/a.out", buffer_p, 1024) == -1) {NH_LOADER_DIAGNOSTIC_END(NH_LOADER_ERROR_BAD_STATE)}

    int i;
    for (i = strlen(buffer_p); i > -1 && buffer_p[i] != '/'; --i) {}
    if (i == -1) {NH_LOADER_DIAGNOSTIC_END(NH_LOADER_ERROR_BAD_STATE)}

    buffer_p[i] = '\0'; // remove exe name

#endif

NH_LOADER_DIAGNOSTIC_END(NH_LOADER_SUCCESS)
}

NH_LOADER_RESULT Nh_Loader_downloadNetzhaut()
{
NH_LOADER_BEGIN()

#ifdef __unix__

    if (!Nh_Loader_canRunCommand("wget"))  {NH_LOADER_DIAGNOSTIC_END(NH_LOADER_ERROR_BAD_STATE)}
    if (!Nh_Loader_canRunCommand("unzip")) {NH_LOADER_DIAGNOSTIC_END(NH_LOADER_ERROR_BAD_STATE)}

    NH_BYTE exeDir_p[2048] = {'\0'};
    if (Nh_Loader_getExeDir(exeDir_p, 2048) != NH_LOADER_SUCCESS) {NH_LOADER_DIAGNOSTIC_END(NH_LOADER_ERROR_BAD_STATE)}

    NH_BYTE zip_p[2048] = {'\0'};
    sprintf(zip_p, "%s%s", exeDir_p, "Netzhaut.zip");

    NH_BYTE command_p[2048] = {'\0'};
    sprintf(command_p, "wget -O %s https://github.com/DajoFrey/Netzhaut/archive/master.zip", zip_p);
    int status = system(command_p);
    if (WEXITSTATUS(status) || WIFSIGNALED(status)) {NH_LOADER_DIAGNOSTIC_END(NH_LOADER_ERROR_BAD_STATE)}

    memset(command_p, '\0', sizeof(NH_BYTE) * 2048);
    sprintf(command_p, "unzip -o %s -d %s", zip_p, exeDir_p);
    status = system(command_p);
    if (WEXITSTATUS(status) || WIFSIGNALED(status)) {NH_LOADER_DIAGNOSTIC_END(NH_LOADER_ERROR_BAD_STATE)}

    remove(zip_p);

#endif

NH_LOADER_DIAGNOSTIC_END(NH_LOADER_SUCCESS)
}

// BUILD INSTALLER =================================================================================

NH_LOADER_RESULT Nh_Loader_buildNhInstaller()
{
NH_LOADER_BEGIN()

#ifdef __unix__

    NH_BYTE command_p[2048] = {'\0'};
    memset(command_p, '\0', sizeof(NH_BYTE) * 2048);
    sprintf(command_p, "./Netzhaut-master/data/installer/build.sh");
    int status = system(command_p);
    if (WEXITSTATUS(status) || WIFSIGNALED(status)) {NH_LOADER_DIAGNOSTIC_END(NH_LOADER_ERROR_BAD_STATE)}

#endif

NH_LOADER_DIAGNOSTIC_END(NH_LOADER_SUCCESS)
}


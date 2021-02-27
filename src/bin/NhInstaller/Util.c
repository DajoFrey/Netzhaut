// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "Util.h"
#include "Main.h"
#include "Message.h"

#include "Common/Macro.h"

#include NH_INSTALLER_FLOW
#include NH_INSTALLER_DEFAULT_CHECK

#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <stdio.h>
#include <pwd.h>

#ifdef __unix__
    #include <sys/stat.h>
    #include <sys/types.h>
#endif

// DIRECTORY =======================================================================================

NH_INSTALLER_RESULT Nh_Installer_getExeDir(
    NH_BYTE *buffer_p, size_t size)
{
NH_INSTALLER_BEGIN()

    if (readlink("/proc/self/exe", buffer_p, 1024) == -1 
    &&  readlink("/proc/curproc/file", buffer_p, 1024) == -1
    &&  readlink("/proc/self/path/a.out", buffer_p, 1024) == -1) {NH_INSTALLER_DIAGNOSTIC_END(NH_INSTALLER_ERROR_BAD_STATE)}

    int i;
    for (i = strlen(buffer_p); i > -1 && buffer_p[i] != '/'; --i) {}
    if (i == -1) {NH_INSTALLER_DIAGNOSTIC_END(NH_INSTALLER_ERROR_BAD_STATE)}
    buffer_p[i] = '\0'; // remove exe name

NH_INSTALLER_DIAGNOSTIC_END(NH_INSTALLER_SUCCESS)
}

NH_INSTALLER_RESULT Nh_Installer_getProjectDir(
    NH_BYTE *buffer_p, size_t size)
{
NH_INSTALLER_BEGIN()

    if (Nh_Installer_getExeDir(buffer_p, size) != NH_INSTALLER_SUCCESS) {NH_INSTALLER_DIAGNOSTIC_END(NH_INSTALLER_ERROR_BAD_STATE)}

    int i;
    for (i = strlen(buffer_p); i > -1 && buffer_p[i] != '/'; --i) {}
    if (i == -1) {NH_INSTALLER_DIAGNOSTIC_END(NH_INSTALLER_ERROR_BAD_STATE)}
    buffer_p[i] = '\0';

NH_INSTALLER_DIAGNOSTIC_END(NH_INSTALLER_SUCCESS)
}

const NH_BYTE *Nh_Installer_getHomeDir()
{
NH_INSTALLER_BEGIN()

    const NH_BYTE *homedir_p = NULL;
    if ((homedir_p = getenv("HOME")) == NULL) {
        homedir_p = getpwuid(getuid())->pw_dir;
    }

NH_INSTALLER_END(homedir_p)
}

NH_INSTALLER_RESULT Nh_Installer_createDir(
    const NH_BYTE *path_p)
{
NH_INSTALLER_BEGIN()

    NH_BYTE wrkDir_p[2048] = {'\0'};
    getcwd(wrkDir_p, 2048);

    Nh_Installer_messagef("CREATE DIR %s/%s", wrkDir_p, path_p);
 
    struct stat st = {0};
    if (stat(path_p, &st) == -1) {
        int err = mkdir(path_p, 0755);  // 0755 aka drwxr-xr-x
        if (err != 0) {
            NH_INSTALLER_DIAGNOSTIC_END(NH_INSTALLER_ERROR_CANT_CREATE_DIRECTORY)
        }
    }

NH_INSTALLER_DIAGNOSTIC_END(NH_INSTALLER_SUCCESS)
}

// COMMAND =========================================================================================

NH_BOOL Nh_Installer_canRunCommand(
    const NH_BYTE *cmd) 
{
NH_INSTALLER_BEGIN()

    if(strchr(cmd, '/')) {
        // if cmd includes a slash, no path search must be performed,
        // go straight to checking if it's executable
        return access(cmd, X_OK)==0;
    }
    const NH_BYTE *path = getenv("PATH");
    if(!path) {NH_INSTALLER_DIAGNOSTIC_END(NH_FALSE)} // something is horribly wrong...
    // we are sure we won't need a buffer any longer
    NH_BYTE *buf = malloc(strlen(path)+strlen(cmd)+3);
    if(!buf) {NH_INSTALLER_DIAGNOSTIC_END(NH_FALSE)} // actually useless, see comment
    // loop as long as we have stuff to examine in path
    for(; *path; ++path) {
        // start from the beginning of the buffer
        NH_BYTE *p = buf;
        // copy in buf the current path element
        for(; *path && *path!=':'; ++path,++p) {
            *p = *path;
        }
        // empty path entries are treated like "."
        if(p==buf) *p++='.';
        // slash and command name
        if(p[-1]!='/') *p++='/';
        strcpy(p, cmd);
        // check if we can execute it
        if(access(buf, X_OK)==0) {
            free(buf);
            NH_INSTALLER_DIAGNOSTIC_END(NH_TRUE)
        }
        // quit at last cycle
        if(!*path) break;
    }
    // not found
    free(buf);

NH_INSTALLER_DIAGNOSTIC_END(NH_FALSE)
}

// FILE ============================================================================================

NH_BOOL Nh_Installer_canFindSharedLib(
    const NH_BYTE *lib_p)
{
NH_INSTALLER_BEGIN()

    NH_BYTE path_p[1024] = {'\0'};
    sprintf(path_p, "/usr/lib/lib%s.so", lib_p);

    if(access(path_p, F_OK) != -1) {NH_INSTALLER_DIAGNOSTIC_END(NH_TRUE)}

NH_INSTALLER_DIAGNOSTIC_END(NH_FALSE)
}

NH_INSTALLER_RESULT Nh_Installer_createPICObjectFileUsingGCC(
    const NH_BYTE *in_p, const NH_BYTE *out_p, NH_BYTE *extra_p)
{
NH_INSTALLER_BEGIN()

    NH_BYTE realout_p[1024] = {'\0'};
    realpath(out_p, realout_p);
    Nh_Installer_messagef("CREATE FILE %s", realout_p);

    NH_BYTE command_p[1024] = {'\0'};

    if (extra_p != NULL) {sprintf(command_p, "gcc -fPIC %s -c %s -o %s", extra_p, in_p, out_p);}
    else {sprintf(command_p, "gcc -fPIC -c %s -o %s", in_p, out_p);}

    int status = system(command_p);
    if (WEXITSTATUS(status) || WIFSIGNALED(status)) {NH_INSTALLER_DIAGNOSTIC_END(NH_INSTALLER_ERROR_GCC_EXECUTION_FAILED)}

NH_INSTALLER_DIAGNOSTIC_END(NH_INSTALLER_SUCCESS)
}

NH_INSTALLER_RESULT Nh_Installer_createSharedLibraryUsingGCC(
    const NH_BYTE *objects_p, const NH_BYTE *out_p, const NH_BYTE *extra_p)
{
NH_INSTALLER_BEGIN()

    static NH_BYTE command_p[16384] = {'\0'};
    sprintf(command_p, "gcc -shared %s %s -o %s", objects_p, extra_p, out_p);

    int status = system(command_p);
    if (WEXITSTATUS(status) || WIFSIGNALED(status)) {NH_INSTALLER_DIAGNOSTIC_END(NH_INSTALLER_ERROR_GCC_EXECUTION_FAILED)}

NH_INSTALLER_DIAGNOSTIC_END(NH_INSTALLER_SUCCESS)
}

NH_INSTALLER_RESULT Nh_Installer_copy(
    NH_BYTE *in_p, NH_BYTE *out_p, NH_BOOL recursive, NH_BOOL toAbsolute, NH_BOOL sudo)
{
NH_INSTALLER_BEGIN()

    NH_BYTE realin_p[1024] = {'\0'}, realout_p[1024] = {'\0'};
    realpath(out_p, realout_p);
    realpath(in_p, realin_p);
    Nh_Installer_messagef(sudo ? "COPY %s TO %s \e[1;31mSUDO PREPENDED\e[0m" : "COPY %s TO %s", realin_p, realout_p);

#include NH_INSTALLER_CUSTOM_CHECK

    NH_BYTE projDir_p[2048] = {'\0'};
    NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_GET_PROJECT_DIRECTORY, Nh_Installer_getProjectDir(projDir_p, 2048))

#include NH_INSTALLER_DEFAULT_CHECK

    if (!Nh_Installer_canRunCommand("cp")) {NH_INSTALLER_DIAGNOSTIC_END(NH_INSTALLER_ERROR_CP_NOT_FOUND)}

#ifdef __unix__

    NH_BYTE command_p[32] = {'\0'};
    sprintf(command_p, sudo ? "sudo cp" : "cp");
    NH_BYTE fullCommand_p[1024] = {'\0'};

    if (!toAbsolute) {
        sprintf(fullCommand_p, recursive ? "%s -rp %s/%s %s/%s" : "%s -p %s/%s %s/%s", command_p, projDir_p, in_p, projDir_p, out_p);
    }
    else {
        sprintf(fullCommand_p, recursive ? "%s -rp %s/%s %s" : "%s -p %s/%s %s", command_p, projDir_p, in_p, out_p);
    }

    int status = system(fullCommand_p);
    if (WEXITSTATUS(status) || WIFSIGNALED(status)) {NH_INSTALLER_DIAGNOSTIC_END(NH_INSTALLER_ERROR_CP_EXECUTION_FAILED)}

#elif defined(WIN_32)

#endif

NH_INSTALLER_DIAGNOSTIC_END(NH_INSTALLER_SUCCESS)
}

NH_INSTALLER_RESULT Nh_Installer_createSymLink(
    NH_BYTE *filePath_p, NH_BYTE *symLinkPath_p, NH_BOOL sudo)
{
NH_INSTALLER_BEGIN()

#ifdef __unix__

    if (!Nh_Installer_canRunCommand("ln")) {NH_INSTALLER_DIAGNOSTIC_END(NH_INSTALLER_ERROR_CP_NOT_FOUND)}

    NH_BYTE command_p[64] = {'\0'};
    sprintf(command_p, sudo ? "sudo ln -s" : "ln -s");

    NH_BYTE fullCommand_p[1024] = {'\0'};
    sprintf(fullCommand_p, "%s %s %s", command_p, filePath_p, symLinkPath_p);

    int status = system(fullCommand_p);
    if (WEXITSTATUS(status) || WIFSIGNALED(status)) {NH_INSTALLER_DIAGNOSTIC_END(NH_INSTALLER_ERROR_CP_EXECUTION_FAILED)}

#elif defined(WIN_32)

#endif

NH_INSTALLER_DIAGNOSTIC_END(NH_INSTALLER_SUCCESS)
}

void *Nh_Installer_getFileData(
    const NH_BYTE* path_p, long *size_p)
{
NH_INSTALLER_BEGIN()

    FILE *fh = fopen(path_p, "rb");
    if (fh == NULL) {NH_INSTALLER_END(NULL)}
    
    if (fseek(fh, 0L, SEEK_END) != 0) {NH_INSTALLER_END(NULL)}

    long size = ftell(fh);

    if (fseek(fh, 0L, SEEK_SET) != 0) {NH_INSTALLER_END(NULL)}

    if(size <= 0) {
        fclose(fh);
        NH_INSTALLER_END(NULL)
    }

    NH_BYTE *data_p = (NH_BYTE*)malloc(((size_t)size) + 1); 
    if (data_p == NULL) {NH_INSTALLER_END(NULL)}
    
    fread(data_p, 1, size, fh);
    fclose(fh);

    data_p[size] = '\0';
    if (size_p != NULL) {*size_p = size;}

NH_INSTALLER_END(data_p)
}

NH_BOOL Nh_Installer_fileExists(
    NH_BYTE *filename_p)
{
NH_INSTALLER_BEGIN()
NH_INSTALLER_END(access(filename_p, F_OK) != -1 ? NH_TRUE : NH_FALSE)
}

NH_INSTALLER_RESULT Nh_Installer_writeCharsToFile(
    NH_BYTE *filename_p, NH_BYTE *NH_BYTEs_p)
{
NH_INSTALLER_BEGIN()

    NH_INSTALLER_CHECK_NULL(NH_BYTEs_p)
    NH_INSTALLER_CHECK_NULL(filename_p)

    FILE *f = fopen(filename_p, "w");
    NH_INSTALLER_CHECK_NULL(f)    
    
    fprintf(f, "%s\n", NH_BYTEs_p);
    fclose(f);

NH_INSTALLER_DIAGNOSTIC_END(NH_INSTALLER_SUCCESS)
}


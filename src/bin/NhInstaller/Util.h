#ifndef NH_INSTALLER_UTIL_H
#define NH_INSTALLER_UTIL_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "Common/Result.h"
#include "Common/API.h"

#include <stddef.h>

#endif

/** @addtogroup NhInstallerFunctions
 *  @{
 */

    NH_INSTALLER_RESULT Nh_Installer_getExeDir(
        char *buffer_p, size_t size
    );

    NH_INSTALLER_RESULT Nh_Installer_getProjectDir(
        char *buffer_p, size_t size
    );

    const char *Nh_Installer_getHomeDir(
    );

    NH_INSTALLER_RESULT Nh_Installer_createDir(
        const char *path_p
    );

    NH_BOOL Nh_Installer_canRunCommand(
        const char *cmd
    );

    NH_BOOL Nh_Installer_canFindSharedLib(
        const char *lib_p
    );

    NH_INSTALLER_RESULT Nh_Installer_createPICObjectFileUsingGCC(
        const char *in_p, const char *out_p, char *extra_p
    );

    NH_INSTALLER_RESULT Nh_Installer_createSharedLibraryUsingGCC(
        const char *objects_p, const char *out_p, const char *extra_p
    );

    NH_INSTALLER_RESULT Nh_Installer_copy(
        char *in_p, char *out_p, NH_BOOL recursive, NH_BOOL toAbsolute, NH_BOOL sudo
    );

    NH_INSTALLER_RESULT Nh_Installer_createSymLink(
        char *filePath_p, char *symLinkPath_p, NH_BOOL sudo
    );

    NH_BOOL Nh_Installer_fileExists(
        char *filename_p
    );

    void *Nh_Installer_getFileData(
        const char* path_p, long *size_p
    );

    NH_INSTALLER_RESULT Nh_Installer_writeCharsToFile(
        char *filename_p, char *chars_p
    );

/** @} */

#endif

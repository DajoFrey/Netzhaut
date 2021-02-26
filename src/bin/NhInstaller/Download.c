// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "Download.h"
#include "Util.h"

#include "Common/Macro.h"

#include NH_INSTALLER_FLOW
#include NH_INSTALLER_DEFAULT_CHECK

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// GITHUB ==========================================================================================

static NH_INSTALLER_RESULT Nh_Installer_downloadFromGithub(
    const char *owner_p, const char *repo_p)
{
NH_INSTALLER_BEGIN()

    if (!Nh_Installer_canRunCommand("wget")) {NH_INSTALLER_DIAGNOSTIC_END(NH_INSTALLER_ERROR_WGET_NOT_FOUND)}
    if (!Nh_Installer_canRunCommand("unzip")) {NH_INSTALLER_DIAGNOSTIC_END(NH_INSTALLER_ERROR_UNZIP_NOT_FOUND)}

#include NH_INSTALLER_CUSTOM_CHECK

    char projDir_p[2048] = {'\0'};
    NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_GET_PROJECT_DIRECTORY, Nh_Installer_getProjectDir(projDir_p, 2048))

    char zip_p[2048] = {'\0'};
    sprintf(zip_p, "%s%s", projDir_p, "tmp.zip");

    char command_p[2048] = {'\0'};
    sprintf(command_p, "wget -O %s https://github.com/%s/%s/archive/master.zip", zip_p, owner_p, repo_p);

    int status = system(command_p);
    if (WEXITSTATUS(status) || WIFSIGNALED(status)) {NH_INSTALLER_DIAGNOSTIC_END(NH_INSTALLER_ERROR_WGET_EXECUTION_FAILED)}

    memset(command_p, '\0', sizeof(char) * 2048);
    sprintf(command_p, "unzip -o %s -d %s/external/DOWNLOADS/", zip_p, projDir_p);

    status = system(command_p);
    if (WEXITSTATUS(status) || WIFSIGNALED(status)) {NH_INSTALLER_DIAGNOSTIC_END(NH_INSTALLER_ERROR_UNZIP_EXECUTION_FAILED)}

    remove(zip_p);

#include NH_INSTALLER_DEFAULT_CHECK

NH_INSTALLER_DIAGNOSTIC_END(NH_INSTALLER_SUCCESS)
}

// https://github.com/KhronosGroup/glslang.git
// https://github.com/g-truc/gli.git
// https://github.com/KhronosGroup/Vulkan-Headers.git
// https://github.com/zeux/volk.git
// https://git.sv.nongnu.org/r/freetype/freetype2.git

// DOWNLOAD ========================================================================================

NH_INSTALLER_RESULT Nh_Installer_downloadExternals()
{
NH_INSTALLER_BEGIN()

#include NH_INSTALLER_CUSTOM_CHECK

    if (!Nh_Installer_canFindSharedLib("freetype")) {
        NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_CANT_DOWNLOAD_FREETYPE, NH_INSTALLER_ERROR_NOT_IMPLEMENTED)
    }
    if (!Nh_Installer_canFindSharedLib("ssl")) { // part of OpenSSL
        NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_CANT_DOWNLOAD_OPENSSL, NH_INSTALLER_ERROR_NOT_IMPLEMENTED)
    }

    NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_CANT_DOWNLOAD_VULKAN_HEADERS, Nh_Installer_downloadFromGithub("KhronosGroup", "Vulkan-Headers"))
    NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_CANT_DOWNLOAD_VOLK, Nh_Installer_downloadFromGithub("zeux", "volk"))

#include NH_INSTALLER_DEFAULT_CHECK

NH_INSTALLER_DIAGNOSTIC_END(NH_INSTALLER_SUCCESS)
}


// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "Example.h"
#include "Util.h"
#include "Main.h"

#include "Common/Macro.h"

#include NH_INSTALLER_FLOW
#include NH_INSTALLER_CUSTOM_CHECK

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// BUILD ===========================================================================================

NH_INSTALLER_RESULT Nh_Installer_buildTerminal()
{
NH_INSTALLER_BEGIN()

    char projDir_p[2048] = {'\0'};
    NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_GET_PROJECT_DIRECTORY, Nh_Installer_getProjectDir(projDir_p, 2048))

    // set -no-pie because of https://stackoverflow.com/questions/41398444/gcc-creates-mime-type-application-x-sharedlib-instead-of-application-x-applicati
    static char command_p[2048] = {'\0'};
    if (NH_INSTALLER_RELEASE) {
        sprintf(command_p, "gcc -std=gnu99 -Wl,-rpath=/usr/local/lib -o%s/bin/NhTerminal -no-pie -L%s/lib -lNetzhaut %s/src/bin/NhTerminal/Terminal.c", projDir_p, projDir_p, projDir_p, projDir_p);
    }
    else {
        sprintf(command_p, "gcc -std=gnu99 -Wl,-rpath=%s/lib:/usr/local/lib -o%s/bin/NhTerminal -no-pie -L%s/lib -lNetzhaut %s/src/bin/NhTerminal/Terminal.c", projDir_p, projDir_p, projDir_p, projDir_p);
    }

    int status = system(command_p);
    if (WEXITSTATUS(status) || WIFSIGNALED(status)) {NH_INSTALLER_DIAGNOSTIC_END(NH_INSTALLER_ERROR_GCC_EXECUTION_FAILED)}

NH_INSTALLER_DIAGNOSTIC_END(NH_INSTALLER_SUCCESS)
}

NH_INSTALLER_RESULT Nh_Installer_buildWebBrowser()
{
NH_INSTALLER_BEGIN()

    char projDir_p[2048] = {'\0'};
    NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_GET_PROJECT_DIRECTORY, Nh_Installer_getProjectDir(projDir_p, 2048))

    // set -no-pie because of https://stackoverflow.com/questions/41398444/gcc-creates-mime-type-application-x-sharedlib-instead-of-application-x-applicati
    static char command_p[2048] = {'\0'};
    sprintf(command_p, "gcc -std=gnu99 -Wl,-rpath=%s/lib:/usr/local/lib -o%s/bin/NhWebBrowser -no-pie -L%s/lib -lNetzhaut %s/src/bin/NhWebBrowser/WebBrowser.c", projDir_p, projDir_p, projDir_p, projDir_p);

    int status = system(command_p);
    if (WEXITSTATUS(status) || WIFSIGNALED(status)) {NH_INSTALLER_DIAGNOSTIC_END(NH_INSTALLER_ERROR_GCC_EXECUTION_FAILED)}

NH_INSTALLER_DIAGNOSTIC_END(NH_INSTALLER_SUCCESS)
}

// INSTALL_ALL =========================================================================================

NH_INSTALLER_RESULT Nh_Installer_installTerminal()
{
NH_INSTALLER_BEGIN()

    char wrkDir_p[2048] = {'\0'};
    NH_INSTALLER_CHECK_NULL(NH_INSTALLER_ERROR_GET_WORK_DIRECTORY, getcwd(wrkDir_p, 2048))

    char projDir_p[2048] = {'\0'};
    NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_GET_PROJECT_DIRECTORY, Nh_Installer_getProjectDir(projDir_p, 2048))

    chdir(projDir_p);

    const char *homedir_p = Nh_Installer_getHomeDir();
    char dest_p[512] = {'\0'};

    sprintf(dest_p, "%s/.local/share/applications/", homedir_p);
    NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_COPY_FAILED, Nh_Installer_copy("data/examples/NhTerminal/NhTerminal.desktop", dest_p, NH_FALSE, NH_TRUE, NH_FALSE))
    NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_COPY_FAILED, Nh_Installer_copy("bin/NhTerminal", "/usr/local/bin", NH_FALSE, NH_TRUE, NH_TRUE))

    chdir(wrkDir_p);

NH_INSTALLER_DIAGNOSTIC_END(NH_INSTALLER_SUCCESS)
}

NH_INSTALLER_RESULT Nh_Installer_installWebBrowser()
{
NH_INSTALLER_BEGIN()

    char wrkDir_p[2048] = {'\0'};
    NH_INSTALLER_CHECK_NULL(NH_INSTALLER_ERROR_GET_WORK_DIRECTORY, getcwd(wrkDir_p, 2048))

    char projDir_p[2048] = {'\0'};
    NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_GET_PROJECT_DIRECTORY, Nh_Installer_getProjectDir(projDir_p, 2048))

    chdir(projDir_p);

    const char *homedir_p = Nh_Installer_getHomeDir();
    char dest_p[512] = {'\0'};

    sprintf(dest_p, "%s/.local/share/applications/", homedir_p);
    NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_COPY_FAILED, Nh_Installer_copy("data/examples/NhWebBrowser/NhWebBrowser.desktop", dest_p, NH_FALSE, NH_TRUE, NH_FALSE))
    NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_COPY_FAILED, Nh_Installer_copy("bin/NhWebBrowser", "/usr/local/bin", NH_FALSE, NH_TRUE, NH_TRUE))

    chdir(wrkDir_p);

NH_INSTALLER_DIAGNOSTIC_END(NH_INSTALLER_SUCCESS)
}

NH_INSTALLER_RESULT Nh_Installer_installLogo()
{
NH_INSTALLER_BEGIN()

    char wrkDir_p[2048] = {'\0'};
    NH_INSTALLER_CHECK_NULL(NH_INSTALLER_ERROR_GET_WORK_DIRECTORY, getcwd(wrkDir_p, 2048))

    char projDir_p[2048] = {'\0'};
    NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_GET_PROJECT_DIRECTORY, Nh_Installer_getProjectDir(projDir_p, 2048))

    chdir(projDir_p);

    const char *homedir_p = Nh_Installer_getHomeDir();
    char dest_p[512] = {'\0'};

    sprintf(dest_p, "%s/.local/share/icons/hicolor/32x32/apps", homedir_p);
    NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_COPY_FAILED, Nh_Installer_copy("data/logo/32x32/netzhaut.png", dest_p, NH_FALSE, NH_TRUE, NH_TRUE))

    memset(dest_p, '\0', sizeof(char) * 512);
    sprintf(dest_p, "%s/.local/share/icons/hicolor/64x64/apps", homedir_p);
    NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_COPY_FAILED, Nh_Installer_copy("data/logo/64x64/netzhaut.png", dest_p, NH_FALSE, NH_TRUE, NH_TRUE))

    memset(dest_p, '\0', sizeof(char) * 512);
    sprintf(dest_p, "%s/.local/share/icons/hicolor/128x128/apps", homedir_p);
    NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_COPY_FAILED, Nh_Installer_copy("data/logo/128x128/netzhaut.png", dest_p, NH_FALSE, NH_TRUE, NH_TRUE))

    memset(dest_p, '\0', sizeof(char) * 512);
    sprintf(dest_p, "%s/.local/share/icons/hicolor/256x256/apps", homedir_p);
    NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_COPY_FAILED, Nh_Installer_copy("data/logo/256x256/netzhaut.png", dest_p, NH_FALSE, NH_TRUE, NH_TRUE))

    memset(dest_p, '\0', sizeof(char) * 512);
    sprintf(dest_p, "%s/.local/share/icons/hicolor/512x512/apps", homedir_p);
    NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_COPY_FAILED, Nh_Installer_copy("data/logo/512x512/netzhaut.png", dest_p, NH_FALSE, NH_TRUE, NH_TRUE))

    chdir(wrkDir_p);

NH_INSTALLER_DIAGNOSTIC_END(NH_INSTALLER_SUCCESS)
}


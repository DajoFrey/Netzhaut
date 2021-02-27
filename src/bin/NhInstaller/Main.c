// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "Main.h"
#include "Documents.h"
#include "Util.h"
#include "Download.h" 
#include "Binaries.h"
#include "GUI.h"
#include "Libraries.h"
#include "Message.h"

#include "Common/Macro.h"

#include NH_INSTALLER_FLOW
#include NH_INSTALLER_DEFAULT_CHECK

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// DATA ============================================================================================

static NH_BOOL RUN = NH_FALSE;

// libs
static NH_BOOL BUILD_A_LIBRARY     = NH_FALSE;
static NH_BOOL BUILD_ALL_LIBRARIES = NH_FALSE;

static NH_BOOL LIBRARY_NETZHAUT   = NH_FALSE;
static NH_BOOL LIBRARY_LOADER     = NH_FALSE;
static NH_BOOL LIBRARY_CORE       = NH_FALSE;
static NH_BOOL LIBRARY_EXTERNAL   = NH_FALSE;
static NH_BOOL LIBRARY_IO         = NH_FALSE;
static NH_BOOL LIBRARY_TTY        = NH_FALSE;
static NH_BOOL LIBRARY_NETWORK    = NH_FALSE;
static NH_BOOL LIBRARY_HTML       = NH_FALSE;
static NH_BOOL LIBRARY_ECMASCRIPT = NH_FALSE;
static NH_BOOL LIBRARY_WEBIDL     = NH_FALSE;

// bins
static NH_BOOL BUILD_A_BINARY     = NH_FALSE;
static NH_BOOL BUILD_ALL_BINARIES = NH_FALSE;

static NH_BOOL BINARY_WEB_BROWSER   = NH_FALSE;
static NH_BOOL BINARY_TERMINAL      = NH_FALSE;
static NH_BOOL BINARY_WINDOW_SYSTEM = NH_FALSE;

// install
static NH_BOOL INSTALL_ALL_LIBRARIES = NH_FALSE;
static NH_BOOL INSTALL_ALL_BINARIES  = NH_FALSE;

// other
static NH_BOOL GENERATE_DOCS  = NH_FALSE;
static NH_BOOL VULKAN_SHADERS = NH_FALSE;
static NH_BOOL OFFLINE        = NH_FALSE;

NH_BOOL NH_INSTALLER_QUIET        = NH_FALSE;
NH_BOOL NH_INSTALLER_FLOW_LOGGING = NH_FALSE;
NH_BOOL NH_INSTALLER_GUI          = NH_FALSE;

// PARSE INPUT =====================================================================================

static NH_INSTALLER_RESULT Nh_Installer_parseInput(
    int argc, char **argv_pp)
{
NH_INSTALLER_BEGIN()

    NH_BOOL libs = NH_FALSE, bins = NH_FALSE;
    NH_BOOL negate = NH_FALSE;

    for (int i = 1; i < argc; ++i) 
    {
        if (argv_pp[i][0] == '-' && argv_pp[i][1] != '-')
        {
            negate = NH_FALSE, libs = NH_FALSE, bins = NH_FALSE;

            if (strstr(argv_pp[i], "n")) {
                negate = NH_TRUE;
            }
            if (strstr(argv_pp[i], "l")) {
                RUN = libs = NH_TRUE;
                BUILD_ALL_LIBRARIES = NH_TRUE;
                if (negate) {
                    LIBRARY_NETZHAUT   = NH_TRUE;
                    LIBRARY_LOADER     = NH_TRUE;
                    LIBRARY_CORE       = NH_TRUE;
                    LIBRARY_IO         = NH_TRUE;
                    LIBRARY_TTY        = NH_TRUE;
                    LIBRARY_NETWORK    = NH_TRUE;
                    LIBRARY_ECMASCRIPT = NH_TRUE;
                    LIBRARY_HTML       = NH_TRUE;
                    LIBRARY_WEBIDL     = NH_TRUE;
                }
            }
            if (strstr(argv_pp[i], "b")) {
                RUN = bins = NH_TRUE; 
                BUILD_ALL_BINARIES = NH_TRUE;
            }
            if (strstr(argv_pp[i], "i")) {
                if (libs) {INSTALL_ALL_LIBRARIES = NH_TRUE;}
                if (bins) {INSTALL_ALL_BINARIES = NH_TRUE;}
                RUN = NH_TRUE; 
            }
            if (!strstr(argv_pp[i], "l")
            &&  !strstr(argv_pp[i], "b")) {
                Nh_Installer_noticef("Invalid option \"%s\"", argv_pp[i]);
                NH_INSTALLER_END(NH_INSTALLER_ERROR_INVALID_OPTION)
            }
            continue;
        }

        if (argv_pp[i][0] == '-' && argv_pp[i][1] == '-')
        {
            if (!strcmp(argv_pp[i] + 2, "docs")) {
                RUN = NH_TRUE; 
                libs = bins = NH_FALSE; 
                GENERATE_DOCS = NH_TRUE;
            }
            else if (!strcmp(argv_pp[i] + 2, "flow")) {
                libs = bins = NH_FALSE; 
                NH_INSTALLER_FLOW_LOGGING = NH_TRUE;
            }
            else if (!strcmp(argv_pp[i] + 2, "offline")) {
                libs = bins = NH_FALSE; 
                OFFLINE = NH_TRUE;
            }
            else if (!strcmp(argv_pp[i] + 2, "version")) {
                RUN = NH_FALSE; 
                Nh_Installer_printVersion(); break;
            }
            else if (!strcmp(argv_pp[i] + 2, "help")) {
                RUN = NH_FALSE; 
                Nh_Installer_printHelp(); break;
            }
            else if (!strcmp(argv_pp[i] + 2, "quiet")) {
                libs = bins = NH_FALSE; 
                NH_INSTALLER_QUIET = NH_TRUE;
            }
            else {
                Nh_Installer_noticef("Invalid option \"%s\"", argv_pp[i]);
                NH_INSTALLER_END(NH_INSTALLER_ERROR_INVALID_OPTION)
            }
            continue;
        }

        if (libs)
        {
            if (strcmp(argv_pp[i], "netzhaut")
            &&  strcmp(argv_pp[i], "loader")
            &&  strcmp(argv_pp[i], "core")
            &&  strcmp(argv_pp[i], "io")
            &&  strcmp(argv_pp[i], "tty")
            &&  strcmp(argv_pp[i], "network")
            &&  strcmp(argv_pp[i], "html")
            &&  strcmp(argv_pp[i], "webidl")
            &&  strcmp(argv_pp[i], "ecmascript")) {
                Nh_Installer_noticef("Invalid option \"%s\"", argv_pp[i]);
                NH_INSTALLER_END(NH_INSTALLER_ERROR_INVALID_OPTION)
            }
        }
        if (libs && !negate) 
        {
            LIBRARY_NETZHAUT   = !strcmp(argv_pp[i], "netzhaut") || LIBRARY_NETZHAUT;
            LIBRARY_LOADER     = !strcmp(argv_pp[i], "loader") || LIBRARY_LOADER;
            LIBRARY_CORE       = !strcmp(argv_pp[i], "core") || LIBRARY_CORE;
            LIBRARY_IO         = !strcmp(argv_pp[i], "io") || LIBRARY_IO;
            LIBRARY_TTY        = !strcmp(argv_pp[i], "tty") || LIBRARY_TTY;
            LIBRARY_ECMASCRIPT = !strcmp(argv_pp[i], "ecmascript") || LIBRARY_ECMASCRIPT;
            LIBRARY_HTML       = !strcmp(argv_pp[i], "html") || LIBRARY_HTML;
            LIBRARY_NETWORK    = !strcmp(argv_pp[i], "network") || LIBRARY_NETWORK;
            LIBRARY_WEBIDL     = !strcmp(argv_pp[i], "webidl") || LIBRARY_WEBIDL;
        }
        if (libs && negate) {
            LIBRARY_NETZHAUT   = strcmp(argv_pp[i], "netzhaut") && LIBRARY_NETZHAUT;
            LIBRARY_LOADER     = strcmp(argv_pp[i], "loader") && LIBRARY_LOADER;
            LIBRARY_CORE       = strcmp(argv_pp[i], "core") && LIBRARY_CORE;
            LIBRARY_IO         = strcmp(argv_pp[i], "io") && LIBRARY_IO;
            LIBRARY_TTY        = strcmp(argv_pp[i], "tty") && LIBRARY_TTY;
            LIBRARY_ECMASCRIPT = strcmp(argv_pp[i], "ecmascript") && LIBRARY_ECMASCRIPT;
            LIBRARY_HTML       = strcmp(argv_pp[i], "html") && LIBRARY_HTML;
            LIBRARY_NETWORK    = strcmp(argv_pp[i], "network") && LIBRARY_NETWORK;
            LIBRARY_WEBIDL     = strcmp(argv_pp[i], "webidl") && LIBRARY_WEBIDL;
        }

        if (bins)
        {
            if (strcmp(argv_pp[i], "webbrowser")
            &&  strcmp(argv_pp[i], "terminal")
            &&  strcmp(argv_pp[i], "windowsystem")) {
                Nh_Installer_noticef("Invalid option \"%s\"", argv_pp[i]);
                NH_INSTALLER_END(NH_INSTALLER_ERROR_INVALID_OPTION)
            }
        }
        if (bins)
        {
            BINARY_TERMINAL      = !strcmp(argv_pp[i], "terminal") || BINARY_TERMINAL;
            BINARY_WEB_BROWSER   = !strcmp(argv_pp[i], "webbrowser") || BINARY_WEB_BROWSER;
            BINARY_WINDOW_SYSTEM = !strcmp(argv_pp[i], "windowsystem") || BINARY_WINDOW_SYSTEM;
        }
        if (bins && negate)
        {
            BINARY_WEB_BROWSER   = BINARY_WEB_BROWSER ? !BINARY_WEB_BROWSER : BINARY_WEB_BROWSER;
            BINARY_WINDOW_SYSTEM = BINARY_WINDOW_SYSTEM ? !BINARY_WINDOW_SYSTEM : BINARY_WINDOW_SYSTEM;
            BINARY_TERMINAL      = BINARY_TERMINAL ? !BINARY_TERMINAL : BINARY_TERMINAL;
        }
    }

    if (!RUN) {NH_INSTALLER_QUIET = NH_TRUE;}

    BUILD_A_LIBRARY = 
        LIBRARY_NETZHAUT || LIBRARY_LOADER || LIBRARY_CORE || LIBRARY_IO || LIBRARY_TTY || LIBRARY_NETWORK || LIBRARY_ECMASCRIPT || LIBRARY_HTML || LIBRARY_WEBIDL;

    if (!BUILD_A_LIBRARY && BUILD_ALL_LIBRARIES) {
        LIBRARY_NETZHAUT   = NH_TRUE;
        LIBRARY_LOADER     = NH_TRUE;
        LIBRARY_CORE       = NH_TRUE;
        LIBRARY_IO         = NH_TRUE;
        LIBRARY_TTY        = NH_TRUE;
        LIBRARY_NETWORK    = NH_TRUE;
        LIBRARY_HTML       = NH_TRUE;
        LIBRARY_ECMASCRIPT = NH_TRUE;
        LIBRARY_WEBIDL     = NH_TRUE;
        BUILD_A_LIBRARY = NH_TRUE;
    }

    BUILD_A_BINARY = 
        BINARY_WEB_BROWSER || BINARY_WINDOW_SYSTEM || BINARY_TERMINAL;

    if (!BUILD_A_BINARY && BUILD_ALL_BINARIES) {
        BINARY_WEB_BROWSER   = NH_TRUE;
        BINARY_WINDOW_SYSTEM = NH_TRUE;
        BINARY_TERMINAL      = NH_TRUE;
        BUILD_A_BINARY       = NH_TRUE;
    }

NH_INSTALLER_END(NH_INSTALLER_SUCCESS)
}

// RUN =============================================================================================

static NH_INSTALLER_RESULT Nh_Installer_run()
{
NH_INSTALLER_BEGIN()

#include NH_INSTALLER_CUSTOM_CHECK

    if (GENERATE_DOCS) {
        Nh_Installer_operationf("GENERATE DOCS");
        NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_GENERATE_DOCUMENTS_FAILURE, Nh_Installer_generateDocs())
    }

    if (!OFFLINE && (BUILD_A_LIBRARY)) {
        Nh_Installer_operationf("DOWNLOAD EXTERNALS");
        NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_DOWNLOAD_FAILED, Nh_Installer_downloadExternals())
    }

    if (BUILD_A_LIBRARY) {
        NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_BUILD_LIBRARY_FAILED, Nh_Installer_prepareLibraryBuild())
        NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_BUILD_LIBRARY_FAILED, Nh_Installer_buildLibrary("NhExternal", INSTALL_ALL_LIBRARIES))
    }

    if (LIBRARY_NETZHAUT) {
        NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_BUILD_LIBRARY_FAILED, Nh_Installer_buildLibrary("Netzhaut", INSTALL_ALL_LIBRARIES))
    }
    if (LIBRARY_LOADER) {
        NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_BUILD_LIBRARY_FAILED, Nh_Installer_buildLibrary("NhLoader", INSTALL_ALL_LIBRARIES))
    }
    if (LIBRARY_CORE) {
        NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_BUILD_LIBRARY_FAILED, Nh_Installer_buildLibrary("NhCore", INSTALL_ALL_LIBRARIES))
    }
    if (LIBRARY_IO) {
        NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_BUILD_LIBRARY_FAILED, Nh_Installer_buildLibrary("NhIO", INSTALL_ALL_LIBRARIES))
    }
    if (LIBRARY_TTY) {
        NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_BUILD_LIBRARY_FAILED, Nh_Installer_buildLibrary("NhTTY", INSTALL_ALL_LIBRARIES))
    }
    if (LIBRARY_NETWORK) {
        NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_BUILD_LIBRARY_FAILED, Nh_Installer_buildLibrary("NhNetwork", INSTALL_ALL_LIBRARIES))
    }
    if (LIBRARY_ECMASCRIPT) {
        NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_BUILD_LIBRARY_FAILED, Nh_Installer_buildLibrary("NhECMAScript", INSTALL_ALL_LIBRARIES))
    }
    if (LIBRARY_HTML) {
        NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_BUILD_LIBRARY_FAILED, Nh_Installer_buildLibrary("NhHTML", INSTALL_ALL_LIBRARIES))
    }
    if (LIBRARY_WEBIDL) {
        NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_BUILD_LIBRARY_FAILED, Nh_Installer_buildLibrary("NhWebIDL", INSTALL_ALL_LIBRARIES))
    }

    if (INSTALL_ALL_LIBRARIES) {
        Nh_Installer_operationf("INSTALL_ALL INCLUDES");
        NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_INSTALL_ALL_LIBRARY_FAILED, Nh_Installer_installIncludes())
    }

    if (BINARY_TERMINAL) {
        Nh_Installer_operationf("BUILD TERMINAL");
        NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_BUILD_WEB_BROWSER_FAILED, Nh_Installer_buildNhTerminal())
        if (INSTALL_ALL_BINARIES) {
            Nh_Installer_operationf("INSTALL_ALL TERMINAL");
            NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_INSTALL_ALL_WEB_BROWSER_FAILED, Nh_Installer_installNhTerminal())
        }
    }

//    if (BUILD_ALL_BINARIES || BINARY_WEB_BROWSER) 
//    {
//        Nh_Installer_operationf("BUILD WEB BROWSER");
//        NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_BUILD_WEB_BROWSER_FAILED, Nh_Installer_buildNhWebBrowser())
//
//        if (INSTALL_ALL_BINARIES)
//        {
//            Nh_Installer_operationf("INSTALL_ALL WEB BROWSER");
//            NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_INSTALL_ALL_WEB_BROWSER_FAILED, Nh_Installer_installNhWebBrowser())
//        }
//    }

    if (INSTALL_ALL_BINARIES && (BUILD_ALL_BINARIES || BUILD_A_BINARY)) {
        Nh_Installer_operationf("INSTALL_ALL LOGO");
        NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_INSTALL_ALL_LOGO_FAILED, Nh_Installer_installLogo())
    }

#include NH_INSTALLER_DEFAULT_CHECK

NH_INSTALLER_END(NH_INSTALLER_SUCCESS)
}

// CHECK LOCATION ==================================================================================

static NH_BOOL Nh_Installer_validLocation()
{
NH_INSTALLER_BEGIN()

    char projDir_p[2048] = {'\0'};
    if (Nh_Installer_getProjectDir(projDir_p, 2048) != NH_INSTALLER_SUCCESS) {NH_INSTALLER_END(NH_FALSE)}

    char check_p[2048] = {'\0'};
    sprintf(check_p, "%s%s", projDir_p, "/data");
    if (!Nh_Installer_fileExists(check_p)) {NH_INSTALLER_END(NH_FALSE)}

    memset(check_p, '\0', sizeof(char) * 2048); 
    sprintf(check_p, "%s%s", projDir_p, "/src");
    if (!Nh_Installer_fileExists(check_p)) {NH_INSTALLER_END(NH_FALSE)}

    memset(check_p, '\0', sizeof(char) * 2048);
    sprintf(check_p, "%s%s", projDir_p, "/README.md");
    if (!Nh_Installer_fileExists(check_p)) {NH_INSTALLER_END(NH_FALSE)}

NH_INSTALLER_END(NH_TRUE)
}

// MAIN ============================================================================================

NH_INSTALLER_RESULT Nh_Installer_main(
    int argc, char **argv_pp)
{
NH_INSTALLER_BEGIN()

    NH_INSTALLER_RESULT result = NH_INSTALLER_SUCCESS;

    if (Nh_Installer_validLocation()) 
    {
        if (argc == 1) {
            NH_INSTALLER_GUI = NH_TRUE;
            result = Nh_Installer_runGUI();
        }
        else { 
            result = Nh_Installer_parseInput(argc, argv_pp);
            if (result == NH_INSTALLER_SUCCESS && RUN) {
                result = Nh_Installer_run();
            }
        }
    }
    else {result = NH_INSTALLER_ERROR_BAD_STATE;}

    NH_INSTALLER_CHECK(Nh_Installer_exitMessage(result))

NH_INSTALLER_DIAGNOSTIC_END(result)
}

int main(
    int argc, char **argv_pp)
{
NH_INSTALLER_BEGIN()
NH_INSTALLER_DIAGNOSTIC_END(Nh_Installer_main(argc, argv_pp))
} 


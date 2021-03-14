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
#include "Parser.h"

#include "Common/Macro.h"

#include NH_INSTALLER_FLOW
#include NH_INSTALLER_DEFAULT_CHECK

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

// DATA ============================================================================================

static NH_BOOL RUN = NH_FALSE;
static NH_BOOL BUILD_A_LIBRARY     = NH_FALSE;
static NH_BOOL BUILD_ALL_LIBRARIES = NH_FALSE;
static NH_BOOL BUILD_A_BINARY     = NH_FALSE;
static NH_BOOL BUILD_ALL_BINARIES = NH_FALSE;
static NH_BOOL BINARY_WEB_BROWSER   = NH_FALSE;
static NH_BOOL BINARY_TERMINAL      = NH_FALSE;
static NH_BOOL BINARY_WINDOW_SYSTEM = NH_FALSE;
static NH_BOOL INSTALL_ALL_LIBRARIES = NH_FALSE;
static NH_BOOL INSTALL_ALL_BINARIES  = NH_FALSE;
static NH_BOOL GENERATE_DOCS  = NH_FALSE;
static NH_BOOL VULKAN_SHADERS = NH_FALSE;
static NH_BOOL OFFLINE        = NH_FALSE;

NH_BOOL NH_INSTALLER_QUIET        = NH_FALSE;
NH_BOOL NH_INSTALLER_FLOW_LOGGING = NH_FALSE;
NH_BOOL NH_INSTALLER_GUI          = NH_FALSE;

NH_BYTE *NH_INSTALLER_FILE_DATA_P = NULL;
Nh_Installer_Library NH_INSTALLER_LIBRARIES_P[NH_INSTALLER_MAX_LIBRARIES];
Nh_Installer_Source NH_INSTALLER_SOURCES_P[NH_INSTALLER_MAX_SOURCES];

// GET LIBRARY NAME ================================================================================ 

NH_BYTE *Nh_Installer_getLibraryName(
    int i)
{
NH_INSTALLER_BEGIN()

    static NH_BYTE libraryName_p[255];

    memset(libraryName_p, 0, 255);
    Nh_Installer_Library *Library_p = &NH_INSTALLER_LIBRARIES_P[i];

    if (!Library_p->name_p) {NH_INSTALLER_END(libraryName_p)}

    else if (!strcmp(Library_p->name_p, "Netzhaut")) {
        strcpy(libraryName_p, "netzhaut");
    }
    else {
        strcpy(libraryName_p, Library_p->name_p + 2);
        for (int j = 0; j < strlen(libraryName_p); ++j) {
            libraryName_p[j] = tolower(libraryName_p[j]);
        }
    }

NH_INSTALLER_END(libraryName_p)
}

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
                    for (int j = 0; j < NH_INSTALLER_MAX_LIBRARIES; ++j) {
                        if (NH_INSTALLER_LIBRARIES_P[j].name_p) {
                            NH_INSTALLER_LIBRARIES_P[j].build = NH_TRUE;
                        }
                    }
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
            NH_BOOL valid = NH_FALSE;
            for (int j = 0; j < NH_INSTALLER_MAX_LIBRARIES; ++j) {
                if (!strcmp(argv_pp[i], Nh_Installer_getLibraryName(j))) {
                    valid = NH_TRUE;
                    break;
                }
            }
            if (!valid) {
                Nh_Installer_noticef("Invalid option \"%s\"", argv_pp[i]);
                NH_INSTALLER_END(NH_INSTALLER_ERROR_INVALID_OPTION)
            }
        }

        if (libs && !negate) {
            for (int j = 0; j < NH_INSTALLER_MAX_LIBRARIES; ++j) {
                NH_INSTALLER_LIBRARIES_P[j].build = !strcmp(argv_pp[i], Nh_Installer_getLibraryName(j)) || NH_INSTALLER_LIBRARIES_P[j].build;
            }
        }
        if (libs && negate) {
            for (int j = 0; j < NH_INSTALLER_MAX_LIBRARIES; ++j) {
                NH_INSTALLER_LIBRARIES_P[j].build = strcmp(argv_pp[i], Nh_Installer_getLibraryName(j)) && NH_INSTALLER_LIBRARIES_P[j].build;
            }
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

    for (int j = 0; j < NH_INSTALLER_MAX_LIBRARIES; ++j) {
        if (NH_INSTALLER_LIBRARIES_P[j].build && NH_INSTALLER_LIBRARIES_P[j].name_p) {
            BUILD_A_LIBRARY = NH_TRUE;
            break;
        }
    }

    if (!BUILD_A_LIBRARY && BUILD_ALL_LIBRARIES) {
        for (int j = 0; j < NH_INSTALLER_MAX_LIBRARIES; ++j) {
            if (NH_INSTALLER_LIBRARIES_P[j].name_p) {
                NH_INSTALLER_LIBRARIES_P[j].build = NH_TRUE;
            }
        }
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
        NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_BUILD_LIBRARY_FAILED, Nh_Installer_buildNhExternal(INSTALL_ALL_LIBRARIES))
    }

    for (int i = 0; i < NH_INSTALLER_MAX_LIBRARIES; ++i) {
        if (NH_INSTALLER_LIBRARIES_P[i].build && NH_INSTALLER_LIBRARIES_P[i].name_p) {
            NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_BUILD_LIBRARY_FAILED, Nh_Installer_buildLibrary(&NH_INSTALLER_LIBRARIES_P[i], INSTALL_ALL_LIBRARIES))
        }
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

void Nh_Installer_showParseResult()
{
NH_INSTALLER_BEGIN()

    for (int i = 0; i < 255; ++i) {
        Nh_Installer_Library *Library_p = &NH_INSTALLER_LIBRARIES_P[i];
        if (Library_p->name_p) {Nh_Installer_noticef("Library: %s (CompileArgs:%s, LinkArgs:%s)", Library_p->name_p, Library_p->compileArgs_p, Library_p->linkArgs_p);}
    }

    for (int i = 0; i < 1024; ++i) {
        Nh_Installer_Source *Source_p = &NH_INSTALLER_SOURCES_P[i];
        if (Source_p->path_p) {Nh_Installer_noticef("%s %s", Source_p->Library_p->name_p, Source_p->path_p + strlen(Source_p->Library_p->name_p) + 1);}
    }

NH_INSTALLER_SILENT_END()
}

NH_INSTALLER_RESULT Nh_Installer_main(
    int argc, char **argv_pp)
{
NH_INSTALLER_BEGIN()

    NH_INSTALLER_RESULT result = NH_INSTALLER_SUCCESS;

    if (Nh_Installer_validLocation()) 
    {
        char projDir_p[2048] = {'\0'};
        if (Nh_Installer_getProjectDir(projDir_p, 2048) != NH_INSTALLER_SUCCESS) {NH_INSTALLER_END(NH_FALSE)}

        Nh_Installer_parseInstallerFile(projDir_p);
        Nh_Installer_showParseResult();

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

    for (int i = 0; i < 255; ++i) {
        NH_INSTALLER_LIBRARIES_P[i].name_p = NULL;
        NH_INSTALLER_LIBRARIES_P[i].compileArgs_p = NULL;
        NH_INSTALLER_LIBRARIES_P[i].linkArgs_p = NULL;
        NH_INSTALLER_LIBRARIES_P[i].build = NH_FALSE;
    } 

    for (int i = 0; i < 1024; ++i) {
        NH_INSTALLER_SOURCES_P[i].Library_p = NULL;
        NH_INSTALLER_SOURCES_P[i].path_p = NULL;
    } 

    NH_INSTALLER_FILE_DATA_P = NULL;

    NH_INSTALLER_RESULT result = Nh_Installer_main(argc, argv_pp);

    free(NH_INSTALLER_FILE_DATA_P);

NH_INSTALLER_DIAGNOSTIC_END(result)
} 


// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "Docs.h"
#include "Util.h"
#include "Main.h"

#include "Common/Macro.h"

#include NH_INSTALLER_FLOW
#include NH_INSTALLER_DEFAULT_CHECK

#include "../../lib/Netzhaut/Netzhaut.h"

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>

// DEFINE ==========================================================================================

#define INSERT_BEGIN "NETZHAUT_INSTALLER_INSERT_BEGIN"
#define INSERT_END "NETZHAUT_INSTALLER_INSERT_END"

// TIME ============================================================================================

static void Nh_Installer_getTime(
    char set_p[26])
{
NH_INSTALLER_BEGIN()

    time_t timer;
    struct tm* tm_info;

    timer = time(NULL);
    tm_info = localtime(&timer);

    strftime(set_p, 26, "%Y-%m-%d %H:%M:%S", tm_info);

NH_INSTALLER_SILENT_END()
}

// GENERATE ========================================================================================

static NH_INSTALLER_RESULT Nh_Installer_generateHomePage()
{
NH_INSTALLER_BEGIN()

#include NH_INSTALLER_CUSTOM_CHECK

    char projDir_p[2048] = {'\0'};
    NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_GET_PROJECT_DIRECTORY, Nh_Installer_getProjectDir(projDir_p, 2048))

#include NH_INSTALLER_DEFAULT_CHECK

    char path_p[2048] = {'\0'};
    sprintf(path_p, "%s%s", projDir_p, "/docs/home.html");
   
    char *part1_p = Nh_Installer_getFileData(path_p, NULL);
    char *tmp_p = strstr(part1_p, INSERT_BEGIN);
    tmp_p[strlen(INSERT_BEGIN) + 3] = '\0';
    tmp_p = tmp_p + strlen(INSERT_BEGIN) + 4;

    char *part5_p = strstr(tmp_p, INSERT_END) - 4;

    char time_p[26] = {'\0'};
    Nh_Installer_getTime(time_p);

    char helpPath_p[2048] = {'\0'};
    sprintf(helpPath_p, "%s%s", projDir_p, "/data/installer/help.txt");
    char *help_p = Nh_Installer_getFileData(helpPath_p, NULL); 

    char generate_p[20000] = {'\0'}; // TODO stack smashing inc
    sprintf(generate_p, "%s\n%s\n%s", part1_p, help_p, part5_p);

    NH_INSTALLER_CHECK(Nh_Installer_writeCharsToFile(path_p, generate_p))
    free(part1_p);
    free(help_p);

NH_INSTALLER_END(NH_INSTALLER_SUCCESS)
}

static NH_INSTALLER_RESULT Nh_Installer_generateREADME()
{
NH_INSTALLER_BEGIN()

#include NH_INSTALLER_CUSTOM_CHECK

    char projDir_p[2048] = {'\0'};
    NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_GET_PROJECT_DIRECTORY, Nh_Installer_getProjectDir(projDir_p, 2048))

#include NH_INSTALLER_DEFAULT_CHECK

    char path_p[2048] = {'\0'};
    sprintf(path_p, "%s%s", projDir_p, "/README.md");
   
    char *part1_p = Nh_Installer_getFileData(path_p, NULL); 
    char *tmp_p = strstr(part1_p, INSERT_BEGIN);
    tmp_p[strlen(INSERT_BEGIN) + 3] = '\0';
    tmp_p = tmp_p + strlen(INSERT_BEGIN) + 4;

    char *part3_p = strstr(tmp_p, INSERT_END) - 4;

    char time_p[26] = {'\0'};
    Nh_Installer_getTime(time_p);

    char part2_p[255] = {'\0'};
    sprintf(part2_p, "```%s | Netzhaut v%d.%d.%d INITIAL DEVELOPMENT```", time_p, NETZHAUT_MAJOR_VERSION, NETZHAUT_MINOR_VERSION, NETZHAUT_PATCH_VERSION);

    char generate_p[10000] = {'\0'};
    sprintf(generate_p, "%s\n%s\n%s", part1_p, part2_p, part3_p);

    NH_INSTALLER_CHECK(Nh_Installer_writeCharsToFile(path_p, generate_p))
    free(part1_p);

NH_INSTALLER_END(NH_INSTALLER_SUCCESS)
}

static NH_INSTALLER_RESULT Nh_Installer_generateHubUsingDoxygen()
{
NH_INSTALLER_BEGIN()

    if (!Nh_Installer_canRunCommand("doxygen")) {NH_INSTALLER_DIAGNOSTIC_END(NH_INSTALLER_ERROR_DOXYGEN_NOT_FOUND)}

#include NH_INSTALLER_CUSTOM_CHECK

    char wrkdir_p[2048] = {'\0'};
    NH_INSTALLER_CHECK_NULL(NH_INSTALLER_ERROR_GET_WORK_DIRECTORY, getcwd(wrkdir_p, 2048))

    char projDir_p[2048] = {'\0'};
    NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_GET_PROJECT_DIRECTORY, Nh_Installer_getProjectDir(projDir_p, 2048))

#include NH_INSTALLER_DEFAULT_CHECK

    char path_p[2048] = {'\0'};
    sprintf(path_p, "%s%s", projDir_p, "/docs/Development");
    chdir(path_p);

    int status = system("doxygen Doxyfile");
    if (WEXITSTATUS(status) || WIFSIGNALED(status)) {NH_INSTALLER_DIAGNOSTIC_END(NH_INSTALLER_ERROR_DOXYGEN_EXECUTION_FAILED)}

    sprintf(path_p, "%s%s", projDir_p, "/docs/API");
    chdir(path_p);

    status = system("doxygen Doxyfile");
    if (WEXITSTATUS(status) || WIFSIGNALED(status)) {NH_INSTALLER_DIAGNOSTIC_END(NH_INSTALLER_ERROR_DOXYGEN_EXECUTION_FAILED)}

    chdir(wrkdir_p);

NH_INSTALLER_DIAGNOSTIC_END(NH_INSTALLER_SUCCESS)
}

NH_INSTALLER_RESULT Nh_Installer_generateDocs()
{
NH_INSTALLER_BEGIN()

#include NH_INSTALLER_CUSTOM_CHECK

    NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_GENERATE_VERSION_DEPENDENT_DOCS, Nh_Installer_generateHomePage())
    NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_GENERATE_VERSION_DEPENDENT_DOCS, Nh_Installer_generateREADME())
    NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_GENERATE_DOCS_USING_DOXYGEN, Nh_Installer_generateHubUsingDoxygen())

#include NH_INSTALLER_DEFAULT_CHECK

NH_INSTALLER_DIAGNOSTIC_END(NH_INSTALLER_SUCCESS)
}


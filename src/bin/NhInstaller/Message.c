// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "Message.h"
#include "Main.h"
#include "Util.h"

#include "Common/Macro.h"
#include "Common/Version.h"

#include NH_INSTALLER_FLOW
#include NH_INSTALLER_DEFAULT_CHECK

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

// DATA ============================================================================================

int messages = 0;

// HELP ============================================================================================

NH_INSTALLER_RESULT Nh_Installer_printHelp()
{
NH_INSTALLER_BEGIN()

#ifdef NH_INSTALLER_EXECUTABLE

#include NH_INSTALLER_CUSTOM_CHECK

    char projDir_p[2048] = {'\0'};
    if (Nh_Installer_getProjectDir(projDir_p, 2048) != NH_INSTALLER_SUCCESS) {NH_INSTALLER_DIAGNOSTIC_END(NH_INSTALLER_ERROR_BAD_STATE)}

#include NH_INSTALLER_DEFAULT_CHECK

    char helpPath_p[2048] = {'\0'};
    sprintf(helpPath_p, "%s%s", projDir_p, "/data/installer/help.txt");
    char *help_p = Nh_Installer_getFileData(helpPath_p, NULL); 

    printf("\n%s\n", help_p);

    free(help_p);

#endif

NH_INSTALLER_DIAGNOSTIC_END(NH_INSTALLER_SUCCESS)
}

// VERSION =========================================================================================

NH_INSTALLER_RESULT Nh_Installer_printVersion()
{
NH_INSTALLER_BEGIN()

#ifdef NH_INSTALLER_EXECUTABLE

    puts("");
    puts("  NETZHAUT-INSTALLER VERSION");
    puts("");

#ifdef __unix__
    char *target_p = "LINUX";
#elif defined(_WIN32)
    char *target_p = "WINDOWS";
#endif

    printf("  %s %d.%d.%d\n", target_p, NH_INSTALLER_MAJOR_VERSION, NH_INSTALLER_MINOR_VERSION, NH_INSTALLER_PATCH_VERSION);
    printf("  %s %s\n", __DATE__, __TIME__);

    switch (__STDC_VERSION__)
    {
        case 199409L : puts("  GNU C95"); break;
        case 199901L : puts("  GNU C99"); break;
        case 201112L : puts("  GNU C11"); break;
        case 201710L : puts("  GNU C18"); break;
    }

    puts("");

#endif

NH_INSTALLER_DIAGNOSTIC_END(NH_INSTALLER_SUCCESS)
}

// MESSAGE =========================================================================================

NH_INSTALLER_RESULT Nh_Installer_operationf(
    char *format_p, ...)
{
NH_INSTALLER_BEGIN()

    if (NH_INSTALLER_QUIET) {NH_INSTALLER_DIAGNOSTIC_END(NH_INSTALLER_SUCCESS)}

#ifdef NH_INSTALLER_EXECUTABLE

    if (NH_INSTALLER_GUI)
    {

    }
    else 
    {
        va_list args;
        va_start(args, format_p);
    
        char message_p[1024] = {'\0'};
        vsprintf(message_p, format_p, args);
    
        va_end(args);
    
        if (messages == 0) {
            printf("NhInstaller: -> \e[1;32m%s\e[0m\nNhInstaller:\n", message_p);
        }
        else {printf("NhInstaller:\nNhInstaller: -> \e[1;32m%s\e[0m\nNhInstaller:\n", message_p);}
    
        messages = 0;
    }

#endif

NH_INSTALLER_DIAGNOSTIC_END(NH_INSTALLER_SUCCESS)
}

NH_INSTALLER_RESULT Nh_Installer_messagef(
    char *format_p, ...)
{
NH_INSTALLER_BEGIN()

    if (NH_INSTALLER_QUIET) {NH_INSTALLER_DIAGNOSTIC_END(NH_INSTALLER_SUCCESS)}

#ifdef NH_INSTALLER_EXECUTABLE

    if (NH_INSTALLER_GUI)
    {

    }
    else 
    {
        va_list args;
        va_start(args, format_p);

        char message_p[1024] = {'\0'};
        vsprintf(message_p, format_p, args);

        va_end(args);

        printf("NhInstaller: %s\n", message_p);
        messages++;
    }

#endif

NH_INSTALLER_DIAGNOSTIC_END(NH_INSTALLER_SUCCESS)
}

NH_INSTALLER_RESULT Nh_Installer_noticef(
    char *format_p, ...)
{
NH_INSTALLER_BEGIN()

    if (NH_INSTALLER_QUIET) {NH_INSTALLER_DIAGNOSTIC_END(NH_INSTALLER_SUCCESS)}

#ifdef NH_INSTALLER_EXECUTABLE

    if (NH_INSTALLER_GUI)
    {

    }
    else 
    {
        va_list args;
        va_start(args, format_p);

        char message_p[1024] = {'\0'};
        vsprintf(message_p, format_p, args);

        va_end(args);

        printf("NhInstaller: \e[1;35mNOTICE:\e[0m %s\n", message_p);
        messages++;
    }

#endif

NH_INSTALLER_DIAGNOSTIC_END(NH_INSTALLER_SUCCESS)
}

NH_INSTALLER_RESULT Nh_Installer_externalMessage(
    char *prepend_p, char *message_p)
{
NH_INSTALLER_BEGIN()

    if (NH_INSTALLER_QUIET) {NH_INSTALLER_DIAGNOSTIC_END(NH_INSTALLER_SUCCESS)}

#ifdef NH_INSTALLER_EXECUTABLE

    if (NH_INSTALLER_GUI)
    {

    }
    else 
    {
        char *p = message_p;
        for (int i = 0; i < strlen(message_p); ++i) 
        {
            if (i + 1 == strlen(message_p)) {i++;}

            if (message_p[i] == '\n' || message_p[i] == '\0') 
            {
                char replace = message_p[i];
                message_p[i] = '\0';
                printf("%s%s\n", prepend_p, p);
                p = &message_p[i + 1];
                message_p[i] = replace;
                messages++;
            }
        }
    }

#endif

NH_INSTALLER_DIAGNOSTIC_END(NH_INSTALLER_SUCCESS)
}

NH_INSTALLER_RESULT Nh_Installer_exitMessage(
    NH_INSTALLER_RESULT result)
{
NH_INSTALLER_BEGIN()

    if (NH_INSTALLER_QUIET) {NH_INSTALLER_DIAGNOSTIC_END(NH_INSTALLER_SUCCESS)}

#ifdef NH_INSTALLER_EXECUTABLE

    if (NH_INSTALLER_GUI)
    {

    }
    else 
    {
        if (result) {
            printf("NhInstaller: -> \e[1;31mERROR\e[0m %s\n", NH_INSTALLER_RESULTS_PP[result]);
            printf("NhInstaller: -> \e[1;31mEXECUTION ERROR\e[0m -> \e[1;31mEXITING\e[0m\n");
        }
        else {printf("NhInstaller:\nNhInstaller: -> \e[1;32mEXECUTION SUCCESS\e[0m -> \e[1;32mEXITING\e[0m\n");}
    }

#endif

NH_INSTALLER_DIAGNOSTIC_END(NH_INSTALLER_SUCCESS)
}


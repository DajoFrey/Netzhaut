// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "GUI.h"
#include "Util.h"
#include "X11.h"

#include "Common/Macro.h"
#include "Common/API.h"

#include NH_INSTALLER_FLOW
#include NH_INSTALLER_DEFAULT_CHECK

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// DATA ============================================================================================

NH_BOOL run = NH_TRUE;

// BUILD ===========================================================================================

NH_INSTALLER_RESULT Nh_Installer_runGUI()
{
NH_INSTALLER_BEGIN()

    Nh_Installer_X11_createWindow("Netzhaut-Installer", 300, 300);
    while (run) {Nh_Installer_X11_getInput();}
    
NH_INSTALLER_DIAGNOSTIC_END(NH_INSTALLER_SUCCESS)
}

NH_INSTALLER_RESULT Nh_Installer_handleGUIExpose()
{
NH_INSTALLER_BEGIN()

    Nh_Installer_X11_drawTextLine(5, 5, "Building via GUI is planned, but not implemented yet. Please use the command-line for now.");

NH_INSTALLER_END(NH_INSTALLER_SUCCESS)
}

void Nh_Installer_handleGUIExit()
{
NH_INSTALLER_BEGIN()

    run = NH_FALSE;

NH_INSTALLER_SILENT_END()
}


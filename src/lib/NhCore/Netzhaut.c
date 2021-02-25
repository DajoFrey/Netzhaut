// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "Netzhaut.h"
#include "Memory.h"
#include "Config.h"
#include "Window.h"
#include "Tab.h"
#include "Process.h"
#include "Thread.h"
#include "Logger.h"
#include "Unicode.h"

#include "../NhNetwork/Network.h"
#include "../NhTTY/Terminal.h"
#include "../NhIO/Window.h"

#include "Common/IndexMap.h"
#include "Common/Macro.h"
#include NH_FLOW
#include NH_DEFAULT_CHECK

#include <dlfcn.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// INIT ============================================================================================

static NH_RESULT Nh_startInternalWorkloads()
{
NH_BEGIN()

    Nh_activateWorkload(
        NULL, Nh_runSystemUpdater, NULL, NH_FALSE
    );

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

NH_RESULT Nh_init()
{
NH_BEGIN()

    NH_CHECK(Nh_initLogger())
    NH_CHECK(Nh_initFlags())
    NH_CHECK(Nh_initMemory())
    NH_CHECK(Nh_initProcessPool())
    NH_CHECK(Nh_initConfig())
    NH_CHECK(Nh_logUnicodeCodepoints())
    NH_CHECK(Nh_initFontManager())
    NH_CHECK(Nh_addInitialFonts())
    NH_CHECK(Nh_addInitialFontPrototypes())
    NH_CHECK(Nh_createIndexMap())
    NH_CHECK(Nh_initSystem())
    NH_CHECK(Nh_startInternalWorkloads())

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

// RUN =============================================================================================

unsigned int Nh_run()
{
NH_BEGIN()
NH_END(Nh_runWorkloads())
}

// KEEP RUNNING? ===================================================================================

NH_BOOL Nh_keepRunning()
{
NH_BEGIN()

    Nh_checkForks();

NH_END(Nh_activeWorkloads(NH_TRUE) > 0 || Nh_activeForks() > 0 ? NH_TRUE : NH_FALSE)
}

// RELEASE =========================================================================================

NH_RESULT Nh_release()
{
NH_BEGIN()

    puts("TODO release");
    exit(0);
//#include NH_CUSTOM_CHECK
//
//    if (_Nh_getThreadPool() == NULL) {return NH_USAGE_ERROR_INIT_MISSING;}
//
//    if (Output_p == NULL || Output_p == NULL)
//    {
//        NH_BEGIN()
//
//        if (Nh_keepRunning()) {NH_DIAGNOSTIC_END(NH_USAGE_ERROR_STILL_RUNNING)}
//
//#ifdef __unix__
//
//        if (Nh_TTY_getTerminal()->run)
//        {
//            Nh_TTY_Terminal *Terminal_p = Nh_TTY_getTerminal();
//            kill(Terminal_p->IPC.pid, SIGTERM);
//            int w = -1;
//            wait(&w);
//            close(Terminal_p->IPC.readFD_p[0]);
//        }
//    
//        for (int i = 0; i < NH_MAX_THREADS; ++i) {
//            if (Nh_getThreadPool()->Threads_p[i].id != 0) {
//                pthread_join(Nh_getThreadPool()->Threads_p[i].id, NULL);
//            } 
//        }
//
//#endif
//
//        Nh_destroyFontManager();
//        Nh_freeIndexMap();
//        Nh_freeSystem();
//        Nh_freeOutputs();
//        Nh_freeConfig();
//
//        NH_DIAGNOSTIC_END(NH_SUCCESS)
//    }
//    else {_Nh_freeOutput(Output_p);}

NH_DIAGNOSTIC_END(NH_SUCCESS)
}


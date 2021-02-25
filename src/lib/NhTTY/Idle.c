// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "Idle.h"
#include "Program.h"
#include "TreeListing.h"
#include "Row.h"
#include "Terminal.h"
#include "Shell.h"

#include "../NhCore/Memory.h"
#include "../NhCore/Common/Macro.h"
#include NH_FLOW
#include NH_CUSTOM_CHECK

#include "Common/Macro.h"
#include NH_TTY_FLOW
#include NH_TTY_DEFAULT_CHECK

#include <stddef.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>

// INIT ============================================================================================

void *Nh_TTY_initIdle()
{
NH_TTY_BEGIN()

#include NH_TTY_CUSTOM_CHECK

    Nh_TTY_Idle *Idle_p = Nh_allocate(sizeof(Nh_TTY_Idle));
    NH_TTY_CHECK_MEM(NULL, Idle_p)

    Idle_p->LastUpdate = Nh_getSystemTime();
    Idle_p->updateIntervalInSeconds = 0.5;
    Idle_p->current = 0;

#include NH_TTY_DEFAULT_CHECK

NH_TTY_END(Idle_p)
}

// INPUT ===========================================================================================

NH_TTY_RESULT Nh_TTY_handleIdleInput(
    Nh_TTY_Program *Program_p, NH_UNICODE_CODEPOINT c)
{
NH_TTY_BEGIN()

NH_TTY_END(NH_TTY_SUCCESS)
}

// UPDATE ==========================================================================================

NH_TTY_RESULT Nh_TTY_updateIdle(
    Nh_TTY_Program *Program_p)
{
NH_TTY_BEGIN()

    Nh_TTY_Idle *Idle_p = Program_p->handle_p;
    Nh_SystemTime Now = Nh_getSystemTime();

    if (Nh_getSystemTimeDiffInSeconds(Idle_p->LastUpdate, Now) > Idle_p->updateIntervalInSeconds) {
        Program_p->refresh = NH_TRUE;
        Idle_p->LastUpdate = Now;
    }

NH_TTY_END(NH_TTY_SUCCESS)
}

// DRAW ============================================================================================

static NH_TTY_RESULT Nh_TTY_drawNoise(
    Nh_TTY_Idle *Idle_p, Nh_UTF8String *Row_p, int width, int height, int row)
{
NH_TTY_BEGIN()

    long seed = Idle_p->LastUpdate.milliseconds * (row + 1);

    for (int i = 0; i < width; ++i) 
    {
        long b = i * seed % 64;
        if (b > 32) {
            Nh_appendToString(Row_p, " ", 1);
        }
        else {
            Nh_appendFormatToString(Row_p, "%s", NH_TTY_INVERSE_TILE_COLORS_PP[Nh_TTY_getTileColorFromProgHandle(Idle_p)]);
            Nh_appendToString(Row_p, " \e[0m", 5);
        }
    }

NH_TTY_END(NH_TTY_SUCCESS)
}

NH_TTY_RESULT Nh_TTY_drawIdleRow(
    Nh_TTY_Program *Program_p, Nh_UTF8String *Row_p, int width, int height, int row)
{
NH_TTY_BEGIN()

    Nh_TTY_Idle *Idle_p = Program_p->handle_p;

    switch (Idle_p->current)
    {
        case 0 :
            NH_TTY_DIAGNOSTIC_END(Nh_TTY_drawNoise(Idle_p, Row_p, width, height, row))
    }

NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS)
}


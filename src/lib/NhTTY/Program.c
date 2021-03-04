// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Blineser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "Program.h"
#include "Terminal.h"
#include "Shell.h"
#include "Editor.h"
#include "Tile.h"
#include "Logger.h"
#include "Idle.h"

#include "Common/Macro.h"
#include NH_TTY_FLOW
#include NH_TTY_CUSTOM_CHECK

#include "../NhCore/Memory.h"
#include "../NhCore/Common/Macro.h"
#include NH_FLOW
#include NH_CUSTOM_CHECK

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// CREATE ==========================================================================================

static Nh_TTY_ProgramPrototype *Nh_TTY_createProgramPrototype(
    NH_UNICODE_CODEPOINT *name_p, int nameLength, Nh_TTY_init_f init_f, Nh_TTY_draw_f draw_f, 
    Nh_TTY_handleInput_f handleInput_f, Nh_TTY_getCursorPosition_f getCursorPosition_f,
    Nh_TTY_update_f update_f, Nh_TTY_handleConsoleInput_f handleConsoleInput_f)
{
NH_TTY_BEGIN()

    Nh_TTY_ProgramPrototype *Prototype_p = Nh_allocate(sizeof(Nh_TTY_ProgramPrototype));
    NH_TTY_CHECK_MEM(NULL, Prototype_p)

    Prototype_p->Name = Nh_initUnicodeString(nameLength);
    Nh_appendToUnicodeString(&Prototype_p->Name, name_p, nameLength);

    Prototype_p->init_f = init_f; 
    Prototype_p->draw_f = draw_f; 
    Prototype_p->handleInput_f = handleInput_f; 
    Prototype_p->getCursorPosition_f = getCursorPosition_f; 
    Prototype_p->update_f = update_f;
    Prototype_p->handleConsoleInput_f = handleConsoleInput_f;

NH_TTY_END(Prototype_p)
}

static NH_TTY_RESULT Nh_TTY_appendProgramInstance(
    Nh_Array *Programs_p, Nh_TTY_ProgramPrototype *Prototype_p)
{
NH_TTY_BEGIN()

    Nh_TTY_Program *Program_p = Nh_getFromArray(Programs_p, -1);
    NH_TTY_CHECK_MEM(NH_TTY_ERROR_BAD_STATE, Program_p)

    Program_p->refresh     = NH_FALSE;
    Program_p->handle_p    = Prototype_p->init_f(); 
    Program_p->Programs_p  = Programs_p;
    Program_p->Prototype_p = Prototype_p;

NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS)
}

// ADD =============================================================================================

static NH_UNICODE_CODEPOINT defaultProgNames_pp[4][6] = {
    {101, 100, 105, 116, 111, 114}, // editor
    {108, 111, 103, 103, 101, 114}, // logger
    {115, 104, 101, 108, 108, 0},   // shell
};

NH_TTY_RESULT Nh_TTY_addDefaultProgram(
    Nh_TTY_Terminal *Terminal_p, NH_BYTE *name_p)
{
NH_TTY_BEGIN()

    
    if (!Terminal_p || !name_p) {NH_TTY_DIAGNOSTIC_END(NH_TTY_ERROR_BAD_STATE)}

    Nh_List *Prototypes_p = &Terminal_p->Prototypes;

    if (!strcmp(name_p, "editor")) {
        NH_CHECK(NH_TTY_ERROR_BAD_STATE, Nh_appendToList(Prototypes_p, Nh_TTY_createProgramPrototype(
            defaultProgNames_pp[0], 6, Nh_TTY_initEditor, Nh_TTY_drawEditorRow, Nh_TTY_handleEditorInput, 
            Nh_TTY_getEditorCursor, Nh_TTY_updateEditor, Nh_TTY_executeEditorCommand
        )))
    }
    else if (!strcmp(name_p, "logger")) {
        NH_CHECK(NH_TTY_ERROR_BAD_STATE, Nh_appendToList(Prototypes_p, Nh_TTY_createProgramPrototype(
            defaultProgNames_pp[1], 6, Nh_TTY_initLogger, Nh_TTY_drawLoggerRow, Nh_TTY_handleLoggerInput, 
            NULL, Nh_TTY_updateLogger, NULL 
        )))
    }
    else {NH_TTY_DIAGNOSTIC_END(NH_TTY_ERROR_BAD_STATE)}

    Nh_List Tiles = Nh_TTY_getTiles(((Nh_TTY_Terminal*)Terminal_p)->Tab_p->RootTile_p);
    for (int i = 0; i < Tiles.size; ++i) {
        Nh_TTY_Tile *Tile_p = Tiles.pp[i];
        NH_TTY_CHECK(NH_TTY_ERROR_BAD_STATE, Nh_TTY_appendProgramInstance(
            Tile_p->Console.Programs_p, Prototypes_p->pp[Prototypes_p->size - 1]
        ))
    }
    Nh_freeList(&Tiles, NH_FALSE);

NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS)
}

// INIT ============================================================================================

Nh_Array *Nh_TTY_createProgramInstances(
    Nh_List *Prototypes_p)
{
NH_TTY_BEGIN()

    Nh_Array *Programs_p = Nh_allocate(sizeof(Nh_Array));
    NH_TTY_CHECK_MEM(NULL, Programs_p)

    *Programs_p = Nh_initArray(sizeof(Nh_TTY_Program), 8);

    for (int i = 0; i < Prototypes_p->size; ++i) 
    {
        Nh_TTY_ProgramPrototype *Prototype_p = Prototypes_p->pp[i];
        NH_TTY_CHECK(NULL, Nh_TTY_appendProgramInstance(Programs_p, Prototype_p))
    }

NH_TTY_END(Programs_p)
}


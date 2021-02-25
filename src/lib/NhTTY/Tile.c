// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "Tab.h"
#include "Tile.h"
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
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// UPDATE ==========================================================================================

void Nh_TTY_updateTileSize(
    Nh_TTY_Tile *Tile_p, int screenRows, int screenCols)
{
NH_TTY_BEGIN()

    if (Tile_p->Parent_p == NULL) {
        Tile_p->rowPosition = 0;
        Tile_p->colPosition = 0;
        Tile_p->rowSize = screenRows;
        Tile_p->colSize = screenCols;
    }
    else {
        Tile_p->rowPosition = Tile_p->rowPosition;
        Tile_p->colPosition = Tile_p->colPosition;
        Tile_p->rowSize = Tile_p->rowSize;
        Tile_p->colSize = Tile_p->colSize;
    }

//    if (Tile_p->Console.Programs_p != NULL) {
//        Nh_TTY_resizeShell(Nh_TTY_getProgram(Tile_p->Console.Programs_p, NH_TTY_SHELL_NAME)->handle_p, Tile_p->rowSize, Tile_p->colSize, -1, -1);
//    }

    for (int i = 0; i < Tile_p->Children.count; ++i) 
    {
        Nh_TTY_Tile *Child_p = Nh_getFromLinkedList(&Tile_p->Children, i);

        switch (Tile_p->orientation)
        {
            case NH_TTY_TILE_ORIENTATION_VERTICAL :
                Child_p->colSize     = Tile_p->colSize / Tile_p->Children.count;
                Child_p->colPosition = Tile_p->colPosition + Child_p->colSize * i;
                Child_p->rowSize     = Tile_p->rowSize;
                Child_p->rowPosition = Tile_p->rowPosition;
                if (Tile_p->colSize % Tile_p->Children.count > 0 && i == Tile_p->Children.count - 1) {
                    Child_p->colSize += Tile_p->colSize % Tile_p->Children.count;
                }
                break;
            case NH_TTY_TILE_ORIENTATION_HORIZONTAL :
                Child_p->colSize     = Tile_p->colSize;
                Child_p->colPosition = Tile_p->colPosition;
                Child_p->rowSize     = Tile_p->rowSize / Tile_p->Children.count;
                Child_p->rowPosition = Tile_p->rowPosition + Child_p->rowSize * i;
                if (Tile_p->rowSize % Tile_p->Children.count > 0 && i == Tile_p->Children.count - 1) {
                    Child_p->rowSize += Tile_p->rowSize % Tile_p->Children.count;
                }
                break;
        }

        Nh_TTY_updateTileSize(Child_p, screenRows, screenCols);
    }

NH_TTY_SILENT_END()
}

NH_TTY_RESULT Nh_TTY_updateTile(
    Nh_TTY_Tile *Tile_p)
{
NH_TTY_BEGIN()

    if (Tile_p->Console.Programs_p == NULL) {NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS)}

    Nh_TTY_Program *Current_p = Nh_TTY_getCurrentProgram(&Tile_p->Console);
    if (Current_p == NULL) {NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS)}

    if (Current_p->Prototype_p->update_f != NULL) {
        NH_TTY_CHECK(Current_p->Prototype_p->update_f(Current_p))
    }
 
    if (Current_p->refresh == NH_TRUE) {
        Tile_p->refresh = NH_TRUE;
        Current_p->refresh = NH_FALSE;
    }
    
NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS)
}

// INIT ============================================================================================

Nh_TTY_Tile *Nh_TTY_initTile(
    Nh_TTY_Tile *Parent_p, Nh_Array *Programs_p, int index)
{
NH_TTY_BEGIN()

    Nh_TTY_Tile *Tile_p = Nh_allocate(sizeof(Nh_TTY_Tile));

    Tile_p->refresh     = NH_FALSE;
    Tile_p->orientation = NH_TTY_TILE_ORIENTATION_VERTICAL;
    Tile_p->Children    = Nh_initLinkedList();
    Tile_p->Parent_p    = Parent_p;
    Tile_p->Prev_p      = NULL;
    Tile_p->Console     = Programs_p == NULL || Parent_p == NULL ? Nh_TTY_initConsole(Programs_p) : Parent_p->Console;
    Tile_p->close       = NH_FALSE;

    if (Parent_p != NULL) {
        Nh_insertIntoLinkedList(&Parent_p->Children, Tile_p, index);
    }

NH_TTY_END(Tile_p)
}

// INSERT ==========================================================================================

void Nh_TTY_moveInsertTile(
    Nh_TTY_Tile *Tile_p, NH_UNICODE_CODEPOINT c)
{
NH_TTY_BEGIN()

    int index = Nh_TTY_getInsertTileIndex(Tile_p);
    if (index == -1) {NH_TTY_SILENT_END()}

    Nh_TTY_Tile *Insert_p = Nh_getFromLinkedList(&Tile_p->Children, index);

    switch (c) 
    {
        case NH_TTY_INSERT_TILE_TOP_KEY :
            Tile_p->orientation = NH_TTY_TILE_ORIENTATION_HORIZONTAL;
            if (index > 0) {
                Nh_setInLinkedList(&Tile_p->Children, index, Nh_getFromLinkedList(&Tile_p->Children, index - 1));
                Nh_setInLinkedList(&Tile_p->Children, index - 1, Insert_p);
            }
            break;

        case NH_TTY_INSERT_TILE_BOTTOM_KEY :
            Tile_p->orientation = NH_TTY_TILE_ORIENTATION_HORIZONTAL;
            if (index < Tile_p->Children.count - 1) {
                Nh_setInLinkedList(&Tile_p->Children, index, Nh_getFromLinkedList(&Tile_p->Children, index + 1));
                Nh_setInLinkedList(&Tile_p->Children, index + 1, Insert_p);
            }
            break;

        case NH_TTY_INSERT_TILE_LEFT_KEY :
            Tile_p->orientation = NH_TTY_TILE_ORIENTATION_VERTICAL;
            if (index > 0) {
                Nh_setInLinkedList(&Tile_p->Children, index, Nh_getFromLinkedList(&Tile_p->Children, index - 1));
                Nh_setInLinkedList(&Tile_p->Children, index - 1, Insert_p);
            }
            break;

        case NH_TTY_INSERT_TILE_RIGHT_KEY :
            Tile_p->orientation = NH_TTY_TILE_ORIENTATION_VERTICAL;
            if (index < Tile_p->Children.count - 1) {
                Nh_setInLinkedList(&Tile_p->Children, index, Nh_getFromLinkedList(&Tile_p->Children, index + 1));
                Nh_setInLinkedList(&Tile_p->Children, index + 1, Insert_p);
            }
            break;
    }

NH_TTY_SILENT_END()
}

static Nh_TTY_Tile *Nh_TTY_insertTile(
    Nh_TTY_Tile *Tile_p, Nh_TTY_Tile *Parent_p, NH_UNICODE_CODEPOINT c)
{
NH_TTY_BEGIN()

    switch (c) 
    {
        case NH_TTY_INSERT_TILE_TOP_KEY    :
        case NH_TTY_INSERT_TILE_BOTTOM_KEY :
            Parent_p->orientation = NH_TTY_TILE_ORIENTATION_HORIZONTAL;
            Tile_p->orientation = NH_TTY_TILE_ORIENTATION_HORIZONTAL;
            break;
        case NH_TTY_INSERT_TILE_LEFT_KEY  :
        case NH_TTY_INSERT_TILE_RIGHT_KEY :
            Parent_p->orientation = NH_TTY_TILE_ORIENTATION_VERTICAL;
            Tile_p->orientation = NH_TTY_TILE_ORIENTATION_VERTICAL;
            break;
    }

    switch (c)
    {
        case NH_TTY_INSERT_TILE_TOP_KEY  :
        case NH_TTY_INSERT_TILE_LEFT_KEY :
            if (Parent_p->Children.count == 0) {
                Nh_TTY_initTile(Parent_p, NULL, 0);
                Nh_TTY_initTile(Parent_p, Tile_p->Console.Programs_p, 1);
                Tile_p->Console.Programs_p = NULL;
            }
            else {Nh_TTY_initTile(Parent_p, NULL, 0);}
            break;
        case NH_TTY_INSERT_TILE_BOTTOM_KEY :
        case NH_TTY_INSERT_TILE_RIGHT_KEY  :
            if (Parent_p->Children.count == 0) {
                Nh_TTY_initTile(Parent_p, Tile_p->Console.Programs_p, 0);
                Nh_TTY_initTile(Parent_p, NULL, 1);
                Tile_p->Console.Programs_p = NULL;
            }
            else {Nh_TTY_initTile(Parent_p, NULL, Parent_p->Children.count);}
            break;
    }

NH_TTY_END(Parent_p)
}

Nh_TTY_Tile *Nh_TTY_addTile(
    Nh_TTY_Tile *Tile_p, NH_UNICODE_CODEPOINT c)
{
NH_TTY_BEGIN()

    if (Tile_p->Parent_p == NULL || Tile_p->Parent_p->Children.count == 0) {NH_TTY_END(NULL)}

NH_TTY_END(Nh_TTY_insertTile(Tile_p, Tile_p->Parent_p, c))
}

Nh_TTY_Tile *Nh_TTY_splitTile(
    Nh_TTY_Tile *Tile_p, NH_UNICODE_CODEPOINT c)
{
NH_TTY_BEGIN()

    if (Tile_p->Children.count != 0) {NH_TTY_END(NULL)}

NH_TTY_END(Nh_TTY_insertTile(Tile_p, Tile_p, c))
}

// GET =============================================================================================

int Nh_TTY_getTileNumber(
    Nh_TTY_Tile *Current_p, Nh_TTY_Tile *Stop_p, int *nr_p)
{
NH_TTY_BEGIN()

    if (Current_p == Stop_p) {NH_TTY_END(*nr_p)}

    for (int i = 0; i < Current_p->Children.count; ++i) {
        Nh_TTY_Tile *Child_p = Nh_getFromLinkedList(&Current_p->Children, i);
        if (Child_p->Children.count <= 0) {*nr_p += 1;}
        int result = Nh_TTY_getTileNumber(Child_p, Stop_p, nr_p);
        if (result > -1) {NH_TTY_END(result)}
    }

NH_TTY_END(-1)
}

Nh_TTY_Tile *Nh_TTY_getTileFromNumber(
    Nh_TTY_Tile *Current_p, int stop, int *nr_p)
{
NH_TTY_BEGIN()

    if (*nr_p == stop) {NH_TTY_END(Current_p)}

    for (int i = 0; i < Current_p->Children.count; ++i) {
        Nh_TTY_Tile *Child_p = Nh_getFromLinkedList(&Current_p->Children, i);
        if (Child_p->Children.count <= 0) {*nr_p += 1;}
        Nh_TTY_Tile *Result_p = Nh_TTY_getTileFromNumber(Child_p, stop, nr_p);
        if (Result_p != NULL) {NH_TTY_END(Result_p)}
    }

NH_TTY_END(NULL)
}

Nh_List Nh_TTY_getTiles(
    Nh_TTY_Tile *Root_p)
{
NH_TTY_BEGIN()

    Nh_List List = Nh_initList(4);

    for (int i = 0;;++i) {
        int start = 0;
        Nh_TTY_Tile *Tile_p = Nh_TTY_getTileFromNumber(Root_p, i, &start);
        if (Tile_p == NULL) {break;}
        Nh_appendToList(&List, Tile_p);      
    }

NH_TTY_END(List)
}

int Nh_TTY_getInsertTileIndex(
    Nh_TTY_Tile *Tile_p)
{
NH_TTY_BEGIN()

    for (int i = 0; i < Tile_p->Children.count; ++i) {
        Nh_TTY_Tile *Child_p = Nh_getFromLinkedList(&Tile_p->Children, i);
        if (Child_p->Console.Programs_p == NULL && Child_p->Children.count == 0) {NH_TTY_END(i)}
    }

NH_TTY_END(-1)
}

// COLOR ===========================================================================================

NH_BYTE *NH_TTY_INVERSE_TILE_COLORS_PP[] = {
    "\x1b[7;34m", // BLU
    "\x1b[7;32m", // GRN
    "\x1b[7;33m", // YEL
    "\x1b[7;35m", // MAG
    "\x1b[7;36m", // CYN
    "\x1b[7;31m", // RED
    "\x1b[7;37m", // WHT
};

static NH_TTY_TILE_COLOR Nh_TTY_computeTileColor(
    int index)
{
NH_TTY_BEGIN()
NH_TTY_END(index % 7)
}

NH_TTY_TILE_COLOR Nh_TTY_getTileColor(
    Nh_TTY_Tile *Tile_p)
{
NH_TTY_BEGIN()

    NH_TTY_TILE_COLOR color = NH_TTY_TILE_COLOR_WHITE;

    Nh_TTY_Terminal *Terminal_p = Nh_TTY_getTerminal();
    if (Terminal_p == NULL) {NH_TTY_DIAGNOSTIC_END(NH_TTY_ERROR_BAD_STATE)}

    Nh_List Tiles = Nh_TTY_getTiles(Terminal_p->Tab_p->RootTile_p);

    int i = 0;
    for (i = 0; i < Tiles.size; ++i) {
        if (Tiles.handles_pp[i] == Tile_p) {break;}
    }

    color = Nh_TTY_computeTileColor(i);
    Nh_freeList(&Tiles, NH_FALSE);

NH_TTY_END(color)
}

NH_TTY_TILE_COLOR Nh_TTY_getTileColorFromProgHandle(
    void *prog_p)
{
NH_TTY_BEGIN()

    NH_TTY_TILE_COLOR color = NH_TTY_TILE_COLOR_WHITE;
    Nh_TTY_Terminal *Terminal_p = Nh_TTY_getTerminal();

    Nh_List Tiles = Nh_TTY_getTiles(Terminal_p->Tab_p->RootTile_p);

    int index = -1;
    for (int i = 0; i < Tiles.size; ++i) {
        Nh_TTY_Tile *Tile_p = Tiles.handles_pp[i];
        for (int j = 0; Tile_p->Console.Programs_p != NULL && j < Tile_p->Console.Programs_p->length; ++j) {
            if (((Nh_TTY_Program*)Tile_p->Console.Programs_p->bytes_p)[j].handle_p == prog_p) {
                index = i;
                break;
            }
        }
        if (index > -1) {break;}
    }

    color = Nh_TTY_computeTileColor(index);
    Nh_freeList(&Tiles, NH_FALSE);

NH_TTY_END(color)
}

NH_TTY_TILE_COLOR Nh_TTY_getTileColorFromConsole(
    Nh_TTY_Console *Console_p)
{
NH_TTY_BEGIN()

    NH_TTY_TILE_COLOR color = NH_TTY_TILE_COLOR_WHITE;
    Nh_TTY_Terminal *Terminal_p = Nh_TTY_getTerminal();

    Nh_List Tiles = Nh_TTY_getTiles(Terminal_p->Tab_p->RootTile_p);

    int index = -1;
    for (int i = 0; i < Tiles.size; ++i) {
        if (&((Nh_TTY_Tile*)Tiles.handles_pp[i])->Console == Console_p) {
            index = i;
            break;
        }
    }

    if (index > -1) {color = Nh_TTY_computeTileColor(index);}
    Nh_freeList(&Tiles, NH_FALSE);

NH_TTY_END(color)
}

// DRAW ============================================================================================

static NH_TTY_RESULT Nh_TTY_drawPreviewRow(
    Nh_TTY_Tile *Tile_p, Nh_String *Data_p, int row, int rows, int width)
{
NH_TTY_BEGIN()

    Nh_appendToString(Data_p, NH_TTY_INVERSE_TILE_COLORS_PP[Nh_TTY_getTileColor(Tile_p)], 7);
    Nh_appendToString(Data_p, " ", 1);

    if (row >= 0 && row < rows - 1) {
        Nh_appendToString(Data_p, "\x1b[0m", 4);
    }
    for (int i = 0; i < width - 2; ++i) {Nh_appendToString(Data_p, " ", 1);}
    if (row >= 0 && row < rows - 1) {
        Nh_appendToString(Data_p, NH_TTY_INVERSE_TILE_COLORS_PP[Nh_TTY_getTileColor(Tile_p)], 7);
    }

    Nh_appendToString(Data_p, " \x1b[0m", 5);

NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS)
}

static NH_TTY_RESULT Nh_TTY_drawEmptyRow(
    Nh_TTY_Tile *Tile_p, Nh_String *Data_p, int width)
{
NH_TTY_BEGIN()

    for (int i = 0; i < width; ++i) {
        NH_CHECK(NH_TTY_ERROR_BAD_STATE, Nh_appendToString(Data_p, " ", 1))
    }

NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS)
}

NH_TTY_RESULT Nh_TTY_drawTileRow(
    Nh_TTY_Tile *Tile_p, Nh_String *Row_p, int row)
{
NH_TTY_BEGIN()

    int relativeRow = Tile_p->Console.displace ? row - Tile_p->Console.height - Tile_p->rowPosition : row - Tile_p->rowPosition - 1;
    int height = Tile_p->rowSize - Tile_p->Console.height;

    if (Tile_p->Console.displace ? relativeRow < 0 : relativeRow < Tile_p->Console.height - 1) {
        NH_TTY_CHECK(Nh_TTY_drawConsoleRow(&Tile_p->Console, Row_p, relativeRow, Tile_p->colSize))
    }
    else if (Tile_p->Console.Programs_p == NULL) {
        NH_TTY_CHECK(Nh_TTY_drawPreviewRow(Tile_p, Row_p, relativeRow, height, Tile_p->colSize))
    }
    else if (Nh_TTY_getCurrentProgram(&Tile_p->Console) != NULL) {
        NH_TTY_CHECK(Nh_TTY_getCurrentProgram(&Tile_p->Console)->Prototype_p->draw_f(
            Nh_TTY_getCurrentProgram(&Tile_p->Console), Row_p, Tile_p->colSize, height, relativeRow
        ))
    }
    else {NH_TTY_CHECK(Nh_TTY_drawEmptyRow(Tile_p, Row_p, Tile_p->colSize))}

NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS)
}

// INPUT ===========================================================================================

NH_TTY_RESULT Nh_TTY_handleTileInput(
    Nh_TTY_Tile *Tile_p, NH_UNICODE_CODEPOINT c)
{
NH_TTY_BEGIN()

    if (Tile_p->Console.block) {
        NH_BOOL _continue = NH_FALSE;
        NH_TTY_CHECK(Tile_p->Console.callback_f(&Tile_p->Console, c, &_continue))
        Tile_p->refresh = NH_TRUE; 
        if (!_continue) {NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS)}
    }

    if (c == NH_TTY_QUIT_KEY) {
        Tile_p->close = NH_TRUE;
    }
    else if (c == NH_TTY_CONSOLE_KEY) {
        Nh_TTY_toggleConsole(&Tile_p->Console);
        Tile_p->refresh = NH_TRUE;
    }
    else if (Tile_p->Console.hasFocus) {
        NH_TTY_CHECK(Nh_TTY_handleConsoleInput(&Tile_p->Console, c))
        Tile_p->refresh = NH_TRUE;
    }
    else if (Nh_TTY_getCurrentProgram(&Tile_p->Console) != NULL) {
        NH_TTY_CHECK(Nh_TTY_getCurrentProgram(&Tile_p->Console)->Prototype_p->handleInput_f(
            Nh_TTY_getCurrentProgram(&Tile_p->Console), c
        ))
        if (Nh_TTY_getCurrentProgram(&Tile_p->Console)->refresh) {
            Tile_p->refresh = NH_TRUE;
            Nh_TTY_getCurrentProgram(&Tile_p->Console)->refresh = NH_FALSE;
        }
    }
    else {NH_TTY_CHECK(Nh_TTY_setDefaultMessage(&Tile_p->Console, NH_TTY_MESSAGE_MISC_NO_CURRENT_PROGRAM))}

NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS)
}

NH_TTY_RESULT Nh_TTY_handleCurrentTileInput(
    NH_UNICODE_CODEPOINT c)
{
NH_TTY_BEGIN()

    Nh_TTY_Terminal *Terminal_p = Nh_TTY_getTerminal();
    NH_TTY_CHECK_NULL(Terminal_p)

    Nh_TTY_Tile *Tile_p = Terminal_p->Tab_p->Tile_p;
    NH_TTY_CHECK_NULL(Tile_p)

NH_TTY_DIAGNOSTIC_END(Nh_TTY_handleTileInput(Tile_p, c))
}


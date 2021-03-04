// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 *
 * See https://vt100.net/docs/vt100-ug/chapter3.html for escape sequences.
 */

// DEFINE ==========================================================================================

#define _DEFAULT_SOURCE
#define _BSD_SOURCE
#define _GNU_SOURCE

// INCLUDE =========================================================================================

#include "Draw.h"
#include "Shell.h"
#include "Editor.h"
#include "Tile.h"
#include "Row.h"
#include "Console.h"

#include "../NhCore/Memory.h"

#include "Common/Macro.h"
#include NH_TTY_FLOW
#include NH_TTY_DEFAULT_CHECK

#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>

// CURSOR ==========================================================================================

static NH_TTY_RESULT Nh_TTY_getCursorPosition(
    Nh_TTY_Terminal *Terminal_p, int *x_p, int *y_p)
{
NH_TTY_BEGIN()

    Nh_TTY_Tile *Tile_p = Terminal_p->Tab_p->Tile_p;
    Nh_TTY_Program *Current_p = Nh_TTY_getCurrentProgram(&Tile_p->Console);

    *x_p = -1;
    *y_p = -1;

    if (Tile_p->Console.hasFocus) {
        *x_p = Tile_p->Console.cursorX;
        *y_p = Tile_p->Console.cursorY + 1;
    }
    else if (Current_p != NULL && Current_p->Prototype_p->getCursorPosition_f != NULL) {
        NH_TTY_CHECK(Current_p->Prototype_p->getCursorPosition_f(Current_p, x_p, y_p))
        *y_p += Tile_p->Console.height; 
    }

    if (*x_p > -1 && *y_p > -1) {
        *x_p += Tile_p->colPosition + 1;
        *y_p += Tile_p->rowPosition;
    }

NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS)
}

NH_TTY_RESULT Nh_TTY_refreshCursor(
    Nh_TTY_Terminal *Terminal_p) 
{
NH_TTY_BEGIN()

    Nh_String Data = Nh_initString(255);

    int x, y;
    NH_TTY_CHECK(Nh_TTY_getCursorPosition(Terminal_p, &x, &y))
    
    if (x > -1 && y > -1) 
    {
        char buf[32] = {'\0'};
        snprintf(buf, sizeof(buf), "\x1b[%d;%dH", y, x);
        Nh_appendFormatToString(&Data, buf);
        Nh_appendFormatToString(&Data, "\x1b[?25h");

    #ifdef __unix__
        write(STDOUT_FILENO, Data.bytes_p, Data.length);
    #endif

        Nh_freeString(&Data);
    }

    Terminal_p->Tab_p->refreshCursor = NH_FALSE;

NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS)
}

// REFRESH =========================================================================================

NH_TTY_RESULT Nh_TTY_refreshScreen(
    Nh_TTY_Terminal *Terminal_p) 
{
NH_TTY_BEGIN()

    for (int i = 0; i < Terminal_p->Tabs.count; ++i) {
        Nh_TTY_Tab *Tab_p = Nh_getFromLinkedList(&Terminal_p->Tabs, i);
        Nh_TTY_updateTileSize(Tab_p->RootTile_p, Terminal_p->screenRows, Terminal_p->screenCols);
    }

    Nh_String Data = Nh_initString(255);
    Nh_TTY_Tab *Tab_p = Terminal_p->Tab_p;

    Nh_appendFormatToString(&Data, "\x1b[H"); // move cursor to home
    Nh_appendFormatToString(&Data, "\x1b[?25l");
//    Nh_appendFormatToString(&Data, "\x1b[2J"); // clear entire screen
 
    Nh_List Tiles = Nh_TTY_getTiles(Tab_p->RootTile_p);

    for (int row = 0; row < Terminal_p->screenRows; ++row) {
        int col = 0;
        while (col < Terminal_p->screenCols) {
            for (int tile = 0; tile < Tiles.size; ++tile)
            {
                Nh_TTY_Tile *Tile_p = Tiles.pp[tile];
                if (Tile_p->Children.count > 0) {continue;}

                Tile_p->refresh = NH_FALSE;

                if (Tile_p->rowPosition <= row 
                &&  Tile_p->rowPosition + Tile_p->rowSize > row
                &&  Tile_p->colPosition == col)
                {
                    Nh_TTY_drawTabRow(Tab_p, Tile_p, &Data, row);
                    col += Tile_p->colSize;
                    break;    
                }
            }
        }
        if (row < Terminal_p->screenRows - 1) {Nh_appendFormatToString(&Data, "\r\n");}
    }

#ifdef __unix__
    write(STDOUT_FILENO, Data.bytes_p, Data.length);
#endif

    Nh_freeList(&Tiles, NH_FALSE);
    Nh_freeString(&Data);

    Tab_p->refreshScreen = NH_FALSE;

NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS)
}


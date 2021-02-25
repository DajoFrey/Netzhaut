#ifndef NH_TTY_TILE_H
#define NH_TTY_TILE_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "Console.h"
#include "Common/Result.h"

#include "../NhCore/String.h"
#include "../NhCore/LinkedList.h"
#include "../NhCore/List.h"

#endif

/** @addtogroup NhTTYMacros
 *  @{
 */

    #define CTRL_KEY(k) ((k) & 0x1f)

    #define NH_TTY_INSERT_TILE_LEFT_KEY   'a'
    #define NH_TTY_INSERT_TILE_RIGHT_KEY  'd'
    #define NH_TTY_INSERT_TILE_TOP_KEY    'w'
    #define NH_TTY_INSERT_TILE_BOTTOM_KEY 's'

    #define NH_TTY_QUIT_KEY CTRL_KEY('q')
    #define NH_TTY_CONSOLE_KEY CTRL_KEY('e')
    #define NH_TTY_TILING_KEY CTRL_KEY('r')

/** @} */

/** @addtogroup NhTTYEnums
 *  @{
 */

    typedef enum NH_TTY_TILE_ORIENTATION { 
        NH_TTY_TILE_ORIENTATION_VERTICAL,
        NH_TTY_TILE_ORIENTATION_HORIZONTAL,
    } NH_TTY_TILE_ORIENTATION;

    typedef enum NH_TTY_TILE_COLOR {
        NH_TTY_TILE_COLOR_BLUE,
        NH_TTY_TILE_COLOR_GREEN,
        NH_TTY_TILE_COLOR_YELLOW,
        NH_TTY_TILE_COLOR_MAGENTA,
        NH_TTY_TILE_COLOR_CYAN,
        NH_TTY_TILE_COLOR_RED,
        NH_TTY_TILE_COLOR_WHITE,
    } NH_TTY_TILE_COLOR;

/** @} */

/** @addtogroup NhTTYStructs
 *  @{
 */

    typedef struct Nh_TTY_Tile {
        Nh_TTY_Console Console;
        int rowPosition, colPosition;
        int rowSize, colSize;
        NH_BOOL refresh;
        NH_BOOL close;
        NH_TTY_TILE_ORIENTATION orientation;
        Nh_LinkedList Children;
        struct Nh_TTY_Tile *Parent_p;
        struct Nh_TTY_Tile *Prev_p;
    } Nh_TTY_Tile;

/** @} */

/** @addtogroup NhTTYVars
 *  @{
 */

    extern NH_BYTE *NH_TTY_INVERSE_TILE_COLORS_PP[];

/** @} */

/** @addtogroup NhTTYFunctions
 *  @{
 */

// UPDATE

    void Nh_TTY_updateTileSize(
        Nh_TTY_Tile *Tile_p, int screenRows, int screenCols
    );

    NH_TTY_RESULT Nh_TTY_updateTile(
        Nh_TTY_Tile *Tile_p
    );

// INIT

    Nh_TTY_Tile *Nh_TTY_initTile(
        Nh_TTY_Tile *Parent_p, Nh_Array *Programs_p, int index
    );
    
// INSERT

    Nh_TTY_Tile *Nh_TTY_addTile(
        Nh_TTY_Tile *Tile_p, NH_UNICODE_CODEPOINT c
    );
    
    Nh_TTY_Tile *Nh_TTY_splitTile(
        Nh_TTY_Tile *Tile_p, NH_UNICODE_CODEPOINT c
    );

// GET

    Nh_List Nh_TTY_getTiles(
        Nh_TTY_Tile *Root_p
    );

    int Nh_TTY_getInsertTileIndex(
        Nh_TTY_Tile *Tile_p
    );

    int Nh_TTY_getTileNumber(
        Nh_TTY_Tile *Current_p, Nh_TTY_Tile *Stop_p, int *nr_p
    );

    Nh_TTY_Tile *Nh_TTY_getTileFromNumber(
        Nh_TTY_Tile *Current_p, int stop, int *nr_p
    );

// COLOR

    NH_TTY_TILE_COLOR Nh_TTY_getTileColor(
        Nh_TTY_Tile *Tile_p
    );
    
    NH_TTY_TILE_COLOR Nh_TTY_getTileColorFromProgHandle(
        void *prog_p
    );

    NH_TTY_TILE_COLOR Nh_TTY_getTileColorFromConsole(
        Nh_TTY_Console *Console_p
    );

// DRAW

    NH_TTY_RESULT Nh_TTY_drawTileRow(
        Nh_TTY_Tile *Tile_p, Nh_String *Row_p, int row
    );

// INPUT 

    void Nh_TTY_moveInsertTile(
        Nh_TTY_Tile *Tile_p, NH_UNICODE_CODEPOINT c
    );

    NH_TTY_RESULT Nh_TTY_handleTileInput(
        Nh_TTY_Tile *Tile_p, NH_UNICODE_CODEPOINT c
    );

    NH_TTY_RESULT Nh_TTY_handleCurrentTileInput(
        NH_UNICODE_CODEPOINT c
    );

/** @} */

#endif 

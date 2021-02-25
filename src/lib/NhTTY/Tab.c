// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// DEFINE ==========================================================================================

#define _DEFAULT_SOURCE
#define _BSD_SOURCE
#define _GNU_SOURCE

// INCLUDE =========================================================================================

#include "Tab.h"
#include "Tile.h"
#include "Program.h"
#include "Terminal.h"
#include "Messages.h"

#include "../NhCore/Memory.h"
#include "../NhCore/Config.h"
//#include "../NhCore/Log.h"

#include "Common/Macro.h"
#include NH_TTY_FLOW
#include NH_TTY_DEFAULT_CHECK

#include <sys/ioctl.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

// INIT ============================================================================================

Nh_TTY_Tab *Nh_TTY_initTab(
    Nh_List *Prototypes_p)
{
NH_TTY_BEGIN()

#include NH_TTY_CUSTOM_CHECK

    Nh_TTY_Tab *Tab_p = Nh_allocate(sizeof(Nh_TTY_Tab));
    NH_TTY_CHECK_MEM(NULL, Tab_p)

    Nh_Array *Programs_p = Nh_TTY_createProgramInstances(Prototypes_p);

    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    Tab_p->tilingState   = 0;
    Tab_p->RootTile_p    = Nh_TTY_initTile(NULL, Programs_p, 0);
    Tab_p->Tile_p        = Tab_p->RootTile_p;
    Tab_p->refreshScreen = NH_TRUE;
    Tab_p->refreshCursor = NH_TRUE;
    Tab_p->close         = NH_FALSE;

#include NH_TTY_DEFAULT_CHECK

NH_TTY_END(Tab_p)
}

// UPDATE ==========================================================================================

NH_TTY_RESULT Nh_TTY_updateTab(
    Nh_TTY_Tab *Tab_p)
{
NH_TTY_BEGIN()

    Nh_List Tiles = Nh_TTY_getTiles(Tab_p->RootTile_p);

    for (int i = 0; i < Tiles.size; ++i) 
    {
        Nh_TTY_Tile *Tile_p = Tiles.handles_pp[i];
        NH_TTY_CHECK(Nh_TTY_updateTile(Tile_p))

        if (Tile_p->refresh) {
            Tab_p->refreshScreen = NH_TRUE;
        }
        if (Tile_p->close) 
        {
            if (Tile_p->Parent_p == NULL) {Tab_p->close = NH_TRUE;}
            else {
                int index = Nh_getLinkedListIndex(&Tile_p->Parent_p->Children, Tile_p);
                if (Tile_p->Parent_p->Children.count == 2) {
                    Tile_p->Parent_p->Console.Programs_p = 
                        ((Nh_TTY_Tile*)Nh_getFromLinkedList(&Tile_p->Parent_p->Children, index == 0 ? 1 : 0))->Console.Programs_p;
                    Tab_p->Tile_p = Tile_p->Parent_p;
                    Nh_destroyLinkedList(&Tab_p->Tile_p->Children, NH_TRUE);
                    break;
                }
                else {
                    index = index == 0 ? 1 : index - 1;
                    Tab_p->Tile_p = Nh_getFromLinkedList(&Tile_p->Parent_p->Children, index);
                    Nh_removeFromLinkedList2(&Tile_p->Parent_p->Children, Tile_p, NH_TRUE);
                    break;
                }
            }
        }
    }

    Nh_freeList(&Tiles, NH_FALSE);

NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS)
}

// BUSY? ===========================================================================================

NH_BOOL Nh_TTY_tabBusy(
    Nh_TTY_Tab *Tab_p)
{
NH_TTY_BEGIN()

    NH_BOOL busy = NH_FALSE;
//    Nh_List Tiles = Nh_TTY_getTiles(Tab_p->RootTile_p);
//
//    for (int i = 0; i < Tiles.size; ++i) {
//        if (Nh_TTY_programBusy(((Nh_TTY_Tile*)Tiles.handles_pp[i])->Console.Programs_p)) {
//            busy = NH_TRUE;
//            break;
//        }
//    }
//
//    Nh_freeList(&Tiles, NH_FALSE);

NH_TTY_END(busy)
}

// DRAW ============================================================================================

NH_TTY_RESULT Nh_TTY_drawTabRow(
    Nh_TTY_Tab *Tab_p, Nh_TTY_Tile *Tile_p, Nh_String *Data_p, int row)
{
NH_TTY_BEGIN()
NH_TTY_DIAGNOSTIC_END(Nh_TTY_drawTileRow(Tile_p, Data_p, row))
}

// CONSOLES ========================================================================================

static NH_TTY_RESULT Nh_TTY_updateConsoles(
    Nh_TTY_Tab *Tab_p)
{
NH_TTY_BEGIN()

    Tab_p->refreshScreen = NH_TRUE;
    Nh_List Tiles = Nh_TTY_getTiles(Tab_p->RootTile_p);

    for (int i = 0; i < Tiles.size; ++i)
    {
        Nh_TTY_Tile *Tile_p = Tiles.handles_pp[i];

        if (Tile_p != Tab_p->Tile_p)
        {
              Nh_UnicodeString FocusOrInsert = Nh_initUnicodeString(32);
              NH_UNICODE_CODEPOINT c = '[';
              Nh_appendToUnicodeString(&FocusOrInsert, &c, 1);
              int start = 0;
              c = Nh_TTY_getTileNumber(Tab_p->RootTile_p, Tile_p, &start) + 48;
              Nh_appendToUnicodeString(&FocusOrInsert, &c, 1);
              int messageLength;
              NH_UNICODE_CODEPOINT *message_p = Nh_TTY_getMessage(Tile_p->Console.Programs_p != NULL ? NH_TTY_MESSAGE_TILING_FOCUS : NH_TTY_MESSAGE_TILING_INSERT, &messageLength);
              Nh_appendToUnicodeString(&FocusOrInsert, message_p, messageLength);
              NH_TTY_CHECK(Nh_TTY_setMessage(&Tile_p->Console, (NH_UNICODE_CODEPOINT*)FocusOrInsert.bytes_p, FocusOrInsert.length))
              Nh_freeUnicodeString(&FocusOrInsert);
        }
        else if (Tile_p->Parent_p == NULL) {
            NH_TTY_CHECK(Nh_TTY_setDefaultMessage(&Tile_p->Console, NH_TTY_MESSAGE_TILING_WASD))
        }
        else {
            NH_TTY_CHECK(Nh_TTY_setDefaultMessage(&Tile_p->Console, NH_TTY_MESSAGE_TILING_WASDF))
        }
    }

    Nh_freeList(&Tiles, NH_FALSE);

NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS)
}

static NH_TTY_RESULT Nh_TTY_clearFreshConsoleLines(
    Nh_TTY_Tab *Tab_p)
{
NH_TTY_BEGIN()

    Nh_List Tiles = Nh_TTY_getTiles(Tab_p->RootTile_p);

    for (int i = 0; i < Tiles.size; ++i) 
    {
        Nh_TTY_Tile *Tile_p = Tiles.handles_pp[i];
        NH_TTY_CHECK(Nh_TTY_setMessage(&Tile_p->Console, NULL, 0))
        Tile_p->refresh = NH_TRUE;
    }

    Nh_freeList(&Tiles, NH_FALSE);

NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS)
}

// INPUT ===========================================================================================

static Nh_TTY_Tile *Nh_TTY_getTileFromCodepoint(
    Nh_TTY_Tab *Tab_p, NH_UNICODE_CODEPOINT c)
{
NH_TTY_BEGIN()

    int start = 0;
    char c_p[2] = {'\0'};
    c_p[0] = c;

NH_TTY_END(Nh_TTY_getTileFromNumber(Tab_p->RootTile_p, atoi(c_p), &start))
}

static NH_TTY_RESULT Nh_TTY_leaveTilingMode(
    Nh_TTY_Tab *Tab_p)
{
NH_TTY_BEGIN()

    Tab_p->tilingState = 0;
    Tab_p->refreshScreen = NH_TRUE;
    NH_TTY_DIAGNOSTIC_END(Nh_TTY_clearFreshConsoleLines(Tab_p))

NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS)
}

static NH_TTY_RESULT Nh_TTY_focusTile(
    Nh_TTY_Tab *Tab_p, Nh_TTY_Tile *Focus_p)
{
NH_TTY_BEGIN()

    NH_TTY_CHECK(Nh_TTY_leaveTilingMode(Tab_p))

    if (Tab_p->Tile_p != Focus_p) {
        NH_TTY_CHECK(Nh_TTY_setDefaultMessage(&Focus_p->Console, NH_TTY_MESSAGE_TILING_FOCUS_SWITCHED))
    }
    else {
        NH_TTY_CHECK(Nh_TTY_setDefaultMessage(&Focus_p->Console, NH_TTY_MESSAGE_TILING_ALREADY_FOCUSED))
    }

    Tab_p->Tile_p = Focus_p;

NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS)
}

static NH_TTY_RESULT Nh_TTY_handleTileInsertion(
    Nh_TTY_Tab *Tab_p, NH_UNICODE_CODEPOINT c)
{
NH_TTY_BEGIN()

    int index = Nh_TTY_getInsertTileIndex(Tab_p->Tile_p);
    if (index == -1) {NH_TTY_DIAGNOSTIC_END(NH_TTY_ERROR_BAD_STATE)}

    Nh_TTY_Tile *Select_p = NULL;

    if (c > 47 && c < 58) 
    {
        Select_p = Nh_TTY_getTileFromCodepoint(Tab_p, c);
        if (Select_p == Nh_getFromLinkedList(&Tab_p->Tile_p->Children, index)) {
            Select_p->Console.Programs_p = Nh_TTY_createProgramInstances(&Nh_TTY_getTerminal()->Prototypes); 
            NH_TTY_DIAGNOSTIC_END(Nh_TTY_focusTile(Tab_p, Select_p))
        }
    }

    switch (c) 
    {
        case NH_TTY_INSERT_TILE_TOP_KEY    :
        case NH_TTY_INSERT_TILE_BOTTOM_KEY :
        case NH_TTY_INSERT_TILE_LEFT_KEY   :
        case NH_TTY_INSERT_TILE_RIGHT_KEY  :

            Nh_TTY_moveInsertTile(Tab_p->Tile_p, c);
            break;

        default :

            if (Tab_p->Tile_p->Children.count == 2) 
            {
                Tab_p->Tile_p->Console.Programs_p = 
                    ((Nh_TTY_Tile*)Nh_getFromLinkedList(&Tab_p->Tile_p->Children, index == 0 ? 1 : 0))->Console.Programs_p;
                if (Select_p != NULL && Nh_inLinkedList(&Tab_p->Tile_p->Children, Select_p)) {Select_p = Tab_p->Tile_p;}
                Nh_destroyLinkedList(&Tab_p->Tile_p->Children, NH_TRUE);
            }
            else {Nh_removeFromLinkedList(&Tab_p->Tile_p->Children, index, NH_TRUE);}

            if (Select_p != NULL) {NH_TTY_DIAGNOSTIC_END(Nh_TTY_focusTile(Tab_p, Select_p))}
            else {NH_TTY_DIAGNOSTIC_END(Nh_TTY_leaveTilingMode(Tab_p))}
    }

NH_TTY_DIAGNOSTIC_END(Nh_TTY_updateConsoles(Tab_p))
}

NH_TTY_RESULT Nh_TTY_handleTabInput(
    Nh_TTY_Tab *Tab_p, NH_UNICODE_CODEPOINT c)
{
NH_TTY_BEGIN()

#include NH_TTY_DEFAULT_CHECK

    switch (Tab_p->tilingState)
    {
        case 0:
            if (c == NH_TTY_TILING_KEY) {
                Tab_p->tilingState = 1;
                NH_TTY_DIAGNOSTIC_END(Nh_TTY_updateConsoles(Tab_p))
            }
            break;

        case 1:
            if (c > 47 && c < 58 && Nh_TTY_getTileFromCodepoint(Tab_p, c) != NULL) {
                NH_TTY_DIAGNOSTIC_END(Nh_TTY_focusTile(Tab_p, Nh_TTY_getTileFromCodepoint(Tab_p, c)))
            }
            else if (c == 'f' && Tab_p->Tile_p->Parent_p != NULL) {
                Tab_p->Tile_p        = Nh_TTY_splitTile(Tab_p->Tile_p, NH_TTY_INSERT_TILE_RIGHT_KEY);
                Tab_p->tilingState   = 2;
                Tab_p->refreshScreen = NH_TRUE;
                NH_TTY_DIAGNOSTIC_END(Nh_TTY_updateConsoles(Tab_p))
            }
            else if (c == NH_TTY_INSERT_TILE_LEFT_KEY || c == NH_TTY_INSERT_TILE_RIGHT_KEY || c == NH_TTY_INSERT_TILE_TOP_KEY || c == NH_TTY_INSERT_TILE_BOTTOM_KEY) {
                Tab_p->Tile_p        = Tab_p->Tile_p->Parent_p == NULL ? Nh_TTY_splitTile(Tab_p->Tile_p, c) : Nh_TTY_addTile(Tab_p->Tile_p, c);
                Tab_p->tilingState   = 2;
                Tab_p->refreshScreen = NH_TRUE;
                NH_TTY_DIAGNOSTIC_END(Nh_TTY_updateConsoles(Tab_p))
            }
            else {NH_TTY_DIAGNOSTIC_END(Nh_TTY_leaveTilingMode(Tab_p))}

        case 2:
            Tab_p->refreshScreen = NH_TRUE;
            NH_TTY_DIAGNOSTIC_END(Nh_TTY_handleTileInsertion(Tab_p, c))
    }

    NH_TTY_CHECK(Nh_TTY_handleTileInput(Tab_p->Tile_p, c))
    Tab_p->refreshScreen = Tab_p->Tile_p->refresh;

NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS)
}


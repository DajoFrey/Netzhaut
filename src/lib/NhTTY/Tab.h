#ifndef NH_TTY_TAB_H
#define NH_TTY_TAB_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "Tile.h"

#include "Common/Result.h"

#endif

/** @addtogroup NhTTYStructs
 *  @{
 */

    typedef struct Nh_TTY_Tab {
        NH_BOOL refreshScreen;
        NH_BOOL refreshCursor;
        NH_BOOL close;
        int tilingState;
        Nh_TTY_Tile *RootTile_p;
        Nh_TTY_Tile *Tile_p;
    } Nh_TTY_Tab;

/** @} */

/** @addtogroup NhTTYFunctions
 *  @{
 */

    Nh_TTY_Tab *Nh_TTY_initTab(
        Nh_List *Prototypes_p 
    );

    NH_TTY_RESULT Nh_TTY_updateTab(
        Nh_TTY_Tab *Tab_p
    );

    NH_BOOL Nh_TTY_tabBusy(
        Nh_TTY_Tab *Tab_p
    );

    NH_TTY_RESULT Nh_TTY_drawTabRow(
        Nh_TTY_Tab *Tab_p, Nh_TTY_Tile *Tile_p, Nh_String *Data_p, int row
    );

    NH_TTY_RESULT Nh_TTY_handleTabInput(
        Nh_TTY_Tab *Tab_p, NH_UNICODE_CODEPOINT c
    );

/** @} */

#endif 

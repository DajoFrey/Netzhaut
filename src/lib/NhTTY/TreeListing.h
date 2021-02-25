#ifndef NH_TTY_TREE_LISTING_H
#define NH_TTY_TREE_LISTING_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "Program.h"
#include "Tile.h"
#include "FileEditor.h"
#include "Common/Result.h"

#include "../NhCore/Time.h"
#include "../NhCore/List.h"
#include "../NhCore/String.h"

#endif

/** @addtogroup NhTTYStructs
 *  @{
 */

    typedef struct Nh_TTY_TreeListingNode {
        NH_BOOL open;
        Nh_TTY_File *File_p;
        char *path_p;
        Nh_List Children; 
        struct Nh_TTY_TreeListingNode *Parent_p;
    } Nh_TTY_TreeListingNode;

    typedef struct Nh_TTY_TreeListing {
        Nh_TTY_TreeListingNode *Root_p;
        NH_BOOL preview;
        Nh_TTY_TreeListingNode *Preview_p;
        int treeCurrent;
        int screenCurrent;
        int rowOffset;
        int width;
        Nh_Array RenderLines;
        Nh_SystemTime LastUpdate;
        double updateIntervalInSeconds;
    } Nh_TTY_TreeListing;

/** @} */

/** @addtogroup NhTTYFunctions
 *  @{
 */

    Nh_TTY_TreeListing Nh_TTY_initTreeListing(
    );

    NH_TTY_RESULT Nh_TTY_updateTreeListing(
        Nh_TTY_TreeListing *Listing_p, NH_BOOL *refresh_p
    );

    NH_TTY_RESULT Nh_TTY_handleTreeListingInput(
        Nh_TTY_Program *Program_p, int rows, NH_UNICODE_CODEPOINT c
    );

    NH_TTY_RESULT Nh_TTY_drawTreeListingRow(
        Nh_TTY_TreeListing *Listing_p, Nh_String *Data_p, NH_TTY_TILE_COLOR color, int row
    );

    NH_TTY_RESULT Nh_TTY_setTreeListingCursor(
        Nh_TTY_TreeListing *Listing_p, int rows, Nh_TTY_File *File_p 
    );

    NH_TTY_RESULT Nh_TTY_insertNewFile(
        Nh_TTY_TreeListing *Listing_p, Nh_TTY_File *File_p
    );

/** @} */

#endif 

#ifndef NH_TTY_TEXT_EDITOR_H
#define NH_TTY_TEXT_EDITOR_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "Program.h"
#include "Common/Result.h"

#include "../NhCore/Unicode.h"
#include "../NhCore/String.h"
#include "../NhCore/LinkedList.h"
#include "../NhCore/List.h"

#endif

/** @addtogroup NhTTYEnums
 *  @{
 */

    typedef enum NH_TTY_FILE {
        NH_TTY_FILE_TEXT,
    } NH_TTY_FILE;

/** @} */


/** @addtogroup NhTTYStructs
 *  @{
 */

    typedef struct Nh_TTY_File {
        NH_TTY_FILE type;
        void *handle_p;
        NH_BYTE *path_p;
        Nh_String BottomBar;
        NH_BOOL readOnly;
    } Nh_TTY_File;

    typedef struct Nh_TTY_FileEditor {
        Nh_LinkedList Files;
        int current;
        int width;
        int helpScroll;
    } Nh_TTY_FileEditor;

/** @} */

/** @addtogroup NhTTYFunctions
 *  @{
 */

    Nh_TTY_FileEditor Nh_TTY_initFileEditor(
    );

    Nh_TTY_File *Nh_TTY_openFile(
        Nh_TTY_FileEditor *Editor_p, NH_BYTE *path_p, NH_BOOL readOnly
    );

    NH_TTY_RESULT Nh_TTY_closeFile(
        Nh_TTY_FileEditor *Editor_p, Nh_TTY_File *File_p
    );

    NH_TTY_RESULT Nh_TTY_handleFileEditorInput(
        Nh_TTY_Program *Program_p, int height, NH_UNICODE_CODEPOINT c
    );

    NH_TTY_RESULT Nh_TTY_drawFileEditorRow(
        Nh_TTY_FileEditor *Editor_p, Nh_String *Data_p, int width, int height, int row
    );

/** @} */

#endif 

#ifndef NH_TTY_EDITOR_H
#define NH_TTY_EDITOR_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "Program.h"
#include "TreeListing.h"
#include "FileEditor.h"
#include "Output.h"
#include "Common/Result.h"

#include "../NhCore/String.h"

#endif

/** @addtogroup NhTTYStructs
 *  @{
 */

    typedef struct Nh_TTY_Editor {
        int height;
        int focus;
        NH_BOOL insertMode;
        NH_BOOL treeListing;
        Nh_TTY_TreeListing TreeListing;
        Nh_TTY_FileEditor FileEditor;
        Nh_TTY_Output Output;
    } Nh_TTY_Editor;

/** @} */

/** @addtogroup NhTTYFunctions
 *  @{
 */

    void *Nh_TTY_initEditor(
    );

    NH_TTY_RESULT Nh_TTY_updateEditor(
        Nh_TTY_Program *Program_p
    );

    NH_TTY_RESULT Nh_TTY_handleEditorInput(
        Nh_TTY_Program *Program_p, NH_UNICODE_CODEPOINT c
    );

    NH_TTY_RESULT Nh_TTY_drawEditorRow(
        Nh_TTY_Program *Program_p, Nh_UTF8String *Row_p, int width, int height, int row 
    );

    NH_TTY_RESULT Nh_TTY_getEditorCursor(
        Nh_TTY_Program *Program_p, int *x_p, int *y_p
    );

    NH_TTY_RESULT Nh_TTY_executeEditorCommand(
        Nh_TTY_Program *Program_p, NH_UNICODE_CODEPOINT codepoint 
    );

/** @} */

#endif 

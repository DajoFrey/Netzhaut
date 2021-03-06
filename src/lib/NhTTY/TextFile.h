#ifndef NH_TTY_TEXT_FILE_H
#define NH_TTY_TEXT_FILE_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "FileEditor.h"
#include "Program.h"
#include "SyntaxHighlights.h"
#include "Common/Result.h"

#include "../NhCore/Unicode.h"
#include "../NhCore/String.h"
#include "../NhCore/LinkedList.h"
#include "../NhCore/List.h"

#endif

/** @addtogroup NhTTYStructs
 *  @{
 */

    typedef struct Nh_TTY_TextFile {
        NH_TTY_TEXT textType;
        int lineNumberOffset;
        int fileCursorXTarget;
        int fileCursorX;
        int fileCursorY;
        int screenCursorX;
        int screenCursorY;
        int rowOffset;
        int colOffset;
        int width;
        int tabSize;
        Nh_List Lines;
        Nh_Array RenderLines;
    } Nh_TTY_TextFile;

/** @} */

/** @addtogroup NhTTYFunctions
 *  @{
 */

    Nh_TTY_TextFile *Nh_TTY_openTextFile(
        NH_BYTE *path_p
    );

    NH_TTY_RESULT Nh_TTY_closeTextFile(
        Nh_TTY_TextFile *File_p
    );

    NH_TTY_RESULT Nh_TTY_executeTextFile(
        Nh_TTY_TextFile *TextFile_p
    );

    NH_TTY_RESULT Nh_TTY_parseWebTextFile(
        NH_BYTE *name_p, Nh_TTY_TextFile *TextFile_p
    );

    NH_TTY_RESULT Nh_TTY_handleTextFileInput(
        Nh_TTY_File *File_p, int height, NH_UNICODE_CODEPOINT c, NH_BOOL insertMode, NH_BOOL *refresh_p
    );

    NH_TTY_RESULT Nh_TTY_renderTextFile(
        Nh_TTY_TextFile *File_p, int lineWidth
    );

    NH_TTY_RESULT Nh_TTY_drawTextFileLine(
        Nh_TTY_TextFile *File_p, Nh_String *String_p, int line
    );

/** @} */

#endif 

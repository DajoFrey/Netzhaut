// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "FileEditor.h"
#include "TreeListing.h"
#include "TextFile.h"
#include "Program.h"
#include "Row.h"
#include "Terminal.h"
#include "Shell.h"
#include "Editor.h"
#include "SyntaxHighlights.h"

#include "../NhCore/Process.h"
#include "../NhCore/Memory.h"
#include "../NhCore/Common/Macro.h"
#include NH_FLOW
#include NH_CUSTOM_CHECK

#include "Common/Macro.h"
#include NH_TTY_FLOW
#include NH_TTY_DEFAULT_CHECK

#include <stddef.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>

// INIT ============================================================================================

Nh_TTY_FileEditor Nh_TTY_initFileEditor()
{
NH_TTY_BEGIN()

    Nh_TTY_FileEditor Editor;
    Editor.Files   = Nh_initLinkedList();
    Editor.current = 0;
    Editor.helpScroll = 0;
    Editor.width = 0;

NH_TTY_END(Editor)
}

// RENDER ==========================================================================================

static NH_TTY_RESULT Nh_TTY_renderBottomBar(
    Nh_TTY_File *File_p, int width)
{
NH_TTY_BEGIN()

    Nh_freeString(&File_p->BottomBar);
    File_p->BottomBar = Nh_initString(width);

    Nh_appendToString(&File_p->BottomBar, "\x1b[7m", 4);

    if (File_p->path_p != NULL)
    {
        NH_BYTE *fname_p = NULL;
        for (int i = strlen(File_p->path_p) - 1; i >= 0; --i) {
            if (File_p->path_p[i] == '/') {fname_p = File_p->path_p + i + 1; break;}
        }
        Nh_appendToString(&File_p->BottomBar, fname_p, strlen(fname_p)); 
        for (int i = 0; i < width - strlen(fname_p); ++i) {
            Nh_appendToString(&File_p->BottomBar, " ", 1);
        }
    }
    else {
        Nh_appendToString(&File_p->BottomBar, "New File", 8); 
        for (int i = 0; i < width - 8; ++i) {
            Nh_appendToString(&File_p->BottomBar, " ", 1);
        }
    }

    Nh_appendToString(&File_p->BottomBar, "\x1b[0m", 4);

NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS)
}

static NH_TTY_RESULT Nh_TTY_renderFile(
    Nh_TTY_File *File_p, int width)
{
NH_TTY_BEGIN()

    switch (File_p->type)
    {
        case NH_TTY_FILE_TEXT :
            NH_TTY_CHECK(Nh_TTY_renderTextFile(File_p->handle_p, width))
            break;
    }

NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS)
}

static NH_TTY_RESULT Nh_TTY_renderFileEditor(
    Nh_TTY_FileEditor *Editor_p)
{
NH_TTY_BEGIN()

    int oddLeftOver, fileWidth = Editor_p->width;

    if (Editor_p->Files.count > 0)
    {
        oddLeftOver = Editor_p->width / Editor_p->Files.count % Editor_p->Files.count;
        fileWidth = Editor_p->width / Editor_p->Files.count;
    }

    for (int file = 0; file < Editor_p->Files.count; ++file) 
    {
        Nh_TTY_File *File_p = Nh_getFromLinkedList(&Editor_p->Files, file);
        NH_TTY_CHECK_NULL(File_p)

        if (file == Editor_p->Files.count - 1) {fileWidth += oddLeftOver;}

        NH_TTY_CHECK(Nh_TTY_renderFile(File_p, fileWidth))
        NH_TTY_CHECK(Nh_TTY_renderBottomBar(File_p, fileWidth))
    }

NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS)
}

// OPEN/CLOSE FILE =================================================================================

static NH_BOOL Nh_TTY_hasFile(
    Nh_TTY_FileEditor *Editor_p, Nh_TTY_File *File_p)
{
NH_TTY_BEGIN()

    NH_BOOL hasFile = NH_FALSE;

    for (int i = 0; i < Editor_p->Files.count; ++i) {
        Nh_TTY_File *Compare_p = Nh_getFromLinkedList(&Editor_p->Files, i);
        if (File_p == Compare_p) {
            hasFile = NH_TRUE;
            break;
        }
    }

NH_TTY_END(hasFile)
}

Nh_TTY_File *Nh_TTY_openFile(
    Nh_TTY_FileEditor *Editor_p, NH_BYTE *path_p, NH_BOOL readOnly)
{
NH_TTY_BEGIN()

#include NH_TTY_CUSTOM_CHECK

    void *handle_p = Nh_TTY_openTextFile(path_p);
    NH_TTY_CHECK_NULL(NULL, handle_p)

    Nh_TTY_File *File_p = Nh_allocate(sizeof(Nh_TTY_File));
    NH_TTY_CHECK_MEM(NULL, File_p)

    File_p->type      = NH_TTY_FILE_TEXT;
    File_p->handle_p  = handle_p;
    File_p->BottomBar = Nh_initString(255);
    File_p->path_p    = Nh_allocateBytes(path_p);
    File_p->readOnly  = readOnly;

    if (path_p != NULL) {NH_TTY_CHECK_MEM(NULL, File_p->path_p)}

    Nh_prependToLinkedList(&Editor_p->Files, File_p);
    Editor_p->current = 0;

    Nh_TTY_renderFileEditor(Editor_p);

#include NH_TTY_DEFAULT_CHECK

NH_TTY_END(File_p)
}

NH_TTY_RESULT Nh_TTY_closeFile(
    Nh_TTY_FileEditor *Editor_p, Nh_TTY_File *File_p)
{
NH_TTY_BEGIN()

    if (File_p == NULL || !Nh_TTY_hasFile(Editor_p, File_p)) {NH_TTY_DIAGNOSTIC_END(NH_TTY_ERROR_BAD_STATE)}

    switch (File_p->type)
    {
        case NH_TTY_FILE_TEXT :
            Nh_TTY_closeTextFile(File_p->handle_p);
            break;
    }

    Nh_free(File_p->path_p);
    Nh_removeFromLinkedList2(&Editor_p->Files, File_p, NH_TRUE);

    Nh_TTY_renderFileEditor(Editor_p);

NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS)
}

// INPUT ===========================================================================================

static const NH_BYTE *help_pp[] =
{
    "",
    "NhTTY Editor",
    "",
    "\e[1;4mGeneral Controls\e[0m                                      ",
    "",
    "\e[1;7mt      \e[0m Toggle tree listing.                          ",
    "Press 't' to hide/show the tree listing.              ",
    "",
    "\e[1;7mf,g    \e[0m Jump to left/right opened file.               ",
    "Press 'f' to go to the left neighbour file (if any).  ",
    "Press 'g' to go to the right neighbour file (if any). ",
    "",
    "\e[1;7mm      \e[0m Toggle insert mode.                           ",
    "Press 'm' to toggle insert mode. Please note that you ",
    "need to hold CTRL if you want to perform any of the   ",
    "operations listed here while in insert mode. Insert   ",
    "mode allows for char insertion if the file type       ",
    "supports it.                                          ",
    "",
    "\e[1;7mp      \e[0m Toggle preview.                               ",
    "Press 'p' to toggle previews.                         ",
    "",
    "\e[1;7mw,a,s,d\e[0m Navigate tree listing.                        ",
    "Press 'w' and 's' to navigate up and down the tree    ",
    "listing. Press 'd' to open the current node. If the   ",
    "node is a file, the file will be opened for editing.  ",
    "Press 'd' twice on a directory node to set it as the  ",
    "root node of the directory tree. Press 'a' to close   ",
    "the current node, this will close any related files as",
    "well.                                                 ",
    "",
    "\e[1;7mn      \e[0m New file.                                     ",
    "Press 'n' to open a new file. The file is not written ",
    "until saved.                                          ",
    "",
    "\e[1;7mb      \e[0m Save file.                                    ",
    "Press 'b' to save the current file.                   ",
    "",
    "\e[1;4mGeneral Commands\e[0m                                      ",
    "",
    "\e[1;7mecma\e[0m                                                  ",
    "Execute current file using the ECMAScript engine.     ",
    "",
    "\e[1;4mText File Controls\e[0m                                    ",
    "",
    "\e[1;7mh,j,k,l\e[0m Vim like cursor control.                      ",
    "Press 'h' and 'l' to move the cursor left or right.   ",
    "Press 'k' and 'j' to move the cursor up or down.      ",
    "",
    "\e[1;7mu      \e[0m Backward delete.                              ",
    "\e[1;7mi      \e[0m Forward delete.                               ",
    "\e[1;7mo      \e[0m Add line.                                     ",
    "\e[1;7mx      \e[0m Delete line.                                  ",
    "\e[1;7mz      \e[0m Undo.                                         ",
    "\e[1;7my      \e[0m Redo.                                         ",
    "\e[1;7mc      \e[0m Select and Copy.                              ",
    "\e[1;7mv      \e[0m Paste.                                        ",
    "",
    "\e[1;4mText File Commands\e[0m                                    ",
};

static NH_TTY_RESULT Nh_TTY_handleFileInput(
    Nh_TTY_File *File_p, int height, NH_UNICODE_CODEPOINT c, NH_BOOL insertMode, NH_BOOL *refresh_p)
{
NH_TTY_BEGIN()

    switch (File_p->type)
    {
        case NH_TTY_FILE_TEXT :
            NH_TTY_CHECK(Nh_TTY_handleTextFileInput(
                File_p, height - 1, c, insertMode, refresh_p
            )) 
            break;    
    }

    if (File_p->readOnly) 
    {
        NH_UNICODE_CODEPOINT readOnly_p[] = {70, 105, 108, 101, 32, 105, 115, 32, 99, 117, 114, 114, 101, 110, 116, 108, 121, 32, 114, 101, 97, 100, 111, 110, 108, 121, 33};
        if (!Nh_TTY_compareMessage2(readOnly_p, sizeof(readOnly_p)/sizeof(NH_UNICODE_CODEPOINT))) {
            NH_TTY_CHECK(Nh_TTY_setMessage2(readOnly_p, sizeof(readOnly_p)/sizeof(NH_UNICODE_CODEPOINT)))
            *refresh_p = NH_TRUE;
        }
    }

NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS)
}

NH_TTY_RESULT Nh_TTY_handleFileEditorInput(
    Nh_TTY_Program *Program_p, int height, NH_UNICODE_CODEPOINT c)
{
NH_TTY_BEGIN()

    Nh_TTY_Editor *Editor_p = Program_p->handle_p;
    Nh_TTY_FileEditor *FileEditor_p = &((Nh_TTY_Editor*)Editor_p)->FileEditor;
    Nh_TTY_File *File_p = Nh_getFromLinkedList(&FileEditor_p->Files, FileEditor_p->current);

    if (File_p != NULL) {
        NH_TTY_CHECK(Nh_TTY_handleFileInput(File_p, height, c, Editor_p->insertMode, &Program_p->refresh))
    }
    else {
        switch (c)
        {
            case 'k' :
                if (FileEditor_p->helpScroll + height < sizeof(help_pp)/sizeof(help_pp[0])) {
                    FileEditor_p->helpScroll++;
                    Program_p->refresh = NH_TRUE;
                }
                break;
            case 'j' :
                if (FileEditor_p->helpScroll > 0) {FileEditor_p->helpScroll--;}
                Program_p->refresh = NH_TRUE;
                break;
        }
    }

NH_TTY_END(NH_TTY_SUCCESS)
}

// DRAW ============================================================================================

static int Nh_TTY_getTotalEscapeLength(
    NH_BYTE *string_p)
{
NH_TTY_BEGIN()

    int count = 0;
    NH_BOOL esc = NH_FALSE;

    for (int i = 0;; ++i) {
        if (string_p[i] == '\0') {break;}
        if (string_p[i] == '\x1b') {esc = NH_TRUE;}
        if (esc) {count++;}
        if (string_p[i] == 'm' && esc) {esc = NH_FALSE;}
    }

NH_TTY_END(count)
}

static NH_TTY_RESULT Nh_TTY_drawHelp(
    Nh_String *Data_p, int width, int line, int lines, int scroll)
{
NH_TTY_BEGIN()

    int entries = sizeof(help_pp)/sizeof(help_pp[0]);
    int index = line + scroll;

    if (index < entries) 
    {
        int escapes = Nh_TTY_getTotalEscapeLength((NH_BYTE*)help_pp[index]);
        int len = strlen(help_pp[index]) - escapes;

        if (width - len <= 0) {
            Nh_appendToString(Data_p, (NH_BYTE*)help_pp[index], width + escapes);
        } else {
            for (int i = 0; i < (width - len) / 2; ++i) {
                Nh_appendToString(Data_p, " ", 1);
            }
            Nh_appendToString(Data_p, (NH_BYTE*)help_pp[index], strlen(help_pp[index]));
            for (int i = 0; i < (width - len) / 2; ++i) {
                Nh_appendToString(Data_p, " ", 1);
            }

            int drawCount = (width - len) / 2 + (width - len) / 2 + strlen(help_pp[index]);
            drawCount -= escapes;
            if (drawCount < width) {
                for (int i = 0; i < width - drawCount; ++i) {
                    Nh_appendToString(Data_p, " ", 1);
                }
            }
        }
    }
    else {
        for (int i = 0; i < width; ++i) {
            Nh_appendToString(Data_p, " ", 1);
        }
    }

NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS)
}

NH_TTY_RESULT Nh_TTY_drawFileEditorRow(
    Nh_TTY_FileEditor *Editor_p, Nh_String *String_p, int width, int height, int row)
{
NH_TTY_BEGIN()

    if (Editor_p->width != width) {
        Editor_p->width = width;
        Nh_TTY_renderFileEditor(Editor_p);
    }

    if (Editor_p->Files.count == 0) {
        NH_TTY_CHECK(Nh_TTY_drawHelp(String_p, Editor_p->width, row, height, Editor_p->helpScroll))
    }

    for (int i = 0; i < Editor_p->Files.count; ++i) 
    {
        Nh_TTY_File *File_p = Nh_getFromLinkedList(&Editor_p->Files, i);
        NH_TTY_CHECK_NULL(File_p)

        if (row == height - 1) {
            Nh_appendToString(String_p, File_p->BottomBar.bytes_p, strlen(File_p->BottomBar.bytes_p));
            continue;
        }

        switch (File_p->type)
        {
            case NH_TTY_FILE_TEXT :
                Nh_TTY_drawTextFileLine(File_p->handle_p, String_p, row);
                break;
        }
    }

NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS)
}


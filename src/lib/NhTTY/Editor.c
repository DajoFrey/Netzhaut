// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "Editor.h"
#include "TextFile.h"
#include "TreeListing.h"
#include "Program.h"
#include "Terminal.h"
#include "Shell.h"

#include "Common/Macro.h"
#include NH_TTY_FLOW
#include NH_TTY_DEFAULT_CHECK

#include "../NhCore/Memory.h"

#include <stddef.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>

// INIT ============================================================================================

void *Nh_TTY_initEditor()
{
NH_TTY_BEGIN()

#include NH_TTY_CUSTOM_CHECK

    Nh_TTY_Editor *Editor_p = Nh_allocate(sizeof(Nh_TTY_Editor));
    NH_TTY_CHECK_MEM(NULL, Editor_p)

    Editor_p->height      = 0;
    Editor_p->focus       = 0;
    Editor_p->insertMode  = NH_FALSE;
    Editor_p->treeListing = NH_TRUE;
    Editor_p->Output      = Nh_TTY_initOutput();
    Editor_p->TreeListing = Nh_TTY_initTreeListing();
    Editor_p->FileEditor  = Nh_TTY_initFileEditor();

#include NH_TTY_DEFAULT_CHECK

NH_TTY_END(Editor_p)
}

// UPDATE ==========================================================================================

NH_TTY_RESULT Nh_TTY_updateEditor(
    Nh_TTY_Program *Program_p)
{
NH_TTY_BEGIN()

    Nh_TTY_Editor *Editor_p = Program_p->handle_p;

    NH_BOOL refresh = NH_FALSE;
    NH_TTY_CHECK(Nh_TTY_updateTreeListing(&Editor_p->TreeListing, &refresh))

    if (refresh) {
        Program_p->refresh = NH_TRUE;
    }

NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS)
}

// INPUT ===========================================================================================

static NH_TTY_RESULT Nh_TTY_cycleThroughFiles(
    Nh_TTY_Editor *Editor_p, NH_UNICODE_CODEPOINT c)
{
NH_TTY_BEGIN()

    Nh_TTY_FileEditor *FileEditor_p = &Editor_p->FileEditor;

    switch (c)
    {
        case 'f' :
        case CTRL_KEY('f') :

            if (FileEditor_p->current == 0) {
                FileEditor_p->current = FileEditor_p->Files.count - 1;
            } else {
                FileEditor_p->current--;
            }
            break;

        case 'g' :
        case CTRL_KEY('g') :

            if (FileEditor_p->current == FileEditor_p->Files.count - 1) {
                FileEditor_p->current = 0;
            } else {
                FileEditor_p->current++;
            }
            break;
    }

    Nh_TTY_File *File_p = Nh_getFromLinkedList(&FileEditor_p->Files, FileEditor_p->current);

    if (File_p != NULL) {
        NH_TTY_CHECK(Nh_TTY_setTreeListingCursor(&Editor_p->TreeListing, Editor_p->height, File_p))
    }

NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS)
}

NH_TTY_RESULT Nh_TTY_handleEditorInput(
    Nh_TTY_Program *Program_p, NH_UNICODE_CODEPOINT c)
{
NH_TTY_BEGIN()

    Nh_TTY_Editor *Editor_p = Program_p->handle_p;

    switch (c) 
    {
        case 't' : if (Editor_p->insertMode) {goto FILE_EDITOR_INPUT;}
        case CTRL_KEY('t') :

            Editor_p->treeListing = !Editor_p->treeListing; 
            Nh_TTY_setDefaultMessage2(
               Editor_p->treeListing ? NH_TTY_MESSAGE_EDITOR_SHOW_TREE : NH_TTY_MESSAGE_EDITOR_HIDE_TREE 
            );
            Program_p->refresh = NH_TRUE;
            break;

        case 'm' : if (Editor_p->insertMode) {goto FILE_EDITOR_INPUT;}
        case CTRL_KEY('m') :

            Editor_p->insertMode = !Editor_p->insertMode; 
            Nh_TTY_setDefaultMessage2(
                Editor_p->treeListing ? NH_TTY_MESSAGE_EDITOR_INSERT_ACTIVATED : NH_TTY_MESSAGE_EDITOR_INSERT_DEACTIVATED 
            );
            Program_p->refresh = NH_TRUE;
            break;

        case 'n' : if (Editor_p->insertMode) {goto FILE_EDITOR_INPUT;}
        case CTRL_KEY('n') : ;

            Nh_TTY_File *File_p = Nh_TTY_openFile(&Editor_p->FileEditor, NULL, NH_FALSE);
            NH_TTY_CHECK_NULL(File_p)
            NH_TTY_CHECK(Nh_TTY_insertNewFile(&Editor_p->TreeListing, File_p))
            Program_p->refresh = NH_TRUE;
            break;

        case 'f' :
        case 'g' : if (Editor_p->insertMode) {goto FILE_EDITOR_INPUT;}
        case CTRL_KEY('f') : 
        case CTRL_KEY('g') :

            NH_TTY_CHECK(Nh_TTY_cycleThroughFiles(Editor_p, c))
            Program_p->refresh = NH_TRUE;
            break;

        case 'w' :
        case 'a' :
        case 's' :
        case 'd' : if (Editor_p->insertMode) {goto FILE_EDITOR_INPUT;}

        case CTRL_KEY('w') :
        case CTRL_KEY('a') :
        case CTRL_KEY('s') :
        case CTRL_KEY('d') :

            NH_TTY_CHECK(Nh_TTY_handleTreeListingInput(Program_p, Editor_p->height, c))
            Program_p->refresh = NH_TRUE;
            break;

        default :
            goto FILE_EDITOR_INPUT;
    }
 
NH_TTY_END(NH_TTY_SUCCESS)

FILE_EDITOR_INPUT :
    NH_TTY_CHECK(Nh_TTY_handleFileEditorInput(Program_p, Editor_p->height, c))

NH_TTY_END(NH_TTY_SUCCESS)
}

// DRAW ============================================================================================

NH_TTY_RESULT Nh_TTY_drawEditorRow(
    Nh_TTY_Program *Program_p, Nh_UTF8String *Row_p, int width, int height, int row)
{
NH_TTY_BEGIN()

    Nh_TTY_Editor *Editor_p = Program_p->handle_p;
    Editor_p->height = height;

    if (Editor_p->treeListing) {
        Nh_TTY_drawTreeListingRow(&Editor_p->TreeListing, Row_p, Nh_TTY_getTileColorFromProgHandle(Editor_p), row);
    }
    
    Nh_TTY_drawFileEditorRow(&Editor_p->FileEditor, Row_p, width - Editor_p->TreeListing.width, height, row);

NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS)
}

// GET =============================================================================================

NH_TTY_RESULT Nh_TTY_getEditorCursor(
    Nh_TTY_Program *Program_p, int *x_p, int *y_p)
{
NH_TTY_BEGIN()

    Nh_TTY_Editor *Editor_p = Program_p->handle_p;
    int x = Editor_p->treeListing ? Editor_p->TreeListing.width : 0;

    Nh_TTY_File *File_p = NULL;
    for (int i = 0; i < Editor_p->FileEditor.Files.count; ++i) 
    {
        File_p = Nh_getFromLinkedList(&Editor_p->FileEditor.Files, i);
        if (i == Editor_p->FileEditor.current) {break;}
        x += Editor_p->FileEditor.width / Editor_p->FileEditor.Files.count;
    }

    if (File_p != NULL && File_p->type == NH_TTY_FILE_TEXT) 
    {
        Nh_TTY_TextFile *TextFile_p = File_p->handle_p;
        *x_p = TextFile_p->screenCursorX + TextFile_p->lineNumberOffset + x;
        *y_p = TextFile_p->screenCursorY + 1;
    }
    else {
        *x_p = -1;
        *y_p = -1;
    }

NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS)
}

// COMMANDS ========================================================================================

NH_TTY_RESULT Nh_TTY_executeEditorCommand(
    Nh_TTY_Program *Program_p, NH_UNICODE_CODEPOINT codepoint)
{
NH_TTY_BEGIN()

    Nh_TTY_Editor *Editor_p = Program_p->handle_p;

    Nh_SystemTime SystemTime = Nh_getSystemTime();
    NH_BYTE logName_p[255] = {'\0'};
    sprintf(logName_p, "%lu%lu", SystemTime.seconds, SystemTime.milliseconds);

    if (codepoint == 'e') {
        Nh_TTY_File *File_p = Nh_getFromLinkedList(&Editor_p->FileEditor.Files, Editor_p->FileEditor.current);
        Nh_TTY_executeTextFile(File_p->handle_p);
    }
    else if (codepoint == 'w') {
        Nh_TTY_File *File_p = Nh_getFromLinkedList(&Editor_p->FileEditor.Files, Editor_p->FileEditor.current);
        if (File_p != NULL) {
            NH_TTY_CHECK(Nh_TTY_parseWebTextFile(logName_p, File_p->handle_p))
        }
    }
    else if (codepoint == 'h') {
        Nh_TTY_File *File_p = Nh_getFromLinkedList(&Editor_p->FileEditor.Files, Editor_p->FileEditor.current);
        if (File_p != NULL) {
            NH_TTY_CHECK(Nh_TTY_parseHTML(logName_p, File_p->handle_p))
        }
    }

NH_TTY_DIAGNOSTIC_END(NH_TTY_ERROR_UNKNOWN_COMMAND)
}


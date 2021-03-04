// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "TreeListing.h"
#include "Editor.h"
#include "Program.h"
#include "Row.h"
#include "Terminal.h"
#include "Shell.h"

#include "Common/Macro.h"
#include NH_TTY_FLOW
#include NH_TTY_DEFAULT_CHECK

#include "../NhCore/Memory.h"
#include "../NhCore/Common/Macro.h"
#include NH_FLOW
#include NH_CUSTOM_CHECK

#include <stddef.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>

// DECLARE =========================================================================================

static NH_TTY_RESULT Nh_TTY_renderTreeListing(
    Nh_TTY_TreeListing *Listing_p
);

// NODE ============================================================================================

static int Nh_TTY_isRegularFile(
    const NH_BYTE *path)
{
    struct stat path_stat;
    stat(path, &path_stat);
    return S_ISREG(path_stat.st_mode);
}

static Nh_TTY_TreeListingNode *Nh_TTY_createTreeListingNode(
    Nh_TTY_TreeListingNode *Parent_p)
{
NH_TTY_BEGIN()

#include NH_TTY_CUSTOM_CHECK

    Nh_TTY_TreeListingNode *Node_p = Nh_allocate(sizeof(Nh_TTY_TreeListingNode));
    NH_TTY_CHECK_MEM(NULL, Node_p)

#include NH_TTY_DEFAULT_CHECK

    Node_p->path_p   = NULL;
    Node_p->Children = Nh_initList(16);
    Node_p->Parent_p = Parent_p;
    Node_p->open     = NH_FALSE;
    Node_p->File_p   = NULL;

NH_TTY_END(Node_p)
}

static void Nh_TTY_getNodeList(
    Nh_List *List_p, Nh_TTY_TreeListingNode *Node_p)
{
NH_TTY_BEGIN()

    Nh_appendToList(List_p, Node_p);

    if (!Node_p->open) {NH_TTY_SILENT_END()}

    for (int i = 0; i < Node_p->Children.size; ++i) {
        Nh_TTY_getNodeList(List_p, Node_p->Children.pp[i]);
    }

NH_TTY_SILENT_END()
}

Nh_TTY_TreeListingNode *Nh_TTY_getCurrentNode(
    Nh_TTY_TreeListing *Listing_p)
{
NH_TTY_BEGIN()

    int current = 0; 
    Nh_List Nodes = Nh_initList(128);
    Nh_TTY_getNodeList(&Nodes, Listing_p->Root_p);

    Nh_TTY_TreeListingNode *Current_p = Nodes.pp[Listing_p->treeCurrent];

    Nh_freeList(&Nodes, NH_FALSE);

NH_TTY_END(Current_p)
}

static void Nh_TTY_closeNode(
    Nh_TTY_TreeListingNode *Node_p)
{
NH_TTY_BEGIN()

    for (int i = 0; i < Node_p->Children.size; ++i) {
        Nh_TTY_TreeListingNode *Child_p = Node_p->Children.pp[i];
        free(Child_p->path_p);
        Nh_TTY_closeNode(Child_p);
    }

    Nh_freeList(&Node_p->Children, NH_TRUE);

NH_TTY_SILENT_END()
}

static NH_TTY_RESULT Nh_TTY_openNode(
    Nh_TTY_TreeListingNode *Node_p)
{
NH_TTY_BEGIN()

    NH_TTY_CHECK_NULL(Node_p)
    NH_TTY_CHECK_NULL(Node_p->path_p)

    if (Node_p->Children.size > 0 || Nh_TTY_isRegularFile(Node_p->path_p)) {
        NH_TTY_DIAGNOSTIC_END(NH_TTY_ERROR_BAD_STATE)
    }

#ifdef __unix__ 

    NH_BYTE newPath_p[1024] = {'\0'};
    struct dirent **namelist_pp;
    int n;

    n = scandir(Node_p->path_p, &namelist_pp, 0, alphasort);
    if (n < 0) {NH_TTY_DIAGNOSTIC_END(NH_TTY_ERROR_BAD_STATE)}

    for (int i = 0; i < n; ++i) 
    {
        if (Node_p->path_p[strlen(Node_p->path_p) - 1] != '/') {
            sprintf(newPath_p, "%s/%s", Node_p->path_p, namelist_pp[i]->d_name);
        } else {
            sprintf(newPath_p, "%s%s", Node_p->path_p, namelist_pp[i]->d_name);
        }
        if (strcmp(namelist_pp[i]->d_name, ".") && strcmp(namelist_pp[i]->d_name, "..")) 
        {
            Nh_TTY_TreeListingNode *New_p = Nh_TTY_createTreeListingNode(Node_p);
            NH_TTY_CHECK_MEM(New_p)
            New_p->path_p = Nh_allocateBytes(newPath_p);
            NH_TTY_CHECK_NULL(New_p->path_p)
            NH_CHECK(NH_TTY_ERROR_BAD_STATE, Nh_appendToList(&Node_p->Children, New_p))
        }

        memset(newPath_p, 0, 1024);
        free(namelist_pp[i]);
    }
    free(namelist_pp);

#elif defined(_WIN32) || defined(WIN32)

    printf("microsoft windows not supported\n");
    exit(0);

#endif

    Node_p->open = NH_TRUE;

NH_TTY_END(NH_TTY_SUCCESS)
}

NH_TTY_RESULT Nh_TTY_insertNewFile(
    Nh_TTY_TreeListing *Listing_p, Nh_TTY_File *File_p)
{
NH_TTY_BEGIN()

    Nh_TTY_TreeListingNode *Current_p = Nh_TTY_getCurrentNode(Listing_p);

    if (Current_p->Children.size > 0) {
        Nh_TTY_TreeListingNode *New_p = Nh_TTY_createTreeListingNode(Current_p);
        NH_TTY_CHECK_MEM(New_p)
        New_p->File_p = File_p;
        NH_CHECK(NH_TTY_ERROR_BAD_STATE, Nh_appendToList(&Current_p->Children, New_p))
    }
    else if (Current_p->Parent_p != NULL) {
        Nh_TTY_TreeListingNode *New_p = Nh_TTY_createTreeListingNode(Current_p->Parent_p);
        NH_TTY_CHECK_MEM(New_p)
        New_p->File_p = File_p;
        NH_CHECK(NH_TTY_ERROR_BAD_STATE, Nh_appendToList(&Current_p->Parent_p->Children, New_p))
    }
    else {NH_TTY_DIAGNOSTIC_END(NH_TTY_ERROR_BAD_STATE)}

    Nh_TTY_renderTreeListing(Listing_p);

NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS)
}

// INIT ============================================================================================

Nh_TTY_TreeListing Nh_TTY_initTreeListing()
{
NH_TTY_BEGIN()

    Nh_TTY_TreeListing Listing;

    Listing.Root_p        = Nh_TTY_createTreeListingNode(NULL);
    Listing.treeCurrent   = 0;
    Listing.rowOffset     = 0;
    Listing.RenderLines   = Nh_initArray(sizeof(Nh_String), 255);
    Listing.width         = 0;
    Listing.screenCurrent = 0;
    Listing.preview       = NH_TRUE;
    Listing.Preview_p     = NULL;
    Listing.LastUpdate    = Nh_getSystemTime();
    Listing.updateIntervalInSeconds = 1.0;

    NH_BYTE wrkDir_p[2048];
    memset(wrkDir_p, 0, 2048);
    getcwd(wrkDir_p, 2048);

    if (wrkDir_p != NULL) {
        Listing.Root_p->path_p = Nh_allocateBytes(wrkDir_p);
        Nh_TTY_openNode(Listing.Root_p); 
        Nh_TTY_renderTreeListing(&Listing);
    }

NH_TTY_END(Listing)
}

// UPDATE ==========================================================================================

static NH_TTY_RESULT Nh_TTY_updateTreeListingNode(
    Nh_TTY_TreeListingNode *Node_p, NH_BOOL *updated_p)
{
NH_TTY_BEGIN()

#ifdef __unix__

    NH_BYTE absolutePath_p[1024] = {'\0'};
    struct dirent **namelist_pp;
    int n;

    n = scandir(Node_p->path_p, &namelist_pp, 0, alphasort);
    if (n < 0) {NH_TTY_DIAGNOSTIC_END(NH_TTY_ERROR_BAD_STATE)}

    for (int i = 0, j = 0; i < n; ++i)
    {
        if (Node_p->path_p[strlen(Node_p->path_p) - 1] != '/') {
            sprintf(absolutePath_p, "%s/%s", Node_p->path_p, namelist_pp[i]->d_name);
        } else {
            sprintf(absolutePath_p, "%s%s", Node_p->path_p, namelist_pp[i]->d_name);
        }

        if (strcmp(namelist_pp[i]->d_name, ".") && strcmp(namelist_pp[i]->d_name, "..")) 
        {
            Nh_TTY_TreeListingNode *Child_p = Nh_getFromList(&Node_p->Children, j);
            if (Child_p == NULL || strcmp(Child_p->path_p, absolutePath_p)) {
                Nh_TTY_TreeListingNode *New_p = Nh_TTY_createTreeListingNode(Node_p);
                NH_TTY_CHECK_MEM(New_p)
                New_p->path_p = Nh_allocateBytes(absolutePath_p);
                NH_TTY_CHECK_NULL(New_p->path_p)
                NH_CHECK(NH_TTY_ERROR_BAD_STATE, Nh_insertIntoList(&Node_p->Children, New_p, j))
                *updated_p = NH_TRUE;
            }
            if (Child_p->Children.size > 0) {
                NH_TTY_CHECK(Nh_TTY_updateTreeListingNode(Child_p, updated_p))
            }
            j++;
        }

        memset(absolutePath_p, 0, 1024);
        free(namelist_pp[i]);
    }

    free(namelist_pp);

#elif defined(_WIN32) || defined(WIN32)

    printf("microsoft windows not supported\n");
    exit(0);

#endif

NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS)
}

NH_TTY_RESULT Nh_TTY_updateTreeListing(
    Nh_TTY_TreeListing *Listing_p, NH_BOOL *refresh_p)
{
NH_TTY_BEGIN()

    Nh_SystemTime Now = Nh_getSystemTime();
    if (Nh_getSystemTimeDiffInSeconds(Listing_p->LastUpdate, Now) < Listing_p->updateIntervalInSeconds) {
        NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS)
    }

    NH_BOOL updated = NH_FALSE;
    NH_TTY_CHECK(Nh_TTY_updateTreeListingNode(Listing_p->Root_p, &updated))

    if (updated) {
        NH_TTY_CHECK(Nh_TTY_renderTreeListing(Listing_p))
        *refresh_p = NH_TRUE;
    }

    Listing_p->LastUpdate = Now;

NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS)
}

// INPUT ===========================================================================================

static NH_TTY_RESULT Nh_TTY_delete(
    Nh_TTY_Console *Console_p, NH_UNICODE_CODEPOINT input, NH_BOOL *continue_p)
{
NH_TTY_BEGIN()

    if (input == 'y' || input == 'n') 
    {
        Console_p->block  = NH_FALSE;
        Console_p->args_p = NULL;
        Console_p->callback_f = NULL;
    
        if (input == 'n') {
            NH_TTY_CHECK(Nh_TTY_setDefaultMessage2(NH_TTY_MESSAGE_BINARY_QUERY_DELETE_INTERRUPTED))
        }
        if (input == 'y') {
//            NH_TTY_CHECK(Nh_TTY_setDefaultMessage2(NH_TTY_MESSAGE_ERROR_NOT_IMPLEMENTED))
        }
    }

NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS)
}

static NH_TTY_RESULT Nh_TTY_setCurrentToRoot(
    Nh_TTY_TreeListing *Listing_p, Nh_Array *Programs_p)
{
NH_TTY_BEGIN()

    Nh_TTY_TreeListingNode *Current_p = Nh_TTY_getCurrentNode(Listing_p);

    if (Current_p->Children.size > 0)
    {
        for (Nh_TTY_TreeListingNode *Parent_p = Current_p; Parent_p = Parent_p->Parent_p;) {
            Parent_p->open = NH_FALSE;
        }
        Listing_p->Root_p = Current_p;
    }

//    // sync shell
//    NH_BYTE cmd_p[1024] = {'\0'};
//    sprintf(cmd_p, "cd %s\r", Current_p->path_p);
//    Nh_TTY_writeToShell(Nh_TTY_getProgram(Programs_p, NH_TTY_SHELL_NAME)->handle_p, cmd_p, strlen(cmd_p), 0);

NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS)
}

static NH_TTY_RESULT Nh_TTY_setParentToRoot(
    Nh_TTY_TreeListing *Listing_p, Nh_Array *Programs_p)
{
NH_TTY_BEGIN()

    if (Listing_p->Root_p->Parent_p != NULL) {
        Listing_p->Root_p = Listing_p->Root_p->Parent_p;
        Listing_p->Root_p->open = NH_TRUE;
    }
    else {
        NH_BYTE path_p[2048] = {'\0'};
        strcpy(path_p, Listing_p->Root_p->path_p); 
        for (int i = strlen(path_p) - 1; path_p[i] != '/'; --i) {path_p[i] = '\0';}
        path_p[strlen(path_p) == 1 ? 1 : strlen(path_p) - 1] = '\0';

        Nh_TTY_TreeListingNode *OldRoot_p = Listing_p->Root_p;

        Listing_p->Root_p = Nh_TTY_createTreeListingNode(NULL);
        NH_TTY_CHECK_MEM(Listing_p->Root_p)
        Listing_p->Root_p->path_p = Nh_allocateBytes(path_p);
        NH_TTY_CHECK_MEM(Listing_p->Root_p->path_p)

        OldRoot_p->Parent_p = Listing_p->Root_p;
        Nh_TTY_openNode(Listing_p->Root_p);

        NH_BOOL isChild = NH_FALSE;
        for (int i = 0; i < Listing_p->Root_p->Children.size; ++i) {
            if (!strcmp(((Nh_TTY_TreeListingNode*)Listing_p->Root_p->Children.pp[i])->path_p, OldRoot_p->path_p)) {
                Nh_free(((Nh_TTY_TreeListingNode*)Listing_p->Root_p->Children.pp[i])->path_p);
                Nh_free(Listing_p->Root_p->Children.pp[i]);
                Listing_p->Root_p->Children.pp[i] = OldRoot_p;
                isChild = NH_TRUE;
                break;
            }
        }

        if (!isChild) {NH_TTY_DIAGNOSTIC_END(NH_TTY_ERROR_BAD_STATE)}
    }

//    // sync shell
//    NH_BYTE cmd_p[1024] = {'\0'};
//    sprintf(cmd_p, "cd %s\r", Listing_p->Root_p->path_p);
//    Nh_TTY_writeToShell(Nh_TTY_getProgram(Programs_p, NH_TTY_SHELL_NAME)->handle_p, cmd_p, strlen(cmd_p), 0);

NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS)
}

static void Nh_TTY_moveCursorVertically(
    Nh_TTY_FileEditor *FileEditor_p, Nh_TTY_TreeListing *Listing_p, int rows, int key) 
{
NH_TTY_BEGIN()

    Nh_List Nodes = Nh_initList(32);
    Nh_TTY_getNodeList(&Nodes, Listing_p->Root_p);

    switch (key) 
    {
        case 'w' :
        case CTRL_KEY('w') :

            if (Listing_p->treeCurrent > 0) {
                Listing_p->treeCurrent--;
                if (Listing_p->screenCurrent > 0) {Listing_p->screenCurrent--;}
                else if (Listing_p->rowOffset > 0) {Listing_p->rowOffset--;} 
            }
            break;

        case 's' :
        case CTRL_KEY('s') :

            if (Listing_p->treeCurrent < Nodes.size - 1) { 
                Listing_p->treeCurrent++;
                if (Listing_p->screenCurrent < rows - 1) {Listing_p->screenCurrent++;}
                else {Listing_p->rowOffset++;}
            }
            break;
    }

    Nh_freeList(&Nodes, NH_FALSE);

    if (Listing_p->preview && FileEditor_p != NULL)
    {
        if (Listing_p->Preview_p != NULL) {
            Nh_TTY_closeFile(FileEditor_p, Listing_p->Preview_p->File_p);   
            Listing_p->Preview_p->File_p = NULL;
            Listing_p->Preview_p = NULL;
        }

        Nh_TTY_TreeListingNode *Current_p = Nh_TTY_getCurrentNode(Listing_p);

        if (Current_p->File_p == NULL && Current_p->path_p != NULL && Nh_TTY_isRegularFile(Current_p->path_p)) {
            Current_p->File_p = Nh_TTY_openFile(FileEditor_p, Current_p->path_p, NH_TRUE);
            Listing_p->Preview_p = Current_p; 
        }
    }

NH_TTY_SILENT_END()
}

NH_TTY_RESULT Nh_TTY_handleTreeListingInput(
    Nh_TTY_Program *Program_p, int rows, NH_UNICODE_CODEPOINT c)
{
NH_TTY_BEGIN()

    Nh_TTY_Editor *Editor_p = Program_p->handle_p;
    Nh_TTY_FileEditor *FileEditor_p = &Editor_p->FileEditor;
    Nh_TTY_TreeListing *Listing_p = &Editor_p->TreeListing;
    Nh_TTY_TreeListingNode *Current_p = Nh_TTY_getCurrentNode(Listing_p);

    switch (c)
    {
        case 'w' :
        case 's' :
        case CTRL_KEY('w') :
        case CTRL_KEY('s') :

            Nh_TTY_moveCursorVertically(FileEditor_p, Listing_p, rows, c);
            break;

        case 'a' :
        case CTRL_KEY('a') :

            if (Listing_p->treeCurrent == 0) {
                Nh_TTY_setParentToRoot(Listing_p, Program_p->Programs_p);
            } 
            else if (Current_p->Children.size > 0 && Current_p->open) {
                Current_p->open = NH_FALSE;
            }
            else if (Current_p->Children.size == 0 && Current_p->File_p != NULL && Current_p->path_p != NULL) {
                NH_TTY_CHECK(Nh_TTY_closeFile(FileEditor_p, Current_p->File_p))   
                Current_p->File_p = NULL;
            }
            else if (Current_p->Children.size == 0 && Current_p->File_p != NULL) {
                NH_TTY_CHECK(Nh_TTY_closeFile(FileEditor_p, Current_p->File_p))
                Nh_removeFromList2(&Current_p->Parent_p->Children, NH_TRUE, Current_p);
                NH_TTY_CHECK(Nh_TTY_handleTreeListingInput(Program_p, rows, 'w'))
            }
            else { // delete ?
                Nh_UnicodeString Question = Nh_initUnicodeString(128);
                int deleteLength;
                NH_UNICODE_CODEPOINT *delete_p = Nh_TTY_getMessage(NH_TTY_MESSAGE_BINARY_QUERY_DELETE, &deleteLength);
                Nh_UnicodeString File = Nh_decodeUTF8Text(Current_p->path_p);
                NH_CHECK(NH_TTY_ERROR_BAD_STATE, Nh_appendToUnicodeString(&Question, delete_p, deleteLength))
                NH_CHECK(NH_TTY_ERROR_BAD_STATE, Nh_appendToUnicodeString(&Question, (NH_UNICODE_CODEPOINT*)File.bytes_p, File.length))
                NH_TTY_CHECK(Nh_TTY_setBinaryQueryMessage2((NH_UNICODE_CODEPOINT*)Question.bytes_p, Question.length, NULL, Nh_TTY_delete))
                Nh_freeUnicodeString(&Question);
                Nh_freeUnicodeString(&File);
            }
            break;

        case 'd' :
        case CTRL_KEY('d') :

            if (Listing_p->Preview_p != NULL) {
                Listing_p->Preview_p->File_p->readOnly = NH_FALSE;
                Listing_p->Preview_p = NULL;
            }
            else if (Current_p->Children.size > 0 && !Current_p->open) {
                Current_p->open = NH_TRUE;
            }
            else if (Current_p->Children.size == 0 && !Nh_TTY_isRegularFile(Current_p->path_p)) {
                NH_TTY_CHECK(Nh_TTY_openNode(Current_p))
            }
            else if (Current_p->Children.size == 0 && Nh_TTY_isRegularFile(Current_p->path_p)) {
                if (Current_p->File_p == NULL) {
                    Current_p->File_p = Nh_TTY_openFile(FileEditor_p, Current_p->path_p, NH_FALSE);
                }
                else {
                    // TODO
                }
            } else {
                Nh_TTY_setCurrentToRoot(Listing_p, Program_p->Programs_p);
                Listing_p->treeCurrent = 0;
                Listing_p->screenCurrent = 0;
                Listing_p->rowOffset = 0;
            }

            break;
    }
 
    NH_TTY_CHECK(Nh_TTY_renderTreeListing(Listing_p))

NH_TTY_END(NH_TTY_SUCCESS)
}

// RENDER ==========================================================================================

static NH_TTY_RESULT Nh_TTY_renderTreeListingNode(
    Nh_TTY_TreeListingNode *Node_p, NH_BYTE *row_p)
{
NH_TTY_BEGIN()

    NH_BYTE *p = NULL;
    int offset = 0;
    Nh_TTY_TreeListingNode *Parent_p = Node_p->Parent_p;
    while (Parent_p != NULL && Parent_p->open) {
        offset += 2; 
        Parent_p = Parent_p->Parent_p;
    }
    for (int i = 0; i < offset; i++) {row_p[i] = ' ';}

    if (Node_p->path_p != NULL)
    {
        for (int i = 0; i < strlen(Node_p->path_p); ++i) {
            if (Node_p->path_p[i] == '/') {p = &Node_p->path_p[i];}
        }
        p = p + 1;
        sprintf(row_p + offset, p, strlen(p));
        if (!Nh_TTY_isRegularFile(Node_p->path_p)) {
            row_p[strlen(row_p)] = '/'; 
        }
    }
    else {
        sprintf(row_p + offset, "New File", 8);
    }

NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS)
}

static NH_TTY_RESULT Nh_TTY_renderTreeListingRow(
    Nh_List *Nodes_p, Nh_TTY_TreeListingNode *Current_p, Nh_String *Row_p, int row, int cols)
{
NH_TTY_BEGIN()

    Nh_TTY_TreeListingNode *Node_p = Nh_getFromList(Nodes_p, row);

    if (Node_p != NULL) 
    {
        NH_BYTE row_p[1024] = {'\0'};
        NH_TTY_CHECK(Nh_TTY_renderTreeListingNode(Node_p, row_p))

        if (Current_p == Node_p) {
            Nh_appendToString(Row_p, "\e[0m\e[1;7m", 10);
        }
        else if (Current_p == Node_p && Node_p->File_p != NULL && !Node_p->File_p->readOnly) {
            Nh_appendToString(Row_p, "\e[0m\e[1;7m\e[1;5m", 16);
        }
        else if (Node_p->File_p != NULL && !Node_p->File_p->readOnly) {
            Nh_appendToString(Row_p, "\e[1;5m", 6);
        }
        Nh_appendFormatToString(Row_p, row_p);

        for (int i = 0; i < cols - strlen(row_p); ++i) {
            Nh_appendToString(Row_p, " ", 1);
        }
    }

NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS)
}

static int Nh_TTY_getTreeListingWidth(
    Nh_TTY_TreeListing *Listing_p)
{
NH_TTY_BEGIN()

    Nh_List Nodes = Nh_initList(64);
    Nh_TTY_getNodeList(&Nodes, Listing_p->Root_p);

    int width = 0;
    NH_BYTE row_p[1024] = {'\0'};

    for (int i = 0; i < Nodes.size; ++i) {
        NH_TTY_CHECK(Nh_TTY_renderTreeListingNode(Nodes.pp[i], row_p))
        if (strlen(row_p) > width) {width = strlen(row_p);}
        memset(row_p, 0, 1024);
    }

    Nh_freeList(&Nodes, NH_FALSE);

NH_TTY_END(width)
}

static NH_TTY_RESULT Nh_TTY_renderTreeListing(
    Nh_TTY_TreeListing *Listing_p)
{
NH_TTY_BEGIN()

    Nh_List Nodes = Nh_initList(32);
    Nh_TTY_getNodeList(&Nodes, Listing_p->Root_p);
    Nh_TTY_TreeListingNode *Current_p = Nh_TTY_getCurrentNode(Listing_p);

    for (int i = 0; i < Listing_p->RenderLines.length; ++i) {
        Nh_freeString(&((Nh_String*)Listing_p->RenderLines.bytes_p)[i]);
    }
    Nh_freeArray(&Listing_p->RenderLines);
    Listing_p->RenderLines = Nh_initArray(sizeof(Nh_String), Nodes.size);

    Listing_p->width = Nh_TTY_getTreeListingWidth(Listing_p);

    for (int row = 0; row < Nodes.size; ++row) {
        Nh_String *Line_p = Nh_getFromArray(&Listing_p->RenderLines, -1);
        *Line_p = Nh_initString(128);
        NH_TTY_CHECK(Nh_TTY_renderTreeListingRow(
            &Nodes, Current_p, Line_p, row, Listing_p->width
        ))
    }

    Nh_freeList(&Nodes, NH_FALSE);

NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS)
}

// DRAW ============================================================================================

NH_TTY_RESULT Nh_TTY_drawTreeListingRow(
    Nh_TTY_TreeListing *Listing_p, Nh_String *Row_p, NH_TTY_TILE_COLOR color, int row)
{
NH_TTY_BEGIN()

    row += Listing_p->rowOffset; 
    Nh_appendToString(Row_p, NH_TTY_INVERSE_TILE_COLORS_PP[color], 7);

    if (row < Listing_p->RenderLines.length) {
        Nh_String *RenderLine_p = &((Nh_String*)Listing_p->RenderLines.bytes_p)[row];
        Nh_appendToString(Row_p, RenderLine_p->bytes_p, RenderLine_p->length);
    }
    else {
        NH_BYTE whitespace_p[1024];
        memset(whitespace_p, 32, 1024);
        Nh_appendToString(Row_p, whitespace_p, Listing_p->width);
    }

    Nh_appendToString(Row_p, "\e[0m", 4);

NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS)
}

// GET/SET ==========================================================================================

NH_TTY_RESULT Nh_TTY_setTreeListingCursor(
    Nh_TTY_TreeListing *Listing_p, int rows, Nh_TTY_File *File_p)
{
NH_TTY_BEGIN()

    Nh_List Nodes = Nh_initList(16);
    Nh_TTY_getNodeList(&Nodes, Listing_p->Root_p);

    for (int i = 0; i < Nodes.size; ++i) {
        if (((Nh_TTY_TreeListingNode*)Nodes.pp[i])->File_p == File_p) {
            if (Listing_p->treeCurrent < i) {
                int diff = i - Listing_p->treeCurrent;
                while (diff-- > 0) {
                    Nh_TTY_moveCursorVertically(NULL, Listing_p, rows, CTRL_KEY('s'));
                }
            }
            else if (Listing_p->treeCurrent > i) {
                int diff = Listing_p->treeCurrent - i;
                while (diff-- > 0) {
                    Nh_TTY_moveCursorVertically(NULL, Listing_p, rows, CTRL_KEY('w'));
                }
            }
            break;
        }
    }

    Nh_freeList(&Nodes, NH_FALSE);
    NH_TTY_CHECK(Nh_TTY_renderTreeListing(Listing_p))

NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS)
}


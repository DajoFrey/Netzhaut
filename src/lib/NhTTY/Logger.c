// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "Logger.h"
#include "Editor.h"
#include "Program.h"
#include "Row.h"
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

// UPDATE LOGGER ===================================================================================

static NH_TTY_RESULT Nh_TTY_updateLoggerNode(
    Nh_LoggerNode *LoggerNode_p, Nh_TTY_LoggerNode *InstanceNode_p)
{
NH_TTY_BEGIN()

    for (int i = 0; i < LoggerNode_p->Children.size; ++i) 
    {
        Nh_LoggerNode *NextLoggerNode_p = LoggerNode_p->Children.handles_pp[i];
        Nh_TTY_LoggerNode *NextInstanceNode_p = Nh_getFromList(&InstanceNode_p->Children, i);

        if (NextInstanceNode_p == NULL) 
        {
            NextInstanceNode_p = Nh_allocate(sizeof(Nh_TTY_LoggerNode));
            NH_TTY_CHECK_MEM(NextInstanceNode_p)

            NextInstanceNode_p->Children     = Nh_initList(8);
            NextInstanceNode_p->LoggerNode_p = NextLoggerNode_p;
            NextInstanceNode_p->isOpen       = NH_FALSE;
            NextInstanceNode_p->isCurrent    = NH_FALSE;
            NextInstanceNode_p->isSelected   = NH_FALSE;
            NextInstanceNode_p->hasFocus     = NH_FALSE;
            NextInstanceNode_p->focusedRow   = 0;
            NextInstanceNode_p->offset       = 0;
            NextInstanceNode_p->peak         = 0;
            NextInstanceNode_p->Parent_p     = InstanceNode_p;

            Nh_appendToList(&InstanceNode_p->Children, NextInstanceNode_p);
        }

        NH_TTY_CHECK(Nh_TTY_updateLoggerNode(NextLoggerNode_p, NextInstanceNode_p))
    }

NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS)
}

static void Nh_TTY_getLoggerNodes(
    Nh_TTY_LoggerNode *Node_p, Nh_List *List_p)
{
NH_TTY_BEGIN()

    if (!Node_p->isOpen) {NH_TTY_SILENT_END()}

    for (int i = 0; i < Node_p->Children.size; ++i) 
    {
        Nh_appendToList(List_p, Node_p->Children.handles_pp[i]);
        Nh_TTY_getLoggerNodes(Node_p->Children.handles_pp[i], List_p);
    }

NH_TTY_SILENT_END()
}

static int Nh_TTY_getLoggerNodeDepth(
    Nh_TTY_LoggerNode *Node_p)
{
NH_TTY_BEGIN()

    int depth = 0;
    Nh_TTY_LoggerNode *Parent_p = Node_p->Parent_p;
    while ((Parent_p = Parent_p->Parent_p) != NULL) {
        depth++;
    }

NH_TTY_END(depth)
}

static int Nh_TTY_getCategoryListingWidth(
    Nh_TTY_Logger *Logger_p)
{
NH_TTY_BEGIN()

    Nh_List Nodes = Nh_initList(16);
    Nh_TTY_getLoggerNodes(&Logger_p->Root, &Nodes);

    int maxWidth = 0;
    Nh_TTY_LoggerNode *Node_p = NULL;
    for (int i = 0; (Node_p = Nh_getFromList(&Nodes, i)) != NULL; ++i) {
        int width = (Nh_TTY_getLoggerNodeDepth(Node_p) * 2) + strlen(Node_p->LoggerNode_p->category_p);
        if (width > maxWidth) {
            maxWidth = width;
        }
    }

    Nh_freeList(&Nodes, NH_FALSE);

NH_TTY_END(maxWidth)
}

NH_TTY_RESULT Nh_TTY_updateLogger(
    Nh_TTY_Program *Program_p)
{
NH_TTY_BEGIN()

    Nh_TTY_Logger *Logger_p = Program_p->handle_p;

    Nh_SystemTime Now = Nh_getSystemTime();
    if (Nh_getSystemTimeDiffInSeconds(Logger_p->LastUpdate, Now) < Logger_p->updateIntervalInSeconds) {
        NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS)
    }

    NH_BOOL init = Logger_p->Root.Children.size == 0;

    Nh_TTY_updateLoggerNode(&NH_LOGGER.Root, &Logger_p->Root);

    if (init && Logger_p->Root.Children.size > 0) {
        ((Nh_TTY_LoggerNode*)Logger_p->Root.Children.handles_pp[0])->isCurrent = NH_TRUE;
    }

    Logger_p->listingWidth = Nh_TTY_getCategoryListingWidth(Logger_p);
    Logger_p->LastUpdate = Now;

    Program_p->refresh = NH_TRUE;

NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS)
}

// INIT ============================================================================================

void *Nh_TTY_initLogger()
{
NH_TTY_BEGIN()

#include NH_TTY_CUSTOM_CHECK

    Nh_TTY_Logger *Logger_p = Nh_allocate(sizeof(Nh_TTY_Logger));
    NH_TTY_CHECK_MEM(NULL, Logger_p)

    Logger_p->showCategories = NH_TRUE;
    Logger_p->listingWidth   = 0;
    Logger_p->height         = 0;
    Logger_p->LastUpdate     = Nh_getSystemTime();
    Logger_p->updateIntervalInSeconds = 1.0;

    Logger_p->Root.LoggerNode_p = &NH_LOGGER.Root;
    Logger_p->Root.isOpen       = NH_TRUE;
    Logger_p->Root.isCurrent    = NH_FALSE;
    Logger_p->Root.Children     = Nh_initList(4);
    Logger_p->Root.Parent_p     = NULL;

#include NH_TTY_DEFAULT_CHECK

NH_TTY_END(Logger_p)
}

// INPUT ===========================================================================================

static Nh_List Nh_TTY_getSelectedLoggerNodes(
    Nh_TTY_Logger *Logger_p)
{
NH_TTY_BEGIN()

    Nh_List SelectedNodes = Nh_initList(8);
    Nh_List Nodes = Nh_initList(32);
    Nh_TTY_getLoggerNodes(&Logger_p->Root, &Nodes);

    for (int i = 0; i < Nodes.size; ++i) {
        if (((Nh_TTY_LoggerNode*)Nodes.handles_pp[i])->isSelected) {
            Nh_appendToList(&SelectedNodes, Nodes.handles_pp[i]);
        }
    }

    Nh_freeList(&Nodes, NH_FALSE);

NH_TTY_END(SelectedNodes)
}

static void Nh_TTY_resetLoggerFocuses(
    Nh_TTY_Logger *Logger_p)
{
NH_TTY_BEGIN()

    Nh_List SelectedNodes = Nh_TTY_getSelectedLoggerNodes(Logger_p);
    for (int i = 0; i < SelectedNodes.size; ++i) {
        ((Nh_TTY_LoggerNode*)SelectedNodes.handles_pp[i])->hasFocus = NH_FALSE;
    }
    Nh_freeList(&SelectedNodes, NH_FALSE);

NH_TTY_SILENT_END()
}

static Nh_TTY_LoggerNode *Nh_TTY_getFocusedLoggerNode(
    Nh_TTY_Logger *Logger_p)
{
NH_TTY_BEGIN()

    Nh_List SelectedNodes = Nh_TTY_getSelectedLoggerNodes(Logger_p);
    if (SelectedNodes.size <= 0) {NH_TTY_END(NULL)}

    int focus;
    for (focus = 0; focus < SelectedNodes.size && !((Nh_TTY_LoggerNode*)SelectedNodes.handles_pp[focus])->hasFocus; ++focus);
    Nh_TTY_LoggerNode *Focus_p = SelectedNodes.handles_pp[focus];

    Nh_freeList(&SelectedNodes, NH_FALSE);

NH_TTY_END(Focus_p)
}

static Nh_TTY_LoggerNode *Nh_TTY_getCurrentLoggerNode(
    Nh_TTY_LoggerNode *Node_p)
{
NH_TTY_BEGIN()

    if (Node_p->isCurrent) {
        NH_TTY_END(Node_p)
    }
    else
    {
        for (int i = 0; Node_p->isOpen && i < Node_p->Children.size; ++i) 
        {
            Nh_TTY_LoggerNode *Result_p = 
                Nh_TTY_getCurrentLoggerNode(Node_p->Children.handles_pp[i]);
            if (Result_p != NULL) {NH_TTY_END(Result_p)}
        }
    }

NH_TTY_END(NULL)
}

static void Nh_TTY_moveCursorVertically(
    Nh_TTY_Logger *Logger_p, Nh_TTY_LoggerNode *Current_p, int key) 
{
NH_TTY_BEGIN()

    Nh_List Nodes = Nh_initList(16);
    Nh_TTY_getLoggerNodes(&Logger_p->Root, &Nodes);
    int index;
    for (index = 0; index < Nodes.size && Nodes.handles_pp[index] != Current_p; ++index);

    switch (key) 
    {
        case 'w' :
            if (index > 0) {
                Current_p->isCurrent = NH_FALSE;
                ((Nh_TTY_LoggerNode*)Nodes.handles_pp[index - 1])->isCurrent = NH_TRUE;
            }
            break;
        case 's' :
            if (Nodes.size > index + 1) {
                Current_p->isCurrent = NH_FALSE;
                ((Nh_TTY_LoggerNode*)Nodes.handles_pp[index + 1])->isCurrent = NH_TRUE;
            }
            break;
    }

    Nh_freeList(&Nodes, NH_FALSE);

NH_TTY_SILENT_END()
}

static void Nh_TTY_unselectLoggerNode(
    Nh_TTY_Logger *Logger_p, Nh_TTY_LoggerNode *Selected_p)
{
NH_TTY_BEGIN()

    Nh_List SelectedNodes = Nh_TTY_getSelectedLoggerNodes(Logger_p);
    
    if (SelectedNodes.size > 1 && Selected_p->hasFocus) 
    {
        int index;
        for (index = 0; index < SelectedNodes.size && SelectedNodes.handles_pp[index] != Selected_p; ++index);

        if (index == 0) {((Nh_TTY_LoggerNode*)SelectedNodes.handles_pp[SelectedNodes.size - 1])->hasFocus = NH_TRUE;}
        else if (index == SelectedNodes.size - 1) {
            ((Nh_TTY_LoggerNode*)SelectedNodes.handles_pp[0])->hasFocus = NH_TRUE;
        }
        else {
            ((Nh_TTY_LoggerNode*)SelectedNodes.handles_pp[index - 1])->hasFocus = NH_TRUE;
        }
    }

    Selected_p->isSelected = NH_FALSE;
    Selected_p->hasFocus = NH_FALSE;

    Nh_freeList(&SelectedNodes, NH_FALSE);

NH_TTY_SILENT_END()
}

static void Nh_TTY_changeFocus(
    Nh_TTY_Logger *Logger_p, int c)
{
NH_TTY_BEGIN()

    Nh_List SelectedNodes = Nh_TTY_getSelectedLoggerNodes(Logger_p);
    if (SelectedNodes.size <= 1) {NH_TTY_SILENT_END()}

    int focus;
    for (focus = 0; focus < SelectedNodes.size && !((Nh_TTY_LoggerNode*)SelectedNodes.handles_pp[focus])->hasFocus; ++focus);

    switch (c) 
    {
        case 'f' :
            if (focus > 0) {
                ((Nh_TTY_LoggerNode*)SelectedNodes.handles_pp[focus])->hasFocus = NH_FALSE;
                ((Nh_TTY_LoggerNode*)SelectedNodes.handles_pp[focus - 1])->hasFocus = NH_TRUE;
            }
            break;
        case 'g' :
            if (focus < SelectedNodes.size - 1) {
                ((Nh_TTY_LoggerNode*)SelectedNodes.handles_pp[focus])->hasFocus = NH_FALSE;
                ((Nh_TTY_LoggerNode*)SelectedNodes.handles_pp[focus + 1])->hasFocus = NH_TRUE;
            }
            break;
    }

    Nh_freeList(&SelectedNodes, NH_FALSE);

NH_TTY_SILENT_END()
}

NH_TTY_RESULT Nh_TTY_handleLoggerInput(
    Nh_TTY_Program *Program_p, NH_UNICODE_CODEPOINT c)
{
NH_TTY_BEGIN()

    Nh_TTY_Logger *Logger_p = Program_p->handle_p;
    Nh_TTY_LoggerNode *Current_p = Nh_TTY_getCurrentLoggerNode(&Logger_p->Root);
    if (Current_p == NULL) {NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS)}

    Nh_TTY_LoggerNode *Focus_p = Nh_TTY_getFocusedLoggerNode(Logger_p);

    switch (c)
    {
        case 'w' :
        case 's' :
            Nh_TTY_moveCursorVertically(Logger_p, Current_p, c);
            break;

        case 'f' :
        case 'g' :
            Nh_TTY_changeFocus(Logger_p, c);
            break;

        case 'h' :
            if (Focus_p != NULL && Focus_p->peak > 0) {Focus_p->peak--;}
            break;
        case 'l' :
            if (Focus_p != NULL) {Focus_p->peak++;}
            break;

        case 'j' :
            if (Focus_p != NULL) {
                Focus_p->focusedRow++;
                Focus_p->peak = 0;
            }
            break;
        case 'k' :
            if (Focus_p != NULL) {
                Focus_p->focusedRow--;
                Focus_p->peak = 0;
            }
            break;

        case 'a' :
            if (Current_p->isSelected) {
                Nh_TTY_unselectLoggerNode(Logger_p, Current_p);
            }
            else if (Current_p->Parent_p->Parent_p != NULL) 
            {
                for (int i = 0; i < Current_p->Parent_p->Children.size; ++i) 
                {
                    Nh_TTY_LoggerNode *Sibling_p = Current_p->Parent_p->Children.handles_pp[i];
                    if (Sibling_p->isSelected) {
                        Nh_TTY_unselectLoggerNode(Logger_p, Sibling_p);
                    } 
                    Sibling_p->isCurrent = NH_FALSE;
                }
                Current_p->Parent_p->isOpen = NH_FALSE;
                Current_p->Parent_p->isCurrent = NH_TRUE;
            }
            break;

        case 'd' :

            Current_p->isOpen = NH_TRUE;

            if (Current_p->LoggerNode_p->Messages.size == 0) {
                Logger_p->listingWidth = Nh_TTY_getCategoryListingWidth(Logger_p);
            }
            else {
                Nh_TTY_resetLoggerFocuses(Logger_p);
                Current_p->isSelected = NH_TRUE;
                Current_p->hasFocus = NH_TRUE;
            }
            break;

        case CTRL_KEY('b') :
            Logger_p->showCategories = !Logger_p->showCategories; 
            break;
    }
 
    Program_p->refresh = NH_TRUE;

NH_TTY_END(NH_TTY_SUCCESS)
}

// DRAW ============================================================================================

static const char *help_pp[] =
{
    "NhTTY Logger",
    " ",
    "\e[1;7mw,a,s,d\e[0m Navigate logging categories.    ",
    "\e[1;7mh,j,k,l\e[0m Navigate log entries.           ",
    "\e[1;7mf,g\e[0m     Jump to left/right selected log.",
    "\e[1;7mc\e[0m       Copy selected log entry.        ",
    "\e[1;7mb\e[0m       Toggle logging categories.      ",
};

static NH_TTY_RESULT Nh_TTY_drawHelp(
    Nh_String *Data_p, int row, int cols, int rows)
{
NH_TTY_BEGIN()

    int entries = sizeof(help_pp) / sizeof(help_pp[0]);
    int begin = (rows / 2) - (entries / 2);

    if (row >= begin && row < begin + entries) {
        int len = row - begin > 1 ? strlen(help_pp[row - begin]) - 10 : strlen(help_pp[row - begin]);
        for (int i = 0; i < (cols - len) / 2; ++i) {
            Nh_appendToString(Data_p, " ", 1);
        }
        Nh_appendToString(Data_p, (NH_BYTE*)help_pp[row - begin], strlen(help_pp[row - begin]));
        for (int i = 0; i < (cols - len) / 2; ++i) {
            Nh_appendToString(Data_p, " ", 1);
        }
    }
    else {
        for (int i = 0; i < cols; ++i) {
            Nh_appendToString(Data_p, " ", 1);
        }
    }

NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS)
}

static NH_TTY_RESULT Nh_TTY_drawBottomBar(
    Nh_TTY_LoggerNode *SelectedNode_p, Nh_String *Data_p, int cols)
{
NH_TTY_BEGIN()

    Nh_appendToString(Data_p, "\e[1;7m", 6);

    Nh_appendFormatToString(Data_p, SelectedNode_p->LoggerNode_p->category_p);

    for (int i = 0; i < cols - strlen(SelectedNode_p->LoggerNode_p->category_p); ++i) {
        Nh_appendToString(Data_p, " ", 1);
    }

    Nh_appendToString(Data_p, "\e[0m", 4);

NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS)
}

static NH_TTY_RESULT Nh_TTY_drawSelected(
    Nh_TTY_Logger *Logger_p, Nh_String *Data_p, int row, int cols, int rows)
{
NH_TTY_BEGIN()

    Nh_List SelectedNodes = Nh_TTY_getSelectedLoggerNodes(Logger_p);
    cols = SelectedNodes.size > 0 ? cols / SelectedNodes.size : cols;

    for (int i = 0; i < SelectedNodes.size; ++i)
    {
        Nh_TTY_LoggerNode *SelectedNode_p = SelectedNodes.handles_pp[i]; 

        if (row == rows - 1) {
            Nh_TTY_drawBottomBar(SelectedNode_p, Data_p, cols);
            continue;        
        }

        if (SelectedNode_p->focusedRow > rows - 1) {
            SelectedNode_p->offset++;
            SelectedNode_p->focusedRow = rows - 1;
        }
        else if (SelectedNode_p->focusedRow < 0) {
            if (SelectedNode_p->offset > 0) {SelectedNode_p->offset--;}
            SelectedNode_p->focusedRow = 0;
        }

        Nh_List *Messages_p = &SelectedNode_p->LoggerNode_p->Messages;
        NH_BYTE *message_p = Nh_getFromList(Messages_p, row + SelectedNode_p->offset);

        if (message_p != NULL) 
        {
            if (SelectedNode_p->hasFocus && SelectedNode_p->focusedRow == row && strlen(message_p) > cols) {
                for (int i = 0; i < SelectedNode_p->peak && strlen(message_p) > cols; ++i) {
                    message_p = message_p + cols;
                }
            }

            int length = strlen(message_p) > cols ? cols : strlen(message_p);

            if (SelectedNode_p->hasFocus && SelectedNode_p->focusedRow == row) {
                Nh_appendToString(Data_p, "\e[1;7m", 6);
                Nh_appendToString(Data_p, message_p, length);
            } else {
                Nh_appendToString(Data_p, message_p, length);
            }
            
            for (int j = 0; j < cols - length; ++j) {
                Nh_appendToString(Data_p, " ", 1);
            }

            if (SelectedNode_p->hasFocus && SelectedNode_p->focusedRow == row) {
                Nh_appendToString(Data_p, "\e[0m", 4);
            }
        }
        else {
            for (int j = 0; j < cols; ++j) {Nh_appendToString(Data_p, " ", 1);}
        }
    }

    if (SelectedNodes.size == 0) {Nh_TTY_drawHelp(Data_p, row, cols, rows);}

    Nh_freeList(&SelectedNodes, NH_FALSE);

NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS)
}

NH_TTY_RESULT Nh_TTY_drawLoggerRow(
    Nh_TTY_Program *Program_p, Nh_UTF8String *Row_p, int width, int height, int row)
{
NH_TTY_BEGIN()

    Nh_TTY_Logger *Logger_p = Program_p->handle_p;
    Logger_p->height = height;

    int tmp = -1;
    Nh_List Nodes = Nh_initList(16);
    Nh_TTY_getLoggerNodes(&Logger_p->Root, &Nodes);
    Nh_TTY_LoggerNode *Node_p = Nh_getFromList(&Nodes, row);

    if (Logger_p->showCategories)
    {
        NH_TTY_TILE_COLOR color = Nh_TTY_getTileColorFromProgHandle(Logger_p);
        Nh_appendToString(Row_p, NH_TTY_INVERSE_TILE_COLORS_PP[color], 7);

        if (Node_p != NULL && row < Nodes.size) 
        {
            if (Node_p->isCurrent) {
                Nh_appendToString(Row_p, "\e[0m\e[1;7m", 10);
            }
            else if (Node_p->isSelected && Node_p->isCurrent) {
                Nh_appendToString(Row_p, "\e[0m\e[1;7m\e[1;5m", 16);
            }
            else if (Node_p->isSelected) {
                Nh_appendToString(Row_p, "\e[1;5m", 6);
            }

            int offset = Nh_TTY_getLoggerNodeDepth(Node_p);
            for (int i = 0; i < offset; ++i) {Nh_appendToString(Row_p, "  ", 2);}
            offset *= 2;
    
            Nh_appendFormatToString(Row_p, Node_p->LoggerNode_p->category_p);
            for (int i = 0; i < Logger_p->listingWidth - (strlen(Node_p->LoggerNode_p->category_p) + offset); ++i) {
                Nh_appendToString(Row_p, " ", 1);
            }
        }
        else {
            for (int i = 0; i < Logger_p->listingWidth; ++i) {
                Nh_appendToString(Row_p, " ", 1);
            }
        }

        Nh_appendToString(Row_p, "\e[0m", 4);
        NH_TTY_CHECK(Nh_TTY_drawSelected(Logger_p, Row_p, row, width - Logger_p->listingWidth, height))
    }
    else {NH_TTY_CHECK(Nh_TTY_drawSelected(Logger_p, Row_p, row, width, height))}

    Nh_freeList(&Nodes, NH_FALSE);

NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS)
}


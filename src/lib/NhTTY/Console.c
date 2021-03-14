// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "Console.h"
#include "Program.h"
#include "TreeListing.h"
#include "Row.h"
#include "Terminal.h"
#include "Shell.h"

#include "../NhLoader/Loader.h"
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

// APPEND ==========================================================================================

static NH_TTY_RESULT Nh_TTY_appendToConsole(
    Nh_TTY_Console *Console_p, NH_UNICODE_CODEPOINT *message_p, int length)
{
NH_TTY_BEGIN()

    if (Console_p->block) {NH_TTY_DIAGNOSTIC_END(NH_TTY_ERROR_BAD_STATE)}

    Nh_UnicodeString *NewLine_p = Nh_incrementArray(&Console_p->History); 
    NH_TTY_CHECK_MEM(NewLine_p)
    *NewLine_p = Nh_initUnicodeString(32);

    if (message_p != NULL && length > 0) {
        Nh_appendToUnicodeString(NewLine_p, message_p, length);
    }

NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS)
}

static NH_TTY_RESULT Nh_TTY_appendToCurrentConsole(
    NH_UNICODE_CODEPOINT *message_p, int length)
{
NH_TTY_BEGIN()

    Nh_TTY_Terminal *Terminal_p = Nh_TTY_getTerminal();
    if (Terminal_p == NULL) {NH_TTY_DIAGNOSTIC_END(NH_TTY_ERROR_BAD_STATE)}

    Nh_TTY_Console *Console_p = &Terminal_p->Tab_p->Tile_p->Console;
    if (Console_p->block) {NH_TTY_DIAGNOSTIC_END(NH_TTY_ERROR_BAD_STATE)}

NH_TTY_DIAGNOSTIC_END(Nh_TTY_appendToConsole(Console_p, message_p, length))
}

// INIT ============================================================================================

Nh_TTY_Console Nh_TTY_initConsole(
    Nh_Array *Programs_p)
{
NH_TTY_BEGIN()

    Nh_TTY_Console Console;

    Console.Programs_p = Programs_p;
    Console.currentProgram = 0;

    Console.hasFocus     = NH_FALSE;
    Console.cursorX      = 0;
    Console.cursorY      = 0;
    Console.height       = 1;
    Console.cachedHeight = 1;
    Console.History      = Nh_initArray(sizeof(Nh_UnicodeString), 255);
    Console.block        = NH_FALSE;
    Console.displace     = NH_TRUE;
    Console.hide         = NH_FALSE;
    Console.args_p       = NULL;
    Console.callback_f   = NULL;

    Nh_TTY_appendToConsole(&Console, NULL, 0);

NH_TTY_END(Console)
}

// LAST CONSOLE LINE ===============================================================================

static Nh_UnicodeString *Nh_TTY_getLastConsoleLine(
    Nh_TTY_Console *Console_p)
{
NH_TTY_BEGIN()

    if (Console_p->History.length == 0) {NH_TTY_END(NULL)}

NH_TTY_END(&((Nh_UnicodeString*)Console_p->History.bytes_p)[Console_p->History.length - 1])
}

static NH_TTY_RESULT Nh_TTY_appendToLastConsoleLine(
    Nh_TTY_Console *Console_p, NH_UNICODE_CODEPOINT *codepoints_p, int length)
{
NH_TTY_BEGIN()

    Nh_UnicodeString *Line_p = Nh_TTY_getLastConsoleLine(Console_p);
    Nh_appendToUnicodeString(Line_p, codepoints_p, length);
    Console_p->cursorX += length;

NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS)
}

static NH_TTY_RESULT Nh_TTY_clearLastConsoleLine(
    Nh_TTY_Console *Console_p)
{
NH_TTY_BEGIN()

    Nh_UnicodeString *Line_p = Nh_TTY_getLastConsoleLine(Console_p);
    Nh_freeUnicodeString(Line_p);
    Console_p->cursorX = 0;
    *Line_p = Nh_initUnicodeString(32);

NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS)
}

// MESSAGE =========================================================================================

NH_BOOL Nh_TTY_compareMessage(
    Nh_TTY_Console *Console_p, NH_UNICODE_CODEPOINT *codepoints_p, int length)
{
NH_TTY_BEGIN()

    Nh_UnicodeString *Line_p = Nh_TTY_getLastConsoleLine(Console_p);

    NH_BOOL equal = length == Line_p->length;
    for (int i = 0; equal && i < Line_p->length; ++i) {
        if (Line_p->p[i] != codepoints_p[i]) {equal = NH_FALSE;}
    }

NH_TTY_END(equal)
}

NH_BOOL Nh_TTY_compareMessage2(
    NH_UNICODE_CODEPOINT *codepoints_p, int length)
{
NH_TTY_BEGIN()

    Nh_TTY_Console *Console_p = &Nh_TTY_getTerminal()->Tab_p->Tile_p->Console;
    NH_BOOL equal = Nh_TTY_compareMessage(Console_p, codepoints_p, length);

NH_TTY_END(equal)
}

NH_TTY_RESULT Nh_TTY_setMessage(
    Nh_TTY_Console *Console_p, NH_UNICODE_CODEPOINT *codepoints_p, int length)
{
NH_TTY_BEGIN()

    if (Nh_TTY_compareMessage(Console_p, codepoints_p, length)) {NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS)}

    NH_TTY_CHECK(Nh_TTY_clearLastConsoleLine(Console_p))
    NH_TTY_CHECK(Nh_TTY_appendToLastConsoleLine(Console_p, codepoints_p, length))

NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS)
}

NH_TTY_RESULT Nh_TTY_setMessage2(
    NH_UNICODE_CODEPOINT *codepoints_p, int length)
{
NH_TTY_BEGIN()

    Nh_TTY_Console *Console_p = &Nh_TTY_getTerminal()->Tab_p->Tile_p->Console;
    NH_TTY_CHECK(Nh_TTY_setMessage(Console_p, codepoints_p, length))

NH_TTY_END(NH_TTY_SUCCESS)
}

NH_TTY_RESULT Nh_TTY_setDefaultMessage(
    Nh_TTY_Console *Console_p, NH_TTY_MESSAGE message)
{
NH_TTY_BEGIN()

    int length;
    NH_UNICODE_CODEPOINT *codepoints_p = Nh_TTY_getMessage(message, &length);
    NH_TTY_CHECK(Nh_TTY_setMessage(Console_p, codepoints_p, length))

NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS)
}

NH_TTY_RESULT Nh_TTY_setDefaultMessage2(
    NH_TTY_MESSAGE message)
{
NH_TTY_BEGIN()

    Nh_TTY_Console *Console_p = &Nh_TTY_getTerminal()->Tab_p->Tile_p->Console;
    NH_TTY_CHECK(Nh_TTY_setDefaultMessage(Console_p, message))

NH_TTY_END(NH_TTY_SUCCESS)
}

NH_TTY_RESULT Nh_TTY_setBinaryQueryMessage(
    Nh_TTY_Console *Console_p, NH_UNICODE_CODEPOINT *query_p, int length, void *args_p, 
    NH_TTY_RESULT (*callback_f)(Nh_TTY_Console *Console_p, NH_UNICODE_CODEPOINT input, NH_BOOL *continue_p))
{
NH_TTY_BEGIN()

    if (Console_p->block) {NH_TTY_DIAGNOSTIC_END(NH_TTY_ERROR_BAD_STATE)}

    int yesNoLength;
    NH_UNICODE_CODEPOINT *yesNo_p = Nh_TTY_getMessage(NH_TTY_MESSAGE_BINARY_QUERY_YES_NO, &yesNoLength);

    Nh_UnicodeString Question = Nh_initUnicodeString(128);
    Nh_appendToUnicodeString(&Question, query_p, length);
    Nh_appendToUnicodeString(&Question, yesNo_p, yesNoLength);

    NH_TTY_CHECK(Nh_TTY_setMessage(Console_p, Question.p, Question.length))

    Console_p->block = NH_TRUE;
    Console_p->args_p = args_p;
    Console_p->callback_f = callback_f;

    Nh_freeUnicodeString(&Question);

NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS)
}

NH_TTY_RESULT Nh_TTY_setBinaryQueryMessage2(
    NH_UNICODE_CODEPOINT *query_p, int length, void *args_p, NH_TTY_RESULT (*callback_f)(Nh_TTY_Console *Console_p, NH_UNICODE_CODEPOINT input, NH_BOOL *continue_p))
{
NH_TTY_BEGIN()

    Nh_TTY_Terminal *Terminal_p = Nh_TTY_getTerminal();
    if (Terminal_p == NULL) {NH_TTY_DIAGNOSTIC_END(NH_TTY_ERROR_BAD_STATE)}

    Nh_TTY_Console *Console_p = &Terminal_p->Tab_p->Tile_p->Console;
    if (Console_p->block) {NH_TTY_DIAGNOSTIC_END(NH_TTY_ERROR_BAD_STATE)}

NH_TTY_DIAGNOSTIC_END(Nh_TTY_setBinaryQueryMessage(Console_p, query_p, length, args_p, callback_f))
}

// TOGGLE ==========================================================================================

NH_TTY_RESULT Nh_TTY_toggleConsole(
    Nh_TTY_Console *Console_p)
{
NH_TTY_BEGIN()

    Console_p->hasFocus = !Console_p->hasFocus;

    if (Console_p->hasFocus) 
    {
        Nh_UnicodeString *Line_p = Nh_TTY_getLastConsoleLine(Console_p);

        if (Line_p == NULL || Line_p->length > 0) {
            Console_p->cursorX = 0;
            NH_TTY_CHECK(Nh_TTY_appendToCurrentConsole(NULL, 0))
        }
        if (Console_p->height == 0) {
            Console_p->height = Console_p->cachedHeight;
        }
    }
    else {
        if (Console_p->hide) {
            Console_p->height = 0;
        }
    }

NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS)
}

// PROGRAM SWITCH COMMAND ==========================================================================

static Nh_List Nh_TTY_matchCodepoint(
    Nh_List *Prototypes_p, int index, NH_UNICODE_CODEPOINT codepoint)
{
NH_TTY_BEGIN()

    Nh_List Matches = Nh_initList(16);

    for (int name = 0; name < Prototypes_p->size; ++name) {
        Nh_UnicodeString *Name_p = &((Nh_TTY_ProgramPrototype*)Prototypes_p->pp[name])->Name;
        if (index < Name_p->length) {
            if (Name_p->p[index] == codepoint) {
                Nh_appendToList(&Matches, Name_p); 
            }
        }
    } 

NH_TTY_END(Matches)
}

static NH_UNICODE_CODEPOINT SWITCH_P[] = {83, 119, 105, 116, 99, 104, 101, 100, 32, 116, 111, 32};

static NH_TTY_RESULT Nh_TTY_switchProgram(
    Nh_TTY_Console *Console_p, Nh_List *Prototypes_p, Nh_UnicodeString *Name_p)
{
NH_TTY_BEGIN()

    int index = -1;
    for (int i = 0; i < Prototypes_p->size; ++i) {
        if (&((Nh_TTY_ProgramPrototype*)Prototypes_p->pp[i])->Name == Name_p) {
            index = i;
            break;
        }
    }

    if (index > -1) {
        Console_p->currentProgram = index; 
        NH_TTY_CHECK(Nh_TTY_appendToConsole(Console_p, SWITCH_P, sizeof(SWITCH_P)/sizeof(SWITCH_P[0])))
        NH_TTY_CHECK(Nh_TTY_appendToLastConsoleLine(Console_p, Name_p->p, Name_p->length))
        NH_TTY_CHECK(Nh_TTY_handleCurrentTileInput(NH_TTY_CONSOLE_KEY))
        NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS)
    }

NH_TTY_DIAGNOSTIC_END(NH_TTY_ERROR_BAD_STATE)
}

static NH_TTY_RESULT Nh_TTY_handleProgramSwitchCommand(
    Nh_TTY_Console *Console_p, NH_UNICODE_CODEPOINT c)
{
NH_TTY_BEGIN()

    NH_TTY_CHECK(Nh_TTY_appendToLastConsoleLine(Console_p, &c, 1))
    Nh_UnicodeString *Line_p = Nh_TTY_getLastConsoleLine(Console_p);

    Nh_List *Prototypes_p = &Nh_TTY_getTerminal()->Prototypes;
    Nh_List Matches = Nh_TTY_matchCodepoint(Prototypes_p, 0, Line_p->p[0]);

    if (Matches.size == 0) {
        NH_TTY_END(NH_TTY_SUCCESS)
    } 
    if (Matches.size == 1) {
        NH_TTY_CHECK(Nh_TTY_switchProgram(Console_p, Prototypes_p, Matches.pp[0]))
        Nh_freeList(&Matches, NH_FALSE);
        NH_TTY_END(NH_TTY_SUCCESS)  
    }

    for (int index = 1; index < Line_p->length; ++index)
    {
        Nh_List NewMatches = Nh_TTY_matchCodepoint(&Matches, index, Line_p->p[index]);
        if (Matches.size <= 1) {break;}
        else {
            Nh_freeList(&Matches, NH_FALSE);
            Matches = NewMatches;
        }
    }

    if (Matches.size == 1) {
        NH_TTY_CHECK(Nh_TTY_switchProgram(Console_p, Prototypes_p, Matches.pp[0]))
    }

    Nh_freeList(&Matches, NH_FALSE);

NH_TTY_END(NH_TTY_SUCCESS)
}

// PROGRAM COMMAND ==================================================================================

NH_UNICODE_CODEPOINT UNKNOWN_COMMAND_P[] = {
    85, 110, 107, 110, 111, 119, 110, 32, 99, 111, 109, 109, 97, 110, 100, 46
};

static NH_TTY_RESULT Nh_TTY_handleProgramCommand(
    Nh_TTY_Console *Console_p, NH_UNICODE_CODEPOINT c)
{
NH_TTY_BEGIN()

    Nh_UnicodeString *CurrentConsoleLine_p = Nh_TTY_getLastConsoleLine(Console_p);

    if (CurrentConsoleLine_p->p[0] != 64) {
        NH_TTY_DIAGNOSTIC_END(NH_TTY_ERROR_BAD_STATE)
    }

    Nh_TTY_Program *Program_p = Nh_TTY_getCurrentProgram(Console_p);

    if (Program_p->Prototype_p->handleConsoleInput_f != NULL) 
    {
        NH_TTY_RESULT result = Program_p->Prototype_p->handleConsoleInput_f(Program_p, c);
        if (result == NH_TTY_ERROR_UNKNOWN_COMMAND) {
            NH_TTY_CHECK(Nh_TTY_appendToCurrentConsole(UNKNOWN_COMMAND_P, sizeof(UNKNOWN_COMMAND_P)/sizeof(UNKNOWN_COMMAND_P[0])))
            NH_TTY_CHECK(Nh_TTY_handleCurrentTileInput(NH_TTY_CONSOLE_KEY))
        }
    }

NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS)
}

// NETZHAUT COMMAND ================================================================================

static NH_TTY_RESULT Nh_TTY_handleNetzhautCommand(
    Nh_TTY_Console *Console_p, NH_UNICODE_CODEPOINT c)
{
NH_TTY_BEGIN()

    NH_LOADER.load_f("NhWebIDL", 0);
    NH_TTY_CHECK(Nh_TTY_handleCurrentTileInput(NH_TTY_CONSOLE_KEY))

NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS)
}

// INPUT ===========================================================================================

NH_TTY_RESULT Nh_TTY_handleFirstCodepoint(
    Nh_TTY_Console *Console_p, NH_UNICODE_CODEPOINT c)
{
NH_TTY_BEGIN()

    switch (c)
    {
        case 49:
            Console_p->height++;
            Console_p->cachedHeight++;
            Console_p->cursorY++;
            break;

        case 50:
            if (Console_p->height > 1) {
                Console_p->height--;
                Console_p->cachedHeight--;
                Console_p->cursorY--;
            }
            break;

        case 51:
            Console_p->hide = !Console_p->hide;
            break;

        case 64:
        {
            Nh_TTY_ProgramPrototype *Prototype_p = Nh_TTY_getCurrentProgram(Console_p)->Prototype_p;
            NH_TTY_CHECK(Nh_TTY_appendToLastConsoleLine(Console_p, &c, 1))
            NH_TTY_CHECK(Nh_TTY_appendToLastConsoleLine(Console_p, Prototype_p->Name.p, Prototype_p->Name.length))
            NH_UNICODE_CODEPOINT whitespace = 32;
            NH_TTY_CHECK(Nh_TTY_appendToLastConsoleLine(Console_p, &whitespace, 1))
            break;
        }

        case 33:
        {
            NH_UNICODE_CODEPOINT codepoints_p[] = {110, 101, 116, 122, 104, 97, 117, 116};
            NH_TTY_CHECK(Nh_TTY_appendToLastConsoleLine(Console_p, &c, 1))
            NH_TTY_CHECK(Nh_TTY_appendToLastConsoleLine(Console_p, codepoints_p, 8))
            NH_UNICODE_CODEPOINT whitespace = 32;
            NH_TTY_CHECK(Nh_TTY_appendToLastConsoleLine(Console_p, &whitespace, 1))
            break;
        }

        case '\n':
        case '\r':
            NH_TTY_CHECK(Nh_TTY_handleCurrentTileInput(NH_TTY_CONSOLE_KEY))
            break;

        default :
            NH_TTY_CHECK(Nh_TTY_handleProgramSwitchCommand(Console_p, c))
            break;
    }

NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS)
}

NH_TTY_RESULT Nh_TTY_handleConsoleInput(
    Nh_TTY_Console *Console_p, NH_UNICODE_CODEPOINT c)
{
NH_TTY_BEGIN()

    switch (c)
    {
        case '\x1b':
            NH_TTY_CHECK(Nh_TTY_clearLastConsoleLine(Console_p))
            NH_TTY_DIAGNOSTIC_END(Nh_TTY_handleCurrentTileInput(NH_TTY_CONSOLE_KEY))
        case 127:
            if (Nh_TTY_getLastConsoleLine(Console_p)->length > 0) {
                Nh_removeTailFromUnicodeString(Nh_TTY_getLastConsoleLine(Console_p), 1);
                Console_p->cursorX--;
            }
            NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS) 
    }
    
    if (Nh_TTY_getLastConsoleLine(Console_p)->length == 0) {
        NH_TTY_CHECK(Nh_TTY_handleFirstCodepoint(Console_p, c))
    }
    else if (Nh_TTY_getLastConsoleLine(Console_p)->p[0] == 64) {
        NH_TTY_CHECK(Nh_TTY_handleProgramCommand(Console_p, c))
    }
    else if (Nh_TTY_getLastConsoleLine(Console_p)->p[0] == 33) {
        NH_TTY_CHECK(Nh_TTY_handleNetzhautCommand(Console_p, c))
    }
    else if (c != '\n' && c != '\r') {
        NH_TTY_CHECK(Nh_TTY_handleProgramSwitchCommand(Console_p, c))
    }
    else {
        NH_TTY_CHECK(Nh_TTY_appendToCurrentConsole(UNKNOWN_COMMAND_P, sizeof(UNKNOWN_COMMAND_P)/sizeof(UNKNOWN_COMMAND_P[0])))
        NH_TTY_CHECK(Nh_TTY_handleCurrentTileInput(NH_TTY_CONSOLE_KEY))
    }

NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS)
}

// DRAW ============================================================================================

NH_TTY_RESULT Nh_TTY_drawConsoleRow(
    Nh_TTY_Console *Console_p, Nh_String *Row_p, int row, int cols)
{
NH_TTY_BEGIN()

    Nh_appendFormatToString(Row_p, "%s", NH_TTY_INVERSE_TILE_COLORS_PP[Nh_TTY_getTileColorFromConsole(Console_p)]);
 
    if (Console_p->displace ? Console_p->History.length + row < 0 : Console_p->History.length - row > 0) {
        Nh_appendFormatToString(Row_p, "%s", NH_TTY_INVERSE_TILE_COLORS_PP[Nh_TTY_getTileColorFromConsole(Console_p)]);
        NH_BYTE row_p[1024];
        memset(row_p, 32, 1024);
        Nh_appendToString(Row_p, row_p, cols);
    }
    else {
        Nh_UnicodeString *Line_p = &((Nh_UnicodeString*)Console_p->History.bytes_p)[Console_p->History.length + row];
        Nh_Encoding_UTF8String UTF8 = Nh_encodeTextToUTF8(Line_p->p, Line_p->length);
        Nh_appendToString(Row_p, UTF8.bytes_p, UTF8.length);
        for (int i = 0; i < cols - Line_p->length; ++i) {
            Nh_appendToString(Row_p, " ", 1); 
        }
    }

    Nh_appendToString(Row_p, "\e[0m", 4);

NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS)
}

// GET CURRENT PROGRAM =============================================================================

Nh_TTY_Program *Nh_TTY_getCurrentProgram(
    Nh_TTY_Console *Console_p)
{
NH_TTY_BEGIN()

    if (Console_p->Programs_p == NULL) {NH_TTY_END(NULL)}

NH_TTY_END(&((Nh_TTY_Program*)Console_p->Programs_p->bytes_p)[Console_p->currentProgram])
}


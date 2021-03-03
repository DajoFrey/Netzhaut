// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "TextFile.h"
#include "TreeListing.h"
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

#include "../NhWebIDL/Runtime/Parser.h"
#include "../NhLoader/Loader.h"

#include <stddef.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>

// OPEN ============================================================================================

static NH_BYTE *Nh_TTY_readFile(
    NH_BYTE *path_p)
{
NH_TTY_BEGIN()

    NH_BYTE *buffer_p = 0;
    long length;
    FILE *f = fopen(path_p, "r");
    
    if (f)
    {
        fseek(f, 0, SEEK_END);
        length = ftell(f);

        if (length > 0) {
            fseek(f, 0, SEEK_SET);
            buffer_p = malloc(length);
            if (buffer_p) {fread(buffer_p, 1, length, f);}
        }
        else { // maybe we can read it
            Nh_String String = Nh_initString(1024);
            NH_BYTE str_p[1024];
            memset(str_p, 0, 1024);
            read(fileno(f), str_p, 1023);
            Nh_appendToString(&String, str_p, strlen(str_p));
            Nh_appendToString(&String, "\0", 1);
            buffer_p = String.bytes_p;
        }

        fclose(f);
    }
    
NH_TTY_END(buffer_p)
}

static Nh_Array *Nh_TTY_newLine(
    Nh_TTY_TextFile *TextFile_p, int index)
{
NH_TTY_BEGIN()

#include NH_TTY_CUSTOM_CHECK

    Nh_Array *New_p = Nh_allocate(sizeof(Nh_Array));
    NH_TTY_CHECK_MEM(NULL, New_p)
    *New_p = Nh_initArray(sizeof(NH_UNICODE_CODEPOINT), 128);
    NH_CHECK(NULL, Nh_insertIntoList(&TextFile_p->Lines, New_p, index))

#include NH_TTY_DEFAULT_CHECK

NH_TTY_END(New_p)
}

Nh_TTY_TextFile *Nh_TTY_openTextFile(
    NH_BYTE *path_p)
{
NH_TTY_BEGIN()

#include NH_TTY_CUSTOM_CHECK

    Nh_TTY_TextFile *TextFile_p = malloc(sizeof(Nh_TTY_TextFile));
    NH_TTY_CHECK_MEM(NULL, TextFile_p)

    TextFile_p->textType  = 0;
    TextFile_p->fileCursorXTarget = 0;
    TextFile_p->fileCursorX   = 0;
    TextFile_p->fileCursorY   = 0;
    TextFile_p->screenCursorX = 0;
    TextFile_p->screenCursorY = 0;
    TextFile_p->rowOffset = 0;
    TextFile_p->colOffset = 0;
    TextFile_p->tabSize   = 4;

    TextFile_p->Lines       = Nh_initList(1024);
    TextFile_p->RenderLines = Nh_initArray(sizeof(Nh_String), 1024);

    if (path_p) 
    {
        TextFile_p->textType = Nh_TTY_getTextType(path_p);

        NH_BYTE *bytes_p = Nh_TTY_readFile(path_p);
        NH_TTY_CHECK_MEM(NULL, bytes_p)

        Nh_Array UnicodeCodepoints = Nh_decodeUTF8Text(bytes_p);

        for (unsigned long i = 0, lineStart = 0; i < UnicodeCodepoints.length; ++i) 
        {
            if (((NH_UNICODE_CODEPOINT*)UnicodeCodepoints.bytes_p)[i] == '\n')
            {
                Nh_Array *New_p = Nh_TTY_newLine(TextFile_p, TextFile_p->Lines.size);
                NH_TTY_CHECK_MEM(NULL, New_p)
                NH_CHECK(NULL, Nh_appendToArray(New_p, &((NH_UNICODE_CODEPOINT*)UnicodeCodepoints.bytes_p)[lineStart], i - lineStart))
                lineStart = i + 1;
            }
            if (((NH_UNICODE_CODEPOINT*)UnicodeCodepoints.bytes_p)[i] == '\0') {break;}
        }

        Nh_freeArray(&UnicodeCodepoints);
        free(bytes_p);
    }
    else {NH_TTY_CHECK_MEM(NULL, Nh_TTY_newLine(TextFile_p, TextFile_p->Lines.size))}

#include NH_TTY_DEFAULT_CHECK

NH_TTY_END(TextFile_p)
}

// CLOSE ===========================================================================================

NH_TTY_RESULT Nh_TTY_closeTextFile(
    Nh_TTY_TextFile *TextFile_p)
{
NH_TTY_BEGIN()

    for (int i = 0; i < TextFile_p->Lines.size; ++i) {
        Nh_Array *String_p = TextFile_p->Lines.handles_pp[i];
        Nh_freeArray(String_p);
    }
    Nh_freeList(&TextFile_p->Lines, NH_FALSE);

    for (int i = 0; i < TextFile_p->RenderLines.length; ++i) {
        Nh_String *String_p = &((Nh_String*)TextFile_p->RenderLines.bytes_p)[i];
        Nh_freeString(String_p);
    }
    Nh_freeArray(&TextFile_p->RenderLines);

    Nh_free(TextFile_p);

NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS)
}

// EXECUTE =========================================================================================

NH_TTY_RESULT Nh_TTY_parseWebIDLTextFile(
    NH_BYTE *name_p, Nh_TTY_TextFile *TextFile_p)
{
NH_TTY_BEGIN()

    Nh_UTF8String Bytes = Nh_initString(255);

    for (int i = 0; i < TextFile_p->Lines.size; ++i) 
    {
        if (i > 0) {Nh_appendToString(&Bytes, "\n", 1);}
        Nh_Array *UnicodeCodepoints_p = TextFile_p->Lines.handles_pp[i];

        for (int j = 0; j < UnicodeCodepoints_p->length; ++j) {
            NH_BYTE bytes_p[4] = {'\0'};
            size_t byteSize = Nh_encodeToUTF8(((NH_UNICODE_CODEPOINT*)UnicodeCodepoints_p->bytes_p)[j], bytes_p);
            Nh_appendToString(&Bytes, bytes_p, byteSize);
        }
    }

    Nh_WebIDL_parseFragment_f parseFragment_f = NH_LOADER.loadFunction_f("NhWebIDL", 0, "Nh_WebIDL_parseFragment");

    if (parseFragment_f != NULL) 
    {
        Nh_WebIDL_FinalParseResult ParseResult = parseFragment_f(name_p, Bytes.bytes_p);
        Nh_WebIDL_unparseFragment_f unparseFragment_f = NH_LOADER.loadFunction_f("NhWebIDL", 0, "Nh_WebIDL_unparseFragment");
        if (unparseFragment_f != NULL) {unparseFragment_f(ParseResult);}
        else {NH_TTY_DIAGNOSTIC_END(NH_TTY_ERROR_BAD_STATE)}
    }
    else {NH_TTY_DIAGNOSTIC_END(NH_TTY_ERROR_BAD_STATE)}

    Nh_freeString(&Bytes);

NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS)
}

NH_TTY_RESULT Nh_TTY_executeTextFile(
    Nh_TTY_TextFile *TextFile_p)
{
NH_TTY_BEGIN()

//    Nh_String Bytes = Nh_initString(255);
//    Nh_appendToString(&Bytes, "NH_IPC_ECMASCRIPT", 17);
//
//    for (int i = 0; i < TextFile_p->Lines.size; ++i) 
//    {
//        if (i > 0) {Nh_appendToString(&Bytes, "\n", 1);}
//        Nh_Array *UnicodeCodepoints_p = TextFile_p->Lines.handles_pp[i];
//        for (int j = 0; j < UnicodeCodepoints_p->length; ++j) {
//            NH_BYTE bytes_p[4] = {'\0'};
//            size_t byteSize = Nh_encodeToUTF8(((NH_UNICODE_CODEPOINT*)UnicodeCodepoints_p->bytes_p)[j], bytes_p);
//            Nh_appendToString(&Bytes, bytes_p, byteSize);
//        }
//    }
//
//    if (Nh_getFork()) {
//        Nh_writeToChannel(&Nh_getFork()->IPC.Out, Bytes.bytes_p, Bytes.length);
//    }
//    else {Nh_handleIPCReceive(Bytes.bytes_p);}
//
//// TODO free

NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS)
}

// RENDER ==========================================================================================

static int Nh_TTY_renderTextFileLineNumber(
    Nh_TTY_TextFile *TextFile_p, Nh_String *RenderLine_p, int line)
{
NH_TTY_BEGIN()

    NH_BYTE maxLineNumber_p[16] = {'\0'};
    sprintf(maxLineNumber_p, "%d", TextFile_p->Lines.size);

    NH_BYTE lineNumber_p[16] = {'\0'};
    sprintf(lineNumber_p, "%d", line + 1);

    Nh_appendFormatToString(RenderLine_p, "\e[1;7m%s", lineNumber_p);
    for (int i = 0; i < strlen(maxLineNumber_p) - strlen(lineNumber_p); ++i) {
        Nh_appendToString(RenderLine_p, " ", 1);
    }
    Nh_appendToString(RenderLine_p, "\e[0m ", 5);

NH_TTY_END(strlen(maxLineNumber_p) + 1)
}

static int Nh_TTY_getRenderLineWidth(
    Nh_TTY_TextFile *TextFile_p, NH_UNICODE_CODEPOINT *codepoints_p, int codepoints)
{
NH_TTY_BEGIN()

    int renderCodepoints = 0;

    for (int i = 0; i < codepoints; ++i) {
        if (codepoints_p[i] == 9) {renderCodepoints += TextFile_p->tabSize;} // tab
        else {renderCodepoints++;}
    }

NH_TTY_END(renderCodepoints)
}

static NH_TTY_RESULT Nh_TTY_renderTextFileLine(
    Nh_TTY_TextFile *TextFile_p, Nh_String *RenderLine_p, int line)
{
NH_TTY_BEGIN()

    TextFile_p->lineNumberOffset = Nh_TTY_renderTextFileLineNumber(TextFile_p, RenderLine_p, line);
    int lineWidth = TextFile_p->width - TextFile_p->lineNumberOffset;

    Nh_UnicodeString *EditorLine_p = TextFile_p->Lines.handles_pp[line];
    int renderCodepoints, codepoints = EditorLine_p->length;
    for (renderCodepoints = codepoints; (renderCodepoints = Nh_TTY_getRenderLineWidth(TextFile_p, (NH_UNICODE_CODEPOINT*)EditorLine_p->bytes_p, codepoints)) > lineWidth;) {codepoints--;}

//    Nh_UnicodeString HighlightedLine = *Line_p;
//
//    switch (TextFile_p->textType)
//    {
//        case NH_TTY_TEXT_C   : HighlightedLine = Nh_TTY_cSyntaxHighlights(Line_p, &length); break;
//        case NH_TTY_TEXT_CPP : HighlightedLine = Nh_TTY_cppSyntaxHighlights(Line_p, &length); break;
//    }

    for (int i = 0; i < codepoints; ++i) 
    {
        if (((NH_UNICODE_CODEPOINT*)EditorLine_p->bytes_p)[i] == 9) {
            for (int j = 0; j < TextFile_p->tabSize; ++j) {Nh_appendToString(RenderLine_p, " ", 1);}
        }
        else {
            NH_BYTE bytes_p[4] = {'\0'};
            size_t byteSize = Nh_encodeToUTF8(((NH_UNICODE_CODEPOINT*)EditorLine_p->bytes_p)[i], bytes_p);
            Nh_appendToString(RenderLine_p, bytes_p, byteSize);
        }
    }

    for (int i = 0; i < lineWidth - renderCodepoints; ++i) {Nh_appendToString(RenderLine_p, " ", 1);}

//    if (TextFile_p->textType != NH_TTY_TEXT_UNDEFINED) {Nh_freeArray(&HighlightedLine);}
//    Nh_appendToString(Data_p, "\e[0m", 4);

NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS)
}

static NH_TTY_RESULT Nh_TTY_updateTextFileRenderLine(
    Nh_TTY_TextFile *TextFile_p, int line)
{
NH_TTY_BEGIN()

    Nh_String *RenderLine_p = &((Nh_String*)TextFile_p->RenderLines.bytes_p)[line];
    Nh_freeString(RenderLine_p);
    *RenderLine_p = Nh_initString(256);
   
    NH_TTY_CHECK(Nh_TTY_renderTextFileLine(TextFile_p, RenderLine_p, line))

NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS)
}

NH_TTY_RESULT Nh_TTY_renderTextFile(
    Nh_TTY_TextFile *TextFile_p, int width)
{
NH_TTY_BEGIN()

    if (width > -1) {TextFile_p->width = width;}

    for (int i = 0; i < TextFile_p->RenderLines.length; ++i) {
        Nh_freeString(&((Nh_String*)TextFile_p->RenderLines.bytes_p)[i]);
    }

    Nh_freeArray(&TextFile_p->RenderLines);
    TextFile_p->RenderLines = Nh_initArray(sizeof(Nh_String), TextFile_p->Lines.size);

    for (int line = 0; line < TextFile_p->Lines.size; ++line) {
        Nh_String *RenderLine_p = Nh_getFromArray(&TextFile_p->RenderLines, -1);
        *RenderLine_p = Nh_initString(256);
        NH_TTY_CHECK(Nh_TTY_renderTextFileLine(TextFile_p, RenderLine_p, line))
    }

NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS)
}

// INPUT ===========================================================================================

static void Nh_TTY_handleFileCursorXTarget(
    Nh_TTY_TextFile *TextFile_p, Nh_Array *Line_p)
{
NH_TTY_BEGIN()

    if (TextFile_p->fileCursorXTarget > Line_p->length) {
        TextFile_p->fileCursorX = Line_p->length < 1 ? 0 : Line_p->length - 1;
        TextFile_p->screenCursorX = TextFile_p->fileCursorX % TextFile_p->width;
    }
    else {
        TextFile_p->fileCursorX = TextFile_p->fileCursorXTarget;
        TextFile_p->screenCursorX = TextFile_p->fileCursorX % TextFile_p->width;
    }

NH_TTY_SILENT_END()
}

NH_TTY_RESULT Nh_TTY_handleTextFileInput(
    Nh_TTY_File *File_p, int height, NH_UNICODE_CODEPOINT c, NH_BOOL insertMode, NH_BOOL *refresh_p)
{
NH_TTY_BEGIN()

    Nh_TTY_TextFile *TextFile_p = File_p->handle_p;
    Nh_Array *Line_p = Nh_getFromList(&TextFile_p->Lines, TextFile_p->fileCursorY);
    NH_TTY_CHECK_NULL(Line_p)

    switch (c)
    {
        case 'j' : if (insertMode) {goto INSERT_CHAR;}
        case CTRL_KEY('j') : 

            if (TextFile_p->fileCursorY < TextFile_p->Lines.size - 1) 
            {
                TextFile_p->fileCursorY++;

                if (TextFile_p->screenCursorY < height - 1) {TextFile_p->screenCursorY++;}
                else {
                    TextFile_p->rowOffset++;
                    *refresh_p = NH_TRUE;
                }

                Nh_TTY_handleFileCursorXTarget(TextFile_p, Nh_getFromList(&TextFile_p->Lines, TextFile_p->fileCursorY));
            }
            break; 

        case 'k' : if (insertMode) {goto INSERT_CHAR;}
        case CTRL_KEY('k') : 

            if (TextFile_p->fileCursorY > 0)
            {
                TextFile_p->fileCursorY--;
                if (TextFile_p->screenCursorY > 0) {TextFile_p->screenCursorY--;}
                else if (TextFile_p->rowOffset > 0) {
                    TextFile_p->rowOffset--;
                    *refresh_p = NH_TRUE;
                }

                Nh_TTY_handleFileCursorXTarget(TextFile_p, Nh_getFromList(&TextFile_p->Lines, TextFile_p->fileCursorY));
            }
            break; 

        case 'l' : if (insertMode) {goto INSERT_CHAR;}
        case CTRL_KEY('l') : 

            if (TextFile_p->fileCursorX < Line_p->length) 
            {
                TextFile_p->fileCursorX++;
                TextFile_p->fileCursorXTarget++;
                if (TextFile_p->screenCursorX < TextFile_p->width - TextFile_p->lineNumberOffset - 1) {TextFile_p->screenCursorX++;}
                else {
                    // TODO
                }
            }
            break;

        case 'h' : if (insertMode) {goto INSERT_CHAR;}
        case CTRL_KEY('h') : 

            if (TextFile_p->fileCursorX > 0) 
            {
                TextFile_p->fileCursorX--;
                TextFile_p->fileCursorXTarget--;
                if (TextFile_p->screenCursorX > 0) {TextFile_p->screenCursorX--;}
                else {
                    // TODO
                }
            }
            break;
    }

    if (File_p->readOnly) {NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS)}

    switch (c)
    {
        case CTRL_KEY('h') : 
        case CTRL_KEY('j') : 
        case CTRL_KEY('k') : 
        case CTRL_KEY('l') : 
            break;

        case 'o' : if (insertMode) {goto INSERT_CHAR;}
        case CTRL_KEY('o') :

            NH_TTY_CHECK_MEM(Nh_TTY_newLine(TextFile_p, TextFile_p->fileCursorY))
            NH_TTY_CHECK(Nh_TTY_handleTextFileInput(File_p, height, 'j', NH_FALSE, refresh_p))
            Nh_TTY_renderTextFile(TextFile_p, -1);
            *refresh_p = NH_TRUE;
            break;

        case 'x' : if (insertMode) {goto INSERT_CHAR;}
        case CTRL_KEY('x') :

            if (TextFile_p->fileCursorY >= TextFile_p->Lines.size - 1) {
                NH_TTY_CHECK(Nh_TTY_handleTextFileInput(File_p, height, 'k', NH_FALSE, refresh_p))
                NH_CHECK(NH_TTY_ERROR_BAD_STATE, Nh_removeFromList(&TextFile_p->Lines, NH_TRUE, TextFile_p->fileCursorY + 1))
            }
            else {
                NH_CHECK(NH_TTY_ERROR_BAD_STATE, Nh_removeFromList(&TextFile_p->Lines, NH_TRUE, TextFile_p->fileCursorY))
            }
            Nh_TTY_renderTextFile(TextFile_p, -1);
            *refresh_p = NH_TRUE;
            break;

        case 'i' : if (insertMode) {goto INSERT_CHAR;}
        case CTRL_KEY('i') :

            NH_CHECK(NH_TTY_ERROR_BAD_STATE, Nh_removeFromArray(Line_p, TextFile_p->fileCursorX, 1))
            Nh_TTY_renderTextFile(TextFile_p, -1);
            *refresh_p = NH_TRUE;
            break;

        case 'u' : if (insertMode) {goto INSERT_CHAR;}
        case CTRL_KEY('u') :

            if (TextFile_p->fileCursorX > 0) {
                NH_CHECK(NH_TTY_ERROR_BAD_STATE, Nh_removeFromArray(Line_p, TextFile_p->fileCursorX - 1, 1))
                NH_TTY_CHECK(Nh_TTY_handleTextFileInput(File_p, height, 'h', NH_FALSE, refresh_p))
                Nh_TTY_renderTextFile(TextFile_p, -1);
                *refresh_p = NH_TRUE;
            }
            break;

        default : if (insertMode) {goto INSERT_CHAR;}
    }
 
NH_TTY_END(NH_TTY_SUCCESS)

INSERT_CHAR:

    if (!File_p->readOnly) {
        NH_CHECK(NH_TTY_ERROR_BAD_STATE, Nh_insertIntoArray(Line_p, TextFile_p->fileCursorX, &c, 1))
        Nh_TTY_handleTextFileInput(File_p, height, 'l', NH_FALSE, refresh_p);
        Nh_TTY_updateTextFileRenderLine(TextFile_p, TextFile_p->fileCursorY);
    }

    *refresh_p = NH_TRUE;

NH_TTY_END(NH_TTY_SUCCESS)
}

// DRAW ============================================================================================

NH_TTY_RESULT Nh_TTY_drawTextFileLine(
    Nh_TTY_TextFile *TextFile_p, Nh_String *String_p, int line)
{
NH_TTY_BEGIN()

    int offsetLine = line + TextFile_p->rowOffset;

    if (offsetLine < TextFile_p->Lines.size) 
    {
        Nh_String *RenderLine_p = &((Nh_String*)TextFile_p->RenderLines.bytes_p)[offsetLine];
        Nh_appendToString(String_p, RenderLine_p->bytes_p, RenderLine_p->length);
    }
    else {
        NH_BYTE whitespace_p[1024];
        memset(whitespace_p, 32, 1024);
        Nh_appendToString(String_p, whitespace_p, TextFile_p->width);
    }

NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS)
}


// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "SyntaxHighlights.h"
#include "TreeListing.h"
#include "Program.h"
#include "Row.h"
#include "Terminal.h"
#include "Shell.h"
#include "Editor.h"

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

// WORDS ===========================================================================================

static NH_BYTE *cTypeKeywords_pp[] = {
    "auto",
    "char",
    "const",
    "double",
    "enum",
    "extern",
    "float",
    "inline",
    "int",
    "long",
    "register",
    "restrict",
    "short",
    "signed",
    "static",
    "struct",
    "typedef",
    "union",
    "unsigned",
    "void",
    "volatile",
    "_Alignas",
    "_Atomic",
    "_Bool",
    "_Complex",
    "_Decimal128",
    "_Decimal32",
    "_Decimal64",
    "_Imaginary",
    "_Noreturn",
    "_Thread_local",
};

static NH_BYTE *cControlKeywords_pp[] = {
    "break",
    "case",
    "continue",
    "default",
    "do",
    "else",
    "for",
    "goto",
    "if",
    "return",
    "sizeof",
    "switch",
    "while",
    "_Alignof",
    "_Generic",
    "_Static_assert",
};

//// INIT ============================================================================================
//
//static NH_TTY_RESULT Nh_TTY_insertHighlightWord(
//    Nh_UnicodeString *String_p, Nh_UnicodeString Word, NH_BYTE *prefix_p, NH_BYTE *suffix_p)
//{
//NH_TTY_BEGIN()
//
//    Nh_UnicodeString Prefix = Nh_decodeUTF8Text(prefix_p);
//    Nh_UnicodeString Suffix = Nh_decodeUTF8Text(suffix_p);
//
//    Nh_appendToArray(String_p, Prefix.bytes_p, Prefix.length);
//    Nh_appendToArray(String_p, Word.bytes_p, Word.length);
//    Nh_appendToArray(String_p, Suffix.bytes_p, Suffix.length);
//
//    Nh_freeArray(&Prefix);
//    Nh_freeArray(&Suffix);
//
//NH_TTY_END(NH_TTY_SUCCESS)
//}
//
//Nh_UnicodeString Nh_TTY_highlightWord(
//    Nh_UnicodeString Word, Nh_UnicodeString *String_p, NH_BYTE *prefix_p, NH_BYTE *suffix_p)
//{
//NH_TTY_BEGIN()
//
//    Nh_UnicodeString NewString = Nh_initArray(sizeof(NH_UNICODE_CODEPOINT), 64);
//
//    for (int i = 0; i < String_p->length; ++i) 
//    {
//        NH_UNICODE_CODEPOINT *codepoint_p = &((NH_UNICODE_CODEPOINT*)String_p->bytes_p)[i];
//
//        if (*codepoint_p == ((NH_UNICODE_CODEPOINT*)Word.bytes_p)[0]) 
//        {
//            NH_BOOL match = NH_TRUE;
//            for (int j = 1; j < Word.length; ++j) {
//                if (((NH_UNICODE_CODEPOINT*)Word.bytes_p)[j] != codepoint_p[j] || j + i >= String_p->length) {
//                    match = NH_FALSE;
//                    break;
//                }
//            }
//
//            if (match) {
//                Nh_TTY_insertHighlightWord(&NewString, Word, prefix_p, suffix_p);
//                i += Word.length - 1;
//                continue;
//            }
//        }
//
//        Nh_appendToArray(&NewString, codepoint_p, 1);
//    }
//
//NH_TTY_END(NewString)
//}
//
//Nh_UnicodeString Nh_TTY_highlightWords(
//    NH_BYTE *words_pp[], size_t words, Nh_UnicodeString *String_p, NH_BYTE *prefix_p, NH_BYTE *suffix_p)
//{
//NH_TTY_BEGIN()
//
//    Nh_UnicodeString String = Nh_initArray(sizeof(NH_UNICODE_CODEPOINT), 64);
//
//    for (int i = 0; i < words; ++i) 
//    {
//        NH_BYTE bytes_p[255] = {'\0'};
//        Nh_UnicodeString Word = Nh_decodeUTF8Text(words_pp[i]);
//        Nh_UnicodeString NewString = Nh_TTY_highlightWord(Word, String_p, prefix_p, suffix_p);
//        Nh_freeArray(&String);
//        String = NewString;
//        String_p = &String;
//        Nh_freeArray(&Word);
//    }
//
//NH_TTY_END(String)
//}
//
//// C ===============================================================================================
//
//Nh_UnicodeString Nh_TTY_cSyntaxHighlights(
//    Nh_UnicodeString *String_p)
//{
//NH_TTY_BEGIN()
//
//    Nh_UnicodeString String1 = Nh_TTY_highlightWords(
//        cTypeKeywords_pp, sizeof(cTypeKeywords_pp) / sizeof(cTypeKeywords_pp[0]), String_p, "\e[0;32m", "\e[0m"
//    ); 
//    Nh_UnicodeString String2 = Nh_TTY_highlightWords(
//        cControlKeywords_pp, sizeof(cControlKeywords_pp) / sizeof(cControlKeywords_pp[0]), &String1, "\e[1;33m", "\e[0m"
//    ); 
//    Nh_freeArray(&String1);
//
//NH_TTY_END(String2)
//}
//
//// CPP =============================================================================================
//
//Nh_UnicodeString Nh_TTY_cppSyntaxHighlights(
//    Nh_UnicodeString *String_p)
//{
//NH_TTY_BEGIN()
//NH_TTY_END(Nh_initArray(0, 0))
//}
//
//// CODE TYPE =======================================================================================

NH_TTY_TEXT Nh_TTY_getTextType(
    NH_BYTE *path_p)
{
NH_TTY_BEGIN()

    if (path_p == NULL) {NH_TTY_END(NH_TTY_TEXT_UNDEFINED)}

    if (strstr(path_p, ".c") || strstr(path_p, ".h")) {
        NH_TTY_END(NH_TTY_TEXT_C)
    }

NH_TTY_END(NH_TTY_TEXT_UNDEFINED)
}


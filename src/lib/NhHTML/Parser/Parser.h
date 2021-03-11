#ifndef NH_HTML_PARSER_H
#define NH_HTML_PARSER_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "Tokenizer.h"

#include "../Common/API.h"
#include "../Common/Result.h"

#include "../../NhCore/Stack.h"

#endif

/** @addtogroup NhHTMLEnums
 *  @{
 */

    typedef enum NH_HTML_INSERTION_MODE {
        NH_HTML_INSERTION_MODE_INITIAL,
        NH_HTML_INSERTION_MODE_BEFORE_HTML,
        NH_HTML_INSERTION_MODE_BEFORE_HEAD,
        NH_HTML_INSERTION_MODE_IN_HEAD,
        NH_HTML_INSERTION_MODE_IN_HEAD_NOSCRIPT,
        NH_HTML_INSERTION_MODE_AFTER_HEAD,
        NH_HTML_INSERTION_MODE_IN_BODY,
        NH_HTML_INSERTION_MODE_TEXT,
        NH_HTML_INSERTION_MODE_IN_TABLE,
        NH_HTML_INSERTION_MODE_IN_TABLE_TEXT,
        NH_HTML_INSERTION_MODE_IN_CAPTION,
        NH_HTML_INSERTION_MODE_IN_COLUMN_GROUP,
        NH_HTML_INSERTION_MODE_IN_TABLE_BODY,
        NH_HTML_INSERTION_MODE_IN_ROW,
        NH_HTML_INSERTION_MODE_IN_CELL,
        NH_HTML_INSERTION_MODE_IN_SELECT,
        NH_HTML_INSERTION_MODE_IN_SELECT_IN_TABLE,
        NH_HTML_INSERTION_MODE_IN_TEMPLATE,
        NH_HTML_INSERTION_MODE_AFTER_BODY,
        NH_HTML_INSERTION_MODE_IN_FRAMESET,
        NH_HTML_INSERTION_MODE_AFTER_FRAMESET,
        NH_HTML_INSERTION_MODE_AFTER_AFTER_BODY,
        NH_HTML_INSERTION_MODE_AFTER_AFTER_FRAMESET,
    } NH_HTML_INSERTION_MODE;

    typedef enum NH_HTML_PARSE_ERROR {
        NH_HTML_PARSE_ERROR_ABRUPT_CLOSING_OF_EMPTY_COMMENT,
        NH_HTML_PARSE_ERROR_ABRUPT_DOCTYPE_PUBLIC_IDENTIFIER,
        NH_HTML_PARSE_ERROR_ABRUPT_DOCTYPE_SYSTEM_IDENTIFIER,
        NH_HTML_PARSE_ERROR_ABSENCE_OF_DIGITS_IN_NUMERIC_CHARACTER_REFERENCE,
        NH_HTML_PARSE_ERROR_CDATA_IN_HTML_CONTENT,
        NH_HTML_PARSE_ERROR_CHARACTER_REFERENCE_OUTSIDE_UNICODE_RANGE,
        NH_HTML_PARSE_ERROR_CONTROL_CHARACTER_IN_INPUT_STREAM,
        NH_HTML_PARSE_ERROR_CONTROL_CHARACTER_REFERENCE,
        NH_HTML_PARSE_ERROR_END_TAG_WITH_ATTRIBUTES,
        NH_HTML_PARSE_ERROR_DUPLICATE_ATTRIBUTE,
        NH_HTML_PARSE_ERROR_END_TAG_WITH_TRAILING_SOLIDUS,
        NH_HTML_PARSE_ERROR_EOF_BEFORE_TAG_NAME,
        NH_HTML_PARSE_ERROR_EOF_IN_CDATA,
        NH_HTML_PARSE_ERROR_EOF_IN_COMMENT,
        NH_HTML_PARSE_ERROR_EOF_IN_DOCTYPE,
        NH_HTML_PARSE_ERROR_EOF_IN_SCRIPT_HTML_COMMENT_LIKE_TEXT,
        NH_HTML_PARSE_ERROR_EOF_IN_TAG,
        NH_HTML_PARSE_ERROR_INCORRECTLY_CLOSED_COMMENT,
        NH_HTML_PARSE_ERROR_INCORRECTLY_OPENED_COMMENT,
        NH_HTML_PARSE_ERROR_INVALID_CHARACTER_SEQUENCE_AFTER_DOCTYPE_NAME,
        NH_HTML_PARSE_ERROR_INVALID_FIRST_CHARACTER_OF_TAG_NAME,
        NH_HTML_PARSE_ERROR_MISSING_ATTRIBUTE_VALUE,
        NH_HTML_PARSE_ERROR_MISSING_DOCTYPE_NAME,
        NH_HTML_PARSE_ERROR_MISSING_DOCTYPE_PUBLIC_IDENTIFIER,
        NH_HTML_PARSE_ERROR_MISSING_DOCTYPE_SYSTEM_IDENTIFIER,
        NH_HTML_PARSE_ERROR_MISSING_END_TAG_NAME,
        NH_HTML_PARSE_ERROR_MISSING_QUOTE_BEFORE_DOCTYPE_PUBLIC_IDENTIFIER,
        NH_HTML_PARSE_ERROR_MISSING_QUOTE_BEFORE_DOCTYPE_SYSTEM_IDENTIFIER,
        NH_HTML_PARSE_ERROR_MISSING_SEMICOLON_AFTER_CHARACTER_REFERENCE,
        NH_HTML_PARSE_ERROR_MISSING_WHITESPACE_AFTER_DOCTYPE_PUBLIC_KEYWORD,
        NH_HTML_PARSE_ERROR_MISSING_WHITESPACE_AFTER_DOCTYPE_SYSTEM_KEYWORD,
        NH_HTML_PARSE_ERROR_MISSING_WHITESPACE_BEFORE_DOCTYPE_NAME,
        NH_HTML_PARSE_ERROR_MISSING_WHITESPACE_BETWEEN_ATTRIBUTES,
        NH_HTML_PARSE_ERROR_MISSING_WHITESPACE_BETWEEN_DOCTYPE_PUBLIC_AND_SYSTEM_IDENTIFIERS,
        NH_HTML_PARSE_ERROR_NESTED_COMMENT,
        NH_HTML_PARSE_ERROR_NONCHARACTER_CHARACTER_REFERENCE,
        NH_HTML_PARSE_ERROR_NONCHARACTER_IN_INPUT_STREAM,
        NH_HTML_PARSE_ERROR_NON_VOID_HTML_ELEMENT_START_TAG_WITH_TRAILING_SOLIDUS,
        NH_HTML_PARSE_ERROR_NULL_CHARACTER_REFERENCE,
        NH_HTML_PARSE_ERROR_SURROGATE_CHARACTER_REFERENCE,
        NH_HTML_PARSE_ERROR_SURROGATE_IN_INPUT_STREAM,
        NH_HTML_PARSE_ERROR_UNEXPECTED_CHARACTER_AFTER_DOCTYPE_SYSTEM_IDENTIFIER,
        NH_HTML_PARSE_ERROR_UNEXPECTED_CHARACTER_IN_ATTRIBUTE_NAME,
        NH_HTML_PARSE_ERROR_UNEXPECTED_CHARACTER_IN_UNQUOTED_ATTRIBUTE_VALUE,
        NH_HTML_PARSE_ERROR_UNEXPECTED_EQUALS_SIGN_BEFORE_ATTRIBUTE_NAME,
        NH_HTML_PARSE_ERROR_UNEXPECTED_NULL_CHARACTER,
        NH_HTML_PARSE_ERROR_UNEXPECTED_QUESTION_MARK_INSTEAD_OF_TAG_NAME,
        NH_HTML_PARSE_ERROR_UNEXPECTED_SOLIDUS_IN_TAG,
        NH_HTML_PARSE_ERROR_UNKNOWN_NAMED_CHARACTER_REFERENCE,
    } NH_HTML_PARSE_ERROR;

/** @} */

/** @addtogroup NhHTMLStructs
 *  @{
 */

    typedef struct Nh_HTML_ParseError {
        NH_HTML_PARSE_ERROR type;
        unsigned long index;
    } Nh_HTML_ParseError;

    typedef struct Nh_HTML_Parser {
        NH_HTML_INSERTION_MODE insertionMode;
        NH_HTML_INSERTION_MODE originalInsertionMode;
        NH_BOOL framesetOk;
        NH_BOOL pause;
        NH_BOOL stop;
        NH_BOOL scriptingEnabled;
        NH_BOOL fosterParenting;
        unsigned int scriptNestingLevel;
        Nh_Stack OpenElements;
        Nh_WebIDL_Object *Document_p;
        Nh_WebIDL_Object *HeadElement_p;
        Nh_Array Errors;
        Nh_HTML_Token *Token_p;
    } Nh_HTML_Parser;

/** @} */

/** @addtogroup NhHTMLTypedefs
 *  @{
 */

    typedef Nh_WebIDL_Object *(*Nh_HTML_parseDocument_f)(
        Nh_UnicodeString InputStream, Nh_WebIDL_Object *Document_p
    );

/** @} */

/** @addtogroup NhHTMLFunctions
 *  @{
 */

    NH_HTML_RESULT Nh_HTML_newParseError(
        Nh_HTML_Parser *Parser_p, unsigned long index, NH_HTML_PARSE_ERROR type
    );

    Nh_WebIDL_Object *Nh_HTML_parseDocument(
        Nh_UnicodeString InputStream, Nh_WebIDL_Object *Document_p
    );

/** @} */

#endif

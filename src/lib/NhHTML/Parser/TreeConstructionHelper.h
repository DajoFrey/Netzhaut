#ifndef NH_HTML_TREE_CONSTRUCTION_HELPER_H
#define NH_HTML_TREE_CONSTRUCTION_HELPER_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "Elements.h"
#include "Parser.h"

#endif

/** @addtogroup NhHTMLFunctions
 *  @{
 */

    Nh_WebIDL_Object *Nh_HTML_createElementForToken(
        Nh_HTML_Token *Token_p, Nh_WebIDL_DOMString *Namespace_p, Nh_WebIDL_Object *IntendedParent_p
    );
    
    NH_WEBIDL_UNSIGNED_LONG Nh_HTML_getAppropriatePlaceForInsertingNode(
        Nh_HTML_Parser *Parser_p, Nh_WebIDL_Object **Target_pp
    );
    
    NH_HTML_RESULT Nh_HTML_insertCommentAtPosition(
        Nh_HTML_Parser *Parser_p, Nh_WebIDL_Object *Target_p, NH_WEBIDL_UNSIGNED_LONG position
    );
    
    NH_HTML_RESULT Nh_HTML_insertComment(
        Nh_HTML_Parser *Parser_p, Nh_WebIDL_Object *Node_p
    );
    
    Nh_WebIDL_Object *Nh_HTML_insertForeignElement(
        Nh_HTML_Parser *Parser_p, Nh_HTML_Token *Token_p, Nh_WebIDL_DOMString *Namespace_p
    );
    
    Nh_WebIDL_Object *Nh_HTML_insertHTMLElement(
        Nh_HTML_Parser *Parser_p, Nh_HTML_Token *Token_p
    );
    
    NH_HTML_RESULT Nh_HTML_insertCharacter(
        Nh_HTML_Parser *Parser_p, Nh_WebIDL_Object *Target_p, Nh_WebIDL_DOMString *Data_p
    );
    
    NH_HTML_RESULT Nh_HTML_parseRAWTEXTOrRCDATA(
        Nh_HTML_Parser *Parser_p, Nh_HTML_Token *Token_p, NH_BOOL RAWTEXT
    );
    
    Nh_HTML_Token *Nh_HTML_getEmptyStartTagToken(
        NH_HTML_TAG tag
    );

// ACTIVE FORMATTING ELEMENTS

    NH_HTML_RESULT Nh_HTML_pushActiveFormattingElement(
        Nh_HTML_Parser *Parser_p, Nh_WebIDL_Object *Element_p
    );

    NH_HTML_RESULT Nh_HTML_insertMarker(
        Nh_HTML_Parser *Parser_p
    );

    NH_HTML_RESULT Nh_HTML_reconstructActiveFormattingElements(
        Nh_HTML_Parser *Parser_p
    );

// IMPLIED END TAGS

    NH_HTML_RESULT Nh_HTML_generateImpliedEndTags(
        Nh_HTML_Parser *Parser_p, NH_BYTE *exclude_p
    );
    
    NH_HTML_RESULT Nh_HTML_generateAllImpliedEndTags(
        Nh_HTML_Parser *Parser_p
    );

// STACK OF OPEN ELEMENTS

    Nh_WebIDL_Object *Nh_HTML_getCurrentNode(
        Nh_HTML_Parser *Parser_p
    );

    Nh_WebIDL_Object *Nh_HTML_getAdjustedCurrentNode(
        Nh_HTML_Parser *Parser_p
    );

    NH_HTML_RESULT Nh_HTML_pushOpenElement(
        Nh_HTML_Parser *Parser_p, Nh_WebIDL_Object *Object_p
    );

    Nh_WebIDL_Object *Nh_HTML_popCurrentNode(
        Nh_HTML_Parser *Parser_p
    );

    NH_BOOL Nh_HTML_inSpecialCategory(
        Nh_WebIDL_Object *Node_p
    );

    NH_BOOL Nh_HTML_hasElementInScope(
        Nh_HTML_Parser *Parser_p, NH_BYTE *target_p
    );
    
    NH_BOOL Nh_HTML_hasElementInListItemScope(
        Nh_HTML_Parser *Parser_p, NH_BYTE *target_p
    );
    
    NH_BOOL Nh_HTML_hasElementInButtonScope(
        Nh_HTML_Parser *Parser_p, NH_BYTE *target_p
    );
    
    NH_BOOL Nh_HTML_hasElementInTableScope(
        Nh_HTML_Parser *Parser_p, NH_BYTE *target_p
    );
    
    NH_BOOL Nh_HTML_hasElementInSelectScope(
        Nh_HTML_Parser *Parser_p, NH_BYTE *target_p
    );

/** @} */

#endif

#ifndef NH_FONT_H
#define NH_FONT_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "URI.h"
#include "Common/API.h"
#include "Common/Result.h"

#endif

/** @addtogroup NhCoreStructs
 *  @{
 */

    typedef struct Nh_Glyph {
        void *external_p;
        int codepoint;
        size_t width;
        size_t height;
        int xOffset;
        int yOffset;
        float xAdvance;
        float yAdvance;
        float u0;
        float v0;
        float u1;
        float v1;
    } Nh_Glyph;

    typedef struct Nh_Atlas {
        void *external_p;
        void *data_p;
        size_t width;
        size_t height;
    } Nh_Atlas;

    typedef struct Nh_Font {
        Nh_URI URI;
        Nh_Atlas Atlas;
        void *external_p;
        char *familyName_p;
        char *styleName_p;
        float ascender;
        float descender;
    } Nh_Font;

/** @} */

/** @addtogroup NhCoreFunctions
 *  @{
 */

    NH_RESULT Nh_initFontManager(
    );

    void Nh_destroyFontManager(
    );

    NH_RESULT Nh_addInitialFonts(
    );
    
    NH_RESULT Nh_addInitialFontPrototypes(
    );

    NH_RESULT Nh_addFontDirectory(
        char *dirPath_p
    );
    
    NH_RESULT Nh_addFont(
        char *absolutePath_p
    );
    
    NH_RESULT Nh_addFontPrototype(
        char *familyName_p, char *styleName_p
    );
    
    Nh_Font *Nh_getFontFromPrototype(
        Nh_Font *Prototype_p
    );
    
    Nh_Font *Nh_getFontFromFamilyName(
        char *familyName_p
    );
    
    Nh_Font *Nh_getFontFromStyleName(
        char *styleName_p
    );
    
    Nh_Font *Nh_getFontFromURI(
        Nh_URI *URI_p
    );

    Nh_Font *Nh_getDefaultFont(
    );
    
    NH_RESULT Nh_initExternalFontLib(
    );
    
    NH_RESULT Nh_closeExternalFontLib(
    );

    NH_RESULT Nh_loadFont(
        Nh_Font *Font_p
    );

    Nh_Glyph Nh_getGlyph(
        Nh_Font *Font_p, const char *codepoint_p, NH_PIXEL fontSize
    );

    float Nh_getKerning(
        Nh_Glyph *Glyph_p, const char *codepoint_p
    );

/** @} */

#endif 

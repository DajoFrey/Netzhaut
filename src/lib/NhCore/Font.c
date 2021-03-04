// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "Font.h"
#include "File.h"
#include "Memory.h"
#include "Config.h"
#include "String.h"
#include "List.h"

#include "External/freetype.h"
#include "External/freetype-gl.h"

#include "Common/Log.h"
#include "Common/Macro.h"
#include NH_FLOW
#include NH_DEFAULT_CHECK

#include <string.h>
#include <stdio.h>

#ifdef __unix__ 	
    #include <dirent.h>
#elif defined(_WIN32) || defined(WIN32)
#endif

// DECLARE =========================================================================================

typedef struct Nh_FontManager {
    Nh_Font *Default_p;
    FT_Library Library;
    Nh_List Fonts;
    Nh_List Prototypes;
} Nh_FontManager;

// DATA ============================================================================================

static Nh_FontManager FontManager;

// CREATE ==========================================================================================

NH_RESULT Nh_initFontManager()
{
NH_BEGIN()

    FontManager.Default_p = NULL;
    FontManager.Fonts = Nh_initList(128);
    FontManager.Prototypes = Nh_initList(128);

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

void Nh_destroyFontManager()
{
NH_BEGIN()

    for (int i = 0; i < FontManager.Fonts.size; ++i) {
        Nh_Font *Font_p = FontManager.Fonts.pp[i];
        Nh_freeURI(&Font_p->URI);
        Nh_free(Font_p->familyName_p);
        Nh_free(Font_p->styleName_p);
        if (Font_p->Atlas.external_p != NULL) {texture_atlas_delete(Font_p->Atlas.external_p);}
        if (Font_p->external_p != NULL) {texture_font_delete(Font_p->external_p);}
    }

    for (int i = 0; i < FontManager.Prototypes.size; ++i) {
        Nh_Font *Prototype_p = FontManager.Prototypes.pp[i];
        Nh_free(Prototype_p->familyName_p);
        Nh_free(Prototype_p->styleName_p);
    }

    Nh_freeList(&FontManager.Fonts, NH_TRUE);
    Nh_freeList(&FontManager.Prototypes, NH_TRUE);

NH_SILENT_END()
}

// ADD =============================================================================================

NH_RESULT Nh_addInitialFonts()
{
NH_BEGIN()

    Nh_initExternalFontLib();

#ifdef __unix__
    NH_CHECK(Nh_addFontDirectory("/usr/share/fonts/"))
    NH_CHECK(Nh_addFontDirectory("/usr/local/share/fonts/"))
    NH_CHECK(Nh_addFontDirectory("~/.fonts/"))
#elif defined(_WIN32) || defined (WIN32)
    exit(0);
#endif

    Nh_closeExternalFontLib();

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

NH_RESULT Nh_addInitialFontPrototypes()
{
NH_BEGIN()

    NH_CHECK(Nh_addFontPrototype("Nimbus Roman", "Regular"))
    NH_CHECK(Nh_addFontPrototype("Arial", "Regular"))   
    NH_CHECK(Nh_addFontPrototype("Verdana", "Regular"))        
    NH_CHECK(Nh_addFontPrototype("Cantarell", "Regular"))      
    NH_CHECK(Nh_addFontPrototype("Georgia", "Regular"))      
    NH_CHECK(Nh_addFontPrototype("Droid Sans", "Regular"))     
    NH_CHECK(Nh_addFontPrototype("Noto Sans Mono", "Regular"))

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

NH_RESULT Nh_addFontDirectory(
    NH_BYTE *dirPath_p)
{
NH_BEGIN()

#ifdef __unix__ 

    struct dirent *entry_p = NULL;
    NH_BYTE fullPath_p[1024] = {'\0'};

    DIR *dir_p = opendir(dirPath_p); 
    if (dir_p == NULL) {NH_DIAGNOSTIC_END(NH_SUCCESS)}
  
    while ((entry_p = readdir(dir_p)) != NULL) 
    {
        if (dirPath_p[strlen(dirPath_p) - 1] != '/') {
            sprintf(fullPath_p, "%s/%s", dirPath_p, entry_p->d_name);
        } else {
            sprintf(fullPath_p, "%s%s", dirPath_p, entry_p->d_name);
        }

        NH_RESULT result = Nh_addFont(fullPath_p);
        if (result != NH_SUCCESS && result != NH_ERROR_INVALID_FILE_FORMAT) {closedir(dir_p); NH_END(result)}

        if (strcmp(entry_p->d_name, "..") != 0 && strcmp(entry_p->d_name, ".") != 0) {
            NH_CHECK(Nh_addFontDirectory(fullPath_p))
        }
    }
  
    closedir(dir_p);    

#elif defined(_WIN32) || defined(WIN32)

    printf("windows not supported\n");
    exit(0);

#endif

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

NH_RESULT Nh_addFont(
    NH_BYTE *absolutePath_p)
{
NH_BEGIN()

    if (!Nh_isRegularFile(absolutePath_p)) {NH_DIAGNOSTIC_END(NH_ERROR_INVALID_FILE_FORMAT)}

    Nh_URI URI = Nh_createURI(absolutePath_p, NULL, NULL, -1);
    NH_BYTE *data_p = Nh_getPayload(&URI);

    if (data_p == NULL) {
        Nh_freeURI(&URI);
        return NH_ERROR_INVALID_FILE_FORMAT;
    } 

    FT_Face Face; 
    int error = FT_New_Memory_Face(FontManager.Library, data_p, URI.size, 0, &Face);

    if (error == FT_Err_Unknown_File_Format) {
        Nh_freePayload(&URI);
        NH_DIAGNOSTIC_END(NH_ERROR_INVALID_FILE_FORMAT)
    }
    else if (error) {
        Nh_freePayload(&URI);
        NH_DIAGNOSTIC_END(NH_SUCCESS)
    }

    Nh_Font *Font_p = Nh_allocate(sizeof(Nh_Font));
    NH_CHECK_MEM(Font_p)

    Font_p->Atlas.external_p = NULL;
    Font_p->Atlas.data_p     = NULL;
    Font_p->external_p       = NULL;
    Font_p->URI              = URI;

    Font_p->familyName_p = Nh_allocateBytes(Face->family_name);
    NH_CHECK_MEM(Font_p->familyName_p)

    Font_p->styleName_p = Nh_allocateBytes(Face->style_name);
    NH_CHECK_MEM(Font_p->styleName_p)

    NH_CHECK(Nh_appendToList(&FontManager.Fonts, Font_p))

    FT_Done_Face(Face);
    Nh_freePayload(&Font_p->URI);

    Nh_logNewFont(absolutePath_p);

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

NH_RESULT Nh_addFontPrototype(
    NH_BYTE *familyName_p, NH_BYTE *styleName_p)
{
NH_BEGIN()

    Nh_Font *Font_p = Nh_allocate(sizeof(Nh_Font));
    NH_CHECK_MEM(Font_p)

    Font_p->familyName_p = NULL;
    Font_p->styleName_p  = NULL;

    if (familyName_p != NULL) 
    {
        Font_p->familyName_p = Nh_allocate(sizeof(NH_BYTE) * (strlen(familyName_p) + 1));
        NH_CHECK_MEM(Font_p->familyName_p)
        strcpy(Font_p->familyName_p, familyName_p);
    }

    if (styleName_p != NULL)
    {
        Font_p->styleName_p = Nh_allocate(sizeof(NH_BYTE) * (strlen(styleName_p) + 1));
        NH_CHECK_MEM(Font_p->styleName_p)
        strcpy(Font_p->styleName_p, styleName_p);
    }

    NH_CHECK(Nh_appendToList(&FontManager.Prototypes, Font_p))

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

// LOAD ============================================================================================

NH_RESULT Nh_loadFont(
    Nh_Font *Font_p)
{
NH_BEGIN()

    if (Font_p->Atlas.external_p == NULL) {
        Font_p->Atlas.external_p = texture_atlas_new(1024, 1024, 1);
        NH_CHECK_NULL(Font_p->Atlas.external_p)
        Font_p->Atlas.width  = ((texture_atlas_t*)Font_p->Atlas.external_p)->width; 
        Font_p->Atlas.height = ((texture_atlas_t*)Font_p->Atlas.external_p)->height;
        Font_p->Atlas.data_p = ((texture_atlas_t*)Font_p->Atlas.external_p)->data;
    }
    if (Font_p->external_p == NULL) {
        void *data_p = Nh_getPayload(&Font_p->URI);
        Font_p->external_p = texture_font_new_from_memory(Font_p->Atlas.external_p, 16, data_p, Font_p->URI.size);
        NH_CHECK_NULL(Font_p->external_p)
        ((texture_font_t*)Font_p->external_p)->rendermode = RENDER_SIGNED_DISTANCE_FIELD;
        Font_p->ascender = ((texture_font_t*)Font_p->external_p)->ascender;
        Font_p->descender = ((texture_font_t*)Font_p->external_p)->descender;
    }

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

// GLYPH ===========================================================================================

Nh_Glyph Nh_getGlyph(
    Nh_Font *Font_p, const NH_BYTE *codepoint_p, NH_PIXEL fontSize)
{
NH_BEGIN()

    Nh_Glyph Glyph = {0};

    ((texture_font_t*)Font_p->external_p)->size = fontSize;

    texture_glyph_t *Glyph_p = texture_font_get_glyph(Font_p->external_p, codepoint_p);
    if (Glyph_p == NULL) {NH_END(Glyph)}

    Glyph.external_p = Glyph_p;

    Glyph.codepoint = Glyph_p->codepoint;
    Glyph.width     = Glyph_p->width;
    Glyph.height    = Glyph_p->height;
    Glyph.xOffset   = Glyph_p->offset_x;
    Glyph.yOffset   = Glyph_p->offset_y;
    Glyph.xAdvance  = Glyph_p->advance_x;
    Glyph.yAdvance  = Glyph_p->advance_y;

    Glyph.u0 = Glyph_p->s0;
    Glyph.v0 = Glyph_p->t0;
    Glyph.u1 = Glyph_p->s1;
    Glyph.v1 = Glyph_p->t1;

    ((texture_font_t*)Font_p->external_p)->size = 16;

NH_END(Glyph)
}

float Nh_getKerning(
    Nh_Glyph *Glyph_p, const NH_BYTE *codepoint_p)
{
NH_BEGIN()
NH_END(texture_glyph_get_kerning(Glyph_p->external_p, codepoint_p))
}

// GET =============================================================================================

Nh_Font *Nh_getFontFromPrototype(
    Nh_Font *Prototype_p)
{
NH_BEGIN()

    for (int i = 0; i < FontManager.Fonts.size; ++i) 
    {
        Nh_Font *Font_p = FontManager.Fonts.pp[i];
       
        if (!strcmp(Font_p->familyName_p, Prototype_p->familyName_p)
        &&  !strcmp(Font_p->styleName_p,  Prototype_p->styleName_p ))
        {
            NH_END(Font_p)
        }
    }

NH_END(NULL)
}

Nh_Font *Nh_getFontFromFamilyName(
    NH_BYTE *familyName_p)
{
NH_BEGIN()

    for (int i = 0; i < FontManager.Fonts.size; ++i) 
    {
        Nh_Font *Font_p = FontManager.Fonts.pp[i];
        if (!strcmp(Font_p->familyName_p, familyName_p)) {NH_END(Font_p)}
    }

NH_END(NULL)
}

Nh_Font *Nh_getFontFromStyleName(
    NH_BYTE *styleName_p)
{
NH_BEGIN()

    for (int i = 0; i < FontManager.Fonts.size; ++i) 
    {
        Nh_Font *Font_p = FontManager.Fonts.pp[i];
        if (!strcmp(Font_p->styleName_p, styleName_p)) {NH_END(Font_p)}
    }

NH_END(NULL)
}

Nh_Font *Nh_getFontFromURI(
    Nh_URI *URI_p)
{
NH_BEGIN()

    for (int i = 0; i < FontManager.Fonts.size; ++i) 
    {
        Nh_Font *Font_p = FontManager.Fonts.pp[i];
        if (Nh_equivalentURI(URI_p, &Font_p->URI)) {NH_END(Font_p)}
    }

NH_END(NULL)
}

Nh_Font *Nh_getDefaultFont()
{
NH_BEGIN()

    if (FontManager.Default_p == NULL)
    {
        for (int i = 0; i < FontManager.Prototypes.size; ++i) 
        {
            Nh_Font *Prototype_p = FontManager.Prototypes.pp[i];
            Nh_Font *Font_p = Nh_getFontFromPrototype(Prototype_p); 
            if (Font_p != NULL) {
                FontManager.Default_p = Font_p;
                NH_END(Font_p)
            }
        }
    }
    else {NH_END(FontManager.Default_p)}

NH_END(NULL)
}

// EXTERNAL ========================================================================================

NH_RESULT Nh_initExternalFontLib()
{
NH_BEGIN()

    int error = FT_Init_FreeType(&FontManager.Library);
    if (error) {NH_DIAGNOSTIC_END(NH_ERROR_BAD_STATE)}

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

NH_RESULT Nh_closeExternalFontLib()
{
NH_BEGIN()

    FT_Done_FreeType(FontManager.Library);

NH_DIAGNOSTIC_END(NH_SUCCESS)
}


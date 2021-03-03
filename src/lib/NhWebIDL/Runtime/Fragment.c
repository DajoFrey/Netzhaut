// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "Fragment.h"
#include "Builtin.h"

#include "../Common/Macro.h"
#include NH_WEBIDL_FLOW
#include NH_WEBIDL_DEFAULT_CHECK

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// CREATE ==========================================================================================

static Nh_WebIDL_FinalParseResult Nh_WebIDL_parseBuiltinFragment(
    NH_BYTE *qualifiedName_p)
{
NH_WEBIDL_BEGIN()

    int builtinIndex = -1;
    for (int i = 0; i < NH_WEBIDL_FRAGMENT_NAMES_PP_COUNT; ++i) {
        if (!strcmp(qualifiedName_p, NH_WEBIDL_FRAGMENT_NAMES_PP[i])) {
            builtinIndex = i;
            break;
        }
    }

    if (builtinIndex >= 0) {
        NH_WEBIDL_END(Nh_WebIDL_parseFragment(qualifiedName_p, NH_WEBIDL_FRAGMENTS_PP[builtinIndex]))
    }

NH_WEBIDL_END(Nh_WebIDL_initFinalParseResult())
}

NH_WEBIDL_RESULT Nh_WebIDL_createFragment(
    NH_BYTE *spec_p, NH_BYTE *name_p, Nh_WebIDL_Fragment *Fragment_p)
{
NH_WEBIDL_BEGIN()

    NH_BYTE qualifiedName_p[255] = {'\0'};
    sprintf(qualifiedName_p, "%s_%s", spec_p, name_p);

    Nh_WebIDL_FinalParseResult ParseResult = Nh_WebIDL_parseBuiltinFragment(qualifiedName_p);

    if (ParseResult.Root_p != NULL) {
        Fragment_p->builtin = NH_TRUE; 
        Fragment_p->ParseResult = ParseResult;
        strcpy(Fragment_p->qualifiedName_p, qualifiedName_p);
        NH_WEBIDL_DIAGNOSTIC_END(NH_WEBIDL_SUCCESS)
    }

NH_WEBIDL_DIAGNOSTIC_END(NH_WEBIDL_ERROR_BAD_STATE)
}


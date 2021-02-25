// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "Serializer.h"
#include "Lexer.h"

#include "../Common/Macro.h"
#include NH_WEBIDL_FLOW
#include NH_WEBIDL_DEFAULT_CHECK

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

// SERIALIZE =======================================================================================

NH_WEBIDL_RESULT Nh_WebIDL_serialize(
    char *inFilepath_p, char *outFilepath_p, void (*callback_f)(char *message_p))
{
NH_WEBIDL_BEGIN()

    Nh_WebIDL_FragmentToken FragmentToken = Nh_WebIDL_tokenizeFile(inFilepath_p);
    if (FragmentToken.definitionCount > 0) {
        int length = 0;
        Nh_WebIDL_stringifyFragmentToken(FragmentToken, NULL, &length);
        char *str_p = malloc(sizeof(char) * (length + 1));
        Nh_WebIDL_stringifyFragmentToken(FragmentToken, str_p, NULL);
        callback_f(str_p);
        free(str_p);
    }

NH_WEBIDL_DIAGNOSTIC_END(NH_WEBIDL_SUCCESS)
}


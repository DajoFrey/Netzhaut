// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE ========================================================================================

#include "Type.h"

#include "../Common/Macro.h"
#include NH_ECMASCRIPT_CUSTOM_CHECK
#include NH_ECMASCRIPT_FLOW

#include "../../NhCore/Memory.h"
#include "../../NhCore/Unicode.h"

#include <string.h>

// MISC ============================================================================================

Nh_ECMAScript_Any Nh_ECMAScript_initAny(
    NH_ECMASCRIPT_TYPE type, void *handle_p)
{
NH_ECMASCRIPT_BEGIN()

    Nh_ECMAScript_Any Any;
    Any.type = type;
    Any.handle_p = handle_p;

NH_ECMASCRIPT_END(Any)
}

Nh_ECMAScript_Any Nh_ECMAScript_wrapUndefined()
{
NH_ECMASCRIPT_BEGIN()
NH_ECMASCRIPT_END(Nh_ECMAScript_initAny(NH_ECMASCRIPT_TYPE_UNDEFINED, NULL))
}

Nh_ECMAScript_Any Nh_ECMAScript_wrapNull()
{
NH_ECMASCRIPT_BEGIN()
NH_ECMASCRIPT_END(Nh_ECMAScript_initAny(NH_ECMASCRIPT_TYPE_NULL, NULL))
}

Nh_ECMAScript_Any Nh_ECMAScript_wrapBoolean(
    NH_ECMASCRIPT_BOOLEAN boolean)
{
NH_ECMASCRIPT_BEGIN()
NH_ECMASCRIPT_END(Nh_ECMAScript_initAny(NH_ECMASCRIPT_TYPE_BOOLEAN, (void*)boolean))
}

Nh_ECMAScript_Any Nh_ECMAScript_wrapString(
    Nh_ECMAScript_String *String_p)
{
NH_ECMASCRIPT_BEGIN()
NH_ECMASCRIPT_END(Nh_ECMAScript_initAny(NH_ECMASCRIPT_TYPE_STRING, String_p))
}

Nh_ECMAScript_Any Nh_ECMAScript_wrapObject(
    Nh_ECMAScript_Object *Object_p)
{
NH_ECMASCRIPT_BEGIN()
NH_ECMASCRIPT_END(Nh_ECMAScript_initAny(NH_ECMASCRIPT_TYPE_OBJECT, Object_p))
}

Nh_ECMAScript_String *Nh_ECMAScript_allocateString(
    size_t chunkSize)
{
NH_ECMASCRIPT_BEGIN()

    Nh_ECMAScript_String *String_p = Nh_allocate(sizeof(Nh_ECMAScript_String));
    NH_ECMASCRIPT_CHECK_MEM(NULL, String_p)

    *String_p = Nh_initString(chunkSize);

NH_ECMASCRIPT_END(String_p)
}

NH_ECMASCRIPT_RESULT Nh_ECMAScript_appendFormatToString(
    Nh_ECMAScript_String *String_p, NH_BYTE *bytes_p, size_t len)
{
NH_ECMASCRIPT_BEGIN()

    NH_RESULT result = Nh_appendFormatToString(String_p, bytes_p, len);

NH_ECMASCRIPT_END(NH_ECMASCRIPT_SUCCESS)
}

Nh_ECMAScript_Any Nh_ECMAScript_wrapNumber(
    NH_ECMASCRIPT_NUMBER *number_p)
{
NH_ECMASCRIPT_BEGIN()
NH_ECMASCRIPT_END(Nh_ECMAScript_initAny(NH_ECMASCRIPT_TYPE_NUMBER, number_p))
}

NH_ECMASCRIPT_NUMBER *Nh_ECMAScript_allocateNumber(
    NH_ECMASCRIPT_NUMBER number)
{
NH_ECMASCRIPT_BEGIN()

    NH_ECMASCRIPT_NUMBER *number_p = Nh_allocate(sizeof(NH_ECMASCRIPT_NUMBER));
    NH_ECMASCRIPT_CHECK_MEM(NULL, number_p)

    *number_p = number;

NH_ECMASCRIPT_END(number_p)
}


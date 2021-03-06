// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "Type.h"

#include "../Common/Macro.h"
#include NH_WEB_FLOW
#include NH_WEB_CUSTOM_CHECK

#include "../../NhCore/Memory.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// CREATE ==========================================================================================

NH_WEB_TYPE Nh_Web_getType(
    Nh_Web_ParseNode *Type_p)
{
NH_WEB_BEGIN()

    if (!Type_p || Type_p->type != NH_WEB_PARSE_NODE_TYPE) {NH_WEB_END(-1)}

    if (((Nh_Web_ParseNode*)Type_p->Children.pp[0])->type == NH_WEB_PARSE_NODE_SINGLE_TYPE)
    {
        Nh_Web_ParseNode *SingleType_p = Type_p->Children.pp[0];

        if (SingleType_p->Token_p != NULL && SingleType_p->Token_p->String.bytes_p[0] == '*') {
            NH_WEB_END(NH_WEB_TYPE_WILDCARD)
        }
        if (SingleType_p->Token_p != NULL && !strcmp(SingleType_p->Token_p->String.bytes_p, "any")) {
            NH_WEB_END(NH_WEB_TYPE_ANY)
        }

        if (SingleType_p->Children.size > 0) {
            if (((Nh_Web_ParseNode*)SingleType_p->Children.pp[0])->type == NH_WEB_PARSE_NODE_DISTINGUISHABLE_TYPE)
            {
                Nh_Web_ParseNode *DistinguishableType_p = SingleType_p->Children.pp[0];
    
                if (DistinguishableType_p->Children.size > 0) {      
                    if (((Nh_Web_ParseNode*)DistinguishableType_p->Children.pp[0])->type == NH_WEB_PARSE_NODE_PRIMITIVE_TYPE)
                    {
                        Nh_Web_ParseNode *PrimitiveType_p = DistinguishableType_p->Children.pp[0];

                        if (PrimitiveType_p->Token_p != NULL && !strcmp(PrimitiveType_p->Token_p->String.bytes_p, "undefined")) {
                            NH_WEB_END(NH_WEB_TYPE_UNDEFINED)
                        }
                        if (PrimitiveType_p->Token_p != NULL && !strcmp(PrimitiveType_p->Token_p->String.bytes_p, "boolean")) {
                            NH_WEB_END(NH_WEB_TYPE_BOOLEAN)
                        }
                        if (PrimitiveType_p->Token_p != NULL && !strcmp(PrimitiveType_p->Token_p->String.bytes_p, "byte")) {
                            NH_WEB_END(NH_WEB_TYPE_BYTE)
                        }
                        if (PrimitiveType_p->Token_p != NULL && !strcmp(PrimitiveType_p->Token_p->String.bytes_p, "octet")) {
                            NH_WEB_END(NH_WEB_TYPE_OCTET)
                        }
                        if (PrimitiveType_p->Token_p != NULL && !strcmp(PrimitiveType_p->Token_p->String.bytes_p, "bigint")) {
                            NH_WEB_END(NH_WEB_TYPE_BIGINT)
                        }

                    }
                }
            }
        }

    }
    else {

    }

NH_WEB_END(-1)
}

size_t Nh_Web_getAllocationSize(
    NH_WEB_TYPE type)
{
NH_WEB_BEGIN()

    size_t typeSize = 0;

    switch (type)
    {
        case NH_WEB_TYPE_WILDCARD            : typeSize = 0; break;
        case NH_WEB_TYPE_ANY                 : typeSize = sizeof(Nh_Web_Any); break;
        case NH_WEB_TYPE_UNDEFINED           : typeSize = 0; break;
        case NH_WEB_TYPE_BOOLEAN             : typeSize = 0; break;
        case NH_WEB_TYPE_BYTE                : typeSize = sizeof(NH_WEB_BYTE); break;
        case NH_WEB_TYPE_OCTET               : typeSize = sizeof(NH_WEB_OCTET); break;
        case NH_WEB_TYPE_SHORT               : typeSize = sizeof(NH_WEB_SHORT); break;
        case NH_WEB_TYPE_UNSIGNED_SHORT      : typeSize = sizeof(NH_WEB_UNSIGNED_SHORT); break;
        case NH_WEB_TYPE_LONG                : typeSize = sizeof(NH_WEB_LONG); break;
        case NH_WEB_TYPE_UNSIGNED_LONG       : typeSize = sizeof(NH_WEB_UNSIGNED_LONG); break;
        case NH_WEB_TYPE_LONG_LONG           : typeSize = sizeof(NH_WEB_LONG_LONG); break;
        case NH_WEB_TYPE_UNSIGNED_LONG_LONG  : typeSize = sizeof(NH_WEB_UNSIGNED_LONG_LONG); break; 
        case NH_WEB_TYPE_FLOAT               : typeSize = sizeof(NH_WEB_FLOAT); break;
        case NH_WEB_TYPE_UNRESTRICTED_FLOAT  : typeSize = sizeof(NH_WEB_UNRESTRICTED_FLOAT); break;
        case NH_WEB_TYPE_DOUBLE              : typeSize = sizeof(NH_WEB_DOUBLE); break;
        case NH_WEB_TYPE_UNRESTRICTED_DOUBLE : typeSize = sizeof(NH_WEB_UNRESTRICTED_DOUBLE); break;
        case NH_WEB_TYPE_BIGINT              : typeSize = sizeof(Nh_Web_BigInt); break;
        case NH_WEB_TYPE_DOMSTRING           : typeSize = sizeof(Nh_Web_DOMString); break;
        case NH_WEB_TYPE_BYTESTRING          : typeSize = sizeof(Nh_Web_ByteString); break;
        case NH_WEB_TYPE_USVSTRING           : typeSize = sizeof(Nh_Web_USVString); break;
        case NH_WEB_TYPE_OBJECT              : typeSize = 0; break;
        case NH_WEB_TYPE_SYMBOL              : typeSize = 0; break;
    }

NH_WEB_END(typeSize > sizeof(void*) ? typeSize : 0)
}


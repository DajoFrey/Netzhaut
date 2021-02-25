// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "Serializer.h"
#include "Util.h"

#include "../Common/Macro.h"
#include NH_WEBIDL_FLOW
#include NH_WEBIDL_DEFAULT_CHECK

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// BUFFER ==========================================================================================

static void Nh_WebIDL_initSerializationBuffer(
    Nh_WebIDL_SerializationBuffer *Buffer_p)
{
NH_WEBIDL_BEGIN()

    Buffer_p->size    = 0;
    Buffer_p->index   = -1;
    Buffer_p->maxSize = 20000;

    for (int i = 0; i < 1024; ++i) {Buffer_p->array_pp[i] = NULL;}

NH_WEBIDL_SILENT_END()
}

static void Nh_WebIDL_freeSerializationBuffer(
    Nh_WebIDL_SerializationBuffer *Buffer_p)
{
NH_WEBIDL_BEGIN()

    for (int i = 0; i < 1024; ++i) {
        if (Buffer_p->array_pp[i] != NULL) {
            free(Buffer_p->array_pp[i]);
        }
    }

NH_WEBIDL_SILENT_END()
}

static NH_WEBIDL_RESULT Nh_WebIDL_addToSerializationBuffer(
    Nh_WebIDL_SerializationBuffer *Buffer_p, char *data_p, int dataSize)
{
NH_WEBIDL_BEGIN()

    char *buffer_p = Buffer_p->index >= 0 ? Buffer_p->array_pp[Buffer_p->index] : NULL;
    int dataIndex = 0;

    while (dataSize > 0) 
    {
        if (Buffer_p->size == 0) {
            Buffer_p->array_pp[++Buffer_p->index] = malloc(sizeof(char) * Buffer_p->maxSize);
            Buffer_p->size = Buffer_p->maxSize;
            buffer_p = Buffer_p->array_pp[Buffer_p->index];
        }

        buffer_p[Buffer_p->maxSize - Buffer_p->size] = data_p[dataIndex];
        dataIndex++;
        Buffer_p->size--;
        dataSize--;
    }

NH_WEBIDL_END(NH_WEBIDL_SUCCESS)
}

// SERIALIZE =======================================================================================

static NH_WEBIDL_RESULT Nh_WebIDL_serializeInterface(
    Nh_WebIDL_SerializationBuffer *Buffer_p, Nh_WebIDL_Interface *Interface_p)
{
NH_WEBIDL_BEGIN()

    char empty = '\0';

    Nh_WebIDL_addToSerializationBuffer(
        Buffer_p, Interface_p->name_p, sizeof(char) * (strlen(Interface_p->name_p) + 1)
    );
    Nh_WebIDL_addToSerializationBuffer(
        Buffer_p,
        Interface_p->inherit_p == NULL ? &empty : Interface_p->inherit_p, 
        Interface_p->inherit_p == NULL ? 1 : sizeof(char) * (strlen(Interface_p->inherit_p) + 1)
    );
    Nh_WebIDL_addToSerializationBuffer(Buffer_p, (char*)&Interface_p->Attributes.length, sizeof(int));

    for (int j = 0; j < Interface_p->Attributes.length; ++j) 
    {
        Nh_WebIDL_Attribute *Attribute_p = &((Nh_WebIDL_Attribute*)Interface_p->Attributes.bytes_p)[j];
        Nh_WebIDL_addToSerializationBuffer(Buffer_p, (char*)&Attribute_p->_static, sizeof(NH_BOOL));
        Nh_WebIDL_addToSerializationBuffer(Buffer_p, (char*)&Attribute_p->readonly, sizeof(NH_BOOL));
        Nh_WebIDL_addToSerializationBuffer(Buffer_p, (char*)&Attribute_p->stringifier, sizeof(NH_BOOL));
        Nh_WebIDL_addToSerializationBuffer(
            Buffer_p, Attribute_p->name_p, sizeof(char) * (strlen(Attribute_p->name_p) + 1)
        );
    } 

    for (int j = 0; j < Interface_p->Operations.length; ++j) 
    {
        Nh_WebIDL_Operation *Operation_p = &((Nh_WebIDL_Operation*)Interface_p->Operations.bytes_p)[j];
        Nh_WebIDL_addToSerializationBuffer(
            Buffer_p, Operation_p->name_p, sizeof(char) * (strlen(Operation_p->name_p) + 1)
        );
    }

NH_WEBIDL_END(NH_WEBIDL_SUCCESS)
}

static NH_WEBIDL_RESULT Nh_WebIDL_serializeFragment(
    Nh_WebIDL_SerializationBuffer *Buffer_p, Nh_WebIDL_Fragment Fragment)
{
NH_WEBIDL_BEGIN()

    Nh_WebIDL_addToSerializationBuffer(Buffer_p, (char*)&Fragment.definitionCount, sizeof(int));

    for (int i = 0; i < Fragment.definitionCount; ++i)
    {
        Nh_WebIDL_addToSerializationBuffer(Buffer_p, (char*)&Fragment.Definitions_p[i].type, sizeof(NH_WEBIDL_DEFINITION));

        switch (Fragment.Definitions_p[i].type)
        {
            case NH_WEBIDL_DEFINITION_CALLBACK           : break; 
            case NH_WEBIDL_DEFINITION_CALLBACK_INTERFACE : break;
            case NH_WEBIDL_DEFINITION_INTERFACE          : 
            {
                NH_WEBIDL_CHECK(Nh_WebIDL_serializeInterface(Buffer_p, Fragment.Definitions_p[i].data_p))
                break;
            }
            case NH_WEBIDL_DEFINITION_INTERFACE_MIXIN    : break; 
            case NH_WEBIDL_DEFINITION_MIXIN              : break;
            case NH_WEBIDL_DEFINITION_NAMESPACE          : break;
            case NH_WEBIDL_DEFINITION_PARTIAL            : break;
            case NH_WEBIDL_DEFINITION_DICTIONARY         : break; 
            case NH_WEBIDL_DEFINITION_ENUM               : break;
            case NH_WEBIDL_DEFINITION_TYPEDEF            : break;
            case NH_WEBIDL_DEFINITION_INCLUDES_STATEMENT : break;
        }
    }

NH_WEBIDL_END(NH_WEBIDL_SUCCESS)
}

Nh_WebIDL_SerializationData Nh_WebIDL_serialize(
    char *infilepath_p, void (*callback_f)(char *message_p))
{
NH_WEBIDL_BEGIN()

    Nh_WebIDL_SerializationData Data;

    Data.FragmentToken = Nh_WebIDL_tokenizeFile(infilepath_p);
    Data.Fragment = Nh_WebIDL_parseFragmentToken(Data.FragmentToken);

    Nh_WebIDL_initSerializationBuffer(&Data.Buffer);
    Nh_WebIDL_serializeFragment(&Data.Buffer, Data.Fragment);

NH_WEBIDL_END(Data)
}

void Nh_WebIDL_freeSerializationData(
    Nh_WebIDL_SerializationData Data)
{
NH_WEBIDL_BEGIN()

   Nh_WebIDL_freeSerializationBuffer(&Data.Buffer);
   Nh_WebIDL_unparseFragment(Data.Fragment);
   Nh_WebIDL_untokenize(Data.FragmentToken);
 
NH_WEBIDL_SILENT_END()
}


#ifndef NH_WEBIDL_SERIALIZER_TOKENIZER_H
#define NH_WEBIDL_SERIALIZER_TOKENIZER_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#endif

/** @addtogroup NhWebIDLSerializerEnums
 *  @{
 */

    typedef enum NH_WEBIDL_SERIALIZER_DEFINITION {
        NH_WEBIDL_SERIALIZER_DEFINITION_CALLBACK,
        NH_WEBIDL_SERIALIZER_DEFINITION_CALLBACK_INTERFACE,
        NH_WEBIDL_SERIALIZER_DEFINITION_INTERFACE,
        NH_WEBIDL_SERIALIZER_DEFINITION_INTERFACE_MIXIN,
        NH_WEBIDL_SERIALIZER_DEFINITION_MIXIN,
        NH_WEBIDL_SERIALIZER_DEFINITION_NAMESPACE,
        NH_WEBIDL_SERIALIZER_DEFINITION_PARTIAL,
        NH_WEBIDL_SERIALIZER_DEFINITION_DICTIONARY,
        NH_WEBIDL_SERIALIZER_DEFINITION_ENUM,
        NH_WEBIDL_SERIALIZER_DEFINITION_TYPEDEF,
        NH_WEBIDL_SERIALIZER_DEFINITION_INCLUDES_STATEMENT,
    } NH_WEBIDL_SERIALIZER_DEFINITION;

/** @} */

/** @addtogroup NhWebIDLSerializerStructs
 *  @{
 */

    typedef struct Nh_WebIDLSerializer_CallbackToken {
        char *name_p;
        char *type_p;
        int argumentCount;
        char **arguments_pp; 
    } Nh_WebIDLSerializer_CallbackToken;

    typedef struct Nh_WebIDLSerializer_EnumToken {
        char *name_p;
        int valueCount;
        char **values_pp; 
    } Nh_WebIDLSerializer_EnumToken;

    typedef struct Nh_WebIDLSerializer_IncludesStatementToken {
        char *left_p;
        char *right_p; 
    } Nh_WebIDLSerializer_IncludesStatementToken;

    typedef struct Nh_WebIDLSerializer_InterfaceMixinOrCallbackInterfaceToken {
        char *name_p;
        int memberCount;
        char **members_pp; 
    } Nh_WebIDLSerializer_InterfaceMixinOrCallbackInterfaceToken;

    typedef struct Nh_WebIDLSerializer_InterfaceOrDictionaryToken {
        char *name_p;
        char *inherit_p;
        int memberCount;
        char **members_pp; 
    } Nh_WebIDLSerializer_InterfaceOrDictionaryToken;

    typedef struct Nh_WebIDLSerializer_DefinitionToken {
        NH_WEBIDL_SERIALIZER_DEFINITION type;
        char *extendedAttributeList_p;
        void *data_p;
    } Nh_WebIDLSerializer_DefinitionToken;

    typedef struct Nh_WebIDLSerializer_FragmentToken {
        int definitionCount;
        Nh_WebIDLSerializer_DefinitionToken **Definitions_pp;
    } Nh_WebIDLSerializer_FragmentToken;

/** @} */

/** @addtogroup WebIDLSerializerFunctions
 *  @{
 */

    Nh_WebIDLSerializer_FragmentToken Nh_WebIDLSerializer_tokenizeFile(
        const char *filepath_p
    );

    void Nh_WebIDLSerializer_stringifyFragmentToken(
        Nh_WebIDLSerializer_FragmentToken Fragment, char *str_p, int *length_p
    );

    void Nh_WebIDLSerializer_untokenize(
        Nh_WebIDLSerializer_FragmentToken Token
    );

/** @} */

#endif

#ifndef NH_WEBIDL_LEXER_H
#define NH_WEBIDL_LEXER_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#endif

/** @addtogroup WebIDLEnums Enums
 *  \ingroup WebIDL
 *  @{
 */

    typedef enum NH_WEBIDL_DEFINITION {
        NH_WEBIDL_DEFINITION_CALLBACK,
        NH_WEBIDL_DEFINITION_CALLBACK_INTERFACE,
        NH_WEBIDL_DEFINITION_INTERFACE,
        NH_WEBIDL_DEFINITION_INTERFACE_MIXIN,
        NH_WEBIDL_DEFINITION_MIXIN,
        NH_WEBIDL_DEFINITION_NAMESPACE,
        NH_WEBIDL_DEFINITION_PARTIAL,
        NH_WEBIDL_DEFINITION_DICTIONARY,
        NH_WEBIDL_DEFINITION_ENUM,
        NH_WEBIDL_DEFINITION_TYPEDEF,
        NH_WEBIDL_DEFINITION_INCLUDES_STATEMENT,
    } NH_WEBIDL_DEFINITION;

/** @} */

/** @addtogroup WebIDLStructs Structs
 *  \ingroup WebIDL
 *  @{
 */

    typedef struct Nh_WebIDL_CallbackToken {
        char *name_p;
        char *type_p;
        int argumentCount;
        char **arguments_pp; 
    } Nh_WebIDL_CallbackToken;

    typedef struct Nh_WebIDL_EnumToken {
        char *name_p;
        int valueCount;
        char **values_pp; 
    } Nh_WebIDL_EnumToken;

    typedef struct Nh_WebIDL_IncludesStatementToken {
        char *left_p;
        char *right_p; 
    } Nh_WebIDL_IncludesStatementToken;

    typedef struct Nh_WebIDL_InterfaceMixinOrCallbackInterfaceToken {
        char *name_p;
        int memberCount;
        char **members_pp; 
    } Nh_WebIDL_InterfaceMixinOrCallbackInterfaceToken;

    typedef struct Nh_WebIDL_InterfaceOrDictionaryToken {
        char *name_p;
        char *inherit_p;
        int memberCount;
        char **members_pp; 
    } Nh_WebIDL_InterfaceOrDictionaryToken;

    typedef struct Nh_WebIDL_DefinitionToken {
        NH_WEBIDL_DEFINITION type;
        char *extendedAttributeList_p;
        void *data_p;
    } Nh_WebIDL_DefinitionToken;

    typedef struct Nh_WebIDL_FragmentToken {
        int definitionCount;
        Nh_WebIDL_DefinitionToken **Definitions_pp;
    } Nh_WebIDL_FragmentToken;

/** @} */

/** @addtogroup WebIDLStructs Structs
 *  \ingroup WebIDL
 *  @{
 */

    Nh_WebIDL_FragmentToken Nh_WebIDL_tokenizeFile(
        const char *filepath_p
    );

    void Nh_WebIDL_stringifyFragmentToken(
        Nh_WebIDL_FragmentToken Fragment, char *str_p, int *length_p
    );

    void Nh_WebIDL_untokenize(
        Nh_WebIDL_FragmentToken Token
    );

/** @} */

#endif

#ifndef NH_ECMASCRIPT_ENVIRONMENT_H
#define NH_ECMASCRIPT_ENVIRONMENT_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "Parser.h"
#include "ObjectType.h"

#endif

/** @addtogroup NhECMAScriptEnums
 *  @{
 */

    typedef enum NH_ECMASCRIPT_ENVIRONMENT {
        NH_ECMASCRIPT_ENVIRONMENT_DECLARATIVE,
        NH_ECMASCRIPT_ENVIRONMENT_OBJECT,
        NH_ECMASCRIPT_ENVIRONMENT_GLOBAL,
    } NH_ECMASCRIPT_ENVIRONMENT;

/** @} */

/** @addtogroup NhECMAScriptStructs
 *  @{
 */

    typedef struct Nh_ECMAScript_Environment {
        NH_ECMASCRIPT_ENVIRONMENT type;
        void *Handle_p;
        struct Nh_ECMAScript_Environment *Outer_p;
    } Nh_ECMAScript_Environment;

    typedef struct Nh_ECMAScript_Binding {
        NH_BOOL _mutable;
        NH_BOOL deletable;
        NH_BOOL strict;
        NH_BOOL initialized;
        Nh_UTF8String *Name_p;
        Nh_ECMAScript_Any Value;
    } Nh_ECMAScript_Binding;

    typedef struct Nh_ECMAScript_DeclarativeEnvironment {
        Nh_List Bindings;
    } Nh_ECMAScript_DeclarativeEnvironment;

    typedef struct Nh_ECMAScript_ObjectEnvironment {
        Nh_ECMAScript_Object *BindingObject_p;
        NH_ECMASCRIPT_BOOLEAN withEnvironment;
    } Nh_ECMAScript_ObjectEnvironment;

    typedef struct Nh_ECMAScript_GlobalEnvironment {
        Nh_ECMAScript_Environment *ObjectEnvironment_p;
        Nh_ECMAScript_Environment *DeclarativeEnvironment_p;
        Nh_ECMAScript_Object *GlobalThisValue_p;
        Nh_List VarNames;
    } Nh_ECMAScript_GlobalEnvironment;

/** @} */

/** @addtogroup NhECMAScriptFunctions
 *  @{
 */

// NEW

    Nh_ECMAScript_Environment *Nh_ECMAScript_newDeclarativeEnvironment(
        Nh_ECMAScript_Environment *Outer_p
    );

    Nh_ECMAScript_Environment *Nh_ECMAScript_newObjectEnvironment(
        Nh_ECMAScript_Object *Object_p, Nh_ECMAScript_Environment *Outer_p
    );
    
    Nh_ECMAScript_Environment *Nh_ECMAScript_newGlobalEnvironment(
        Nh_ECMAScript_Object *GlobalObject_p, Nh_ECMAScript_Object *ThisValue_p
    );

// MISC

    NH_BOOL Nh_ECMAScript_canDeclareGlobalVar(
        Nh_ECMAScript_GlobalEnvironment *GlobalEnvironment_p, Nh_UTF8String *String_p
    );

    Nh_ECMAScript_Completion Nh_ECMAScript_createGlobalVarBinding(
        Nh_ECMAScript_GlobalEnvironment *GlobalEnvironment_p, Nh_UTF8String *Name_p, NH_ECMASCRIPT_BOOLEAN deletable
    );

    NH_BOOL Nh_ECMAScript_canDeclareGlobalFunction(
        Nh_ECMAScript_GlobalEnvironment *GlobalEnvironment_p, Nh_UTF8String *String_p
    );

    Nh_ECMAScript_Completion Nh_ECMAScript_createGlobalFunctionBinding(
        Nh_UTF8String *Name_p, Nh_ECMAScript_Any V, NH_ECMASCRIPT_BOOLEAN configurable
    );

    NH_BOOL Nh_ECMAScript_hasVarDeclaration(
        Nh_ECMAScript_GlobalEnvironment *GlobalEnvironment_p, Nh_UTF8String *Name_p
    );
    
    NH_BOOL Nh_ECMAScript_hasLexicalDeclaration(
        Nh_ECMAScript_GlobalEnvironment *GlobalEnvironment_p, Nh_UTF8String *Name_p
    );
    
    NH_BOOL Nh_ECMAScript_hasRestrictedGlobalProperty(
        Nh_ECMAScript_GlobalEnvironment *GlobalEnvironment_p, Nh_UTF8String *Name_p
    );

// ABSTRACT

    NH_BOOL Nh_ECMAScript_hasBinding(
        Nh_ECMAScript_Environment *Environment_p, Nh_UTF8String *Name_p
    );

    Nh_ECMAScript_Completion Nh_ECMAScript_createMutableBinding(
        Nh_ECMAScript_Environment *Environment_p, Nh_UTF8String *Name_p, NH_BOOL deletable
    );
    
    Nh_ECMAScript_Completion Nh_ECMAScript_createImmutableBinding(
        Nh_ECMAScript_Environment *Environment_p, Nh_UTF8String *Name_p, NH_BOOL strict
    );
    
    Nh_ECMAScript_Completion Nh_ECMAScript_initializeBinding(
        Nh_ECMAScript_Environment *Environment_p, Nh_UTF8String *Name_p, Nh_ECMAScript_Any Value
    );

    Nh_ECMAScript_Completion Nh_ECMAScript_setMutableBinding(
        Nh_ECMAScript_Environment *Environment_p, Nh_UTF8String *Name_p, Nh_ECMAScript_Any Value, NH_BOOL _throw
    );

/** @} */

#endif

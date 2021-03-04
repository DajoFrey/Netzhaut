// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "Environment.h"
#include "TestAndCompare.h"

#include "../StaticSemantics/BoundNames.h"
#include "../StaticSemantics/DeclaredNames.h"
#include "../StaticSemantics/ScopedDeclarations.h"

#include "../Common/Macro.h"
#include NH_ECMASCRIPT_FLOW
#include NH_ECMASCRIPT_CUSTOM_CHECK

#include "../../NhCore/Memory.h"

#include <string.h>

// DECLARATIVE ENVIRONMENT =========================================================================

Nh_ECMAScript_Environment *Nh_ECMAScript_newDeclarativeEnvironment(
    Nh_ECMAScript_Environment *Outer_p)
{
NH_ECMASCRIPT_BEGIN()

    Nh_ECMAScript_Environment *Environment_p = Nh_allocate(sizeof(Nh_ECMAScript_Environment));
    NH_ECMASCRIPT_CHECK_MEM(NULL, Environment_p)

    Nh_ECMAScript_DeclarativeEnvironment *DeclarativeEnvironment_p = Nh_allocate(sizeof(Nh_ECMAScript_DeclarativeEnvironment));
    NH_ECMASCRIPT_CHECK_MEM(NULL, DeclarativeEnvironment_p)

    DeclarativeEnvironment_p->Bindings = Nh_initList(16);

    Environment_p->Handle_p = DeclarativeEnvironment_p;
    Environment_p->Outer_p  = Outer_p;
    Environment_p->type     = NH_ECMASCRIPT_ENVIRONMENT_DECLARATIVE;

NH_ECMASCRIPT_END(Environment_p)
}

static NH_BOOL Nh_ECMAScript_declarativeEnvironmentHasBinding(
    Nh_ECMAScript_DeclarativeEnvironment *DeclarativeEnvironment_p, Nh_UTF8String *Name_p)
{
NH_ECMASCRIPT_BEGIN()

    for (int i = 0; i < DeclarativeEnvironment_p->Bindings.size; ++i) {
        Nh_ECMAScript_Binding *Binding_p = DeclarativeEnvironment_p->Bindings.pp[i];
        if (!strcmp(Binding_p->Name_p->bytes_p, Name_p->bytes_p)) {NH_ECMASCRIPT_END(NH_TRUE)}
    }

NH_ECMASCRIPT_END(NH_FALSE)
}

static Nh_ECMAScript_Completion Nh_ECMAScript_declarativeEnvironmentCreateMutableBinding(
    Nh_ECMAScript_DeclarativeEnvironment *DeclarativeEnvironment_p, Nh_UTF8String *Name_p, NH_BOOL deletable)
{
NH_ECMASCRIPT_BEGIN()

    if (Nh_ECMAScript_declarativeEnvironmentHasBinding(DeclarativeEnvironment_p, Name_p)) {
        NH_ECMASCRIPT_END(Nh_ECMAScript_throwTypeError())
    }

    Nh_ECMAScript_Binding *NewBinding_p = Nh_allocate(sizeof(Nh_ECMAScript_Binding));
    NewBinding_p->_mutable    = NH_TRUE;
    NewBinding_p->deletable   = deletable;
    NewBinding_p->initialized = NH_FALSE;
    NewBinding_p->Name_p      = Name_p;
    NewBinding_p->Value       = Nh_ECMAScript_wrapUndefined();

    Nh_appendToList(&DeclarativeEnvironment_p->Bindings, NewBinding_p);

NH_ECMASCRIPT_END(Nh_ECMAScript_normalEmptyCompletion())
}

static Nh_ECMAScript_Completion Nh_ECMAScript_declarativeEnvironmentCreateImmutableBinding(
    Nh_ECMAScript_DeclarativeEnvironment *DeclarativeEnvironment_p, Nh_UTF8String *Name_p, NH_BOOL strict)
{
NH_ECMASCRIPT_BEGIN()

    if (Nh_ECMAScript_declarativeEnvironmentHasBinding(DeclarativeEnvironment_p, Name_p)) {
        NH_ECMASCRIPT_END(Nh_ECMAScript_throwTypeError())
    }

    Nh_ECMAScript_Binding *NewBinding_p = Nh_allocate(sizeof(Nh_ECMAScript_Binding));
    NewBinding_p->_mutable    = NH_FALSE;
    NewBinding_p->strict      = strict;
    NewBinding_p->initialized = NH_FALSE;
    NewBinding_p->Name_p      = Name_p;
    NewBinding_p->Value       = Nh_ECMAScript_wrapUndefined();

    Nh_appendToList(&DeclarativeEnvironment_p->Bindings, NewBinding_p);

NH_ECMASCRIPT_END(Nh_ECMAScript_normalEmptyCompletion())
}

static Nh_ECMAScript_Completion Nh_ECMAScript_declarativeEnvironmentInitializeBinding(
    Nh_ECMAScript_DeclarativeEnvironment *DeclarativeEnvironment_p, Nh_UTF8String *Name_p, Nh_ECMAScript_Any Value)
{
NH_ECMASCRIPT_BEGIN()

    Nh_ECMAScript_Binding *Binding_p = NULL;
    for (int i = 0; i < DeclarativeEnvironment_p->Bindings.size; ++i) {
        Binding_p = DeclarativeEnvironment_p->Bindings.pp[i];
        if (!strcmp(Binding_p->Name_p->bytes_p, Name_p->bytes_p)) {break;}
    }

    if (Binding_p == NULL) {NH_ECMASCRIPT_END(Nh_ECMAScript_throwTypeError())}

    if (!Binding_p->initialized) {
        Binding_p->Value = Value;
        Binding_p->initialized = NH_TRUE;
    }

NH_ECMASCRIPT_END(Nh_ECMAScript_normalEmptyCompletion())
}

// OBJECT ENVIRONMENT ==============================================================================

Nh_ECMAScript_Environment *Nh_ECMAScript_newObjectEnvironment(
    Nh_ECMAScript_Object *Object_p, Nh_ECMAScript_Environment *Outer_p)
{
NH_ECMASCRIPT_BEGIN()

    Nh_ECMAScript_Environment *Environment_p = Nh_allocate(sizeof(Nh_ECMAScript_Environment));
    NH_ECMASCRIPT_CHECK_MEM(NULL, Environment_p)

    Nh_ECMAScript_ObjectEnvironment *ObjectEnvironment_p = Nh_allocate(sizeof(Nh_ECMAScript_ObjectEnvironment));
    NH_ECMASCRIPT_CHECK_MEM(NULL, ObjectEnvironment_p)

    ObjectEnvironment_p->BindingObject_p = Object_p;
    ObjectEnvironment_p->withEnvironment = NH_FALSE;

    Environment_p->Handle_p = ObjectEnvironment_p;
    Environment_p->Outer_p = Outer_p;
    Environment_p->type = NH_ECMASCRIPT_ENVIRONMENT_OBJECT;

NH_ECMASCRIPT_END(Environment_p)
}

static Nh_ECMAScript_Completion Nh_ECMAScript_objectEnvironmentCreateMutableBinding(
    Nh_ECMAScript_ObjectEnvironment *ObjectEnvironment_p, Nh_UTF8String *Name_p, NH_BOOL deletable)
{
NH_ECMASCRIPT_BEGIN()

    Nh_ECMAScript_Object *Bindings_p = ObjectEnvironment_p->BindingObject_p;

    Nh_ECMAScript_PropertyDescriptor Descriptor;
    Descriptor.type = NH_ECMASCRIPT_PROPERTY_DATA;
    Descriptor.enumerable   = NH_TRUE;
    Descriptor.configurable = deletable;
    Descriptor.Fields.Data.writable = NH_TRUE;
    Descriptor.Fields.Data.Value    = Nh_ECMAScript_wrapUndefined();

NH_ECMASCRIPT_END(Nh_ECMAScript_abstractDefinePropertyOrThrow(Bindings_p, Nh_ECMAScript_wrapString(Name_p), Descriptor))
}

static Nh_ECMAScript_Completion Nh_ECMAScript_objectEnvironmentSetMutableBinding(
    Nh_ECMAScript_ObjectEnvironment *ObjectEnvironment_p, Nh_UTF8String *Name_p, Nh_ECMAScript_Any Value, NH_BOOL _throw)
{
NH_ECMASCRIPT_BEGIN()

    Nh_ECMAScript_Object *Bindings_p = ObjectEnvironment_p->BindingObject_p;
    NH_BOOL stillExists = Nh_ECMAScript_abstractHasProperty(Bindings_p, Nh_ECMAScript_wrapString(Name_p));
    if (!stillExists && _throw) {NH_ECMASCRIPT_END(Nh_ECMAScript_throwReferenceError())}

NH_ECMASCRIPT_END(Nh_ECMAScript_abstractSet(Bindings_p, Nh_ECMAScript_wrapString(Name_p), Value, _throw))
}

static Nh_ECMAScript_Completion Nh_ECMAScript_objectEnvironmentInitializeBinding(
    Nh_ECMAScript_ObjectEnvironment *ObjectEnvironment_p, Nh_UTF8String *Name_p, Nh_ECMAScript_Any Value)
{
NH_ECMASCRIPT_BEGIN()
NH_ECMASCRIPT_END(Nh_ECMAScript_objectEnvironmentSetMutableBinding(ObjectEnvironment_p, Name_p, Value, NH_FALSE))
}

static NH_BOOL Nh_ECMAScript_objectEnvironmentHasBinding(
    Nh_ECMAScript_ObjectEnvironment *ObjectEnvironment_p, Nh_UTF8String *Name_p)
{
NH_ECMASCRIPT_BEGIN()

    Nh_ECMAScript_Object *Bindings_p = ObjectEnvironment_p->BindingObject_p;
    NH_BOOL foundBinding = Nh_ECMAScript_abstractHasProperty(Bindings_p, Nh_ECMAScript_wrapString(Name_p));
    if (!foundBinding) {NH_ECMASCRIPT_END(NH_FALSE)}
    if (!ObjectEnvironment_p->withEnvironment) {NH_ECMASCRIPT_END(NH_FALSE)}

// Let unscopables be ? Get(bindings, @@unscopables).
// If Type(unscopables) is Object, then
// 
//     Let blocked be ! ToBoolean(? Get(unscopables, N)).
//     If blocked is true, return false.

NH_ECMASCRIPT_END(NH_TRUE)
}

// GLOBAL ENVIRONMENT ==============================================================================

Nh_ECMAScript_Environment *Nh_ECMAScript_newGlobalEnvironment(
    Nh_ECMAScript_Object *GlobalObject_p, Nh_ECMAScript_Object *ThisValue_p)
{
NH_ECMASCRIPT_BEGIN()

    Nh_ECMAScript_Environment *Abstract_p = Nh_allocate(sizeof(Nh_ECMAScript_Environment));
    NH_ECMASCRIPT_CHECK_MEM(NULL, Abstract_p)

    Nh_ECMAScript_Environment *ObjectEnv_p = Nh_ECMAScript_newObjectEnvironment(GlobalObject_p, NULL);
    NH_ECMASCRIPT_CHECK_MEM(NULL, ObjectEnv_p)

    Nh_ECMAScript_Environment *DeclarativeEnv_p = Nh_ECMAScript_newDeclarativeEnvironment(NULL);
    NH_ECMASCRIPT_CHECK_MEM(NULL, DeclarativeEnv_p)

    Nh_ECMAScript_GlobalEnvironment *GlobalEnv_p = Nh_allocate(sizeof(Nh_ECMAScript_GlobalEnvironment));
    NH_ECMASCRIPT_CHECK_MEM(NULL, GlobalEnv_p)

    GlobalEnv_p->ObjectEnvironment_p = ObjectEnv_p;
    GlobalEnv_p->GlobalThisValue_p = ThisValue_p;
    GlobalEnv_p->DeclarativeEnvironment_p = DeclarativeEnv_p;
    GlobalEnv_p->VarNames = Nh_initList(32);

    Abstract_p->Handle_p = GlobalEnv_p;
    Abstract_p->Outer_p = NULL;
    Abstract_p->type = NH_ECMASCRIPT_ENVIRONMENT_GLOBAL;

NH_ECMASCRIPT_END(Abstract_p)
}

NH_BOOL Nh_ECMAScript_canDeclareGlobalFunction(
    Nh_ECMAScript_GlobalEnvironment *GlobalEnvironment_p, Nh_UTF8String *Name_p)
{
NH_ECMASCRIPT_BEGIN()

    Nh_ECMAScript_ObjectEnvironment *ObjectEnvironment_p = GlobalEnvironment_p->ObjectEnvironment_p->Handle_p;
    Nh_ECMAScript_Object *GlobalObject_p = ObjectEnvironment_p->BindingObject_p;

    Nh_ECMAScript_PropertyDescriptor Descriptor = 
        GlobalObject_p->InternalMethods_p->getOwnProperty_f(GlobalObject_p, Nh_ECMAScript_wrapString(Name_p));

    if (Descriptor.type == -1) {

    }
    if (Descriptor.configurable) {NH_ECMASCRIPT_END(NH_TRUE)}
    if (Nh_ECMAScript_isDataDescriptor(Descriptor)) {
        if (Descriptor.Fields.Data.writable && Descriptor.enumerable) {
            NH_ECMASCRIPT_END(NH_TRUE)
        } 
    }

NH_ECMASCRIPT_END(NH_FALSE)
}

Nh_ECMAScript_Completion Nh_ECMAScript_createGlobalFunctionBinding(
    Nh_UTF8String *Name_p, Nh_ECMAScript_Any V, NH_ECMASCRIPT_BOOLEAN configurable)
{
NH_ECMASCRIPT_BEGIN()

    // TODO

NH_ECMASCRIPT_END(Nh_ECMAScript_normalEmptyCompletion())
}

NH_BOOL Nh_ECMAScript_canDeclareGlobalVar(
    Nh_ECMAScript_GlobalEnvironment *GlobalEnvironment_p, Nh_UTF8String *Name_p)
{
NH_ECMASCRIPT_BEGIN()

    Nh_ECMAScript_ObjectEnvironment *ObjectEnvironment_p = GlobalEnvironment_p->ObjectEnvironment_p->Handle_p;
    Nh_ECMAScript_Object *GlobalObject_p = ObjectEnvironment_p->BindingObject_p;

    NH_BOOL hasProperty = Nh_ECMAScript_abstractHasOwnProperty(GlobalObject_p, Nh_ECMAScript_wrapString(Name_p));

    if (hasProperty) {NH_ECMASCRIPT_END(NH_TRUE)}

NH_ECMASCRIPT_END((NH_BOOL)Nh_ECMAScript_isExtensible(GlobalObject_p).Value.Payload.handle_p)
}

Nh_ECMAScript_Completion Nh_ECMAScript_createGlobalVarBinding(
    Nh_ECMAScript_GlobalEnvironment *GlobalEnvironment_p, Nh_UTF8String *Name_p, NH_ECMASCRIPT_BOOLEAN deletable)
{
NH_ECMASCRIPT_BEGIN()

    Nh_ECMAScript_ObjectEnvironment *ObjectEnvironment_p = GlobalEnvironment_p->ObjectEnvironment_p->Handle_p;
    Nh_ECMAScript_Object *GlobalObject_p = ObjectEnvironment_p->BindingObject_p;

    NH_BOOL hasProperty = Nh_ECMAScript_abstractHasOwnProperty(GlobalObject_p, Nh_ECMAScript_wrapString(Name_p));
    NH_BOOL extensible = Nh_ECMAScript_abstractIsExtensible(GlobalObject_p);

    if (!hasProperty && extensible) {
        Nh_ECMAScript_objectEnvironmentCreateMutableBinding(ObjectEnvironment_p, Name_p, deletable);
        Nh_ECMAScript_objectEnvironmentInitializeBinding(ObjectEnvironment_p, Name_p, Nh_ECMAScript_wrapUndefined());
    }

    Nh_List *VarDeclaredNames_p = &GlobalEnvironment_p->VarNames;
    
    NH_BOOL containsName = NH_FALSE;
    for (int i = 0; i < VarDeclaredNames_p->size; ++i) {
        if (!strcmp(Name_p->bytes_p, ((Nh_UTF8String*)VarDeclaredNames_p->pp[i])->bytes_p)) {
            containsName = NH_TRUE;
            break;
        }
    } 

    if (!containsName) {Nh_appendToList(VarDeclaredNames_p, Name_p);}

NH_ECMASCRIPT_END(Nh_ECMAScript_normalEmptyCompletion())
}

NH_BOOL Nh_ECMAScript_hasVarDeclaration(
    Nh_ECMAScript_GlobalEnvironment *GlobalEnvironment_p, Nh_UTF8String *Name_p)
{
NH_ECMASCRIPT_BEGIN()

    for (int i = 0; i < GlobalEnvironment_p->VarNames.size; ++i) {
        if (!strcmp(((Nh_UTF8String*)GlobalEnvironment_p->VarNames.pp[i])->bytes_p, Name_p->bytes_p)) {
            NH_ECMASCRIPT_END(NH_TRUE)
        }
    }

NH_ECMASCRIPT_END(NH_FALSE)
}

NH_BOOL Nh_ECMAScript_hasLexicalDeclaration(
    Nh_ECMAScript_GlobalEnvironment *GlobalEnvironment_p, Nh_UTF8String *Name_p)
{
NH_ECMASCRIPT_BEGIN()

// TODO 

NH_ECMASCRIPT_END(NH_FALSE)
}

NH_BOOL Nh_ECMAScript_hasRestrictedGlobalProperty(
    Nh_ECMAScript_GlobalEnvironment *GlobalEnvironment_p, Nh_UTF8String *Name_p)
{
NH_ECMASCRIPT_BEGIN()

// TODO 

NH_ECMASCRIPT_END(NH_FALSE)
}

static Nh_ECMAScript_Completion Nh_ECMAScript_globalEnvironmentCreateMutableBinding(
    Nh_ECMAScript_GlobalEnvironment *GlobalEnvironment_p, Nh_UTF8String *Name_p, NH_BOOL deletable)
{
NH_ECMASCRIPT_BEGIN()

    Nh_ECMAScript_DeclarativeEnvironment *DeclarativeEnvironment_p = GlobalEnvironment_p->DeclarativeEnvironment_p->Handle_p;
    if (Nh_ECMAScript_declarativeEnvironmentHasBinding(DeclarativeEnvironment_p, Name_p)) {
        NH_ECMASCRIPT_END(Nh_ECMAScript_throwTypeError())
    } 

NH_ECMASCRIPT_END(Nh_ECMAScript_declarativeEnvironmentCreateMutableBinding(DeclarativeEnvironment_p, Name_p, deletable))
}

static Nh_ECMAScript_Completion Nh_ECMAScript_globalEnvironmentCreateImmutableBinding(
    Nh_ECMAScript_GlobalEnvironment *GlobalEnvironment_p, Nh_UTF8String *Name_p, NH_BOOL strict)
{
NH_ECMASCRIPT_BEGIN()

    Nh_ECMAScript_DeclarativeEnvironment *DeclarativeEnvironment_p = GlobalEnvironment_p->DeclarativeEnvironment_p->Handle_p;
    if (Nh_ECMAScript_declarativeEnvironmentHasBinding(DeclarativeEnvironment_p, Name_p)) {
        NH_ECMASCRIPT_END(Nh_ECMAScript_throwTypeError())
    } 

NH_ECMASCRIPT_END(Nh_ECMAScript_declarativeEnvironmentCreateImmutableBinding(DeclarativeEnvironment_p, Name_p, strict))
}

static NH_BOOL Nh_ECMAScript_globalEnvironmentHasBinding(
    Nh_ECMAScript_GlobalEnvironment *GlobalEnvironment_p, Nh_UTF8String *Name_p)
{
NH_ECMASCRIPT_BEGIN()

    Nh_ECMAScript_DeclarativeEnvironment *DeclarativeEnvironment_p = GlobalEnvironment_p->DeclarativeEnvironment_p->Handle_p;
    if (Nh_ECMAScript_declarativeEnvironmentHasBinding(DeclarativeEnvironment_p, Name_p)) {
        NH_ECMASCRIPT_END(NH_TRUE)
    }

    Nh_ECMAScript_ObjectEnvironment *ObjectEnvironment_p = GlobalEnvironment_p->ObjectEnvironment_p->Handle_p;

NH_ECMASCRIPT_END(Nh_ECMAScript_objectEnvironmentHasBinding(ObjectEnvironment_p, Name_p))
}

// ABSTRACT ========================================================================================

NH_BOOL Nh_ECMAScript_hasBinding(
    Nh_ECMAScript_Environment *Environment_p, Nh_UTF8String *Name_p)
{
NH_ECMASCRIPT_BEGIN()

    switch (Environment_p->type)
    {
        case NH_ECMASCRIPT_ENVIRONMENT_OBJECT :
            NH_ECMASCRIPT_END(Nh_ECMAScript_objectEnvironmentHasBinding(
                Environment_p->Handle_p, Name_p
            ))
        case NH_ECMASCRIPT_ENVIRONMENT_GLOBAL :
            NH_ECMASCRIPT_END(Nh_ECMAScript_globalEnvironmentHasBinding(
                Environment_p->Handle_p, Name_p
            ))
        case NH_ECMASCRIPT_ENVIRONMENT_DECLARATIVE :
            NH_ECMASCRIPT_END(Nh_ECMAScript_declarativeEnvironmentHasBinding(
                Environment_p->Handle_p, Name_p
            ))
    }

NH_ECMASCRIPT_END(NH_FALSE)
}

Nh_ECMAScript_Completion Nh_ECMAScript_createMutableBinding(
    Nh_ECMAScript_Environment *Environment_p, Nh_UTF8String *Name_p, NH_BOOL deletable)
{
NH_ECMASCRIPT_BEGIN()

    switch (Environment_p->type)
    {
        case NH_ECMASCRIPT_ENVIRONMENT_GLOBAL :
            NH_ECMASCRIPT_END(Nh_ECMAScript_globalEnvironmentCreateMutableBinding(
                Environment_p->Handle_p, Name_p, deletable
            ))
        case NH_ECMASCRIPT_ENVIRONMENT_DECLARATIVE :
            NH_ECMASCRIPT_END(Nh_ECMAScript_declarativeEnvironmentCreateMutableBinding(
                Environment_p->Handle_p, Name_p, deletable
            ))
    }

NH_ECMASCRIPT_END(Nh_ECMAScript_throwTypeError())
}

Nh_ECMAScript_Completion Nh_ECMAScript_createImmutableBinding(
    Nh_ECMAScript_Environment *Environment_p, Nh_UTF8String *Name_p, NH_BOOL strict)
{
NH_ECMASCRIPT_BEGIN()

    switch (Environment_p->type)
    {
        case NH_ECMASCRIPT_ENVIRONMENT_GLOBAL :
            NH_ECMASCRIPT_END(Nh_ECMAScript_globalEnvironmentCreateImmutableBinding(
                Environment_p->Handle_p, Name_p, strict
            ))
        case NH_ECMASCRIPT_ENVIRONMENT_DECLARATIVE :
            NH_ECMASCRIPT_END(Nh_ECMAScript_declarativeEnvironmentCreateImmutableBinding(
                Environment_p->Handle_p, Name_p, strict
            ))
    }

NH_ECMASCRIPT_END(Nh_ECMAScript_throwTypeError())
}

Nh_ECMAScript_Completion Nh_ECMAScript_initializeBinding(
    Nh_ECMAScript_Environment *Environment_p, Nh_UTF8String *Name_p, Nh_ECMAScript_Any Value)
{
NH_ECMASCRIPT_BEGIN()

    switch (Environment_p->type)
    {
        case NH_ECMASCRIPT_ENVIRONMENT_GLOBAL : break;
        case NH_ECMASCRIPT_ENVIRONMENT_OBJECT : 
            NH_ECMASCRIPT_END(Nh_ECMAScript_objectEnvironmentInitializeBinding(
                Environment_p->Handle_p, Name_p, Value 
            ))
        case NH_ECMASCRIPT_ENVIRONMENT_DECLARATIVE :
            NH_ECMASCRIPT_END(Nh_ECMAScript_declarativeEnvironmentInitializeBinding(
                Environment_p->Handle_p, Name_p, Value 
            ))
    }

NH_ECMASCRIPT_END(Nh_ECMAScript_throwTypeError())
}

Nh_ECMAScript_Completion Nh_ECMAScript_setMutableBinding(
    Nh_ECMAScript_Environment *Environment_p, Nh_UTF8String *Name_p, Nh_ECMAScript_Any Value, NH_BOOL _throw)
{
NH_ECMASCRIPT_BEGIN()

    switch (Environment_p->type)
    {
        case NH_ECMASCRIPT_ENVIRONMENT_GLOBAL : break;
        case NH_ECMASCRIPT_ENVIRONMENT_OBJECT : 
            NH_ECMASCRIPT_END(Nh_ECMAScript_objectEnvironmentSetMutableBinding(
                Environment_p->Handle_p, Name_p, Value, _throw 
            ))
        case NH_ECMASCRIPT_ENVIRONMENT_DECLARATIVE : break;
    }

NH_ECMASCRIPT_END(Nh_ECMAScript_throwTypeError())
}


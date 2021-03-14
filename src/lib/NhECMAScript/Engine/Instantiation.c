// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "Instantiation.h"
#include "TestAndCompare.h"
#include "OrdinaryFunctionObject.h"
#include "ExecutionContext.h"

#include "../StaticSemantics/BoundNames.h"
#include "../StaticSemantics/DeclaredNames.h"
#include "../StaticSemantics/ScopedDeclarations.h"
#include "../StaticSemantics/Misc.h"

#include "../Common/Macro.h"
#include NH_ECMASCRIPT_FLOW
#include NH_ECMASCRIPT_CUSTOM_CHECK

#include "../../NhCore/Memory.h"

#include <stdio.h>
#include <string.h>

// INSTANTIATE FUNCTION OBJECT =====================================================================

Nh_ECMAScript_Object *Nh_ECMAScript_instantiateFunctionObject(
    Nh_ECMAScript_Environment *Scope_p, Nh_ECMAScript_ParseNode *Node_p)
{
NH_ECMASCRIPT_BEGIN()

    switch (Node_p->type)
    {
        case NH_ECMASCRIPT_PARSE_NODE_FUNCTION_DECLARATION :
            NH_ECMASCRIPT_END(Nh_ECMAScript_instantiateOrdinaryFunctionObject(Scope_p, Node_p))
        case NH_ECMASCRIPT_PARSE_NODE_GENERATOR_DECLARATION :
            break;
        case NH_ECMASCRIPT_PARSE_NODE_ASYNC_GENERATOR_DECLARATION :
            break;
        case NH_ECMASCRIPT_PARSE_NODE_ASYNC_FUNCTION_DECLARATION :
            break;
    }

NH_ECMASCRIPT_END(NULL)
}

// INSTANTIATE ORDINARY FUNCTION OBJECT ============================================================

// https://tc39.es/ecma262/#sec-runtime-semantics-instantiateordinaryfunctionobject
Nh_ECMAScript_Object *Nh_ECMAScript_instantiateOrdinaryFunctionObject(
    Nh_ECMAScript_Environment *Scope_p, Nh_ECMAScript_ParseNode *Node_p)
{
NH_ECMASCRIPT_BEGIN()

    if (Node_p->type != NH_ECMASCRIPT_PARSE_NODE_FUNCTION_DECLARATION) {NH_ECMASCRIPT_END(NULL)}

    if (((Nh_ECMAScript_ParseNode*)Node_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_BINDING_IDENTIFIER) 
    {
        Nh_Encoding_UTF8String *Name_p = &Node_p->Value_p->String;

        // TODO Let sourceText be the source text matched by FunctionDeclaration.

        Nh_ECMAScript_Object *Function_p = Nh_ECMAScript_ordinaryFunctionCreate(
            &Nh_ECMAScript_getCurrentRealm()->Intrinsics.Function.Prototype, NULL, Node_p->Children.pp[1],
            Node_p->Children.pp[2], NH_ECMASCRIPT_THIS_MODE_NON_LEXICAL, Scope_p
        );

        Nh_ECMAScript_setFunctionName(Function_p, Nh_ECMAScript_wrapString(Name_p), NULL);
        Nh_ECMAScript_makeConstructor(Function_p, NH_FALSE, NULL);

        NH_ECMASCRIPT_END(Function_p)
    }
    else {

        Nh_ECMAScript_String Name;
        Name.bytes_p = "default";
        Name.length  = 7;

        // TODO Let sourceText be the source text matched by FunctionDeclaration.

        Nh_ECMAScript_Object *Function_p = Nh_ECMAScript_ordinaryFunctionCreate(
            &Nh_ECMAScript_getCurrentRealm()->Intrinsics.Function.Prototype, NULL, Node_p->Children.pp[1],
            Node_p->Children.pp[2], NH_ECMASCRIPT_THIS_MODE_NON_LEXICAL, Scope_p
        );

        Nh_ECMAScript_setFunctionName(Function_p, Nh_ECMAScript_wrapString(&Name), NULL);
        Nh_ECMAScript_makeConstructor(Function_p, NH_FALSE, NULL);

        NH_ECMASCRIPT_END(Function_p)
    }

NH_ECMASCRIPT_END(NULL)
}

// GLOBAL DECLARATION INSTANTIATION ================================================================

static NH_BOOL Nh_ECMAScript_nameInList(
    Nh_List *Names_p, Nh_Encoding_UTF8String *Name_p)
{
NH_ECMASCRIPT_BEGIN()

    NH_BOOL inList = NH_FALSE;
    for (int j = 0; j < Names_p->size; ++j) {
        Nh_Encoding_UTF8String *Compare_p = Names_p->pp[j];
        if (!strcmp(Name_p->bytes_p, Compare_p->bytes_p)) {
            inList = NH_TRUE;
            break;
        }
    }

NH_ECMASCRIPT_END(inList)
}

// https://tc39.es/ecma262/#sec-globaldeclarationinstantiation
Nh_ECMAScript_Completion Nh_ECMAScript_globalDeclarationInstantiation(
    Nh_ECMAScript_ParseNode *ScriptBody_p, Nh_ECMAScript_Environment *GlobalEnvironment_p)
{
NH_ECMASCRIPT_BEGIN()

    Nh_List LexNames = Nh_ECMAScript_getLexicallyDeclaredNames(ScriptBody_p);
    Nh_List VarNames = Nh_ECMAScript_getVarDeclaredNames(ScriptBody_p);

    for (int i = 0; i < LexNames.size; ++i) 
    {
        if (Nh_ECMAScript_hasVarDeclaration(GlobalEnvironment_p->Handle_p, LexNames.pp[i])) {
            NH_ECMASCRIPT_END(Nh_ECMAScript_throwTypeError())
        }
        if (Nh_ECMAScript_hasLexicalDeclaration(GlobalEnvironment_p->Handle_p, LexNames.pp[i])) {
            NH_ECMASCRIPT_END(Nh_ECMAScript_throwTypeError())
        }
        NH_BOOL hasRestrictedGlobal = Nh_ECMAScript_hasRestrictedGlobalProperty(GlobalEnvironment_p->Handle_p, LexNames.pp[i]);
        if (hasRestrictedGlobal) {
            NH_ECMASCRIPT_END(Nh_ECMAScript_throwTypeError())
        }
    }

    for (int i = 0; i < VarNames.size; ++i) 
    {
        if (Nh_ECMAScript_hasLexicalDeclaration(GlobalEnvironment_p->Handle_p, VarNames.pp[i])) {
            NH_ECMASCRIPT_END(Nh_ECMAScript_throwTypeError())
        }
    }

    Nh_List VarDeclarations = Nh_ECMAScript_getVarScopedDeclarations(ScriptBody_p);
    Nh_List FunctionsToInitialize = Nh_initList(16);
    Nh_List DeclaredFunctionNames = Nh_initList(16);

    for (int i = VarDeclarations.size - 1; i >= 0; --i) 
    {
        Nh_ECMAScript_ParseNode *Declaration_p = VarDeclarations.pp[i];

        if (Declaration_p->type != NH_ECMASCRIPT_PARSE_NODE_VARIABLE_DECLARATION
        &&  Declaration_p->type != NH_ECMASCRIPT_PARSE_NODE_FOR_BINDING
        &&  Declaration_p->type != NH_ECMASCRIPT_PARSE_NODE_BINDING_IDENTIFIER) {

            Nh_List BoundNames = Nh_ECMAScript_getBoundNames(Declaration_p);
            Nh_Encoding_UTF8String *Name_p = BoundNames.pp[BoundNames.size - 1];

            if (!Nh_ECMAScript_nameInList(&DeclaredFunctionNames, Name_p)) {
                NH_BOOL fnDefinable = Nh_ECMAScript_canDeclareGlobalFunction(GlobalEnvironment_p->Handle_p, Name_p);
                if (!fnDefinable) {NH_ECMASCRIPT_END(Nh_ECMAScript_throwTypeError())}
                Nh_appendToList(&DeclaredFunctionNames, Name_p);
                Nh_prependToList(&FunctionsToInitialize, Declaration_p);
            }

            Nh_freeList(&BoundNames, NH_FALSE);
        }
    }

    Nh_List DeclaredVarNames = Nh_initList(16);
    for (int i = 0; i < VarDeclarations.size; ++i) 
    {
        Nh_ECMAScript_ParseNode *Declaration_p = VarDeclarations.pp[i];

        if (Declaration_p->type == NH_ECMASCRIPT_PARSE_NODE_VARIABLE_DECLARATION
        ||  Declaration_p->type == NH_ECMASCRIPT_PARSE_NODE_FOR_BINDING
        ||  Declaration_p->type == NH_ECMASCRIPT_PARSE_NODE_BINDING_IDENTIFIER) {

            Nh_List BoundNames = Nh_ECMAScript_getBoundNames(Declaration_p);

            for (int j = 0; j < BoundNames.size; ++j) 
            {
                Nh_Encoding_UTF8String *Name_p = BoundNames.pp[j];

                if (!Nh_ECMAScript_nameInList(&DeclaredFunctionNames, Name_p)) {
                    NH_BOOL vnDefinable = Nh_ECMAScript_canDeclareGlobalVar(GlobalEnvironment_p->Handle_p, Name_p);
                    if (!vnDefinable) {NH_ECMASCRIPT_END(Nh_ECMAScript_throwTypeError())}
                    if (!Nh_ECMAScript_nameInList(&DeclaredVarNames, Name_p)) {
                        Nh_appendToList(&DeclaredVarNames, Name_p);
                    }
                }
            }

            Nh_freeList(&BoundNames, NH_FALSE);
        }
    } 

    // TODO Annex B.3.3.2 adds additional steps at this point.

    Nh_List LexDeclarations = Nh_ECMAScript_getLexicallyScopedDeclarations(ScriptBody_p);

    for (int i = 0; i < LexDeclarations.size; ++i) 
    {
        Nh_ECMAScript_ParseNode *Declaration_p = LexDeclarations.pp[i];
        Nh_List BoundNames = Nh_ECMAScript_getBoundNames(Declaration_p);

        for (int j = 0; j < BoundNames.size; ++j) {
            Nh_Encoding_UTF8String *Name_p = BoundNames.pp[j];
            if (Nh_ECMAScript_isConstantDeclaration(Declaration_p)) {
                Nh_ECMAScript_createImmutableBinding(GlobalEnvironment_p, Name_p, NH_TRUE);
            } else {
                Nh_ECMAScript_createMutableBinding(GlobalEnvironment_p, Name_p, NH_FALSE);
            }
        }

        Nh_freeList(&BoundNames, NH_FALSE);
    }

    for (int i = 0; i < FunctionsToInitialize.size; ++i) 
    {
        Nh_ECMAScript_ParseNode *Function_p = FunctionsToInitialize.pp[i];
        Nh_List BoundNames = Nh_ECMAScript_getBoundNames(Function_p);
        Nh_Encoding_UTF8String *Name_p = BoundNames.pp[BoundNames.size - 1];
        Nh_ECMAScript_Object *FunctionObject_p = Nh_ECMAScript_instantiateFunctionObject(GlobalEnvironment_p, Function_p);
        Nh_ECMAScript_createGlobalFunctionBinding(Name_p, Nh_ECMAScript_wrapObject(FunctionObject_p), NH_FALSE);
        Nh_freeList(&BoundNames, NH_FALSE);
    }

    for (int i = 0; i < DeclaredVarNames.size; ++i) {
        Nh_ECMAScript_createGlobalVarBinding(GlobalEnvironment_p->Handle_p, DeclaredVarNames.pp[i], NH_FALSE);
    }

    Nh_freeList(&LexNames, NH_FALSE);
    Nh_freeList(&VarNames, NH_FALSE);
    Nh_freeList(&DeclaredVarNames, NH_FALSE);
    Nh_freeList(&DeclaredFunctionNames, NH_FALSE);
    Nh_freeList(&FunctionsToInitialize, NH_FALSE);
    Nh_freeList(&VarDeclarations, NH_FALSE);
    Nh_freeList(&LexDeclarations, NH_FALSE);

NH_ECMASCRIPT_END(Nh_ECMAScript_normalEmptyCompletion())
}


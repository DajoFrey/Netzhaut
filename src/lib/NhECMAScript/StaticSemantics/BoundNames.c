// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "BoundNames.h"

#include "../Engine/Type.h"
#include "../Common/Macro.h"
#include NH_ECMASCRIPT_FLOW
#include NH_ECMASCRIPT_CUSTOM_CHECK

#include "../../NhCore/Memory.h"
#include "../../NhCore/Unicode.h"

#include <stdio.h>
#include <string.h>

// DATA ============================================================================================

static NH_BYTE *synthetic_p = "*default*";

// HELPER ==========================================================================================

void Nh_ECMAScript_freeBoundNames(
    Nh_List *Names_p)
{
NH_ECMASCRIPT_BEGIN()

    for (int i = 0; i < Names_p->size; ++i) {
        if (((Nh_ECMAScript_InputElement*)Names_p->handles_pp[i])->type == -1) {
            Nh_free(Names_p->handles_pp[i]);
        }
    }

    Nh_freeList(Names_p, NH_FALSE);

NH_ECMASCRIPT_SILENT_END()
}

static Nh_List Nh_ECMAScript_getSyntheticName()
{
NH_ECMASCRIPT_BEGIN()

    Nh_List List = Nh_initList(1);

    Nh_ECMAScript_InputElement *InputElement_p = Nh_allocate(sizeof(Nh_ECMAScript_InputElement));
    NH_ECMASCRIPT_CHECK_MEM(List, InputElement_p)

    InputElement_p->type = -1;
    InputElement_p->String.bytes_p = synthetic_p;
    InputElement_p->String.length  = strlen(synthetic_p);

    Nh_appendToList(&List, InputElement_p);

NH_ECMASCRIPT_END(List)
}

// BOUND NAMES =====================================================================================

// https://tc39.es/ecma262/#sec-static-semantics-boundnames
Nh_List Nh_ECMAScript_getBoundNames(
    Nh_ECMAScript_ParseNode *Node_p)
{
NH_ECMASCRIPT_BEGIN()

    switch (Node_p->type)
    {
        case NH_ECMASCRIPT_PARSE_NODE_IDENTIFIER :
        {
            Nh_List Result = Nh_initList(1);
            Nh_appendToList(&Result, &((Nh_ECMAScript_ParseNode*)Node_p->Children.handles_pp[0])->Value_p->String);
            NH_ECMASCRIPT_END(Result)
            break;
        }
        case NH_ECMASCRIPT_PARSE_NODE_BINDING_IDENTIFIER :
            if (((Nh_ECMAScript_ParseNode*)Node_p->Children.handles_pp[0])->Value_p == NULL) {
                NH_ECMASCRIPT_END(Nh_ECMAScript_getBoundNames(Node_p->Children.handles_pp[0]))
            }
            else { // must be 'yield' or 'await'
                Nh_List Result = Nh_initList(1);
                Nh_appendToList(&Result, &((Nh_ECMAScript_ParseNode*)Node_p->Children.handles_pp[0])->Value_p->String);
                NH_ECMASCRIPT_END(Result)
            }
            break;

        case NH_ECMASCRIPT_PARSE_NODE_LEXICAL_DECLARATION :
            NH_ECMASCRIPT_END(Nh_ECMAScript_getBoundNames(Node_p->Children.handles_pp[1]))
            break;

        case NH_ECMASCRIPT_PARSE_NODE_BINDING_LIST :
        {
            Nh_List Names = Nh_ECMAScript_getBoundNames(Node_p->Children.handles_pp[0]);
            if (Node_p->Children.size == 3) {
                Nh_List BindingNames = Nh_ECMAScript_getBoundNames(Node_p->Children.handles_pp[2]);
                Nh_appendItemsToList(&Names, &BindingNames);
                Nh_freeList(&BindingNames, NH_FALSE);
            }
            NH_ECMASCRIPT_END(Names)
        }
        break;

        case NH_ECMASCRIPT_PARSE_NODE_LEXICAL_BINDING :
            NH_ECMASCRIPT_END(Nh_ECMAScript_getBoundNames(Node_p->Children.handles_pp[0]))

        case NH_ECMASCRIPT_PARSE_NODE_VARIABLE_DECLARATION_LIST :
        {
            Nh_List Names = Nh_ECMAScript_getBoundNames(Node_p->Children.handles_pp[0]);
            if (Node_p->Children.size == 3) {
                Nh_List DeclarationNames = Nh_ECMAScript_getBoundNames(Node_p->Children.handles_pp[2]);
                Nh_appendItemsToList(&Names, &DeclarationNames);
                Nh_freeList(&DeclarationNames, NH_FALSE);
            }
            NH_ECMASCRIPT_END(Names)
        }

        case NH_ECMASCRIPT_PARSE_NODE_VARIABLE_DECLARATION :
            NH_ECMASCRIPT_END(Nh_ECMAScript_getBoundNames(Node_p->Children.handles_pp[0]))

        case NH_ECMASCRIPT_PARSE_NODE_OBJECT_BINDING_PATTERN :
            if (Node_p->Children.size == 5) {
                Nh_List Names = Nh_ECMAScript_getBoundNames(Node_p->Children.handles_pp[1]);
                Nh_List RestNames = Nh_ECMAScript_getBoundNames(Node_p->Children.handles_pp[3]);
                Nh_appendItemsToList(&Names, &RestNames);
                Nh_freeList(&RestNames, NH_FALSE);
                NH_ECMASCRIPT_END(Names)
            }
            if (Node_p->Children.size >= 3) {
                NH_ECMASCRIPT_END(Nh_ECMAScript_getBoundNames(Node_p->Children.handles_pp[1]))
            }
            break;

        case NH_ECMASCRIPT_PARSE_NODE_BINDING_REST_PROPERTY :
            NH_ECMASCRIPT_END(Nh_ECMAScript_getBoundNames(Node_p->Children.handles_pp[0]))

        case NH_ECMASCRIPT_PARSE_NODE_BINDING_PROPERTY_LIST :
        {
            Nh_List Names = Nh_ECMAScript_getBoundNames(Node_p->Children.handles_pp[0]);
            if (Node_p->Children.size == 3) {
                Nh_List PropertyNames = Nh_ECMAScript_getBoundNames(Node_p->Children.handles_pp[1]);
                Nh_appendItemsToList(&Names, &PropertyNames);
                Nh_freeList(&PropertyNames, NH_FALSE);
            }
            NH_ECMASCRIPT_END(Names)
        }

//        case NH_ECMASCRIPT_PARSE_NODE_ARRAY_BINDING_PATTERN :
//            if (Node_p->Children.size == 1) {
//                if (((Nh_ECMAScript_ParseNode*)Node_p->Children.handles_pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_BINDING_REST_ELEMENT) {
//                    NH_ECMASCRIPT_END(Nh_ECMAScript_getBoundNames(Node_p->Children.handles_pp[0]))
//                }
//                if (((Nh_ECMAScript_ParseNode*)Node_p->Children.handles_pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_BINDING_ELEMENT_LIST) {
//                    NH_ECMASCRIPT_END(Nh_ECMAScript_getBoundNames(Node_p->Children.handles_pp[0]))
//                }
//            }
//            else if (Node_p->Children.size == 2) {
//                if (((Nh_ECMAScript_ParseNode*)Node_p->Children.handles_pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_ELISION) {
//                    if (((Nh_ECMAScript_ParseNode*)Node_p->Children.handles_pp[1])->type == NH_ECMASCRIPT_PARSE_NODE_BINDING_REST_ELEMENT) {
//                        NH_ECMASCRIPT_END(Nh_ECMAScript_getBoundNames(Node_p->Children.handles_pp[1]))
//                    }
//                }
//                else if (((Nh_ECMAScript_ParseNode*)Node_p->Children.handles_pp[1])->type == NH_ECMASCRIPT_PARSE_NODE_ELISION) {
//                    if (((Nh_ECMAScript_ParseNode*)Node_p->Children.handles_pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_BINDING_ELEMENT_LIST) {
//                        NH_ECMASCRIPT_END(Nh_ECMAScript_getBoundNames(Node_p->Children.handles_pp[0]))
//                    }
//                }
//                else if (((Nh_ECMAScript_ParseNode*)Node_p->Children.handles_pp[1])->type == NH_ECMASCRIPT_PARSE_NODE_BINDING_REST_ELEMENT) {
//                    Nh_List BindingElementListNames = Nh_ECMAScript_getBoundNames(Node_p->Children.handles_pp[0]);
//                    Nh_List BindingRestElementNames = Nh_ECMAScript_getBoundNames(Node_p->Children.handles_pp[1]);
//                    for (int i = 0; i < BindingRestElementNames.size; ++i) {
//                        Nh_appendToList(&BindingElementListNames, BindingRestElementNames.handles_pp[i]);
//                    }
//                    Nh_freeList(&BindingRestElementNames, NH_FALSE);
//                    NH_ECMASCRIPT_END(BindingElementListNames)
//                }
//            }
//            else if (Node_p->Children.size == 3) {
//                Nh_List BindingElementListNames = Nh_ECMAScript_getBoundNames(Node_p->Children.handles_pp[0]);
//                Nh_List BindingRestElementNames = Nh_ECMAScript_getBoundNames(Node_p->Children.handles_pp[2]);
//                for (int i = 0; i < BindingRestElementNames.size; ++i) {
//                    Nh_appendToList(&BindingElementListNames, BindingRestElementNames.handles_pp[i]);
//                }
//                Nh_freeList(&BindingRestElementNames, NH_FALSE);
//                NH_ECMASCRIPT_END(BindingElementListNames)
//            }
//            break;

        case NH_ECMASCRIPT_PARSE_NODE_BINDING_REST_ELEMENT :
            NH_ECMASCRIPT_END(Nh_ECMAScript_getBoundNames(Node_p->Children.handles_pp[0]))

        case NH_ECMASCRIPT_PARSE_NODE_BINDING_PATTERN :
            NH_ECMASCRIPT_END(Nh_ECMAScript_getBoundNames(Node_p->Children.handles_pp[0]))

        case NH_ECMASCRIPT_PARSE_NODE_BINDING_ELEMENT_LIST :
        {
            Nh_List Names = Nh_ECMAScript_getBoundNames(Node_p->Children.handles_pp[0]);
            if (Node_p->Children.size == 2) {
                Nh_List ElementNames = Nh_ECMAScript_getBoundNames(Node_p->Children.handles_pp[1]);
                Nh_appendItemsToList(&Names, &ElementNames);
                Nh_freeList(&ElementNames, NH_FALSE);
            }
            NH_ECMASCRIPT_END(Names)
        }

//        case NH_ECMASCRIPT_PARSE_NODE_BINDING_ELISION_ELEMENT :
//            if (Node_p->Children.size == 1) {
//                NH_ECMASCRIPT_END(Nh_ECMAScript_getBoundNames(Node_p->Children.handles_pp[0]))
//            }
//            else {NH_ECMASCRIPT_END(Nh_ECMAScript_getBoundNames(Node_p->Children.handles_pp[1]))}
//            break;
//
//        case NH_ECMASCRIPT_PARSE_NODE_BINDING_ELEMENT :
//            if (((Nh_ECMAScript_ParseNode*)Node_p->Children.handles_pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_BINDING_PATTERN) {
//                NH_ECMASCRIPT_END(Nh_ECMAScript_getBoundNames(Node_p->Children.handles_pp[0]))
//            }
//            break;
//
//        case NH_ECMASCRIPT_PARSE_NODE_SINGLE_NAME_BINDING :
//            NH_ECMASCRIPT_END(Nh_ECMAScript_getBoundNames(Node_p->Children.handles_pp[0]))
//            break;
//
//        case NH_ECMASCRIPT_PARSE_NODE_FOR_DECLARATION :
//            NH_ECMASCRIPT_END(Nh_ECMAScript_getBoundNames(Node_p->Children.handles_pp[1]))
//
//        case NH_ECMASCRIPT_PARSE_NODE_FOR_BINDING :
//            NH_ECMASCRIPT_END(Nh_ECMAScript_getBoundNames(Node_p->Children.handles_pp[0]))
//
//        case NH_ECMASCRIPT_PARSE_NODE_FUNCTION_DECLARATION :
//            if (((Nh_ECMAScript_ParseNode*)Node_p->Children.handles_pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_BINDING_IDENTIFIER) {
//                NH_ECMASCRIPT_END(Nh_ECMAScript_getBoundNames(Node_p->Children.handles_pp[0]))
//            }
//            else {NH_ECMASCRIPT_END(Nh_ECMAScript_getSyntheticName())}
//            break;
//
//        case NH_ECMASCRIPT_PARSE_NODE_FORMAL_PARAMETERS :
//            if (Node_p->Children.size == 2) {
//                Nh_List FormalParameterListNames = Nh_ECMAScript_getBoundNames(Node_p->Children.handles_pp[0]);
//                Nh_List FunctionRestParameterNames = Nh_ECMAScript_getBoundNames(Node_p->Children.handles_pp[1]);
//                for (int i = 0; i < FunctionRestParameterNames.size; ++i) {
//                    Nh_appendToList(&FormalParameterListNames, FunctionRestParameterNames.handles_pp[i]);
//                }
//                Nh_freeList(&FunctionRestParameterNames, NH_FALSE);
//                NH_ECMASCRIPT_END(FormalParameterListNames)
//            }
//            break;
//
//        case NH_ECMASCRIPT_PARSE_NODE_FORMAL_PARAMETER_LIST :
//        {
//            Nh_List Names = Nh_ECMAScript_getBoundNames(Node_p->Children.handles_pp[0]);
//            if (Node_p->Children.size == 2) {
//                Nh_List FormalParameterNames = Nh_ECMAScript_getBoundNames(Node_p->Children.handles_pp[1]);
//                for (int i = 0; i < FormalParameterNames.size; ++i) {
//                    Nh_appendToList(&Names, FormalParameterNames.handles_pp[i]);
//                }
//                Nh_freeList(&FormalParameterNames, NH_FALSE);
//            }
//            NH_ECMASCRIPT_END(Names)
//        }
//
//        case NH_ECMASCRIPT_PARSE_NODE_FORMAL_PARAMETER :
//            NH_ECMASCRIPT_END(Nh_ECMAScript_getBoundNames(Node_p->Children.handles_pp[0]))
//            break;
//
// ArrowParameters : CoverParenthesizedExpressionAndArrowParameterList
// 
//     Let formals be CoveredFormalsList of CoverParenthesizedExpressionAndArrowParameterList.
//     Return the BoundNames of formals.
// 
// GeneratorDeclaration : function * BindingIdentifier ( FormalParameters ) { GeneratorBody }
// 
//     Return the BoundNames of BindingIdentifier.
// 
// GeneratorDeclaration : function * ( FormalParameters ) { GeneratorBody }
// 
//     Return « "*default*" ».
// 
// AsyncGeneratorDeclaration : async function * BindingIdentifier ( FormalParameters ) { AsyncGeneratorBody }
// 
//     Return the BoundNames of BindingIdentifier.
// 
// AsyncGeneratorDeclaration : async function * ( FormalParameters ) { AsyncGeneratorBody }
// 
//     Return « "*default*" ».
// 
// ClassDeclaration : class BindingIdentifier ClassTail
// 
//     Return the BoundNames of BindingIdentifier.
// 
// ClassDeclaration : class ClassTail
// 
//     Return « "*default*" ».
// 
// AsyncFunctionDeclaration : async function BindingIdentifier ( FormalParameters ) { AsyncFunctionBody }
// 
//     Return the BoundNames of BindingIdentifier.
// 
// AsyncFunctionDeclaration : async function ( FormalParameters ) { AsyncFunctionBody }
// 
//     Return « "*default*" ».
// 
// CoverCallExpressionAndAsyncArrowHead : MemberExpression Arguments
// 
//     Let head be CoveredAsyncArrowHead of CoverCallExpressionAndAsyncArrowHead.
//     Return the BoundNames of head.
// 
// ImportDeclaration : import ImportClause FromClause ;
// 
//     Return the BoundNames of ImportClause.
// 
// ImportDeclaration : import ModuleSpecifier ;
// 
//     Return a new empty List.
// 
// ImportClause : ImportedDefaultBinding , NameSpaceImport
// 
//     Let names be the BoundNames of ImportedDefaultBinding.
//     Append to names the elements of the BoundNames of NameSpaceImport.
//     Return names.
// 
// ImportClause : ImportedDefaultBinding , NamedImports
// 
//     Let names be the BoundNames of ImportedDefaultBinding.
//     Append to names the elements of the BoundNames of NamedImports.
//     Return names.
// 
// NamedImports : { }
// 
//     Return a new empty List.
// 
// ImportsList : ImportsList , ImportSpecifier
// 
//     Let names be the BoundNames of ImportsList.
//     Append to names the elements of the BoundNames of ImportSpecifier.
//     Return names.
// 
// ImportSpecifier : IdentifierName as ImportedBinding
// 
//     Return the BoundNames of ImportedBinding.
// 
// ExportDeclaration :
// export ExportFromClause FromClause ;
// export NamedExports ;
// 
//     Return a new empty List.
// 
// ExportDeclaration : export VariableStatement
// 
//     Return the BoundNames of VariableStatement.
// 
// ExportDeclaration : export Declaration
// 
//     Return the BoundNames of Declaration.
// 
// ExportDeclaration : export default HoistableDeclaration
// 
//     Let declarationNames be the BoundNames of HoistableDeclaration.
//     If declarationNames does not include the element "*default*", append "*default*" to declarationNames.
//     Return declarationNames.
// 
// ExportDeclaration : export default ClassDeclaration
// 
//     Let declarationNames be the BoundNames of ClassDeclaration.
//     If declarationNames does not include the element "*default*", append "*default*" to declarationNames.
//     Return declarationNames.
// 
// ExportDeclaration : export default AssignmentExpression ;
// 
//     Return « "*default*" ».
    }

NH_ECMASCRIPT_END(Nh_initList(8))
}


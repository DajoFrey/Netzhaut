// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "ScopedDeclarations.h"

#include "../Engine/Type.h"
#include "../Common/Macro.h"
#include NH_ECMASCRIPT_FLOW
#include NH_ECMASCRIPT_CUSTOM_CHECK

#include <stdio.h>
#include <string.h>

// DECLARATION PART ================================================================================

// https://tc39.es/ecma262/#sec-static-semantics-declarationpart
static Nh_List Nh_ECMAScript_getDeclarationPart(
    Nh_ECMAScript_ParseNode *Node_p)
{
NH_ECMASCRIPT_BEGIN()

    switch (Node_p->type)
    {
        case NH_ECMASCRIPT_PARSE_NODE_HOISTABLE_DECLARATION :
        {
            Nh_List List = Nh_initList(8);
            Nh_appendToList(&List, Node_p->Children.handles_pp[0]);
            NH_ECMASCRIPT_END(List)
            break;
        }
        case NH_ECMASCRIPT_PARSE_NODE_DECLARATION :
            if (((Nh_ECMAScript_ParseNode*)Node_p->Children.handles_pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_HOISTABLE_DECLARATION) {
                NH_ECMASCRIPT_END(Nh_ECMAScript_getDeclarationPart(Node_p->Children.handles_pp[0]))
            }
            else { // must be ClassDeclaration or LexicalDeclaration
                Nh_List List = Nh_initList(8);
                Nh_appendToList(&List, Node_p->Children.handles_pp[0]);
                NH_ECMASCRIPT_END(List)
            }
            break;
    }

NH_ECMASCRIPT_END(Nh_initList(8))
}

// LEXICALLY SCOPED DECLARATIONS ===================================================================

// https://tc39.es/ecma262/#sec-static-semantics-lexicallyscopeddeclarations
Nh_List Nh_ECMAScript_getLexicallyScopedDeclarations(
    Nh_ECMAScript_ParseNode *Node_p)
{
NH_ECMASCRIPT_BEGIN()

    switch (Node_p->type)
    {
        case NH_ECMASCRIPT_PARSE_NODE_STATEMENT_LIST :
        {
            Nh_List Declarations = Nh_ECMAScript_getLexicallyScopedDeclarations(Node_p->Children.handles_pp[0]);
            if (Node_p->Children.size == 2) {
                Nh_List StatementDeclarations = Nh_ECMAScript_getLexicallyScopedDeclarations(Node_p->Children.handles_pp[1]);
                Nh_appendItemsToList(&Declarations, &StatementDeclarations);
                Nh_freeList(&StatementDeclarations, NH_FALSE);
            }
            NH_ECMASCRIPT_END(Declarations)
        }

        case NH_ECMASCRIPT_PARSE_NODE_STATEMENT_LIST_ITEM :
            if (((Nh_ECMAScript_ParseNode*)Node_p->Children.handles_pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_STATEMENT) {
                Nh_ECMAScript_ParseNode *Statement_p = Node_p->Children.handles_pp[0];
                if (((Nh_ECMAScript_ParseNode*)Statement_p->Children.handles_pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_LABELLED_STATEMENT) {
                    NH_ECMASCRIPT_END(Nh_ECMAScript_getLexicallyScopedDeclarations(Statement_p->Children.handles_pp[0]))
                }
            }
            else { // must be Declaration
                NH_ECMASCRIPT_END(Nh_ECMAScript_getDeclarationPart(Node_p->Children.handles_pp[0]))
            }
            break;

//        case NH_ECMASCRIPT_PARSE_NODE_CASE_BLOCK :
//            if (Node_p->Children.size == 1 && ((Nh_ECMAScript_ParseNode*)Node_p->Children.handles_pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_CASE_CLAUSES) {
//                NH_ECMASCRIPT_END(Nh_ECMAScript_getLexicallyScopedDeclarations(Node_p->Children.handles_pp[0]))
//            }
//            else if (Node_p->Children.size > 0)
//            {
//                Nh_List Declarations = Nh_initList(8);
//                if (((Nh_ECMAScript_ParseNode*)Node_p->Children.handles_pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_CASE_CLAUSES) {
//                    Declarations = Nh_ECMAScript_getLexicallyScopedDeclarations(Node_p->Children.handles_pp[0]);
//                    Nh_List DefaultClauseDeclarations = Nh_ECMAScript_getLexicallyScopedDeclarations(Node_p->Children.handles_pp[1]);
//                    for (int i = 0; i < DefaultClauseDeclarations.size; ++i) {
//                        Nh_appendToList(&Declarations, DefaultClauseDeclarations.handles_pp[i]);
//                    }
//                    Nh_freeList(&DefaultClauseDeclarations, NH_FALSE);
//                    if (Node_p->Children.size > 2) {
//                        Nh_List CaseClausesDeclarations = Nh_ECMAScript_getLexicallyScopedDeclarations(Node_p->Children.handles_pp[1]);
//                        for (int i = 0; i < CaseClausesDeclarations.size; ++i) {
//                            Nh_appendToList(&Declarations, CaseClausesDeclarations.handles_pp[i]);
//                        }
//                        Nh_freeList(&CaseClausesDeclarations, NH_FALSE);
//                    }
//                }
//                else {
//                    Declarations = Nh_ECMAScript_getLexicallyScopedDeclarations(Node_p->Children.handles_pp[0]);
//                    if (Node_p->Children.size > 1) {
//                        Nh_List CaseClausesDeclarations = Nh_ECMAScript_getLexicallyScopedDeclarations(Node_p->Children.handles_pp[1]);
//                        for (int i = 0; i < CaseClausesDeclarations.size; ++i) {
//                            Nh_appendToList(&Declarations, CaseClausesDeclarations.handles_pp[i]);
//                        }
//                        Nh_freeList(&CaseClausesDeclarations, NH_FALSE);
//                    }
//                }
//                NH_ECMASCRIPT_END(Declarations)
//            }
//            break;
//
        case NH_ECMASCRIPT_PARSE_NODE_CASE_CLAUSES :
        {
            Nh_List Declarations = Nh_ECMAScript_getLexicallyScopedDeclarations(Node_p->Children.handles_pp[0]);
            if (Node_p->Children.size == 2) {
                Nh_List CaseDeclarations = Nh_ECMAScript_getLexicallyScopedDeclarations(Node_p->Children.handles_pp[1]);
                Nh_appendItemsToList(&Declarations, &CaseDeclarations);
                Nh_freeList(&CaseDeclarations, NH_FALSE);
            }
            NH_ECMASCRIPT_END(Declarations)
        }

        case NH_ECMASCRIPT_PARSE_NODE_CASE_CLAUSE :
            if (Node_p->Children.size == 2) {
                NH_ECMASCRIPT_END(Nh_ECMAScript_getLexicallyScopedDeclarations(Node_p->Children.handles_pp[1]))
            }
            break;

        case NH_ECMASCRIPT_PARSE_NODE_DEFAULT_CLAUSE :
            if (Node_p->Children.size == 1) {
                NH_ECMASCRIPT_END(Nh_ECMAScript_getLexicallyScopedDeclarations(Node_p->Children.handles_pp[0]))
            }
            break;

        case NH_ECMASCRIPT_PARSE_NODE_LABELLED_STATEMENT :
            NH_ECMASCRIPT_END(Nh_ECMAScript_getLexicallyScopedDeclarations(Node_p->Children.handles_pp[1]))

        case NH_ECMASCRIPT_PARSE_NODE_LABELLED_ITEM :
            if (((Nh_ECMAScript_ParseNode*)Node_p->Children.handles_pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_FUNCTION_DECLARATION) {
                Nh_List List = Nh_initList(8);
                Nh_appendToList(&List, Node_p->Children.handles_pp[0]);
                NH_ECMASCRIPT_END(List)
            }
            break;

        case NH_ECMASCRIPT_PARSE_NODE_FUNCTION_STATEMENT_LIST :
            if (Node_p->Children.size > 0) {
                NH_ECMASCRIPT_END(Nh_ECMAScript_getTopLevelLexicallyScopedDeclarations(Node_p->Children.handles_pp[0]))
            }
            break;

        case NH_ECMASCRIPT_PARSE_NODE_CONCISE_BODY :
            if (((Nh_ECMAScript_ParseNode*)Node_p->Children.handles_pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_FUNCTION_BODY) {
                Nh_ECMAScript_ParseNode *FunctionBody_p = Node_p->Children.handles_pp[0];
                NH_ECMASCRIPT_END(Nh_ECMAScript_getLexicallyScopedDeclarations(FunctionBody_p->Children.handles_pp[0]))
            }
            break;

        case NH_ECMASCRIPT_PARSE_NODE_ASYNC_CONCISE_BODY :
            if (((Nh_ECMAScript_ParseNode*)Node_p->Children.handles_pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_ASYNC_FUNCTION_BODY) {
                Nh_ECMAScript_ParseNode *AsyncFunctionBody_p = Node_p->Children.handles_pp[0];
                Nh_ECMAScript_ParseNode *FunctionBody_p = AsyncFunctionBody_p->Children.handles_pp[0];
                NH_ECMASCRIPT_END(Nh_ECMAScript_getLexicallyScopedDeclarations(FunctionBody_p->Children.handles_pp[0]))
            }
            break;

        case NH_ECMASCRIPT_PARSE_NODE_SCRIPT_BODY :
            NH_ECMASCRIPT_END(Nh_ECMAScript_getTopLevelLexicallyScopedDeclarations(Node_p->Children.handles_pp[0]))

        case NH_ECMASCRIPT_PARSE_NODE_MODULE :
            NH_ECMASCRIPT_END(Nh_ECMAScript_getLexicallyScopedDeclarations(Node_p->Children.handles_pp[0]))

        case NH_ECMASCRIPT_PARSE_NODE_MODULE_BODY :
            NH_ECMASCRIPT_END(Nh_ECMAScript_getLexicallyScopedDeclarations(Node_p->Children.handles_pp[0]))

        case NH_ECMASCRIPT_PARSE_NODE_MODULE_ITEM_LIST :
        {
            Nh_List Declarations = Nh_ECMAScript_getLexicallyScopedDeclarations(Node_p->Children.handles_pp[0]);
            if (Node_p->Children.size == 2) {
                Nh_List ModuleDeclarations = Nh_ECMAScript_getLexicallyScopedDeclarations(Node_p->Children.handles_pp[1]);
                Nh_appendItemsToList(&Declarations, &ModuleDeclarations);
                Nh_freeList(&ModuleDeclarations, NH_FALSE);
            }
            NH_ECMASCRIPT_END(Declarations)
        }

        case NH_ECMASCRIPT_PARSE_NODE_MODULE_ITEM :
            if (((Nh_ECMAScript_ParseNode*)Node_p->Children.handles_pp[0])->type != NH_ECMASCRIPT_PARSE_NODE_IMPORT_DECLARATION) {
                NH_ECMASCRIPT_END(Nh_ECMAScript_getLexicallyScopedDeclarations(Node_p->Children.handles_pp[0]))
            }
            break;

        case NH_ECMASCRIPT_PARSE_NODE_EXPORT_DECLARATION :
            if (((Nh_ECMAScript_ParseNode*)Node_p->Children.handles_pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_EXPORT_FROM_CLAUSE
            ||  ((Nh_ECMAScript_ParseNode*)Node_p->Children.handles_pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_NAMED_EXPORTS
            ||  ((Nh_ECMAScript_ParseNode*)Node_p->Children.handles_pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_VARIABLE_STATEMENT) {
                break;
            }
            else if (((Nh_ECMAScript_ParseNode*)Node_p->Children.handles_pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_DECLARATION) {
                NH_ECMASCRIPT_END(Nh_ECMAScript_getDeclarationPart(Node_p->Children.handles_pp[0]))
            }
            else if (((Nh_ECMAScript_ParseNode*)Node_p->Children.handles_pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_HOISTABLE_DECLARATION) {
                NH_ECMASCRIPT_END(Nh_ECMAScript_getDeclarationPart(Node_p->Children.handles_pp[0]))
            }
            else if (((Nh_ECMAScript_ParseNode*)Node_p->Children.handles_pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_CLASS_DECLARATION) {
                Nh_List List = Nh_initList(8);
                Nh_appendToList(&List, Node_p->Children.handles_pp[0]);
                NH_ECMASCRIPT_END(List)
            }
            else if (((Nh_ECMAScript_ParseNode*)Node_p->Children.handles_pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_ASSIGNMENT_EXPRESSION) {
                Nh_List List = Nh_initList(8);
                Nh_appendToList(&List, Node_p);
                NH_ECMASCRIPT_END(List)
            }
            break;
    }

NH_ECMASCRIPT_END(Nh_initList(8))
}

// TOP LEVEL LEXICALLY SCOPED DECLARATIONS =========================================================

// https://tc39.es/ecma262/#sec-static-semantics-toplevellexicallyscopeddeclarations
Nh_List Nh_ECMAScript_getTopLevelLexicallyScopedDeclarations(
    Nh_ECMAScript_ParseNode *Node_p)
{
NH_ECMASCRIPT_BEGIN()

    switch (Node_p->type)
    {
        case NH_ECMASCRIPT_PARSE_NODE_BLOCK :
            if (Node_p->Children.size == 1) {
                NH_ECMASCRIPT_END(Nh_ECMAScript_getTopLevelLexicallyScopedDeclarations(Node_p->Children.handles_pp[0]))
            }
            break;

        case NH_ECMASCRIPT_PARSE_NODE_STATEMENT_LIST :
        {
            Nh_List Declarations = Nh_ECMAScript_getTopLevelLexicallyScopedDeclarations(Node_p->Children.handles_pp[0]);
            if (Node_p->Children.size == 2) {
                Nh_List StatementDeclarations = Nh_ECMAScript_getTopLevelLexicallyScopedDeclarations(Node_p->Children.handles_pp[1]);
                Nh_appendItemsToList(&Declarations, &StatementDeclarations);
                Nh_freeList(&StatementDeclarations, NH_FALSE);
            }
            NH_ECMASCRIPT_END(Declarations)
        }

        case NH_ECMASCRIPT_PARSE_NODE_STATEMENT_LIST_ITEM :
        {
            if (((Nh_ECMAScript_ParseNode*)Node_p->Children.handles_pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_DECLARATION) 
            {
                Nh_ECMAScript_ParseNode *Declaration_p = Node_p->Children.handles_pp[0];
                if (((Nh_ECMAScript_ParseNode*)Declaration_p->Children.handles_pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_HOISTABLE_DECLARATION) {
                    break;
                }
                Nh_List List = Nh_initList(8);
                Nh_appendToList(&List, Node_p->Children.handles_pp[0]);
                NH_ECMASCRIPT_END(List)
            }
            break;
        }
    }

NH_ECMASCRIPT_END(Nh_initList(8))
}

// VAR SCOPED DECLARATIONS ===================================================================

// https://tc39.es/ecma262/#sec-static-semantics-varscopeddeclarations
Nh_List Nh_ECMAScript_getVarScopedDeclarations(
    Nh_ECMAScript_ParseNode *Node_p)
{
NH_ECMASCRIPT_BEGIN()

    switch (Node_p->type)
    {
        case NH_ECMASCRIPT_PARSE_NODE_STATEMENT :
            if (((Nh_ECMAScript_ParseNode*)Node_p->Children.handles_pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_EMPTY_STATEMENT
            ||  ((Nh_ECMAScript_ParseNode*)Node_p->Children.handles_pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_EXPRESSION_STATEMENT
            ||  ((Nh_ECMAScript_ParseNode*)Node_p->Children.handles_pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_CONTINUE_STATEMENT
            ||  ((Nh_ECMAScript_ParseNode*)Node_p->Children.handles_pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_BREAK_STATEMENT
            ||  ((Nh_ECMAScript_ParseNode*)Node_p->Children.handles_pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_RETURN_STATEMENT
            ||  ((Nh_ECMAScript_ParseNode*)Node_p->Children.handles_pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_THROW_STATEMENT
            ||  ((Nh_ECMAScript_ParseNode*)Node_p->Children.handles_pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_DEBUGGER_STATEMENT) {
                break;
            }
            NH_ECMASCRIPT_END(Nh_ECMAScript_getVarScopedDeclarations(Node_p->Children.handles_pp[0]))

        case NH_ECMASCRIPT_PARSE_NODE_VARIABLE_STATEMENT :
            NH_ECMASCRIPT_END(Nh_ECMAScript_getVarScopedDeclarations(Node_p->Children.handles_pp[0]))

        case NH_ECMASCRIPT_PARSE_NODE_BLOCK :
            NH_ECMASCRIPT_END(Nh_ECMAScript_getVarScopedDeclarations(Node_p->Children.handles_pp[0]))

        case NH_ECMASCRIPT_PARSE_NODE_STATEMENT_LIST :
        {
            Nh_List Declarations = Nh_ECMAScript_getVarScopedDeclarations(Node_p->Children.handles_pp[0]);
            if (Node_p->Children.size == 2) {
                Nh_List StatementDeclarations = Nh_ECMAScript_getVarScopedDeclarations(Node_p->Children.handles_pp[1]);
                Nh_appendItemsToList(&Declarations, &StatementDeclarations);
                Nh_freeList(&StatementDeclarations, NH_FALSE);
            }
            NH_ECMASCRIPT_END(Declarations)
        }

        case NH_ECMASCRIPT_PARSE_NODE_STATEMENT_LIST_ITEM :
            if (((Nh_ECMAScript_ParseNode*)Node_p->Children.handles_pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_STATEMENT) {
                NH_ECMASCRIPT_END(Nh_ECMAScript_getVarScopedDeclarations(Node_p->Children.handles_pp[0]))
            }
            break;

        case NH_ECMASCRIPT_PARSE_NODE_VARIABLE_DECLARATION_LIST :
            if (Node_p->Children.size == 1) {
                Nh_List Declarations = Nh_initList(8); 
                Nh_appendToList(&Declarations, Node_p->Children.handles_pp[0]);
                NH_ECMASCRIPT_END(Declarations)
            }
            else {
                Nh_List Declarations = Nh_ECMAScript_getVarScopedDeclarations(Node_p->Children.handles_pp[0]); 
                Nh_appendToList(&Declarations, Node_p->Children.handles_pp[1]);
                NH_ECMASCRIPT_END(Declarations)
            }

        case NH_ECMASCRIPT_PARSE_NODE_IF_STATEMENT :
            if (Node_p->Children.size == 3) {
                Nh_List Declarations = Nh_ECMAScript_getVarScopedDeclarations(Node_p->Children.handles_pp[1]);
                Nh_List Tmp = Nh_ECMAScript_getVarScopedDeclarations(Node_p->Children.handles_pp[2]);
                Nh_appendItemsToList(&Declarations, &Tmp);
                Nh_freeList(&Tmp, NH_FALSE);
                NH_ECMASCRIPT_END(Declarations)
            }
            else {
                NH_ECMASCRIPT_END(Nh_ECMAScript_getVarScopedDeclarations(Node_p->Children.handles_pp[1]))
            }
            break;

        case NH_ECMASCRIPT_PARSE_NODE_DO_WHILE_STATEMENT :
            NH_ECMASCRIPT_END(Nh_ECMAScript_getVarScopedDeclarations(Node_p->Children.handles_pp[0]))

        case NH_ECMASCRIPT_PARSE_NODE_WHILE_STATEMENT :
            NH_ECMASCRIPT_END(Nh_ECMAScript_getVarScopedDeclarations(Node_p->Children.handles_pp[1]))

        case NH_ECMASCRIPT_PARSE_NODE_FOR_STATEMENT :
            if (((Nh_ECMAScript_ParseNode*)Node_p->Children.handles_pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_VARIABLE_DECLARATION_LIST) {
                Nh_List Declarations = Nh_ECMAScript_getVarScopedDeclarations(Node_p->Children.handles_pp[0]);
                Nh_List StatementDeclarations = Nh_ECMAScript_getVarScopedDeclarations(Node_p->Children.handles_pp[Node_p->Children.size - 1]);
                Nh_appendItemsToList(&Declarations, &StatementDeclarations);
                Nh_freeList(&StatementDeclarations, NH_FALSE);
                NH_ECMASCRIPT_END(Declarations)
            }
            else {
                NH_ECMASCRIPT_END(Nh_ECMAScript_getVarScopedDeclarations(Node_p->Children.handles_pp[Node_p->Children.size - 1]))
            }

        case NH_ECMASCRIPT_PARSE_NODE_FOR_IN_OF_STATEMENT :
            if (((Nh_ECMAScript_ParseNode*)Node_p->Children.handles_pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_FOR_BINDING) {
                Nh_List Declarations = Nh_initList(8);
                Nh_appendToList(&Declarations, Node_p->Children.handles_pp[0]);
                Nh_List StatementDeclarations = Nh_ECMAScript_getVarScopedDeclarations(Node_p->Children.handles_pp[Node_p->Children.size - 1]);
                Nh_appendItemsToList(&Declarations, &StatementDeclarations);
                Nh_freeList(&StatementDeclarations, NH_FALSE);
                NH_ECMASCRIPT_END(Declarations)
            }
            else {
                NH_ECMASCRIPT_END(Nh_ECMAScript_getVarScopedDeclarations(Node_p->Children.handles_pp[Node_p->Children.size - 1]))
            }
            break;

        case NH_ECMASCRIPT_PARSE_NODE_WITH_STATEMENT :
            NH_ECMASCRIPT_END(Nh_ECMAScript_getVarScopedDeclarations(Node_p->Children.handles_pp[1]))

        case NH_ECMASCRIPT_PARSE_NODE_SWITCH_STATEMENT :
            NH_ECMASCRIPT_END(Nh_ECMAScript_getVarScopedDeclarations(Node_p->Children.handles_pp[1]))

//        case NH_ECMASCRIPT_PARSE_NODE_CASE_BLOCK :
//            if (Node_p->Children.size == 1 && ((Nh_ECMAScript_ParseNode*)Node_p->Children.handles_pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_CASE_CLAUSES) {
//                NH_ECMASCRIPT_END(Nh_ECMAScript_getVarScopedDeclarations(Node_p->Children.handles_pp[0]))
//            }
//            else if (Node_p->Children.size > 0)
//            {
//                Nh_List Declarations = Nh_initList(8);
//                if (((Nh_ECMAScript_ParseNode*)Node_p->Children.handles_pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_CASE_CLAUSES) {
//                    Declarations = Nh_ECMAScript_getVarScopedDeclarations(Node_p->Children.handles_pp[0]);
//                    Nh_List DefaultClauseDeclarations = Nh_ECMAScript_getVarScopedDeclarations(Node_p->Children.handles_pp[1]);
//                    for (int i = 0; i < DefaultClauseDeclarations.size; ++i) {
//                        Nh_appendToList(&Declarations, DefaultClauseDeclarations.handles_pp[i]);
//                    }
//                    Nh_freeList(&DefaultClauseDeclarations, NH_FALSE);
//                    if (Node_p->Children.size > 2) {
//                        Nh_List CaseClausesDeclarations = Nh_ECMAScript_getVarScopedDeclarations(Node_p->Children.handles_pp[1]);
//                        for (int i = 0; i < CaseClausesDeclarations.size; ++i) {
//                            Nh_appendToList(&Declarations, CaseClausesDeclarations.handles_pp[i]);
//                        }
//                        Nh_freeList(&CaseClausesDeclarations, NH_FALSE);
//                    }
//                }
//                else {
//                    Declarations = Nh_ECMAScript_getVarScopedDeclarations(Node_p->Children.handles_pp[0]);
//                    if (Node_p->Children.size > 1) {
//                        Nh_List CaseClausesDeclarations = Nh_ECMAScript_getVarScopedDeclarations(Node_p->Children.handles_pp[1]);
//                        for (int i = 0; i < CaseClausesDeclarations.size; ++i) {
//                            Nh_appendToList(&Declarations, CaseClausesDeclarations.handles_pp[i]);
//                        }
//                        Nh_freeList(&CaseClausesDeclarations, NH_FALSE);
//                    }
//                }
//                NH_ECMASCRIPT_END(Declarations)
//            }
//            break;
//
        case NH_ECMASCRIPT_PARSE_NODE_CASE_CLAUSES :
        {
            Nh_List Declarations = Nh_ECMAScript_getVarScopedDeclarations(Node_p->Children.handles_pp[0]);
            if (Node_p->Children.size == 2) {
                Nh_List CaseDeclarations = Nh_ECMAScript_getVarScopedDeclarations(Node_p->Children.handles_pp[1]);
                Nh_appendItemsToList(&Declarations, &CaseDeclarations);
                Nh_freeList(&CaseDeclarations, NH_FALSE);
            }
            NH_ECMASCRIPT_END(Declarations)
        }

        case NH_ECMASCRIPT_PARSE_NODE_CASE_CLAUSE :
            if (Node_p->Children.size == 2) {
                NH_ECMASCRIPT_END(Nh_ECMAScript_getVarScopedDeclarations(Node_p->Children.handles_pp[1]))
            }
            break;

        case NH_ECMASCRIPT_PARSE_NODE_DEFAULT_CLAUSE :
            if (Node_p->Children.size == 1) {
                NH_ECMASCRIPT_END(Nh_ECMAScript_getVarScopedDeclarations(Node_p->Children.handles_pp[0]))
            }
            break;

        case NH_ECMASCRIPT_PARSE_NODE_LABELLED_STATEMENT :
            NH_ECMASCRIPT_END(Nh_ECMAScript_getVarScopedDeclarations(Node_p->Children.handles_pp[1]))

        case NH_ECMASCRIPT_PARSE_NODE_LABELLED_ITEM :
            if (((Nh_ECMAScript_ParseNode*)Node_p->Children.handles_pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_STATEMENT) {
                NH_ECMASCRIPT_END(Nh_ECMAScript_getVarScopedDeclarations(Node_p->Children.handles_pp[0]))
            }
            break;

        case NH_ECMASCRIPT_PARSE_NODE_TRY_STATEMENT :
            if (Node_p->Children.size == 2) {
                Nh_List Declarations = Nh_ECMAScript_getVarScopedDeclarations(Node_p->Children.handles_pp[0]);
                Nh_List CatchDeclarations = Nh_ECMAScript_getVarScopedDeclarations(Node_p->Children.handles_pp[1]);
                Nh_appendItemsToList(&Declarations, &CatchDeclarations);
                Nh_freeList(&CatchDeclarations, NH_FALSE);
                NH_ECMASCRIPT_END(Declarations)
            }
            else if (Node_p->Children.size == 3) {
                Nh_List Declarations = Nh_ECMAScript_getVarScopedDeclarations(Node_p->Children.handles_pp[0]);
                Nh_List CatchDeclarations = Nh_ECMAScript_getVarScopedDeclarations(Node_p->Children.handles_pp[1]);
                Nh_List FinallyDeclarations = Nh_ECMAScript_getVarScopedDeclarations(Node_p->Children.handles_pp[2]);
                Nh_appendItemsToList(&Declarations, &CatchDeclarations);
                Nh_appendItemsToList(&Declarations, &FinallyDeclarations);
                Nh_freeList(&CatchDeclarations, NH_FALSE);
                Nh_freeList(&FinallyDeclarations, NH_FALSE);
                NH_ECMASCRIPT_END(Declarations)
            }
            break;

        case NH_ECMASCRIPT_PARSE_NODE_CATCH :
            NH_ECMASCRIPT_END(Nh_ECMAScript_getVarScopedDeclarations(Node_p->Children.handles_pp[Node_p->Children.size - 1]))

        case NH_ECMASCRIPT_PARSE_NODE_FUNCTION_STATEMENT_LIST :
            if (Node_p->Children.size > 0) {
                NH_ECMASCRIPT_END(Nh_ECMAScript_getTopLevelVarScopedDeclarations(Node_p->Children.handles_pp[0]))
            }
            break;

        case NH_ECMASCRIPT_PARSE_NODE_CONCISE_BODY :
            if (((Nh_ECMAScript_ParseNode*)Node_p->Children.handles_pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_FUNCTION_BODY) {
                Nh_ECMAScript_ParseNode *FunctionBody_p = Node_p->Children.handles_pp[0];
                NH_ECMASCRIPT_END(Nh_ECMAScript_getVarScopedDeclarations(FunctionBody_p->Children.handles_pp[0]))
            }
            break;

        case NH_ECMASCRIPT_PARSE_NODE_ASYNC_CONCISE_BODY :
            if (((Nh_ECMAScript_ParseNode*)Node_p->Children.handles_pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_ASYNC_FUNCTION_BODY) {
                Nh_ECMAScript_ParseNode *AsyncFunctionBody_p = Node_p->Children.handles_pp[0];
                Nh_ECMAScript_ParseNode *FunctionBody_p = AsyncFunctionBody_p->Children.handles_pp[0];
                NH_ECMASCRIPT_END(Nh_ECMAScript_getVarScopedDeclarations(FunctionBody_p->Children.handles_pp[0]))
            }
            break;

        case NH_ECMASCRIPT_PARSE_NODE_SCRIPT_BODY :
            NH_ECMASCRIPT_END(Nh_ECMAScript_getTopLevelVarScopedDeclarations(Node_p->Children.handles_pp[0]))

        case NH_ECMASCRIPT_PARSE_NODE_MODULE :
            if (Node_p->Children.size > 0) {
                NH_ECMASCRIPT_END(Nh_ECMAScript_getVarScopedDeclarations(Node_p->Children.handles_pp[0])) 
            }

        case NH_ECMASCRIPT_PARSE_NODE_MODULE_BODY :
            NH_ECMASCRIPT_END(Nh_ECMAScript_getVarScopedDeclarations(Node_p->Children.handles_pp[0]))

        case NH_ECMASCRIPT_PARSE_NODE_MODULE_ITEM_LIST :
        {
            Nh_List Declarations = Nh_ECMAScript_getVarScopedDeclarations(Node_p->Children.handles_pp[0]);
            if (Node_p->Children.size == 2) {
                Nh_List ModuleDeclarations = Nh_ECMAScript_getVarScopedDeclarations(Node_p->Children.handles_pp[1]);
                Nh_appendItemsToList(&Declarations, &ModuleDeclarations);
                Nh_freeList(&ModuleDeclarations, NH_FALSE);
            }
            NH_ECMASCRIPT_END(Declarations)
        }

        case NH_ECMASCRIPT_PARSE_NODE_MODULE_ITEM :
            if (((Nh_ECMAScript_ParseNode*)Node_p->Children.handles_pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_STATEMENT_LIST_ITEM) {
                NH_ECMASCRIPT_END(Nh_ECMAScript_getVarScopedDeclarations(Node_p->Children.handles_pp[0]))
            }
            else if (((Nh_ECMAScript_ParseNode*)Node_p->Children.handles_pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_EXPORT_DECLARATION) {
                Nh_ECMAScript_ParseNode *ExportDeclaration_p = Node_p->Children.handles_pp[0];
                if (((Nh_ECMAScript_ParseNode*)Node_p->Children.handles_pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_VARIABLE_STATEMENT) {
                    NH_ECMASCRIPT_END(Nh_ECMAScript_getVarScopedDeclarations(ExportDeclaration_p->Children.handles_pp[0]))
                }
            }
            break;           
    }

NH_ECMASCRIPT_END(Nh_initList(8))
}

// TOP LEVEL VAR SCOPED DECLARATIONS ===============================================================

// https://tc39.es/ecma262/#sec-static-semantics-toplevelvarscopeddeclarations
Nh_List Nh_ECMAScript_getTopLevelVarScopedDeclarations(
    Nh_ECMAScript_ParseNode *Node_p)
{
NH_ECMASCRIPT_BEGIN()

    switch (Node_p->type)
    {
        case NH_ECMASCRIPT_PARSE_NODE_BLOCK :
            if (Node_p->Children.size == 1) {
                NH_ECMASCRIPT_END(Nh_ECMAScript_getTopLevelVarScopedDeclarations(Node_p->Children.handles_pp[0]))
            }
            break;

        case NH_ECMASCRIPT_PARSE_NODE_STATEMENT_LIST :
        {
            Nh_List Declarations = Nh_ECMAScript_getTopLevelVarScopedDeclarations(Node_p->Children.handles_pp[0]);
            if (Node_p->Children.size == 2) {
                Nh_List StatementDeclarations = Nh_ECMAScript_getTopLevelVarScopedDeclarations(Node_p->Children.handles_pp[1]);
                Nh_appendItemsToList(&Declarations, &StatementDeclarations);
                Nh_freeList(&StatementDeclarations, NH_FALSE);
            }
            NH_ECMASCRIPT_END(Declarations)
        }

        case NH_ECMASCRIPT_PARSE_NODE_STATEMENT_LIST_ITEM :
        {
            if (((Nh_ECMAScript_ParseNode*)Node_p->Children.handles_pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_DECLARATION) 
            {
                Nh_ECMAScript_ParseNode *Declaration_p = Node_p->Children.handles_pp[0];
                if (((Nh_ECMAScript_ParseNode*)Declaration_p->Children.handles_pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_HOISTABLE_DECLARATION) {
                    Nh_List Declaration = Nh_ECMAScript_getDeclarationPart(Declaration_p->Children.handles_pp[0]);
                    NH_ECMASCRIPT_END(Declaration)
                }
            }
            else {
                Nh_ECMAScript_ParseNode *Statement_p = Node_p->Children.handles_pp[0];
                if (((Nh_ECMAScript_ParseNode*)Statement_p->Children.handles_pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_LABELLED_STATEMENT) {
                    NH_ECMASCRIPT_END(Nh_ECMAScript_getTopLevelVarScopedDeclarations(Statement_p->Children.handles_pp[0]))
                }
                NH_ECMASCRIPT_END(Nh_ECMAScript_getVarScopedDeclarations(Statement_p))
            }
            break;
        }

        case NH_ECMASCRIPT_PARSE_NODE_LABELLED_STATEMENT :
            NH_ECMASCRIPT_END(Nh_ECMAScript_getTopLevelVarScopedDeclarations(Node_p->Children.handles_pp[1]))

        case NH_ECMASCRIPT_PARSE_NODE_LABELLED_ITEM :
            if (((Nh_ECMAScript_ParseNode*)Node_p->Children.handles_pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_STATEMENT) 
            {
                Nh_ECMAScript_ParseNode *Statement_p = Node_p->Children.handles_pp[0];
                if (((Nh_ECMAScript_ParseNode*)Statement_p->Children.handles_pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_LABELLED_STATEMENT) {
                    NH_ECMASCRIPT_END(Nh_ECMAScript_getTopLevelVarScopedDeclarations(Statement_p->Children.handles_pp[0]))
                }
                NH_ECMASCRIPT_END(Nh_ECMAScript_getVarScopedDeclarations(Statement_p))
            }
            if (((Nh_ECMAScript_ParseNode*)Node_p->Children.handles_pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_FUNCTION_DECLARATION) {
                Nh_ECMAScript_ParseNode *FunctionDeclaration_p = Node_p->Children.handles_pp[0];
                Nh_List Declarations = Nh_initList(8);
                Nh_appendToList(&Declarations, FunctionDeclaration_p);
                NH_ECMASCRIPT_END(Declarations)
            } 
            break;
    }

NH_ECMASCRIPT_END(Nh_initList(8))
}


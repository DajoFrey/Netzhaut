// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "DeclaredNames.h"
#include "BoundNames.h"

#include "../Engine/Type.h"
#include "../Common/Macro.h"
#include NH_ECMASCRIPT_FLOW
#include NH_ECMASCRIPT_CUSTOM_CHECK

#include "../../NhCore/Memory.h"
#include "../../NhCore/Unicode.h"

#include <string.h>

// LEXICALLY DECLARED NAMES ========================================================================

Nh_List Nh_ECMAScript_getLexicallyDeclaredNames(
    Nh_ECMAScript_ParseNode *Node_p)
{
NH_ECMASCRIPT_BEGIN()

    switch (Node_p->type)
    {
        case NH_ECMASCRIPT_PARSE_NODE_BLOCK :
            if (Node_p->Children.size == 3) {
                NH_ECMASCRIPT_END(Nh_ECMAScript_getLexicallyDeclaredNames(Node_p->Children.pp[1]))
            }
            break;

        case NH_ECMASCRIPT_PARSE_NODE_STATEMENT_LIST :
        {
            Nh_List Names = Nh_ECMAScript_getLexicallyDeclaredNames(Node_p->Children.pp[0]);
            if (Node_p->Children.size == 2) {
                Nh_List StatementNames = Nh_ECMAScript_getLexicallyDeclaredNames(Node_p->Children.pp[1]);
                Nh_appendItemsToList(&Names, &StatementNames);
                Nh_freeList(&StatementNames, NH_FALSE);
            }
            NH_ECMASCRIPT_END(Names)
        }

        case NH_ECMASCRIPT_PARSE_NODE_STATEMENT_LIST_ITEM :
            if (((Nh_ECMAScript_ParseNode*)Node_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_STATEMENT) {
                Nh_ECMAScript_ParseNode *Statement_p = Node_p->Children.pp[0];
                if (((Nh_ECMAScript_ParseNode*)Statement_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_LABELLED_STATEMENT) {
                    NH_ECMASCRIPT_END(Nh_ECMAScript_getLexicallyDeclaredNames(Statement_p->Children.pp[0]))
                }
            }
            else { // must be Declaration
                Nh_ECMAScript_ParseNode *Declaration_p = Node_p->Children.pp[0];
                NH_ECMASCRIPT_END(Nh_ECMAScript_getBoundNames(Declaration_p->Children.pp[0]))
            }
            break;

        case NH_ECMASCRIPT_PARSE_NODE_SCRIPT_BODY :
        {
            Nh_ECMAScript_ParseNode *StatementList_p = Node_p->Children.pp[0];
            NH_ECMASCRIPT_END(Nh_ECMAScript_getTopLevelLexicallyDeclaredNames(StatementList_p))
        }

        case NH_ECMASCRIPT_PARSE_NODE_MODULE_ITEM_LIST :
        {
            Nh_List Names = Nh_ECMAScript_getLexicallyDeclaredNames(Node_p->Children.pp[0]);
            if (Node_p->Children.size == 2) {
                Nh_List ModuleNames = Nh_ECMAScript_getLexicallyDeclaredNames(Node_p->Children.pp[1]);
                Nh_appendItemsToList(&Names, &ModuleNames);
                Nh_freeList(&ModuleNames, NH_FALSE);
            }
            NH_ECMASCRIPT_END(Names)
        }

        case NH_ECMASCRIPT_PARSE_NODE_MODULE_ITEM :
            if (((Nh_ECMAScript_ParseNode*)Node_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_IMPORT_DECLARATION) {
                NH_ECMASCRIPT_END(Nh_ECMAScript_getBoundNames(Node_p->Children.pp[0]))
            }
            else if (((Nh_ECMAScript_ParseNode*)Node_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_EXPORT_DECLARATION) {
                Nh_ECMAScript_ParseNode *ExportDeclaration_p = Node_p->Children.pp[0];
                if (((Nh_ECMAScript_ParseNode*)ExportDeclaration_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_VARIABLE_STATEMENT) {
                    break;
                } 
                NH_ECMASCRIPT_END(Nh_ECMAScript_getBoundNames(ExportDeclaration_p))
            }
            else if (((Nh_ECMAScript_ParseNode*)Node_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_STATEMENT_LIST_ITEM) {
                NH_ECMASCRIPT_END(Nh_ECMAScript_getLexicallyDeclaredNames(Node_p->Children.pp[0]))
            }          
            break;

// CaseBlock : { }
//
//    Return a new empty List.
//
//CaseBlock : { CaseClausesopt DefaultClause CaseClausesopt }
//
//    If the first CaseClauses is present, let names be the LexicallyDeclaredNames of the first CaseClauses.
//    Else, let names be a new empty List.
//    Append to names the elements of the LexicallyDeclaredNames of DefaultClause.
//    If the second CaseClauses is not present, return names.
//    Return the result of appending to names the elements of the LexicallyDeclaredNames of the second CaseClauses.
//
//CaseClauses : CaseClauses CaseClause
//
//    Let names be LexicallyDeclaredNames of CaseClauses.
//    Append to names the elements of the LexicallyDeclaredNames of CaseClause.
//    Return names.
//
//CaseClause : case Expression : StatementListopt
//
//    If the StatementList is present, return the LexicallyDeclaredNames of StatementList.
//    Return a new empty List.
//
//DefaultClause : default : StatementListopt
//
//    If the StatementList is present, return the LexicallyDeclaredNames of StatementList.
//    Return a new empty List.
//
//LabelledStatement : LabelIdentifier : LabelledItem
//
//    Return the LexicallyDeclaredNames of LabelledItem.
//
//LabelledItem : Statement
//
//    Return a new empty List.
//
//LabelledItem : FunctionDeclaration
//
//    Return BoundNames of FunctionDeclaration.
//
//FunctionStatementList : [empty]
//
//    Return a new empty List.
//
//FunctionStatementList : StatementList
//
//    Return TopLevelLexicallyDeclaredNames of StatementList.
//
//ConciseBody : ExpressionBody
//
//    Return a new empty List.
//
//AsyncConciseBody : ExpressionBody
//
//    Return a new empty List.
    }

NH_ECMASCRIPT_END(Nh_initList(8))
}

// TOP LEVEL LEXICALLY DECLARED NAMES ==============================================================

// corresponds to https://tc39.es/ecma262/#sec-static-semantics-toplevellexicallydeclarednames
Nh_List Nh_ECMAScript_getTopLevelLexicallyDeclaredNames(
    Nh_ECMAScript_ParseNode *Node_p)
{
NH_ECMASCRIPT_BEGIN()

    switch (Node_p->type)
    {
        case NH_ECMASCRIPT_PARSE_NODE_BLOCK :
            if (Node_p->Children.size == 3) {
                NH_ECMASCRIPT_END(Nh_ECMAScript_getTopLevelLexicallyDeclaredNames(Node_p->Children.pp[1]))
            }
            break;

        case NH_ECMASCRIPT_PARSE_NODE_STATEMENT_LIST :
        {
            Nh_List Names = Nh_ECMAScript_getTopLevelLexicallyDeclaredNames(Node_p->Children.pp[0]);
            if (Node_p->Children.size == 2) {
                Nh_List StatementNames = Nh_ECMAScript_getTopLevelLexicallyDeclaredNames(Node_p->Children.pp[1]);
                Nh_appendItemsToList(&Names, &StatementNames);
                Nh_freeList(&StatementNames, NH_FALSE);
            }
            NH_ECMASCRIPT_END(Names)
        }

        case NH_ECMASCRIPT_PARSE_NODE_STATEMENT_LIST_ITEM :
        {
            if (((Nh_ECMAScript_ParseNode*)Node_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_DECLARATION) 
            {
                Nh_ECMAScript_ParseNode *Declaration_p = Node_p->Children.pp[0];
                if (((Nh_ECMAScript_ParseNode*)Declaration_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_HOISTABLE_DECLARATION) {
                    break;
                }
                NH_ECMASCRIPT_END(Nh_ECMAScript_getBoundNames(Declaration_p))
            }
            break;
        }
    }

NH_ECMASCRIPT_END(Nh_initList(8))
}

// VAR DECLARED NAMES ==============================================================================

Nh_List Nh_ECMAScript_getVarDeclaredNames(
    Nh_ECMAScript_ParseNode *Node_p)
{
NH_ECMASCRIPT_BEGIN()

    switch (Node_p->type)
    {
        case NH_ECMASCRIPT_PARSE_NODE_SCRIPT_BODY :
        {
            Nh_ECMAScript_ParseNode *StatementList_p = Node_p->Children.pp[0];
            NH_ECMASCRIPT_END(Nh_ECMAScript_getTopLevelVarDeclaredNames(StatementList_p))
        }

        case NH_ECMASCRIPT_PARSE_NODE_STATEMENT :
            if (((Nh_ECMAScript_ParseNode*)Node_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_EMPTY_STATEMENT
            ||  ((Nh_ECMAScript_ParseNode*)Node_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_EXPRESSION_STATEMENT
            ||  ((Nh_ECMAScript_ParseNode*)Node_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_CONTINUE_STATEMENT
            ||  ((Nh_ECMAScript_ParseNode*)Node_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_BREAK_STATEMENT
            ||  ((Nh_ECMAScript_ParseNode*)Node_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_RETURN_STATEMENT
            ||  ((Nh_ECMAScript_ParseNode*)Node_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_THROW_STATEMENT
            ||  ((Nh_ECMAScript_ParseNode*)Node_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_DEBUGGER_STATEMENT) {
                break;
            }
            NH_ECMASCRIPT_END(Nh_ECMAScript_getVarDeclaredNames(Node_p->Children.pp[0]))

        case NH_ECMASCRIPT_PARSE_NODE_BLOCK :
            if (Node_p->Children.size == 3) {
                NH_ECMASCRIPT_END(Nh_ECMAScript_getVarDeclaredNames(Node_p->Children.pp[1]))
            }
            break;

        case NH_ECMASCRIPT_PARSE_NODE_STATEMENT_LIST :
            {
                Nh_List Names = Nh_ECMAScript_getVarDeclaredNames(Node_p->Children.pp[0]);
                if (Node_p->Children.size == 2) {
                    Nh_List StatementNames = Nh_ECMAScript_getBoundNames(Node_p->Children.pp[1]);
                    Nh_appendItemsToList(&Names, &StatementNames);
                    Nh_freeList(&StatementNames, NH_FALSE);
                }
                NH_ECMASCRIPT_END(Names)
            }
            break;

        case NH_ECMASCRIPT_PARSE_NODE_STATEMENT_LIST_ITEM :
            NH_ECMASCRIPT_END(Nh_ECMAScript_getVarDeclaredNames(Node_p->Children.pp[0]))

        case NH_ECMASCRIPT_PARSE_NODE_VARIABLE_STATEMENT :
            NH_ECMASCRIPT_END(Nh_ECMAScript_getBoundNames(Node_p->Children.pp[1]))

//        case NH_ECMASCRIPT_PARSE_NODE_IF_STATEMENT :
//            if (Node_p->Children.size == 3)
//            {
//                Nh_List FirstStatementNames = Nh_ECMAScript_getVarDeclaredNames(Node_p->Children.pp[1]);
//                Nh_List SecondStatementNames = Nh_ECMAScript_getVarDeclaredNames(Node_p->Children.pp[2]);
//                for (int i = 0; i < SecondStatementNames.size; ++i) {
//                    Nh_appendToList(&FirstStatementNames, SecondStatementNames.pp[i]);
//                }
//                Nh_freeList(&SecondStatementNames, NH_FALSE);
//                NH_ECMASCRIPT_END(FirstStatementNames)
//            }
//            else if (Node_p->Children.size == 2) {
//                NH_ECMASCRIPT_END(Nh_ECMAScript_getVarDeclaredNames(Node_p->Children.pp[1]))
//            }
//            break;
//
//        case NH_ECMASCRIPT_PARSE_NODE_DO_WHILE_STATEMENT :
//            NH_ECMASCRIPT_END(Nh_ECMAScript_getVarDeclaredNames(Node_p->Children.pp[0]))
//
//        case NH_ECMASCRIPT_PARSE_NODE_WHILE_STATEMENT :
//            NH_ECMASCRIPT_END(Nh_ECMAScript_getVarDeclaredNames(Node_p->Children.pp[1]))
//
//        case NH_ECMASCRIPT_PARSE_NODE_FOR_STATEMENT :
//            if (((Nh_ECMAScript_ParseNode*)Node_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_VARIABLE_DECLARATION_LIST) {
//                Nh_List VariableDeclarationListNames = Nh_ECMAScript_getBoundNames(Node_p->Children.pp[0]);
//                Nh_List StatementNames = Nh_ECMAScript_getVarDeclaredNames(Node_p->Children.pp[Node_p->Children.size - 1]);
//                for (int i = 0; i < StatementNames.size; ++i) {
//                    Nh_appendToList(&VariableDeclarationListNames, StatementNames.pp[i]);
//                }
//                Nh_freeList(&StatementNames, NH_FALSE);
//                NH_ECMASCRIPT_END(VariableDeclarationListNames)
//            }
//            else {
//                NH_ECMASCRIPT_END(Nh_ECMAScript_getVarDeclaredNames(Node_p->Children.pp[Node_p->Children.size - 1]))
//            }
//
//        case NH_ECMASCRIPT_PARSE_NODE_FOR_IN_OF_STATEMENT :
//            if (((Nh_ECMAScript_ParseNode*)Node_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_FOR_BINDING) {
//                Nh_List ForBindingNames = Nh_ECMAScript_getBoundNames(Node_p->Children.pp[0]);
//                Nh_List StatementNames = Nh_ECMAScript_getVarDeclaredNames(Node_p->Children.pp[Node_p->Children.size - 1]);
//                for (int i = 0; i < StatementNames.size; ++i) {
//                    Nh_appendToList(&ForBindingNames, StatementNames.pp[i]);
//                }
//                Nh_freeList(&StatementNames, NH_FALSE);
//                NH_ECMASCRIPT_END(ForBindingNames)
//            }
//            else {
//                NH_ECMASCRIPT_END(Nh_ECMAScript_getVarDeclaredNames(Node_p->Children.pp[Node_p->Children.size - 1]))
//            }
//            break;
//
//        case NH_ECMASCRIPT_PARSE_NODE_WITH_STATEMENT :
//            NH_ECMASCRIPT_END(Nh_ECMAScript_getVarDeclaredNames(Node_p->Children.pp[1]))
//
//        case NH_ECMASCRIPT_PARSE_NODE_SWITCH_STATEMENT :
//            NH_ECMASCRIPT_END(Nh_ECMAScript_getVarDeclaredNames(Node_p->Children.pp[1]))
//
//        case NH_ECMASCRIPT_PARSE_NODE_CASE_BLOCK :
//            if (Node_p->Children.size == 1 && ((Nh_ECMAScript_ParseNode*)Node_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_CASE_CLAUSES) {
//                NH_ECMASCRIPT_END(Nh_ECMAScript_getVarDeclaredNames(Node_p->Children.pp[0]))
//            }
//            else if (Node_p->Children.size > 0)
//            {
//                Nh_List Names = Nh_initList(8);
//                if (((Nh_ECMAScript_ParseNode*)Node_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_CASE_CLAUSES) {
//                    Names = Nh_ECMAScript_getVarDeclaredNames(Node_p->Children.pp[0]);
//                    Nh_List DefaultClauseNames = Nh_ECMAScript_getVarDeclaredNames(Node_p->Children.pp[1]);
//                    for (int i = 0; i < DefaultClauseNames.size; ++i) {
//                        Nh_appendToList(&Names, DefaultClauseNames.pp[i]);
//                    }
//                    Nh_freeList(&DefaultClauseNames, NH_FALSE);
//                    if (Node_p->Children.size > 2) {
//                        Nh_List CaseClausesNames = Nh_ECMAScript_getVarDeclaredNames(Node_p->Children.pp[1]);
//                        for (int i = 0; i < CaseClausesNames.size; ++i) {
//                            Nh_appendToList(&Names, CaseClausesNames.pp[i]);
//                        }
//                        Nh_freeList(&CaseClausesNames, NH_FALSE);
//                    }
//                }
//                else {
//                    Names = Nh_ECMAScript_getVarDeclaredNames(Node_p->Children.pp[0]);
//                    if (Node_p->Children.size > 1) {
//                        Nh_List CaseClausesNames = Nh_ECMAScript_getVarDeclaredNames(Node_p->Children.pp[1]);
//                        for (int i = 0; i < CaseClausesNames.size; ++i) {
//                            Nh_appendToList(&Names, CaseClausesNames.pp[i]);
//                        }
//                        Nh_freeList(&CaseClausesNames, NH_FALSE);
//                    }
//                }
//                NH_ECMASCRIPT_END(Names)
//            }
//            break;
//
//        case NH_ECMASCRIPT_PARSE_NODE_CASE_CLAUSES :
//            {
//                Nh_List Names = Nh_ECMAScript_getVarDeclaredNames(Node_p->Children.pp[0]);
//                if (Node_p->Children.size == 2) {
//                    Nh_List ClauseNames = Nh_ECMAScript_getVarDeclaredNames(Node_p->Children.pp[1]);
//                    for (int i = 0; i < ClauseNames.size; ++i) {
//                        Nh_appendToList(&Names, ClauseNames.pp[i]);
//                    }
//                    Nh_freeList(&ClauseNames, NH_FALSE);
//                }
//                NH_ECMASCRIPT_END(Names)
//            }
//            break;
//
//        case NH_ECMASCRIPT_PARSE_NODE_CASE_CLAUSE :
//            if (Node_p->Children.size == 2) {
//                NH_ECMASCRIPT_END(Nh_ECMAScript_getVarDeclaredNames(Node_p->Children.pp[1]))
//            }
//            break;
//
//        case NH_ECMASCRIPT_PARSE_NODE_DEFAULT_CLAUSE :
//            if (Node_p->Children.size == 1) {
//                NH_ECMASCRIPT_END(Nh_ECMAScript_getVarDeclaredNames(Node_p->Children.pp[0]))
//            }
//            break;
//
//        case NH_ECMASCRIPT_PARSE_NODE_LABELLED_STATEMENT :
//            NH_ECMASCRIPT_END(Nh_ECMAScript_getVarDeclaredNames(Node_p->Children.pp[1]))
//
//        case NH_ECMASCRIPT_PARSE_NODE_LABELLED_ITEM :
//            if (((Nh_ECMAScript_ParseNode*)Node_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_STATEMENT) {
//                NH_ECMASCRIPT_END(Nh_ECMAScript_getVarDeclaredNames(Node_p->Children.pp[0]))
//            }
//            break;
//
//        case NH_ECMASCRIPT_PARSE_NODE_TRY_STATEMENT :
//            if (Node_p->Children.size == 2) {
//                Nh_List BlockNames = Nh_ECMAScript_getBoundNames(Node_p->Children.pp[0]);
//                Nh_List CatchOrFinallyNames = Nh_ECMAScript_getVarDeclaredNames(Node_p->Children.pp[1]);
//                for (int i = 0; i < CatchOrFinallyNames.size; ++i) {
//                    Nh_appendToList(&BlockNames, CatchOrFinallyNames.pp[i]);
//                }
//                Nh_freeList(&CatchOrFinallyNames, NH_FALSE);
//                NH_ECMASCRIPT_END(BlockNames)
//            }
//            else if (Node_p->Children.size == 3) {
//                Nh_List BlockNames = Nh_ECMAScript_getBoundNames(Node_p->Children.pp[0]);
//                Nh_List CatchNames = Nh_ECMAScript_getVarDeclaredNames(Node_p->Children.pp[1]);
//                Nh_List FinallyNames = Nh_ECMAScript_getVarDeclaredNames(Node_p->Children.pp[2]);
//                for (int i = 0; i < CatchNames.size; ++i) {
//                    Nh_appendToList(&BlockNames, CatchNames.pp[i]);
//                }
//                for (int i = 0; i < FinallyNames.size; ++i) {
//                    Nh_appendToList(&BlockNames, FinallyNames.pp[i]);
//                }
//                Nh_freeList(&CatchNames, NH_FALSE);
//                Nh_freeList(&FinallyNames, NH_FALSE);
//                NH_ECMASCRIPT_END(BlockNames)
//            }
//            break;
//
//        case NH_ECMASCRIPT_PARSE_NODE_CATCH :
//            NH_ECMASCRIPT_END(Nh_ECMAScript_getVarDeclaredNames(Node_p->Children.pp[Node_p->Children.size - 1]))
//
//        case NH_ECMASCRIPT_PARSE_NODE_FUNCTION_STATEMENT_LIST :
//            if (Node_p->Children.size > 0) {
//                NH_ECMASCRIPT_END(Nh_ECMAScript_getTopLevelVarDeclaredNames(Node_p->Children.pp[0]))
//            }
//            break;
//
//        case NH_ECMASCRIPT_PARSE_NODE_CONCISE_BODY :
//            break;
    }

NH_ECMASCRIPT_END(Nh_initList(8))
}

// TOP LEVEL VAR DECLARED NAMES ==============================================================

Nh_List Nh_ECMAScript_getTopLevelVarDeclaredNames(
    Nh_ECMAScript_ParseNode *Node_p)
{
NH_ECMASCRIPT_BEGIN()

    switch (Node_p->type)
    {
        case NH_ECMASCRIPT_PARSE_NODE_BLOCK :
            if (Node_p->Children.size == 3) {
                NH_ECMASCRIPT_END(Nh_ECMAScript_getTopLevelVarDeclaredNames(Node_p->Children.pp[1]))
            }
            break;

        case NH_ECMASCRIPT_PARSE_NODE_STATEMENT_LIST :
            {
                Nh_List Names = Nh_ECMAScript_getTopLevelVarDeclaredNames(Node_p->Children.pp[0]);
                if (Node_p->Children.size == 2) {
                    Nh_List StatementNames = Nh_ECMAScript_getTopLevelVarDeclaredNames(Node_p->Children.pp[1]);
                    Nh_appendItemsToList(&Names, &StatementNames);
                    Nh_freeList(&StatementNames, NH_FALSE);
                }
                NH_ECMASCRIPT_END(Names)
            }
            break;

        case NH_ECMASCRIPT_PARSE_NODE_STATEMENT_LIST_ITEM :
            if (((Nh_ECMAScript_ParseNode*)Node_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_DECLARATION) 
            {
                Nh_ECMAScript_ParseNode *Declaration_p = Node_p->Children.pp[0];
                if (((Nh_ECMAScript_ParseNode*)Declaration_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_HOISTABLE_DECLARATION) {
                    NH_ECMASCRIPT_END(Nh_ECMAScript_getBoundNames(((Nh_ECMAScript_ParseNode*)Declaration_p->Children.pp[0])->Children.pp[0]))
                }
            }
            else {
                Nh_ECMAScript_ParseNode *Statement_p = Node_p->Children.pp[0];
                if (((Nh_ECMAScript_ParseNode*)Statement_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_LABELLED_STATEMENT) {
                    NH_ECMASCRIPT_END(Nh_ECMAScript_getTopLevelVarDeclaredNames(Statement_p->Children.pp[0]))
                }
                NH_ECMASCRIPT_END(Nh_ECMAScript_getVarDeclaredNames(Statement_p))
            }
            break;

        case NH_ECMASCRIPT_PARSE_NODE_LABELLED_STATEMENT :
            NH_ECMASCRIPT_END(Nh_ECMAScript_getTopLevelVarDeclaredNames(Node_p->Children.pp[2]))
            break;

        case NH_ECMASCRIPT_PARSE_NODE_LABELLED_ITEM :
            if (((Nh_ECMAScript_ParseNode*)Node_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_STATEMENT) {
                Nh_ECMAScript_ParseNode *Statement_p = Node_p->Children.pp[0];
                if (((Nh_ECMAScript_ParseNode*)Statement_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_LABELLED_STATEMENT) {
                    NH_ECMASCRIPT_END(Nh_ECMAScript_getTopLevelVarDeclaredNames(Statement_p->Children.pp[0]))
                }
                NH_ECMASCRIPT_END(Nh_ECMAScript_getVarDeclaredNames(Statement_p))
            }
            else {
                NH_ECMASCRIPT_END(Nh_ECMAScript_getBoundNames(Node_p->Children.pp[0]))
            }
            break;
    }

NH_ECMASCRIPT_END(Nh_initList(8))
}


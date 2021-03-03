// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "WebIDL.h"
#include "Util.h"

#include "Common/Macro.h"
#include NH_INSTALLER_FLOW
#include NH_INSTALLER_DEFAULT_CHECK

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

// DECLARE =========================================================================================

#define LICENSE_NOTICE \
    "/**\n * Netzhaut - Web Browser Engine\n * Copyright (C) 2020  Dajo Frey\n * Published under LGPLv3\n *\n * This file was generated by NhInstaller\n */\n\n"

static NH_BYTE *files_pp[] = {
    "src/lib/NhWebIDL/Specs/DOM/TreeWalker.idl",
    "src/lib/NhWebIDL/Specs/DOM/Text.idl",
    "src/lib/NhWebIDL/Specs/DOM/StaticRange.idl",
    "src/lib/NhWebIDL/Specs/DOM/Slottable.idl",
    "src/lib/NhWebIDL/Specs/DOM/ShadowRoot.idl",
    "src/lib/NhWebIDL/Specs/DOM/Range.idl",
    "src/lib/NhWebIDL/Specs/DOM/ProcessingInstruction.idl",
    "src/lib/NhWebIDL/Specs/DOM/ParentNode.idl",
    "src/lib/NhWebIDL/Specs/DOM/NonElementParentNode.idl",
    "src/lib/NhWebIDL/Specs/DOM/NonDocumentTypeChildNode.idl",
    "src/lib/NhWebIDL/Specs/DOM/NodeList.idl",
    "src/lib/NhWebIDL/Specs/DOM/NodeIterator.idl",
    "src/lib/NhWebIDL/Specs/DOM/NodeFilter.idl",
    "src/lib/NhWebIDL/Specs/DOM/Node.idl",
    "src/lib/NhWebIDL/Specs/DOM/NamedNodeMap.idl",
    "src/lib/NhWebIDL/Specs/DOM/MutationRecord.idl",
    "src/lib/NhWebIDL/Specs/DOM/MutationObserver.idl",
    "src/lib/NhWebIDL/Specs/DOM/HTMLCollection.idl",
    "src/lib/NhWebIDL/Specs/DOM/EventTarget.idl",
    "src/lib/NhWebIDL/Specs/DOM/Event.idl",
    "src/lib/NhWebIDL/Specs/DOM/Element.idl",
    "src/lib/NhWebIDL/Specs/DOM/DocumentType.idl",
    "src/lib/NhWebIDL/Specs/DOM/DocumentOrShadowRoot.idl",
    "src/lib/NhWebIDL/Specs/DOM/DocumentFragment.idl",
    "src/lib/NhWebIDL/Specs/DOM/Document.idl",
    "src/lib/NhWebIDL/Specs/DOM/DOMTokenList.idl",
    "src/lib/NhWebIDL/Specs/DOM/DOMImplementation.idl",
    "src/lib/NhWebIDL/Specs/DOM/Comment.idl",
    "src/lib/NhWebIDL/Specs/DOM/ChildNode.idl",
    "src/lib/NhWebIDL/Specs/DOM/CharacterData.idl",
    "src/lib/NhWebIDL/Specs/DOM/CDataSection.idl",
    "src/lib/NhWebIDL/Specs/DOM/Attr.idl",
    "src/lib/NhWebIDL/Specs/DOM/AbstractRange.idl",
    "src/lib/NhWebIDL/Specs/DOM/AbortSignal.idl",
    "src/lib/NhWebIDL/Specs/DOM/AbortController.idl",
    "src/lib/NhWebIDL/Specs/HTML/Document.idl",
    "src/lib/NhWebIDL/Specs/HTML/DocumentOrShadowRoot.idl",
    "src/lib/NhWebIDL/Specs/HTML/HTMLElement.idl",
    "src/lib/NhWebIDL/Specs/HTML/HTMLOrSVGElement.idl",
};

static int fileCount = sizeof(files_pp)/sizeof(files_pp[0]);
static NH_BYTE *totalOperations_pp[16384] = {'\0'}; // should suffice
static int totalOperationCount = 0;

// HELPER ==========================================================================================

static NH_INSTALLER_RESULT Nh_Installer_getFragmentName(
    NH_BYTE *filepath_p, NH_BYTE *fragmentname_p)
{
NH_INSTALLER_BEGIN()

    filepath_p = filepath_p + strlen(filepath_p);
    while (*filepath_p != '/') {filepath_p = filepath_p - 1;}
    filepath_p = filepath_p - 1;
    while (*filepath_p != '/') {filepath_p = filepath_p - 1;}
    filepath_p = filepath_p + 1;
    for (int i = 0; strcmp(filepath_p, ".idl"); ++i) {fragmentname_p[i] = *filepath_p; filepath_p = filepath_p + 1;}
    for (int i = 0; i < strlen(fragmentname_p); ++i) {if (fragmentname_p[i] == '/') {fragmentname_p[i] = '_';}}

NH_INSTALLER_END(NH_INSTALLER_SUCCESS)
}

//static NH_BOOL Nh_Installer_hasExtendedAttribute(
//    Nh_Array ExtendedAttributes, NH_INSTALLER_EXTENDED_ATTRIBUTE type)
//{
//NH_INSTALLER_BEGIN()
//
//    for (int i = 0; i < ExtendedAttributes.length; ++i) {
//        Nh_Installer_ExtendedAttribute *ExtendedAttribute_p = 
//            &((Nh_Installer_ExtendedAttribute*)ExtendedAttributes.bytes_p)[i];
//        if (ExtendedAttribute_p->type == type) {
//            NH_INSTALLER_END(NH_TRUE)
//        }
//    }
//
//NH_INSTALLER_END(NH_FALSE)
//}
//
//// FRAGMENT DATA HEADER ============================================================================
//
//static int fileCount = 0;
//static NH_BYTE *files_pp[2048] = {NULL};
//
//static int serializationDataCount = 0;
//static Nh_Installer_SerializationData SerializationData_p[2048];
//
//static NH_BOOL Nh_Installer_functionImplemented(
//    NH_BYTE *runtimePath_p, NH_BYTE *fragmentName_p, NH_BYTE *operationName_p)
//{
//NH_INSTALLER_BEGIN()
//
//    NH_BYTE str_p[1024] = {'\0'};
//    strcpy(str_p, runtimePath_p);
//    for (int i = strlen(str_p) - 1; str_p[i] != '/'; --i) {str_p[i] = '\0';}
//
//    NH_BYTE tmp_p[1024] = {'\0'};
//    strcpy(tmp_p, fragmentName_p);
//    for (int i = 0; i < strlen(tmp_p); ++i) {if (tmp_p[i] == '_') {tmp_p[i] = '/';}}
//    sprintf(str_p + strlen(str_p), "%s.idl.c", tmp_p);
//
//    NH_BYTE *buffer_p = 0;
//    long length;
//
//    FILE *f = fopen(str_p, "r");
//    if (f == NULL) {NH_INSTALLER_END(NH_FALSE)}
//
//    fseek (f, 0, SEEK_END);
//    length = ftell(f);
//    fseek (f, 0, SEEK_SET);
//
//    if (length > 0)
//    {
//        buffer_p = malloc(sizeof(NH_BYTE) * (length + 1));
//        if (buffer_p) {fread(buffer_p, 1, length, f);}
//        buffer_p[length] = '\0';
//
//        sprintf(str_p, "Nh_Installer_%s_%s(\n", fragmentName_p, operationName_p);
//        if (strstr(buffer_p, str_p)) {NH_INSTALLER_END(NH_TRUE)}
//
//        free(buffer_p);
//    }
//
//    fclose(f);
//
//NH_INSTALLER_END(NH_FALSE)
//}

static NH_INSTALLER_RESULT Nh_Installer_createIncludes(
    FILE *f)
{
NH_INSTALLER_BEGIN()

    fprintf(f, "// INCLUDE =========================================================================================\n\n");
    fprintf(f, "#include \"Builtin.h\"\n\n");

    for (int i = 0; i < fileCount; ++i) 
    {
        NH_BYTE fragmentName_p[1024] = {'\0'};
        Nh_Installer_getFragmentName(files_pp[i], fragmentName_p);

        for (int j = 0; j < strlen(fragmentName_p); ++j) {
            if (fragmentName_p[j] == '_') {
                fragmentName_p[j] = '/';
            }
        }

        fprintf(f,"#include \"../Specs/%s.idl.h\"\n", fragmentName_p);
    }

NH_INSTALLER_DIAGNOSTIC_END(NH_INSTALLER_SUCCESS)
}

static NH_INSTALLER_RESULT Nh_Installer_createFragmentDataArrays(
    FILE *f, int mode)
{
NH_INSTALLER_BEGIN()

    if (mode == 0) {
        fprintf(f, "\n// FRAGMENT NAMES ==================================================================================\n\n");
        fprintf(f, "NH_BYTE *NH_WEBIDL_FRAGMENT_NAMES_PP[] = {\n");
    }
    else if (mode == 1) {
        fprintf(f, "\n// FRAGMENTS =======================================================================================\n\n");
        fprintf(f, "unsigned NH_BYTE *NH_WEBIDL_FRAGMENTS_PP[] = {\n");
    }
    else if (mode == 2) {
        fprintf(f, "\n// FRAGMENT LENGTHS =================================================================================\n\n");
        fprintf(f, "const unsigned int NH_WEBIDL_FRAGMENT_LENGTHS_P[] = {\n");
    }

    for (int i = 0; i < fileCount; ++i) 
    {
        NH_BYTE fragmentName_p[1024] = {'\0'}, *p;
        Nh_Installer_getFragmentName(files_pp[i], fragmentName_p);

        if (mode == 0) {
            fprintf(f, "    \"%s\",\n", fragmentName_p);
        }
        else if (mode == 1) {
            fprintf(f, "    Nh_WebIDL_%s_p,\n", fragmentName_p);
        }
        else if (mode == 2) {
            fprintf(f, "    Nh_WebIDL_%s_p_len,\n", fragmentName_p);
        }
    }

    fprintf(f, "};\n");

    if (mode == 0) {
        fprintf(f, "\nsize_t NH_WEBIDL_FRAGMENT_NAMES_PP_COUNT = sizeof(NH_WEBIDL_FRAGMENT_NAMES_PP) / sizeof(NH_WEBIDL_FRAGMENT_NAMES_PP[0]);\n");
    }
    else if (mode == 1) {
        fprintf(f, "\nsize_t NH_WEBIDL_FRAGMENTS_PP_COUNT = sizeof(NH_WEBIDL_FRAGMENTS_PP) / sizeof(NH_WEBIDL_FRAGMENTS_PP[0]);\n");
    }

NH_INSTALLER_DIAGNOSTIC_END(NH_INSTALLER_SUCCESS)
}

//static NH_INSTALLER_RESULT Nh_Installer_createFragmentFunctionArrays(
//    FILE *f, NH_BYTE *runtimePath_p, NH_BOOL asString)
//{
//NH_INSTALLER_BEGIN()
//
//    if (asString) {
//        fprintf(f, "\n// FRAGMENT FUNCTION NAMES =========================================================================\n\n");
//        fprintf(f, "const NH_BYTE *NH_INSTALLER_FRAGMENT_FUNCTION_NAMES_PP[] = {");
//    }
//    else {
//        fprintf(f, "\n// FRAGMENT FUNCTIONS ==============================================================================\n\n");
//        fprintf(f, "Nh_Installer_Result (*NH_INSTALLER_FRAGMENT_FUNCTIONS_PP[]) (Nh_ECMAScript_Object *This_p, Nh_List Arguments) = {");
//    }
//
//    for (int i = 0; i < fileCount; ++i) 
//    {
//        Nh_Installer_Operation *Operation_p = &((Nh_Installer_Operation*)Interface_p->Operations.bytes_p)[k];
//        if (Nh_Installer_functionImplemented(runtimePath_p, fragmentName_p, Operation_p->name_p)) {
//            fprintf(f, asString ? "\n    \"%s_%s\"," : "\n    Nh_Installer_%s_%s,", fragmentName_p, Operation_p->name_p);
//        }
//        else {
//            fprintf(f, asString ? "\n//    \"%s_%s\"," : "\n//    Nh_Installer_%s_%s,", fragmentName_p, Operation_p->name_p);
//        }
//    }
//
//    fprintf(f, "\n};\n");
//
//    if (asString) {
//        fprintf(f, "\nsize_t NH_INSTALLER_FRAGMENT_FUNCTION_NAMES_PP_COUNT = sizeof(NH_INSTALLER_FRAGMENT_FUNCTION_NAMES_PP) / sizeof(NH_INSTALLER_FRAGMENT_FUNCTION_NAMES_PP[0]);\n");
//    }
//    else {
//        fprintf(f, "\nsize_t NH_INSTALLER_FRAGMENT_FUNCTIONS_PP_COUNT = sizeof(NH_INSTALLER_FRAGMENT_FUNCTIONS_PP) / sizeof(NH_INSTALLER_FRAGMENT_FUNCTIONS_PP[0]);\n");
//    }
//
//NH_INSTALLER_DIAGNOSTIC_END(NH_INSTALLER_SUCCESS)
//}

static NH_INSTALLER_RESULT Nh_Installer_createIncludeFile(
    NH_BYTE *runtimePath_p)
{
NH_INSTALLER_BEGIN()

    NH_BYTE filepath_p[1024] = {'\0'};
    sprintf(filepath_p, "%s/Builtin.c", runtimePath_p);

    FILE *f = fopen(filepath_p, "w");
    if (f == NULL) {
        printf("Error opening file!\n");
        exit(1);
    }

    fprintf(f, "// LICENSE NOTICE ==================================================================================\n\n");
    fprintf(f, LICENSE_NOTICE);

    Nh_Installer_createIncludes(f);
    Nh_Installer_createFragmentDataArrays(f, 0);
    Nh_Installer_createFragmentDataArrays(f, 1);
    Nh_Installer_createFragmentDataArrays(f, 2);
//    Nh_Installer_createFragmentFunctions(f, runtimePath_p, NH_TRUE);
//    Nh_Installer_createFragmentFunctions(f, runtimePath_p, NH_FALSE);

    fclose(f);

NH_INSTALLER_DIAGNOSTIC_END(NH_INSTALLER_SUCCESS)
}

// HEADER ===========================================================================================

static NH_INSTALLER_RESULT Nh_Installer_generateOperationsFile(
    NH_BYTE *fragmentName_p, NH_BYTE *outdir_p, NH_BYTE **operations_pp, int count)
{
NH_INSTALLER_BEGIN()

    NH_BYTE *shortName_p = NULL;
    for (int i = 0; i < strlen(fragmentName_p); ++i) {
        if (fragmentName_p[i] == '_') {shortName_p = &fragmentName_p[i + 1]; break;}
    }
    NH_INSTALLER_CHECK_NULL(shortName_p)

    NH_BYTE outfilepath_p[1024] = {'\0'};
    sprintf(outfilepath_p, "%s/%s.h", outdir_p, shortName_p);

    FILE *File_p = fopen(outfilepath_p, "w");
    NH_INSTALLER_CHECK_NULL(File_p)

    NH_BYTE fragmentNameUpper_p[1024] = {'\0'};
    for (int i = 0; i < strlen(fragmentName_p); ++i) {
        fragmentNameUpper_p[i] = toupper(fragmentName_p[i]);
    }

    fprintf(File_p,"#ifndef NH_%s_H\n#define NH_%s_H\n\n", fragmentNameUpper_p, fragmentNameUpper_p);

    fprintf(File_p,"#ifndef DOXYGEN_SHOULD_SKIP_THIS\n\n");
    fprintf(File_p, LICENSE_NOTICE);
    fprintf(File_p,"#endif\n\n");

    fprintf(File_p,"/** @addtogroup NhFunctions Functions\n *  @{\n */\n");

    for (int i = 0; i < count; ++i) {
        fprintf(
            File_p, 
            "\n    Nh_WebIDL_Result Nh_%s_%s(\n        Nh_WebIDL_Object *This_p, Nh_List Arguments\n    );\n", 
            fragmentName_p, operations_pp[i]
        );
    }

    fprintf(File_p,"\n/** @} */\n\n#endif");
    fclose(File_p);

    NH_INSTALLER_CHECK(Nh_Installer_messagef("GENERATE %s", outfilepath_p))

NH_INSTALLER_DIAGNOSTIC_END(NH_INSTALLER_SUCCESS)
}

static NH_INSTALLER_RESULT Nh_Installer_generateHexDumpFile(
    NH_BYTE *filePath_p, NH_BYTE *fragmentName_p)
{
NH_INSTALLER_BEGIN()

    NH_BYTE outfilepath_p[1024] = {'\0'};
    sprintf(outfilepath_p, "%s.h", filePath_p);

    FILE *File_p = fopen(outfilepath_p, "w");
    NH_INSTALLER_CHECK_NULL(File_p)

    NH_BYTE name_p[1024] = {'\0'};
    sprintf(name_p, "Nh_WebIDL_%s", fragmentName_p);

    NH_BYTE command_p[1024] = {'\0'};
    sprintf(command_p, "./bin/external/xxd -i %s %s.h Nh_WebIDL_%s_p", filePath_p, filePath_p, fragmentName_p);

    int status = system(command_p);
    if (WEXITSTATUS(status) || WIFSIGNALED(status)) {NH_INSTALLER_DIAGNOSTIC_END(NH_INSTALLER_ERROR_XXD_EXECUTION_FAILED)}

    fclose(File_p);

    NH_INSTALLER_CHECK(Nh_Installer_messagef("GENERATE %s", outfilepath_p))

NH_INSTALLER_DIAGNOSTIC_END(NH_INSTALLER_SUCCESS)
}

// GET OPERATIONS ==================================================================================

NH_INSTALLER_RESULT Nh_Installer_getFragmentOperations(
    NH_BYTE *filePath_p, NH_BYTE **operations_pp, int *count_p)
{
NH_INSTALLER_BEGIN()

    long size = 0;
    NH_BYTE *bytes_p = Nh_Installer_getFileData(filePath_p, &size);
    NH_INSTALLER_CHECK_NULL(bytes_p)

    int operation = 0;

    for (int i = 0, depth = 0; i < size; ++i) 
    {
             if (bytes_p[i] == '{') {depth++;}
        else if (bytes_p[i] == '}') {depth--;}
        else if (depth > 0) 
        {
            if (bytes_p[i] == '(') 
            {
                bytes_p[i] = '\0';
                int begin = i;
                while (bytes_p[begin] == ' ' && begin > 0) {--begin;}
                while (bytes_p[begin] != ' ' && begin > 0) {--begin;}
                NH_BYTE *operation_p = malloc(sizeof(NH_BYTE) * ((i - begin) + 1));
                NH_INSTALLER_CHECK_NULL(operation_p)
                strcpy(operation_p, bytes_p + begin + 1);
                operations_pp[operation++] = operation_p;
                bytes_p[i] = '(';
                while (bytes_p[i] != ';' && i < size) {++i;}
            } 
        }
    }

    *count_p = operation;
    free(bytes_p);

NH_INSTALLER_DIAGNOSTIC_END(NH_INSTALLER_SUCCESS)
}

// MAIN ============================================================================================

static NH_INSTALLER_RESULT Nh_Installer_processWebIDLFragment(
    NH_BYTE *operationsOut_p, NH_BYTE *filePath_p)
{
NH_INSTALLER_BEGIN()

    int count = 0;
    NH_INSTALLER_CHECK(Nh_Installer_getFragmentOperations(filePath_p, &totalOperations_pp[totalOperationCount], &count))
    totalOperationCount += count;

    NH_BYTE fragmentName_p[1024] = {'\0'};
    Nh_Installer_getFragmentName(filePath_p, fragmentName_p);

    NH_INSTALLER_CHECK(Nh_Installer_generateOperationsFile(fragmentName_p, operationsOut_p, &totalOperations_pp[totalOperationCount - count], count))
    NH_INSTALLER_CHECK(Nh_Installer_generateHexDumpFile(filePath_p, fragmentName_p))
    NH_INSTALLER_CHECK(Nh_Installer_createIncludeFile("src/lib/NhWebIDL/Runtime"))

NH_INSTALLER_DIAGNOSTIC_END(NH_INSTALLER_SUCCESS)
}

NH_INSTALLER_RESULT Nh_Installer_processWebIDLSpecs()
{
NH_INSTALLER_BEGIN()

    for (int i = 0; i < 34; ++i) {
        NH_INSTALLER_CHECK(Nh_Installer_processWebIDLFragment("src/lib/NhDOM/Interfaces", files_pp[i]))
    }
    for (int i = 34; i < 39; ++i) {
        NH_INSTALLER_CHECK(Nh_Installer_processWebIDLFragment("src/lib/NhHTML/Interfaces", files_pp[i]))
    }

    for (int i = 0; i < totalOperationCount; ++i) {free(totalOperations_pp[i]);}

NH_INSTALLER_DIAGNOSTIC_END(NH_INSTALLER_SUCCESS)
} 


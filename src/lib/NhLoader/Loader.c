// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#define _GNU_SOURCE

#include "Loader.h"
#include "Reload.h"
#include "Library.h"

#include "Common/Macro.h"
#include NH_LOADER_DEFAULT_CHECK
#include NH_LOADER_FLOW

#include <link.h>
#include <dlfcn.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>

// DATA ============================================================================================

Nh_Loader NH_LOADER;

// MODULE ==========================================================================================

static Nh_Loader_Module Nh_Loader_initModule()
{
NH_LOADER_BEGIN()

    Nh_Loader_Module Module;
    Module.lib_p = NULL;
    Module.major = 0;
    Module.name_p = NULL;
    Module.lastModified_p = NULL;

NH_LOADER_END(Module)
}

static Nh_Loader_Module *Nh_Loader_getModule(
    const NH_BYTE *name_p, int major)
{
NH_LOADER_BEGIN()

    for (int i = 0; i < NH_LOADER_MAX_MODULES; ++i) {
        if (NH_LOADER.Modules_p[i].name_p != NULL && !strcmp(name_p, NH_LOADER.Modules_p[i].name_p) && NH_LOADER.Modules_p[i].major == major) {
            NH_LOADER_END(&NH_LOADER.Modules_p[i])
        }
    }

NH_LOADER_END(NULL)
}

// LOAD LIBRARY ====================================================================================

typedef int (*initialize_f)();

static NH_LOADER_RESULT Nh_Loader_callDefaultInitializer(
    Nh_Loader_Module *Module_p)
{
NH_LOADER_BEGIN()

    NH_BYTE functionName_p[255] = {'\0'};

    if (strcmp(Module_p->name_p, "NhCore")) {
        sprintf(functionName_p, "Nh_%s_initialize", Module_p->name_p + 2);
    }
    else {
        sprintf(functionName_p, "Nh_initialize");
    }

    initialize_f initializer_f = NH_LOADER.loadFunction_f(Module_p->name_p, Module_p->major, functionName_p);
    if (initializer_f != NULL) {initializer_f();}

NH_LOADER_DIAGNOSTIC_END(NH_LOADER_SUCCESS)
}

static NH_LOADER_RESULT Nh_Loader_load(
    const NH_BYTE *name_p, int major)
{
NH_LOADER_BEGIN()

    if (Nh_Loader_getModule(name_p, major)) {NH_LOADER_DIAGNOSTIC_END(NH_LOADER_SUCCESS)}

    void *lib_p = Nh_Loader_loadLibrary((NH_BYTE*)name_p, major);
    NH_LOADER_CHECK_NULL(lib_p)

    Nh_Loader_Module *Module_p = NULL;
    for (int i = 0; i < NH_LOADER_MAX_MODULES; ++i) {
        if (NH_LOADER.Modules_p[i].name_p == NULL && NH_LOADER.Modules_p[i].lib_p == NULL) {Module_p = &NH_LOADER.Modules_p[i]; break;}
    }
    NH_LOADER_CHECK_NULL(Module_p)

    Module_p->lib_p = lib_p;
    Module_p->lastModified_p = Nh_Loader_lastModified(lib_p);

    NH_LOADER_CHECK_MEM(Module_p->lastModified_p)
    Module_p->name_p = malloc(sizeof(NH_BYTE) * (strlen(name_p) + 1));

    NH_LOADER_CHECK_MEM(Module_p->name_p)
    strcpy(Module_p->name_p, name_p);

    Nh_Loader_callDefaultInitializer(Module_p);

NH_LOADER_DIAGNOSTIC_END(NH_LOADER_SUCCESS)
}

// UNLOAD LIBRARY ==================================================================================

typedef int (*terminate_f)();

static NH_LOADER_RESULT Nh_Loader_callDefaultTerminator(
    Nh_Loader_Module *Module_p)
{
NH_LOADER_BEGIN()

    NH_BYTE functionName_p[255] = {'\0'};

    if (strcmp(Module_p->name_p, "NhCore")) {
        sprintf(functionName_p, "Nh_%s_terminate", Module_p->name_p + 2);
    }
    else {
        sprintf(functionName_p, "Nh_terminate");
    }

    terminate_f terminator_f = NH_LOADER.loadFunction_f(Module_p->name_p, Module_p->major, functionName_p);
    if (terminator_f != NULL) {terminator_f();}

NH_LOADER_DIAGNOSTIC_END(NH_LOADER_SUCCESS)
}

static NH_LOADER_RESULT Nh_Loader_unload(
    const NH_BYTE *name_p, int major)
{
NH_LOADER_BEGIN()

    Nh_Loader_Module *Module_p = Nh_Loader_getModule(name_p, major);
    NH_LOADER_CHECK_NULL(Module_p)

    NH_LOADER_CHECK(Nh_Loader_callDefaultTerminator(Module_p))
    NH_LOADER_CHECK(Nh_Loader_unloadLibrary(Module_p->lib_p))

    if (Module_p->name_p != NULL) {free(Module_p->name_p);}
    if (Module_p->lastModified_p != NULL) {free(Module_p->lastModified_p);}

    *Module_p = Nh_Loader_initModule();

NH_LOADER_DIAGNOSTIC_END(NH_LOADER_SUCCESS)
}

// LOAD FUNCTION ===================================================================================

static void *Nh_Loader_loadFunction(
    const NH_BYTE *libName_p, int major, const NH_BYTE *functionName_p)
{
NH_LOADER_BEGIN()

    Nh_Loader_Module *Module_p = Nh_Loader_getModule(libName_p, major);
    if (Module_p == NULL) {
        if (Nh_Loader_load(libName_p, major) == NH_LOADER_SUCCESS) {
            Module_p = Nh_Loader_getModule(libName_p, major);
        }
    }
    if (Module_p == NULL) {NH_LOADER_END(NULL)}

#ifdef __unix__

    NH_BYTE *error_p;
    dlerror(); // clear any existing error

    void *function_p = dlsym(Module_p->lib_p, functionName_p); 
    if ((error_p = dlerror()) != NULL) {NH_LOADER_END(NULL)}

#endif

NH_LOADER_END(function_p)
}

// INIT ============================================================================================

Nh_Loader *Nh_Loader_initLoader(
    NH_LOADER_SCOPE scope, NH_BOOL fallback, NH_BOOL install)
{
NH_LOADER_BEGIN()

    NH_LOADER.scope = scope;
    NH_LOADER.install = install;

    NH_LOADER.load_f = Nh_Loader_load; 
    NH_LOADER.unload_f = Nh_Loader_unload;
    NH_LOADER.loadFunction_f = Nh_Loader_loadFunction;

    for (int i = 0; i < NH_LOADER_MAX_MODULES; ++i) {
        NH_LOADER.Modules_p[i] = Nh_Loader_initModule();
    }

    if (fallback && install) {
        puts("TODO replace fallback loader");
        exit(0);
    }

NH_LOADER_END(&NH_LOADER)
}


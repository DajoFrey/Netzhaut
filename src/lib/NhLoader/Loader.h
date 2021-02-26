#ifndef NH_LOADER_H
#define NH_LOADER_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "Common/API.h"
#include "Common/Result.h"

#endif

/** @addtogroup NhLoaderMacros
 *  @{
 */

    #define NH_LOADER_MAX_MODULES 1024

/** @} */

/** @addtogroup NhLoaderTypedefs 
 *  @{
 */

    typedef struct Nh_Loader Nh_Loader;

    typedef Nh_Loader *(*Nh_Loader_initLoader_f)(
        NH_LOADER_SCOPE scope, NH_BOOL fallback, NH_BOOL install
    );

    typedef NH_LOADER_RESULT (*Nh_Loader_load_f)(
        const NH_BYTE *name_p, int major
    );
    
    typedef NH_LOADER_RESULT (*Nh_Loader_unload_f)(
        const NH_BYTE *name_p, int major
    );

    typedef void *(*Nh_Loader_loadFunction_f)(
        const NH_BYTE *libName_p, int major, const NH_BYTE *functionName_p
    );

/** @} */

/** @addtogroup NhLoaderStructs
 *  @{
 */

    typedef struct Nh_Loader_Module {
        NH_BYTE *name_p;
        int major;
        void *lib_p;
        NH_BYTE *lastModified_p;
    } Nh_Loader_Module;

    typedef struct Nh_Loader {
        NH_LOADER_SCOPE scope;
        NH_BOOL install;
        Nh_Loader_load_f load_f;
        Nh_Loader_unload_f unload_f;
        Nh_Loader_loadFunction_f loadFunction_f;
        Nh_Loader_Module Modules_p[NH_LOADER_MAX_MODULES];
    } Nh_Loader;

/** @} */

/** @addtogroup NhLoaderVars
 *  @{
 */

    extern Nh_Loader NH_LOADER;

/** @} */

/** @addtogroup NhLoaderFunctions
 *  @{
 */

    Nh_Loader *Nh_Loader_initLoader(
        NH_LOADER_SCOPE scope, NH_BOOL fallback, NH_BOOL install
    );

/** @} */

#endif

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

/** @addtogroup CoreFunctions Functions
 *  \ingroup Core 
 *  @{
 */

    typedef struct Nh_Loader Nh_Loader;

    typedef Nh_Loader *(*Nh_Loader_getLoader_f)(
    );

    typedef NH_LOADER_RESULT (*Nh_Loader_load_f)(
        const NH_BYTE *name_p, int major, NH_BOOL install
    );
    
    typedef NH_LOADER_RESULT (*Nh_Loader_unload_f)(
        const NH_BYTE *name_p, int major
    );

    typedef NH_BOOL (*Nh_Loader_isLoaded_f)(
        const NH_BYTE *name_p, int major
    );

    typedef void *(*Nh_Loader_loadFunction_f)(
        const NH_BYTE *libName_p, int major, const NH_BYTE *functionName_p
    );

    typedef NH_BYTE *(*Nh_Loader_getModifiedLib_f)(
    );

/** @} */

/** @addtogroup CoreFunctions Functions
 *  \ingroup Core 
 *  @{
 */

    typedef struct Nh_Loader {
        Nh_Loader_load_f load_f;
        Nh_Loader_unload_f unload_f;
        Nh_Loader_isLoaded_f isLoaded_f;
        Nh_Loader_loadFunction_f loadFunction_f;
        Nh_Loader_getModifiedLib_f getModifiedLib_f;
    } Nh_Loader;

    extern Nh_Loader NH_LOADER;

/** @} */

/** @addtogroup CoreFunctions Functions
 *  \ingroup Core 
 *  @{
 */

    Nh_Loader *Nh_Loader_getLoader(
    );

    NH_LOADER_RESULT Nh_Loader_load(
        const NH_BYTE *name_p, int major, NH_BOOL install
    );
    
    NH_LOADER_RESULT Nh_Loader_unload(
        const NH_BYTE *name_p, int major
    );

    NH_BOOL Nh_Loader_isLoaded(
        const NH_BYTE *name_p, int major
    );

    void *Nh_Loader_loadFunction(
        const NH_BYTE *libName_p, int major, const NH_BYTE *functionName_p
    );

    NH_BYTE *Nh_Loader_getModifiedLib(
    );

/** @} */

#endif

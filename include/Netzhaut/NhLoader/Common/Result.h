#ifndef NH_LOADER_RESULT_H
#define NH_LOADER_RESULT_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#endif

/** @addtogroup NhLoaderEnums
 *  @{
 */ 

    /**
     * Return values for functions.
     */
    typedef enum NH_LOADER_RESULT 
    {
        NH_LOADER_SUCCESS, /**<Indicates that something worked as planned.*/                   
        NH_LOADER_ERROR_NULL_POINTER,         
        NH_LOADER_ERROR_BAD_STATE, 
        NH_LOADER_ERROR_MEMORY_ALLOCATION,

    } NH_LOADER_RESULT;

/** @} */

/** @addtogroup NhLoaderVars
 *  @{
 */ 

    extern const char *NH_LOADER_RESULTS_PP[];
    extern unsigned int NH_LOADER_RESULTS_PP_COUNT;

/** @} */

#endif

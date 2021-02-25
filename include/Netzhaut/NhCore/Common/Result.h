#ifndef NH_RESULT_H
#define NH_RESULT_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#endif

/** @addtogroup NhCoreEnums
 *  @{
 */ 

    /**
     * Return values for functions.
     */
    typedef enum NH_RESULT 
    {
        NH_SUCCESS, /**<Indicates that something worked as planned.*/                   
        NH_PENDING, 
        NH_ERROR_NULL_POINTER,         
        NH_ERROR_BAD_STATE, 
        NH_ERROR_THREAD_INITIALIZATION,
        NH_ERROR_THREADPOOL_CREATION,
        NH_ERROR_MEMORY_INITIALIZATION,
        NH_ERROR_VULKAN_HOST_CREATION_FAILED,
        NH_ERROR_HTML_BASE_CREATION_FAILED,
        NH_ERROR_HTML_THREAD_START_FAILED,
        NH_ERROR_WINDOW_THREAD_START_FAILED,
        NH_ERROR_INSUFFICIENT_RESOURCES,
        NH_ERROR_MEMORY_ALLOCATION,
        NH_ERROR_PARAMETERS,
        NH_ERROR_NOT_ENOUGH_THREADS,
        NH_ERROR_UNKNOWN_FILE_FORMAT,
        NH_ERROR_TYPE_MISMATCH,
        NH_ERROR_STRING_LENGTH,
        NH_ERROR_RESIZE_NEEDED,
        NH_ERROR_INVALID_FILE_FORMAT,
        NH_ERROR_EXPECTED_TRUE,
        NH_ERROR_EXPECTED_FALSE,
        NH_ERROR_INVALID_ARGUMENT,

        NH_USAGE_ERROR_ALREADY_INITIALIZED,
        NH_USAGE_ERROR_INIT_MISSING,
        NH_USAGE_ERROR_STILL_RUNNING,

    } NH_RESULT;

/** @} */

/** @addtogroup NhCoreVars
 *  @{
 */ 

    extern const char *NH_RESULTS_PP[];
    extern unsigned int NH_RESULTS_PP_COUNT;

/** @} */

#endif

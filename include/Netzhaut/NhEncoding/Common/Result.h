#ifndef NH_ENCODING_RESULT_H
#define NH_ENCODING_RESULT_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#endif

/** @addtogroup NhEncodingEnums 
 *  @{
 */ 

    /**
     * Return values for functions.
     */
    typedef enum NH_ENCODING_RESULT 
    {
        NH_ENCODING_SUCCESS, /**<Indicates that something worked as planned.*/                   
        NH_ENCODING_ERROR_NULL_POINTER,         
        NH_ENCODING_ERROR_BAD_STATE, 
        NH_ENCODING_ERROR_MEMORY_ALLOCATION,

    } NH_ENCODING_RESULT;

/** @} */

/** @addtogroup NhEncodingVars
 *  @{
 */ 

    extern const char *NH_ENCODING_RESULTS_PP[];
    extern unsigned int NH_ENCODING_RESULTS_PP_COUNT;

/** @} */

#endif

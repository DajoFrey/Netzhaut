#ifndef NH_DOM_RESULT_H
#define NH_DOM_RESULT_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#endif

/** @addtogroup NhDOMEnums 
 *  @{
 */ 

    /**
     * Return values for functions.
     */
    typedef enum NH_DOM_RESULT 
    {
        NH_DOM_SUCCESS, /**<Indicates that something worked as planned.*/                   
        NH_DOM_ERROR_NULL_POINTER,         
        NH_DOM_ERROR_BAD_STATE, 
        NH_DOM_ERROR_MEMORY_ALLOCATION,

    } NH_DOM_RESULT;

/** @} */

/** @addtogroup NhDOMVars
 *  @{
 */ 

    extern const char *NH_DOM_RESULTS_PP[];
    extern unsigned int NH_DOM_RESULTS_PP_COUNT;

/** @} */

#endif

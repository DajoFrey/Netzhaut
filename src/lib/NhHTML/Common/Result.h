#ifndef NH_HTML_RESULT_H
#define NH_HTML_RESULT_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#endif

/** @addtogroup NhHTMLEnums 
 *  @{
 */ 

    /**
     * Return values for functions.
     */
    typedef enum NH_HTML_RESULT 
    {
        NH_HTML_SUCCESS, /**<Indicates that something worked as planned.*/                   
        NH_HTML_ERROR_NULL_POINTER,         
        NH_HTML_ERROR_BAD_STATE, 
        NH_HTML_ERROR_MEMORY_ALLOCATION,

    } NH_HTML_RESULT;

/** @} */

/** @addtogroup NhHTMLVars
 *  @{
 */ 

    extern const char *NH_HTML_RESULTS_PP[];
    extern unsigned int NH_HTML_RESULTS_PP_COUNT;

/** @} */

#endif

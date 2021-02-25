#ifndef NH_NET_RESULT_H
#define NH_NET_RESULT_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#endif

/** @addtogroup NetworkEnums Enums
 *  \ingroup Network
 *  @{
 */ 

    /**
     * Return values for functions.
     */
    typedef enum NH_NET_RESULT 
    {
        NH_NET_SUCCESS, /**<Indicates that something worked as planned.*/                   
        NH_NET_ERROR_NULL_POINTER,         
        NH_NET_ERROR_BAD_STATE, 
        NH_NET_ERROR_MEMORY_ALLOCATION,

    } NH_NET_RESULT;

/** @} */

/** @addtogroup NetworkData Data
 *  \ingroup Network
 *  @{
 */ 

    extern const char *NH_NET_RESULTS_PP[];
    extern unsigned int NH_NET_RESULTS_PP_COUNT;

/** @} */

#endif

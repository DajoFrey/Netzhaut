#ifndef NH_WSI_RESULT_H
#define NH_WSI_RESULT_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#endif

/** @addtogroup IOEnum Enum
 *  \ingroup IO
 *  @{
 */ 

    /**
     * Return values for functions.
     */
    typedef enum NH_WSI_RESULT 
    {
        NH_WSI_SUCCESS, /**<Indicates that something worked as planned.*/                   
        NH_WSI_ERROR_NULL_POINTER,         
        NH_WSI_ERROR_BAD_STATE, 
        NH_WSI_ERROR_MEMORY_ALLOCATION,

    } NH_WSI_RESULT;

/** @} */

/** @addtogroup IOData Data
 *  \ingroup IO 
 *  @{
 */ 

    extern const char *NH_WSI_RESULTS_PP[];
    extern unsigned int NH_WSI_RESULTS_PP_COUNT;

/** @} */

#endif

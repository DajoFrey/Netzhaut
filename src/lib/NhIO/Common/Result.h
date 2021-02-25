#ifndef NH_IO_RESULT_H
#define NH_IO_RESULT_H

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
    typedef enum NH_IO_RESULT 
    {
        NH_IO_SUCCESS, /**<Indicates that something worked as planned.*/                   
        NH_IO_ERROR_NULL_POINTER,         
        NH_IO_ERROR_BAD_STATE, 
        NH_IO_ERROR_MEMORY_ALLOCATION,

    } NH_IO_RESULT;

/** @} */

/** @addtogroup IOData Data
 *  \ingroup IO 
 *  @{
 */ 

    extern const char *NH_IO_RESULTS_PP[];
    extern unsigned int NH_IO_RESULTS_PP_COUNT;

/** @} */

#endif

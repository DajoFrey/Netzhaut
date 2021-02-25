#ifndef NH_TTY_RESULT_H
#define NH_TTY_RESULT_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#endif

/** @addtogroup NhTTYEnums 
 *  @{
 */ 

    /**
     * Return values for functions.
     */
    typedef enum NH_TTY_RESULT 
    {
        NH_TTY_SUCCESS, /**<Indicates that something worked as planned.*/                   
        NH_TTY_ERROR_NULL_POINTER,         
        NH_TTY_ERROR_BAD_STATE, 
        NH_TTY_ERROR_MEMORY_ALLOCATION,
        NH_TTY_ERROR_TERMINFO_DATA_CANNOT_BE_FOUND,
        NH_TTY_ERROR_UNKNOWN_TERMINAL_TYPE,
        NH_TTY_ERROR_TERMINAL_IS_HARDCOPY,
        NH_TTY_ERROR_UNKNOWN_COMMAND,

    } NH_TTY_RESULT;

/** @} */

/** @addtogroup NhTTYVars
 *  @{
 */ 

    extern const char *NH_TTY_RESULTS_PP[];
    extern unsigned int NH_TTY_RESULTS_PP_COUNT;

/** @} */

#endif

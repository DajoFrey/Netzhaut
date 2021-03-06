#ifndef NH_WEB_RESULT_H
#define NH_WEB_RESULT_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#endif

/** @addtogroup NhWebEnums
 *  @{
 */ 

    /**
     * Return values for functions.
     */
    typedef enum NH_WEB_RESULT 
    {
        NH_WEB_SUCCESS, /**<Indicates that something worked as planned.*/                   
        NH_WEB_ERROR_NULL_POINTER,         
        NH_WEB_ERROR_BAD_STATE, 
        NH_WEB_ERROR_MEMORY_ALLOCATION,

    } NH_WEB_RESULT;

/** @} */

/** @addtogroup NhWebStructs
 *  @{
 */ 

//    /**
//     * Return values for functions.
//     */
//    typedef struct Nh_Web_Result {
//        void *data_p;
//    } Nh_Web_Result;
//
//    typedef struct Nh_Web_NewObject {
//        Nh_Web_Object *Exception_p;
//        Nh_Web_Object *NewObject_p;
//    } Nh_Web_NewObject;
//
//    typedef struct Nh_Web_Boolean {
//        Nh_Web_Object *Exception_p;
//        NH_BOOL boolean;
//    } Nh_Web_Boolean;

/** @} */

/** @addtogroup NhWebVars
 *  @{
 */ 

    extern const char *NH_WEB_RESULTS_PP[];
    extern unsigned int NH_WEB_RESULTS_PP_COUNT;

/** @} */

#endif

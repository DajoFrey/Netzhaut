#ifndef NH_WEBIDL_RESULT_H
#define NH_WEBIDL_RESULT_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#endif

/** @addtogroup NhWebIDLEnums
 *  @{
 */ 

    /**
     * Return values for functions.
     */
    typedef enum NH_WEBIDL_RESULT 
    {
        NH_WEBIDL_SUCCESS, /**<Indicates that something worked as planned.*/                   
        NH_WEBIDL_ERROR_NULL_POINTER,         
        NH_WEBIDL_ERROR_BAD_STATE, 
        NH_WEBIDL_ERROR_MEMORY_ALLOCATION,

    } NH_WEBIDL_RESULT;

/** @} */

/** @addtogroup NhWebIDLStructs
 *  @{
 */ 

//    /**
//     * Return values for functions.
//     */
//    typedef struct Nh_WebIDL_Result {
//        void *data_p;
//    } Nh_WebIDL_Result;
//
//    typedef struct Nh_WebIDL_NewObject {
//        Nh_WebIDL_Object *Exception_p;
//        Nh_WebIDL_Object *NewObject_p;
//    } Nh_WebIDL_NewObject;
//
//    typedef struct Nh_WebIDL_Boolean {
//        Nh_WebIDL_Object *Exception_p;
//        NH_BOOL boolean;
//    } Nh_WebIDL_Boolean;

/** @} */

/** @addtogroup NhWebIDLVars
 *  @{
 */ 

    extern const char *NH_WEBIDL_RESULTS_PP[];
    extern unsigned int NH_WEBIDL_RESULTS_PP_COUNT;

/** @} */

#endif

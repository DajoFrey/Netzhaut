#ifndef NH_WEBIDL_GENERATOR_H
#define NH_WEBIDL_GENERATOR_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "../Common/Result.h"

#endif

/** @addtogroup WebIDLEnums Enums
 *  \ingroup WebIDL
 *  @{
 */

    typedef enum NH_WEBIDL_BINDING {
        NH_WEBIDL_BINDING_ECMASCRIPT,
    } NH_WEBIDL_BINDING;

/** @} */


/** @addtogroup WebIDLDefines Defines
 *  \ingroup WebIDL
 *  @{
 */

    NH_WEBIDL_RESULT (*Nh_WebIDL_beginGenerator_f)(
        void (*callback_f)(char *message_p), NH_WEBIDL_BINDING binding
    );

    NH_WEBIDL_RESULT (*Nh_WebIDL_generate_f)(
        char *outdir_p, char *filepath_p
    );

    NH_WEBIDL_RESULT (*Nh_WebIDL_endGenerator_f)(
        char *runtimepath_p
    );

/** @} */

/** @addtogroup WebIDLFunctions Functions
 *  \ingroup WebIDL
 *  @{
 */

    NH_WEBIDL_RESULT Nh_WebIDL_beginGenerator(
        void (*callback_f)(char *message_p), NH_WEBIDL_BINDING binding
    );

    NH_WEBIDL_RESULT Nh_WebIDL_generate(
        char *outdir_p, char *filepath_p
    );

    NH_WEBIDL_RESULT Nh_WebIDL_endGenerator(
        char *runtimepath_p
    );

/** @} */

#endif

#ifndef NH_NETZHAUT_H
#define NH_NETZHAUT_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "Array.h"

#include "Common/API.h"
#include "Common/Result.h"

#endif

/** @addtogroup NhCoreTypedefs
 *  @{
 */

    typedef NH_RESULT (*Nh_init_f)(
    );

    typedef unsigned int (*Nh_run_f)(
    );

    typedef NH_BOOL (*Nh_keepRunning_f)(
    );

    typedef NH_RESULT (*Nh_release_f)(
    );

/** @} */

/** @addtogroup NhCoreFunctions
 *  @{
 */

    NH_RESULT Nh_init(
    );

    unsigned int Nh_run(
    );

    NH_BOOL Nh_keepRunning(
    );

    NH_RESULT Nh_release(
    );

/** @} */

#endif

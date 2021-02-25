#ifndef NH_TIME_H
#define NH_TIME_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "Common/Result.h"
    
#ifdef __unix__
    #include <sys/time.h>
#elif defined(_WIN32) || defined(WIN32)
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
#endif

#endif

/** @addtogroup NhCoreStructs
 *  @{
 */

    typedef struct Nh_SystemTime {
        long seconds;
        long milliseconds;
        long microseconds;
    } Nh_SystemTime;

    typedef struct Nh_LocalTime {
        long weekday;
        long years;
        long months;
        long weeks;
        long days;
        long hours;
        long minutes;
        long seconds;
    } Nh_LocalTime;

    typedef struct Nh_GeneralTime {
        long weeks;
        long days;
        long hours;
        long minutes;
        long seconds;
    } Nh_GeneralTime;

/** @} */

/** @addtogroup NhCoreFunctions
 *  @{
 */

    Nh_GeneralTime Nh_humanizeSeconds(
        long seconds
    );

    Nh_SystemTime Nh_getSystemTime(
    );

    Nh_LocalTime Nh_getLocalTime(
    );

    double Nh_getSystemTimeDiffInSeconds(
        Nh_SystemTime Time1, Nh_SystemTime Time2
    );

/** @} */

#endif 

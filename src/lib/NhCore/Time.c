// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE ========================================================================================

#include "Time.h"

#include "Common/Macro.h"
#include NH_FLOW
#include NH_DEFAULT_CHECK

#include <string.h>
#include <time.h>
#include <stdarg.h>

// HUMANIZE ========================================================================================

Nh_GeneralTime Nh_humanizeSeconds(
    long seconds)
{
NH_BEGIN()

    Nh_GeneralTime Time;
    Time.weeks   = seconds / 604800; 
    Time.days    = (seconds / 86400) % 7; 
    Time.hours   = (seconds / 3600) % 24;
    Time.minutes = (seconds / 60) % 60;
    Time.seconds = seconds % 60;

NH_END(Time)
}

// TIME ============================================================================================

Nh_LocalTime Nh_getLocalTime()
{
NH_BEGIN()

    Nh_LocalTime Time;

#ifdef __unix__ 	
    struct timeval LTime;
    gettimeofday(&LTime, NULL);
    struct tm *tm_p = localtime(&LTime.tv_sec);
    Time.seconds = tm_p->tm_sec;  
    Time.minutes = tm_p->tm_min;  
    Time.hours   = tm_p->tm_hour; 
    Time.days    = tm_p->tm_mday;
    Time.months  = tm_p->tm_mon;  
    Time.years   = tm_p->tm_year + 1900; 
    Time.weekday = tm_p->tm_wday;
#elif defined(_WIN32) || defined(WIN32)
    SYSTEMTIME WTime;
    GetLocalTime(&WTime);
    // TODO
#endif

NH_END(Time)
}

Nh_SystemTime Nh_getSystemTime()
{
NH_BEGIN()

    Nh_SystemTime Time;

#ifdef __unix__ 	
    struct timeval LTime;
    gettimeofday(&LTime, NULL);
    Time.seconds      = LTime.tv_sec;  
    Time.milliseconds = LTime.tv_usec / 1000;  
    Time.microseconds = LTime.tv_usec;  
#elif defined(_WIN32) || defined(WIN32)
    SYSTEMTIME WTime;
    GetSystemTime(&WTime);
#endif

NH_END(Time)
}

double Nh_getSystemTimeDiffInSeconds(
    Nh_SystemTime Time1, Nh_SystemTime Time2)
{
NH_BEGIN()

    double diff;
#ifdef __unix__
    diff  = (double) (Time2.microseconds - Time1.microseconds) / 1000000 +
            (double) (Time2.seconds - Time1.seconds);
#elif defined(_WIN32) || defined(WIN32)
    diff = (double) (Time2.wSecond - Time1.wSecond) +
           (double) ((Time2.wMilliseconds - Time1.wMilliseconds) / 1000.0);
#endif

NH_END(diff)
}


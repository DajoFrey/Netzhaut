#ifndef NH_THREAD_H
#define NH_THREAD_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "Common/API.h"
#include "Common/Result.h"

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
    
#ifdef __unix__
#include <pthread.h>
#elif defined(_WIN32) || defined(WIN32)
#include <windows.h>
#endif

#endif

/** @addtogroup NhCoreMacros
 *  @{
 */

    #define NH_MAX_THREADS 64
    #define NH_MAX_WORKLOADS 128

/** @} */

/** @addtogroup NhCoreStructs
 *  @{
 */

    typedef struct Nh_Thread Nh_Thread;

    typedef struct Nh_Workload {
        Nh_Thread *Thread_p;
        NH_SIGNAL signal;
        NH_BOOL crucial;
        void *args_p;
        void *(*InitFunction_p)(void*);
        NH_SIGNAL (*Function_p)(void*);
        struct {
            int idleDelay; 
            int runDelay;  
            int haltDelay; 
            int waitDelay; 
            double turnAround;
        } Timing;
    } Nh_Workload;

    typedef struct Nh_Thread {
        int depth;
        Nh_Workload *CurrentWorkload_p;
        struct {
    #ifdef __unix__
            int writeTTY_p[2];
            int readTTY_p[2];
    #endif
        } IPC;
    #ifdef __unix__
        pthread_t id;              
    #elif defined(_WIN32) || defined (WIN32)
        DWORD id;              
    #endif
    } Nh_Thread;

    typedef struct Nh_ThreadPool {
        Nh_Thread Main;
        Nh_Thread Threads_p[NH_MAX_THREADS];
        Nh_Workload Workloads_p[NH_MAX_WORKLOADS];
        int threadCount;    
    } Nh_ThreadPool;

/** @} */

/** @addtogroup NhCoreFunctions
 *  @{
 */

    Nh_ThreadPool Nh_initThreadPool(
    );
    
    NH_RESULT Nh_runWorkload(
        Nh_Workload *Workload_p
    );

    unsigned int Nh_runWorkloads(
    );
    
    void *Nh_activateWorkload(
        void *(*InitFunction_p)(void*), NH_SIGNAL (*Function_p)(void*), void *args_p, NH_BOOL crucial
    );
    
    int Nh_activeWorkloads(
        NH_BOOL onlyCrucial
    );
    
    int Nh_activeThreads(
    );

    Nh_ThreadPool *_Nh_getThreadPool(
    );
    
    Nh_ThreadPool *Nh_getThreadPool(
    );

    Nh_Thread *_Nh_getThread(
    );
    
    Nh_Thread *Nh_getThread(
    );
    
    Nh_Workload *Nh_getWorkloadFromArgs(
        void *args_p
    );
    
    Nh_Thread *Nh_getThreadFromArgs(
        void *args_p
    );

    int Nh_getThreadIndex(
    );

    NH_RESULT Nh_sleepMs(
        int milliseconds
    );

/** @} */

#endif 

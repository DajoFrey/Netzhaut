// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "Thread.h"
#include "Process.h"
#include "Time.h"

#include "Common/Macro.h"
#include NH_FLOW
#include NH_DEFAULT_CHECK

#include <string.h>
#include <stdio.h>

#ifdef __unix__
    #include <sys/sysinfo.h>
    #include <sys/types.h>
    #include <sys/wait.h>
    #include <sys/time.h>
#elif defined(_WIN32) || defined (WIN32)
    #include <windows.h>
#endif

// INIT ============================================================================================

static inline void Nh_initThread(
    Nh_Thread *Thread_p)
{
NH_BEGIN()

    Thread_p->CurrentWorkload_p = NULL;
    Thread_p->depth = 0;
    Thread_p->id = 0;

NH_SILENT_END()
}

static inline void Nh_initWorkload(
    Nh_Workload *Workload_p)
{
NH_BEGIN()

    Workload_p->signal = NH_SIGNAL_INACTIVE;
    Workload_p->crucial = NH_FALSE;

    Workload_p->args_p   = NULL;
    Workload_p->Thread_p = NULL;

    Workload_p->InitFunction_p = NULL;
    Workload_p->Function_p = NULL;

    Workload_p->Timing.idleDelay = 0;
    Workload_p->Timing.runDelay  = 0;
    Workload_p->Timing.haltDelay = 0;
    Workload_p->Timing.waitDelay = 0;
    Workload_p->Timing.turnAround = 0.0;

NH_SILENT_END()
}

Nh_ThreadPool Nh_initThreadPool()
{
NH_BEGIN()

    Nh_ThreadPool ThreadPool;
    ThreadPool.threadCount = 0;
    Nh_initThread(&ThreadPool.Main);

    for (int i = 0; i < NH_MAX_THREADS; ++i) {
        Nh_initThread(&ThreadPool.Threads_p[i]);
    }
    for (int i = 0; i < NH_MAX_WORKLOADS; ++i) {
        Nh_initWorkload(&ThreadPool.Workloads_p[i]);
    }

NH_END(ThreadPool)
}

// RUN =============================================================================================

NH_RESULT Nh_runWorkload(
    Nh_Workload *Workload_p)
{
NH_BEGIN()

    NH_CHECK_NULL(Workload_p)

    Nh_SystemTime TAT = Nh_getSystemTime();

    if (Workload_p->signal == NH_SIGNAL_INIT) 
    {
        if (Workload_p->InitFunction_p != NULL) {
            Workload_p->args_p = Workload_p->InitFunction_p(Workload_p->args_p);
        }
        Workload_p->signal = NH_SIGNAL_DONE;
    }
    else {
        Workload_p->signal = Workload_p->Function_p(Workload_p->args_p);
        switch (Workload_p->signal)
        {
            case NH_SIGNAL_OK    : Nh_sleepMs(Workload_p->Timing.runDelay); break;
            case NH_SIGNAL_IDLE  : Nh_sleepMs(Workload_p->Timing.idleDelay); break;
            case NH_SIGNAL_DONE  : 
            {
                Nh_initWorkload(Workload_p); 
                NH_DIAGNOSTIC_END(NH_SUCCESS)
            }
            case NH_SIGNAL_ERROR : 
            {
                Nh_initWorkload(Workload_p); 
                NH_DIAGNOSTIC_END(NH_ERROR_BAD_STATE) 
            }
        }
    }

    Workload_p->Timing.turnAround = Nh_getSystemTimeDiffInSeconds(TAT, Nh_getSystemTime());

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

unsigned int Nh_runWorkloads()
{
NH_BEGIN()

    Nh_ThreadPool *ThreadPool_p = Nh_getThreadPool();
    if (ThreadPool_p == NULL) {NH_END(0)}

    unsigned int count = 0;
    Nh_Thread *Thread_p = Nh_getThread();

    for (int i = 0; i < NH_MAX_WORKLOADS; ++i) 
    {
        Nh_Workload *Workload_p = &ThreadPool_p->Workloads_p[i];

        if (Workload_p->Thread_p == Thread_p) 
        {
            Thread_p->CurrentWorkload_p = Workload_p;
            NH_CHECK(Nh_runWorkload(Workload_p))
            count++;
        }
    }

NH_END(count)
}

//static void Nh_runThread(
//    void *index_p)
//{
//    Nh_Thread *Thread_p = &ThreadPool.Threads_p[*((int*)index_p)];
//
//#ifdef __unix__
//    Thread_p->id = pthread_self();
//#elif defined(_WIN32) || defined (WIN32)
//    Thread_p->id = GetCurrentThreadId();
//#endif
//
//NH_BEGIN()
//
//    Nh_initializeThreadMemory();
//
//    NH_BOOL run = NH_TRUE;
//    while (run) {run = Nh_runWorkloads();}
//
//#ifdef __unix__
//
//    pthread_exit(NULL);
//
//#endif
//
//NH_SILENT_END()
//}

static void Nh_startThread(
    int index)
{
NH_BEGIN()

#ifdef __unix__
    // TODO
#elif defined(_WIN32) || defined (WIN32)
//        _beginthread(Nh_runPhysicalThread, 0, (void*)Data_p);
#endif

NH_SILENT_END()
}

// KEEP RUNNING? ===================================================================================

NH_BOOL Nh_keepRunning()
{
NH_BEGIN()

    Nh_checkForks();

NH_END(Nh_activeWorkloads(NH_TRUE) > 0 || Nh_activeForks() > 0 ? NH_TRUE : NH_FALSE)
}

// ACTIVATE ========================================================================================

static void Nh_assignToThread(
    Nh_ThreadPool *ThreadPool_p, Nh_Workload *Workload_p)
{
NH_BEGIN()

    if (ThreadPool_p->threadCount == 0) {Workload_p->Thread_p = &ThreadPool_p->Main;}
    else {
        // TODO
    }

NH_SILENT_END()
}

static void Nh_waitForInitCompletion(
    Nh_Workload *Workload_p)
{
NH_BEGIN()

    if (Workload_p->Thread_p == Nh_getThread()) {
        Nh_runWorkload(Workload_p);
    }
    else {while (Workload_p->signal != NH_SIGNAL_DONE) {}}

NH_SILENT_END()
}

void *Nh_activateWorkload(
    void *(*InitFunction_p)(void*), NH_SIGNAL (*Function_p)(void*), void *args_p, NH_BOOL crucial)
{
NH_BEGIN()

    Nh_ThreadPool *ThreadPool_p = Nh_getThreadPool();
    if (ThreadPool_p == NULL) {return NULL;}

    Nh_Workload *Workload_p = NULL;

    for (int i = 0; i < NH_MAX_WORKLOADS; ++i) {
        if (ThreadPool_p->Workloads_p[i].signal == NH_SIGNAL_INACTIVE) {
            Workload_p = &ThreadPool_p->Workloads_p[i]; 
            break;
        }
    }

    if (Workload_p == NULL) {NH_END(NULL)}

    Workload_p->signal         = NH_SIGNAL_INIT;
    Workload_p->args_p         = args_p;
    Workload_p->InitFunction_p = InitFunction_p;
    Workload_p->Function_p     = Function_p;
    Workload_p->crucial        = crucial;

    Nh_assignToThread(ThreadPool_p, Workload_p);
    Nh_waitForInitCompletion(Workload_p);

NH_END(Workload_p->args_p)
}

// COUNT ==========================================================================================

int Nh_activeThreads()
{
NH_BEGIN()

    Nh_ThreadPool *ThreadPool_p = Nh_getThreadPool();
    if (ThreadPool_p == NULL) {NH_END(0)}

    int count = 0;
    for (int i = 0; i < NH_MAX_THREADS; ++i) {
        if (ThreadPool_p->Threads_p[i].id != 0) {count++;}
    }

NH_END(count)
}

int Nh_activeWorkloads(
    NH_BOOL onlyCrucial)
{
NH_BEGIN()

    Nh_ThreadPool *ThreadPool_p = Nh_getThreadPool();
    if (ThreadPool_p == NULL) {NH_END(0)}

    int count = 0;
    for (int i = 0; i < NH_MAX_WORKLOADS; ++i) {
        if (ThreadPool_p->Workloads_p[i].signal != NH_SIGNAL_INACTIVE) {
            if (onlyCrucial && !ThreadPool_p->Workloads_p[i].crucial) {continue;}
            count++;
        }
    }

NH_END(count)
}

// GET =============================================================================================

Nh_ThreadPool *_Nh_getThreadPool()
{
return &NH_PROCESS_POOL.Main.ThreadPool;
}

Nh_ThreadPool *Nh_getThreadPool()
{
NH_BEGIN()
NH_END(_Nh_getThreadPool())
}

Nh_Thread *_Nh_getThread()
{
    Nh_ThreadPool *ThreadPool_p = _Nh_getThreadPool();
    if (ThreadPool_p == NULL) {return NULL;}

    for (int i = 0; i < NH_MAX_THREADS; ++i) 
    {
#ifdef __unix__
        if (ThreadPool_p->Threads_p[i].id == pthread_self())
#elif defined(_WIN32) || defined (WIN32)
        if (ThreadPool_p->Threads_p[i].id == GetCurrentThreadId())
#endif
        {
            return &ThreadPool_p->Threads_p[i];
        }
    }

    return &ThreadPool_p->Main;
}

Nh_Thread *Nh_getThread()
{
NH_BEGIN()
NH_END(_Nh_getThread())
}

Nh_Workload *Nh_getWorkloadFromArgs(
    void *args_p)
{
NH_BEGIN()

    if (args_p == NULL) {NH_END(NULL)}

    Nh_ThreadPool *ThreadPool_p = Nh_getThreadPool();
    if (ThreadPool_p == NULL) {NH_END(NULL)}

    for (int i = 0; i < NH_MAX_WORKLOADS; ++i) {
        if (args_p == ThreadPool_p->Workloads_p[i].args_p) {
            NH_END(&ThreadPool_p->Workloads_p[i])
        }
    }

NH_END(NULL)
}

Nh_Thread *Nh_getThreadFromArgs(
    void *args_p)
{
NH_BEGIN()

    if (args_p == NULL) {NH_END(NULL)}

    Nh_ThreadPool *ThreadPool_p = Nh_getThreadPool();
    if (ThreadPool_p == NULL) {NH_END(NULL)}

    for (int i = 0; i < NH_MAX_WORKLOADS; ++i) {
        if (args_p == ThreadPool_p->Workloads_p[i].args_p) {
            NH_END(ThreadPool_p->Workloads_p[i].Thread_p)
        }
    }

NH_END(NULL)
}

int Nh_getThreadIndex()
{
NH_BEGIN()

    Nh_ThreadPool *ThreadPool_p = Nh_getThreadPool();
    if (ThreadPool_p == NULL) {NH_END(0)}

    Nh_Thread *Thread_p = Nh_getThread();
    for (int i = 0; i < NH_MAX_THREADS; ++i) {
        if (Thread_p == &ThreadPool_p->Threads_p[i]) {NH_END(i)}
    }

NH_END(0)
}

// SLEEP ===========================================================================================

NH_RESULT Nh_sleepMs(
    int milliseconds)
{
NH_BEGIN()

#ifdef WIN32
    Sleep(milliseconds);
#elif _POSIX_C_SOURCE >= 199309L
    struct timespec ts;
    ts.tv_sec = milliseconds / 1000;
    ts.tv_nsec = (milliseconds % 1000) * 1000000;
    nanosleep(&ts, NULL);
#else
    usleep(milliseconds * 1000);
#endif

NH_DIAGNOSTIC_END(NH_SUCCESS)
}


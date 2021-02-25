#ifndef NH_PROCESS_H
#define NH_PROCESS_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "Thread.h"
#include "Channel.h"
#include "String.h"
#include "Time.h"

#include "Common/Result.h"

#include <stdint.h>
#include <stdlib.h>
    
#ifdef __unix__
    #include <sys/types.h>
#elif defined(_WIN32) || defined(WIN32)
#endif

#endif

/** @addtogroup NhCoreMacros
 *  @{
 */

    #define NH_MAX_FORKS 16

/** @} */

/** @addtogroup NhCoreStructs
 *  @{
 */

#ifdef __unix__
    typedef pid_t NH_PROCESS;
#endif

    typedef struct Nh_ProcessIPC {
        Nh_Channel In;
        Nh_Channel Out;
    } Nh_ProcessIPC;

    typedef struct Nh_Process {
        NH_PROCESS id;
        Nh_ThreadPool ThreadPool;
        Nh_ProcessIPC IPC;
    } Nh_Process;

    typedef struct Nh_ProcessPoolIPC {
        Nh_SystemTime LastUpdate;
        double updateIntervalInSeconds;
    } Nh_ProcessPoolIPC;

    typedef struct Nh_ProcessPool {
        Nh_ProcessPoolIPC IPC;
        Nh_Process Main;
        Nh_Process Forks_p[NH_MAX_FORKS];
        int forks;
    } Nh_ProcessPool;

/** @} */

/** @addtogroup NhCoreVars
 *  @{
 */

    extern Nh_ProcessPool NH_PROCESS_POOL;

/** @} */

/** @addtogroup NhCoreFunctions
 *  @{
 */

    NH_RESULT Nh_initProcessPool(
    );
    
    /**
     * Forks the executing process. This creates a child process which can obtain its own process handle
     * using Nh_getFork. The function returns NULL if the executing process is the new child process, if
     * the executing process is the main process it returns the child process handle.
     * @return Process handle or NULL.
     */
    Nh_Process *Nh_fork(
    );

    /**
     * Returns the process handle of the executing process if it is a child of the main process
     * and was created using Nh_fork. Otherwise returns NULL.
     * @return Process handle or NULL.
     */
    Nh_Process *Nh_getFork(
    );

    void Nh_checkForks(
    );

    int Nh_activeForks(
    );

    void Nh_killForks(
    );

    /**
     * @param write_p Must have prefix "NH_IPC_ID", "NH_IPC_UPDATE" or "NH_IPC_LOG".
     */
    NH_BYTE *Nh_writeToProcess(
        Nh_Process *Proc_p, NH_BYTE *write_p, int writeLen, NH_BOOL getResponse
    );

    /**
     * @param write_p Must have prefix "NH_IPC_ID", "NH_IPC_UPDATE" or "NH_IPC_LOG".
     */
    NH_BYTE *_Nh_writeToProcess(
        Nh_Process *Proc_p, NH_BYTE *write_p, int writeLen, NH_BOOL getResponse
    );

    NH_SIGNAL Nh_runIPCHandler(
        void *args_p
    );

    NH_RESULT Nh_handleIPCReceive(
        NH_BYTE *bytes_p
    );

/** @} */

#endif 

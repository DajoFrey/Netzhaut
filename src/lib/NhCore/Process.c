// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "Process.h"

#include "Common/API.h"
#include "Common/Macro.h"
#include NH_FLOW
#include NH_DEFAULT_CHECK

#include "../NhLoader/Loader.h"
#include "../NhECMAScript/Engine/Agent.h"

#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#ifdef __unix__
    #include <sys/wait.h>
#endif

// DATA ============================================================================================

static NH_BOOL init = NH_FALSE;
Nh_ProcessPool NH_PROCESS_POOL;

// INIT ============================================================================================

static void Nh_initProcess(
    Nh_Process *Proc_p)
{
NH_BEGIN()

    Proc_p->id = 0;
    Proc_p->ThreadPool = Nh_initThreadPool();

    Nh_initChannel(&Proc_p->IPC.In);
    Nh_initChannel(&Proc_p->IPC.Out);

NH_SILENT_END()
}

NH_RESULT Nh_initProcessPool()
{
NH_BEGIN()

    if (init == NH_TRUE) {NH_END(NH_ERROR_BAD_STATE)}

    NH_PROCESS_POOL.forks = 0;
    NH_PROCESS_POOL.IPC.updateIntervalInSeconds = 1;
    NH_PROCESS_POOL.IPC.LastUpdate = Nh_getSystemTime();

    for (int i = 0; i < NH_MAX_FORKS; ++i) {
        Nh_initProcess(&NH_PROCESS_POOL.Forks_p[i]);
    }
    Nh_initProcess(&NH_PROCESS_POOL.Main);

    init = NH_TRUE;
 
NH_END(NH_SUCCESS)
}

// FORK ============================================================================================

static Nh_Process *Nh_getAvailableFork()
{
NH_BEGIN()

    if (NH_PROCESS_POOL.forks == NH_MAX_FORKS) {NH_END(NULL)}

    for (int i = 0; i < NH_MAX_FORKS; ++i) {
        if (NH_PROCESS_POOL.Forks_p[i].id == 0) {
            NH_PROCESS_POOL.forks++;
            NH_END(&NH_PROCESS_POOL.Forks_p[i])
        }
    }

NH_END(NULL)
}

Nh_Process *Nh_fork()
{
NH_BEGIN()

    if (init == NH_FALSE) {NH_END(NULL)}
    Nh_Process *Fork_p = Nh_getAvailableFork();
    if (Fork_p == NULL) {NH_END(NULL)}

    Nh_openChannel(&Fork_p->IPC.In);
    Nh_openChannel(&Fork_p->IPC.Out);

    Fork_p->id = fork();

    if (Fork_p->id == 0) { // child
        Fork_p->id = getpid();
        Nh_closeChannelWriteAccess(&Fork_p->IPC.In);
        Nh_closeChannelReadAccess(&Fork_p->IPC.Out);
        NH_END(NULL)
    }

    Nh_closeChannelReadAccess(&Fork_p->IPC.In);
    Nh_closeChannelWriteAccess(&Fork_p->IPC.Out);

NH_END(Fork_p)
}

static NH_RESULT Nh_unregisterFork(
    Nh_Process *Fork_p)
{
NH_BEGIN()

    if (init == NH_FALSE || Fork_p->id == 0) {NH_DIAGNOSTIC_END(NH_ERROR_BAD_STATE)}

    Nh_closeChannelWriteAccess(&Fork_p->IPC.In);
    Nh_closeChannelReadAccess(&Fork_p->IPC.Out);

    Nh_initProcess(Fork_p);

    NH_PROCESS_POOL.forks--;

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

void Nh_checkForks()
{
NH_BEGIN()

    if (init == NH_FALSE) {NH_SILENT_END()}

    for (int i = 0; i < NH_MAX_FORKS; ++i) {
        Nh_Process *Proc_p = &NH_PROCESS_POOL.Forks_p[i];
        if (Proc_p->id != 0) {
            int status;
#ifdef __unix__
            int result = waitpid(Proc_p->id, &status, WNOHANG);
            if (result == -1) {
                printf("Nh_checkForks %s\n", strerror(errno));
            }
            if (result == -1 || WIFEXITED(status)) {
                Nh_unregisterFork(&NH_PROCESS_POOL.Forks_p[i]);
            }
#endif
        }
    }

NH_SILENT_END()
}

int Nh_activeForks()
{
NH_BEGIN()

    if (init == NH_FALSE) {NH_END(0)}

NH_END(NH_PROCESS_POOL.forks)
}

void Nh_killFork(
    Nh_Process *Fork_p)
{
NH_BEGIN()

    if (init == NH_FALSE) {NH_SILENT_END()}

    kill(Fork_p->id, SIGTERM);
    Nh_unregisterFork(Fork_p);

NH_SILENT_END()
}

void Nh_killForks()
{
NH_BEGIN()

    if (init == NH_FALSE) {NH_SILENT_END()}

    for (int i = 0; i < NH_MAX_FORKS; ++i) { 
        if (NH_PROCESS_POOL.Forks_p[i].id != 0) {
            kill(NH_PROCESS_POOL.Forks_p[i].id, SIGTERM);
            Nh_unregisterFork(&NH_PROCESS_POOL.Forks_p[i]);
        }
    }

NH_SILENT_END()
}

Nh_Process *Nh_getFork()
{
NH_BEGIN()

    pid_t pid = getpid();

    for (int i = 0; i < NH_MAX_FORKS; ++i) {
        if (NH_PROCESS_POOL.Forks_p[i].id == pid) {
            NH_END(&NH_PROCESS_POOL.Forks_p[i])
        }
    }

NH_END(NULL)
}

// WRITE ===========================================================================================

NH_BYTE *_Nh_writeToProcess(
    Nh_Process *Proc_p, NH_BYTE *write_p, int writeLen, NH_BOOL getResponse)
{
    Nh_writeToChannel(&Proc_p->IPC.In, write_p, writeLen);

    while (getResponse) 
    {
        NH_BYTE *response_p = Nh_readFromChannel(&Proc_p->IPC.Out, NULL);
        if (response_p != NULL) {
            return response_p;
        }
    }

    return NULL;
}

NH_BYTE *Nh_writeToProcess(
    Nh_Process *Proc_p, NH_BYTE *write_p, int writeLen, NH_BOOL getResponse)
{
NH_BEGIN()
NH_END(_Nh_writeToProcess(Proc_p, write_p, writeLen, getResponse))
}

// IPC HANDLER =====================================================================================

// TODO make better
NH_RESULT Nh_handleIPCReceive(
    NH_BYTE *bytes_p)
{
NH_BEGIN()

    Nh_ECMAScript_AgentArgs Args;
    Args.encoding = NH_UNICODE_ENCODING_UTF8;
    Args.bytes_p  = bytes_p + 17;

    if (NH_LOADER.load_f("NhECMAScript", 0) == NH_SUCCESS) 
    {
        Nh_activateWorkload(
            (Nh_ECMAScript_initAgent_f)NH_LOADER.loadFunction_f("NhECMAScript", 0, "Nh_ECMAScript_initAgent"), 
            (Nh_ECMAScript_runAgent_f)NH_LOADER.loadFunction_f("NhECMAScript", 0, "Nh_ECMAScript_runAgent"), 
            &Args, NH_FALSE
        );
    }

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

NH_SIGNAL Nh_runIPCHandler(
    void *args_p)
{
NH_BEGIN()

    if (Nh_getSystemTimeDiffInSeconds(NH_PROCESS_POOL.IPC.LastUpdate, Nh_getSystemTime()) < NH_PROCESS_POOL.IPC.updateIntervalInSeconds) {
        NH_END(NH_SIGNAL_OK)
    }

    for (int i = 0; i < NH_MAX_FORKS; ++i) {
        if (NH_PROCESS_POOL.Forks_p[i].id != 0) {
            NH_BYTE *receive_p = Nh_readFromChannel(&NH_PROCESS_POOL.Forks_p[i].IPC.Out, NULL);
            if (receive_p != NULL) {
                NH_CHECK(Nh_handleIPCReceive(receive_p))
            }
// TODO free?
        }
    }

    NH_PROCESS_POOL.IPC.LastUpdate = Nh_getSystemTime();

NH_END(NH_SIGNAL_OK)
}


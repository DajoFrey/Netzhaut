// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "Logger.h"
#include "Thread.h"
#include "Process.h"
#include "Memory.h"

#include "Common/Macro.h"
#include NH_FLOW
#include NH_DEFAULT_CHECK

#include <string.h>
#include <stdio.h>

// DATA ============================================================================================

#define NH_LOG_CALLBACKS 128
static void *callbacks_pp[NH_LOG_CALLBACKS] = {NULL};
Nh_Logger NH_LOGGER;

// INIT ============================================================================================

NH_RESULT Nh_initLogger()
{
NH_BEGIN()

    NH_LOGGER.Root.category_p = NULL;
    NH_LOGGER.Root.Parent_p   = NULL;
    NH_LOGGER.Root.Children   = Nh_initList(64);
    NH_LOGGER.Root.Messages   = Nh_initList(64);

NH_END(NH_SUCCESS)
}

// UPDATE LOGGER ===================================================================================

typedef struct Nh_LoggerOptions {
    int replace;
} Nh_LoggerOptions;

static void _Nh_getLogMessageCategory(
    NH_BYTE *message_p, char *category_p)
{
    int length;
    for (length = 0; message_p[length] != ']' && message_p[length] != ':'; ++length);

    char replace = message_p[length];
    message_p[length] = '\0';
    sprintf(category_p, message_p);
    message_p[length] = replace;
}

static NH_BYTE *_Nh_processCategory(
    Nh_LoggerNode *Node_p, NH_BYTE *message_p, Nh_LoggerNode **Target_pp)
{
    if (*message_p == '[' || *message_p == ':') 
    {
        Nh_LoggerNode *Next_p = NULL;
        message_p = message_p + 1;

        char category_p[128] = {'\0'};
        _Nh_getLogMessageCategory(message_p, category_p);

        for (int i = 0; i < Node_p->Children.size; ++i) {
            Nh_LoggerNode *Child_p = Node_p->Children.pp[i];
            if (!strcmp(Child_p->category_p, category_p)) {Next_p = Child_p; break;}
        }

        if (Next_p == NULL) 
        {
            Next_p = Nh_allocate(sizeof(Nh_LoggerNode));
            if (Next_p == NULL) {return NULL;}

            Next_p->category_p = Nh_allocateBytes(category_p);
            if (Next_p->category_p == NULL) {return NULL;}

            Next_p->Parent_p = Node_p;
            Next_p->Children = Nh_initList(8);
            Next_p->Messages = Nh_initList(255);

            Nh_appendToList(&Node_p->Children, Next_p);
        }

        return _Nh_processCategory(Next_p, message_p + strlen(category_p), Target_pp);
    }
    else if (*message_p == ']') 
    {
        *Target_pp = Node_p;
        return message_p + 1;
    }

    return NULL;
}

static NH_BYTE *_Nh_processOptions(
    NH_BYTE *message_p, Nh_LoggerOptions *Options_p)
{
    Options_p->replace = -1;

    if (*message_p != '(') {return message_p;}

    int messageIndex = 0, optionIndex = 0;
    char option_p[64] = {'\0'};

    for (messageIndex = 0; message_p[messageIndex] != '{'; ++messageIndex) 
    {
        if (message_p[messageIndex] == ':' || message_p[messageIndex] == ')') {
            if (strstr(option_p, "REPLACE")) {
                Options_p->replace = atoi(&option_p[7]);
            }
        }
        if (message_p[messageIndex] == '(' || message_p[messageIndex] == ':') {
            memset(option_p, 0, 64);
            optionIndex = 0;
        }
        else {option_p[optionIndex++] = message_p[messageIndex];}
    }

    return message_p + messageIndex;
}

static NH_RESULT _Nh_handlePayload(
    Nh_LoggerNode *Node_p, Nh_LoggerOptions Options, NH_BYTE *message_p)
{
    if (*message_p != '{') {
        return NH_ERROR_BAD_STATE;
    }
    else {message_p = message_p + 1;}

    int payloadLength;
    for (payloadLength = 0; message_p[payloadLength] != '}'; ++payloadLength);

    NH_BYTE *payload_p = Nh_allocate(payloadLength + 1);
    if (payload_p == NULL) {return NH_ERROR_BAD_STATE;}

    memset(payload_p, 0, payloadLength + 1);
    for (int i = 0; i < payloadLength; ++i) {payload_p[i] = message_p[i];}

    if (Options.replace >= 0 && Node_p->Messages.size > Options.replace) {
        Nh_free(Node_p->Messages.pp[Options.replace]);
        Node_p->Messages.pp[Options.replace] = payload_p;
    }
    else {Nh_appendToList(&Node_p->Messages, payload_p);}

    return NH_SUCCESS;
}

NH_RESULT _Nh_updateLogger(
    NH_BYTE *message_p)
{
    Nh_LoggerNode *Target_p = NULL;
    message_p = _Nh_processCategory(&NH_LOGGER.Root, message_p, &Target_p);
    if (message_p == NULL) {return NH_ERROR_BAD_STATE;}

    Nh_LoggerOptions Options;
    message_p = _Nh_processOptions(message_p, &Options);

    return _Nh_handlePayload(Target_p, Options, message_p);
}

// CALLBACKS =======================================================================================

NH_RESULT Nh_addLogCallback(
    void (*logCallback_f)(NH_BYTE *bytes_p))
{
NH_BEGIN()

    NH_RESULT result = NH_ERROR_BAD_STATE;
    for (int i = 0; i < NH_LOG_CALLBACKS; ++i) {
        if (callbacks_pp[i] == NULL) {
            callbacks_pp[i] = logCallback_f;
            result = NH_SUCCESS;
            break;
        }
    }
 
NH_END(result)
}

NH_RESULT _Nh_sendLogMessage(
    NH_BYTE *message_p)
{
    if (message_p == NULL) {return NH_ERROR_BAD_STATE;}

    // send to logger
    if (_Nh_updateLogger(message_p) != NH_SUCCESS) {return NH_ERROR_BAD_STATE;}

    // send to callbacks
    for (int i = 0; i < NH_LOG_CALLBACKS && callbacks_pp[i] != NULL; ++i) {
        ((void (*)(NH_BYTE *bytes_p))callbacks_pp[i])(message_p);
    }

    // send to forks
    if (NH_PROCESS_POOL.forks > 0)
    {
        NH_BYTE *messageIPC_p = Nh_allocate(strlen(message_p) + 11);
        if (messageIPC_p == NULL) {return NH_ERROR_BAD_STATE;}
        sprintf(messageIPC_p, "NH_IPC_LOG%s", message_p);
    
        for (int i = 0; i < NH_MAX_FORKS; ++i) {
            if (NH_PROCESS_POOL.Forks_p[i].id != 0) {
                _Nh_writeToProcess(&NH_PROCESS_POOL.Forks_p[i], messageIPC_p, strlen(messageIPC_p), NH_FALSE);
            }
        }
    
        Nh_free(messageIPC_p);
    }

    return NH_SUCCESS;
}

NH_RESULT Nh_sendLogMessage(
    NH_BYTE *message_p)
{
NH_BEGIN()
NH_DIAGNOSTIC_END(_Nh_sendLogMessage(message_p))
}

// FLOW ============================================================================================

#define NH_FLOW_INDENT 2

NH_RESULT _Nh_begin(
    const NH_BYTE *file_p, const NH_BYTE *function_p)
{
    Nh_Thread *Thread_p = _Nh_getThread();
    if (Thread_p == NULL) {return NH_ERROR_BAD_STATE;}

    NH_BYTE NH_BYTEs_p[1024] = {'\0'};
    memset(NH_BYTEs_p, ' ', sizeof(NH_BYTE) * (Thread_p->depth * NH_FLOW_INDENT));
    sprintf(NH_BYTEs_p + strlen(NH_BYTEs_p), "-> %s (%s)\n", function_p, file_p);

    Thread_p->depth++;

    return _Nh_sendLogMessage(NH_BYTEs_p);
}

NH_RESULT _Nh_end(
    const NH_BYTE *file_p, const NH_BYTE *function_p)
{
    Nh_Thread *Thread_p = _Nh_getThread();
    if (Thread_p == NULL) {return NH_ERROR_BAD_STATE;}

    Thread_p->depth--;

    NH_BYTE NH_BYTEs_p[1024] = {'\0'};
    memset(NH_BYTEs_p, ' ', sizeof(NH_BYTE) * (Thread_p->depth * NH_FLOW_INDENT));
    sprintf(NH_BYTEs_p + strlen(NH_BYTEs_p), "<- %s (%s)\n", function_p, file_p);

    return _Nh_sendLogMessage(NH_BYTEs_p);
}

NH_RESULT _Nh_diagnosticEnd(
    const NH_BYTE *file_p, const NH_BYTE *function_p, const NH_BYTE *result_p, int line, NH_BOOL success)
{
    Nh_Thread *Thread_p = _Nh_getThread();
    if (Thread_p == NULL) {return NH_ERROR_BAD_STATE;}

    Thread_p->depth--;

    NH_BYTE NH_BYTEs_p[1024] = {'\0'};
    memset(NH_BYTEs_p, ' ', sizeof(NH_BYTE) * (Thread_p->depth * NH_FLOW_INDENT));

    if (success) {
        sprintf(NH_BYTEs_p + strlen(NH_BYTEs_p), "<- %s (%s)\n", function_p, file_p);
    }
    else {
        sprintf(NH_BYTEs_p + strlen(NH_BYTEs_p), "<- %s (%s LINE %d) \e[1;31m%s\e[0m\n", function_p, file_p, line, result_p);
    }

    return _Nh_sendLogMessage(NH_BYTEs_p);
}


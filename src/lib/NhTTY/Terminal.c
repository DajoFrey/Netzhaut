// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// DEFINE ==========================================================================================

#define _DEFAULT_SOURCE
#define _BSD_SOURCE
#define _GNU_SOURCE

// INCLUDE =========================================================================================

#include "Terminal.h"
#include "Draw.h"
#include "Row.h"
#include "Tile.h"
#include "Tab.h"
#include "Logger.h"

#include "../NhCore/Thread.h"
#include "../NhCore/Memory.h"
#include "../NhCore/Config.h"
#include "../NhCore/Process.h"
#include "../NhCore/Logger.h"

#include "../NhEncoding/Encodings/UTF8.h"

#include "Common/Macro.h"
#include NH_TTY_FLOW
#include NH_TTY_DEFAULT_CHECK

#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <stdio.h>

#ifdef __unix__
//    #include <curses.h>
//    #include <term.h>
    #include <sys/types.h>
    #include <termios.h>
    #include <sys/ioctl.h>
#endif

// CALLBACKS =======================================================================================

void Nh_TTY_write(
    NH_BYTE *message_p)
{
#ifdef __unix__
    write(Nh_getThread()->IPC.writeTTY_p[1], message_p, strlen(message_p));
#endif
}

// INIT ============================================================================================
 
static NH_TTY_RESULT Nh_TTY_enableRawMode(
    Nh_TTY_Terminal *Terminal_p) 
{
NH_TTY_BEGIN()

#ifdef __unix__

    if (tcgetattr(STDIN_FILENO, &Terminal_p->Linux.origTermios) == -1) {
        NH_TTY_DIAGNOSTIC_END(NH_TTY_ERROR_BAD_STATE)
    }
  
    struct termios raw = Terminal_p->Linux.origTermios;

    raw.c_iflag    &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    raw.c_oflag    &= ~(OPOST);
    raw.c_cflag    |= (CS8);
    raw.c_lflag    &= ~(ECHO | ICANON | IEXTEN | ISIG);
    raw.c_cc[VMIN]  = 0;
    raw.c_cc[VTIME] = 1;
  
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) {
        NH_TTY_DIAGNOSTIC_END(NH_TTY_ERROR_BAD_STATE)
    }

#endif

NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS)
}

static NH_TTY_RESULT Nh_TTY_getLinuxCursorPosition(
    short unsigned int *rows, short unsigned int *cols) 
{
NH_TTY_BEGIN()

#ifdef __unix__

    char buf[32];
    unsigned int i = 0;
  
    if (write(STDOUT_FILENO, "\x1b[6n", 4) != 4) {
        NH_TTY_DIAGNOSTIC_END(NH_TTY_ERROR_BAD_STATE)
    }
  
    while (i < sizeof(buf) - 1) {
        if (read(STDIN_FILENO, &buf[i], 1) != 1) break;
        if (buf[i] == 'R') break;
        i++;
    }
    buf[i] = '\0';
  
    if (buf[0] != '\x1b' || buf[1] != '[') {
        NH_TTY_DIAGNOSTIC_END(NH_TTY_ERROR_BAD_STATE)
    }
    if (sscanf(&buf[2], "%d;%d", rows, cols) != 2) {
        NH_TTY_DIAGNOSTIC_END(NH_TTY_ERROR_BAD_STATE)
    }
  
#endif

NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS)
}

static NH_TTY_RESULT Nh_TTY_getWindowSize(
    Nh_TTY_Terminal *Terminal_p) 
{
NH_TTY_BEGIN()

#ifdef __unix__

    struct winsize ws;
  
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0) {
        if (write(STDOUT_FILENO, "\x1b[999C\x1b[999B", 12) != 12) {
            NH_TTY_DIAGNOSTIC_END(NH_TTY_ERROR_BAD_STATE)
        }
        Nh_TTY_getLinuxCursorPosition(&ws.ws_row, &ws.ws_col);
    } 

    if (Terminal_p->screenCols != ws.ws_col || Terminal_p->screenRows != ws.ws_row) {
        if (Terminal_p->Tab_p != NULL) {Terminal_p->Tab_p->refreshScreen = NH_TRUE;}
    }
    Terminal_p->screenCols = ws.ws_col;
    Terminal_p->screenRows = ws.ws_row;

#endif

NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS)
}

static NH_TTY_RESULT Nh_TTY_addTab(
    Nh_TTY_Terminal *Terminal_p)
{
NH_TTY_BEGIN()

    Nh_TTY_Tab *Tab_p = Nh_TTY_initTab(&Terminal_p->Prototypes);
    Nh_appendToLinkedList(&Terminal_p->Tabs, Tab_p);
    Terminal_p->Tab_p = Tab_p;

NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS)
}

void *Nh_TTY_initTerminal(
    void *args_p)
{
NH_TTY_BEGIN()

#include NH_TTY_CUSTOM_CHECK

    Nh_TTY_Terminal *Terminal_p = Nh_allocate(sizeof(Nh_TTY_Terminal));
    NH_TTY_CHECK_MEM(NULL, Terminal_p)

    Terminal_p->close      = NH_FALSE;
    Terminal_p->standalone = NH_FALSE;

    Terminal_p->Tab_p = NULL;
    Terminal_p->Tabs  = Nh_initLinkedList();

    Terminal_p->Prototypes = Nh_initList(8);

    NH_TTY_CHECK(NULL, Nh_TTY_enableRawMode(Terminal_p))
    NH_TTY_CHECK(NULL, Nh_TTY_getWindowSize(Terminal_p))
    NH_TTY_CHECK(NULL, Nh_TTY_addTab(Terminal_p))

#include NH_TTY_DEFAULT_CHECK

NH_TTY_END(Terminal_p)
}

// IPC =============================================================================================

static NH_TTY_RESULT Nh_TTY_updateLoggers(
    Nh_TTY_Terminal *Terminal_p)
{
NH_TTY_BEGIN()

//    Nh_TTY_Tab *Tab_p = NULL;
//    for (int i = 0; (Tab_p = Nh_getFromLinkedList(&Terminal_p->Tabs, i)) != NULL; ++i)
//    {
//        Nh_List Tiles = Nh_TTY_getTiles(Tab_p->RootTile_p);
//        Nh_TTY_Tile *Tile_p = NULL;
//
//        for (int j = 0; (Tile_p = Nh_getFromList(&Tiles, j)) != NULL; ++j) 
//        {
//            if (Tile_p->Console.Programs.Instances_p == NULL) {continue;}
//
//            Nh_TTY_Logger *Logger_p = Nh_TTY_getProgram(Tile_p->Console.Programs_p, NH_TTY_LOGGER_NAME)->handle_p;
//            Nh_TTY_updateLogger(Logger_p);
//
//            if (Nh_TTY_getProgramIndex(Tile_p->Console.Programs_p, NH_TTY_LOGGER_NAME) == Tile_p->Console.currentProgram) {
//                Tab_p->refreshScreen = NH_TRUE;
//            }
//        }
//
//        Nh_freeList(&Tiles, NH_FALSE);
//    }

NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS)
}

static void Nh_TTY_handleIPC(
    Nh_TTY_Terminal *Terminal_p)
{
    size_t size;
    NH_BYTE *msg_p = Nh_readFromChannel(&Nh_getFork()->IPC.In, &size);

    if (msg_p != NULL)
    {
        if (!strcmp(msg_p, "NH_IPC_ID")) {
            Nh_writeToChannel(&Nh_getFork()->IPC.Out, "NhTTY", 5 + 1);
        }
        else if (!strcmp(msg_p, "NH_IPC_UPDATE")) {
            Nh_writeToChannel(&Nh_getFork()->IPC.Out, "Nh_TTY_startTerminalProcess", 27 + 1);
            Terminal_p->close = NH_TRUE;
        }
        else if (strstr(msg_p, "NH_IPC_LOG")) 
        {
            char *next_p = msg_p;

            while (next_p = strstr(next_p, "NH_IPC_LOG"))
            {
                _Nh_updateLogger(next_p + 10);
                Nh_TTY_updateLoggers(Terminal_p);
                next_p += 10;
            }
        }

        free(msg_p);
    }
}

// RUN =============================================================================================

static void Nh_TTY_atExit(
    Nh_TTY_Terminal *Terminal_p) 
{
NH_TTY_BEGIN()

    write(STDOUT_FILENO, "\x1b[2J", 4);
    write(STDOUT_FILENO, "\x1b[H", 3);
    write(STDOUT_FILENO, "\x1b[?25h", 6);

#ifdef __unix__
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &Terminal_p->Linux.origTermios) == -1) {
        exit(1); 
    }
#endif

NH_TTY_SILENT_END()
}

static NH_TTY_RESULT Nh_TTY_readLinuxKey(
    NH_UNICODE_CODEPOINT *codepoint_p)
{
NH_TTY_BEGIN()

#ifdef __unix__

    fd_set set;
    FD_ZERO(&set);
    FD_SET(fileno(stdin), &set);
  
    struct timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = 1;
  
    int rv = select(fileno(stdin) + 1, &set, NULL, NULL, &timeout);
    if (rv == -1 || rv == 0) {NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS)}

    int nread;
    NH_BYTE bytes_p[4];
    memset(bytes_p, 0, 4);
    nread = read(STDIN_FILENO, bytes_p, 4);

    if (nread) {*codepoint_p = Nh_Encoding_decodeUTF8Single(bytes_p);}

#endif

NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS)
}

static NH_TTY_RESULT Nh_TTY_processKeypress(
    Nh_TTY_Terminal *Terminal_p) 
{
NH_TTY_BEGIN()

    NH_UNICODE_CODEPOINT codepoint = 0;

#ifdef __unix__
    NH_TTY_CHECK(Nh_TTY_readLinuxKey(&codepoint))
#endif

    if (codepoint != 0) {
        Terminal_p->Tab_p->refreshCursor = NH_TRUE;
        NH_TTY_CHECK(Nh_TTY_handleTabInput(Terminal_p->Tab_p, codepoint))
    }

NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS)
}

static int Nh_TTY_runTerminalProcess() 
{
NH_TTY_BEGIN()

#ifdef __unix__

    Nh_TTY_Terminal *Terminal_p = Nh_TTY_initTerminal(NULL);

    struct timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = 1;

    NH_BOOL run = NH_TRUE; 
    while (run) 
    {
        Nh_TTY_getWindowSize(Terminal_p);

        if (Terminal_p->Tab_p->refreshScreen) {Nh_TTY_refreshScreen(Terminal_p);}
        if (Terminal_p->Tab_p->refreshCursor) {Nh_TTY_refreshCursor(Terminal_p);}

        Nh_TTY_processKeypress(Terminal_p);
        Nh_TTY_updateTab(Terminal_p->Tab_p);
        Nh_TTY_handleIPC(Terminal_p);

        run = !Terminal_p->Tab_p->close;
    }

    Nh_closeChannelReadAccess(&Nh_getFork()->IPC.In);
    Nh_closeChannelWriteAccess(&Nh_getFork()->IPC.Out);

    Nh_TTY_atExit(Terminal_p);

#endif

NH_TTY_END(0)
}

static NH_RESULT Nh_TTY_startTerminalProcess(
    int argc, NH_BYTE **argv_pp) 
{
NH_TTY_BEGIN()

#ifdef __unix__

    Nh_Process *Fork_p = Nh_fork(); 

    if (Fork_p == NULL) { // child process handles TTY
        Nh_TTY_runTerminalProcess();
        exit(0);
    }

#endif

NH_TTY_END(NH_SUCCESS)
}

static NH_SIGNAL Nh_TTY_runTerminal(
    void *args_p) 
{
NH_TTY_BEGIN()

#include NH_TTY_CUSTOM_CHECK

    NH_TTY_CHECK_NULL(NH_SIGNAL_ERROR, args_p)
    Nh_TTY_Terminal *Terminal_p = args_p;

    Nh_TTY_updateLoggers(Terminal_p);
    Nh_TTY_getWindowSize(Terminal_p);

    if (Terminal_p->Tab_p->refreshScreen) {NH_TTY_CHECK(NH_SIGNAL_ERROR, Nh_TTY_refreshScreen(Terminal_p))}
    if (Terminal_p->Tab_p->refreshCursor) {NH_TTY_CHECK(NH_SIGNAL_ERROR, Nh_TTY_refreshCursor(Terminal_p))}
 
    NH_TTY_CHECK(NH_SIGNAL_ERROR, Nh_TTY_processKeypress(Terminal_p))
    NH_TTY_CHECK(NH_SIGNAL_ERROR, Nh_TTY_updateTab(Terminal_p->Tab_p))
 
    if (Terminal_p->Tab_p->close) 
    {
        Nh_TTY_atExit(Terminal_p);
        NH_TTY_END(NH_SIGNAL_DONE)
    }

#include NH_TTY_DEFAULT_CHECK

NH_TTY_END(NH_SIGNAL_OK)
}

// OPEN ============================================================================================

Nh_TTY_Terminal *Nh_TTY_openTerminal()
{
NH_TTY_BEGIN()

//    NH_CHECK(((Nh_TTY_startTerminalProcess_f)Nh_loadFunction("NhTTY", "Nh_TTY_startTerminalProcess"))(0, NULL))
    Nh_TTY_Terminal *Terminal_p = Nh_activateWorkload(Nh_TTY_initTerminal, Nh_TTY_runTerminal, NULL, NH_TRUE);

NH_TTY_END(Terminal_p)
}

// GET =============================================================================================

Nh_TTY_Terminal *Nh_TTY_getTerminal()
{
NH_TTY_BEGIN()

#include NH_TTY_CUSTOM_CHECK

    Nh_Thread *Thread_p = Nh_getThread();
    NH_TTY_CHECK_NULL(NULL, Thread_p)

    Nh_TTY_Terminal *Terminal_p = Thread_p->CurrentWorkload_p->args_p;
    NH_TTY_CHECK_NULL(NULL, Terminal_p)

#include NH_TTY_DEFAULT_CHECK

NH_TTY_END(Terminal_p)
}


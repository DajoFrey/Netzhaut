// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 *
 * Based on st - simple terminal implementation.
 */

// INCLUDE =========================================================================================

#include "Shell.h"
#include "Row.h"

#include "Common/Macro.h"
#include NH_TTY_FLOW
#include NH_TTY_DEFAULT_CHECK

#include "../NhCore/Memory.h"

#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <pwd.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <termios.h>
#include <time.h>
#include <wchar.h>
#include <pty.h>
#include <stdint.h>

// DEFINE ==========================================================================================

#define UTF_SIZ          4
#define LEN(a)		 (sizeof(a) / sizeof(a)[0])
#define DEFAULT(a, b)	 (a) = (a) ? (a) : (b)
#define BETWEEN(x, a, b) ((a) <= (x) && (x) <= (b))
#define ISCONTROLC0(c)	 (BETWEEN(c, 0, 0x1f) || (c) == '\177')
#define ISCONTROLC1(c)	 (BETWEEN(c, 0x80, 0x9f))
#define ISCONTROL(c)	 (ISCONTROLC0(c) || ISCONTROLC1(c))

typedef uint_least32_t Rune;

#define Glyph Glyph_
typedef struct {
	Rune u;           /* character code */
	ushort mode;      /* attribute flags */
	uint32_t fg;      /* foreground  */
	uint32_t bg;      /* background  */
} Glyph;

// VARS ============================================================================================

static pid_t pid;
char *utmp = NULL;
char *stty_args = "stty raw pass8 nl -echo -iexten -cstopb 38400";
char *termname = "st-256color";

// SHELL ===========================================================================================

static void die(
    const char *errstr, ...)
{
    va_list ap;
    
    va_start(ap, errstr);
    vfprintf(stderr, errstr, ap);
    va_end(ap);
    exit(1);
}

static void Nh_TTY_execsh(
    char *cmd, char **args)
{
NH_TTY_BEGIN()

    char *sh, *prog;
    const struct passwd *pw;
    
    errno = 0;
    if ((pw = getpwuid(getuid())) == NULL) {
        if (errno)
            die("getpwuid: %s\n", strerror(errno));
        else
    	    die("who are you?\n");
    }
    
    if ((sh = getenv("SHELL")) == NULL)
        sh = (pw->pw_shell[0]) ? pw->pw_shell : cmd;
    
    if (args)
        prog = args[0];
    else if (utmp)
        prog = utmp;
    else
        prog = sh;
    DEFAULT(args, ((char *[]) {prog, NULL}));
    
    unsetenv("COLUMNS");
    unsetenv("LINES");
    unsetenv("TERMCAP");
    setenv("LOGNAME", pw->pw_name, 1);
    setenv("USER", pw->pw_name, 1);
    setenv("SHELL", sh, 1);
    setenv("HOME", pw->pw_dir, 1);
    setenv("TERM", termname, 1);
    
    signal(SIGCHLD, SIG_DFL);
    signal(SIGHUP, SIG_DFL);
    signal(SIGINT, SIG_DFL);
    signal(SIGQUIT, SIG_DFL);
    signal(SIGTERM, SIG_DFL);
    signal(SIGALRM, SIG_DFL);
    
    execvp(prog, args);
    _exit(1);

NH_TTY_SILENT_END()
}

static void Nh_TTY_stty(
    char **args)
{
NH_TTY_BEGIN()

    char cmd[_POSIX_ARG_MAX], **p, *q, *s;
    size_t n, siz;
    
    if ((n = strlen(stty_args)) > sizeof(cmd)-1) {
        die("incorrect stty parameters\n");
    }
    memcpy(cmd, stty_args, n);
    q = cmd + n;
    siz = sizeof(cmd) - n;
    for (p = args; p && (s = *p); ++p) {
        if ((n = strlen(s)) > siz-1) {
            die("stty parameter length too long\n");
        }
        *q++ = ' ';
        memcpy(q, s, n);
        q += n;
        siz -= n + 1;
    }
    *q = '\0';
    if (system(cmd) != 0) {
        perror("Couldn't call stty");
    }

NH_TTY_SILENT_END()
}

static void Nh_TTY_sigchld(
    int a)
{
    int stat;
    pid_t p;
    
    if ((p = waitpid(pid, &stat, WNOHANG)) < 0) {
        die("waiting for pid %hd failed: %s\n", pid, strerror(errno));
    }
    if (pid != p) {
        return;
    }
    if (WIFEXITED(stat) && WEXITSTATUS(stat)) {
        die("child exited with status %d\n", WEXITSTATUS(stat));
    }
    else if (WIFSIGNALED(stat)) {
        die("child terminated due to signal %d\n", WTERMSIG(stat));
    }

    exit(0);
}

static NH_TTY_RESULT Nh_TTY_newShell(
    Nh_TTY_Shell *Shell_p, char *line, char *cmd, char *out, char **args)
{
NH_TTY_BEGIN()

    int m, s;

//	if (out) {

//		term.mode |= MODE_PRINT;
//		iofd = (!strcmp(out, "-")) ?
//			  1 : open(out, O_WRONLY | O_CREAT, 0666);
//		if (iofd < 0) {
//			fprintf(stderr, "Error opening %s:%s\n",
//				out, strerror(errno));
//		}
//	}

    if (line) {
        if ((Shell_p->fd = open(line, O_RDWR)) < 0) {
            die("open line '%s' failed: %s\n", line, strerror(errno));
        }
        dup2(Shell_p->fd, 0);
        Nh_TTY_stty(args);
        NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS)
    }

    /* seems to work fine on linux, openbsd and freebsd */
    if (openpty(&m, &s, NULL, NULL, NULL) < 0)
        die("openpty failed: %s\n", strerror(errno));
    
    switch (Shell_p->pid = fork()) 
    {
        case -1:
            die("fork failed: %s\n", strerror(errno));
            break;

        case 0:
            setsid(); /* create a new process group */
            dup2(s, 0);
            dup2(s, 1);
            dup2(s, 2);
            if (ioctl(s, TIOCSCTTY, NULL) < 0)
            	die("ioctl TIOCSCTTY failed: %s\n", strerror(errno));
            close(s);
            close(m);
#ifdef __OpenBSD__
            if (pledge("stdio getpw proc exec", NULL) == -1)
                die("pledge\n");
#endif
            Nh_TTY_execsh(cmd, args);
            break;

	default:
#ifdef __OpenBSD__
            if (pledge("stdio rpath tty proc", NULL) == -1) {
                die("pledge\n");
            }
#endif
            close(s);
            Shell_p->fd = m;
            signal(SIGCHLD, Nh_TTY_sigchld);
            break;
    }

NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS)
}

static Nh_String Nh_TTY_getFormattedShellOutput(
    const char *buf, int buflen, int show_ctrl)
{
NH_TTY_BEGIN()

    int charsize, n;
    Rune u;

    Nh_String String = Nh_initString(128);
    
//    for (n = 0; n < buflen; n += charsize) {
//    //	if (IS_SET(MODE_UTF8) && !IS_SET(MODE_SIXEL)) {
//    //		/* process a complete utf8 char */
//    //		charsize = decodeUTF8(buf + n, &u, buflen - n);
//    //		if (charsize == 0)
//    //			break;
//    //	} else {
//    		u = buf[n] & 0xFF;
//    		charsize = 1;
//    //	}
//        if (show_ctrl && ISCONTROL(u)) {
//            if (u & 0x80) {
//                u &= 0x7f;
//                Nh_appendToString3(&String, "^", 1);
//                Nh_appendToString3(&String, "[", 1);
//            } else if (u != '\n' && u != '\r' && u != '\t') {
//                u ^= 0x40;
//                Nh_appendToString(&String, "^", 1);
//            }
//        }
//        Nh_appendToString(&String, &u, 1);
//    }

NH_TTY_END(String)
}

// READ ============================================================================================

static Nh_String Nh_TTY_readFromShell(
    int shellfd)
{
NH_TTY_BEGIN()

    static char buf[BUFSIZ];
    static int buflen = 0;
    int written;
    int ret;
    
    /* append read bytes to unprocessed bytes */
    if ((ret = read(shellfd, buf+buflen, LEN(buf)-buflen)) < 0) {
        die("couldn't read from shell: %s\n", strerror(errno));
    }
    buflen += ret;
    
    Nh_String ShellOutput = Nh_TTY_getFormattedShellOutput(buf, buflen, 0);
    buflen -= ShellOutput.length;

    /* keep any uncomplete utf8 char for the next call */
    if (buflen > 0) {
        memmove(buf, buf + ShellOutput.length, buflen);
    }

NH_TTY_END(ShellOutput)
}

static Nh_String Nh_TTY_tryToReadFromShell(
    Nh_TTY_Shell Shell)
{
NH_TTY_BEGIN()

    pid = Shell.pid;

    fd_set readfd;
    FD_ZERO(&readfd);
    FD_SET(Shell.fd, &readfd);
    
    if (FD_ISSET(Shell.fd, &readfd)) 
    {
        // Set timeout to 10 milliseconds
        struct timeval timeout;
        timeout.tv_sec = 0.01f;
        timeout.tv_usec = 0;

        // wait until timeout or read from shell
        if (select(Shell.fd + 1, &readfd, NULL, NULL, &timeout) == 1) {
            NH_TTY_END(Nh_TTY_readFromShell(Shell.fd))
        }
    }

NH_TTY_END(Nh_initString(0))
}

// WRITE ===========================================================================================

static void Nh_TTY_writeToShellRaw(
    int shellfd, const char *s, size_t n)
{
NH_TTY_BEGIN()

    fd_set wfd, rfd;
    ssize_t r;
    size_t lim = 256;
    
    /*
     * Remember that we are using a pty, which might be a modem line.
     * Writing too much will clog the line. That's why we are doing this
     * dance.
     * FIXME: Migrate the world to Plan 9.
     */
    while (n > 0) 
    {
        FD_ZERO(&wfd);
        FD_ZERO(&rfd);
        FD_SET(shellfd, &wfd);
        FD_SET(shellfd, &rfd);
        
        /* Check if we can write. */
        if (pselect(shellfd+1, &rfd, &wfd, NULL, NULL, NULL) < 0) {
            if (errno == EINTR)
            	continue;
            die("select failed: %s\n", strerror(errno));
        }
    
        if (FD_ISSET(shellfd, &wfd)) {
            /*
             * Only write the bytes written by Nh_TTY_writeToShell() or the
             * default of 256. This seems to be a reasonable value
             * for a serial line. Bigger values might clog the I/O.
             */
            if ((r = write(shellfd, s, (n < lim)? n : lim)) < 0)
            	goto write_error;
            if (r < n) {
                /*
                 * We weren't able to write out everything.
                 * This means the buffer is getting full
                 * again. Empty it.
                 */
                if (n < lim) {
                    Nh_String ShellOutput = Nh_TTY_readFromShell(shellfd);
                    lim = ShellOutput.length;
                    Nh_freeString(&ShellOutput);
                }
                n -= r;
                s += r;
            } else {
            	/* All bytes have been written. */
            	break;
            }
        }
        if (FD_ISSET(shellfd, &rfd)) {
            Nh_String ShellOutput = Nh_TTY_readFromShell(shellfd);
            lim = ShellOutput.length;
            Nh_freeString(&ShellOutput);
        }
    }

NH_TTY_SILENT_END()

write_error:
	die("write error on tty: %s\n", strerror(errno));
}

void Nh_TTY_writeToShell(
    Nh_TTY_Shell *Shell_p, const char *s, size_t n, int may_echo)
{
NH_TTY_BEGIN()

    const char *next;
//	Arg arg = (Arg) { .i = term.scr };
//
//	kscrolldown(&arg);
//
//	if (may_echo && IS_SET(MODE_ECHO))
//		twrite(s, n, 1);
//
    if (!Shell_p->crlf) {
    	Nh_TTY_writeToShellRaw(Shell_p->fd, s, n);
    	return;
    }

    /* This is similar to how the kernel handles ONLCR for ttys */
    while (n > 0) {
        if (*s == '\r') {
            next = s + 1;
            Nh_TTY_writeToShellRaw(Shell_p->fd, "\r\n", 2);
        } else {
            next = memchr(s, '\r', n);
            DEFAULT(next, s + n);
            Nh_TTY_writeToShellRaw(Shell_p->fd, s, next - s);
        }
        n -= next - s;
        s = next;
    }

NH_TTY_SILENT_END()
}

// RESIZE ==========================================================================================

void Nh_TTY_resizeShell(
    Nh_TTY_Shell *Shell_p, int row, int col, int xpixel, int ypixel)
{
NH_TTY_BEGIN()

    struct winsize w;
    w.ws_row = row;
    w.ws_col = col;
    w.ws_xpixel = xpixel;
    w.ws_ypixel = ypixel;

    if (ioctl(Shell_p->fd, TIOCSWINSZ, &w) < 0) {
        fprintf(stderr, "Couldn't set window size: %s\n", strerror(errno));
    }

    Nh_TTY_destroyRows(&Shell_p->Output, 1);
    Nh_TTY_addRow(&Shell_p->Output);

NH_TTY_SILENT_END()
}

// INIT ============================================================================================

Nh_TTY_Shell *Nh_TTY_initShell()
{
NH_TTY_BEGIN()

#include NH_TTY_CUSTOM_CHECK

    Nh_TTY_Shell *Shell_p = Nh_allocate(sizeof(Nh_TTY_Shell)); 
    NH_TTY_CHECK_MEM(NULL, Shell_p)

    NH_TTY_CHECK(NULL, Nh_TTY_newShell(Shell_p, NULL, "/bin/sh", NULL, NULL))

    Shell_p->crlf = NH_FALSE;
    Shell_p->Output = Nh_TTY_initOutput();

#include NH_TTY_DEFAULT_CHECK

NH_TTY_END(Shell_p)
}

// UPDATE ==========================================================================================

NH_TTY_RESULT Nh_TTY_updateShell(
    Nh_TTY_Program *Prog_p)
{
NH_TTY_BEGIN()

    Nh_TTY_Shell *Shell_p = Prog_p->handle_p;
    Nh_String ShellOutput = Nh_TTY_tryToReadFromShell(*Shell_p);

    if (ShellOutput.length > 0) 
    {
//        FILE *fp = fopen("shell.txt", "ab");
//        if (fp != NULL) {
//            fputs(ShellOutput.bytes_p, fp);
//            fclose(fp);
//        }

        Nh_TTY_writeToOutput(&Shell_p->Output, ShellOutput.bytes_p, ShellOutput.length);
        Nh_freeString(&ShellOutput);

        Prog_p->refresh = NH_TRUE;

        // update cursor position
        int rcols, rsize;
        Nh_TTY_getRenderSizes(
            &((Nh_TTY_Row*)Shell_p->Output.Rows.bytes_p)[Shell_p->Output.Rows.length - 1], 
            -1, 0, &rcols, &rsize
        );
        Shell_p->Output.cursorX = rcols + 1;
        Shell_p->Output.cursorY = Shell_p->Output.Rows.length;
    }

NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS)
}

// DRAW ============================================================================================

NH_TTY_RESULT Nh_TTY_drawShellRow(
    Nh_TTY_Shell *Shell_p, Nh_String *Data_p, int row, int cols, int rows)
{
NH_TTY_BEGIN()

    Nh_TTY_Output *Output_p = &Shell_p->Output;
    int rowOffset = Output_p->Rows.length > rows ? Output_p->Rows.length - rows : 0;
    int filerow = row + rowOffset;

    if (filerow < Output_p->Rows.length) 
    {
        int rcols, rsize;
        Nh_TTY_getRenderSizes(
            &((Nh_TTY_Row*)Output_p->Rows.bytes_p)[filerow], cols, Output_p->colOffset, &rcols, &rsize
        );
        Nh_appendToString(
            Data_p, &((Nh_TTY_Row*)Output_p->Rows.bytes_p)[filerow].chars_p[Output_p->colOffset], rsize
        );

        // TODO could cause problems
        // reset escape sequences that may have been set but we only do a partial render of
        Nh_appendToString(Data_p, "\e[0m", 4);

        for (int i = 0; i < (cols - rcols); ++i) {
            Nh_appendToString(Data_p, " ", 1);
        }
    }
    else {
        for (int i = 0; i < cols; ++i) {
            Nh_appendToString(Data_p, " ", 1);
        }
    }

NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS)
}

// BUSY? ===========================================================================================

NH_BOOL Nh_TTY_shellBusy(
    Nh_TTY_Shell *Shell_p)
{
NH_TTY_BEGIN()

    // TODO just a quick hack, please fix
//    if (Shell_p->strstr(((Nh_TTY_Row*)Shell_p->Output.Rows.bytes_p)[Shell_p->Output.Rows.length - 1].chars_p, "]$")) {
//        NH_TTY_END(NH_FALSE)
//    }

NH_TTY_END(NH_FALSE)
}


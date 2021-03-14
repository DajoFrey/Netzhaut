//// LICENSE NOTICE ==================================================================================
//
///**
// * Netzhaut - Web Browser Engine
// * Copyright (C) 2020  Dajo Frey
// * Published under LGPLv3
// */
//
//// DEFINE ==========================================================================================
//
//#define TAB_STOP 8
//#define _XOPEN_SOURCE 600
//
//#define ESC_BUF_SIZ (128*NH_UTF8_SIZE)
//#define ESC_ARG_SIZ 16
//#define STR_BUF_SIZ ESC_BUF_SIZ
//#define STR_ARG_SIZ ESC_ARG_SIZ
//
//#define DEFAULT(a, b)	 (a) = (a) ? (a) : (b)
//#define BETWEEN(x, a, b) ((a) <= (x) && (x) <= (b))
//#define ISCONTROLC0(c)	 (BETWEEN(c, 0, 0x1f) || (c) == '\177')
//#define ISCONTROLC1(c)	 (BETWEEN(c, 0x80, 0x9f))
//#define ISCONTROL(c)	 (ISCONTROLC0(c) || ISCONTROLC1(c))
//
//// INCLUDE =========================================================================================
//
//#include "Output.h"
//#include "Row.h"
//
//#include "../NhCore/Unicode.h"
//#include "../NhCore/Memory.h"
//
//#include "Common/Macro.h"
//#include NH_TTY_FLOW
//#include NH_TTY_CUSTOM_CHECK
//
//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//#include <unistd.h>
//#include <wchar.h>
//#include <limits.h>
//
//// DECLARE =========================================================================================
//
///* CSI Escape sequence structs */
///* ESC '[' [[ [<priv>] <arg> [;]] <mode> [<mode>]] */
//typedef struct Nh_TTY_CSIEscape {
//    char buf[ESC_BUF_SIZ]; /* raw string */
//    int len;               /* raw string length */
//    char priv;
//    int arg[ESC_ARG_SIZ];
//    int narg;              /* nb of args */
//    char mode[2];
//} Nh_TTY_CSIEscape;
//
///* STR Escape sequence structs */
///* ESC type [[ [<priv>] <arg> [;]] <mode>] ESC '\' */
//typedef struct Nh_TTY_STREscape{
//    char type;             /* ESC type ... */
//    char buf[STR_BUF_SIZ]; /* raw string */
//    int len;               /* raw string length */
//    char *args[STR_ARG_SIZ];
//    int narg;              /* nb of args */
//} Nh_TTY_STREscape;
//
//// DATA ============================================================================================
//
//Nh_TTY_CSIEscape CSIEscSeq;
//Nh_TTY_STREscape STREscSeq;
//
//// INIT ============================================================================================
//
//Nh_TTY_Output Nh_TTY_initOutput()
//{
//NH_TTY_BEGIN()
//
//    Nh_TTY_Output Output;
//
//    Output.Rows = Nh_initArray(sizeof(Nh_TTY_Row), 1024);
//    Output.showCTRL = NH_FALSE;
//    Output.encoding = NH_TTY_ENCODING_UTF8;
//    Output.cursorX = 1;
//    Output.cursorY = 1;
//    Output.rowOffset = 0;
//    Output.colOffset = 0;
//    Output.rx = 0;
//    Output.esc = 0;
//
//NH_TTY_END(Output)
//}
//
//// CURSOR ==========================================================================================
//
////void Nh_TTY_moveCursor(
////    Nh_TTY_Output *Output_p, int key) 
////{
////NH_TTY_BEGIN()
////
////    Nh_TTY_Row *Row_p = (Output_p->cursorY > Output_p->Rows.length) ? NULL : &((Nh_TTY_Row*)Output_p->Rows.bytes_p)[Output_p->cursorY];
////
////    switch (key) 
////    {
////        case NH_TTY_KEY_ARROW_LEFT:
////            if (Output_p->cursorX != 0) {
////                Output_p->cursorX--;
////            } else if (Output_p->cursorY > 0) {
////                Output_p->cursorY--;
////                Output_p->cursorX = ((Nh_TTY_Row*)Output_p->Rows.bytes_p)[Output_p->cursorY].size;
////            }
////            break;
////
////        case NH_TTY_KEY_ARROW_RIGHT:
////            if (Row_p && Output_p->cursorX < Row_p->size) {
////                Output_p->cursorX++;
////            } else if (Row_p && Output_p->cursorX == Row_p->size) {
////                Output_p->cursorY++;
////                Output_p->cursorX = 0;
////            }
////            break;
////
////        case NH_TTY_KEY_ARROW_UP:
////            if (Output_p->cursorY != 0) {Output_p->cursorY--;}
////            break;
////
////        case NH_TTY_KEY_ARROW_DOWN:
////            if (Output_p->cursorY < Output_p->Rows.length) {Output_p->cursorY++;}
////            break;
////    }
////  
////    Row_p = (Output_p->cursorY > Output_p->Rows.length) ? NULL : &((Nh_TTY_Row*)Output_p->Rows.bytes_p)[Output_p->cursorY];
////    int rowlen = Row_p ? Row_p->size : 0;
////    if (Output_p->cursorX > rowlen) {Output_p->cursorX = rowlen;}
////
////NH_TTY_SILENT_END()
////}
//
//void Nh_TTY_scroll(
//    Nh_TTY_Output *Output_p, int screenRows, int screenCols) 
//{
//NH_TTY_BEGIN()
//
//    Output_p->rx = 0;
//
//    if (Output_p->cursorY < Output_p->Rows.length) {
//        Output_p->rx = Nh_TTY_rowCxToRx(&((Nh_TTY_Row*)Output_p->Rows.bytes_p)[Output_p->cursorY], Output_p->cursorX);
//    }
//  
//    if (Output_p->cursorY < Output_p->rowOffset) {
//        Output_p->rowOffset = Output_p->cursorY;
//    }
//    if (Output_p->cursorY >= Output_p->rowOffset + screenRows) {
//        Output_p->rowOffset = Output_p->cursorY - screenRows + 1;
//    }
//    if (Output_p->rx < Output_p->colOffset) {
//        Output_p->colOffset = Output_p->rx;
//    }
//    if (Output_p->rx >= Output_p->colOffset + screenCols) {
//        Output_p->colOffset = Output_p->rx - screenCols + 1;
//    }
//
//NH_TTY_SILENT_END()
//}
//
//// ESC SEQUENCE ====================================================================================
//
//static void Nh_TTY_resetSTR()
//{
//    memset(&STREscSeq, 0, sizeof(STREscSeq));
//}
//
//static void Nh_TTY_parseSTR(void)
//{
//NH_TTY_BEGIN()
//
//    int c;
//    char *p = STREscSeq.buf;
//    
//    STREscSeq.narg = 0;
//    STREscSeq.buf[STREscSeq.len] = '\0';
//    
//    if (*p == '\0') {NH_TTY_SILENT_END()}
//    
//    while (STREscSeq.narg < STR_ARG_SIZ) {
//        STREscSeq.args[STREscSeq.narg++] = p;
//        while ((c = *p) != ';' && c != '\0') {++p;}
//        if (c == '\0') {NH_TTY_SILENT_END()}
//        *p++ = '\0';
//    }
//
//NH_TTY_SILENT_END()
//}
//
//static void Nh_TTY_handleSTR(
//    Nh_TTY_Output *Output_p)
//{
//NH_TTY_BEGIN()
//
//    char *p = NULL, *dec;
//    int j, narg, par;
//    
//    Output_p->esc &= ~(NH_TTY_ESC_STR_END | NH_TTY_ESC_STR);
//    Nh_TTY_parseSTR();
//    par = (narg = STREscSeq.narg) ? atoi(STREscSeq.args[0]) : 0;
//    
//    switch (STREscSeq.type) 
//    {
//        case ']': /* OSC -- Operating System Command */
//            switch (par) {
//                case 0:
//                case 1:
//                case 2:
////                  if (narg > 1) {xsettitle(strescseq.args[1]);}
//                    NH_TTY_SILENT_END()
//                case 52:
//    //              if (narg > 2) {
//    //                  dec = base64dec(strescseq.args[2]);
//    //                  if (dec) {
//    //                      xsetsel(dec);
//    //                      xclipcopy();
//    //                  } else {
//    //                      fprintf(stderr, "erresc: invalid base64\n");
//    //                  }
//    //              }
//                    NH_TTY_SILENT_END()
//                case 4: /* color set */
//                case 10: /* foreground set */
//                case 11: /* background set */
//                case 12: /* cursor color */
//                    if ((par == 4 && narg < 3) || narg < 2)
//                    	break;
//                    p = STREscSeq.args[((par == 4) ? 2 : 1)];
//                    /* FALLTHROUGH */
//                case 104: /* color reset, here p = NULL */
//                    if (par == 10) {
////                        j = defaultfg;
//                    } else if (par == 11) {
////                        j = defaultbg;
//                    } else if (par == 12) {
////                        j = defaultcs;
//                    } else {
//                        j = (narg > 1) ? atoi(STREscSeq.args[1]) : -1;
//                    } 
//    //            	if (xsetcolorname(j, p)) {
//    //            		if (par == 104 && narg <= 1)
//    //            			return; /* color reset without parameter */
//    //            		fprintf(stderr, "erresc: invalid color j=%d, p=%s\n",
//    //            			j, p ? p : "(null)");
//    //            	} else {
//    //            		/*
//    //            		 * TODO if defaultbg color is changed, borders
//    //            		 * are dirty
//    //            		 */
//    //            		if (j == defaultbg)
//    //            			xclearwin();
//    //            		redraw();
//    //            	}
//                    return;
//            }
//            break;
//        case 'k': /* old title set compatibility */
////            xsettitle(strescseq.args[0]);
//            NH_TTY_SILENT_END()
//        case 'P': /* DCS -- Device Control String */
////        	term.mode |= ESC_DCS;
//        case '_': /* APC -- Application Program Command */
//        case '^': /* PM -- Privacy Message */
//	    NH_TTY_SILENT_END()
//    }
//
//    fprintf(stderr, "erresc: unknown str ");
//
//NH_TTY_SILENT_END()
//}
//
//static void Nh_TTY_tstrsequence(
//    Nh_TTY_Output *Output_p, unsigned char c)
//{
//NH_TTY_BEGIN()
//
//    Nh_TTY_resetSTR();
//    
//    switch (c) {
//        case 0x90:   /* DCS -- Device Control String */
//            c = 'P';
//            Output_p->esc |= NH_TTY_ESC_DCS;
//            break;
//        case 0x9f:   /* APC -- Application Program Command */
//            c = '_';
//            break;
//        case 0x9e:   /* PM -- Privacy Message */
//            c = '^';
//            break;
//        case 0x9d:   /* OSC -- Operating System Command */
//            c = ']';
//            break;
//    }
//    STREscSeq.type = c;
//    Output_p->esc |= NH_TTY_ESC_STR;
//
//NH_TTY_SILENT_END()
//}
//
//static int Nh_TTY_handleEsc(
//    Nh_TTY_Output *Output_p, unsigned char ascii)
//{
//    switch (ascii) 
//    {
//        case '[':
//            Output_p->esc |= NH_TTY_ESC_CSI;
//            return 0;
//        case '#':
//            Output_p->esc |= NH_TTY_ESC_TEST;
//            return 0;
//        case '%':
//            Output_p->esc |= NH_TTY_ESC_UTF8;
//            return 0;
//        case 'P': /* DCS -- Device Control String */
//        case '_': /* APC -- Application Program Command */
//        case '^': /* PM -- Privacy Message */
//        case ']': /* OSC -- Operating System Command */
//        case 'k': /* old title set compatibility */
//            Nh_TTY_tstrsequence(Output_p, ascii);
//            return 0;
//        case 'n': /* LS2 -- Locking shift 2 */
//        case 'o': /* LS3 -- Locking shift 3 */
////            term.charset = 2 + (ascii - 'n');
//            break;
//        case '(': /* GZD4 -- set primary charset G0 */
//        case ')': /* G1D4 -- set secondary charset G1 */
//        case '*': /* G2D4 -- set tertiary charset G2 */
//        case '+': /* G3D4 -- set quaternary charset G3 */
////	    term.icharset = ascii - '(';
//            Output_p->esc |= NH_TTY_ESC_ALTCHARSET;
//            return 0;
//        case 'D': /* IND -- Linefeed */
////            if (term.c.y == term.bot) {
////            	tscrollup(term.top, 1, 1);
////            } else {
////            	tmoveto(term.c.x, term.c.y+1);
////            }
//            break;
//        case 'E': /* NEL -- Next line */
////            tnewline(1); /* always go to first col */
//            break;
//        case 'H': /* HTS -- Horizontal tab stop */
////            term.tabs[term.c.x] = 1;
//            break;
//        case 'M': /* RI -- Reverse index */
////            if (term.c.y == term.top) {
////            	tscrolldown(term.top, 1, 1);
////            } else {
////            	tmoveto(term.c.x, term.c.y-1);
////            }
//            break;
//        case 'Z': /* DECID -- Identify Terminal */
////            ttywrite(vtiden, strlen(vtiden), 0);
//            break;
//        case 'c': /* RIS -- Reset to initial state */
////            treset();
////            resettitle();
////            xloadcols();
//            break;
//        case '=': /* DECPAM -- Application keypad */
////            xsetmode(1, MODE_APPKEYPAD);
//            break;
//        case '>': /* DECPNM -- Normal keypad */
////            xsetmode(0, MODE_APPKEYPAD);
//            break;
//        case '7': /* DECSC -- Save Cursor */
////            tcursor(CURSOR_SAVE);
//            break;
//        case '8': /* DECRC -- Restore Cursor */
////            tcursor(CURSOR_LOAD);
//            break;
//        case '\\': /* ST -- String Terminator */
//            if (Output_p->esc & NH_TTY_ESC_STR_END)
//                Nh_TTY_handleSTR(Output_p);
//            break;
//        default:
////            fprintf(stderr, "erresc: unknown sequence ESC 0x%02X '%c'\n",
////            	(unsigned char) ascii, isprint(ascii)? ascii:'.');
//            break;
//    }
//
//    return 1;
//}
//
//// TERMINAL MODE ===================================================================================
//
//static void Nh_TTY_setTerminalMode(
//    int priv, int set, int *args, int narg)
//{
//    int alt, *lim;
//    
//    for (lim = args + narg; args < lim; ++args) {
//        if (priv) {
//            switch (*args) {
//                case 1: /* DECCKM -- Cursor key */
////                    xsetmode(set, MODE_APPCURSOR);
//                    break;
//                case 5: /* DECSCNM -- Reverse video */
////                    xsetmode(set, MODE_REVERSE);
//                    break;
//                case 6: /* DECOM -- Origin */
////                    MODBIT(term.c.state, set, CURSOR_ORIGIN);
////                    tmoveato(0, 0);
//                    break;
//                case 7: /* DECAWM -- Auto wrap */
////                    MODBIT(term.mode, set, MODE_WRAP);
//                    break;
//                case 0:  /* Error (IGNORED) */
//                case 2:  /* DECANM -- ANSI/VT52 (IGNORED) */
//                case 3:  /* DECCOLM -- Column  (IGNORED) */
//                case 4:  /* DECSCLM -- Scroll (IGNORED) */
//                case 8:  /* DECARM -- Auto repeat (IGNORED) */
//                case 18: /* DECPFF -- Printer feed (IGNORED) */
//                case 19: /* DECPEX -- Printer extent (IGNORED) */
//                case 42: /* DECNRCM -- National characters (IGNORED) */
//                case 12: /* att610 -- Start blinking cursor (IGNORED) */
//                    break;
//                case 25: /* DECTCEM -- Text Cursor Enable Mode */
////                    xsetmode(!set, MODE_HIDE);
//                    break;
//                case 9:    /* X10 mouse compatibility mode */
////                    xsetpointermotion(0);
////                    xsetmode(0, MODE_MOUSE);
////                    xsetmode(set, MODE_MOUSEX10);
//                    break;
//                case 1000: /* 1000: report button press */
////                    xsetpointermotion(0);
////                    xsetmode(0, MODE_MOUSE);
////                    xsetmode(set, MODE_MOUSEBTN);
//                    break;
//                case 1002: /* 1002: report motion on button press */
////                    xsetpointermotion(0);
////                    xsetmode(0, MODE_MOUSE);
////                    xsetmode(set, MODE_MOUSEMOTION);
//                    break;
//                case 1003: /* 1003: enable all mouse motions */
////                    xsetpointermotion(set);
////                    xsetmode(0, MODE_MOUSE);
////                    xsetmode(set, MODE_MOUSEMANY);
//                    break;
//                case 1004: /* 1004: send focus events to tty */
////                    xsetmode(set, MODE_FOCUS);
//                    break;
//                case 1006: /* 1006: extended reporting mode */
////                    xsetmode(set, MODE_MOUSESGR);
//                    break;
//                case 1034:
////                    xsetmode(set, MODE_8BIT);
//                    break;
//                case 1049: /* swap screen & set/restore cursor as xterm */
////                    if (!allowaltscreen)
////                    	break;
////                    tcursor((set) ? CURSOR_SAVE : CURSOR_LOAD);
//                    /* FALLTHROUGH */
//                case 47: /* swap screen */
//                case 1047:
////                    if (!allowaltscreen)
////                    	break;
////                    alt = IS_SET(MODE_ALTSCREEN);
////                    if (alt) {
////                    	tclearregion(0, 0, term.col-1,
////                    			term.row-1);
////                    }
////                    if (set ^ alt) /* set is always 1 or 0 */
////                    	tswapscreen();
////                    if (*args != 1049)
////                    	break;
//                    /* FALLTHROUGH */
//                case 1048:
////                    tcursor((set) ? CURSOR_SAVE : CURSOR_LOAD);
//                    break;
//                case 2004: /* 2004: bracketed paste mode */
////                    xsetmode(set, MODE_BRCKTPASTE);
//                    break;
//                    /* Not implemented mouse modes. See comments there. */
//                case 1001: /* mouse highlight mode; can hang the
//                	      terminal by design when implemented. */
//                case 1005: /* UTF-8 mouse mode; will confuse
//                	      applications not supporting UTF-8
//                	      and luit. */
//                case 1015: /* urxvt mangled mouse mode; incompatible
//                	      and can be mistaken for other control
//                	      codes. */
//                    break;
//                default:
//            	    break;
//            }
//        } else {
//            switch (*args) {
//            case 0:  /* Error (IGNORED) */
//                break;
//            case 2:
////                xsetmode(set, MODE_KBDLOCK);
//                break;
//            case 4:  /* IRM -- Insertion-replacement */
////                MODBIT(term.mode, set, MODE_INSERT);
//                break;
//            case 12: /* SRM -- Send/Receive */
////                MODBIT(term.mode, !set, MODE_ECHO);
//                break;
//            case 20: /* LNM -- Linefeed/new line */
////                MODBIT(term.mode, set, MODE_CRLF);
//                break;
//            default:
//                break;
//            }
//        }
//    }
//}
//
//// CSI SEQUENCE ====================================================================================
//
//static void Nh_TTY_parseCSI()
//{
//    char *p = CSIEscSeq.buf, *np;
//    long int v;
//    
//    CSIEscSeq.narg = 0;
//    if (*p == '?') {
//        CSIEscSeq.priv = 1;
//        p++;
//    }
//    
//    CSIEscSeq.buf[CSIEscSeq.len] = '\0';
//    while (p < CSIEscSeq.buf + CSIEscSeq.len) {
//        np = NULL;
//        v = strtol(p, &np, 10);
//        if (np == p)
//        	v = 0;
//        if (v == LONG_MAX || v == LONG_MIN)
//        	v = -1;
//        CSIEscSeq.arg[CSIEscSeq.narg++] = v;
//        p = np;
//        if (*p != ';' || CSIEscSeq.narg == ESC_ARG_SIZ)
//        	break;
//        p++;
//    }
//    CSIEscSeq.mode[0] = *p++;
//    CSIEscSeq.mode[1] = (p < CSIEscSeq.buf + CSIEscSeq.len) ? *p : '\0';
//}
//
//static void Nh_TTY_resetCSI()
//{
//NH_TTY_BEGIN()
//
//    memset(&CSIEscSeq, 0, sizeof(CSIEscSeq));
//
//NH_TTY_SILENT_END()
//}
//
//static void Nh_TTY_handleCSI()
//{
//    char buf[40];
//    int len;
//    
//    switch (CSIEscSeq.mode[0]) 
//    {
//        default:
//        unknown:
////            printf(stderr, "erresc: unknown csi ");
//            break;
//        case '@': /* ICH -- Insert <n> blank char */
//            DEFAULT(CSIEscSeq.arg[0], 1);
//            break;
//        case 'A': /* CUU -- Cursor <n> Up */
//            DEFAULT(CSIEscSeq.arg[0], 1);
//            break;
//        case 'B': /* CUD -- Cursor <n> Down */
//        case 'e': /* VPR --Cursor <n> Down */
//            DEFAULT(CSIEscSeq.arg[0], 1);
//            break;
//        case 'i': /* MC -- Media Copy */
//            switch (CSIEscSeq.arg[0]) {
//                case 0:
//                    break;
//                case 1:
//                    break;
//                case 2:
//                    break;
//                case 4:
//                    break;
//                case 5:
//                    break;
//            }
//            break;
//        case 'c': /* DA -- Device Attributes */
//            break;
//        case 'C': /* CUF -- Cursor <n> Forward */
//        case 'a': /* HPR -- Cursor <n> Forward */
//            DEFAULT(CSIEscSeq.arg[0], 1);
//            break;
//        case 'D': /* CUB -- Cursor <n> Backward */
//            DEFAULT(CSIEscSeq.arg[0], 1);
//            break;
//        case 'E': /* CNL -- Cursor <n> Down and first col */
//            DEFAULT(CSIEscSeq.arg[0], 1);
//            break;
//        case 'F': /* CPL -- Cursor <n> Up and first col */
//            DEFAULT(CSIEscSeq.arg[0], 1);
//            break;
//        case 'g': /* TBC -- Tabulation clear */
//            switch (CSIEscSeq.arg[0]) {
//                case 0: /* clear current tab stop */
//                    break;
//                case 3: /* clear all the tabs */
//                    break;
//                default:
//                    goto unknown;
//            }
//            break;
//        case 'G': /* CHA -- Move to <col> */
//        case '`': /* HPA */
//            DEFAULT(CSIEscSeq.arg[0], 1);
//            break;
//        case 'H': /* CUP -- Move to <row> <col> */
//        case 'f': /* HVP */
//            DEFAULT(CSIEscSeq.arg[0], 1);
//            DEFAULT(CSIEscSeq.arg[1], 1);
//            break;
//        case 'I': /* CHT -- Cursor Forward Tabulation <n> tab stops */
//            DEFAULT(CSIEscSeq.arg[0], 1);
//            break;
//        case 'J': /* ED -- Clear screen */
//            switch (CSIEscSeq.arg[0]) {
//                case 0: /* below */
//                    break;
//                case 1: /* above */
//                    break;
//                case 2: /* all */
//                    break;
//                default:
//                    goto unknown;
//            }
//            break;
//        case 'K': /* EL -- Clear line */
//            switch (CSIEscSeq.arg[0]) {
//                case 0: /* right */
//                    break;
//                case 1: /* left */
//                    break;
//                case 2: /* all */
//                    break;
//            }
//            break;
//        case 'S': /* SU -- Scroll <n> line up */
//            DEFAULT(CSIEscSeq.arg[0], 1);
////            tscrollup(term.top, csiescseq.arg[0], 0);
//            break;
//        case 'T': /* SD -- Scroll <n> line down */
//            DEFAULT(CSIEscSeq.arg[0], 1);
////            tscrolldown(term.top, csiescseq.arg[0], 0);
//            break;
//        case 'L': /* IL -- Insert <n> blank lines */
//            DEFAULT(CSIEscSeq.arg[0], 1);
////            tinsertblankline(csiescseq.arg[0]);
//            break;
//        case 'l': /* RM -- Reset Mode */
//            Nh_TTY_setTerminalMode(CSIEscSeq.priv, 0, CSIEscSeq.arg, CSIEscSeq.narg);
//            break;
//        case 'M': /* DL -- Delete <n> lines */
//            DEFAULT(CSIEscSeq.arg[0], 1);
////            tdeleteline(csiescseq.arg[0]);
//            break;
//        case 'X': /* ECH -- Erase <n> char */
//            DEFAULT(CSIEscSeq.arg[0], 1);
////            tclearregion(term.c.x, term.c.y, term.c.x + csiescseq.arg[0] - 1, term.c.y);
//            break;
//        case 'P': /* DCH -- Delete <n> char */
//            DEFAULT(CSIEscSeq.arg[0], 1);
////            tdeletechar(csiescseq.arg[0]);
//            break;
//        case 'Z': /* CBT -- Cursor Backward Tabulation <n> tab stops */
//            DEFAULT(CSIEscSeq.arg[0], 1);
////            tputtab(-csiescseq.arg[0]);
//            break;
//        case 'd': /* VPA -- Move to <row> */
//            DEFAULT(CSIEscSeq.arg[0], 1);
////            tmoveato(term.c.x, csiescseq.arg[0]-1);
//            break;
//        case 'h': /* SM -- Set terminal mode */
//            Nh_TTY_setTerminalMode(CSIEscSeq.priv, 1, CSIEscSeq.arg, CSIEscSeq.narg);
//            break;
//        case 'm': /* SGR -- Terminal attribute (color) */
//            break;
//        case 'n': /* DSR – Device Status Report (cursor position) */
//            break;
//        case 'r': /* DECSTBM -- Set Scrolling Region */
//            if (CSIEscSeq.priv) {
//                goto unknown;
//            } else {
//                DEFAULT(CSIEscSeq.arg[0], 1);
////                DEFAULT(CSIEscSeq.arg[1], term.row);
////                tsetscroll(csiescseq.arg[0]-1, csiescseq.arg[1]-1);
////                tmoveato(0, 0);
//            }
//            break;
//        case 's': /* DECSC -- Save cursor position (ANSI.SYS) */
//            break;
//        case 'u': /* DECRC -- Restore cursor position (ANSI.SYS) */
//            break;
//        case ' ':
//            break;
//    }
//}
//
//// ESC =============================================================================================
//
//static void Nh_TTY_handleControlCode(
//    Nh_TTY_Output *Output_p, uint_least32_t ascii)
//{
//    switch (ascii) 
//    {
//        case '\t':   /* HT */
//            return;
//        case '\b':   /* BS */
//            Nh_TTY_removeFromRow(Output_p, 1);
//            return;
//        case '\r':   /* CR */
//            return;
//        case '\f':   /* LF */
//        case '\v':   /* VT */
//        case '\n':   /* LF */
//           Nh_TTY_addRow(Output_p);
//            return;
//        case '\a':   /* BEL */
//            break;
//        case '\033': /* ESC, in hex: \x1b */
//            Nh_TTY_resetCSI();
//            Output_p->esc &= ~(NH_TTY_ESC_CSI | NH_TTY_ESC_ALTCHARSET | NH_TTY_ESC_TEST);
//            Output_p->esc |= NH_TTY_ESC_START;
//
////            if (!Nh_TTY_getTerminal()->standalone) {
////                Nh_TTY_addToRow(Output_p, (char*)&ascii, 1, NH_FALSE);
////            }
//            return;
//        case '\016': /* SO (LS1 -- Locking shift 1) */
//        case '\017': /* SI (LS0 -- Locking shift 0) */
//            return;
//        case '\032': /* SUB */
//        case '\030': /* CAN */
//            Nh_TTY_resetCSI();
//            break;
//        case '\005': /* ENQ (IGNORED) */
//        case '\000': /* NUL (IGNORED) */
//        case '\021': /* XON (IGNORED) */
//        case '\023': /* XOFF (IGNORED) */
//        case 0177:   /* DEL (IGNORED) */
//            return;
//        case 0x80:   /* TODO: PAD */
//        case 0x81:   /* TODO: HOP */
//        case 0x82:   /* TODO: BPH */
//        case 0x83:   /* TODO: NBH */
//        case 0x84:   /* TODO: IND */
//            break;
//        case 0x85:   /* NEL -- Next line */
//            break;
//        case 0x86:   /* TODO: SSA */
//        case 0x87:   /* TODO: ESA */
//            break;
//        case 0x88:   /* HTS -- Horizontal tab stop */
//            break;
//        case 0x89:   /* TODO: HTJ */
//        case 0x8a:   /* TODO: VTS */
//        case 0x8b:   /* TODO: PLD */
//        case 0x8c:   /* TODO: PLU */
//        case 0x8d:   /* TODO: RI */
//        case 0x8e:   /* TODO: SS2 */
//        case 0x8f:   /* TODO: SS3 */
//        case 0x91:   /* TODO: PU1 */
//        case 0x92:   /* TODO: PU2 */
//        case 0x93:   /* TODO: STS */
//        case 0x94:   /* TODO: CCH */
//        case 0x95:   /* TODO: MW */
//        case 0x96:   /* TODO: SPA */
//        case 0x97:   /* TODO: EPA */
//        case 0x98:   /* TODO: SOS */
//        case 0x99:   /* TODO: SGCI */
//            break;
//        case 0x9a:   /* DECID -- Identify Terminal */
////            ttywrite(vtiden, strlen(vtiden), 0);
//            break;
//        case 0x9b:   /* TODO: CSI */
//        case 0x9c:   /* TODO: ST */
//            break;
//        case 0x90:   /* DCS -- Device Control String */
//        case 0x9d:   /* OSC -- Operating System Command */
//        case 0x9e:   /* PM -- Privacy Message */
//        case 0x9f:   /* APC -- Application Program Command */
//            Nh_TTY_tstrsequence(Output_p, ascii);
//            return;
//    }
//
//    /* only CAN, SUB, \a and C1 chars interrupt a sequence */
////    term.esc &= ~(ESC_STR_END|ESC_STR);
//}
//
//// PUT =============================================================================================
//
//static void Nh_TTY_putChar(
//    Nh_TTY_Output *Output_p, uint_least32_t u)
//{
//    NH_BOOL render = NH_TRUE;
//    char bytes_p[NH_UTF8_SIZE];
//    int control = ISCONTROL(u);
//    int width, len;
//    
//    if (Output_p->encoding != NH_TTY_ENCODING_UTF8) {
//        bytes_p[0] = u;
//        width = len = 1;
//    } else {
//        len = Nh_encodeToUTF8(u, bytes_p);
//        if (!control && (width = wcwidth(u)) == -1) {
//            memcpy(bytes_p, "\357\277\275", 4); /* UTF_INVALID */
//            width = 1;
//        }
//    }
//    
//    /*
//     * STR sequence must be checked before anything else
//     * because it uses all following characters until it
//     * receives a ESC, a SUB, a ST or any other C1 control
//     * character.
//     */
//    if (Output_p->esc & NH_TTY_ESC_STR) 
//    {
//        if (u == '\a' || u == 030 || u == 032 || u == 033 || ISCONTROLC1(u)) {
//            Output_p->esc &= ~(NH_TTY_ESC_START | NH_TTY_ESC_STR | NH_TTY_ESC_DCS);
////            if (IS_SET(MODE_SIXEL)) {
////            	/* TODO: render sixel */;
////            	term.mode &= ~MODE_SIXEL;
////            	return;
////            }
//            Output_p->esc |= NH_TTY_ESC_STR_END;
//            goto check_control_code;
//        }
//
////        if (IS_SET(MODE_SIXEL)) {
////        	/* TODO: implement sixel mode */
////        	return;
////        }
////        if (term.esc&ESC_DCS && strescseq.len == 0 && u == 'q')
////            term.mode |= MODE_SIXEL;
//
//        if (STREscSeq.len + len >= sizeof(STREscSeq.buf) - 1) {
//            /*
//             * Here is a bug in terminals. If the user never sends
//             * some code to stop the str or esc command, then st
//             * will stop responding. But this is better than
//             * silently failing with unknown characters. At least
//             * then users will report back.
//             *
//             * In the case users ever get fixed, here is the code:
//             */
//            /*
//             * term.esc = 0;
//             * strhandle();
//             */
//            return;
//        }
//
//        memmove(&STREscSeq.buf[STREscSeq.len], bytes_p, len);
//        STREscSeq.len += len;
////        if (Nh_TTY_getTerminal()->standalone) {return;}
//    }
//
//check_control_code:
//
//    /*
//     * Actions of control codes must be performed as soon they arrive
//     * because they can be embedded inside a control sequence, and
//     * they must not cause conflicts with sequences.
//     */
//    if (control) {
//        Nh_TTY_handleControlCode(Output_p, u);
//        /*
//         * control codes are not shown ever
//         */
//        return;
//    }
//
//    if (Output_p->esc & NH_TTY_ESC_START) 
//    {
////        if (!Nh_TTY_getTerminal()->standalone) {
////            Nh_TTY_addToRow(Output_p, bytes_p, len, NH_FALSE);
////        }
//
//        if (Output_p->esc & NH_TTY_ESC_CSI) {
//            CSIEscSeq.buf[CSIEscSeq.len++] = u;
//            if (BETWEEN(u, 0x40, 0x7E) || CSIEscSeq.len >= sizeof(CSIEscSeq.buf) - 1) {
//                Output_p->esc = 0;
//                Nh_TTY_parseCSI();
//                Nh_TTY_handleCSI();
//            }
//            return;
//        } else if (Output_p->esc & NH_TTY_ESC_UTF8) {
////          tdefutf8(u);
//	} else if (Output_p->esc & NH_TTY_ESC_ALTCHARSET) {
////          tdeftran(u);
//	} else if (Output_p->esc & NH_TTY_ESC_TEST) {
////          tdectest(u);
//	} else {
//            if (!Nh_TTY_handleEsc(Output_p, u)) {return;}
//            /* sequence already finished */
//	}
//
//	Output_p->esc = 0;
//    }
//    else {
//        Nh_TTY_addToRow(Output_p, bytes_p, len, NH_TRUE);
//    }
//}
//
//// WRITE ===========================================================================================
//
//int Nh_TTY_writeToOutput(
//    Nh_TTY_Output *Output_p, const char *bytes_p, int bytes)
//{
//NH_TTY_BEGIN()
//
//    int charsize;
//    uint_least32_t u;
//    int n;
//
//    for (n = 0; n < bytes; n += charsize) 
//    {
//        if (Output_p->encoding == NH_TTY_ENCODING_UTF8) {
//            /* process a complete utf8 char */
//            charsize = Nh_decodeUTF8(bytes_p + n, &u, bytes - n);
//            if (charsize == 0)
//                break;
//        } else {
//            u = bytes_p[n] & 0xFF;
//            charsize = 1;
//        }
//
//        if (Output_p->showCTRL && ISCONTROL(u)) {
//            if (u & 0x80) {
//                u &= 0x7f;
//                Nh_TTY_putChar(Output_p, '^');
//                Nh_TTY_putChar(Output_p, '[');
//            } else if (u != '\n' && u != '\r' && u != '\t') {
//                u ^= 0x40;
//                Nh_TTY_putChar(Output_p, '^');
//            }
//        }
//
//        Nh_TTY_putChar(Output_p, u);
//    }
//
//NH_TTY_END(n)
//}
//
//// MAX COLS ========================================================================================
//
//int Nh_TTY_getMaxCols(
//    Nh_TTY_Output *Output_p, int cols)
//{
//NH_TTY_BEGIN()
//
//    int maxCols = 0;
//    for (int i = 0; i < Output_p->Rows.length; ++i) 
//    {
//        int rcols, rsize;
//        Nh_TTY_getRenderSizes(
//            &((Nh_TTY_Row*)Output_p->Rows.bytes_p)[i], cols, Output_p->colOffset, &rcols, &rsize
//        );
//        if (maxCols < rcols) {maxCols = rcols;}
//    }
//
//NH_TTY_END(maxCols)
//}
//

//#ifndef NH_TTY_SHELL_H
//#define NH_TTY_SHELL_H
//
//#ifndef DOXYGEN_SHOULD_SKIP_THIS
//
///**
// * Netzhaut - Web Browser Engine
// * Copyright (C) 2020  Dajo Frey
// * Published under LGPLv3
// */
//
//#include "Program.h"
//#include "Output.h"
//#include "Common/Result.h"
//
//#include "../NhCore/String.h"
//
//#include <stddef.h>
//#include <unistd.h>
//
//#endif
//
///** @addtogroup NhTTYStructs
// *  @{
// */
//
//    typedef struct Nh_TTY_Shell {
//        NH_BOOL crlf; /**<Transform each carriage-return to carriage-return/line-feed.*/
//        int fd;
//        pid_t pid;
//        Nh_TTY_Output Output;
//    } Nh_TTY_Shell;
//
///** @} */
//
///** @addtogroup NhTTYFunctions
// *  @{
// */
//
//    void Nh_TTY_writeToShell(
//        Nh_TTY_Shell *Shell_p, const char *s, size_t n, int may_echo
//    );
//    
//    void Nh_TTY_resizeShell(
//        Nh_TTY_Shell *Shell_p, int row, int col, int xpixel, int ypixel
//    );
//    
//    Nh_TTY_Shell *Nh_TTY_initShell(
//    );
//    
//    NH_TTY_RESULT Nh_TTY_updateShell(
//        Nh_TTY_Program *Prog_p
//    );
//
//    NH_TTY_RESULT Nh_TTY_drawShellRow(
//        Nh_TTY_Shell *Shell_p, Nh_String *Data_p, int row, int cols, int rows
//    );
//
//    NH_BOOL Nh_TTY_shellBusy(
//        Nh_TTY_Shell *Shell_p
//    );
//
///** @} */
//
//#endif 

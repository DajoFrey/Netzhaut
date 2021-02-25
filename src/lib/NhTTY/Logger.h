#ifndef NH_TTY_LOGGER_H
#define NH_TTY_LOGGER_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "Program.h"

#include "Common/Result.h"

#include "../NhCore/Logger.h"
#include "../NhCore/String.h"
#include "../NhCore/List.h"
#include "../NhCore/Time.h"

#endif

/** @addtogroup NhTTYStructs
 *  @{
 */

    typedef struct Nh_TTY_LoggerNode {
        Nh_LoggerNode *LoggerNode_p;
        NH_BOOL isOpen;
        NH_BOOL isCurrent;
        NH_BOOL isSelected;
        NH_BOOL hasFocus;
        long focusedRow;
        long offset;
        int peak;
        Nh_List Children;
        struct Nh_TTY_LoggerNode *Parent_p;
    } Nh_TTY_LoggerNode;

    typedef struct Nh_TTY_Logger {
        NH_BOOL showCategories;
        int listingWidth;
        int height;
        Nh_TTY_LoggerNode Root;
        Nh_SystemTime LastUpdate;
        double updateIntervalInSeconds;
    } Nh_TTY_Logger;

/** @} */

/** @addtogroup NhTTYFunctions
 *  @{
 */

    void *Nh_TTY_initLogger(
    );

    NH_TTY_RESULT Nh_TTY_updateLogger(
        Nh_TTY_Program *Program_p
    );

    NH_TTY_RESULT Nh_TTY_drawLoggerRow(
        Nh_TTY_Program *Program_p, Nh_UTF8String *Row_p, int width, int height, int row
    );

    NH_TTY_RESULT Nh_TTY_handleLoggerInput(
        Nh_TTY_Program *Program_p, NH_UNICODE_CODEPOINT c
    );

/** @} */

#endif 

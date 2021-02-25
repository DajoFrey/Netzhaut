#ifndef NH_TTY_ROW_H
#define NH_TTY_ROW_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "Output.h"
#include "Terminal.h"
#include "Common/Result.h"

#include <stddef.h>

#endif

/** @addtogroup NhTTYStructs
 *  @{
 */

    typedef struct Nh_TTY_Row {
        int size;
        char *chars_p;
        NH_BOOL *render_p;
        int flags;
    } Nh_TTY_Row;

/** @} */

/** @addtogroup NhTTYFunctions
 *  @{
 */

    int Nh_TTY_rowCxToRx(
        Nh_TTY_Row *Row_p, int cx
    ); 
    
    NH_TTY_RESULT Nh_TTY_addRow(
        Nh_TTY_Output *Output_p
    ); 

    void Nh_TTY_addToRow(
        Nh_TTY_Output *Output_p, char *bytes_p, int count, NH_BOOL render
    );

    NH_TTY_RESULT Nh_TTY_removeFromRow(
        Nh_TTY_Output *Output_p, int count
    );

    void Nh_TTY_destroyAllRows(
        Nh_TTY_Output *Output_p
    );

    void Nh_TTY_destroyRows(
        Nh_TTY_Output *Output_p, int count
    );

    void Nh_TTY_getRenderSizes(
        Nh_TTY_Row *Row_p, int maxCols, int offset, int *rcols_p, int *rsize_p
    );

/** @} */

#endif 

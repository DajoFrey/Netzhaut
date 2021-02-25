#ifndef NH_FILE_H
#define NH_FILE_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "List.h"

#include "Common/API.h"
#include "Common/Result.h"

#include <stddef.h>

#endif

/** @addtogroup NhCoreFunctions
 *  @{
 */

    NH_RESULT Nh_getCurrentDir(
        char *set_p, int size
    );

    Nh_List Nh_getDirContentPaths(
        char *dirPath_p
    );

    NH_RESULT Nh_getFilePath(
        char *filename_p, char *base_p, char *out_p
    );

    void *Nh_getFileData(
        const char* path_p, long *size_p
    );
    
    NH_RESULT Nh_writeCharsToFile(
        char *filename_p, char *chars_p
    );

    NH_BOOL Nh_fileExistsOnMachine(
        char *filename_p, char *base_p
    ); 

    NH_BOOL Nh_isRegularFile(
        const char *path_p
    );

/** @} */

#endif 

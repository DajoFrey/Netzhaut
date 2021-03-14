//#ifndef NH_TTY_OUTPUT_H
//#define NH_TTY_OUTPUT_H
//
//#ifndef DOXYGEN_SHOULD_SKIP_THIS
//
///**
// * Netzhaut - Web Browser Engine
// * Copyright (C) 2020  Dajo Frey
// * Published under LGPLv3
// */
//
//#include "../NhCore/Array.h"
//#include <stddef.h>
//
//#endif
//
///** @addtogroup NhTTYEnums
// *  @{
// */
//
//    typedef enum NH_TTY_ESC {
//        NH_TTY_ESC_START      = 1,
//        NH_TTY_ESC_CSI        = 2,
//        NH_TTY_ESC_STR        = 4,  /* OSC, PM, APC */
//        NH_TTY_ESC_ALTCHARSET = 8,
//        NH_TTY_ESC_STR_END    = 16, /* a final string was encountered */
//        NH_TTY_ESC_TEST       = 32, /* Enter in test mode */
//        NH_TTY_ESC_UTF8       = 64,
//        NH_TTY_ESC_DCS        = 128,
//    } NH_TTY_ESC;
//
//    typedef enum NH_TTY_ENCODING {
//        NH_TTY_ENCODING_UTF8,
//    } NH_TTY_ENCODING;
//
///** @} */
//
///** @addtogroup NhTTYStructs
// *  @{
// */
//
//    typedef struct Nh_TTY_Output {
//        NH_BOOL showCTRL;
//        NH_TTY_ENCODING encoding;
//        NH_TTY_ESC esc;
//        int cursorX;
//        int cursorY;
//        int rowOffset;
//        int colOffset;
//        int rx;
//        Nh_Array Rows;
//    } Nh_TTY_Output;
//
///** @} */
//
///** @addtogroup NhTTYFunctions
// *  @{
// */
//
//    Nh_TTY_Output Nh_TTY_initOutput(
//    );
//
////    void Nh_TTY_moveCursor(
////        Nh_TTY_Output *Output_p, int key
////    ); 
//
//    void Nh_TTY_scroll(
//        Nh_TTY_Output *Output_p, int screenRows, int screenCols
//    ); 
//
//    int Nh_TTY_writeToOutput(
//        Nh_TTY_Output *Output_p, const char *bytes_p, int bytes
//    );
//
//    int Nh_TTY_getMaxCols(
//        Nh_TTY_Output *Output_p, int cols
//    );
//
///** @} */
//
//#endif 

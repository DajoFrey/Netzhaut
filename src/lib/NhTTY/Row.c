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
//#define TAB_STOP 4
//
//// INCLUDE =========================================================================================
//
//#include "Row.h"
//#include "Terminal.h"
//
//#include "../NhCore/String.h"
//#include "../NhCore/Memory.h"
//
//#include "Common/Macro.h"
//#include NH_TTY_FLOW
//#include NH_TTY_DEFAULT_CHECK
//
//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//
//// ROW =============================================================================================
//
//int Nh_TTY_rowCxToRx(
//    Nh_TTY_Row *Row_p, int cx) 
//{
//    int rx = 0, j;
//    for (j = 0; j < cx; j++) {
//        if (Row_p->chars_p[j] == '\t') {
//            rx += (TAB_STOP - 1) - (rx % TAB_STOP);
//        }
//        rx++;
//    }
//    return rx;
//}
//
//NH_TTY_RESULT Nh_TTY_addRow(
//    Nh_TTY_Output *Output_p) 
//{
//NH_TTY_BEGIN()
//
//    Nh_TTY_Row *Row_p = Nh_incrementArray(&Output_p->Rows);
//    NH_TTY_CHECK_NULL(Row_p)
//
//    Row_p->flags = 0;
//    Row_p->size = 0;
//    Row_p->render_p = NULL;
//    Row_p->chars_p = NULL;
//
//NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS)
//}
//
//void Nh_TTY_addToRow(
//    Nh_TTY_Output *Output_p, char *bytes_p, int count, NH_BOOL render)
//{
//NH_TTY_BEGIN()
//
//    if (Output_p->Rows.length <= 0) {Nh_TTY_addRow(Output_p);}
//
//    Nh_TTY_Row *Row_p = &((Nh_TTY_Row*)Output_p->Rows.bytes_p)[Output_p->Rows.length - 1];
//
//    Row_p->chars_p = realloc(Row_p->chars_p, Row_p->size + count);
//    for (int i = Row_p->size, j = 0; i < Row_p->size + count; ++i, ++j) {Row_p->chars_p[i] = bytes_p[j];}
//
//    Row_p->render_p = realloc(Row_p->render_p, sizeof(NH_BOOL) * (Row_p->size + count));
//    for (int i = Row_p->size; i < Row_p->size + count; ++i) {Row_p->render_p[i] = render;}
//
//    Row_p->size += count;
//
//NH_TTY_SILENT_END()
//}
//
//NH_TTY_RESULT Nh_TTY_removeFromRow(
//    Nh_TTY_Output *Output_p, int count)
//{
//NH_TTY_BEGIN()
//
//    if (Output_p->Rows.length <= 0) {NH_TTY_END(NH_TTY_SUCCESS)}
//
//    Nh_TTY_Row *Row_p = &((Nh_TTY_Row*)Output_p->Rows.bytes_p)[Output_p->Rows.length - 1];
//    if (Row_p->size - count < 0) {NH_TTY_END(NH_TTY_SUCCESS)}
//
//    int count2 = 0;
//    for (int i = Row_p->size - 1; count > 0 && i >= 0; --i, count2++) {
//        if (Row_p->render_p[i] == NH_TRUE) {count--;}
//        Row_p->chars_p[i] = '\0'; 
//        Row_p->render_p[i] = 0;
//    }
//
//    Row_p->chars_p = realloc(Row_p->chars_p, Row_p->size - count2);
//    Row_p->render_p = realloc(Row_p->render_p, sizeof(NH_BOOL) * (Row_p->size - count2));
//    Row_p->size -= count2;
//
//NH_TTY_END(NH_TTY_SUCCESS)
//}
//
//void Nh_TTY_getRenderSizes(
//    Nh_TTY_Row *Row_p, int maxCols, int offset, int *rcols_p, int *rsize_p)
//{
//NH_TTY_BEGIN()
//
//    if (offset > 0) {exit(0);} // TODO
//
//    *rcols_p = 0, *rsize_p = 0;
//    for (int i = 0; i < Row_p->size; ++i) {
//        if (Row_p->render_p[i]) {*rcols_p += 1;}
//        *rsize_p  = i + 1;
//        if (maxCols >= 0 && *rcols_p >= maxCols) {break;}
//    }
//
//NH_TTY_SILENT_END()
//}
//
//void Nh_TTY_destroyAllRows(
//    Nh_TTY_Output *Output_p)
//{
//    for (int i = 0; i < Output_p->Rows.length; ++i) {
//        Nh_free(((Nh_TTY_Row*)Output_p->Rows.bytes_p)[i].chars_p);
//        Nh_free(((Nh_TTY_Row*)Output_p->Rows.bytes_p)[i].render_p);
//    }
//    Nh_freeArray(&Output_p->Rows);
//}
//
//void Nh_TTY_destroyRows(
//    Nh_TTY_Output *Output_p, int count)
//{
//    for (int i = 0; i < count && Output_p->Rows.length > 0; ++i) {
//        Nh_free(((Nh_TTY_Row*)Output_p->Rows.bytes_p)[Output_p->Rows.length - (i + 1)].chars_p);
//        Nh_free(((Nh_TTY_Row*)Output_p->Rows.bytes_p)[Output_p->Rows.length - (i + 1)].render_p);
//    }
//    Nh_removeTailFromArray(&Output_p->Rows, count);
//}
//

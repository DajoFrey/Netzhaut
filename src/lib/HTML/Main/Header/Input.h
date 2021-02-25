#ifndef NH_HTML_INPUT_H
#define NH_HTML_INPUT_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "../../../NhIO/Input.h"
#include "../../../NhCore/RingIterator.h"
#include "../../../NhCore/Netzhaut.h"

typedef struct Nh_HTML_Node Nh_HTML_Node;
typedef struct Nh_HTML_Document Nh_HTML_Document;

#endif

/** @addtogroup HTMLStructs Structs
 *  \ingroup HTML
 *  @{
 */

    typedef struct Nh_HTML_MouseEvent {
        Nh_IO_MouseEvent Event; 
        Nh_HTML_Node *Target_p;
        Nh_HTML_Node *Previous_p;
    } Nh_HTML_MouseEvent;

    typedef struct Nh_HTML_KeyboardEvent {
        Nh_IO_KeyboardEvent Event; 
        Nh_HTML_Node *Node_p;
    } Nh_HTML_KeyboardEvent;

    typedef struct Nh_HTML_Input {
        Nh_HTML_Node *Previous_p;
        Nh_IO_InputMark Mark;
        Nh_RingIterator MouseEvents;
        Nh_RingIterator KeyboardEvents;
    } Nh_HTML_Input;

/** @} */

/** @addtogroup HTMLFunctions Functions
 *  \ingroup HTML
 *  @{
 */

    NH_RESULT Nh_HTML_initInput(
        Nh_HTML_Input *Input_p
    );

    void Nh_HTML_freeInput(
        Nh_HTML_Input *Input_p
    );

    NH_RESULT Nh_HTML_processInput(
        Nh_HTML_Document *Document_p
    );

/** @} */

#endif 

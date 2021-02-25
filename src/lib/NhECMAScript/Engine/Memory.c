// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE ========================================================================================

#include "Memory.h"
#include "../Header/Macros.h"

#include "../../DOM/Header/HTMLCollection.h"
#include "../../DOM/Header/Event.h"
#include "../../DOM/Header/MouseEvent.h"
#include "../../DOM/Header/WheelEvent.h"

#include "../../../Netzhaut/Memory.h"

#include NH_DEFAULT_CHECK
#include NH_FLOW
#include NH_JS_UTILS

#include <string.h>

// MEMORY ==========================================================================================

Nh_List Nh_ECMAScript_eventSet(
    Nh_ECMAScript_CandidateExecution *CandidateExecution_p)
{
NH_ECMASCRIPT_BEGIN()

    // step 1
    Nh_List Events = Nh_initList(16);
    
    // step 2 
    for (int i = 0; i < CandidateExecution_p->EventsRecords.length; i++) 
    {
        Nh_ECMAScript_AgentEventsRecord *AgentEventsRecord_p = 
            &((Nh_ECMAScript_AgentEventsRecord*)CandidateExecution_p->EventsRecords.bytes_p)[i];

        // step a
        for (int j = 0; j < AgentEventsRecord_p->EventList.length; ++j) {
            Nh_ECMAScript_SharedDataBlockEvent *SharedDataBlockEvent_p = 
                &((Nh_ECMAScript_SharedDataBlockEvent*)AgentEventsRecord_p->EventList.bytes_p)[j];

            // step i
            Nh_appendToList(&Events, SharedDataBlockEvent_p);
        }
    }

NH_ECMASCRIPT_END(Events)
}

Nh_List Nh_ECMAScript_sharedDataBlockEventSet(
    Nh_ECMAScript_CandidateExecution *CandidateExecution_p)
{
NH_ECMASCRIPT_BEGIN()

    // step 1
    Nh_List Events = Nh_initList(16);
    Nh_List Tmp = Nh_ECMAScript_eventSet(CandidateExecution_p); 

    // step 2 
    for (int i = 0; i < Tmp.size; i++) 
    {
        Nh_ECMAScript_SharedDataBlockEvent *SharedDataBlockEvent_p = Tmp.data_pp[i]; 

        // step a
        if (SharedDataBlockEvent_p->type == NH_ECMASCRIPT_SHARED_DATA_BLOCK_EVENT_READ_SHARED_MEMORY
        ||  SharedDataBlockEvent_p->type == NH_ECMASCRIPT_SHARED_DATA_BLOCK_EVENT_WRITE_SHARED_MEMORY
        ||  SharedDataBlockEvent_p->type == NH_ECMASCRIPT_SHARED_DATA_BLOCK_EVENT_READ_MODIFY_WRITE_SHARED_MEMORY) {
            Nh_appendToList(&Events, SharedDataBlockEvent_p);
        }
    }

    Nh_freeList(&Tmp, NH_FALSE);

NH_ECMASCRIPT_END(Events)
}

Nh_List Nh_ECMAScript_hostEventSet(
    Nh_ECMAScript_CandidateExecution *CandidateExecution_p)
{
NH_ECMASCRIPT_BEGIN()

    // step 1
    Nh_List Events = Nh_initList(16);
    Nh_List Tmp1 = Nh_ECMAScript_eventSet(CandidateExecution_p); 
    Nh_List Tmp2 = Nh_ECMAScript_sharedDataBlockEventSet(CandidateExecution_p); 

    // step 2 
    for (int i = 0; i < Tmp.size; i++) 
    {
        Nh_ECMAScript_SharedDataBlockEvent *SharedDataBlockEvent_p = Tmp1.data_pp[i]; 

        // step
        if (!Nh_isInList(&Tmp2, SharedDataBlockEvent_p)) {
            Nh_appendToList(&Events, SharedDataBlockEvent_p);
        }
    }

    Nh_freeList(&Tmp1, NH_FALSE);
    Nh_freeList(&Tmp2, NH_FALSE);

NH_ECMASCRIPT_END(Events)
}

Nh_Array Nh_ECMAScript_composeWriteEventBytes(
    Nh_ECMAScript_CandidateExecution *CandidateExecution_p, unsigned int byteIndex, Nh_List WriteEvents)
{
NH_ECMASCRIPT_BEGIN()

    // step 1
    unsigned int byteLocation = byteIndex;

    // step 2
    Nh_Array BytesRead = Nh_initArray(1, 255);

    // step3
    for (int i = 0; i < WriteEvents.size; ++i) {
        // TODO
    }

NH_ECMASCRIPT_END(BytesRead)
}

Nh_Array Nh_ECMAScript_valueOfReadEvent(
    Nh_ECMAScript_CandidateExecution *CandidateExecution_p, Nh_ECMAScript_SharedDataBlockEvent ReadEvent)
{
NH_ECMASCRIPT_BEGIN()

    Nh_Array Result = Nh_initArray(1, 255);
    
    // step 1
    if (ReadEvent.type != NH_ECMASCRIPT_SHARED_DATA_BLOCK_EVENT_READ_SHARED_MEMORY
    &&  ReadEvent.type != NH_ECMASCRIPT_SHARED_DATA_BLOCK_EVENT_READ_MODIFY_WRITE_SHARED_MEMORY) {
        NH_ECMASCRIPT_END(Result)
    }

    // TODO

NH_ECMASCRIPT_END(Result)
}


#ifndef NH_ECMASCRIPT_MEMORY_H
#define NH_ECMASCRIPT_MEMORY_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#endif

/** @addtogroup NhECMAScriptEnums
 *  @{
 */

    // https://tc39.es/ecma262/#sec-agent-event-records
    typedef enum NH_ECMASCRIPT_SHARED_DATA_BLOCK_EVENT {
        NH_ECMASCRIPT_SHARED_DATA_BLOCK_EVENT_READ_SHARED_MEMORY,
        NH_ECMASCRIPT_SHARED_DATA_BLOCK_EVENT_WRITE_SHARED_MEMORY,
        NH_ECMASCRIPT_SHARED_DATA_BLOCK_EVENT_READ_MODIFY_WRITE_SHARED_MEMORY,
    } NH_ECMASCRIPT_SHARED_DATA_BLOCK_EVENT;

/** @} */

/** @addtogroup NhECMAScriptStructs
 *  @{
 */

    // https://tc39.es/ecma262/#sec-data-blocks
    typedef struct Nh_ECMAScript_DataBlock {
        NH_BOOL shared;
        Nh_Array Data; 
    } Nh_ECMAScript_DataBlock;

    // https://tc39.es/ecma262/#sec-agent-event-records
    typedef struct Nh_ECMAScript_SharedDataBlockEvent {
        NH_ECMASCRIPT_SHARED_DATA_BLOCK_EVENT type;
        int order;
        NH_BOOL noTear;
        Nh_ECMAScript_DataBlock *Block_p;
        unsigned int byteIndex;
        unsigned int elementSize;
        Nh_Array Payload;
//      ? modifyOp;       
    } Nh_ECMAScript_SharedDataBlockEvent;

    // https://tc39.es/ecma262/#sec-agent-event-records
    typedef struct Nh_ECMAScript_AgentEventsRecord {
        int agentSignifier;
        Nh_Array EventList;
        Nh_Array AgentSynchronizesWith;
    } Nh_ECMAScript_AgentEventsRecord;

    // https://tc39.es/ecma262/#sec-candidate-executions
    typedef struct Nh_ECMAScript_CandidateExecution {
        Nh_Array EventsRecords;
        Nh_Array ChosenValues;
    } Nh_ECMAScript_CandidateExecution;

/** @} */

/** @addtogroup NhECMAScriptFunctions
 *  @{
 */

    // https://tc39.es/ecma262/#sec-event-set
    Nh_ECMAScript_eventSet(
        Nh_ECMAScript_CandidateExecution *CandidateExecution_p
    );

    // https://tc39.es/ecma262/#sec-sharedatablockeventset
    Nh_List Nh_ECMAScript_sharedDataBlockEventSet(
        Nh_ECMAScript_CandidateExecution *CandidateExecution_p
    );
    
    // https://tc39.es/ecma262/#sec-hosteventset
    Nh_List Nh_ECMAScript_hostEventSet(
        Nh_ECMAScript_CandidateExecution *CandidateExecution_p
    );
    
    // https://tc39.es/ecma262/#sec-composewriteeventbytes
    Nh_Array Nh_ECMAScript_composeWriteEventBytes(
        Nh_ECMAScript_CandidateExecution *CandidateExecution_p, unsigned int byteIndex, Nh_List WriteEvents
    );
    
    // https://tc39.es/ecma262/#sec-valueofreadevent
    Nh_Array Nh_ECMAScript_valueOfReadEvent(
        Nh_ECMAScript_CandidateExecution *CandidateExecution_p, Nh_ECMAScript_SharedDataBlockEvent ReadEvent
    );

/** @} */

#endif

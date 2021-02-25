#ifndef NH_RING_ITERATOR_H
#define NH_RING_ITERATOR_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "Common/Result.h"

#endif

/** @addtogroup NhCoreStructs
 *  @{
 */  

    typedef struct Nh_RingIterator {
        int itemCount, itemByteSize;
        int index;                                      
        int overflow;
        void *data_p; 
    } Nh_RingIterator;
    
    typedef struct Nh_RingIteratorMark {
        int index;
        int overflow;
    } Nh_RingIteratorMark;

/** @} */

/** @addtogroup NhCoreFunctions
 *  @{
 */

    NH_RESULT Nh_initRingIterator(
        Nh_RingIterator *Iterator_p, int itemCount, int itemByteSize, void InitBuffer(Nh_RingIterator *Iterator_p, int itemCount)
    );

    void *Nh_advanceRingIterator(
        Nh_RingIterator *Iterator_p
    ); 

    void Nh_resetRingIterator(
        Nh_RingIterator *Iterator_p
    );

    void Nh_freeRingIterator(
        Nh_RingIterator *Iterator_p
    );

    void Nh_initRingIteratorMark(
        Nh_RingIteratorMark *Mark_p
    );
   
    void *Nh_incrementRingIteratorMark(
        Nh_RingIterator *Iterator_p, Nh_RingIteratorMark *Mark_p
    );
    
/** @} */

#endif 

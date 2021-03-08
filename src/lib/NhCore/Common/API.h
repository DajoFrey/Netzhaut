#ifndef NH_API_H
#define NH_API_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include <stdint.h>

#endif

/** @addtogroup NhCoreMacros
 *  @{
 */

    #define NH_BYTE char 
    #define NH_UNSIGNED_BYTE unsigned char
    #define NH_UNICODE_CODEPOINT uint_least32_t 

    #define Nh_String Nh_Array
    #define Nh_UTF8String Nh_String

    /**
     * Value inside the pixel coordinate system. Pixel (0, 0) is the top left pixel of the screen.
     */
    #define NH_PIXEL int
    
    /**
     * Value inside the clip coordinate system. Values from -1.0f to 1.0f are inside the viewport, everything else is outside.
     */
    #define NH_CLIP float

    #define NH_CLIP_LENGTH(clipValue) \
    (                                 \
        clipValue + 1.0f              \
    )
    
    #define NH_CLIP_TO_PIXEL(clipValue, Content_p, basedOnWidth)                                                                             \
    (                                                                                                                                        \
        basedOnWidth ? (NH_PIXEL)(((((float)clipValue + 1.0f) / Content_p->Settings.scale) * ((float)Content_p->Tab_p->Size.width)) / 2.0f)  \
                     : (NH_PIXEL)(((((float)clipValue + 1.0f) / Content_p->Settings.scale) * ((float)Content_p->Tab_p->Size.height)) / 2.0f) \
    )
    
    #define NH_PIXEL_TO_CLIP(pixel, Content_p, basedOnWidth)                                                                 \
    (                                                                                                                        \
        basedOnWidth ? ((((float)pixel) / ((float)Content_p->Tab_p->Size.width))  * Content_p->Settings.scale) * 2.0f - 1.0f \
                     : ((((float)pixel) / ((float)Content_p->Tab_p->Size.height)) * Content_p->Settings.scale) * 2.0f - 1.0f \
    )

/** @} */

/** @addtogroup NhCoreEnums
 *  @{
 */ 

    /**
     * Boolean values.
     */
    typedef enum NH_BOOL {
        NH_FALSE = 0, /**<Indicates false.*/
        NH_TRUE = 1,  /**<Indicates true.*/
    } NH_BOOL;

    /**
     * Alternative boolean values.
     */
    typedef enum NH_SWITCH {
        NH_OFF = 0, /**<Indicates off.*/
        NH_ON = 1,  /**<Indicates on.*/
    } NH_SWITCH;

    typedef enum NH_SIGNAL {
        NH_SIGNAL_OK,
        NH_SIGNAL_DONE,
        NH_SIGNAL_ERROR,
        NH_SIGNAL_IDLE,
        NH_SIGNAL_OPEN,
        NH_SIGNAL_CLOSE,
        NH_SIGNAL_UPDATE,
        NH_SIGNAL_INACTIVE,
        NH_SIGNAL_INIT,
        NH_SIGNAL_RESIZE,
        NH_SIGNAL_RESTART,
        NH_SIGNAL_SET_BACKGROUND_COLOR,
        NH_SIGNAL_SET_TITLE,
    } NH_SIGNAL;

/** @} */

/** @addtogroup NhCoreStructs
 *  @{
 */

    /**
     * Generic linked-list which holds data pointers. 
     */
    typedef struct Nh_Array {
        int elementSize;             /**<Number of items.*/
        int allocatedLengthPerChunk;
        int allocatedLength;
        unsigned long length;
        NH_BYTE *bytes_p;   /**<Opaque pointer to the first item in the list. A list with \ref count == 0 should have this set as NULL. The actual data should always be queried using @ref Nh_getFromLinkedList.*/
    } Nh_Array;

    typedef struct Nh_UnicodeString {
        NH_UNICODE_CODEPOINT *p;
        unsigned long length;
        Nh_Array Array;
    } Nh_UnicodeString;

    /**
     * Dynamic list implementation using a two dimensional array. 
     */
    typedef struct Nh_List {
        int chunkSize;   
        int chunkCount;
        int size;
        void **pp;
    } Nh_List;

    typedef struct Nh_BigInt {
        int TODO;   
    } Nh_BigInt;

    typedef struct Nh_PixelPosition {
        NH_PIXEL x;
        NH_PIXEL y;
    } Nh_PixelPosition;

    typedef struct Nh_PixelSize {
        NH_PIXEL width;
        NH_PIXEL height;
    } Nh_PixelSize;

    typedef struct Nh_ClipPosition {
        NH_CLIP x;
        NH_CLIP y;
    } Nh_ClipPosition;

    typedef struct Nh_ClipSize {
        NH_CLIP width;
        NH_CLIP height;
    } Nh_ClipSize;

    typedef struct Nh_Color {
        float r, g, b, a;
    } Nh_Color;

/** @} */

#endif

// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "../Header/Event.h"
#include "../Header/EventListener.h"
#include "../Header/MouseEvent.h"
#include "../Header/WheelEvent.h"
#include "../Header/HTMLElement.h"

#include "../../Main/Header/HashMap.h"
#include "../../Main/Header/Macros.h"
#include "../../Main/Header/Log.h"
#include "../../Main/Header/Interpreter.h"

#include "../../../Netzhaut/String.h"
#include "../../../Netzhaut/Memory.h"

#include NH_CUSTOM_CHECK
#include NH_FLOW

// API =============================================================================================

Nh_JS_Result Nh_JS_Event_Event(
    Nh_JS_Runtime *JS_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

    if (aCount == 1 && Arguments_p[0].type == NH_JS_TYPE_STRING)
    {
        Nh_JS_Event Event;
        Event.type = -1;
        Event.type_p = NULL;
        Event.Target_p = NULL;
        Event.CurrentTarget_p = NULL;

        Nh_HashValue *HashValue_p = NULL;
        hashmap_get(NH_JS_HASHMAPS.EventTypes, Arguments_p[0].data_p, (void**)(&HashValue_p));

        if (HashValue_p != NULL) {Event.type = HashValue_p->number;}
        else {Event.type_p = Arguments_p[0].data_p;}

        Nh_JS_Object *Object_p;
        Nh_JS_createEventObject(&Object_p, &Event);

        NH_END(NH_JS_RESULT(NH_JS_TYPE_OBJECT, NH_TRUE, Object_p)) 
    }

NH_END(NH_JS_NULL_RESULT())
}

Nh_JS_Result Nh_JS_Event_bubbles(
    Nh_JS_Runtime *JS_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(NH_JS_NULL_RESULT())
}

Nh_JS_Result Nh_JS_Event_cancelBubble(
    Nh_JS_Runtime *JS_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(NH_JS_NULL_RESULT())
}

Nh_JS_Result Nh_JS_Event_cancelable(
    Nh_JS_Runtime *JS_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(NH_JS_NULL_RESULT())
}

Nh_JS_Result Nh_JS_Event_composed(
    Nh_JS_Runtime *JS_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(NH_JS_NULL_RESULT())
}

Nh_JS_Result Nh_JS_Event_currentTarget(
    Nh_JS_Runtime *JS_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

//    Nh_JS_Object *Object_p = Nh_JS_getObject(Function_p, NH_JS_OBJECT_EVENT);
//    Nh_JS_Event *Event_p = Object_p->data_p;
//    NH_END(NH_JS_RESULT(NH_JS_TYPE_OBJECT, NH_FALSE, Event_p->CurrentTarget.data_p))

NH_END(NH_JS_NULL_RESULT())
}

Nh_JS_Result Nh_JS_Event_deepPath(
    Nh_JS_Runtime *JS_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(NH_JS_NULL_RESULT())
}

Nh_JS_Result Nh_JS_Event_defaultPrevented(
    Nh_JS_Runtime *JS_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(NH_JS_NULL_RESULT())
}

Nh_JS_Result Nh_JS_Event_eventPhase(
    Nh_JS_Runtime *JS_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(NH_JS_NULL_RESULT())
}

Nh_JS_Result Nh_JS_Event_explicitOriginalTarget(
    Nh_JS_Runtime *JS_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(NH_JS_NULL_RESULT())
}

Nh_JS_Result Nh_JS_Event_originalTarget(
    Nh_JS_Runtime *JS_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(NH_JS_NULL_RESULT())
}

Nh_JS_Result Nh_JS_Event_returnValue(
    Nh_JS_Runtime *JS_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(NH_JS_NULL_RESULT())
}

Nh_JS_Result Nh_JS_Event_srcElement(
    Nh_JS_Runtime *JS_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(NH_JS_NULL_RESULT())
}

Nh_JS_Result Nh_JS_Event_target(
    Nh_JS_Runtime *JS_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()
    
//    Nh_JS_Object *Object_p = Nh_JS_getObject(Function_p, NH_JS_OBJECT_EVENT);
//    Nh_JS_Event *Event_p = Object_p->data_p;
//    NH_END(NH_JS_RESULT(NH_JS_TYPE_OBJECT, NH_FALSE, Event_p->Target.data_p))

NH_END(NH_JS_NULL_RESULT())
}

Nh_JS_Result Nh_JS_Event_timeStamp(
    Nh_JS_Runtime *JS_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(NH_JS_NULL_RESULT())
}

Nh_JS_Result Nh_JS_Event_type(
    Nh_JS_Runtime *JS_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(NH_JS_NULL_RESULT())
}

Nh_JS_Result Nh_JS_Event_isTrusted(
    Nh_JS_Runtime *JS_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(NH_JS_NULL_RESULT())
}

Nh_JS_Result Nh_JS_Event_createEvent(
    Nh_JS_Runtime *JS_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(NH_JS_NULL_RESULT())
}

Nh_JS_Result Nh_JS_Event_composedPath(
    Nh_JS_Runtime *JS_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(NH_JS_NULL_RESULT())
}

Nh_JS_Result Nh_JS_Event_initEvent(
    Nh_JS_Runtime *JS_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(NH_JS_NULL_RESULT())
}

Nh_JS_Result Nh_JS_Event_preventDefault(
    Nh_JS_Runtime *JS_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(NH_JS_NULL_RESULT())
}

Nh_JS_Result Nh_JS_Event_stopImmediatePropagation(
    Nh_JS_Runtime *JS_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(NH_JS_NULL_RESULT())
}

Nh_JS_Result Nh_JS_Event_stopPropagation(
    Nh_JS_Runtime *JS_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(NH_JS_NULL_RESULT())
}

// INTERNAL ========================================================================================

#include NH_DEFAULT_CHECK

NH_RESULT Nh_JS_createEventObject(
    Nh_JS_Object **Object_pp, Nh_JS_Event *Event_p)
{
NH_BEGIN()

    NH_CHECK(Nh_JS_createObject(NH_JS_OBJECT_EVENT, Object_pp))
    (*Object_pp)->data_p = Nh_allocate(sizeof(Nh_JS_Event));
    NH_CHECK_MEM((*Object_pp)->data_p);

    Nh_JS_Event Event;
    Event.type = -1;
    Event.type_p = NULL;
    Event.CurrentTarget_p = NULL;
    Event.Target_p = NULL;

    *((Nh_JS_Event*)(*Object_pp)->data_p) = Event_p == NULL ? Event : *Event_p;

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

NH_RESULT Nh_JS_destroyEventObject(
    Nh_JS_Object *Object_p)
{
NH_BEGIN()

    Nh_free(Object_p->data_p);
    Nh_JS_destroyObject(Object_p);

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

static NH_RESULT Nh_JS_dispatchEvent(
    Nh_JS_Runtime *JS_p, Nh_JS_Event *Event_p, Nh_JS_Object *Listener_p, va_list args)
{
NH_BEGIN()

//    Nh_JS_Object *Object_p = Nh_allocate(sizeof(Nh_JS_Object));
//    NH_CHECK_MEM(Object_p)
//
//    switch (Event_p->type)
//    {
//        case NH_JS_EVENT_MOUSE_MOVE  :
//        case NH_JS_EVENT_MOUSE_ENTER :
//        case NH_JS_EVENT_MOUSE_LEAVE :
//        case NH_JS_EVENT_MOUSE_DOWN  :
//        case NH_JS_EVENT_MOUSE_UP    :
//        case NH_JS_EVENT_CLICK       :
//            NH_CHECK(Nh_JS_createMouseEventObject(JS_p, &Object_p, Event_p, va_arg(args, void*)))
//            break;
//
//        case NH_JS_EVENT_WHEEL  :
//        case NH_JS_EVENT_SCROLL :
//            NH_CHECK(Nh_JS_createWheelEventObject(JS_p, &Object_p, Event_p, va_arg(args, void*)))
//            break;
//
//        default :
//            NH_CHECK(Nh_JS_createEventObject(JS_p, &Object_p, Event_p))
//    }
//
//    Nh_JS_Result Result = {
//        .freeData = true,
//        .type     = NH_JS_TYPE_OBJECT,
//        .data_p   = Object_p,
//    };
//
//    Nh_JS_logEventListener(JS_p, Listener_p, 1);
//    Nh_JS_interpretFunction(JS_p, &Listener_p->Functions_p[0], 1, &Result);

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

static NH_RESULT Nh_JS_dispatchTextualEvent(
    Nh_JS_Runtime *JS_p, Nh_JS_Event *Event_p, va_list args)
{
NH_BEGIN()

//    Nh_JS_logEvent(JS_p, Event_p);	
//
//    for (int i = 0; i < JS_p->DOM.EventListeners.count; ++i) 
//    {
//        Nh_JS_Object *ListenerObject_p = Nh_getFromLinkedList(&JS_p->DOM.EventListeners, i);
//        Nh_JS_EventListener *Listener_p = ListenerObject_p->data_p;
//
//        if (Listener_p != NULL 
//        &&  Listener_p->eventType == Event_p->type 
//        &&  Listener_p->Target.data_p == Event_p->CurrentTarget.data_p
//        &&  Listener_p->text == NH_TRUE)
//        {
//            NH_CHECK(Nh_JS_dispatchEvent(JS_p, Event_p, ListenerObject_p, args))
//        }
//    }

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

static NH_RESULT Nh_JS_dispatchNonTextualEvent(
    Nh_JS_Runtime *JS_p, Nh_JS_Event *Event_p, va_list args)
{
NH_BEGIN()

//    Nh_JS_logEvent(JS_p, Event_p);
//
//    for (int i = 0; i < JS_p->DOM.EventListeners.count; ++i) 
//    {
//        Nh_JS_Object *ListenerObject_p = Nh_getFromLinkedList(&JS_p->DOM.EventListeners, i);
//        Nh_JS_EventListener *Listener_p = ListenerObject_p->data_p;
//
//        if (Listener_p != NULL 
//        &&  Listener_p->eventType == Event_p->type 
//        &&  Listener_p->Target.data_p == Event_p->CurrentTarget.data_p) 
//        {
//            NH_CHECK(Nh_JS_dispatchEvent(JS_p, Event_p, ListenerObject_p, args))
//        }
//    }

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

NH_RESULT Nh_JS_dispatchHTMLEvent(
    Nh_JS_Runtime *JS_p, Nh_HTML_Node *BubbleTarget_p, Nh_HTML_Node *Target_p, NH_JS_EVENT type, 
    char *type_p, ...)
{
NH_BEGIN()

    va_list args;
    va_start(args, type_p);

//    Nh_JS_Event Event;
//    Event.type   = type;
//    Event.type_p = type_p;
//    Event.CurrentTarget.data_p     = Nh_JS_getHTMLElementObject(JS_p, BubbleTarget_p);
//    Event.CurrentTarget.distinct_p = Nh_JS_getDistinction(JS_p, Event.CurrentTarget.data_p);
//    Event.Target.data_p     = Event.CurrentTarget.data_p;
//    Event.Target.distinct_p = Event.CurrentTarget.distinct_p;
//
//    if (Target_p->tag == NH_HTML_TAG_CHAR || Target_p->tag == NH_HTML_TAG_TEXT) {
//        NH_CHECK(Nh_JS_dispatchTextualEvent(JS_p, &Event, args))
//    }
//    else {NH_CHECK(Nh_JS_dispatchNonTextualEvent(JS_p, &Event, args))}
//
//    if (Target_p == BubbleTarget_p)
//    {
//        Event.CurrentTarget.data_p     = JS_p->DOM.Objects.HTMLDocument_p;
//        Event.CurrentTarget.distinct_p = NULL;
//
//        if (Target_p->tag == NH_HTML_TAG_CHAR || Target_p->tag == NH_HTML_TAG_TEXT) {
//            NH_CHECK(Nh_JS_dispatchTextualEvent(JS_p, &Event, args))
//        }
//        else {NH_CHECK(Nh_JS_dispatchNonTextualEvent(JS_p, &Event, args))}
//    }

    va_end(args);

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

#include NH_CUSTOM_CHECK

char *Nh_JS_stringifyEventData(
    Nh_JS_Event *Event_p, bool newline)
{
NH_BEGIN()

    Nh_String *String_p = Nh_allocateString(NULL);

//    char *obj_p = Nh_JS_stringifyObject(Event_p->CurrentTarget.data_p, false, true);
//    NH_CHECK_NULL(NULL, obj_p)
//
//    NH_CHECK(NULL, Nh_appendFormatToString(String_p, "%s %s", Nh_JS_getEventTypes(NULL)[Event_p->type], obj_p))
//
//    if (newline) {NH_CHECK(NULL, Nh_appendFormatToString(String_p, "\n"))}
//
    char *chars_p = Nh_getChars(String_p);
//    Nh_freeString(String_p, false);
//    Nh_free(obj_p);

NH_END(chars_p)
}


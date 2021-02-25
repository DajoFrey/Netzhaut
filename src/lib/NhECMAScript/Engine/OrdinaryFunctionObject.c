// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE ========================================================================================

#include "OrdinaryFunctionObject.h"
#include "OrdinaryObject.h"
#include "TestAndCompare.h"
#include "ExecutionContext.h"

#include "../StaticSemantics/ParameterLists.h"

#include "../Common/Macro.h"
#include NH_ECMASCRIPT_CUSTOM_CHECK
#include NH_ECMASCRIPT_FLOW

#include "../../NhCore/Memory.h"

#include <string.h>

// ORDINARY FUNCTION OBJECT INTERNAL METHODS =======================================================

Nh_ECMAScript_Completion Nh_ECMAScript_ordinaryCall(
    Nh_ECMAScript_Object *This_p, Nh_ECMAScript_Any ThisArgument, Nh_List ArgumentsList)
{
NH_ECMASCRIPT_BEGIN()

NH_ECMASCRIPT_END(Nh_ECMAScript_normalEmptyCompletion())
}

// DATA ============================================================================================

static const int lookup_p[] = {0, 1, 2, -1, -1, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};

static Nh_ECMAScript_InternalMethods InternalMethods = {
    .getPrototypeOf_f    = Nh_ECMAScript_ordinaryGetPrototypeOf,
    .setPrototypeOf_f    = Nh_ECMAScript_ordinarySetPrototypeOf,
    .isExtensible_f      = Nh_ECMAScript_ordinaryIsExtensible,
    .preventExtensions_f = Nh_ECMAScript_ordinaryPreventExtensions,
    .getOwnProperty_f    = Nh_ECMAScript_ordinaryGetOwnProperty,
    .defineOwnProperty_f = Nh_ECMAScript_ordinaryDefineOwnProperty,
    .hasProperty_f       = Nh_ECMAScript_ordinaryHasProperty,
    .get_f               = Nh_ECMAScript_ordinaryGet,
    .set_f               = Nh_ECMAScript_ordinarySet,
    .delete_f            = Nh_ECMAScript_ordinaryDelete,
    .ownPropertyKeys_f   = Nh_ECMAScript_ordinaryOwnPropertyKeys,
    .call_f              = Nh_ECMAScript_ordinaryCall,
//    .construct_f         = NULL,
};

// OTHER ===========================================================================================

// https://tc39.es/ecma262/#sec-ordinaryfunctioncreate
Nh_ECMAScript_Object *Nh_ECMAScript_ordinaryFunctionCreate(
    Nh_ECMAScript_Object *Prototype_p, void *SourceText_p, Nh_ECMAScript_ParseNode *ParameterList_p, 
    Nh_ECMAScript_ParseNode *Body_p, NH_ECMASCRIPT_THIS_MODE thisMode, Nh_ECMAScript_Environment *Scope_p)
{
NH_ECMASCRIPT_BEGIN()

    Nh_ECMAScript_Object *Function_p = Nh_ECMAScript_ordinaryObjectCreate(Prototype_p, lookup_p, 16);
    NH_ECMASCRIPT_CHECK_NULL(NULL, Function_p)

    Function_p->InternalMethods_p = &InternalMethods;

    Nh_ECMAScript_setInternalSlot(&Function_p->InternalSlots, NH_ECMASCRIPT_INTERNAL_SLOT_SOURCE_TEXT, NULL);
    Nh_ECMAScript_setInternalSlot(&Function_p->InternalSlots, NH_ECMASCRIPT_INTERNAL_SLOT_FORMAL_PARAMETERS, ParameterList_p);
    Nh_ECMAScript_setInternalSlot(&Function_p->InternalSlots, NH_ECMASCRIPT_INTERNAL_SLOT_ECMASCRIPT_CODE, Body_p);

    // TODO If the source text matching Body is strict mode code, let Strict be true; else let Strict be false.
    NH_BOOL strict = NH_FALSE;
    Nh_ECMAScript_setInternalSlot(&Function_p->InternalSlots, NH_ECMASCRIPT_INTERNAL_SLOT_STRICT, (void*)strict);

    if (thisMode == NH_ECMASCRIPT_THIS_MODE_LEXICAL) {
        Nh_ECMAScript_setInternalSlot(&Function_p->InternalSlots, NH_ECMASCRIPT_INTERNAL_SLOT_THIS_MODE, (void*)NH_ECMASCRIPT_THIS_MODE_LEXICAL);
    } else if (strict) {
        Nh_ECMAScript_setInternalSlot(&Function_p->InternalSlots, NH_ECMASCRIPT_INTERNAL_SLOT_THIS_MODE, (void*)NH_ECMASCRIPT_THIS_MODE_STRICT);
    } else {
        Nh_ECMAScript_setInternalSlot(&Function_p->InternalSlots, NH_ECMASCRIPT_INTERNAL_SLOT_THIS_MODE, (void*)NH_ECMASCRIPT_THIS_MODE_GLOBAL);
    }

    Nh_ECMAScript_setInternalSlot(&Function_p->InternalSlots, NH_ECMASCRIPT_INTERNAL_SLOT_IS_CLASS_CONSTRUCTOR, (void*)NH_FALSE);
    Nh_ECMAScript_setInternalSlot(&Function_p->InternalSlots, NH_ECMASCRIPT_INTERNAL_SLOT_ENVIRONMENT, Scope_p);
    Nh_ECMAScript_setInternalSlot(&Function_p->InternalSlots, NH_ECMASCRIPT_INTERNAL_SLOT_SCRIPT_OR_MODULE, Nh_ECMAScript_getActiveScriptOrModule());
    Nh_ECMAScript_setInternalSlot(&Function_p->InternalSlots, NH_ECMASCRIPT_INTERNAL_SLOT_REALM, Nh_ECMAScript_getRunningExecutionContext()->Realm_p);
    Nh_ECMAScript_setInternalSlot(&Function_p->InternalSlots, NH_ECMASCRIPT_INTERNAL_SLOT_HOME_OBJECT, NULL);

    int len = Nh_ECMAScript_getExpectedArgumentCount(ParameterList_p);
    Nh_ECMAScript_setFunctionLength(Function_p, len);

NH_ECMASCRIPT_END(Function_p)
}

// https://tc39.es/ecma262/#sec-makeconstructor
Nh_ECMAScript_Completion Nh_ECMAScript_makeConstructor(
    Nh_ECMAScript_Object *Function_p, NH_BOOL writablePrototype, Nh_ECMAScript_Object *Prototype_p)
{
NH_ECMASCRIPT_BEGIN()

    // TODO

NH_ECMASCRIPT_END(Nh_ECMAScript_normalEmptyCompletion())
}

// https://tc39.es/ecma262/#sec-setfunctionname
Nh_ECMAScript_Completion Nh_ECMAScript_setFunctionName(
    Nh_ECMAScript_Object *Function_p, Nh_ECMAScript_Any Name, NH_BYTE *prefix_p)
{
NH_ECMASCRIPT_BEGIN()

    NH_BYTE *propertyName_p = "name";

    Nh_ECMAScript_String PropertyName;
    PropertyName.bytes_p = propertyName_p;
    PropertyName.length  = strlen(propertyName_p);

    if (Name.type == NH_ECMASCRIPT_TYPE_SYMBOL) {
        // TODO
    }

    if (Function_p->InternalSlots.lookup_p[NH_ECMASCRIPT_INTERNAL_SLOT_INITIAL_NAME] >= 0) {
        // TODO 
    }

    if (prefix_p != NULL) {
        // TODO
    }

    Nh_ECMAScript_PropertyDescriptor Descriptor;
    Descriptor.type = NH_ECMASCRIPT_PROPERTY_DATA;
    Descriptor.enumerable   = NH_FALSE;
    Descriptor.configurable = NH_TRUE;
    Descriptor.Fields.Data.writable = NH_FALSE;
    Descriptor.Fields.Data.Value    = Name; 

NH_ECMASCRIPT_END(Nh_ECMAScript_abstractDefinePropertyOrThrow(Function_p, Nh_ECMAScript_wrapString(&PropertyName), Descriptor))
}

// https://tc39.es/ecma262/#sec-setfunctionlength
Nh_ECMAScript_Completion Nh_ECMAScript_setFunctionLength(
    Nh_ECMAScript_Object *Function_p, NH_ECMASCRIPT_NUMBER len)
{
NH_ECMASCRIPT_BEGIN()

    NH_BYTE *propertyName_p = "length";

    Nh_ECMAScript_String PropertyName;
    PropertyName.bytes_p = propertyName_p;
    PropertyName.length  = strlen(propertyName_p);

    Nh_ECMAScript_PropertyDescriptor Descriptor;
    Descriptor.type = NH_ECMASCRIPT_PROPERTY_DATA;
    Descriptor.enumerable   = NH_FALSE;
    Descriptor.configurable = NH_FALSE;
    Descriptor.Fields.Data.writable = NH_FALSE;
    Descriptor.Fields.Data.Value    = Nh_ECMAScript_wrapNumber(&len); 

NH_ECMASCRIPT_END(Nh_ECMAScript_abstractDefinePropertyOrThrow(Function_p, Nh_ECMAScript_wrapString(&PropertyName), Descriptor))
}


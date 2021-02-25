// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE ========================================================================================

#include "Realm.h"
#include "ExecutionContext.h"
#include "BuiltinFunctionObject.h"
#include "OrdinaryObject.h"

#include "../Intrinsics/GlobalObject.h"
#include "../Intrinsics/Object.h"
#include "../Intrinsics/Function.h"

#include "../Common/Macro.h"
#include NH_ECMASCRIPT_DEFAULT_CHECK
#include NH_ECMASCRIPT_FLOW

#include "../../NhCore/Memory.h"

#include <string.h>

// INTRINSICS ======================================================================================

// corresponds to https://tc39.es/ecma262/#sec-createintrinsics
static NH_ECMASCRIPT_RESULT Nh_ECMAScript_createIntrinsics(
    Nh_ECMAScript_Realm *Realm_p)
{
NH_ECMASCRIPT_BEGIN()

    NH_ECMASCRIPT_CHECK(Nh_ECMAScript_createIntrinsicObject(&Realm_p->Intrinsics.Object, Realm_p))
//    NH_ECMASCRIPT_CHECK(Nh_ECMAScript_createIntrinsicFunction(&Realm_p->Intrinsics.Function, Realm_p))

NH_ECMASCRIPT_DIAGNOSTIC_END(NH_ECMASCRIPT_SUCCESS)
}

static void Nh_ECMAScript_freeIntrinsics(
    Nh_ECMAScript_Intrinsics Intrinsics)
{
NH_ECMASCRIPT_BEGIN()
NH_ECMASCRIPT_SILENT_END()
}

// REALM ===========================================================================================

// corresponds to https://www.262.ecma-international.org/11.0/index.html#sec-createrealm
static Nh_ECMAScript_Realm *Nh_ECMAScript_createRealm()
{
NH_ECMASCRIPT_BEGIN()

#include NH_ECMASCRIPT_CUSTOM_CHECK

    Nh_ECMAScript_Realm *Realm_p = Nh_allocate(sizeof(Nh_ECMAScript_Realm));
    NH_ECMASCRIPT_CHECK_MEM(NULL, Realm_p)

    NH_ECMASCRIPT_CHECK(NULL, Nh_ECMAScript_createIntrinsics(Realm_p))

    Realm_p->GlobalObject_p = NULL;
    Realm_p->GlobalEnvironment_p = NULL;

#include NH_ECMASCRIPT_DEFAULT_CHECK

NH_ECMASCRIPT_END(Realm_p)
}

// corresponds to https://www.262.ecma-international.org/11.0/index.html#sec-setrealmglobalobject
static Nh_ECMAScript_Realm *Nh_ECMAScript_setRealmGlobalObject(
    Nh_ECMAScript_Realm *Realm_p, Nh_ECMAScript_Object *GlobalObject_p, Nh_ECMAScript_Object *ThisValue_p)
{
NH_ECMASCRIPT_BEGIN()

#include NH_ECMASCRIPT_CUSTOM_CHECK

    if (GlobalObject_p == NULL) {
        Nh_ECMAScript_Intrinsics *Intrinsics_p = &Realm_p->Intrinsics;
        GlobalObject_p = Nh_ECMAScript_ordinaryObjectCreate(&Intrinsics_p->Object.Prototype, NULL, -1);
        NH_ECMASCRIPT_CHECK_MEM(NULL, GlobalObject_p)
    }

    if (ThisValue_p == NULL) {ThisValue_p = GlobalObject_p;}

    Realm_p->GlobalObject_p = GlobalObject_p;
    Realm_p->GlobalEnvironment_p = Nh_ECMAScript_newGlobalEnvironment(GlobalObject_p, ThisValue_p);
    NH_ECMASCRIPT_CHECK_MEM(NULL, Realm_p->GlobalEnvironment_p)

#include NH_ECMASCRIPT_DEFAULT_CHECK

NH_ECMASCRIPT_END(Realm_p)
}

// corresponds to https://www.262.ecma-international.org/11.0/index.html#sec-initializehostdefinedrealm
NH_ECMASCRIPT_RESULT Nh_ECMAScript_initializeHostDefinedRealm(
    Nh_Stack *ExecutionContextStack_p)
{
NH_ECMASCRIPT_BEGIN()

    Nh_ECMAScript_Realm *Realm_p = Nh_ECMAScript_createRealm();
    NH_ECMASCRIPT_CHECK_MEM(Realm_p)

    Nh_ECMAScript_ExecutionContext *NewContext_p = Nh_ECMAScript_allocateExecutionContext();
    NH_ECMASCRIPT_CHECK_MEM(NewContext_p)

    NewContext_p->Function_p = NULL;
    NewContext_p->Realm_p    = Realm_p;
    NewContext_p->ScriptOrModule.isScript = -1;
    NewContext_p->ScriptOrModule.handle_p = NULL;

    Nh_pushStack(ExecutionContextStack_p, NewContext_p);

    Nh_ECMAScript_Object *Global_p = NULL;
    Nh_ECMAScript_Object *ThisValue_p = NULL;

    NH_ECMASCRIPT_CHECK_MEM(Nh_ECMAScript_setRealmGlobalObject(Realm_p, Global_p, ThisValue_p))

    Global_p = Nh_ECMAScript_setDefaultGlobalBindings(Realm_p);

// TODO Create any host-defined global object properties on globalObj.

NH_ECMASCRIPT_DIAGNOSTIC_END(NH_ECMASCRIPT_SUCCESS)
}


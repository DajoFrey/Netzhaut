#ifndef NH_ECMASCRIPT_REALM_H
#define NH_ECMASCRIPT_REALM_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "ObjectType.h"
#include "Environment.h"

#include "../Common/Result.h"
#include "../../NhCore/Stack.h"

#endif

/** @addtogroup NhECMAScriptStructs
 *  @{
 */

    typedef struct Nh_ECMAScript_IntrinsicObject {
        // internal
        Nh_ECMAScript_String Keys_p[29];
        Nh_ECMAScript_Property Properties_p[29];
        // main objects
        Nh_ECMAScript_Object Constructor;
        Nh_ECMAScript_Object Prototype;
        // object constructor builtin function objects
        Nh_ECMAScript_Object Assign;
        Nh_ECMAScript_Object Create;
        Nh_ECMAScript_Object DefineProperties;
        Nh_ECMAScript_Object DefineProperty;
        Nh_ECMAScript_Object Entries;
        Nh_ECMAScript_Object Freeze;
        Nh_ECMAScript_Object FromEntries;
        Nh_ECMAScript_Object GetOwnPropertyDescriptor;
        Nh_ECMAScript_Object GetOwnPropertyDescriptors;
        Nh_ECMAScript_Object GetOwnPropertyNames;
        Nh_ECMAScript_Object GetOwnPropertySymbols;
        Nh_ECMAScript_Object GetPrototypeOf;
        Nh_ECMAScript_Object Is;
        Nh_ECMAScript_Object IsExtensible;
        Nh_ECMAScript_Object IsFrozen;
        Nh_ECMAScript_Object IsSealed;
        Nh_ECMAScript_Object Keys;
        Nh_ECMAScript_Object PreventExtensions;
        Nh_ECMAScript_Object Seal;
        Nh_ECMAScript_Object SetPrototypeOf;
        Nh_ECMAScript_Object Values;
        // object prototype  builtin function objects
        Nh_ECMAScript_Object HasOwnProperty;
        Nh_ECMAScript_Object IsPrototypeOf;
        Nh_ECMAScript_Object PropertyIsEnumerable;
        Nh_ECMAScript_Object ToLocaleString;
        Nh_ECMAScript_Object ToString;
        Nh_ECMAScript_Object ValueOf;
    } Nh_ECMAScript_IntrinsicObject;

    typedef struct Nh_ECMAScript_IntrinsicFunction {
        Nh_ECMAScript_Object Constructor;
        Nh_ECMAScript_Object Prototype;
    } Nh_ECMAScript_IntrinsicFunction;

    typedef struct Nh_ECMAScript_Intrinsics {
        // internal
        Nh_ECMAScript_String Keys_p[52];
        Nh_ECMAScript_Property Properties_p[52];
        // global object properties
//        Nh_ECMAScript_Object AggregateError;
//        Nh_ECMAScript_Object Array;
//        Nh_ECMAScript_Object ArrayBuffer;
//        Nh_ECMAScript_Object ArrayBufferPrototype;
//        Nh_ECMAScript_Object ArrayIteratorPrototype;
//        Nh_ECMAScript_Object ArrayPrototype;
//        Nh_ECMAScript_Object ArrayProto_entries;
//        Nh_ECMAScript_Object ArrayProto_forEach;
//        Nh_ECMAScript_Object ArrayProto_keys;
//        Nh_ECMAScript_Object ArrayProto_values;
//        Nh_ECMAScript_Object AsyncFromSyncIteratorPrototype;
//        Nh_ECMAScript_Object AsyncFunction;
//        Nh_ECMAScript_Object AsyncFunctionPrototype;
//        Nh_ECMAScript_Object AsyncGenerator;
//        Nh_ECMAScript_Object AsyncGeneratorFunction;
//        Nh_ECMAScript_Object AsyncGeneratorPrototype;
//        Nh_ECMAScript_Object AsyncIteratorPrototype;
//        Nh_ECMAScript_Object Atomics;
//        Nh_ECMAScript_Object BigInt;
//        Nh_ECMAScript_Object BigInt64Array;
//        Nh_ECMAScript_Object BigUint64Array;
//        Nh_ECMAScript_Object Boolean;
//        Nh_ECMAScript_Object BooleanPrototype;
//        Nh_ECMAScript_Object DataView;
//        Nh_ECMAScript_Object DataViewPrototype;
//        Nh_ECMAScript_Object Date;
//        Nh_ECMAScript_Object DatePrototype;
//        Nh_ECMAScript_Object decodeURI;
//        Nh_ECMAScript_Object decodeURIComponent;
//        Nh_ECMAScript_Object encodeURI;
//        Nh_ECMAScript_Object encodeURIComponent;
//        Nh_ECMAScript_Object Error;
//        Nh_ECMAScript_Object ErrorPrototype;
//        Nh_ECMAScript_Object eval;
//        Nh_ECMAScript_Object EvalError;
//        Nh_ECMAScript_Object EvalErrorPrototype;
//        Nh_ECMAScript_Object Float32Array;
//        Nh_ECMAScript_Object Float32ArrayPrototype;
//        Nh_ECMAScript_Object Float64Array;
//        Nh_ECMAScript_Object Float64ArrayPrototype;
//        Nh_ECMAScript_Object ForInIteratorPrototype;
        Nh_ECMAScript_IntrinsicFunction Function;
//        Nh_ECMAScript_Object FunctionPrototype;
//        Nh_ECMAScript_Object Generator;
//        Nh_ECMAScript_Object GeneratorFunction;
//        Nh_ECMAScript_Object GeneratorPrototype;
//        Nh_ECMAScript_Object Int8Array;
//        Nh_ECMAScript_Object Int8ArrayPrototype;
//        Nh_ECMAScript_Object Int16Array;
//        Nh_ECMAScript_Object Int16ArrayPrototype;
//        Nh_ECMAScript_Object Int32Array;
//        Nh_ECMAScript_Object Int32ArrayPrototype;
//        Nh_ECMAScript_Object isFinite;
//        Nh_ECMAScript_Object isNaN;
//        Nh_ECMAScript_Object IteratorPrototype;
//        Nh_ECMAScript_Object JSON;
//        Nh_ECMAScript_Object JSONParse;
//        Nh_ECMAScript_Object JSONStringify;
//        Nh_ECMAScript_Object Map;
//        Nh_ECMAScript_Object MapIteratorPrototype;
//        Nh_ECMAScript_Object MapPrototype;
//        Nh_ECMAScript_Object Math;
//        Nh_ECMAScript_Object Number;
//        Nh_ECMAScript_Object NumberPrototype;
        Nh_ECMAScript_IntrinsicObject Object;
//        Nh_ECMAScript_Object ObjProto_toString;
//        Nh_ECMAScript_Object ObjProto_valueOf;
//        Nh_ECMAScript_Object parseFloat;
//        Nh_ECMAScript_Object parseInt;
//        Nh_ECMAScript_Object Promise;
//        Nh_ECMAScript_Object PromisePrototype;
//        Nh_ECMAScript_Object PromiseProto_then;
//        Nh_ECMAScript_Object Promise_all;
//        Nh_ECMAScript_Object Promise_reject;
//        Nh_ECMAScript_Object Promise_resolve;
//        Nh_ECMAScript_Object Proxy;
//        Nh_ECMAScript_Object RangeError;
//        Nh_ECMAScript_Object RangeErrorPrototype;
//        Nh_ECMAScript_Object ReferenceError;
//        Nh_ECMAScript_Object ReferenceErrorPrototype;
//        Nh_ECMAScript_Object Reflect;
//        Nh_ECMAScript_Object RegExp;
//        Nh_ECMAScript_Object RegExpPrototype;
//        Nh_ECMAScript_Object RegExpStringIteratorPrototype;
//        Nh_ECMAScript_Object Set;
//        Nh_ECMAScript_Object SetIteratorPrototype;
//        Nh_ECMAScript_Object SetPrototype;
//        Nh_ECMAScript_Object SharedArrayBuffer;
//        Nh_ECMAScript_Object SharedArrayBufferPrototype;
//        Nh_ECMAScript_Object String;
//        Nh_ECMAScript_Object StringIteratorPrototype;
//        Nh_ECMAScript_Object StringPrototype;
//        Nh_ECMAScript_Object Symbol;
//        Nh_ECMAScript_Object SymbolPrototype;
//        Nh_ECMAScript_Object SyntaxError;
//        Nh_ECMAScript_Object SyntaxErrorPrototype;
//        Nh_ECMAScript_Object ThrowTypeError;
//        Nh_ECMAScript_Object TypedArray;
//        Nh_ECMAScript_Object TypedArrayPrototype;
//        Nh_ECMAScript_Object TypeError;
//        Nh_ECMAScript_Object TypeErrorPrototype;
//        Nh_ECMAScript_Object Uint8Array;
//        Nh_ECMAScript_Object Uint8ArrayPrototype;
//        Nh_ECMAScript_Object Uint8ClampedArray;
//        Nh_ECMAScript_Object Uint8ClampedArrayPrototype;
//        Nh_ECMAScript_Object Uint16Array;
//        Nh_ECMAScript_Object Uint16ArrayPrototype;
//        Nh_ECMAScript_Object Uint32Array;
//        Nh_ECMAScript_Object Uint32ArrayPrototype;
//        Nh_ECMAScript_Object URIError;
//        Nh_ECMAScript_Object URIErrorPrototype;
//        Nh_ECMAScript_Object WeakMap;
//        Nh_ECMAScript_Object WeakMapPrototype;
//        Nh_ECMAScript_Object WeakSet;
//        Nh_ECMAScript_Object WeakSetPrototype;
    } Nh_ECMAScript_Intrinsics;

/** @} */

/** @addtogroup NhECMAScriptStructs
 *  @{
 */

    typedef struct Nh_ECMAScript_RealmHostDefined {
        NH_BYTE *temporary_p;
    } Nh_ECMAScript_RealmHostDefined;

    // https://tc39.es/ecma262/#sec-code-realms
    typedef struct Nh_ECMAScript_Realm {
        Nh_ECMAScript_Intrinsics Intrinsics;
        Nh_ECMAScript_Object *GlobalObject_p;
        Nh_ECMAScript_Environment *GlobalEnvironment_p;
        Nh_ECMAScript_RealmHostDefined HostDefined;
    } Nh_ECMAScript_Realm;

/** @} */

/** @addtogroup NhECMAScriptFunctions
 *  @{
 */

    NH_ECMASCRIPT_RESULT Nh_ECMAScript_initializeHostDefinedRealm(
        Nh_Stack *ExecutionContextStack_p
    );

/** @} */

#endif

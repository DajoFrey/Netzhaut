// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "Objects.h"
#include "Util.h"
#include "Libraries.h"
#include "Main.h"

#include "Common/Macro.h"

#include NH_INSTALLER_FLOW
#include NH_INSTALLER_CUSTOM_CHECK

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <pthread.h>
#include <dirent.h>

#define CREATE(in, out, extra) \
    NH_INSTALLER_CHECK((void*)NH_INSTALLER_ERROR_CANT_CREATE_OBJECT_FILE, Nh_Installer_createPICObjectFileUsingGCC(in, out, extra))


// NETZHAUT OBJECTS ===============================================================================

static void *Nh_Installer_createNetzhautObjects(
    void *extra_p)
{
NH_INSTALLER_BEGIN()

    CREATE("Netzhaut/Netzhaut.c", "../../lib/OBJECTS/NETZHAUT/Netzhaut.o", extra_p)
    CREATE("Netzhaut/NetzhautTTY.c", "../../lib/OBJECTS/NETZHAUT/NetzhautTTY.o", extra_p)

NH_INSTALLER_END((void*)NH_INSTALLER_SUCCESS)
}

// LOADER OBJECTS =================================================================================

static void *Nh_Installer_createNhLoaderObjects(
    void *extra_p)
{
NH_INSTALLER_BEGIN()

    CREATE("NhLoader/Loader.c", "../../lib/OBJECTS/LOADER/Loader.o", extra_p)
    CREATE("NhLoader/Library.c", "../../lib/OBJECTS/LOADER/Library.o", extra_p)
    CREATE("NhLoader/Reload.c", "../../lib/OBJECTS/LOADER/Reload.o", extra_p)
    CREATE("NhLoader/Repository.c", "../../lib/OBJECTS/LOADER/Repository.o", extra_p)
    CREATE("NhLoader/Common/Result.c", "../../lib/OBJECTS/LOADER/Result.o", extra_p)
    CREATE("NhLoader/Common/Log.c", "../../lib/OBJECTS/LOADER/Log.o", extra_p)

NH_INSTALLER_END((void*)NH_INSTALLER_SUCCESS)
}

// CORE OBJECTS ================================================================================

static void *Nh_Installer_createNhCoreObjects1(
    void *extra_p)
{
NH_INSTALLER_BEGIN()

    CREATE("NhCore/System.c", "../../lib/OBJECTS/CORE/System.o", extra_p)
    CREATE("NhCore/Window.c", "../../lib/OBJECTS/CORE/Window.o", extra_p)
    CREATE("NhCore/HashMap.c", "../../lib/OBJECTS/CORE/HashMap.o", extra_p)
    CREATE("NhCore/Sync.c", "../../lib/OBJECTS/CORE/Sync.o", extra_p)
    CREATE("NhCore/Font.c", "../../lib/OBJECTS/CORE/Font.o", extra_p)
    CREATE("NhCore/Tab.c", "../../lib/OBJECTS/CORE/Tab.o", extra_p)
    CREATE("NhCore/Time.c", "../../lib/OBJECTS/CORE/Time.o", extra_p)
    CREATE("NhCore/Config.c", "../../lib/OBJECTS/CORE/Config.o", extra_p)
    CREATE("NhCore/Callbacks.c", "../../lib/OBJECTS/CORE/Callbacks.o", extra_p)
    CREATE("NhCore/String.c", "../../lib/OBJECTS/CORE/String.o", extra_p)
    CREATE("NhCore/Thread.c", "../../lib/OBJECTS/CORE/Thread.o", extra_p)
    CREATE("NhCore/Channel.c", "../../lib/OBJECTS/CORE/Channel.o", extra_p)
    CREATE("NhCore/Logger.c", "../../lib/OBJECTS/CORE/Logger.o", extra_p)
    CREATE("NhCore/Memory.c", "../../lib/OBJECTS/CORE/Memory.o", extra_p)
    CREATE("NhCore/Unicode.c", "../../lib/OBJECTS/CORE/Unicode.o", extra_p)
    CREATE("NhCore/UnicodeLookup.c", "../../lib/OBJECTS/CORE/UnicodeLookup.o", extra_p)

NH_INSTALLER_END((void*)NH_INSTALLER_SUCCESS)
}

static void *Nh_Installer_createNhCoreObjects2(
    void *extra_p)
{
NH_INSTALLER_BEGIN()

    CREATE("NhCore/RingIterator.c", "../../lib/OBJECTS/CORE/RingIterator.o", extra_p)
    CREATE("NhCore/LinkedList.c", "../../lib/OBJECTS/CORE/LinkedList.o", extra_p)
    CREATE("NhCore/Math.c", "../../lib/OBJECTS/CORE/Math.o", extra_p)
    CREATE("NhCore/File.c", "../../lib/OBJECTS/CORE/File.o", extra_p)
    CREATE("NhCore/URI.c", "../../lib/OBJECTS/CORE/URI.o", extra_p)
    CREATE("NhCore/MediaType.c", "../../lib/OBJECTS/CORE/MediaType.o", extra_p)
    CREATE("NhCore/Content.c", "../../lib/OBJECTS/CORE/Content.o", extra_p)
    CREATE("NhCore/List.c", "../../lib/OBJECTS/CORE/List.o", extra_p)
    CREATE("NhCore/Texture.c", "../../lib/OBJECTS/CORE/Texture.o", extra_p)
    CREATE("NhCore/Array.c", "../../lib/OBJECTS/CORE/Array.o", extra_p)
    CREATE("NhCore/Process.c", "../../lib/OBJECTS/CORE/Process.o", extra_p)
    CREATE("NhCore/Stack.c", "../../lib/OBJECTS/CORE/Stack.o", extra_p)
    CREATE("NhCore/Common/Initialize.c", "../../lib/OBJECTS/CORE/Initialize.o", extra_p)
    CREATE("NhCore/Common/Terminate.c", "../../lib/OBJECTS/CORE/Terminate.o", extra_p)
    CREATE("NhCore/Common/Result.c", "../../lib/OBJECTS/CORE/Result.o", extra_p)
    CREATE("NhCore/Common/IndexMap.c", "../../lib/OBJECTS/CORE/IndexMap.o", extra_p)
    CREATE("NhCore/Common/Log.c", "../../lib/OBJECTS/CORE/Log.o", extra_p)

NH_INSTALLER_END((void*)NH_INSTALLER_SUCCESS)
}

// IO OBJECTS ======================================================================================

static void *Nh_Installer_createNhIOObjects(
    void *extra_p)
{
NH_INSTALLER_BEGIN()

    CREATE("NhIO/Window.c", "../../lib/OBJECTS/IO/Window.o", extra_p)
    CREATE("NhIO/Input.c", "../../lib/OBJECTS/IO/Input.o", extra_p)
    CREATE("NhIO/Mouse.c", "../../lib/OBJECTS/IO/Mouse.o", extra_p)
    CREATE("NhIO/Keyboard.c", "../../lib/OBJECTS/IO/Keyboard.o", extra_p)
    CREATE("NhIO/Win32.c", "../../lib/OBJECTS/IO/Win32.o", extra_p)
    CREATE("NhIO/X11.c", "../../lib/OBJECTS/IO/X11.o", extra_p)
    CREATE("NhIO/Common/Log.c", "../../lib/OBJECTS/IO/Log.o", extra_p)
    CREATE("NhIO/Common/Result.c", "../../lib/OBJECTS/IO/Result.o", extra_p)

NH_INSTALLER_END((void*)NH_INSTALLER_SUCCESS)
}

// HTML OBJECTS ====================================================================================

static void *Nh_Installer_createNhHTMLObjects(
    void *extra_p)
{
NH_INSTALLER_BEGIN()

    CREATE("NhHTML/Common/Result.c", "../../lib/OBJECTS/HTML/Result.o", extra_p)

NH_INSTALLER_END((void*)NH_INSTALLER_SUCCESS)
}

// TTY OBJECTS =====================================================================================

static void *Nh_Installer_createNhTTYObjects(
    void *extra_p)
{
NH_INSTALLER_BEGIN()

    CREATE("NhTTY/Terminal.c", "../../lib/OBJECTS/TTY/Terminal.o", extra_p)
    CREATE("NhTTY/Row.c", "../../lib/OBJECTS/TTY/Row.o", extra_p)
    CREATE("NhTTY/Draw.c", "../../lib/OBJECTS/TTY/Draw.o", extra_p)
    CREATE("NhTTY/TreeListing.c", "../../lib/OBJECTS/TTY/TreeListing.o", extra_p)
    CREATE("NhTTY/Program.c", "../../lib/OBJECTS/TTY/Program.o", extra_p)
    CREATE("NhTTY/Output.c", "../../lib/OBJECTS/TTY/Output.o", extra_p)
    CREATE("NhTTY/Tile.c", "../../lib/OBJECTS/TTY/Tile.o", extra_p)
    CREATE("NhTTY/Tab.c", "../../lib/OBJECTS/TTY/Tab.o", extra_p)
    CREATE("NhTTY/Shell.c", "../../lib/OBJECTS/TTY/Shell.o", extra_p)
    CREATE("NhTTY/Editor.c", "../../lib/OBJECTS/TTY/Editor.o", extra_p)
    CREATE("NhTTY/FileEditor.c", "../../lib/OBJECTS/TTY/FileEditor.o", extra_p)
    CREATE("NhTTY/TextFile.c", "../../lib/OBJECTS/TTY/TextFile.o", extra_p)
    CREATE("NhTTY/SyntaxHighlights.c", "../../lib/OBJECTS/TTY/SyntaxHighlights.o", extra_p)
    CREATE("NhTTY/Messages.c", "../../lib/OBJECTS/TTY/Messages.o", extra_p)
    CREATE("NhTTY/Console.c", "../../lib/OBJECTS/TTY/Console.o", extra_p)
    CREATE("NhTTY/Idle.c", "../../lib/OBJECTS/TTY/Idle.o", extra_p)
    CREATE("NhTTY/Logger.c", "../../lib/OBJECTS/TTY/Logger.o", extra_p)
    CREATE("NhTTY/Common/Result.c", "../../lib/OBJECTS/TTY/Result.o", extra_p)

NH_INSTALLER_END((void*)NH_INSTALLER_SUCCESS)
}

// NETWORK OBJECTS =================================================================================

static void *Nh_Installer_createNhNetworkObjects(
    void *extra_p)
{
NH_INSTALLER_BEGIN()

    CREATE("NhNetwork/URL.c", "../../lib/OBJECTS/NETWORK/URL.o", extra_p)
    CREATE("NhNetwork/Socket.c", "../../lib/OBJECTS/NETWORK/Socket.o", extra_p)
    CREATE("NhNetwork/HTTP.c", "../../lib/OBJECTS/NETWORK/HTTP.o", extra_p)
    CREATE("NhNetwork/SSL.c", "../../lib/OBJECTS/NETWORK/SSL.o", extra_p)
    CREATE("NhNetwork/Network.c", "../../lib/OBJECTS/NETWORK/Network.o", extra_p)
    CREATE("NhNetwork/Common/Log.c", "../../lib/OBJECTS/NETWORK/Log.o", extra_p)
    CREATE("NhNetwork/Common/Result.c", "../../lib/OBJECTS/NETWORK/Result.o", extra_p)

NH_INSTALLER_END((void*)NH_INSTALLER_SUCCESS)
}

// WEBIDL OBJECTS ==================================================================================

static void *Nh_Installer_createNhWebIDLObjects(
    void *extra_p)
{
NH_INSTALLER_BEGIN()

    CREATE("NhWebIDL/Runtime/Tokenizer.c", "../../lib/OBJECTS/WEBIDL/Tokenizer.o", extra_p)
    CREATE("NhWebIDL/Runtime/Parser.c", "../../lib/OBJECTS/WEBIDL/Parser.o", extra_p)
    CREATE("NhWebIDL/Runtime/Builtin.c", "../../lib/OBJECTS/WEBIDL/Builtin.o", extra_p)
    CREATE("NhWebIDL/Runtime/Fragment.c", "../../lib/OBJECTS/WEBIDL/Fragment.o", extra_p)
    CREATE("NhWebIDL/Common/Log.c", "../../lib/OBJECTS/WEBIDL/Log.o", extra_p)
    CREATE("NhWebIDL/Common/Initialize.c", "../../lib/OBJECTS/WEBIDL/Initialize.o", extra_p)
    CREATE("NhWebIDL/Common/Terminate.c", "../../lib/OBJECTS/WEBIDL/Terminate.o", extra_p)
    CREATE("NhWebIDL/Common/IndexMap.c", "../../lib/OBJECTS/WEBIDL/IndexMap.o", extra_p)
    CREATE("NhWebIDL/Common/Result.c", "../../lib/OBJECTS/WEBIDL/Result.o", extra_p)

NH_INSTALLER_END((void*)NH_INSTALLER_SUCCESS)
}

// ECMASCRIPT OBJECTS ==============================================================================

static void *Nh_Installer_createNhECMAScriptObjects1(
    void *extra_p)
{
NH_INSTALLER_BEGIN()

    CREATE("NhECMAScript/Engine/Lexer.c", "../../lib/OBJECTS/ECMASCRIPT/Lexer.o", extra_p)
    CREATE("NhECMAScript/Engine/Agent.c", "../../lib/OBJECTS/ECMASCRIPT/Agent.o", extra_p)
    CREATE("NhECMAScript/Engine/Script.c", "../../lib/OBJECTS/ECMASCRIPT/Script.o", extra_p)
    CREATE("NhECMAScript/Engine/Parser.c", "../../lib/OBJECTS/ECMASCRIPT/Parser.o", extra_p)
    CREATE("NhECMAScript/Engine/Realm.c", "../../lib/OBJECTS/ECMASCRIPT/Realm.o", extra_p)
    CREATE("NhECMAScript/Engine/InternalSlots.c", "../../lib/OBJECTS/ECMASCRIPT/InternalSlots.o", extra_p)
    CREATE("NhECMAScript/Engine/ObjectType.c", "../../lib/OBJECTS/ECMASCRIPT/ObjectType.o", extra_p)
    CREATE("NhECMAScript/Engine/Type.c", "../../lib/OBJECTS/ECMASCRIPT/Type.o", extra_p)
    CREATE("NhECMAScript/Engine/BuiltinFunctionObject.c", "../../lib/OBJECTS/ECMASCRIPT/BuiltinFunctionObject.o", extra_p)
    CREATE("NhECMAScript/Engine/Completion.c", "../../lib/OBJECTS/ECMASCRIPT/Completion.o", extra_p)
    CREATE("NhECMAScript/Engine/Properties.c", "../../lib/OBJECTS/ECMASCRIPT/Properties.o", extra_p)
    CREATE("NhECMAScript/Engine/Environment.c", "../../lib/OBJECTS/ECMASCRIPT/Environment.o", extra_p)
    CREATE("NhECMAScript/Engine/OrdinaryFunctionObject.c", "../../lib/OBJECTS/ECMASCRIPT/OrdinaryFunctionObject.o", extra_p)
    CREATE("NhECMAScript/Engine/Evaluation.c", "../../lib/OBJECTS/ECMASCRIPT/Evaluation.o", extra_p)
    CREATE("NhECMAScript/Engine/ExecutionContext.c", "../../lib/OBJECTS/ECMASCRIPT/ExecutionContext.o", extra_p)
    CREATE("NhECMAScript/Engine/CompletionOrReference.c", "../../lib/OBJECTS/ECMASCRIPT/CompletionOrReference.o", extra_p)

NH_INSTALLER_END((void*)NH_INSTALLER_SUCCESS)
}

static void *Nh_Installer_createNhECMAScriptObjects2(
    void *extra_p)
{
NH_INSTALLER_BEGIN()

    CREATE("NhECMAScript/Engine/Reference.c", "../../lib/OBJECTS/ECMASCRIPT/Reference.o", extra_p)
    CREATE("NhECMAScript/Engine/TestAndCompare.c", "../../lib/OBJECTS/ECMASCRIPT/TestAndCompare.o", extra_p)
    CREATE("NhECMAScript/Engine/PropertyDescriptor.c", "../../lib/OBJECTS/ECMASCRIPT/PropertyDescriptor.o", extra_p)
    CREATE("NhECMAScript/Engine/Instantiation.c", "../../lib/OBJECTS/ECMASCRIPT/Instantiation.o", extra_p)
    CREATE("NhECMAScript/Engine/OrdinaryObject.c", "../../lib/OBJECTS/ECMASCRIPT/OrdinaryObject.o", extra_p)
    CREATE("NhECMAScript/StaticSemantics/Misc.c", "../../lib/OBJECTS/ECMASCRIPT/Misc.o", extra_p)
    CREATE("NhECMAScript/StaticSemantics/DeclaredNames.c", "../../lib/OBJECTS/ECMASCRIPT/DeclaredNames.o", extra_p)
    CREATE("NhECMAScript/StaticSemantics/BoundNames.c", "../../lib/OBJECTS/ECMASCRIPT/BoundNames.o", extra_p)
    CREATE("NhECMAScript/StaticSemantics/ScopedDeclarations.c", "../../lib/OBJECTS/ECMASCRIPT/ScopedDeclarations.o", extra_p)
    CREATE("NhECMAScript/StaticSemantics/ParameterLists.c", "../../lib/OBJECTS/ECMASCRIPT/ParameterLists.o", extra_p)
    CREATE("NhECMAScript/Intrinsics/GlobalObject.c", "../../lib/OBJECTS/ECMASCRIPT/GlobalObject.o", extra_p)
    CREATE("NhECMAScript/Intrinsics/Object.c", "../../lib/OBJECTS/ECMASCRIPT/Object.o", extra_p)
    CREATE("NhECMAScript/Intrinsics/Function.c", "../../lib/OBJECTS/ECMASCRIPT/Function.o", extra_p)
    CREATE("NhECMAScript/Common/Initialize.c", "../../lib/OBJECTS/ECMASCRIPT/Initialize.o", extra_p)
    CREATE("NhECMAScript/Common/Terminate.c", "../../lib/OBJECTS/ECMASCRIPT/Terminate.o", extra_p)
    CREATE("NhECMAScript/Common/Log.c", "../../lib/OBJECTS/ECMASCRIPT/Log.o", extra_p)
    CREATE("NhECMAScript/Common/IndexMap.c", "../../lib/OBJECTS/ECMASCRIPT/IndexMap.o", extra_p)
    CREATE("NhECMAScript/Common/Result.c", "../../lib/OBJECTS/ECMASCRIPT/Result.o", extra_p)

NH_INSTALLER_END((void*)NH_INSTALLER_SUCCESS)
}

#undef CREATE

// OBJECTS/NETZHAUT ================================================================================

static NH_INSTALLER_RESULT Nh_Installer_createNetzhautObjectsDir(
    char *projDir_p, char *path_p)
{
NH_INSTALLER_BEGIN()

    chdir(projDir_p);
    NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_CANT_CREATE_BIN_OBJECT_DIRECTORY, Nh_Installer_createDir("lib/OBJECTS/NETZHAUT"))
    chdir(path_p);

    char extra_p[1024] = {'\0'};
    if (NH_INSTALLER_FLOW_LOGGING) {sprintf(extra_p, "-DNH_LOG_FLOW");}
    else {sprintf(extra_p, "");}

    NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_CANT_CREATE_OBJECTS, (NH_INSTALLER_RESULT)Nh_Installer_createNetzhautObjects(extra_p))

NH_INSTALLER_DIAGNOSTIC_END(NH_INSTALLER_SUCCESS)
}

// OBJECTS/LOADER ==================================================================================

static NH_INSTALLER_RESULT Nh_Installer_createNhLoaderObjectsDir(
    char *projDir_p, char *path_p)
{
NH_INSTALLER_BEGIN()

    chdir(projDir_p);
    NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_CANT_CREATE_BIN_OBJECT_DIRECTORY, Nh_Installer_createDir("lib/OBJECTS/LOADER"))
    chdir(path_p);

    char extra_p[1024] = {'\0'};
    if (NH_INSTALLER_FLOW_LOGGING) {sprintf(extra_p, "-DNH_LOG_FLOW");}
    else {sprintf(extra_p, "");}

    NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_CANT_CREATE_OBJECTS, (NH_INSTALLER_RESULT)Nh_Installer_createNhLoaderObjects(extra_p))

NH_INSTALLER_DIAGNOSTIC_END(NH_INSTALLER_SUCCESS)
}

// OBJECTS/CORE ====================================================================================

static NH_INSTALLER_RESULT Nh_Installer_createNhCoreObjectsDir(
    char *projDir_p, char *path_p)
{
NH_INSTALLER_BEGIN()

    chdir(projDir_p);
    NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_CANT_CREATE_BIN_OBJECT_DIRECTORY, (NH_INSTALLER_RESULT)Nh_Installer_createDir("lib/OBJECTS/CORE"))
    chdir(path_p);

#ifdef __unix__

    char extra_p[1024] = {'\0'};
    
    if (NH_INSTALLER_FLOW_LOGGING) {
        sprintf(extra_p, "-I/usr/include/freetype2 -DVK_VERSION_1_2 -DVK_USE_PLATFORM_XLIB_KHR -DVK_KHR_xlib_surface -DNH_LOG_FLOW");
    }
    else {
        sprintf(extra_p, "-I/usr/include/freetype2 -DVK_VERSION_1_2 -DVK_USE_PLATFORM_XLIB_KHR -DVK_KHR_xlib_surface");
    }

    pthread_t thread_p[2];
    void *status_p;

    NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_THREAD_CREATION_FAILED, pthread_create(&thread_p[0], NULL, Nh_Installer_createNhCoreObjects1, extra_p))
    NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_THREAD_CREATION_FAILED, pthread_create(&thread_p[1], NULL, Nh_Installer_createNhCoreObjects2, extra_p))

    pthread_join(thread_p[0], &status_p);
    NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_CANT_CREATE_OBJECTS, (NH_INSTALLER_RESULT)status_p)
    pthread_join(thread_p[1], &status_p);
    NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_CANT_CREATE_OBJECTS, (NH_INSTALLER_RESULT)status_p)

#elif defined(WIN_32)

    char *extra_p = "-I/usr/include/freetype2 -L/usr/local/lib -lfreetype -DVK_VERSION_1_2 -DVK_USE_PLATFORM_WIN32_KHR -DVK_KHR_win32_surface";

#endif

NH_INSTALLER_DIAGNOSTIC_END(NH_INSTALLER_SUCCESS)
}

// OBJECTS/IO ======================================================================================

static NH_INSTALLER_RESULT Nh_Installer_createNhIOObjectsDir(
    char *projDir_p, char *path_p)
{
NH_INSTALLER_BEGIN()

    chdir(projDir_p);
    NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_CANT_CREATE_BIN_OBJECT_DIRECTORY, Nh_Installer_createDir("lib/OBJECTS/IO"))
    chdir(path_p);

    char extra_p[1024] = {'\0'};
    if (NH_INSTALLER_FLOW_LOGGING) {sprintf(extra_p, "-DNH_LOG_FLOW");}
    else {sprintf(extra_p, "");}
   
    NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_CANT_CREATE_OBJECTS, (NH_INSTALLER_RESULT)Nh_Installer_createNhIOObjects(extra_p))

NH_INSTALLER_DIAGNOSTIC_END(NH_INSTALLER_SUCCESS)
}

// OBJECTS/TTY =====================================================================================

static NH_INSTALLER_RESULT Nh_Installer_createNhTTYObjectsDir(
    char *projDir_p, char *path_p)
{
NH_INSTALLER_BEGIN()

    chdir(projDir_p);
    NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_CANT_CREATE_BIN_OBJECT_DIRECTORY, Nh_Installer_createDir("lib/OBJECTS/TTY"))
    chdir(path_p);

    char extra_p[1024] = {'\0'};
    if (NH_INSTALLER_FLOW_LOGGING) {sprintf(extra_p, "-DNH_LOG_FLOW");}
    else {sprintf(extra_p, "");}
   
    NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_CANT_CREATE_OBJECTS, (NH_INSTALLER_RESULT)Nh_Installer_createNhTTYObjects(extra_p))

NH_INSTALLER_DIAGNOSTIC_END(NH_INSTALLER_SUCCESS)
}

// OBJECTS/HTML ====================================================================================

static NH_INSTALLER_RESULT Nh_Installer_createNhHTMLObjectsDir(
    char *projDir_p, char *path_p)
{
NH_INSTALLER_BEGIN()

    chdir(projDir_p);
    NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_CANT_CREATE_BIN_OBJECT_DIRECTORY, Nh_Installer_createDir("lib/OBJECTS/HTML"))
    chdir(path_p);

    char extra_p[1024] = {'\0'};
    if (NH_INSTALLER_FLOW_LOGGING) {sprintf(extra_p, "-DNH_LOG_FLOW");}
    else {sprintf(extra_p, "");}
   
    NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_CANT_CREATE_OBJECTS, (NH_INSTALLER_RESULT)Nh_Installer_createNhHTMLObjects(extra_p))

NH_INSTALLER_DIAGNOSTIC_END(NH_INSTALLER_SUCCESS)
}

// OBJECTS/NETWORK =================================================================================

static NH_INSTALLER_RESULT Nh_Installer_createNhNetworkObjectsDir(
    char *projDir_p, char *path_p)
{
NH_INSTALLER_BEGIN()

    chdir(projDir_p);
    NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_CANT_CREATE_BIN_OBJECT_DIRECTORY, Nh_Installer_createDir("lib/OBJECTS/NETWORK"))
    chdir(path_p);

    char extra_p[1024] = {'\0'};
    if (NH_INSTALLER_FLOW_LOGGING) {sprintf(extra_p, "-DNH_LOG_FLOW");}
    else {sprintf(extra_p, "");}
   
    NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_CANT_CREATE_OBJECTS, (NH_INSTALLER_RESULT)Nh_Installer_createNhNetworkObjects(extra_p))

NH_INSTALLER_DIAGNOSTIC_END(NH_INSTALLER_SUCCESS)
}

// OBJECTS/WEBIDL ==================================================================================

static NH_INSTALLER_RESULT Nh_Installer_createNhWebIDLObjectsDir(
    char *projDir_p, char *path_p)
{
NH_INSTALLER_BEGIN()

    chdir(projDir_p);
    NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_CANT_CREATE_BIN_OBJECT_DIRECTORY, Nh_Installer_createDir("lib/OBJECTS/WEBIDL"))
    chdir(path_p);

    char extra_p[1024] = {'\0'};
    if (NH_INSTALLER_FLOW_LOGGING) {sprintf(extra_p, "-DNH_LOG_FLOW");}
    else {sprintf(extra_p, "");}
   
    NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_CANT_CREATE_OBJECTS, (NH_INSTALLER_RESULT)Nh_Installer_createNhWebIDLObjects(extra_p))

NH_INSTALLER_DIAGNOSTIC_END(NH_INSTALLER_SUCCESS)
}

// OBJECTS/ECMASCRIPT ==========================================================================

static NH_INSTALLER_RESULT Nh_Installer_createNhECMAScriptObjectsDir(
    char *projDir_p, char *path_p)
{
NH_INSTALLER_BEGIN()

    chdir(projDir_p);
    NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_CANT_CREATE_BIN_OBJECT_DIRECTORY, Nh_Installer_createDir("lib/OBJECTS/ECMASCRIPT"))
    chdir(path_p);

#ifdef __unix__

    char extra_p[1024] = {'\0'};
    if (NH_INSTALLER_FLOW_LOGGING) {sprintf(extra_p, "-DNH_LOG_FLOW");}
    else {sprintf(extra_p, "");}
   
    pthread_t thread_p[2];
    void *status_p;

    NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_THREAD_CREATION_FAILED, pthread_create(&thread_p[0], NULL, Nh_Installer_createNhECMAScriptObjects1, extra_p))
    NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_THREAD_CREATION_FAILED, pthread_create(&thread_p[1], NULL, Nh_Installer_createNhECMAScriptObjects2, extra_p))

    pthread_join(thread_p[0], &status_p);
    NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_CANT_CREATE_OBJECTS, (NH_INSTALLER_RESULT)status_p)
    pthread_join(thread_p[1], &status_p);
    NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_CANT_CREATE_OBJECTS, (NH_INSTALLER_RESULT)status_p)

#elif defined(WIN_32)

#endif

NH_INSTALLER_DIAGNOSTIC_END(NH_INSTALLER_SUCCESS)
}

// CREATE ==========================================================================================

NH_INSTALLER_RESULT Nh_Installer_createObjectsDir(
    char *wrkDir_p, char *projDir_p, char *name_p) 
{
NH_INSTALLER_BEGIN()

    char path_p[2048] = {'\0'};
    sprintf(path_p, "%s%s", projDir_p, "/src/lib");

    if (!strcmp(name_p, "Netzhaut")) {
        NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_BUILD_LIBRARY_FAILED, Nh_Installer_createNetzhautObjectsDir(projDir_p, path_p))
    }
    else if (!strcmp(name_p, "NhLoader")) {
        NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_BUILD_LIBRARY_FAILED, Nh_Installer_createNhLoaderObjectsDir(projDir_p, path_p))
    }
    else if (!strcmp(name_p, "NhCore")) {
        NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_BUILD_LIBRARY_FAILED, Nh_Installer_createNhCoreObjectsDir(projDir_p, path_p))
    }
    else if (!strcmp(name_p, "NhIO")) {
        NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_BUILD_LIBRARY_FAILED, Nh_Installer_createNhIOObjectsDir(projDir_p, path_p))
    }
    else if (!strcmp(name_p, "NhTTY")) {
        NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_BUILD_LIBRARY_FAILED, Nh_Installer_createNhTTYObjectsDir(projDir_p, path_p))
    }
    else if (!strcmp(name_p, "NhNetwork")) {
        NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_BUILD_LIBRARY_FAILED, Nh_Installer_createNhNetworkObjectsDir(projDir_p, path_p))
    }
    else if (!strcmp(name_p, "NhWebIDL")) {
        NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_BUILD_LIBRARY_FAILED, Nh_Installer_createNhWebIDLObjectsDir(projDir_p, path_p))
    }
    else if (!strcmp(name_p, "NhECMAScript")) {
        NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_BUILD_LIBRARY_FAILED, Nh_Installer_createNhECMAScriptObjectsDir(projDir_p, path_p))
    }
    else if (!strcmp(name_p, "NhHTML")) {
        NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_BUILD_LIBRARY_FAILED, Nh_Installer_createNhHTMLObjectsDir(projDir_p, path_p))
    }

    chdir(wrkDir_p);

NH_INSTALLER_DIAGNOSTIC_END(NH_INSTALLER_SUCCESS)
}


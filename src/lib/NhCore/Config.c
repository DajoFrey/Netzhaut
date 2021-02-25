// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "Config.h"
#include "Memory.h"
#include "HashMap.h"
#include "String.h"

#include "Common/Macro.h"
#include NH_FLOW
#include NH_DEFAULT_CHECK

#include <string.h>

// DATA ============================================================================================

Nh_Config NH_CONFIG;

// INIT CONFIG =====================================================================================

static NH_RESULT Nh_initSettings()
{
NH_BEGIN()

    NH_CONFIG.Settings.gamma = 2.2f;
    NH_CONFIG.Settings.inputBufferSize = 255;
    NH_CONFIG.Settings.internalURNSchemeName_p = Nh_allocateBytes("nh");
    NH_CONFIG.Settings.startpage_p = NULL;

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

NH_RESULT Nh_initConfig()
{
NH_BEGIN()

    NH_INIT_LINKED_LIST(NH_CONFIG.InternalURNSyntax)
    NH_CHECK(Nh_createInternalURNSyntax())
    NH_CHECK(Nh_initSettings())

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

// FREE CONFIG =====================================================================================

void Nh_freeConfig()
{
NH_BEGIN()

    Nh_destroyLinkedList(&NH_CONFIG.InternalURNSyntax, true);
    Nh_free(NH_CONFIG.Settings.internalURNSchemeName_p);
    Nh_free(NH_CONFIG.Settings.startpage_p);

NH_SILENT_END()
}

// GET =============================================================================================

NH_BOOL Nh_isEnabled(
    char *name_p)
{
NH_BEGIN()

    if (!strcmp(name_p, "CSS")) {NH_END(NH_CONFIG.Flags.Enabled.CSS)}
    if (!strcmp(name_p, "JavaScript")) {NH_END(NH_CONFIG.Flags.Enabled.JavaScript)}

NH_END(NH_FALSE)
}

// SET =============================================================================================

void Nh_setLogFlag(
    char *name_p, NH_BOOL value)
{
NH_BEGIN()

         if (!strcmp(name_p, "init"))          {NH_CONFIG.Flags.Log.init = value;}
    else if (!strcmp(name_p, "mouse"))         {NH_CONFIG.Flags.Log.mouse = value;}
    else if (!strcmp(name_p, "vulkan"))        {NH_CONFIG.Flags.Log.vulkan = value;}
    else if (!strcmp(name_p, "script"))        {NH_CONFIG.Flags.Log.script = value;}
    else if (!strcmp(name_p, "listener"))      {NH_CONFIG.Flags.Log.listener = value;}
    else if (!strcmp(name_p, "event"))         {NH_CONFIG.Flags.Log.event = value;}
    else if (!strcmp(name_p, "jsinterpreter")) {NH_CONFIG.Flags.Log.jsinterpreter = value;}
    else if (!strcmp(name_p, "jsparser"))      {NH_CONFIG.Flags.Log.jsparser = value;}
    else if (!strcmp(name_p, "sheet"))         {NH_CONFIG.Flags.Log.sheet = value;}
    else if (!strcmp(name_p, "cssparser"))     {NH_CONFIG.Flags.Log.cssparser = value;}
    else if (!strcmp(name_p, "prehtml"))       {NH_CONFIG.Flags.Log.prehtml = value;}
    else if (!strcmp(name_p, "posthtml"))      {NH_CONFIG.Flags.Log.posthtml = value;}
    else if (!strcmp(name_p, "receive"))       {NH_CONFIG.Flags.Log.receive = value;}
    else if (!strcmp(name_p, "send"))          {NH_CONFIG.Flags.Log.send = value;}
    else if (!strcmp(name_p, "window"))        {NH_CONFIG.Flags.Log.window = value;}
    else if (!strcmp(name_p, "hoist"))         {NH_CONFIG.Flags.Log.hoist = value;}
    else if (!strcmp(name_p, "global"))        {NH_CONFIG.Flags.Log.global = value;}

NH_SILENT_END()
}

void Nh_setLogFlowFlag(
    char *name_p, NH_BOOL value)
{
NH_BEGIN()

         if (!strcmp(name_p, "core")) {NH_CONFIG.Flags.Log.Flow.core = value;}
    else if (!strcmp(name_p, "gfx"))  {NH_CONFIG.Flags.Log.Flow.gfx  = value;}
    else if (!strcmp(name_p, "html")) {NH_CONFIG.Flags.Log.Flow.html = value;}
    else if (!strcmp(name_p, "css"))  {NH_CONFIG.Flags.Log.Flow.css = value;}
    else if (!strcmp(name_p, "net"))  {NH_CONFIG.Flags.Log.Flow.net = value;}
    else if (!strcmp(name_p, "io"))   {NH_CONFIG.Flags.Log.Flow.io = value;}
    else if (!strcmp(name_p, "js"))   {NH_CONFIG.Flags.Log.Flow.js = value;}

NH_SILENT_END()
}

void Nh_setEnabledFlag(
    char *name_p, NH_BOOL value)
{
NH_BEGIN()

         if (!strcmp(name_p, "js"))  {NH_CONFIG.Flags.Enabled.JavaScript = value;}
    else if (!strcmp(name_p, "css")) {NH_CONFIG.Flags.Enabled.CSS = value;}

NH_SILENT_END()
}

// FLAGS ===========================================================================================

void Nh_initLogFlags()
{
NH_BEGIN()

    NH_CONFIG.Flags.Log.init          = NH_FALSE; 
    NH_CONFIG.Flags.Log.mouse         = NH_FALSE; 
    NH_CONFIG.Flags.Log.vulkan        = NH_FALSE; 
    NH_CONFIG.Flags.Log.script        = NH_FALSE; 
    NH_CONFIG.Flags.Log.listener      = NH_FALSE; 
    NH_CONFIG.Flags.Log.event         = NH_FALSE; 
    NH_CONFIG.Flags.Log.jsinterpreter = NH_FALSE; 
    NH_CONFIG.Flags.Log.jsparser      = NH_FALSE; 
    NH_CONFIG.Flags.Log.sheet         = NH_FALSE; 
    NH_CONFIG.Flags.Log.cssparser     = NH_FALSE; 
    NH_CONFIG.Flags.Log.prehtml       = NH_FALSE; 
    NH_CONFIG.Flags.Log.posthtml      = NH_FALSE; 
    NH_CONFIG.Flags.Log.receive       = NH_FALSE; 
    NH_CONFIG.Flags.Log.send          = NH_FALSE; 
    NH_CONFIG.Flags.Log.window        = NH_FALSE;
    NH_CONFIG.Flags.Log.hoist         = NH_FALSE;
    NH_CONFIG.Flags.Log.global        = NH_FALSE;

    NH_CONFIG.Flags.Log.Flow.core = NH_FALSE; 
    NH_CONFIG.Flags.Log.Flow.gfx  = NH_FALSE; 
    NH_CONFIG.Flags.Log.Flow.html = NH_FALSE; 
    NH_CONFIG.Flags.Log.Flow.css  = NH_FALSE; 
    NH_CONFIG.Flags.Log.Flow.net  = NH_FALSE; 
    NH_CONFIG.Flags.Log.Flow.io   = NH_FALSE; 
    NH_CONFIG.Flags.Log.Flow.js   = NH_FALSE; 

NH_SILENT_END()
}

static void Nh_initInternalFlags()
{
NH_BEGIN()

    NH_CONFIG.Flags.shutdown = NH_FALSE;
    NH_CONFIG.Flags.headless = NH_FALSE;

NH_SILENT_END()
}

static void Nh_initEnabledFlags()
{
NH_BEGIN()

    NH_CONFIG.Flags.Enabled.JavaScript = NH_TRUE;
    NH_CONFIG.Flags.Enabled.CSS = NH_TRUE;

NH_SILENT_END()
}

NH_RESULT Nh_initFlags()
{
NH_BEGIN()

    Nh_initInternalFlags();
    Nh_initLogFlags();
    Nh_initEnabledFlags();

NH_DIAGNOSTIC_END(NH_SUCCESS)
}


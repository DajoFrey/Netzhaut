#ifndef NH_CONFIG_H
#define NH_CONFIG_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "System.h"
#include "Thread.h"
#include "Font.h"
#include "URI.h"
#include "LinkedList.h"

#include "Common/API.h"

#include <stddef.h>

#endif

/** @addtogroup NhCoreStructs
 *  @{
 */

    typedef struct Nh_Config {

        Nh_LinkedList InternalURNSyntax;

        struct Flags {

            NH_BOOL shutdown;
            NH_BOOL headless;
            NH_BOOL update;

            struct Log {
                NH_BOOL flow;
                NH_BOOL init;
                NH_BOOL mouse;
                NH_BOOL vulkan;
                NH_BOOL script;
                NH_BOOL listener;
                NH_BOOL event;
                NH_BOOL jsinterpreter;
                NH_BOOL jsparser;
                NH_BOOL sheet;
                NH_BOOL cssparser;
                NH_BOOL prehtml;
                NH_BOOL posthtml;
                NH_BOOL window;
                NH_BOOL receive;
                NH_BOOL send;
                NH_BOOL hoist;
                NH_BOOL global;
                struct Flow {
                    NH_BOOL core;
                    NH_BOOL gfx;
                    NH_BOOL html;
                    NH_BOOL css;
                    NH_BOOL net;
                    NH_BOOL io;
                    NH_BOOL js;
                } Flow;
            } Log;

            struct Enabled {
                NH_BOOL JavaScript;
                NH_BOOL CSS;
            } Enabled;

        } Flags;

        struct Settings {
            double gamma;
            int inputBufferSize;
            char *internalURNSchemeName_p;
            char *startpage_p;
        } Settings;

    } Nh_Config;

/** @} */

/** @addtogroup NhCoreVars
 *  @{
 */

    extern Nh_Config NH_CONFIG;

/** @} */

/** @addtogroup NhCoreFunctions
 *  @{
 */

    NH_RESULT Nh_initConfig(
    );
    void Nh_freeConfig(
    );
    
    void Nh_setLogFlag(
        char *name_p, NH_BOOL value
    );
    void Nh_setLogFlowFlag(
        char *name_p, NH_BOOL value
    );
    void Nh_setEnabledFlag(
        char *name_p, NH_BOOL value
    );

    void Nh_initLogFlags(
    );
    NH_RESULT Nh_initFlags(
    );

    NH_BOOL Nh_isEnabled(
        char *name_p
    );

/** @} */

#endif

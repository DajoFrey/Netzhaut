#ifndef NH_HTML_BROWSING_CONTEXTS_H
#define NH_HTML_BROWSING_CONTEXTS_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "../Common/API.h"
#include "../Common/Result.h"

#endif

/** @addtogroup NhHTMLEnums
 *  @{
 */

    typedef enum NH_HTML_CROSS_ORIGIN_ISOLATION_MODE {
        NH_HTML_CROSS_ORIGIN_ISOLATION_MODE_NONE,
        NH_HTML_CROSS_ORIGIN_ISOLATION_MODE_LOGICAL,
        NH_HTML_CROSS_ORIGIN_ISOLATION_MODE_CONCRETE,
    } NH_HTML_CROSS_ORIGIN_ISOLATION_MODE;

/** @} */

/** @addtogroup NhHTMLStructs
 *  @{
 */

    typedef struct Nh_HTML_BrowsingContext {
        Nh_HTML_Origin *Origin_p;
        Nh_HTML_URL *CreatorURL_p;
        Nh_HTML_URL *CreatorBaseURL_p;
    } Nh_HTML_BrowsingContext;

    typedef struct Nh_HTML_BrowsingContextGroup {
        Nh_List BrowsingContexts;
        NH_HTML_CROSS_ORIGIN_ISOLATION_MODE crossOriginIsolationMode;
    } Nh_HTML_BrowsingContextGroup;

/** @} */

/** @addtogroup NhHTMLVars
 *  @{
 */

    extern Nh_List NH_HTML_BROWSING_CONTEXT_GROUPS;

/** @} */

/** @addtogroup NhHTMLFunctions
 *  @{
 */

/** @} */

#endif

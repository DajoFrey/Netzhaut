#ifndef NH_GFX_MACROS_H
#define NH_GFX_MACROS_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#ifdef NH_LOG_FLOW
    #include "Log.h"
#endif

#include "../../../Netzhaut/Macros.h"
#include "../../../CSS/Header/Macros.h"

#undef NH_DEFAULT_CHECK
#undef NH_CUSTOM_CHECK

#define NH_DEFAULT_CHECK "../../../Netzhaut/MACROS/DEFAULT_CHECK.h"
#define NH_CUSTOM_CHECK "../../../Netzhaut/MACROS/CUSTOM_CHECK.h"

#undef NH_FLOW
#define NH_FLOW "../../../Graphics/Main/Header/MACROS/FLOW.h"

#undef NH_CSS_UTILS
#define NH_CSS_UTILS "../../../CSS/Header/MACROS/UTILS.h"

#undef NH_HTML_UTILS
#define NH_HTML_UTILS "../../../HTML/Main/Header/MACROS/UTILS.h"

#endif

/** \defgroup Graphics 
 *  \brief Graphics module.
 */

/** \defgroup Vulkan 
 *  \brief Vulkan module.
 *  \ingroup Graphics
 */

#endif 

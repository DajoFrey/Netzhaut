/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#undef NH_BEGIN
#undef NH_END
#undef NH_SILENT_END
#undef NH_DIAGNOSTIC_END

#ifdef NH_LOG_FLOW
    #define NH_BEGIN() {Nh_Gfx_logBegin(__FILE__, __func__);}
#else
    #define NH_BEGIN() 
#endif

#ifdef NH_LOG_FLOW
    #define NH_END(result) {Nh_Gfx_logEnd(__FILE__, __func__); return result;}
#else
    #define NH_END(result) {return result;} 
#endif

#ifdef NH_LOG_FLOW
    #define NH_SILENT_END() {Nh_Gfx_logEnd(__FILE__, __func__); return;}
#else
    #define NH_SILENT_END() {return;} 
#endif

#ifdef NH_LOG_FLOW
    #define NH_DIAGNOSTIC_END(result) {return Nh_Gfx_logDiagnosticEnd(__FILE__, __func__, result, __LINE__);}
#else
    #define NH_DIAGNOSTIC_END(result) {return result;} 
#endif


/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#undef NH_WEB_BEGIN
#undef NH_WEB_END
#undef NH_WEB_SILENT_END
#undef NH_WEB_DIAGNOSTIC_END

#ifdef NH_LOG_FLOW
    #define NH_WEB_BEGIN() {_Nh_Web_logBegin(__FILE__, __func__);}
#else
    #define NH_WEB_BEGIN() 
#endif

#ifdef NH_LOG_FLOW
    #define NH_WEB_END(result) {_Nh_Web_logEnd(__FILE__, __func__); return result;}
#else
    #define NH_WEB_END(result) {return result;} 
#endif

#ifdef NH_LOG_FLOW
    #define NH_WEB_SILENT_END() {_Nh_Web_logEnd(__FILE__, __func__); return;}
#else
    #define NH_WEB_SILENT_END() {return;} 
#endif

#ifdef NH_LOG_FLOW
    #define NH_WEB_DIAGNOSTIC_END(result) {return _Nh_Web_logDiagnosticEnd(__FILE__, __func__, result, __LINE__);}
#else
    #define NH_WEB_DIAGNOSTIC_END(result) {return result;} 
#endif


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
    #define NH_BEGIN() {_Nh_begin(__FILE__, __func__);}
#else
    #define NH_BEGIN() 
#endif

#ifdef NH_LOG_FLOW
    #define NH_END(result) {_Nh_logEnd(__FILE__, __func__); return result;}
#else
    #define NH_END(result) {return result;} 
#endif

#ifdef NH_LOG_FLOW
    #define NH_SILENT_END() {_Nh_logEnd(__FILE__, __func__); return;}
#else
    #define NH_SILENT_END() {return;} 
#endif

#ifdef NH_LOG_FLOW
    #define NH_DIAGNOSTIC_END(result) {return _Nh_diagnosticEnd(__FILE__, __func__, result, __LINE__);}
#else
    #define NH_DIAGNOSTIC_END(result) {return result;} 
#endif


/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#undef NH_WEBIDL_SERIALIZER_BEGIN
#undef NH_WEBIDL_SERIALIZER_END
#undef NH_WEBIDL_SERIALIZER_SILENT_END
#undef NH_WEBIDL_SERIALIZER_DIAGNOSTIC_END

#ifdef NH_LOG_FLOW
    #define NH_WEBIDL_SERIALIZER_BEGIN() {_Nh_WebIDLSerializer_logBegin(__FILE__, __func__);}
#else
    #define NH_WEBIDL_SERIALIZER_BEGIN() 
#endif

#ifdef NH_LOG_FLOW
    #define NH_WEBIDL_SERIALIZER_END(result) {_Nh_WebIDLSerializer_logEnd(__FILE__, __func__); return result;}
#else
    #define NH_WEBIDL_SERIALIZER_END(result) {return result;} 
#endif

#ifdef NH_LOG_FLOW
    #define NH_WEBIDL_SERIALIZER_SILENT_END() {_Nh_WebIDLSerializer_logEnd(__FILE__, __func__); return;}
#else
    #define NH_WEBIDL_SERIALIZER_SILENT_END() {return;} 
#endif

#ifdef NH_LOG_FLOW
    #define NH_WEBIDL_SERIALIZER_DIAGNOSTIC_END(result) {return _Nh_WebIDLSerializer_logDiagnosticEnd(__FILE__, __func__, result, __LINE__);}
#else
    #define NH_WEBIDL_SERIALIZER_DIAGNOSTIC_END(result) {return result;} 
#endif


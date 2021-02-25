/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#undef NH_INSTALLER_BEGIN
#undef NH_INSTALLER_END
#undef NH_INSTALLER_SILENT_END
#undef NH_INSTALLER_DIAGNOSTIC_END

#ifdef NH_LOG_FLOW
    #define NH_INSTALLER_BEGIN()
#else
    #define NH_INSTALLER_BEGIN() 
#endif

#ifdef NH_LOG_FLOW
    #define NH_INSTALLER_END(result) {return result;}
#else
    #define NH_INSTALLER_END(result) {return result;} 
#endif

#ifdef NH_LOG_FLOW
    #define NH_INSTALLER_SILENT_END() {return;}
#else
    #define NH_INSTALLER_SILENT_END() {return;} 
#endif

#ifdef NH_LOG_FLOW
    #define NH_INSTALLER_DIAGNOSTIC_END(result) {return result;}
#else
    #define NH_INSTALLER_DIAGNOSTIC_END(result) {return result;} 
#endif


/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#undef NH_TTY_BEGIN
#undef NH_TTY_END
#undef NH_TTY_SILENT_END
#undef NH_TTY_DIAGNOSTIC_END

#ifdef NH_LOG_FLOW
    #define NH_TTY_BEGIN() {}
#else
    #define NH_TTY_BEGIN() 
#endif

#ifdef NH_LOG_FLOW
    #define NH_TTY_END(result) {return result;}
#else
    #define NH_TTY_END(result) {return result;} 
#endif

#ifdef NH_LOG_FLOW
    #define NH_TTY_SILENT_END() {return;}
#else
    #define NH_TTY_SILENT_END() {return;} 
#endif

#ifdef NH_LOG_FLOW
    #define NH_TTY_DIAGNOSTIC_END(result) {return result;}
#else
    #define NH_TTY_DIAGNOSTIC_END(result) {return result;} 
#endif


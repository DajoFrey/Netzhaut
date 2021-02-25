// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE ========================================================================================

#include "../Header/Helper.h"
#include "../Header/Macros.h"

#include "../../Netzhaut/Tab.h"
#include "../../Netzhaut/Config.h"

#include NH_FLOW

#include <string.h>
#include <ctype.h>
#include <math.h>

// LENGTH ==========================================================================================

// https://developer.mozilla.org/en-US/docs/W../CSS/length
NH_PIXEL Nh_CSS_getLengthInPixel(
    Nh_HTML_Node *Node_p, Nh_Content *Content_p, char *str_p)
{
NH_BEGIN()

    // absolute lengths
    if (strstr(str_p, "cm")) 
    {
        float cm = strtof(str_p, NULL); // TODO might be problematic https:/*/stackoverflow.com/*questions/*7951019/*how-to-convert-string-to-float
        NH_END((NH_PIXEL)(cm * (96.0f / 2.54)))
    } 
    else if (strstr(str_p, "mm")) 
    {
        float mm = strtof(str_p, NULL);
        NH_END((NH_PIXEL)(mm * ((96.0f / 2.56) / 10.0f)))
    }
    else if (strstr(str_p, "in")) 
    {
        float in = strtof(str_p, NULL);
        NH_END((NH_PIXEL)(in * 96.0f))
    }
    else if (strstr(str_p, "px")) 
    {
        float px = strtof(str_p, NULL);
        NH_END((NH_PIXEL)px)
    }
    else if (strstr(str_p, "pt")) 
    {
        float pt = strtof(str_p, NULL);
        NH_END((NH_PIXEL)(pt * ((1.0f / 72.0f) * 96.0f)))
    }
    else if (strstr(str_p, "pc")) 
    {
        float pc = strtof(str_p, NULL);
        NH_END((NH_PIXEL)(pc * ((1.0f / 6.0f) * 96.0f)))
    }

    // font-relative lengths
    else if (strstr(str_p, "em")) 
    {
        float em = strtof(str_p, NULL);
        NH_END(Node_p != NULL ? (NH_PIXEL)(em * (float)Node_p->Properties.Text.fontSize) : -1)
    }
    else if (strstr(str_p, "ex")) 
    {
    }
    else if (strstr(str_p, "ch")) 
    {
    }
    else if (strstr(str_p, "rem")) 
    {
    }

    // viewport-percentage lengths
    else if (strstr(str_p, "vh")) 
    {
        float vh = strtof(str_p, NULL);
        NH_END(Content_p != NULL ?(NH_PIXEL)((vh/100.0f) * ((float)Content_p->Tab_p->Size.height)) : -1)
    }
    else if (strstr(str_p, "vw")) 
    {
        float vw = strtof(str_p, NULL);
        NH_END(Content_p != NULL ?(NH_PIXEL)((vw/100.0f) * ((float)Content_p->Tab_p->Size.width)) : -1)
    }
    else if (strstr(str_p, "vi")) 
    {
    }
    else if (strstr(str_p, "vb")) 
    {
    }
    else if (strstr(str_p, "vmin") || strstr(str_p, "vmax")) 
    {
        float vh = strtof(str_p, NULL);
        NH_PIXEL h = Content_p != NULL ?(NH_PIXEL)((vh/100.0f) * ((float)Content_p->Tab_p->Size.height)) : -1;
        float vw = strtof(str_p, NULL);
        NH_PIXEL w = Content_p != NULL ?(NH_PIXEL)((vw/100.0f) * ((float)Content_p->Tab_p->Size.width)) : -1;
        
        if (strstr(str_p, "vmin")) {NH_END(w < h ? w : h)}
        else {NH_END(w > h ? w : h)}
    }

// can't handle

NH_END(-1)
}

// STRIP ===========================================================================================

void Nh_CSS_strip( // TODO improve robustness
    NH_CSS_STRIP type, char *in_p, char *out_p)
{
NH_BEGIN()

    switch (type)
    {
        case NH_CSS_STRIP_URL :
        {
            in_p = in_p + 4; // jump over 'url('
            while (in_p[0] == ' ') {in_p = in_p + 1;} // jump over white spaces
            
            strcpy(out_p, in_p);
   
            // clip at ')'
            int end = 0;
            for (int i = 0; i < strlen(out_p); ++i) {
                if (out_p[i] == ')') {end = i; break;}
            }
            out_p[end] = '\0';
 
            break;
        }
    }

NH_SILENT_END()
}


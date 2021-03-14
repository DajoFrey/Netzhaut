// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "X11.h"
#include "Mouse.h"
#include "Keyboard.h"

#include "Common/Log.h"
#include "Common/Macro.h"
#include NH_WSI_DEFAULT_CHECK
#include NH_WSI_FLOW

#include <fcntl.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

#ifdef __unix__
    #include <X11/Xcursor/Xcursor.h>
    #include <X11/Xutil.h>
#endif

// DECLARE =========================================================================================

#ifdef __unix__

    typedef struct Property {
        unsigned char *data;
        int format, nitems;
        Atom type;
    } Property;
    
#endif

// HELPER ==========================================================================================

#ifdef __unix__

    static Property Nh_X11_readProperty(
        Display* Display_p, Window Window, Atom property)
    {
        Atom actual_type;
        int actual_format;
        int read_bytes = 1024;	
        unsigned long nitems;
        unsigned long bytes_after;
        unsigned char *ret = 0;	
    
        do {
            if(ret != 0) {
                XFree(ret);
            }
            XGetWindowProperty(
                Display_p, Window, property, 0, read_bytes, False, AnyPropertyType,
    	    &actual_type, &actual_format, &nitems, &bytes_after, &ret
            );
    	read_bytes *= 2;
        }
        while(bytes_after != 0);
        
        Property p = {ret, actual_format, nitems, actual_type};
        
        return p;
    }

    // https://stackoverflow.com/questions/3909713/xlib-xgetwindowattributes-always-returns-1x1
    static Window Nh_X11_getTopLevelParent(
        Display * display, Window window)
    {
         Window parent;
         Window root;
         Window * children;
         unsigned int num_children;
    
         while (1) {
             if (0 == XQueryTree(display, window, &root,
                       &parent, &children, &num_children)) {
                 fprintf(stderr, "XQueryTree error\n");
                 abort(); //change to whatever error handling you prefer
             }
             if (children) { //must test for null
                 XFree(children);
             }
             if (window == root || parent == root) {
                 return window;
             }
             else {
                 window = parent;
             }
         }
    }

#endif

// GET INPUT =======================================================================================

NH_WSI_RESULT Nh_X11_getInput(
    Nh_X11_Window *Window_p, Nh_WSI_Input *Input_p) 
{
NH_WSI_BEGIN()

#ifdef __unix__
    
    XEvent Event;
    Display* Display_p = Window_p->Display_p;

    Atom XdndEnter      = XInternAtom(Display_p, "XdndEnter", False);
    Atom XdndPosition   = XInternAtom(Display_p, "XdndPosition", False);
    Atom XdndStatus     = XInternAtom(Display_p, "XdndStatus", False);
    Atom XdndTypeList   = XInternAtom(Display_p, "XdndTypeList", False);
    Atom XdndActionCopy = XInternAtom(Display_p, "XdndActionCopy", False);
    Atom XdndDrop       = XInternAtom(Display_p, "XdndDrop", False);
    Atom XdndLeave      = XInternAtom(Display_p, "XdndLeave", False);
    Atom XdndFinished   = XInternAtom(Display_p, "XdndFinished", False);
    Atom XdndSelection  = XInternAtom(Display_p, "XdndSelection", False);
    Atom XdndProxy      = XInternAtom(Display_p, "XdndProxy", False);
    Atom sel            = XInternAtom(Display_p, "PRIMARY", 0);

    int xdnd = 0;

    while (XPending(Display_p) > 0) 
    {
        XNextEvent(Display_p, &Event);
        
        if (Event.type == ClientMessage)
        {   
                 if (Event.xclient.data.l[0] == Window_p->DeleteAtom) {exit(0);} // TODO event
            else if (Event.type == Expose) {}
            else if (Event.xclient.message_type == XdndEnter) 
            {
                xdnd = (Event.xclient.data.l[1] >> 24);
            }
            else if (Event.xclient.message_type == XdndDrop)
            {
                Atom string = XInternAtom(Display_p, "STRING", 0);
		
                if (xdnd >= 1) {
		    XConvertSelection(Display_p, XdndSelection, string, sel, Window_p->XWindow, Event.xclient.data.l[2]);
                } else {
                    XConvertSelection(Display_p, XdndSelection, string, sel, Window_p->XWindow, CurrentTime);
                }
            }
            else if (Event.xclient.message_type == XdndPosition)
            {
		Atom action = XdndActionCopy;
		if(xdnd >= 2) {action = Event.xclient.data.l[4];}
		
		XClientMessageEvent m;
		memset(&m, sizeof(m), 0);
		m.type         = ClientMessage;
		m.display      = Event.xclient.display;
		m.window       = Event.xclient.data.l[0];
		m.message_type = XdndStatus;
		m.format       = 32;
		
                m.data.l[0] = Window_p->XWindow;
		m.data.l[1] = 1;
		m.data.l[2] = 0; //Specify an empty rectangle
		m.data.l[3] = 0;
		m.data.l[4] = XdndActionCopy; //We only accept copying anyway.

		XSendEvent(Display_p, Event.xclient.data.l[0], False, NoEventMask, (XEvent*)&m);
                XFlush(Display_p);
            }
            else if (Event.xclient.message_type == XdndLeave) {}
        }

        else if (Event.type == ConfigureNotify) 
        {
            XConfigureEvent Configure = Event.xconfigure;
            Nh_WSI_ConfigureEvent *Event_p = Nh_advanceRingIterator(&Input_p->Configure.Events);

            XWindowAttributes XWA;
            XGetWindowAttributes(Display_p, Nh_X11_getTopLevelParent(Display_p, Window_p->XWindow), &XWA);

            Event_p->Size.width  = Configure.width;
            Event_p->Size.height = Configure.height;
            Event_p->Position.x  = XWA.x;
            Event_p->Position.y  = XWA.y;

            Nh_WSI_logWindow(Event_p);
        }

        else if (Event.type == MotionNotify)
        {
            Nh_WSI_MouseEvent *Event_p = Nh_advanceRingIterator(&Input_p->Mouse.Events);
           
            Event_p->Position.x = Event.xbutton.x;
            Event_p->Position.y = Event.xbutton.y;
            Event_p->trigger    = NH_WSI_TRIGGER_MOVE;
            Event_p->type       = NH_WSI_MOUSE_MOVE;

            Nh_WSI_logMouse(Event_p);
        }
        
        else if (Event.type == ButtonPress || Event.type == ButtonRelease)
        {
            Nh_WSI_MouseEvent *Event_p = Nh_advanceRingIterator(&Input_p->Mouse.Events);

            NH_WSI_TRIGGER trigger = Event.type == ButtonPress ? NH_WSI_TRIGGER_PRESS : NH_WSI_TRIGGER_RELEASE;
            NH_WSI_MOUSE button = 0;

            switch (Event.xbutton.button)
            {
                case Button1 : button = NH_WSI_MOUSE_LEFT; break;
                case Button2 : button = NH_WSI_MOUSE_MIDDLE; break;
                case Button3 : button = NH_WSI_MOUSE_RIGHT; break;
                case Button4 : button = NH_WSI_MOUSE_SCROLL; trigger = NH_WSI_TRIGGER_UP; break;
                case Button5 : button = NH_WSI_MOUSE_SCROLL; trigger = NH_WSI_TRIGGER_DOWN; break;
            }

            Event_p->Position.x = Event.xbutton.x;
            Event_p->Position.y = Event.xbutton.y;
            Event_p->trigger    = trigger;
            Event_p->type       = button;

            Nh_WSI_logMouse(Event_p);
        }
        
        else if (Event.type == KeyPress || Event.type == KeyRelease)
        {
            Nh_WSI_KeyboardEvent *Event_p = Nh_advanceRingIterator(&Input_p->Keyboard.Events);

            NH_WSI_TRIGGER event = Event.type == KeyPress ? NH_WSI_TRIGGER_PRESS : NH_WSI_TRIGGER_RELEASE;
            NH_WSI_KEY key = 0;

            // get key
            switch (XLookupKeysym(&Event.xkey, 0))
            {
                // TTY function keys
                case XKB_KEY_BackSpace    : break;
                case XKB_KEY_Tab          : break;
                case XKB_KEY_Linefeed     : break;
                case XKB_KEY_Clear        : break;
                case XKB_KEY_Return       : break;
                case XKB_KEY_Pause        : break; 
                case XKB_KEY_Scroll_Lock  : break;
                case XKB_KEY_Sys_Req      : break;
                case XKB_KEY_Escape       : break;
                case XKB_KEY_Delete       : break;

                // Cursor control & motion    
                case XKB_KEY_Home         : break;
                case XKB_KEY_Left         : break;      
                case XKB_KEY_Up           : break;               
                case XKB_KEY_Right        : break;
                case XKB_KEY_Down         : break;
                case XKB_KEY_Prior        : break; // same as XKB_KEY_Page_Up
                case XKB_KEY_Next         : break; // same as XKB_KEY_Page_Down
                case XKB_KEY_End          : break;
                case XKB_KEY_Begin        : break;

                // Modifiers
                case XKB_KEY_Shift_L      : key = NH_WSI_KEY_SHIFT_L; break; 
                case XKB_KEY_Shift_R      : key = NH_WSI_KEY_SHIFT_R; break;   
                case XKB_KEY_Control_L    : key = NH_WSI_KEY_CONTROL_L; break; 
                case XKB_KEY_Control_R    : break;
                case XKB_KEY_Caps_Lock    : break; 
                case XKB_KEY_Shift_Lock   : break;
                case XKB_KEY_Meta_L       : break;    
                case XKB_KEY_Meta_R       : break;    
                case XKB_KEY_Alt_L        : break;     
                case XKB_KEY_Alt_R        : break;     
                case XKB_KEY_Super_L      : break;   
                case XKB_KEY_Super_R      : break;   
                case XKB_KEY_Hyper_L      : break;   
                case XKB_KEY_Hyper_R      : break;   

                // Latin 1 (ISO/*IEC 8859-1 = Unicode U+0020..U+00FF)
                case XKB_KEY_space        : break;        
                case XKB_KEY_exclam       : break;       
                case XKB_KEY_quotedbl     : break;     
                case XKB_KEY_numbersign   : break;   
                case XKB_KEY_dollar       : break;       
                case XKB_KEY_percent      : break;      
                case XKB_KEY_ampersand    : break;    
                case XKB_KEY_apostrophe   : break;   
                case XKB_KEY_parenleft    : break;    
                case XKB_KEY_parenright   : break;   
                case XKB_KEY_asterisk     : break;     
                case XKB_KEY_plus         : break;         
                case XKB_KEY_comma        : break;        
                case XKB_KEY_minus        : break;        
                case XKB_KEY_period       : break;       
                case XKB_KEY_slash        : break;        
                case XKB_KEY_0            : break;            
                case XKB_KEY_1            : break;            
                case XKB_KEY_2            : break;            
                case XKB_KEY_3            : break;            
                case XKB_KEY_4            : break;            
                case XKB_KEY_5            : break;            
                case XKB_KEY_6            : break;            
                case XKB_KEY_7            : break;            
                case XKB_KEY_8            : break;            
                case XKB_KEY_9            : break;            
                case XKB_KEY_colon        : break;        
                case XKB_KEY_semicolon    : break;    
                case XKB_KEY_less         : break;         
                case XKB_KEY_equal        : break;        
                case XKB_KEY_greater      : break;      
                case XKB_KEY_question     : break;     
                case XKB_KEY_at           : break;           
                case XKB_KEY_A            : break;
                case XKB_KEY_B            : break;
                case XKB_KEY_C            : break;
                case XKB_KEY_D            : break;
                case XKB_KEY_E            : break;
                case XKB_KEY_F            : break;
                case XKB_KEY_G            : break;
                case XKB_KEY_H            : break;
                case XKB_KEY_I            : break;
                case XKB_KEY_J            : break;
                case XKB_KEY_K            : break;
                case XKB_KEY_L            : break;
                case XKB_KEY_M            : break;
                case XKB_KEY_N            : break;
                case XKB_KEY_O            : break;
                case XKB_KEY_P            : break;
                case XKB_KEY_Q            : break;
                case XKB_KEY_R            : break;
                case XKB_KEY_S            : break;
                case XKB_KEY_T            : break;
                case XKB_KEY_U            : break;
                case XKB_KEY_V            : break;
                case XKB_KEY_W            : break;
                case XKB_KEY_X            : break;
                case XKB_KEY_Y            : break;
                case XKB_KEY_Z            : break;
                case XKB_KEY_bracketleft  : break;  
                case XKB_KEY_backslash    : break;    
                case XKB_KEY_bracketright : break; 
                case XKB_KEY_asciicircum  : break;  
                case XKB_KEY_underscore   : break;   
                case XKB_KEY_grave        : break;        
                case XKB_KEY_a            : break;            
                case XKB_KEY_b            : break;            
                case XKB_KEY_c            : break;            
                case XKB_KEY_d            : break;            
                case XKB_KEY_e            : break;            
                case XKB_KEY_f            : break;            
                case XKB_KEY_g            : break;            
                case XKB_KEY_h            : break;            
                case XKB_KEY_i            : break;            
                case XKB_KEY_j            : break;            
                case XKB_KEY_k            : break;            
                case XKB_KEY_l            : break;            
                case XKB_KEY_m            : break;            
                case XKB_KEY_n            : break;            
                case XKB_KEY_o            : key = NH_WSI_KEY_SMALL_O; break;
                case XKB_KEY_p            : break;
                case XKB_KEY_q            : break;
                case XKB_KEY_r            : key = NH_WSI_KEY_SMALL_R; break;
                case XKB_KEY_s            : break;
                case XKB_KEY_t            : break;
                case XKB_KEY_u            : break;
                case XKB_KEY_v            : break;
                case XKB_KEY_w            : break;
                case XKB_KEY_x            : break;
                case XKB_KEY_y            : break;
                case XKB_KEY_z            : break;
                case XKB_KEY_braceleft    : break; 
                case XKB_KEY_bar          : break;       
                case XKB_KEY_braceright   : break;
                case XKB_KEY_asciitilde   : break; 
            }

            Event_p->key     = key;
            Event_p->trigger = event;

            Input_p->Keyboard.state_p[key] = event == NH_WSI_TRIGGER_PRESS;
        }
        
        else if (Event.type == FocusIn || Event.type == FocusOut)
        {
        }
        
        else if (Event.type == SelectionNotify)
        {
            Atom target = Event.xselection.target;
	    
            if(Event.xselection.property == None) {continue;}
            Property prop = Nh_X11_readProperty(Display_p, Window_p->XWindow, sel); 
            
//            NH_drop(prop.data, Window_p);

            XClientMessageEvent m;
	    memset(&m, sizeof(m), 0);
	    m.type         = ClientMessage;
	    m.display      = Display_p;
	    m.window       = Event.xclient.data.l[0];
	    m.message_type = XdndFinished;
	    m.format    = 32;
	    m.data.l[0] = Window_p->XWindow;
	    m.data.l[1] = 1;
	    m.data.l[2] = XdndActionCopy;

	    XSendEvent(Display_p, Event.xclient.data.l[0], False, NoEventMask, (XEvent*)&m);
            XSync(Display_p, False);
        }
    }

    NH_WSI_DIAGNOSTIC_END(NH_WSI_SUCCESS)

#endif

NH_WSI_DIAGNOSTIC_END(NH_WSI_ERROR_BAD_STATE)
}

// WINDOW ==========================================================================================

NH_WSI_RESULT Nh_X11_createWindow(
    char *title, int width, int height, Nh_X11_Window *Window_p)
{
NH_WSI_BEGIN()

#ifdef __unix__

    Window_p->Display_p = XOpenDisplay(NULL);
    Display *Display_p = Window_p->Display_p;
    NH_WSI_CHECK_NULL(Display_p)

// keyboard
    Window_p->Connection_p = XGetXCBConnection(Display_p);
    Window_p->Context_p    = xkb_context_new(XKB_CONTEXT_NO_FLAGS);
    xkb_x11_setup_xkb_extension(
        Window_p->Connection_p, XKB_X11_MIN_MAJOR_XKB_VERSION, XKB_X11_MIN_MINOR_XKB_VERSION, 
        0, NULL, NULL, NULL, NULL
    ); 
    Window_p->keyboardDeviceID = xkb_x11_get_core_keyboard_device_id(Window_p->Connection_p);
    Window_p->Keymap_p = xkb_x11_keymap_new_from_device(
        Window_p->Context_p, 
        Window_p->Connection_p, 
        Window_p->keyboardDeviceID, 
        XKB_KEYMAP_COMPILE_NO_FLAGS
    );
    Window_p->State_p = xkb_x11_state_new_from_device(
        Window_p->Keymap_p, Window_p->Connection_p, Window_p->keyboardDeviceID
    );

// RGBA color for transparency
    XVisualInfo vinfo;
    XMatchVisualInfo(Display_p, DefaultScreen(Display_p), 32, TrueColor, &vinfo);

// transparent window
    XSetWindowAttributes Attributes;
    Attributes.event_mask = StructureNotifyMask | KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask | PointerMotionMask | EnterWindowMask | LeaveWindowMask | FocusChangeMask | ExposureMask;
    Attributes.border_pixel = 0;
    Attributes.background_pixel = 0xff000000; // ARGB
    Attributes.colormap = XCreateColormap(Display_p, DefaultRootWindow(Display_p), vinfo.visual, AllocNone);

    Window_p->XWindow = XCreateWindow(
        Display_p, DefaultRootWindow(Display_p), 0, 0, width, height, 0,
        vinfo.depth, InputOutput, vinfo.visual, CWEventMask | CWBackPixel | CWBorderPixel | CWColormap, 
        &Attributes
    );
    
    XMapWindow(Display_p, Window_p->XWindow);
    
// window delete protocol
    Window_p->DeleteAtom = XInternAtom(Display_p, "WM_DELETE_WINDOW", 0);
    XSetWMProtocols(Display_p, Window_p->XWindow, &Window_p->DeleteAtom, 1);

// xdnd (file-)drop
    Atom xdndAware = XInternAtom(Display_p, "XdndAware", 0); 
    Atom xdnd = 5;
    XChangeProperty(
        Display_p, Window_p->XWindow, xdndAware, XA_ATOM, 32,
        PropModeReplace, (unsigned char*)&xdnd, 1
    );

// window name   
    XStoreName(Display_p, Window_p->XWindow, title);

// center window
//    Screen *Screen_p = ScreenOfDisplay(Display_p, DefaultScreen(Display_p));
//    XMoveWindow(Display_p, Window_p->XWindow, (int)(Screen_p->width/2) - (int)(width/2), (int)(Screen_p->height/2) - (int)(height/2));
//    XSync(Display_p, false);

    NH_WSI_DIAGNOSTIC_END(NH_WSI_SUCCESS)

#endif

NH_WSI_DIAGNOSTIC_END(NH_WSI_ERROR_BAD_STATE)
}

NH_WSI_RESULT Nh_X11_destroyWindow(
    Nh_X11_Window *Window_p)
{
NH_WSI_BEGIN()

#ifdef __unix__

    XDestroyWindow(Window_p->Display_p, Window_p->XWindow);
    XCloseDisplay(Window_p->Display_p);
    xkb_state_unref(Window_p->State_p);
    xkb_keymap_unref(Window_p->Keymap_p);
    xkb_context_unref(Window_p->Context_p);

    NH_WSI_DIAGNOSTIC_END(NH_WSI_SUCCESS)

#endif

NH_WSI_DIAGNOSTIC_END(NH_WSI_ERROR_BAD_STATE)
}

// BACKGROUND COLOR ================================================================================

NH_WSI_RESULT Nh_X11_setWindowBackgroundColor(
    Nh_X11_Window *Window_p, Nh_Color Color)
{
NH_WSI_BEGIN()

#ifdef __unix__

    uint16_t rgba_p[4] = {
        (uint16_t)Color.r*255,
        (uint16_t)Color.g*255,
        (uint16_t)Color.b*255,
        (uint16_t)Color.a*255,
    };

    long double color = 0x00000000; // == black
    color = rgba_p[0] << 16 | rgba_p[1] << 8 | rgba_p[2] << 0 | rgba_p[3] << 24; 

    XSetWindowAttributes Attributes;
    Attributes.background_pixel = color;
    XChangeWindowAttributes(Window_p->Display_p, Window_p->XWindow, CWBackPixel, &Attributes);

    NH_WSI_DIAGNOSTIC_END(NH_WSI_SUCCESS)

#endif

NH_WSI_DIAGNOSTIC_END(NH_WSI_ERROR_BAD_STATE)
}

// TITLE ===========================================================================================

NH_WSI_RESULT Nh_X11_setWindowTitle(
    Nh_X11_Window *Window_p, char *title_p)
{
NH_WSI_BEGIN()

#ifdef __unix__

    XStoreName(Window_p->Display_p, Window_p->XWindow, title_p);

    NH_WSI_DIAGNOSTIC_END(NH_WSI_SUCCESS)

#endif

NH_WSI_DIAGNOSTIC_END(NH_WSI_ERROR_BAD_STATE)
}
// CURSOR ==========================================================================================

// #ifndef _X11_CURSORFONT_H_
// #define _X11_CURSORFONT_H_
// #define XC_num_glyphs 154
// #define XC_X_cursor 0
// #define XC_arrow 2
// #define XC_based_arrow_down 4
// #define XC_based_arrow_up 6
// #define XC_boat 8
// #define XC_bogosity 10
// #define XC_bottom_left_corner 12
// #define XC_bottom_right_corner 14
// #define XC_bottom_side 16
// #define XC_bottom_tee 18
// #define XC_box_spiral 20
// #define XC_center_ptr 22
// #define XC_circle 24
// #define XC_clock 26
// #define XC_coffee_mug 28
// #define XC_cross 30
// #define XC_cross_reverse 32
// #define XC_crosshair 34
// #define XC_diamond_cross 36
// #define XC_dot 38
// #define XC_dotbox 40
// #define XC_double_arrow 42
// #define XC_draft_large 44
// #define XC_draft_small 46
// #define XC_draped_box 48
// #define XC_exchange 50
// #define XC_fleur 52
// #define XC_gobbler 54
// #define XC_gumby 56
// #define XC_hand1 58
// #define XC_hand2 60
// #define XC_heart 62
// #define XC_icon 64
// #define XC_iron_cross 66
// #define XC_left_ptr 68
// #define XC_left_side 70
// #define XC_left_tee 72
// #define XC_leftbutton 74
// #define XC_ll_angle 76
// #define XC_lr_angle 78
// #define XC_man 80
// #define XC_middlebutton 82
// #define XC_mouse 84
// #define XC_pencil 86
// #define XC_pirate 88
// #define XC_plus 90
// #define XC_question_arrow 92
// #define XC_right_ptr 94
// #define XC_right_side 96
// #define XC_right_tee 98
// #define XC_rightbutton 100
// #define XC_rtl_logo 102
// #define XC_sailboat 104
// #define XC_sb_down_arrow 106
// #define XC_sb_h_double_arrow 108
// #define XC_sb_left_arrow 110
// #define XC_sb_right_arrow 112
// #define XC_sb_up_arrow 114
// #define XC_sb_v_double_arrow 116
// #define XC_shuttle 118
// #define XC_sizing 120
// #define XC_spider 122
// #define XC_spraycan 124
// #define XC_star 126
// #define XC_target 128
// #define XC_tcross 130
// #define XC_top_left_arrow 132
// #define XC_top_left_corner 134
// #define XC_top_right_corner 136
// #define XC_top_side 138
// #define XC_top_tee 140
// #define XC_trek 142
// #define XC_ul_angle 144
// #define XC_umbrella 146
// #define XC_ur_angle 148
// #define XC_watch 150
// #define XC_xterm 152
// #endif /* _X11_CURSORFONT_H_ */

NH_WSI_RESULT Nh_X11_setMouseCursor(
    Nh_X11_Window *Window_p, int type)
{
NH_WSI_BEGIN()

#ifdef __unix__

    Cursor C;

    switch (type)
    {
        case NH_WSI_MOUSE_CURSOR_DEFAULT : C = XcursorLibraryLoadCursor(Window_p->Display_p, "arrow"); break;
        case NH_WSI_MOUSE_CURSOR_TEXT    : C = XcursorLibraryLoadCursor(Window_p->Display_p, "xterm"); break;
    }

    XDefineCursor(Window_p->Display_p, Window_p->XWindow, C);

    NH_WSI_DIAGNOSTIC_END(NH_WSI_SUCCESS)

#endif

NH_WSI_DIAGNOSTIC_END(NH_WSI_ERROR_BAD_STATE)
}


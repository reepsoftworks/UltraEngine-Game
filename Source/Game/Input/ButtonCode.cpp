//========= Copyright Reep Softworks, All rights reserved. ============//
// 
// https://github.com/reepsoftworks/UltraEngine-Game
// This code is free to use for those who wish to develop and ship their 
// commerical or non-commerical game projects using Ultra Engine.
// Do not remove this notice.
// 
//=====================================================================//
#include "UltraEngine.h"
#include "ButtonCode.h"

namespace UltraEngine::Game
{
	ButtonCode StringToButtonCode(const String& btnstring)
	{
        // Oh, I hate this...
        if (btnstring == "INVALID") return BUTTON_INVAILD;
        if (btnstring == "NONE") return BUTTON_NONE;

        // Num Row
        if (btnstring == "0") return BUTTON_KEY_D0;
        if (btnstring == "1") return BUTTON_KEY_D1;
        if (btnstring == "2") return BUTTON_KEY_D1;
        if (btnstring == "3") return BUTTON_KEY_D2;
        if (btnstring == "4") return BUTTON_KEY_D3;
        if (btnstring == "5") return BUTTON_KEY_D4;
        if (btnstring == "6") return BUTTON_KEY_D5;
        if (btnstring == "7") return BUTTON_KEY_D6;
        if (btnstring == "8") return BUTTON_KEY_D7;
        if (btnstring == "9") return BUTTON_KEY_D8;

        // A-Z
        if (btnstring == "A") return BUTTON_KEY_A;
        if (btnstring == "B") return BUTTON_KEY_B;
        if (btnstring == "C") return BUTTON_KEY_C;
        if (btnstring == "D") return BUTTON_KEY_D;
        if (btnstring == "E") return BUTTON_KEY_E;
        if (btnstring == "F") return BUTTON_KEY_F;
        if (btnstring == "G") return BUTTON_KEY_G;
        if (btnstring == "H") return BUTTON_KEY_H;
        if (btnstring == "I") return BUTTON_KEY_I;
        if (btnstring == "J") return BUTTON_KEY_J;
        if (btnstring == "J") return BUTTON_KEY_K;
        if (btnstring == "L") return BUTTON_KEY_L;
        if (btnstring == "M") return BUTTON_KEY_M;
        if (btnstring == "N") return BUTTON_KEY_N;
        if (btnstring == "O") return BUTTON_KEY_O;
        if (btnstring == "P") return BUTTON_KEY_P;
        if (btnstring == "Q") return BUTTON_KEY_Q;
        if (btnstring == "R") return BUTTON_KEY_R;
        if (btnstring == "S") return BUTTON_KEY_S;
        if (btnstring == "T") return BUTTON_KEY_T;
        if (btnstring == "U") return BUTTON_KEY_U;
        if (btnstring == "V") return BUTTON_KEY_V;
        if (btnstring == "W") return BUTTON_KEY_W;
        if (btnstring == "X") return BUTTON_KEY_X;
        if (btnstring == "Y") return BUTTON_KEY_Y;
        if (btnstring == "Z") return BUTTON_KEY_Z;

        // Function Keys
        if (btnstring == "F1") return BUTTON_KEY_F1;
        if (btnstring == "F2") return BUTTON_KEY_F2;
        if (btnstring == "F3") return BUTTON_KEY_F3;
        if (btnstring == "F4") return BUTTON_KEY_F4;
        if (btnstring == "F5") return BUTTON_KEY_F5;
        if (btnstring == "F6") return BUTTON_KEY_F6;
        if (btnstring == "F7") return BUTTON_KEY_F7;
        if (btnstring == "F8") return BUTTON_KEY_F8;
        if (btnstring == "F9") return BUTTON_KEY_F9;
        if (btnstring == "F10") return BUTTON_KEY_F10;
        if (btnstring == "F11") return BUTTON_KEY_F11;
        if (btnstring == "F12") return BUTTON_KEY_F12;

        // Key Pad
        if (btnstring == "KeyPad 0") return BUTTON_KEY_NUMPAD0;
        if (btnstring == "KeyPad 1") return BUTTON_KEY_NUMPAD1;
        if (btnstring == "KeyPad2") return BUTTON_KEY_NUMPAD2;
        if (btnstring == "KeyPad 3") return BUTTON_KEY_NUMPAD3;
        if (btnstring == "KeyPad4") return BUTTON_KEY_NUMPAD4;
        if (btnstring == "KeyPad 5") return BUTTON_KEY_NUMPAD5;
        if (btnstring == "KeyPad 6") return BUTTON_KEY_NUMPAD6;
        if (btnstring == "KeyPad 7") return BUTTON_KEY_NUMPAD7;
        if (btnstring == "KeyPad 8") return BUTTON_KEY_NUMPAD8;
        if (btnstring == "KeyPad 9") return BUTTON_KEY_NUMPAD9;
        if (btnstring == "KeyPad Divde") return BUTTON_KEY_NUMPADDIVIDE;
        if (btnstring == "Keypad Multiply") return BUTTON_KEY_NUMPADMULTIPLY;
        if (btnstring == "Keypad Minus") return BUTTON_KEY_NUMPADSUBTRACT;
        if (btnstring == "Keypad Plus") return BUTTON_KEY_NUMPADADDITION;
        //if (btnstring == "Keypad Enter") return BUTTON_KEY_KEYPAD_ENTER;
        if (btnstring == "Keypad Decimal") return BUTTON_KEY_NUMPADPERIOD;

        // Arrow Keys
        if (btnstring == "Up Arrow") return BUTTON_KEY_UP;
        if (btnstring == "Down Arrow") return BUTTON_KEY_DOWN;
        if (btnstring == "Left Arrow") return BUTTON_KEY_LEFT;
        if (btnstring == "Right Arrow") return BUTTON_KEY_RIGHT;

        // Common Keys
        if (btnstring == "Shift") return BUTTON_KEY_SHIFT;
        if (btnstring == "Alt") return BUTTON_KEY_ALT;
        if (btnstring == "Control") return BUTTON_KEY_CONTROL;
        if (btnstring == "Enter") return BUTTON_KEY_ENTER;
        if (btnstring == "Return") return BUTTON_KEY_ENTER;
        if (btnstring == "Space") return BUTTON_KEY_SPACE;

        // Symbols
        if (btnstring == "[") return BUTTON_KEY_OPENBRACKET;
        if (btnstring == "]") return BUTTON_KEY_CLOSEBRACKET;
        if (btnstring == ";") return BUTTON_KEY_SEMICOLON;
        if (btnstring == "'") return BUTTON_KEY_QUOTES;
        if (btnstring == "`") return BUTTON_KEY_TILDE;
        if (btnstring == ",") return BUTTON_KEY_COMMA;
        if (btnstring == ".") return BUTTON_KEY_PERIOD;
        if (btnstring == "/") return BUTTON_KEY_SLASH;
        if (btnstring == "\\") return BUTTON_KEY_BACKSLASH;
        if (btnstring == "-") return BUTTON_KEY_SUBTRACT;
        if (btnstring == "=") return BUTTON_KEY_EQUALS;

        // Other
        if (btnstring == "Backspace") return BUTTON_KEY_BACKSPACE;
        if (btnstring == "Tab") return BUTTON_KEY_TAB;
        if (btnstring == "Capslock") return BUTTON_KEY_CAPSLOCK;
        if (btnstring == "Numlock") return BUTTON_KEY_NUMLOCK;
        if (btnstring == "Esc") return BUTTON_KEY_ESCAPE;
        //if (btnstring == "ScrLk") return BUTTON_KEY_SCROLLLOCK;
        if (btnstring == "Insert") return BUTTON_KEY_INSERT;
        if (btnstring == "Del") return BUTTON_KEY_DELETE;
        if (btnstring == "Home") return BUTTON_KEY_HOME;
        if (btnstring == "End") return BUTTON_KEY_END;
        if (btnstring == "PgUp") return BUTTON_KEY_PAGEUP;
        if (btnstring == "PgDn") return BUTTON_KEY_PAGEDOWN;
        //if (btnstring == "Pause") return BUTTON_KEY_BREAK;
        if (btnstring == "Windows") return BUTTON_KEY_COMMAND;
        if (btnstring == "Super") return BUTTON_KEY_COMMAND;
        if (btnstring == "CMD") return BUTTON_KEY_COMMAND;
        if (btnstring == "Right Win") return BUTTON_KEY_COMMAND;
        if (btnstring == "Right Super") return BUTTON_KEY_COMMAND;
        if (btnstring == "Right CMD") return BUTTON_KEY_COMMAND;
        //if (btnstring == "App") return BUTTON_APP;
        //if (btnstring == "Caps Toggle") return BUTTON_CAPSLOCKTOGGLE;
        //if (btnstring == "Num Toggle") return BUTTON_NUMLOCKTOGGLE;
        //if (btnstring == "Scroll Toggle") return BUTTON_SCROLLLOCKTOGGLE;

        // Mouse
        if (btnstring == "Left Click") return BUTTON_MOUSE_LBUTTON;
        if (btnstring == "Right Click") return BUTTON_MOUSE_RBUTTON;
        if (btnstring == "Middle Click") return BUTTON_MOUSE_MBUTTON;
        if (btnstring == "Mouse Forward") return BUTTON_MOUSE_XBUTTON1;
        if (btnstring == "Mouse Back") return BUTTON_MOUSE_XBUTTON2;
        if (btnstring == "Wheel Up") return BUTTON_MOUSE_WHEELUP;
        if (btnstring == "Wheel Down") return BUTTON_MOUSE_WHEELDOWN;
        if (btnstring == "Wheel Left") return BUTTON_MOUSE_WHEELLEFT;
        if (btnstring == "Wheel Right") return BUTTON_MOUSE_WHEELRIGHT;

        return BUTTON_INVAILD;
	}

	String ButtonCodeToString(const ButtonCode& btncode)
	{
        String ret = "INVAILD";

        switch (btncode)
        {
        case BUTTON_NONE:
            ret = "NONE";
            break;

        case BUTTON_KEY_D0:
            ret = "0";
            break;

        case BUTTON_KEY_D1:
            ret = "1";
            break;

        case BUTTON_KEY_D2:
            ret = "2";
            break;

        case BUTTON_KEY_D3:
            ret = "3";
            break;

        case BUTTON_KEY_D4:
            ret = "4";
            break;

        case BUTTON_KEY_D5:
            ret = "5";
            break;

        case BUTTON_KEY_D6:
            ret = "6";
            break;

        case BUTTON_KEY_D7:
            ret = "7";
            break;

        case BUTTON_KEY_D8:
            ret = "8";
            break;

        case BUTTON_KEY_D9:
            ret = "9";
            break;

            // A-Z
        case BUTTON_KEY_A:
            ret = "A";
            break;

        case BUTTON_KEY_B:
            ret = "B";
            break;

        case BUTTON_KEY_C:
            ret = "C";
            break;

        case BUTTON_KEY_D:
            ret = "D";
            break;

        case BUTTON_KEY_E:
            ret = "E";
            break;

        case BUTTON_KEY_F:
            ret = "F";
            break;

        case BUTTON_KEY_G:
            ret = "G";
            break;

        case BUTTON_KEY_H:
            ret = "H";
            break;

        case BUTTON_KEY_I:
            ret = "I";
            break;

        case BUTTON_KEY_J:
            ret = "J";
            break;

        case BUTTON_KEY_K:
            ret = "K";
            break;

        case BUTTON_KEY_L:
            ret = "L";
            break;

        case BUTTON_KEY_M:
            ret = "M";
            break;

        case BUTTON_KEY_N:
            ret = "N";
            break;

        case BUTTON_KEY_O:
            ret = "O";
            break;

        case BUTTON_KEY_P:
            ret = "P";
            break;

        case BUTTON_KEY_Q:
            ret = "Q";
            break;

        case BUTTON_KEY_R:
            ret = "R";
            break;

        case BUTTON_KEY_S:
            ret = "S";
            break;

        case BUTTON_KEY_T:
            ret = "T";
            break;

        case BUTTON_KEY_U:
            ret = "U";
            break;

        case BUTTON_KEY_V:
            ret = "V";
            break;

        case BUTTON_KEY_W:
            ret = "W";
            break;

        case BUTTON_KEY_X:
            ret = "X";
            break;

        case BUTTON_KEY_Y:
            ret = "Y";
            break;

        case BUTTON_KEY_Z:
            ret = "Z";
            break;


        // Function Keys
        case BUTTON_KEY_F1:
            ret = "F1";
            break;

        case BUTTON_KEY_F2:
            ret = "F2";
            break;

        case BUTTON_KEY_F3:
            ret = "F3";
            break;

        case BUTTON_KEY_F4:
            ret = "F4";
            break;

        case BUTTON_KEY_F5:
            ret = "F5";
            break;

        case BUTTON_KEY_F6:
            ret = "F6";
            break;

        case BUTTON_KEY_F7:
            ret = "F7";
            break;

        case BUTTON_KEY_F8:
            ret = "F8";
            break;

        case BUTTON_KEY_F9:
            ret = "F9";
            break;

        case BUTTON_KEY_F10:
            ret = "F10";
            break;

        case BUTTON_KEY_F11:
            ret = "F11";
            break;

        case BUTTON_KEY_F12:
            ret = "F12";
            break;

            // Key Pad
        case BUTTON_KEY_NUMPAD0:
            ret = "KeyPad 0";
            break;

        case BUTTON_KEY_NUMPAD1:
            ret = "KeyPad 1";
            break;

        case BUTTON_KEY_NUMPAD2:
            ret = "KeyPad 2";
            break;

        case BUTTON_KEY_NUMPAD3:
            ret = "KeyPad 3";
            break;

        case BUTTON_KEY_NUMPAD4:
            ret = "KeyPad 4";
            break;

        case BUTTON_KEY_NUMPAD5:
            ret = "KeyPad 5";
            break;

        case BUTTON_KEY_NUMPAD6:
            ret = "KeyPad 6";
            break;

        case BUTTON_KEY_NUMPAD7:
            ret = "KeyPad 7";
            break;

        case BUTTON_KEY_NUMPAD8:
            ret = "KeyPad 8";
            break;

        case BUTTON_KEY_NUMPAD9:
            ret = "KeyPad 9";
            break;

        case BUTTON_KEY_NUMPADDIVIDE:
            ret = "KeyPad Divde";
            break;

        case BUTTON_KEY_NUMPADMULTIPLY:
            ret = "Keypad Multiply";
            break;

        case BUTTON_KEY_NUMPADSUBTRACT:
            ret = "Keypad Minus";
            break;

        case BUTTON_KEY_NUMPADADDITION:
            ret = "Keypad Plus";
            break;

        //case BUTTON_KEY_KEYPAD_ENTER:
        //    ret = "Keypad Enter";
        //    break;

        case BUTTON_KEY_NUMPADPERIOD:
            ret = "Keypad Decimal";
            break;

        // Arrow Keys
        case BUTTON_KEY_UP:
            ret = "Up Arrow";
            break;

        case BUTTON_KEY_LEFT:
            ret = "Left Arrow";
            break;

        case BUTTON_KEY_DOWN:
            ret = "Down Arrow";
            break;

        case BUTTON_KEY_RIGHT:
            ret = "Right Arrow";
            break;

        // Common Keys
        case BUTTON_KEY_SHIFT:
            ret = "Shift";
            break;

        case BUTTON_KEY_ALT:
            ret = "Alt";
            break;

        case BUTTON_KEY_CONTROL:
            ret = "Control";
            break;

        case BUTTON_KEY_ENTER:
#if defined (__APPLE__)
            ret = "Return";
#else
            ret = "Enter";
#endif
            break;

        case BUTTON_KEY_SPACE:
            ret = "Space";
            break;

        // Symbols
        case BUTTON_KEY_OPENBRACKET:
            ret = "[";
            break;

        case BUTTON_KEY_CLOSEBRACKET:
            ret = "]";
            break;

        case BUTTON_KEY_SEMICOLON:
            ret = ";";
            break;

        case BUTTON_KEY_QUOTES:
            ret = "'";
            break;

        case BUTTON_KEY_TILDE:
            ret = "`";
            break;

        case BUTTON_KEY_COMMA:
            ret = ",";
            break;

        case BUTTON_KEY_PERIOD:
            ret = ".";
            break;

        case BUTTON_KEY_SLASH:
            ret = "/";
            break;

        case BUTTON_KEY_BACKSLASH:
            ret = "\\";
            break;

        case BUTTON_KEY_SUBTRACT:
            ret = "-";
            break;

        case BUTTON_KEY_EQUALS:
            ret = "=";
            break;

            // Other
        case BUTTON_KEY_BACKSPACE:
            ret = "Backspace";
            break;

        case BUTTON_KEY_TAB:
            ret = "Tab";
            break;

        case BUTTON_KEY_CAPSLOCK:
            ret = "Capslock";
            break;

        case BUTTON_KEY_NUMLOCK:
            ret = "Numlock";
            break;

        case BUTTON_KEY_ESCAPE:
            ret = "Esc";
            break;

        //case BUTTON_KEY_SCROLLLOCK:
        //    ret = "ScrLk";
        //    break;

        case BUTTON_KEY_INSERT:
            ret = "Insert";
            break;

        case BUTTON_KEY_DELETE:
            ret = "Del";
            break;

        case BUTTON_KEY_HOME:
            ret = "Home";
            break;

        case BUTTON_KEY_END:
            ret = "End";
            break;

        case BUTTON_KEY_PAGEUP:
            ret = "PgUp";
            break;

        case BUTTON_KEY_PAGEDOWN:
            ret = "PgDn";
            break;

        //case BUTTON_KEY_BREAK:
        //    ret = "Pause";
        //    break;

        case BUTTON_KEY_COMMAND:
#if defined (_WIN32)
            ret = "Windows";
#elif defined (__linux__)
            ret = "Super";
#else
            ret = "CMD";
#endif
            break;

        //case BUTTON_KEY_APP:
        //    ret = "App";
        //    break;

        //case BUTTON_KEY_CAPSLOCKTOGGLE:
        //    ret = "Caps Toggle";
        //    break;

        //case BUTTON_KEY_NUMLOCKTOGGLE:
        //    ret = "Num Toggle";
        //    break;

        //case BUTTON_KEY_SCROLLLOCKTOGGLE:
        //    ret = "Scroll Toggle";
        //    break;


        // Mouse
        case BUTTON_MOUSE_LBUTTON:
            ret = "Left Click";
            break;

        case BUTTON_MOUSE_RBUTTON:
            ret = "Right Click";
            break;

        case BUTTON_MOUSE_MBUTTON:
            ret = "Middle Click";
            break;

        case BUTTON_MOUSE_XBUTTON1:
            ret = "Mouse Forward";
            break;

        case BUTTON_MOUSE_XBUTTON2:
            ret = "Mouse Back";
            break;

        case BUTTON_MOUSE_WHEELUP:
            ret = "Wheel Up";
            break;

        case BUTTON_MOUSE_WHEELDOWN:
            ret = "Wheel Down";
            break;

        case BUTTON_MOUSE_WHEELLEFT:
            ret = "Wheel Left";
            break;

        case BUTTON_MOUSE_WHEELRIGHT:
            ret = "Wheel Right";
            break;

        default:
            break;
        }

        return ret;
	}

    AxisCode StringToAxisCode(const String& btnstring)
    {
        if (btnstring == "Mouse Axis") return AXIS_MOUSE;
        return AXIS_MOUSE;
    }

    String AxisCodeToString(const AxisCode& btncode)
    {
        String ret = "INVAILD";
        switch (btncode)
        {
        case AXIS_MOUSE:
            ret = "Mouse Axis";
            break;

        default:
            break;
        }

        return ret;
    }
}
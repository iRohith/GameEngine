#pragma once

#include "Core.hpp"
#include <functional>

//#define EVENT_FLAG_CAT_MUL 20
//#define EVENT_FLAG_POS_MUL 1000

namespace GameEngine {
	using Runnable = std::function<void()>;
    
    namespace Events {

        extern GEAPI void Run(Runnable*);

        enum ButtonState {
            NONE_STATE = 0, RELEASED, PRESSED, HELD
        };
        
        enum WindowState {
			RESUME, PAUSE, FOCUS_IN, FOCUS_OUT, QUIT
		};

        enum MouseButton {
            BUTTON1,
            BUTTON2,
            BUTTON3,
            BUTTON4,
            BUTTON5,
            __num_buttons__
        };

        enum KeyCode {
            NONE = 0,
            BACKSPACE,
            TAB,
            ENTER,
            SPACE,
            LSHIFT, RSHIFT,
            LALT, RALT,
            BREAK,
            CAPSLOCK,
            PAGEUP,
            PAGEDOWN,
            END,
            HOME,
            ESC,
            ARROW_LEFT, ARROW_UP, ARROW_RIGHT, ARROW_DOWN,
            DEL,
            NUMLOCK, SCROLLLOCK,
            PRINTSCREEN,
            INSERT,
            SUPER_LEFT,
            SUPER_RIGHT,
            CLEAR,
            LCTRL, RCTRL,
            F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12, F13, F14, F15, F16, F17, F18, F19, F20, F21, F22, F23, F24, F25, F26, F27, F28, F29, F30, F31, F32, F33, F34, F35,
            NumpadPLUS, NumpadMINUS, NumpadMULTIPLY, NumpadDIVIDE, NumpadPERIOD,
            Numpad0, Numpad1, Numpad2, Numpad3, Numpad4, Numpad5, Numpad6, Numpad7, Numpad8, Numpad9,
            __num_keys__
        };
/*
        enum EventCategory {
            MOUSE = 0, TOUCH, KEY, WINDOW
        };

        enum EventMode {
            PRESS = 0, RELEASE, CLICK,
            MOVE, ENTER, LEAVE, SCROLL, 
            PINCH, 
            RESIZE, RESUME, PAUSE, QUIT
        };

        struct GEAPI Event {
            
            static constexpr uint32_t ParseFlags(const EventCategory ec, const EventMode em, const uint32_t pos = 0){
                return ((ec*EVENT_FLAG_CAT_MUL) + em) + (pos * EVENT_FLAG_POS_MUL);
            }

            static constexpr EventCategory GetCategory(const uint32_t flag){
                return (EventCategory)((flag % EVENT_FLAG_POS_MUL) / EVENT_FLAG_CAT_MUL);
            }

            static constexpr EventMode GetEventMode(const uint32_t flag){
                return (EventMode)(flag % EVENT_FLAG_CAT_MUL);
            }

            uint32_t EventFlags;
        };

        struct GEAPI MouseEvent : public Event {
            enum MouseButton {
                NONE, BUTTON1, BUTTON2, BUTTON3
            };
            int X, Y;
            MouseButton button;
            public:
            MouseEvent(MouseButton mb, int x, int y):button(mb),X(x),Y(y){}
            MouseEvent(){}
        };

        struct GEAPI KeyEvent : public Event {
            KeyCode keyCode;
            public:
            KeyEvent(KeyCode kc) : keyCode(kc){}
            KeyEvent(){}
        };

        struct GEAPI WindowEvent : public Event {
            int Width, Height;
            public:
            WindowEvent(int width, int height):Width(width),Height(height){}
            WindowEvent(){}
        };

        typedef bool(*BB_EventCallback)(Event*, void*);
        
        class GEAPI EventHandler {
            public:
            uint32_t addEventCallback(const EventCategory cat, const EventMode mode, BB_EventCallback cb, void* data);
            bool removeEventCallback(const uint32_t eventId);
            void InvokeCallbacks(Event*e, const EventCategory cat, const EventMode mode);
        };

        struct GEAPI TouchEvent : public MouseEvent {
            // TODO
        };
*/
    }
}
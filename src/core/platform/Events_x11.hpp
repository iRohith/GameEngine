#include "../EventHandler.hpp"
#include "../InternalEvents.hpp"
#include <GameEngine/Core/Events.hpp>
#include <GameEngine/Core/App.hpp>
#include "../GELog.hpp"
#include "X11HNDL.hpp"
#include <map>

using namespace GameEngine;

namespace GameEngine {
    extern std::vector<Runnable *> runOnUiThread;
    extern std::map<XWindow, Window*> windowsMap;
    extern Atom wmDeleteWindow;
    extern Display* xdisplay;
    Window* activeWindow;

    Window* _GetActiveWindow(){ return activeWindow; }
}

Events::KeyCode KeyCodeFromX11(int key);

void EventHandler::RegisterWindow(Window* win, void* ptr){
    auto h = GetX11HNDL(ptr);
    windowsMap.insert({h->xwindow, win});
    
    if (wmDeleteWindow == 0){
        wmDeleteWindow = XInternAtom(xdisplay, "WM_DELETE_WINDOW", false);
    }
	XSetWMProtocols(xdisplay, h->xwindow, &wmDeleteWindow, 1);
}

void Events::Run(Runnable* r){
    runOnUiThread.push_back(r);
}

EventHandler::~EventHandler(){
    for (auto r : runOnUiThread) delete r;
    runOnUiThread.clear();
    windowsMap.clear();
}

void EventHandler::Update()
{
    InternalInput::Update();
    XEvent ev;

    while (App::IsAlive() && XPending(xdisplay))
    {
        XNextEvent(xdisplay, &ev);
        auto win = windowsMap[ev.xany.window];
        if ((ev.type == ClientMessage && ev.xclient.data.l[0] == wmDeleteWindow) || ev.type == DestroyNotify)
        {
            OnWindowStateChange(win, WindowState::QUIT);
        }
        else
            switch (ev.type)
            {
            case ButtonPress:
            case ButtonRelease:
            {
                auto mb = Events::MouseButton::BUTTON1;
                if (ev.xbutton.button == Button2)
                    mb = Events::MouseButton::BUTTON2;
                else if (ev.xbutton.button == Button3)
                    mb = Events::MouseButton::BUTTON3;
                OnMouseButtonEvent(win, mb, ev.type == ButtonPress ? Events::PRESSED : Events::RELEASED);
                break;
            }
            case LeaveNotify:
            {
                OnMouseMoveEvent(win, -1, -1);
                break;
            }
            case MotionNotify:
            {
                OnMouseMoveEvent(win, ev.xbutton.x, ev.xbutton.y);
                break;
            }
            case KeyPress:
            case KeyRelease:
            {
                auto bs = ev.type == KeyPress ? Events::PRESSED : Events::RELEASED;
                int keysyms_per_keycode;
                auto keysym = XGetKeyboardMapping(xdisplay, ev.xkey.keycode, 1, &keysyms_per_keycode);
                auto ke = KeyCodeFromX11(*keysym);
                if (ke != Events::KeyCode::NONE)
                {
                    if (bs == RELEASED || Input::GetKeyState(ke) != HELD)
                        OnKeyEvent(win, ke, bs);
                }
                else
                {
                    auto s = XKeysymToString(*keysym);
                    if (s != NULL && (bs == RELEASED || Input::GetCharState((uint8_t)(*s)) != HELD))
                        OnCharEvent(win, (uint8_t)(*s), bs);
                }
                XFree(keysym);
                break;
            }
            case ConfigureNotify:
            {
                int wid = ev.xconfigure.width, hei = ev.xconfigure.height;
                if (win->GetConfig().Width != wid || win->GetConfig().Height != hei)
                    OnWindowResizeEvent(win, wid, hei);
                break;
            }
            case MapNotify:
            case UnmapNotify:
            {
                OnWindowStateChange(win, ev.type == MapNotify ? Events::RESUME : Events::PAUSE);
                break;
            }
            case FocusIn:
            case FocusOut:
            {
                if (ev.type == FocusIn) activeWindow = win;
                OnWindowStateChange(win, ev.type == FocusIn ? Events::FOCUS_IN : Events::FOCUS_OUT);
                break;
            }
            default:
                break;
            }
    }

    for (auto &r : runOnUiThread)
    {
        (*r)();
        delete r;
    }
    runOnUiThread.clear();

}

Events::KeyCode KeyCodeFromX11(int key){
    using KC = Events::KeyCode;
    if (XK_KP_0 <= key && key <= XK_KP_9) return (KC)(key - XK_KP_0 + KC::Numpad0);
    if (XK_F1 <= key && key <= XK_F35) return (KC)(key - XK_F1 + KC::F1);
    switch (key){
        case XK_BackSpace: return KC::BACKSPACE;
        case XK_Tab: return KC::TAB;
        case XK_Return: return KC::ENTER;
        case XK_space: return KC::SPACE;
        case XK_Shift_L: return KC::LSHIFT;
        case XK_Shift_R: return KC::RSHIFT;
        case XK_Alt_L: return KC::LALT;
        case XK_Alt_R: return KC::RALT;
        case XK_Break: return KC::BREAK;
        case XK_Caps_Lock: return KC::CAPSLOCK;
        case XK_Page_Up: return KC::PAGEUP;
        case XK_Page_Down: return KC::PAGEDOWN;
        case XK_End: return KC::END;
        case XK_Home: return KC::HOME;
        case XK_Escape: return KC::ESC;
        case XK_Left: return KC::ARROW_LEFT;
        case XK_Up: return KC::ARROW_UP;
        case XK_Right: return KC::ARROW_RIGHT;
        case XK_Down: return KC::ARROW_DOWN;
        case XK_Delete: return KC::DEL;
        case XK_KP_Add: return KC::NumpadPLUS;
        case XK_KP_Subtract: return KC::NumpadMINUS;
        case XK_KP_Multiply: return KC::NumpadMULTIPLY;
        case XK_KP_Divide: return KC::NumpadDIVIDE;
        case XK_KP_Decimal: return KC::NumpadPERIOD;
        case XK_Num_Lock: return KC::NUMLOCK;
        case XK_Scroll_Lock: return KC::SCROLLLOCK;
        case XK_Print: return KC::PRINTSCREEN;
        case XK_Insert: return KC::INSERT;
        case XK_Super_L: return KC::SUPER_LEFT;
        case XK_Super_R: return KC::SUPER_RIGHT;
        case XK_Clear: return KC::CLEAR;
        case XK_Control_L: return KC::LCTRL;
        case XK_Control_R: return KC::RCTRL;
        default: return KC::NONE;
    }
}

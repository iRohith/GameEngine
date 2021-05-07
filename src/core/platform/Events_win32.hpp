#include "Win32HNDL.hpp"
#include "../EventHandler.hpp"
#include "../InternalEvents.hpp"
#include <GameEngine/Core/Events.hpp>
#include <map>

using namespace GameEngine;

namespace GameEngine {
    extern std::vector<Runnable *> runOnUiThread;
    Window* activeWindow;

    Window* _GetActiveWindow(){ return activeWindow; }
    LRESULT windowProcedure(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
}

void EventHandler::RegisterWindow(Window* win, void* ptr) {}

void Events::Run(Runnable* r) {
	runOnUiThread.push_back(r);
}

EventHandler::~EventHandler(){
    for (auto r : runOnUiThread) delete r;
    runOnUiThread.clear();
}


void EventHandler::Update()
{
	//auto h = GetHNDL(hndl);
	InternalInput::Update();

	MSG Msg;

	while (PeekMessage(&Msg, NULL, 0, 0, PM_REMOVE) > 0) {
		DispatchMessage(&Msg);
	}

    for (auto& r : runOnUiThread)
	{
		(*r)();
		delete r;
	}
	runOnUiThread.clear();
}

KeyCode KeyCodeFromWin32(int key);

LRESULT GameEngine::windowProcedure(
	HWND   hwnd
	, UINT   msg
	, WPARAM wParam
	, LPARAM lParam
) {
	using namespace GameEngine::Events;

	Window* mWin = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
	
	switch (msg)
	{
	case WM_NCCREATE: {
		SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)((CREATESTRUCT*)lParam)->lpCreateParams);
		SetWindowPos(hwnd, 0, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER);
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	case WM_CLOSE: {
		OnWindowStateChange(mWin, WindowState::QUIT);
		break;
	}
	case WM_LBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_MBUTTONUP:
	case WM_RBUTTONUP: {
		auto mb = MouseButton::BUTTON1;
		auto bs = ButtonState::PRESSED;
		switch (msg) {
		case WM_LBUTTONUP: bs = ButtonState::RELEASED; break;
		case WM_MBUTTONDOWN: mb = BUTTON2; break;
		case WM_MBUTTONUP: bs = ButtonState::RELEASED; mb = BUTTON2; break;
		case WM_RBUTTONDOWN: mb = BUTTON3; break;
		case WM_RBUTTONUP: bs = ButtonState::RELEASED; mb = BUTTON3; break;
		default: break;
		}
		OnMouseButtonEvent(mWin, mb, bs);
		break;
	}
	case WM_MOUSEMOVE: {
		OnMouseMoveEvent(mWin, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		break;
	}
	case WM_MOUSELEAVE: {
		OnMouseMoveEvent(mWin, -1, -1);
		break;
	}
	case WM_KEYDOWN:
	case WM_KEYUP: {
		auto key = KeyCodeFromWin32((int)wParam);
		auto state = msg == WM_KEYDOWN ? PRESSED : RELEASED;
		if (key != KeyCode::NONE){
			if (state == RELEASED || Input::GetKeyState(key) != HELD) OnKeyEvent(mWin, key, state);
		} else {
			const int keyboardScanCode = (lParam >> 16) & 0x00ff;
			const int virtualKey = (const int)wParam;
			BYTE keyboardState[256];
			GetKeyboardState(keyboardState);
			WORD ascii = 0;
			const int len = ToAscii(virtualKey, keyboardScanCode, keyboardState, &ascii, 0);
			if (len == 1 && (state == RELEASED || Input::GetCharState((uint8_t)ascii) != HELD))
				OnCharEvent(mWin, (uint8_t)ascii, state);
		}
		break;
	}
	case WM_SIZE: {
		auto Width = LOWORD(lParam);
		auto Height = HIWORD(lParam);
		if (wParam == SIZE_RESTORED && mWin->GetConfig().Width == Width && mWin->GetConfig().Height == Height) {
			OnWindowStateChange(mWin, WindowState::RESUME);
		}
		else if (wParam == SIZE_MINIMIZED) {
			OnWindowStateChange(mWin, WindowState::PAUSE);
		}
		else {
			OnWindowResizeEvent(mWin, Width, Height);
		}
		break;
	}
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}

	return 0;
}

KeyCode KeyCodeFromWin32(int key) {
	using KC = KeyCode;

	if (VK_NUMPAD0 <= key && key <= VK_NUMPAD9) return (KC)(key - VK_NUMPAD0 + KC::Numpad0);
	if (VK_F1 <= key && key <= VK_F24) return (KC)(key - VK_F1 + KC::F1);

	switch (key) {
	case VK_BACK: return KC::BACKSPACE;
	case VK_TAB: return KC::TAB;
	case VK_RETURN: return KC::ENTER;
	case VK_SPACE: return KC::SPACE;
	case VK_SHIFT:
	case VK_LSHIFT: return KC::LSHIFT;
	case VK_RSHIFT: return KC::RSHIFT;
	case VK_MENU:
	case VK_LMENU: return KC::LALT;
	case VK_RMENU: return KC::RALT;
	case VK_PAUSE: return KC::BREAK;
	case VK_CAPITAL: return KC::CAPSLOCK;
	case VK_PRIOR: return KC::PAGEUP;
	case VK_NEXT: return KC::PAGEDOWN;
	case VK_END: return KC::END;
	case VK_HOME: return KC::HOME;
	case VK_ESCAPE: return KC::ESC;
	case VK_LEFT: return KC::ARROW_LEFT;
	case VK_UP: return KC::ARROW_UP;
	case VK_RIGHT: return KC::ARROW_RIGHT;
	case VK_DOWN: return KC::ARROW_DOWN;
	case VK_DELETE: return KC::DEL;
	case VK_ADD: return KC::NumpadPLUS;
	case VK_SUBTRACT: return KC::NumpadMINUS;
	case VK_MULTIPLY: return KC::NumpadMULTIPLY;
	case VK_DIVIDE: return KC::NumpadDIVIDE;
	case VK_DECIMAL: return KC::NumpadPERIOD;
	case VK_NUMLOCK: return KC::NUMLOCK;
	case VK_SCROLL: return KC::SCROLLLOCK;
	case VK_SNAPSHOT: return KC::PRINTSCREEN;
	case VK_INSERT: return KC::INSERT;
	case VK_LWIN: return KC::SUPER_LEFT;
	case VK_RWIN: return KC::SUPER_RIGHT;
	case VK_OEM_CLEAR: return KC::CLEAR;
	case VK_CONTROL:
	case VK_LCONTROL: return KC::LCTRL;
	case VK_RCONTROL: return KC::RCTRL;
	default: return KC::NONE;
	}
}
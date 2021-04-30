#pragma once

#include <GameEngine/Core/Events.hpp>
#include <GameEngine/Core/Window.hpp>
#include "InternalInput.hpp"

namespace GameEngine {

	using namespace Events;

	void OnMouseButtonEvent(Window *win, MouseButton mb, ButtonState bs) {
		InternalInput::SetMouseButtonState(mb, bs);
	}

	void OnMouseMoveEvent(Window *win, int32_t x, int32_t y) {
		InternalInput::SetMouseX(x);
		InternalInput::SetMouseY(y);
	}

	void OnKeyEvent(Window *win, KeyCode kc, ButtonState bs) {
		InternalInput::SetKeyState(kc, bs);
	}

	void OnCharEvent(Window* win, uint8_t c, ButtonState bs) {
		InternalInput::SetCharState(c, bs);
	}

	void OnWindowResizeEvent(Window *win, uint32_t width, uint32_t height) {
		const_cast<WindowConfig&>(win->GetConfig()).Width = width;
        const_cast<WindowConfig&>(win->GetConfig()).Height = height;
		win->GetRenderer()->SetViewPort({0, 0, width, height});
	}

	void OnWindowStateChange(Window *win, WindowState ws) {
		switch (ws) {
			case QUIT: {
				win->Close();
				break;
			}
			case RESUME: {
				break;
			}
			case PAUSE: {
				break;
			}
			case FOCUS_IN: {
				break;
			}
			case FOCUS_OUT: {
				break;
			}
			default: break;
		}
	}

}
#include "InternalInput.hpp"

namespace GameEngine {
	using namespace MathTypes;
	using namespace Events;

	static VecI<2> mousePos(-1, -1);
	static ButtonState mbs[MouseButton::__num_buttons__];
	static ButtonState kbs[KeyCode::__num_keys__];
	static ButtonState cbs[256];

	VecI<2> Input::GetMousePosition() {
		return mousePos;
	}
	int32_t Input::GetMouseX() { return mousePos.x; }
	int32_t Input::GetMouseY() { return mousePos.y; }

	ButtonState Input::GetMouseButtonState(MouseButton mb) {
		return mbs[mb];
	}

	ButtonState Input::GetKeyState(KeyCode kc) {
		return kbs[kc];
	}

	ButtonState Input::GetCharState(uint8_t c) {
		return cbs[c];
	}

	void InternalInput::SetMousePosition(VecI<2> v) {
		mousePos = v;
	}
	void InternalInput::SetMouseX(uint32_t x) { mousePos.x = x; }
	void InternalInput::SetMouseY(uint32_t y) { mousePos.y = y; }

	void InternalInput::SetMouseButtonState(MouseButton mb, ButtonState bs) {
		mbs[mb] = bs;
	}

	void InternalInput::SetKeyState(KeyCode kc, ButtonState bs) {
		kbs[kc] = bs;
	}

	void InternalInput::SetCharState(uint8_t c, ButtonState bs) {
		cbs[c] = bs;
	}

	void InternalInput::Update() {
		for (int i=0; i<MouseButton::__num_buttons__; i++) mbs[i] = mbs[i] == PRESSED || mbs[i] == HELD ? HELD : NONE_STATE;
		for (int i=0; i<KeyCode::__num_keys__; i++) kbs[i] = kbs[i] == PRESSED || kbs[i] == HELD ? HELD : NONE_STATE;
		for (int i=0; i<256; i++) cbs[i] = cbs[i] == PRESSED || cbs[i] == HELD ? HELD : NONE_STATE;
	}
}
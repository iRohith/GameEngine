#pragma once

#include "Core.hpp"
#include "MathTypes.hpp"
#include "Events.hpp"

namespace GameEngine {
	
	class GEAPI Input {
	public:
		static MathTypes::VecI<2> GetMousePosition();
		static int32_t GetMouseX();
		static int32_t GetMouseY();

		static Events::ButtonState GetKeyState(Events::KeyCode);
		static Events::ButtonState GetMouseButtonState(Events::MouseButton);
		static Events::ButtonState GetCharState(uint8_t);

		static inline bool IsMouseButtonPressed(Events::MouseButton mb) { return GetMouseButtonState(mb) == Events::ButtonState::PRESSED;  }		
		static inline bool IsKeyPressed(Events::KeyCode kc) { return GetKeyState(kc) == Events::ButtonState::PRESSED; }
		static inline bool IsCharPressed(uint8_t c) { return GetCharState(c) == Events::ButtonState::PRESSED; }

		static inline bool IsMouseButtonReleased(Events::MouseButton mb) { return GetMouseButtonState(mb) == Events::ButtonState::RELEASED;  }		
		static inline bool IsKeyReleased(Events::KeyCode kc) { return GetKeyState(kc) == Events::ButtonState::RELEASED; }
		static inline bool IsCharReleased(uint8_t c) { return GetCharState(c) == Events::ButtonState::RELEASED; }

		static inline bool IsMouseButtonHeld(Events::MouseButton mb) { auto s = GetMouseButtonState(mb); return s == Events::ButtonState::HELD || s == Events::ButtonState::PRESSED;  }		
		static inline bool IsKeyHeld(Events::KeyCode kc) { auto s = GetKeyState(kc); return s == Events::ButtonState::HELD || s == Events::ButtonState::PRESSED; }
		static inline bool IsCharHeld(uint8_t c) { auto s = GetCharState(c); return s == Events::ButtonState::HELD || s == Events::ButtonState::PRESSED; }

	private:
		Input() = delete;
		~Input() = delete;
	};
}
#pragma once

#include <GameEngine/Core/MathTypes.hpp>
#include <GameEngine/Core/Events.hpp>
#include <GameEngine/Core/Input.hpp>

namespace GameEngine {
	class InternalInput {
	public:
		static void SetMousePosition(MathTypes::VecI<2>);
		static void SetMouseX(uint32_t);
		static void SetMouseY(uint32_t);

		static void SetMouseButtonState(Events::MouseButton, Events::ButtonState);
		static void SetKeyState(Events::KeyCode, Events::ButtonState);
		static void SetCharState(uint8_t, Events::ButtonState);

		static void Update();
	};
}
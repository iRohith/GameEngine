#pragma once

#include "Core.hpp"
#include "stdint.h"

namespace GameEngine {
	class GEAPI Time {
	public:
		static float RunningTime();
		static float DeltaTime();
        static float FrameRate();
		
		static void Sleep(float millis);
		static void BusySleep(uint32_t micros);
	private:
		Time() = delete;
	};
}
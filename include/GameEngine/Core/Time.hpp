#pragma once

#include "Core.hpp"
#include "stdint.h"

namespace GameEngine {
	class GEAPI Time {
	public:
		static double RunningTime();
		static double DeltaTime();
        static double FrameRate();
		
		static void Sleep(double millis);
		static void BusySleep(uint32_t micros);
	private:
		Time() = delete;
	};
}
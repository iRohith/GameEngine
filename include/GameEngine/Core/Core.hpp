#pragma once

#ifndef PLATFORM_SET
#if defined(_MSC_VER) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
	#define PLATFORM_WINDOWS
	#define PLATFORM_DESKTOP
#elif defined(__APPLE__) || defined(__MACH__)
	#include <TargetConditionals.h>
	#if TARGET_IPHONE_SIMULATOR == 1
		#define PLATFORM_IOS_SIMULATOR
		#define PLATFORM_MOBILE
		#error "IOS simulator is not supported!"
	#elif TARGET_OS_IPHONE == 1
		#define PLATFORM_IOS
		#define PLATFORM_MOBILE
		#error "IOS is not supported!"
	#elif TARGET_OS_MAC == 1
		#define PLATFORM_MACOS
		#define PLATFORM_DESKTOP
		#error "MacOS is not supported!"
	#else
		#error "Unknown Apple platform!"
	#endif
#elif defined(__ANDROID__)
	#define PLATFORM_ANDROID
	#define PLATFORM_MOBILE
	#error "Android is not supported!"
#elif defined(__linux__)
	#define PLATFORM_LINUX
	#define PLATFORM_DESKTOP
#else
#error "Unknown platform!"
#endif
#endif


#ifdef PLATFORM_WINDOWS
	#ifdef ENGINE_STATIC_BUILD
    	#define GEAPI
	#else
		#ifdef GAME_ENGINE_IMPL    
			#define GEAPI __declspec(dllexport)
		#else
			#define GEAPI __declspec(dllimport)
		#endif
	#endif
#else
    #define GEAPI
#endif

#ifndef NDEBUG
    #if defined(PLATFORM_WINDOWS)
        #define GE_DEBUGBREAK() __debugbreak()
    #else
        #include <signal.h>
        #define GE_DEBUGBREAK() raise(SIGTRAP)
    #endif
#endif

#define GE_IN_CXP inline constexpr

#include <memory>

namespace GameEngine {
	template<typename T> using Ref = std::shared_ptr<T>;
}
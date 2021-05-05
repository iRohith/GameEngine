#include "GameEngine/Core/Time.hpp"
#include "GameEngine/Core/App.hpp"

#include <chrono>
#include <thread>

#ifdef PLATFORM_WINDOWS
#include <windows.h>
#include <synchapi.h>
#include <timeapi.h>
#else
#include <time.h>
#endif


namespace GameEngine {

    static inline int64_t current_time_micros() {
        return std::chrono::time_point_cast<std::chrono::microseconds>(std::chrono::system_clock::now()).time_since_epoch().count();
    }

    uint64_t initTime = 0, prevUpdateTime = 0;
    float elapsedTime, deltaTime, frameDelay;

    void _Time_Init()
    {
        if (initTime > 0) return;
        frameDelay = 1000000.0 / App::get()->GetAppConfig().frameRate;
        initTime = current_time_micros();
        prevUpdateTime = initTime;

    #ifdef PLATFORM_WINDOWS
        timeBeginPeriod(1);
    #endif
    }

    void _Time_Update(bool accurateFrameRate, float sysgranurality) {
        auto curr = current_time_micros();
        auto fdelta = frameDelay - (curr - prevUpdateTime);

        if (fdelta > 0){
            curr = current_time_micros();
            auto delay = fdelta / 1000.0 - sysgranurality;
            if (delay > 0) Time::Sleep(delay);
            curr = current_time_micros() - curr;
            if (accurateFrameRate && fdelta - curr > 0) Time::BusySleep((uint32_t)(fdelta - curr));
        }

        curr = current_time_micros();
        deltaTime = (curr - prevUpdateTime) / 1000000.0;
        elapsedTime = (curr - initTime) / 1000000.0;
        prevUpdateTime = curr;
    }

    void _Time_Finish(){
    #ifdef PLATFORM_WINDOWS
        timeEndPeriod(1);
    #endif
    }


    void GameEngine::Time::Sleep(float millis) {
    #ifdef PLATFORM_WINDOWS
        ::Sleep((uint32_t)millis);
    #else
        struct timespec tim = {(int)(millis / 1000)};
        tim.tv_nsec = (millis - tim.tv_sec/1000.0f)*1000000.0f;
        nanosleep(&tim, NULL);
    #endif
    }

    void GameEngine::Time::BusySleep(uint32_t micros) {
        auto start = std::chrono::high_resolution_clock::now();
        auto end = start + std::chrono::microseconds(micros);
        do {
            std::this_thread::yield();
        } while (std::chrono::high_resolution_clock::now() < end);
    }

    float Time::RunningTime()
    {
        return elapsedTime;
    }

    float Time::DeltaTime()
    {
        return deltaTime;
    }

    float Time::FrameRate()
    {
        return 1 / DeltaTime();
    }

}
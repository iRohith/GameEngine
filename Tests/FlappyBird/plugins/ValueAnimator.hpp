#pragma once

#include <GameEngine/Util/Util.hpp>
#include <GameEngine/Core/Time.hpp>


template<typename V, typename F> inline constexpr V lerp(V x, V y, F t) {
    return x * ((F)1 - t) + y * t;
}

template<typename T> class ValueAnimator {
public:
    ValueAnimator(){}

    ValueAnimator(
        T& val,
        const T& _start, const T& _end,
        float _duration, float delay = 0, bool _loop = false,
        GameEngine::Runnable* _onComplete = nullptr
    ) : value(&val), start(_start), end(_end), duration(_duration), loop(_loop), onComplete(_onComplete)
    {
        startTime = GameEngine::Time::RunningTime() + delay;
        endTime = startTime + duration;
    }

    void Update(){
        if (value == nullptr || paused) return;
        float currTime = GameEngine::Time::RunningTime();
        if (currTime < startTime) return;
        if (currTime > endTime) {
            *value = end;
            if (onComplete) (*onComplete)();
            if (loop){
                startTime = currTime;
                endTime = startTime + duration;
            }
        } else {
            *value = lerp(start, end, (currTime - startTime) / duration);
        }
    }

    ~ValueAnimator(){
        if (onComplete) delete onComplete;
    }

    void Pause(){
        paused = true;
        remainingTime = endTime - GameEngine::Time::RunningTime();
    }

    void Play(){
        endTime = GameEngine::Time::RunningTime() + remainingTime;
        startTime = endTime - duration;
        paused = false;
    }

    void Stop(){
        endTime = GameEngine::Time::RunningTime();
        remainingTime = duration;
    }

    void Reverse(){
        std::swap(start, end);
    }

    inline void TogglePause(){ if (paused) Play(); else Pause(); }

private:
    T* value;
    T start, end;
    float startTime, endTime, duration, remainingTime;
    bool loop, paused = false;
    GameEngine::Runnable* onComplete;
};
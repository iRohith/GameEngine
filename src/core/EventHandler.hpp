#pragma once

#include <GameEngine/Core/Window.hpp>
#include <GameEngine/Util/Util.hpp>

namespace GameEngine {
    class EventHandler {
    public:
        ~EventHandler();
        void Update();
        void RegisterWindow(Window*,void*);
        static EventHandler* Get();
    };
}
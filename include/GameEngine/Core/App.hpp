#pragma once

#include "Core.hpp"
#include "../Graphics/GraphicsContext.hpp"

namespace GameEngine {
    
    struct GEAPI AppConfig {
        const char* Name;
        float frameRate = 60.0f;
        bool tryAccurateFrameRate = false;
        WindowConfig defaultWindowConfig = {};
        GraphicsConfig gcConfig = {};
    };

    class GEAPI App {
    public:
        static App* Create(const AppConfig& c = {"MyGame"});
        ~App();

        int Run() const;
        bool Update() const;
        void Exit(int exitCode = 0);

        Window* GetDefaultWindow() const;
        Window* GetActiveWindow() const;
        Window* GetWindow(const char*) const;

        inline const AppConfig& GetAppConfig() const { return cfg; }
        inline GraphicsContext* GetGC() const { return gc; }

        static App* get();
        static bool IsAlive();
    private:
        AppConfig cfg;
        GraphicsContext* gc;

        App(const AppConfig& c = {"MyGame"});
        App(const App&) = delete;
    };
}
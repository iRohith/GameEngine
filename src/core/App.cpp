#include "GameEngine/Core/App.hpp"
#include "GELog.hpp"
#include "EventHandler.hpp"

#include <memory>

namespace GameEngine {

    extern void _Time_Init();
    extern void _Time_Update(bool accurateFrameRate, float sysgranurality);
    extern void _Time_Finish();

    extern Window* _GetDefaultWindow();
    extern Window* _GetActiveWindow();
    extern Window* _GetWindow(const char*);
    extern void _CloseAllWindows();
    
    inline void TimeUpdate(bool accurateFrameRate) {
    #ifdef PLATFORM_WINDOWS
        _Time_Update(accurateFrameRate, 1);
    #else
        _Time_Update(accurateFrameRate, 0);
    #endif
    }

    extern std::unique_ptr<App> appInstance;
    bool alive = true;
    int exitCode = 0;

    App* App::Create(const AppConfig& c) {
        if (appInstance){
            GE_LOGE("One app instance is allowed per program");
            return nullptr;
        }

        appInstance = std::unique_ptr<App>(new App(c));
        Log::init();
        _Time_Init();
        appInstance->gc = GraphicsContext::Create(c.gcConfig);

        if (appInstance->gc == nullptr) {
            GE_LOGE("Failed to create app \"{}\"", c.Name);
            appInstance.reset(nullptr);
            return nullptr;
        }

        if (Window::Create(c.defaultWindowConfig) == nullptr) {
            GE_LOGE("Failed to create app \"{}\"", c.Name);
            appInstance.reset(nullptr);
            return nullptr;
        }
        
        TimeUpdate(c.tryAccurateFrameRate);
        GE_LOGD("App \"{}\" created", c.Name);

        return appInstance.get();
    }

    App::App(const AppConfig& c) : cfg(c) {}

    App::~App(){
        _CloseAllWindows();
        if (gc) delete gc;
        _Time_Finish();
        GE_LOGD("App \"{}\" destroyed", cfg.Name);
        alive = false;
    }

    App* App::get(){
        return appInstance.get();
    }

    bool App::IsAlive(){
        return alive;
    }

    bool App::Update() const {
        if (!alive) return false;
        EventHandler::Get()->Update();
        
        TimeUpdate(cfg.tryAccurateFrameRate);
        return alive;
    }

    int App::Run() const {
        while (alive){
            App::Update();
        }
        return exitCode;
    }

    void App::Exit(int _exitCode){
        exitCode = _exitCode;
        alive = false;
    }

    Window* App::GetDefaultWindow() const{
        return _GetDefaultWindow();
    }

    Window* App::GetActiveWindow() const{
        return _GetActiveWindow();
    }

    Window* App::GetWindow(const char* name) const{
        return _GetWindow(name);
    }
}
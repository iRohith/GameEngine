#include "GameEngine/Core/App.hpp"
#include "GameEngine/Core/Log.hpp"
#include "GameEngine/Core/Time.hpp"
#include "GameEngine/Core/Input.hpp"

int test_autoloop(){
    using namespace GameEngine;

    AppConfig cfg;
    cfg.Name = "AutoLoop";
    cfg.frameRate = 30;
    cfg.tryAccurateFrameRate = true;

    App& app = *App::Create(cfg);
    return app.Run();
}

int test_manualloop(){
    using namespace GameEngine;

    AppConfig cfg;
    cfg.Name = "ManualLoop";
    cfg.frameRate = 120;
    cfg.tryAccurateFrameRate = true;

    App& app = *App::Create(cfg);

    //Window& win1 = *Window::Create({"NewWin"});

    while (app.Update() && Time::RunningTime() < 60){
        //Log::debug("Elapsed = {:.3f}, Frame rate = {:.3f}, Delta time = {:.3f}", Time::RunningTime(), Time::FrameRate(), Time::DeltaTime());
        Events::Run(CreateRunnable([](const App& app){
            if (Input::IsKeyPressed(Events::ESC) || Input::IsCharPressed('q')) app.GetActiveWindow()->Close();
            //if (Input::IsCharPressed('w')) any(app.GetWindow("NewWin"))._apply_(it, if(it) it->Close());
            if (Input::IsKeyPressed(Events::F11)) app.GetActiveWindow()->ToggleFullscreen();
            //Log::debug("A pressed = {}", Input::IsCharPressed('a') ? "true" : "false");
            if ( Input::IsCharPressed('a')) Log::debug("A pressed");
        }, app));
    }

    return 0;
}

int main(){
    return test_manualloop();
}
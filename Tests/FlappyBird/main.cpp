#include "GameEngine/Core/App.hpp"
#include "GameEngine/Core/Log.hpp"
#include "GameEngine/Core/Input.hpp"
#include "GameEngine/Core/MathTypes.hpp"
#include "plugins/SpriteRenderer.hpp"
#include "Player.hpp"
#include "Level.hpp"

glm::mat4 view_proj;

const float baseY = 50.f;

inline auto get_view_projection(float aspect){
    //view_proj = glm::ortho(size.x / 2.0f, -size.x / 2.0f, size.y / 2.0f, -size.y / 2.0f, -50.0f, 50.0f) * glm::lookAt(glm::vec3(0,0,-10), glm::vec3(0,0,0), glm::vec3(0,1,0));
    float x = aspect * baseY;
    view_proj = glm::ortho(x, -x, baseY, -baseY, -50.0f, 50.0f) * glm::lookAt(glm::vec3(0,0,-10), glm::vec3(0,0,0), glm::vec3(0,1,0));
    return view_proj;
}

glm::vec3 ScreenToWorld(const glm::vec<2,int>& _pos, const glm::vec<2,int>& wSize) {
    glm::vec2 pos(_pos);
    pos.x = (pos.x/wSize.x)*2 - 1;
    pos.y = 1 - (pos.y/wSize.y)*2;
    return glm::inverse(view_proj) * glm::vec4(pos.x,pos.y,0,1);
}

int main(){
    using namespace GameEngine;

    AppConfig cfg;
    cfg.Name = cfg.defaultWindowConfig.Title = "Flappy Bird";
    cfg.defaultWindowConfig.Width = 1024;
    cfg.defaultWindowConfig.Height = 768;
    cfg.frameRate = 60;
    cfg.tryAccurateFrameRate = false;
    cfg.gcConfig.doubleBuffered = false;
    cfg.gcConfig.vsync = true;
    
    App& app = *App::Create(cfg);
    Window& win = *app.GetDefaultWindow();
    auto prevWinSize = win.GetConfig().size();
    Renderer* renderer = win.GetRenderer();
    SpriteRenderer sr;

    Level pipes(sr);
    
    sr.GetShader()->Use();
    sr.GetShader()->SetMat4("u_ViewProjection", get_view_projection((float)prevWinSize.x / prevWinSize.y));

    while (app.Update()) {
        
        if (Input::IsKeyPressed(Events::SPACE) || Input::IsMouseButtonPressed(Events::BUTTON1)) {
            pipes.Play();
        }

        pipes.Update();
        
        win.GetRenderer()->Clear();
        sr.Update(renderer);
        app.GetGC()->SwapBuffers();

        if (Input::IsKeyPressed(Events::F11)) win.ToggleFullscreen();        
        if (Input::IsKeyPressed(Events::ESC)) win.Close();
        
        auto wSize = win.GetConfig().size();
        if (wSize != prevWinSize){
            sr.GetShader()->SetMat4("u_ViewProjection", get_view_projection((float)wSize.x / wSize.y));
            prevWinSize = wSize;
        }
        Log::debug("FrameRate = {:.0f}, DeltaTime = {:.3}", Time::FrameRate(), Time::DeltaTime());
    }

    return 0;
}
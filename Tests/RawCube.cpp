#define _USE_MATH_DEFINES
#include <cmath>
#include "GameEngine/Core/App.hpp"
#include "GameEngine/Core/Log.hpp"
#include "GameEngine/Core/Time.hpp"
#include "GameEngine/Core/Input.hpp"
#include "GameEngine/Graphics/BufferSpec.hpp"
#include "GameEngine/Graphics/Shader.hpp"
#include <math.h>
#include <glad/gl.h>

std::string_view vertexShaderSource { "#version 330 core\n"
    "#define vertex\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec3 aCol;\n"
    "uniform mat4 view_projection;\n"
    "uniform mat4 model;\n"
    "out vec4 vColor;\n"
    "void main()\n"
    "{\n"
    "   vColor = vec4(aCol,1);\n"
    "   gl_Position = view_projection * model * vec4(aPos, 1.0);\n"
    "}\0"
};
    
std::string_view fragmentShaderSource { "#version 330 core\n"
    "#define fragment\n"
    "in vec4 vColor;\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "    FragColor = vColor;\n"
    "}\0"
};

template<typename V, typename F> inline constexpr V lerp(V x, V y, F t) {
    return x * ((F)1 - t) + y * t;
}
inline constexpr float triangle_wave(float x, float a = 1.0f, float p = 1.0f){
    return glm::abs(a*M_2_PI*glm::asin(glm::sin((2*M_PI*x)/p)));
}

Array<glm::vec3> rand_vecs(size_t n){
    Array<glm::vec3> ret(n);
    for (auto& v : ret) v = {(float)rand() / RAND_MAX, (float)rand() / RAND_MAX, (float)rand() / RAND_MAX};
    return ret;
}

int main(){
    using namespace GameEngine;

    AppConfig cfg;
    cfg.Name = cfg.defaultWindowConfig.Title = "RawCube";
    cfg.defaultWindowConfig.Width = cfg.defaultWindowConfig.Height = 500;
    cfg.frameRate = 240;
    cfg.tryAccurateFrameRate = true;
    cfg.gcConfig.doubleBuffered = false;    // Double buffered has issues with gl sync (not vsync) (need to use multi threading maybe)
    cfg.gcConfig.vsync = true;
    
    App& app = *App::Create(cfg);
    Window& win = *app.GetDefaultWindow();

    auto shader = Shader::CreateFromUnits("MyShader", {
        ShaderUnit::CreateFromCode(vertexShaderSource),
        ShaderUnit::CreateFromCode(fragmentShaderSource)
    });
    
    Array<M::VecF<3>> vertices = {
                {-1, -1,  1},
                { 1, -1,  1},
                { 1,  1,  1},
                {-1,  1,  1},
                
                {-1, -1, -1},
                { 1, -1, -1},
                { 1,  1, -1},
                {-1,  1, -1}
            };

    Array<M::VecF<3>> colorsBase = rand_vecs(8);
    Array<M::VecF<3>> colorsDest = rand_vecs(8);    

    Array<M::VecF<3>> colors(colorsBase.size());
    
    Array<int> indices = {
        // front
        0, 1, 2,
		2, 3, 0,
		// right
		1, 5, 6,
		6, 2, 1,
		// back
		7, 6, 5,
		5, 4, 7,
		// left
		4, 0, 3,
		3, 7, 4,
		// bottom
		4, 5, 1,
		1, 0, 4,
		// top
		3, 2, 6,
		6, 7, 3
    };

    auto bspec = BufferSpec::Create({
        {VERTEXBUFFER, Float3, {{Float3, "aPos"}}, vertices.size()},
        {VERTEXBUFFER, Float3, {{Float3, "aCol"}}, colors.size()},
    }, indices.size());

    bspec->GetBuffers()[0]->SetData<Float3>(vertices);
    bspec->GetBuffers()[1]->SetData<Float3>(colors);
    bspec->GetIndexBuffer()->SetData<Int>(indices);
    bspec->vertexCount = indices.size();
    
    shader->Use();
    auto model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.0, -4.0));
    auto view_projection = glm::perspective(45.0f, (float)win.GetConfig().Width / win.GetConfig().Height, 1.0f, 150.0f);
    
    
    shader->Use();
    shader->SetMat4("view_projection", view_projection);

    float speed = glm::radians(90.0f);
    
    while (app.Update()){
        
        if (Input::IsKeyHeld(Events::ARROW_RIGHT) || Input::IsKeyHeld(Events::ARROW_LEFT)) {
            float angle = (Input::IsKeyHeld(Events::ARROW_RIGHT) ? speed : -speed) * Time::DeltaTime();
            model = glm::rotate(model, angle, {0, 1, 0});
        }
        
        if (Input::IsKeyHeld(Events::ARROW_UP) || Input::IsKeyHeld(Events::ARROW_DOWN)) {
            float angle = (Input::IsKeyHeld(Events::ARROW_DOWN) ? speed : -speed) * Time::DeltaTime();
            model = glm::rotate(model, angle, {1, 0, 0});
        }

        auto wave = triangle_wave(Time::RunningTime(), 1, 4);//(float)glm::abs(glm::sin(glm::radians(10*Time::RunningTime())));
        if (wave > 0.99) colorsBase = rand_vecs(8); else
        if (wave < 0.01) colorsDest = rand_vecs(8);

        for (int i=0; i<colors.size(); ++i) colors[i] = lerp(colorsBase[i], colorsDest[i], wave);
        bspec->GetBuffers()[1]->SetSubData<Float3>(colors.data(), 0, colors.size());
        
        view_projection = glm::perspective(glm::radians(60.f), (float)win.GetConfig().Width / win.GetConfig().Height, 1.0f, 150.0f);
        shader->SetMat4("view_projection", view_projection);

        shader->SetMat4("model", model);
        win.GetRenderer()->Clear();
        win.GetRenderer()->Draw(bspec);
        app.GetGC()->SwapBuffers();

        if (Input::IsKeyPressed(Events::F11)) win.ToggleFullscreen();        
        if (Input::IsKeyPressed(Events::ESC)) win.Close();
        Log::debug("FrameRate = {:.2f}, DeltaTime = {:.3}", Time::FrameRate(), Time::DeltaTime());
    }

    return 0;
}
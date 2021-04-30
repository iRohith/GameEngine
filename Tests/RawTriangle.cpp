#include "GameEngine/Core/App.hpp"
#include "GameEngine/Core/Log.hpp"
#include "GameEngine/Core/Time.hpp"
#include "GameEngine/Core/Input.hpp"
#include "GameEngine/Graphics/BufferSpec.hpp"
#include "GameEngine/Graphics/Shader.hpp"
#include <glad/gl.h>

std::string_view vertexShaderSource { "#version 330 core\n"
    "#define vertex\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec3 aCol;\n"
    "out vec4 vColor;\n"
    "void main()\n"
    "{\n"
    "   vColor = vec4(aCol,1);\n"
    "   gl_Position = vec4(aPos, 1.0);\n"
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

int main(){
    using namespace GameEngine;

    AppConfig cfg;
    cfg.Name = cfg.defaultWindowConfig.Title = "RawTriangle";
    cfg.frameRate = 120;
    cfg.tryAccurateFrameRate = true;
    
    App& app = *App::Create(cfg);
    Window& win = *app.GetDefaultWindow();

    auto shader = Shader::CreateFromUnits("MyShader", {
        ShaderUnit::CreateFromCode(vertexShaderSource),
        ShaderUnit::CreateFromCode(fragmentShaderSource)
    });
    
    Array<M::VecF<3>> vertices = {
                {-0.5f, -0.5f, 0.0f},
                { 0.5f, -0.5f, 0.0f},
                { 0.0f,  0.5f, 0.0f}
            };

    Array<M::VecF<3>> colors = {
                {1, 0, 0},
                {0, 1, 0},
                {0, 0, 1},
            };

    auto bspec = BufferSpec::Create({
        {VERTEXBUFFER, Float3, {{Float3, "aPos"}}, vertices.size()},
        {VERTEXBUFFER, Float3, {{Float3, "aCol"}}, colors.size()},
    });

    bspec->GetBuffers()[0]->SetData<Float3>(vertices);
    bspec->GetBuffers()[1]->SetData<Float3>(colors);
    bspec->vertexCount = vertices.size();

    while (app.Update()){
        shader->Use();
        win.GetRenderer()->Clear();

        win.GetRenderer()->Draw(bspec);
        app.GetGC()->SwapBuffers();

        if (Input::IsKeyPressed(Events::F11)) win.ToggleFullscreen();        
        if (Input::IsKeyPressed(Events::ESC)) win.Close();
    }

    return 0;
}

#include "GameEngine/Core/App.hpp"
#include "GameEngine/Core/Log.hpp"
#include "GameEngine/Core/Time.hpp"
#include "GameEngine/Core/Input.hpp"
#include "GameEngine/Graphics/BufferSpec.hpp"
#include "GameEngine/Graphics/Shader.hpp"
#include "GameEngine/Graphics/Texture.hpp"
#include <glad/gl.h>


std::string_view vertexShaderSource {
    "#version 330 core\n"
    "#define vertex\n"

    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec2 aTexCoord;\n"
    
    "uniform mat4 view_projection;\n"
    "uniform mat4 model;\n"
    
    "out vec2 vTexCoord;\n"
    
    "void main()\n"
    "{\n"
    "   vTexCoord = aTexCoord;\n"
    "   gl_Position = view_projection * model * vec4(aPos, 1.0);\n"
    "}\0"
};
    
std::string_view fragmentShaderSource {
    "#version 330 core\n"
    "#define fragment\n"

    "in vec2 vTexCoord;\n"
    "layout (location = 0) out vec4 color;\n"

    "uniform sampler2D mTexture[3];\n"

    "void main()\n"
    "{\n"
    "    color = texture(mTexture[1], vTexCoord);\n"
    "}\0"
};

int main(){
    using namespace GameEngine;


    AppConfig cfg;
    cfg.Name = cfg.defaultWindowConfig.Title = "RawTexturedCube";
    cfg.defaultWindowConfig.Width = cfg.defaultWindowConfig.Height = 500;
    cfg.frameRate = 120;
    cfg.tryAccurateFrameRate = false;
    cfg.gcConfig.doubleBuffered = false;    // Double buffered has issues with gl sync (not vsync) (need to use multi threading maybe)
    cfg.gcConfig.vsync = true;
    
    App& app = *App::Create(cfg);
    Window& win = *app.GetDefaultWindow();

    auto shader = Shader::CreateFromUnits("MyShader", {
        ShaderUnit::CreateFromCode(vertexShaderSource),
        ShaderUnit::CreateFromCode(fragmentShaderSource)
    });
    
    Array<M::Vec3> vertices {
                {-1, -1,  0},
                { 1, -1,  0},
                { 1,  1,  0},
                {-1,  1,  0},
            };

    Array<M::Vec2> texCoords {
                {0, 0},
                {1, 0},
                {1, 1},
                {0, 1},
            };

    Array<int> indices { 0, 1, 2,    2, 3, 0 };

    auto bspec = BufferSpec::Create({
        {VERTEXBUFFER, Float3, {{Float3, "aPos"}}, vertices.size()},
        {VERTEXBUFFER, Float2, {{Float2, "aTexCoord"}}, texCoords.size()},
    }, indices.size());

    bspec->GetBuffers()[0]->SetData<Float3>(vertices);
    bspec->GetBuffers()[1]->SetData<Float2>(texCoords);
    bspec->GetIndexBuffer()->SetData<Int>(indices);
    bspec->vertexCount = indices.size();

    
    shader->Use();
    auto model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.0, -4.0));
    auto view_projection = glm::perspective(glm::radians(60.0f), (float)win.GetConfig().Width / win.GetConfig().Height, 1.0f, 150.0f);
    auto texture = Texture2D::FromFile(asset_file("container.jpg").c_str());
    texture->Bind(1);
    Log::debug("{}, {}", texture->GetConfig().width, texture->GetConfig().height);
    
    shader->SetMat4("view_projection", view_projection);
    shader->SetInt("mTexture[1]", 1);

    float speed = M_PI;
    
    while (app.Update()){
    
        if (Input::IsKeyHeld(Events::ARROW_RIGHT) || Input::IsKeyHeld(Events::ARROW_LEFT)) {
            float angle = (Input::IsKeyHeld(Events::ARROW_RIGHT) ? speed : -speed) * Time::DeltaTime();
            model = glm::rotate(model, angle, {0, 1, 0});
        }
        
        if (Input::IsKeyHeld(Events::ARROW_UP) || Input::IsKeyHeld(Events::ARROW_DOWN)) {
            float angle = (Input::IsKeyHeld(Events::ARROW_DOWN) ? speed : -speed) * Time::DeltaTime();
            model = glm::rotate(model, angle, {1, 0, 0});
        }

        shader->SetMat4("model", model);
        win.GetRenderer()->Clear();
        win.GetRenderer()->Draw(bspec);
        app.GetGC()->SwapBuffers();

        if (Input::IsKeyPressed(Events::F11)) win.ToggleFullscreen();        
        if (Input::IsKeyPressed(Events::ESC)) win.Close();
        Log::debug("FrameRate = {:.0f}, DeltaTime = {:.3}", Time::FrameRate(), Time::DeltaTime());
    }

    return 0;
}
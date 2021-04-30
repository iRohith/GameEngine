#include <GameEngine/Core/App.hpp>
#include "../core/GELog.hpp"
#include <GameEngine/Graphics/Shader.hpp>
#include "platform/GLShader.hpp"

#include <string_view>

using namespace GameEngine;

Ref<Shader> Shader::Create(const char* name, const char* vertFilePath, const char* fragFilePath, bool addToLibrary){
#ifdef RENDERER_OPENGL
    if (App::get()->GetAppConfig().gcConfig.apiType == GameEngine::GraphicsApiType::OPENGL) {
        auto gls = new GLShader(name, vertFilePath, fragFilePath, addToLibrary);
        if (gls->status == -1){
            delete gls;
            return nullptr;
        } else return Ref<Shader>(gls);
    }
#endif
    return nullptr;
}

Ref<Shader> Shader::CreateFromUnits(const char* name, const ArrayConstView<Ref<ShaderUnit>>& s, bool addToLibrary, bool releaseShaderAfterUse){
#ifdef RENDERER_OPENGL
    if (App::get()->GetAppConfig().gcConfig.apiType == GameEngine::GraphicsApiType::OPENGL) {
        auto gls = new GLShader(name, s, addToLibrary, releaseShaderAfterUse);
        if (gls->status == -1){
            delete gls;
            return nullptr;
        } else return Ref<Shader>(gls);
    }
#endif
    return nullptr;
}

Ref<Shader> Shader::Get(const char* name){
    return nullptr;
}

Ref<ShaderUnit> ShaderUnit::Create(const char* path, ShaderType t){
#ifdef RENDERER_OPENGL
    if (App::get()->GetAppConfig().gcConfig.apiType == GameEngine::GraphicsApiType::OPENGL) {
        auto glsu = new GLShaderUnit(path, t);
        if (glsu->status == -1){
            delete glsu;
            return nullptr;
        } else return Ref<ShaderUnit>(glsu);
    }
#endif
    return nullptr;
}

Ref<ShaderUnit> ShaderUnit::CreateFromCode(const std::string_view& code, ShaderType t){
#ifdef RENDERER_OPENGL
    if (App::get()->GetAppConfig().gcConfig.apiType == GameEngine::GraphicsApiType::OPENGL) {
        auto glsu = new GLShaderUnit(code.data(), code.size(), t);
        if (glsu->status == -1){
            delete glsu;
            return nullptr;
        } else {
            return Ref<ShaderUnit>(glsu);
        }
    }
#endif
    return nullptr;
}
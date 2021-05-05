#include <GameEngine/Core/App.hpp>
#include "../core/GELog.hpp"
#include <GameEngine/Graphics/Texture.hpp>
#include "platform/GLTexture2D.hpp"
#include <stb_image.h>

using namespace GameEngine;

Ref<Texture2D> Texture2D::Create(const Texture2DConfig& cfg){
#ifdef RENDERER_OPENGL
    if (App::get()->GetAppConfig().gcConfig.apiType == GameEngine::GraphicsApiType::OPENGL) {
        return Ref<Texture2D>(new GLTexture2D(cfg));
    }
#endif
    return nullptr;
}

Ref<Texture2D> Texture2D::FromFile(const char* path) {
    int width, height, channels;
    unsigned char *data = stbi_load(path, &width, &height, &channels, 0);
    if (data){
        auto tex = Create({width, height, channels});
        tex->SetData(data);
        stbi_image_free(data);
        return tex;
    } else return nullptr;
}
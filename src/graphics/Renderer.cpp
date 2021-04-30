#include <GameEngine/Graphics/Renderer.hpp>
#include <GameEngine/Graphics/GraphicsContext.hpp>
#include "platform/GLRenderer.hpp"

namespace GameEngine {

Renderer* CreateRenderer(GraphicsApiType api){
#ifdef RENDERER_OPENGL
    if (api == GraphicsApiType::OPENGL) {
        return new GLRenderer();
    }
#endif
    return nullptr;
}

}
#pragma once

#ifdef RENDERER_OPENGL

#include <GameEngine/Graphics/Renderer.hpp>
#include <GameEngine/Graphics/Shader.hpp>
#include "GLBufferSpec.hpp"

namespace GameEngine {
    class GLRenderer : public Renderer {
    public:
        
        void SetViewPort(const M::VecI<4>& rect) {
            glViewport(rect.x, rect.y, rect.z, rect.w);            
        }

        void Clear(bool useColor, const M::VecF<4>& color) {
            if (useColor) glClearColor(color.r, color.g, color.b, color.a);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }

        void Draw(const Ref<BufferSpec>& _bs) {
            GLBufferSpec* bs = (GLBufferSpec*)(_bs.get());
            bs->enableAttributes();
            if (bs->HasIndexBuffer())
                glDrawElements(GL_TRIANGLES, bs->vertexCount, GL_UNSIGNED_INT, 0);
            else
                glDrawArrays(GL_TRIANGLES, 0, bs->vertexCount);
            bs->disableAttributes();
        }

        GLRenderer(){}
    };
}

#endif
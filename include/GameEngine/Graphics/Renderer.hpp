#pragma once

#include "../Core/Core.hpp"
#include "../Core/MathTypes.hpp"
#include "BufferSpec.hpp"

namespace GameEngine {
    class GEAPI Renderer {
    public:
        virtual void SetViewPort(const M::VecI<4>& rect) = 0;
        virtual void Clear(bool useColor = false, const M::VecF<4>& color = {0.0f, 0.0f, 0.0f, 1.0f}) = 0;
        virtual void Draw(const Ref<BufferSpec>&) = 0;
        
        virtual ~Renderer() = default;
    };
}
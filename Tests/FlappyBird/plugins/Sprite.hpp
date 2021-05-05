#pragma once

#include "Transform.hpp"
#include <GameEngine/Graphics/Texture.hpp>
#include <GameEngine/Core/Log.hpp>

namespace GameEngine {
    
    class Sprite {
    public:
        Sprite(){};
        Sprite(const std::string_view& imgFile) {
            texture = Texture2D::FromFile(imgFile.data());
            transform.scale = { AspectRatio(), 1, 1};
        }
        Sprite(Ref<Texture2D> tex) {
            SetTexture(tex);
        }

        inline Transform* GetTransform() { return &transform; }
        inline Texture2D* GetTexture() const { return texture.get(); }
        
        void SetTexture(Ref<Texture2D> tex){
            texture = tex;
            transform.scale = { AspectRatio(), 1, 1};
        }

        M::Vec4 Color {1, 1, 1, 1};
        M::Vec2 tiling {1, 1};

        inline M::VecI<2> ImageSize() const { return {texture->GetConfig().width, texture->GetConfig().height}; }
        inline float AspectRatio() const { return (float)texture->GetConfig().width / texture->GetConfig().height; }

        bool ContainsPoint(const M::Vec2& p) const {
            M::Vec2 bl = transform.TransformPoint({-0.5f, -0.5f, 0.0f});
            M::Vec2 br = transform.TransformPoint({ 0.5f, -0.5f, 0.0f});
            M::Vec2 tr = transform.TransformPoint({ 0.5f,  0.5f, 0.0f});
            M::Vec2 tl = transform.TransformPoint({-0.5f,  0.5f, 0.0f});

            if (p.x < bl.x || p.y < bl.y) return false;
            if (p.x > br.x || p.y < br.y) return false;
            if (p.x < tl.x || p.y > tl.y) return false;
            if (p.x > tr.x || p.y > tr.y) return false;
            return true;
        }

        bool enabled = true;
    protected:
        Transform transform;
        Ref<Texture2D> texture;
    };
}
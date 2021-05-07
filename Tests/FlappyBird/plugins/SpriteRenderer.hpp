#pragma once

#include "Sprite.hpp"
#include <GameEngine/Graphics/Shader.hpp>
#include <GameEngine/Graphics/BufferSpec.hpp>
#include <vector>

#define MAX_SPRITES 8

namespace GameEngine {
    class SpriteRenderer {
        Array<M::Vec3> pos = Array<M::Vec3>(MAX_SPRITES*4);
        Array<M::Vec4> col = Array<M::Vec4>(MAX_SPRITES*4);
        Array<M::Vec2> uvs = Array<M::Vec2>(MAX_SPRITES*4);
        Array<  float> tex = Array<  float>(MAX_SPRITES*4);

    public:
        SpriteRenderer(){
            if (initialized) return;
            shader = Shader::Create("SpriteShader", asset_file("shaders/sprite.vert").c_str(), asset_file("shaders/sprite.frag").c_str());
            shader->Use();
            bSpec = BufferSpec::Create({
                {VERTEXBUFFER, Float3, {{Float3, "a_Position"}}, MAX_SPRITES*4},
                {VERTEXBUFFER, Float4, {{Float4, "a_Color"   }}, MAX_SPRITES*4},
                {VERTEXBUFFER, Float2, {{Float2, "a_TexCoord"}}, MAX_SPRITES*4},
                {VERTEXBUFFER,  Float, {{ Float, "a_TexIndex"}}, MAX_SPRITES*4}
            }, MAX_SPRITES*6);

            Array<int> idBuf(MAX_SPRITES*6);
            for (int i=0,j=0; i<MAX_SPRITES*6; i+=6,j+=4){
                idBuf[i  ] = j+0;
                idBuf[i+1] = j+1;
                idBuf[i+2] = j+2;
                idBuf[i+3] = j+2;
                idBuf[i+4] = j+3;
                idBuf[i+5] = j+0;
            }
            bSpec->GetIndexBuffer()->SetData<Int>(idBuf);
            
            Array<int> samplers(MAX_SPRITES);
            for (int i=0; i<MAX_SPRITES; ++i) samplers[i] = i;
            shader->SetIntArray("u_Textures", samplers);

            sprites.emplace_back();
            //sprites[0].reserve(MAX_SPRITES);
        }

        void Update(Renderer* r){
            shader->Use();
            for (auto&& s : sprites){
                UpdateBuffers(s);
                r->Draw(bSpec);
            }
        }

        void UpdateBuffers(const std::vector<Sprite*>& sprites){
            bSpec->vertexCount = 0;

            int i=0, j=-1, prevTex = -1;
            for (Sprite* sprite : sprites) {
                if (!sprite->enabled) continue;
                sprite->GetTexture()->Bind(sprite->GetTexture()->GetID() == prevTex ? j : ++j);
                prevTex = sprite->GetTexture()->GetID();

                col[i] = col[i + 1] = col[i + 2] = col[i + 3] = sprite->Color;
                tex[i] = tex[i + 1] = tex[i + 2] = tex[i + 3] = j;

                uvs[i+0] = {0, 0};
                uvs[i+1] = {sprite->tiling[0], 0};
                uvs[i+2] = sprite->tiling;
                uvs[i+3] = {0, sprite->tiling[1]};

                pos[i++] = sprite->GetTransform()->TransformPoint({-0.5f, -0.5f, 0.0f});
                pos[i++] = sprite->GetTransform()->TransformPoint({ 0.5f, -0.5f, 0.0f});
                pos[i++] = sprite->GetTransform()->TransformPoint({ 0.5f,  0.5f, 0.0f});
                pos[i++] = sprite->GetTransform()->TransformPoint({-0.5f,  0.5f, 0.0f});

                bSpec->vertexCount += 6;
            }

            bSpec->GetBuffers()[0]->SetSubData<Float3>(pos.data(), 0, pos.size());
            bSpec->GetBuffers()[1]->SetSubData<Float4>(col.data(), 0, col.size());
            bSpec->GetBuffers()[2]->SetSubData<Float2>(uvs.data(), 0, uvs.size());
            bSpec->GetBuffers()[3]->SetSubData<Float >(tex.data(), 0, tex.size());
        }

        void Add(Sprite* s) {
            if (sprites[sprites.size()-1].size() == MAX_SPRITES) {
                sprites.emplace_back();
                //sprites[sprites.size()-1].reserve(MAX_SPRITES);
            }
            sprites[sprites.size()-1].push_back(s);
        }

        inline Ref<Shader> GetShader() const { return shader; }

    private:
        static std::vector<std::vector<Sprite*>> sprites;
        static Ref<BufferSpec> bSpec;
        static Ref<Shader> shader;
        static bool initialized;
    };

    std::vector<std::vector<Sprite*>> SpriteRenderer::sprites;
    Ref<BufferSpec> SpriteRenderer::bSpec;
    Ref<Shader> SpriteRenderer::shader;
    bool SpriteRenderer::initialized = false;
}
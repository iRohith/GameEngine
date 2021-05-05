#pragma once

#include <GameEngine/Util/Util.hpp>
#include <GameEngine/Core/Time.hpp>
#include "Sprite.hpp"

namespace GameEngine {

    class SpriteAnimator : public Sprite {
    public:
        SpriteAnimator(){}
        SpriteAnimator(const ArrayConstView<std::string_view>& imgs, float duration){
            textures.reserve(imgs.size());
            for (auto&& i : imgs) {
                textures.push_back(Texture2D::FromFile(i.data()));
            }
            currTex = 0;
            frameDelay = duration / imgs.size();
            texture = textures[0];
            transform.scale = { AspectRatio(), 1, 1};
            nextFrameTime = Time::RunningTime() + frameDelay;
        }

        virtual void Update(){
            if (paused) return;
            float currTime = Time::RunningTime();
            if (currTime > nextFrameTime) {
                currTex = (currTex+1) % textures.size();
                texture = textures[currTex];
                nextFrameTime = currTime + frameDelay;
            }
        }

    private:
        std::vector<Ref<Texture2D>> textures;
        uint32_t currTex;
        float frameDelay, nextFrameTime;
        bool paused = false;
    };

}

#pragma once

#include "plugins/SpriteRenderer.hpp"

struct Score {
    Score(){}
    Score(GameEngine::SpriteRenderer& _sr) : sr(&_sr) {
        for (int i=0; i<10; ++i){
            numbers[i] = GameEngine::Texture2D::FromFile(GameEngine::asset_file(format_str("flappy_sprites/{}.png", i)).c_str());
        }

        for (int i=0; i<4; ++i){
            digits.emplace_back(numbers[0]);
            digits[i].GetTransform()->scale *= 10;
            digits[i].GetTransform()->position = { digits[i].GetTransform()->scale.x*(0.5f - i), 40, -10 };
            digits[i].enabled = false;
        }
        
        for (int i=0; i<4; ++i) sr->Add(&digits[i]);
        digits[0].enabled = true;
    }

    void SetEnabled(bool e){
        if (e) digits[0].enabled = true; else for (auto&& d : digits) d.enabled = e;
    }

    void ResetScore(){
        SetEnabled(false);
        digits[0].enabled = true;
        SetScore(0);
    }

    void SetScore(int s){
        auto str = std::to_string(s);
        int i=0;
        for (auto c = str.rbegin(); c < str.rend(); ++c, ++i) {
            digits[i].enabled = true;
            digits[i].SetTexture(numbers[int(*c-'0')]);
            digits[i].GetTransform()->scale *= 10;
        }
        score = s;
    }

    inline void Increment(){
        SetScore(score+1);
    }

    std::vector<GameEngine::Sprite> digits;
    Array<GameEngine::Ref<GameEngine::Texture2D>> numbers = Array<GameEngine::Ref<GameEngine::Texture2D>>(10);
    GameEngine::SpriteRenderer* sr;
    int score = 0;
};
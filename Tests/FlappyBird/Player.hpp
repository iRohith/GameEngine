#pragma once

#include "plugins/SpriteAnimator.hpp"
#include "GameEngine/Core/Input.hpp"
#include "GameEngine/Core/Log.hpp"

struct Player : public GameEngine::SpriteAnimator {

    Player(){}

    Player(const std::string_view& color)
        : GameEngine::SpriteAnimator({
            GameEngine::asset_file(format_str("flappy_sprites/{}bird-downflap.png", color)),
            GameEngine::asset_file(format_str("flappy_sprites/{}bird-midflap.png" , color)),
            GameEngine::asset_file(format_str("flappy_sprites/{}bird-upflap.png"  , color)),
            GameEngine::asset_file(format_str("flappy_sprites/{}bird-midflap.png" , color))
        }, 0.5f)
    {
        transform.scale *= 5;
        transform.position.x = -50;
    }

    void AnimUpdate(){
        GameEngine::SpriteAnimator::Update();
    }

    void Update() {
        using namespace GameEngine;
        
        if (Input::IsKeyPressed(Events::SPACE) || Input::IsMouseButtonPressed(Events::BUTTON1)) {
            velocity.y = 30;
            angularVelocity = (20 - rotation) / 0.1f;
        }

        float dt = Time::DeltaTime();

        velocity.y -= gravity * dt;
        angularVelocity -= angularAcc * dt;
        rotation += angularVelocity*dt;

        if (rotation > 20) {
            angularVelocity = 0;
            rotation = 20;
        } else if (rotation < -80) {
            angularVelocity = 0;
            rotation = -80;
        }

        transform.Translate(velocity * dt);
        transform.rotation = M::QuatF({0.f, 0.f, glm::radians(rotation)});
    }

    GameEngine::M::Vec3 velocity {0, 0, 0};
    static constexpr const float gravity = 50, angularAcc = 90;
    float angularVelocity = 0, rotation = 0;
};
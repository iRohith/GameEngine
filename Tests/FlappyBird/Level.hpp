#pragma once

#include "plugins/SpriteAnimator.hpp"
#include "GameEngine/Core/Input.hpp"
#include "GameEngine/Core/Log.hpp"
#include "Player.hpp"
#include "Score.hpp"

struct LevelConfig {
    const char* player_color = "red";
    bool nightBackground = true;
    float worldSpeed = 30.f;
    
    int n_columns = 5;
    float hspacing = 40.f;
    float vspacing = 30.f;
    const char* pipe_color = "green";
};

inline float random(float min=0, float max=1) {
    return min + ((float)rand() / RAND_MAX) * (max - min);
}

struct Level {
    Level(GameEngine::SpriteRenderer& sr, const LevelConfig& _cfg = {})
        : texture(GameEngine::Texture2D::FromFile(GameEngine::asset_file(format_str("flappy_sprites/pipe-{}.png", _cfg.pipe_color)).c_str())),
          back(GameEngine::asset_file(cfg.nightBackground ? "flappy_sprites/background-night.png" : "flappy_sprites/background-day.png")),
          base(GameEngine::asset_file("flappy_sprites/base.png")),
          start(GameEngine::asset_file("flappy_sprites/message.png")),
          gameover(GameEngine::asset_file("flappy_sprites/gameover.png"))
    {

        back.tiling.x = 5;
        base.tiling.x = 5;
        base.GetTransform()->scale.x = back.GetTransform()->scale.x;
        base.GetTransform()->scale.y = 0.15f;
        base.GetTransform()->Translate({0, -0.5f + base.GetTransform()->scale.y/2, 1});
        back.GetTransform()->Translate({0, 0, 2});
        back.GetTransform()->SetParent(&t);
        base.GetTransform()->SetParent(&t);

        start.GetTransform()->scale *= 60;
        gameover.GetTransform()->scale *= 10;
        start.GetTransform()->position.z = gameover.GetTransform()->position.z = -5;

        sr.Add(&base);
        sr.Add(&back);
        sr.Add(&start);
        sr.Add(&gameover);

        t.scale = {back.tiling.x*100, 100, 1};
        width = 100*back.AspectRatio();

        pipes = Array<GameEngine::Sprite>(cfg.n_columns*2);
        pipeCrossed = Array<bool>(pipes.size(), false);

        for (int i=0; i<pipes.size(); ++i) {
            pipes[i] = GameEngine::Sprite(texture);
            GameEngine::Transform* t = pipes[i].GetTransform();
            t->scale *= 100;
            t->position.z = -1;
            if (i % 2) {
                t->scale.y *= -1;
            }
            sr.Add(&pipes[i]);
        }

        player = Player(cfg.player_color);
        playerMinY = -50 + player.GetTransform()->scale.y / 2 + 15;
        playerMaxY =  50 - player.GetTransform()->scale.y / 2;
        sr.Add(&player);

        score = Score(sr);

        Reset();

        start.enabled = true;
        gameover.enabled = false;
        score.SetEnabled(false);
    }

    void Update(){
        if (gameover.enabled) {
            if (GameEngine::Time::RunningTime() > gameOverRemoveTime){
                gameover.enabled = false;
                start.enabled = true;
                score.SetEnabled(false);
                Reset();
            }
            return;
        }

        player.AnimUpdate();
        if (!play) return;
        player.Update();
        
        velocity.x = -cfg.worldSpeed*GameEngine::Time::DeltaTime();
        t.Translate(velocity);
        if (t.position.x < -width/2) t.position.x = width/2;

        GameEngine::Transform* pt = player.GetTransform();

        for (int i=0; i<pipes.size(); ++i){
            GameEngine::Transform* t = pipes[i].GetTransform();
            t->Translate(velocity);
            
            if (i % 2) {
                if (pt->position.x + pt->scale.x/3 > t->position.x - t->scale.x/2 && pt->position.x - pt->scale.x/3 < t->position.x + t->scale.x/2) {
                    if (pt->position.y + pt->scale.y/3 > t->position.y + t->scale.y/2 || pt->position.y - pt->scale.y/3 < pipes[i-1].GetTransform()->position.y - t->scale.y/2) {
                        gameover.enabled = true;
                        gameOverRemoveTime = GameEngine::Time::RunningTime() + 2;
                    }
                }

                if (!pipeCrossed[i] && pt->position.x - pt->scale.x/3 > t->position.x + t->scale.x/2) {
                    pipeCrossed[i] = true;
                    score.Increment();
                }
            }

            if (t->position.x < -100) {
                t->position.x = 100;
                pipeCrossed[i] = false;
            }
        }

        if (pt->position.y < playerMinY || pt->position.y > playerMaxY) {
            gameover.enabled = true;
            gameOverRemoveTime = GameEngine::Time::RunningTime() + 2;
        }
    }

    void Play(){
        if (play) return;

        start.enabled = false;
        gameover.enabled = false;
        score.SetEnabled(true);

        play = true;
    }

    void Pause(){
        if (!play) return;
        play = false;
    }

    void Reset(){
        float hloc = 100, r;
        
        for (int i=0; i<pipes.size(); ++i) {
            GameEngine::Transform* t = pipes[i].GetTransform();
            t->position.x = hloc + t->scale.x;
            if (i % 2) {
                t->position.y = -(t->scale.y / 2) + (cfg.vspacing / 2) + r;
                hloc += cfg.hspacing;
            } else {
                r = random(-cfg.vspacing/2, cfg.vspacing/2);
                t->position.y =  -(t->scale.y / 2) - (cfg.vspacing / 2) + r;
            }
        }

        player.GetTransform()->position.y = 0;
        player.GetTransform()->rotation = glm::quat({0.f, 0.f, 0.f});
        player.rotation = 0;
        player.angularVelocity = 0;
        player.velocity.y = 0;

        play = false;
        score.ResetScore();
        score.SetEnabled(false);
    }

    GameEngine::Ref<GameEngine::Texture2D> texture;
    Array<GameEngine::Sprite> pipes;
    Array<bool> pipeCrossed;
    GameEngine::M::Vec3 velocity {0, 0, 0};
    bool play = false;
    Player player;

    GameEngine::Transform t;
    GameEngine::Sprite back, base, start, gameover;
    float width, gameOverRemoveTime;
    
    float playerMinY = 0, playerMaxY = 0;

    Score score;

    LevelConfig cfg;
};
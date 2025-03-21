#pragma once

#include <raylib.h>
#include <raymath.h>

#include "SceneManager.h"
#include "GameData.cpp"
#include "Game.cpp"

class MainMenuScene : public Scene {
    private:
        int lastScore;
        int highScore;
        float angle = 0;
        Vector2 center = (Vector2){ 400, 300 };
        int radius = 200;
        GameDataScene* data = dynamic_cast<GameDataScene*>(SCENE_MANAGER.getScene("Data"));
    public:
        MainMenuScene() : Scene("Main Menu") {}
        void update() override { 
            if (IsKeyDown(KEY_G)) SCENE_MANAGER.setActiveScene("Game");
            angle = angle += GetFrameTime();
        }
        void draw() override {
            ClearBackground(RAYWHITE);
            DrawCircle(center.x, center.y, radius, RED);
            DrawCircle(center.x + cos(angle) * radius, center.y + sin(angle) * radius, 6, BLACK);

            DrawText("Untitled Space Game", 12, 12, 42, BLACK);
            DrawText("Press G to Start!", 12, 324, 24, BLACK);


            DrawText("Controls:", 12, 200, 16, BLACK);
            DrawText("Left & Right Arrows - Movement", 12, 218, 16, BLACK);
            DrawText("Space - Jump", 12, 236, 16, BLACK);
            DrawText("Mouse - Select Land", 12, 254, 16, BLACK);
            DrawText("Up & Down Arrows - Mine", 12, 272, 16, BLACK);

            DrawText("Highscore:", 12, 54, 16, BLACK);
            DrawText(TextFormat("%i", data->getHighScore()), 12, 72, 32, BLACK);
            DrawText("Last Score:", 12, 104, 16, BLACK);
            DrawText(TextFormat("%i", data->getLastScore()), 12, 122, 32, BLACK);
        }
        void onOpen() override {
            SCENE_MANAGER.addScene(std::make_unique<GameScene>());
        }
        void setLastScore(int score) {
            lastScore = score;
        }
};
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
        GameDataScene* data = dynamic_cast<GameDataScene*>(SCENE_MANAGER.getScene("Data"));
    public:
        MainMenuScene() : Scene("Main Menu") {}
        void update() override { 
            if (IsKeyDown(KEY_G)) SCENE_MANAGER.setActiveScene("Game");
        }
        void draw() override {
            ClearBackground(RAYWHITE);
            DrawText("Untitled Space Game", 12, 12, 42, BLACK);
            DrawText("Press G", 12, 332, 16, BLACK);

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
#pragma once

#include <raylib.h>
#include <raymath.h>

#include "SceneManager.h"
#include "Game.cpp"

class MainMenuScene : public Scene {
    private:
        int lastScore;
        int highScore;
    public:
        MainMenuScene() : Scene("Main Menu") {}
        void update() override { 
            if (IsKeyDown(KEY_G)) SCENE_MANAGER.setActiveScene("Game");
        }
        void draw() override {
            ClearBackground(RAYWHITE);
            DrawText("Untitled Space Game", 12, 12, 42, BLACK);
            DrawText("Press G", 12, 332, 16, BLACK);
        }
        void onOpen() override {
            SCENE_MANAGER.addScene(std::make_unique<GameScene>());
        }
        void setLastScore(int score) {
            lastScore = score;
        }
};
#pragma once

#include "SceneManager.h"
#include <raylib.h>
#include <raymath.h>

#include "GameData.cpp"

#include "Planet.h"
#include "Player.h"

class GameScene : public Scene {
    private:
        Camera2D camera = { 0 };
        PlanetSystem planetSystem;
        Player player = {planetSystem, camera, (Vector2){ 1300.0f, 1300.0f }};
        int points = 0;
        float timer = 20;
    public:
        GameScene() : Scene("Game", DELETE_ON_CLOSE) {
            camera.zoom = 1.0f;
            camera.offset = (Vector2){ (float)GetScreenWidth() / 2, (float)GetScreenHeight() / 2 };
        
            planetSystem.computeField();
        }
        void update() override { 
            if (IsKeyDown(KEY_M)) SCENE_MANAGER.setActiveScene("Main Menu");
            player.update();
            timer = timer - GetFrameTime();
            if (timer < 0) {
                SCENE_MANAGER.setActiveScene("Main Menu");
                timer = 0;
            }
        }
        void draw() override {
            ClearBackground(RAYWHITE);
			
			// Camera
			BeginMode2D(camera);
				planetSystem.drawField(camera);
				planetSystem.draw();
				player.draw();
				
			EndMode2D();

			// UI
			DrawText("Time Left:", 12, 12, 16, BLACK);
            DrawText(TextFormat("%.2f", timer), 12, 30, 28, BLACK);
            DrawText("Points:", 12, 60, 16, BLACK);
            DrawText(TextFormat("%i", points), 12, 78, 28, BLACK);
        }
        void addPoints(int value) { points = points + value; };
        void onClose() override {
            GameDataScene* data = dynamic_cast<GameDataScene*>(SCENE_MANAGER.getScene("Data"));
            data->setLastScore(points);
        }
};
#include "SceneManager.h"

#include <raylib.h>
#include <raymath.h>

#include "Planet.h"
#include "Player.h"

class GameScene : public Scene {
    private:
        Camera2D camera = { 0 };
        PlanetSystem planetSystem;
        Player player = {planetSystem, camera, (Vector2){ 1300.0f, 1300.0f }};
    public:
        GameScene() : Scene("Game") {
            camera.zoom = 1.0f;
            camera.offset = (Vector2){ (float)GetScreenWidth() / 2, (float)GetScreenHeight() / 2 };
        
            planetSystem.computeField();
        }
        void update() override { 
            if (IsKeyDown(KEY_M)) SCENE_MANAGER.setActiveScene("Main Menu");
            player.update();
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
			DrawFPS(12, 12);
			Vector2 playerPos = player.getPosition();
			DrawText(TextFormat("Player: %f %f", playerPos.x, playerPos.y), 12, 36, 16, BLACK);
            DrawText("Press M", 12, 50, 16, BLACK);
        }
};
#include <raylib.h>
#include <raymath.h>

#include "SceneManager.h"

class MainMenuScene : public Scene {
    private:

    public:
        MainMenuScene() : Scene("Main Menu") {}
        void update() override { 
            if (IsKeyDown(KEY_G)) SCENE_MANAGER.setActiveScene("Game");
        }
        void draw() override {
            ClearBackground(RAYWHITE);
			
            DrawText("Press G", 12, 12, 16, BLACK);
        }
};
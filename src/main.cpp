#include <raylib.h>
#include <raymath.h>
#include <string>

#include "SceneManager.h"
#include "Game.cpp"
#include "MainMenu.cpp"

SceneManager SCENE_MANAGER;

int main()
{
	SetConfigFlags(FLAG_VSYNC_HINT);
	InitWindow(480, 360, "Nik's Game");
	SetTargetFPS(60);

	SCENE_MANAGER.addScene(std::make_unique<GameScene>());
	SCENE_MANAGER.addScene(std::make_unique<MainMenuScene>());
	SCENE_MANAGER.setActiveScene("Game");
	
	while (!WindowShouldClose()) {
		// Update
		SCENE_MANAGER.update();

		// Rendering
		BeginDrawing();
			SCENE_MANAGER.draw();
		EndDrawing();

		// Scene changes handled
		SCENE_MANAGER.handleSceneChanges();
	}

	CloseWindow();
	return 0;
}

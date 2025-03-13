#include <raylib.h>
#include <raymath.h>
#include <string>

#include "Planet.h"
#include "Player.h"

int main()
{
	SetConfigFlags(FLAG_VSYNC_HINT);
	InitWindow(480, 360, "Nik's Game");
	SetTargetFPS(60);

	Planet* planetPointer = new Planet(50, (Vector2){240, 180});
	Planet* planetPointer2 = new Planet(200, (Vector2){0, 0});

	Camera2D camera = { 0 };
	camera.zoom = 1.0f;
	camera.offset = (Vector2){ (float)GetScreenWidth() / 2, (float)GetScreenHeight() / 2 };

	Player player(planetPointer2, camera);
	
	while (!WindowShouldClose()) {
		// Update
		player.update();


		// Rendering
		BeginDrawing();
			ClearBackground(RAYWHITE);
			
			// Camera
			BeginMode2D(camera);
				planetPointer->draw();
				planetPointer2->draw();
				player.draw();

				Vector2 playerPos = player.getPosition();
				
				
			EndMode2D();

			// UI
			DrawFPS(12, 12);
			DrawText(TextFormat("Player: %f %f", playerPos.x, playerPos.y), 12, 36, 16, BLACK);

		EndDrawing();
	}

	delete planetPointer;
	CloseWindow();
	return 0;
}

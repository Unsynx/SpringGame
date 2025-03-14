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

	Camera2D camera = { 0 };
	camera.zoom = 1.0f;
	camera.offset = (Vector2){ (float)GetScreenWidth() / 2, (float)GetScreenHeight() / 2 };

	PlanetSystem planetSystem;
	planetSystem.computeField();

	Player player(planetSystem, camera, (Vector2){ 150.0f, 150.0f });
	
	while (!WindowShouldClose()) {
		// Update
		player.update();

		// Rendering
		BeginDrawing();
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

		EndDrawing();
	}

	CloseWindow();
	return 0;
}

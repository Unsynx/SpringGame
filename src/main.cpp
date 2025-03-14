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

	PlanetSystem planetSystem;
	planetSystem.computeField();

	Player player(planetPointer2, camera, Vector2AddValue(planetPointer2->planetToWorldCords(0).position, 20));
	
	while (!WindowShouldClose()) {
		// Update
		player.update();

		// Rendering
		BeginDrawing();
			ClearBackground(RAYWHITE);
			
			// Camera
			BeginMode2D(camera);
				planetSystem.draw();
				planetPointer->draw();
				planetPointer2->draw();
				player.draw();


				planetSystem.drawField(camera);
				
			EndMode2D();

			// UI
			DrawFPS(12, 12);
			Vector2 playerPos = player.getPosition();
			DrawText(TextFormat("Player: %f %f", playerPos.x, playerPos.y), 12, 36, 16, BLACK);

		EndDrawing();
	}

	delete planetPointer;
	CloseWindow();
	return 0;
}

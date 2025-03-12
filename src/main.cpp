#include <raylib.h>
#include <raymath.h>
#include <string>

#include "Planet.h"

int main()
{
	SetConfigFlags(FLAG_VSYNC_HINT);
	InitWindow(480, 360, "Nik's Game");
	SetTargetFPS(60);

	Planet* planetPointer = new Planet(50, (Vector2){240, 180});
	Planet* planetPointer2 = new Planet(200, (Vector2){0, 0});

	Vector2 player = { 0, 0 };
	Camera2D camera = { 0 };
	camera.target = player;
	camera.zoom = 1.0f;


	
	while (!WindowShouldClose()) {
		// Update
		if (IsKeyDown(KEY_RIGHT)) player.x += 2;
        else if (IsKeyDown(KEY_LEFT)) player.x -= 2;
		if (IsKeyDown(KEY_UP)) player.y -= 2;
        else if (IsKeyDown(KEY_DOWN)) player.y += 2;

		camera.target = player;


		// Rendering
		BeginDrawing();
			ClearBackground(RAYWHITE);
			DrawFPS(12, 12);
			BeginMode2D(camera);

			planetPointer->draw();
			planetPointer2->draw();

			EndMode2D();

		EndDrawing();
	}

	delete planetPointer;
	CloseWindow();
	return 0;
}

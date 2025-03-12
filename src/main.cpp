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
	
	while (!WindowShouldClose()) {

		BeginDrawing();
			ClearBackground(RAYWHITE);
			DrawFPS(12, 12);
			planetPointer->draw();
			planetPointer2->draw();

		EndDrawing();
	}

	delete planetPointer;
	CloseWindow();
	return 0;
}

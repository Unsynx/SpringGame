#include <raylib.h>
#include <raymath.h>
#include <string>

#include "Planet.h"


int main()
{
	SetConfigFlags(FLAG_VSYNC_HINT);
	InitWindow(480, 360, "Nik's Game");
	SetTargetFPS(20);

	Planet* planetPointer = new Planet(50, (Vector2){240, 180});
	Planet* planetPointer2 = new Planet(200, (Vector2){0, 0});
	
	while (!WindowShouldClose()) {

		BeginDrawing();
			ClearBackground(RAYWHITE);
			DrawFPS(12, 12);
			planetPointer->draw();
			planetPointer2->draw();
			planetPointer->changeOffset(0, 1);

		EndDrawing();
	}

	delete planetPointer;
	CloseWindow();
	return 0;
}

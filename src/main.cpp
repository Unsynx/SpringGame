#include <raylib.h>
#include <raymath.h>
#include <string>

#include "Planet.h"


int main()
{
	SetConfigFlags(FLAG_VSYNC_HINT);
	InitWindow(480, 360, "Nik's Game");
	SetTargetFPS(60);

	Planet* planetPointer = new Planet(5, 100);
	
	while (!WindowShouldClose()) {

		BeginDrawing();
			ClearBackground(RAYWHITE);
			DrawFPS(12, 12);
			planetPointer->draw();

		EndDrawing();
	}

	delete planetPointer;
	CloseWindow();
	return 0;
}

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

		Vector2 worldMousePos = GetScreenToWorld2D(GetMousePosition(), camera);
		Segment seg = planetPointer2->getHoveredSegment(worldMousePos);

		// Terraforming
		int change = IsKeyDown(KEY_UP) - IsKeyDown(KEY_DOWN);
		if (change) {
			planetPointer2->changeOffset(seg.startNode, change);
			planetPointer2->changeOffset(seg.endNode, change);
		}

		// Rendering
		BeginDrawing();
			ClearBackground(RAYWHITE);
			
			// Camera
			BeginMode2D(camera);
				planetPointer->draw();
				planetPointer2->draw();
				player.draw();

				// Selection visual
				DrawLineEx(seg.node1, seg.node2, 3, BLUE);
				DrawCircle(seg.node1.x, seg.node1.y, 3, BLUE);
				DrawText(TextFormat("%i", seg.startNode), seg.node1.x, seg.node1.y, 16, GREEN);
				DrawCircle(seg.node2.x, seg.node2.y, 3, BLUE);
				DrawText(TextFormat("%i", seg.endNode), seg.node2.x, seg.node2.y, 16, BLACK);

				planetSystem.drawField();
				
			EndMode2D();

			// UI
			DrawFPS(12, 12);
			Vector2 playerPos = player.getPosition();
			DrawText(TextFormat("Player: %f %f", playerPos.x, playerPos.y), 12, 36, 16, BLACK);
			DrawText(TextFormat("Mouse: %f %f", worldMousePos.x, worldMousePos.y), 12, 50, 16, BLACK);

			bool mouseColliding = planetPointer2->isColliding(worldMousePos);
			DrawText(TextFormat("Mouse Collision: %i", mouseColliding), 12, 64, 16, BLACK);

		EndDrawing();
	}

	delete planetPointer;
	CloseWindow();
	return 0;
}

#include <raylib.h>
#include <raymath.h>

int main()
{
	InitWindow(400, 224, "Nik's Game");
	
	while (!WindowShouldClose()) {
        	BeginDrawing();
            	ClearBackground(RAYWHITE);
                DrawText("Hello World!", 12, 12, 20, BLACK);
    
        	EndDrawing();
    	}
   	 CloseWindow();
   	 return 0;
}

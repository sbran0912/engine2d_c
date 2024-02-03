#include "e2d_phys.h"

int main() {

    struct _shape box = createBox(50, 50, 150, 150);
    //ApplyForce(&box, (Vector2){300.0f, 0.0f}, 10.0f);




    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 1200;
    const int screenHeight = 800; 

    InitWindow(screenWidth, screenHeight, "raylib");
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        BeginDrawing();

        ClearBackground(RAYWHITE);
        draw(&box, 3, RED);
        update(&box);
        Vector2 kick = checkKicking(&box);
        if (kick.x != 0) {
            printf("Force = %f %f \n", kick.x, kick.y);    
        }

    	EndDrawing();
    }

    // De-Initialization
    CloseWindow();                  // Close window and OpenGL context
	return 0;


    return EXIT_SUCCESS;
}

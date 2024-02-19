#include "e2d_phys.h"
#define MAX 4

int main() {
    Shape shapes[MAX];
    shapes[0] = e2_Box(150, 150, 250, 100);
    shapes[1] = e2_Box(160, 400, 150, 150);
    shapes[2] = e2_Ball(700, 150, 40);
    shapes[3] = e2_Ball(800, 400, 40);
    e2_applyForce(&shapes[0], (Vector2){0, 0}, 5);
    shapes[0].mass = INFINITY;

    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 1200;
    const int screenHeight = 800; 

    InitWindow(screenWidth, screenHeight, "raylib");
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second

    // Main game loop
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        for (size_t i = 0; i < MAX; i++) {
            for (size_t j = i + 1; j < MAX; j++) {
                e2_checkColl(&shapes[i], &shapes[j]);
            }
            Vector2 force = e2_checkKick(&shapes[i]);
            e2_applyForce(&shapes[i], force, 0);
            e2_shapeUpdate(&shapes[i]);
            e2_shapeDraw(&shapes[i], 3, RED);
        }
        //printf("%f\n", shapes[0].velocity.y);
    	EndDrawing();
    }

    CloseWindow();   
	return 0;


    return EXIT_SUCCESS;
}

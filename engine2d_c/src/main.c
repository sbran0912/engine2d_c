#include "e2d_phys.h"
#define MAX 4

int main() {
    Shape shapes[MAX];
    shapes[0] = createBox(150, 150, 250, 100);
    shapes[1] = createBox(160, 400, 150, 150);
    shapes[2] = createBall(700, 150, 40);
    shapes[3] = createBall(800, 400, 40);
    applyForce(&shapes[0], (Vector2){0, 0}, 5);
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
                checkCollision(&shapes[i], &shapes[j]);
            }
            Vector2 force = checkKicking(&shapes[i]);
            applyForce(&shapes[i], force, 0);
            update(&shapes[i]);
            draw(&shapes[i], 3, RED);
        }
        //printf("%f\n", shapes[0].velocity.y);
    	EndDrawing();
    }

    CloseWindow();   
	return 0;


    return EXIT_SUCCESS;
}

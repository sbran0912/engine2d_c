#include <stdio.h>
#include <tuple>
#include "rl2d_core.h"
#include "rl2d_phys.h"

int main() {

	Vec2d p = Vec2d(5, 7);
	Vec2d c = Vec2d(3, 3);
	auto d = c.copy();

	printf("Vector p.pos.x %f p.pos.y %f\n\n", p.pos.x, p.pos.y);
	printf("Vector c.pos.x %f c.pos.y %f\n\n", c.pos.x, c.pos.y);
	printf("Vector d.pos.x %f d.pos.y %f\n\n", d.pos.x, d.pos.y);
	c.set(4, 4);
	printf("Nach set: Vector c.pos.x %f c.pos.y %f\n", c.pos.x, c.pos.y);
	printf("Vector d.pos.x %f d.pos.y %f\n\n", d.pos.x, d.pos.y);

	Matrix3x1 p_rotated = core::rotate(p, c, 0.2f);
	printf("Bevor Drehung %f %f\n", p.pos.x, p.pos.y);
	printf("Nach Drehung %f %f\n", p_rotated.index[0], p_rotated.index[1]);
	printf("Nach Drehung als Vector p.x %f p.y %f\n\n", core::matrix3x1ToVec(p_rotated).pos.x, core::matrix3x1ToVec(p_rotated).pos.y);
	
	Vec2d a1(20, 10);
	Vec2d a2(200,200);
	Vec2d b1(0, 100);
	Vec2d b2(300, 100);

	std::tuple<float, Vec2d> sp = core::intersect(a1, a2, b1, b2);
	printf("Schnitt: %f %f ", std::get<1>(sp).pos.x, std::get<1>(sp).pos.y);
	printf("Laenge: %f \n\n", std::get<0>(sp));
	printf("Distanz von a1 zu line_b: %f\n\n", core::minDist(a1, b1, b2));
	
	float myNum = -0.8;
	printf("myNum: %f nach limitNum %f \n\n", myNum, core::limitNum(myNum, 0.5));

	Box box{ 400, 200, 150, 80 };
	Ball ball{ 400, 350, 50 };
	unsigned char lifetime = 255;

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
		DrawCircleV(a1.pos,3, Color{ 112, 31, 126, 255 });
		DrawLineV(a1.pos, a2.pos, BLACK);
		DrawLineV(b1.pos, b2.pos, RED);

		box.draw(3, Color{ 112, 31, 126, lifetime });
		ball.draw(2, Color{ 0, 0, 0, lifetime });
		//DrawCircleGradient(500, 500, 50, RED, DARKGREEN);
		//DrawRing(box.location.pos, 100, 104, 0, 180, 1, DARKGREEN);
		ball.rotate(0.005f);
		lifetime--;
		if (lifetime < 0) lifetime = 255;

		//box.rotate(0.05);
		Vec2d kick = box.checkKicking();
		if (kick.mag() > 0) printf("Kick-Stärke: %f\n", kick.mag());
    	EndDrawing();
    }

    // De-Initialization
    CloseWindow();                  // Close window and OpenGL context
	return 0;
}
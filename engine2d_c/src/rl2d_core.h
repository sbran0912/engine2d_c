#ifndef CORE_H
#define CORE_H
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "raylib.h"

typedef struct Matrix3x3 {
	float index[3][3];
} Matrix3x3;

typedef struct Matrix3x1 {
	float index[3];
} Matrix3x1;

Vector2 createVec(float x, float y);
Vector2 copyVec(Vector2 v);

/*
void set(float x, float y);
void mult(float n);
void div(float n);
void add(Vec2d v);
void sub(Vec2d v);
float dot(Vec2d v);
float cross(Vec2d v);
void normalize();
void limit(float max);
float magsq();
float mag();
void setmag(float magnitude);
float distance(Vec2d v);
void rotate(Vec2d base, float n);
Vector2 perp();
Vector2 addVec(Vec2d v1, Vec2d v2);
Vector2 subVec(Vec2d v1, Vec2d v2);
Vector2 multVec(Vec2d v, float n);
Vector2 divVec(Vec2d v, float n);
void DrawArrow(Vec2d v_base, Vec2d v_target, Color c);
*/

int random(int range_start, int range_end);

#endif
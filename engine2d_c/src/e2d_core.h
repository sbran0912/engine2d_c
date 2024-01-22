#ifndef CORE_H
#define CORE_H
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include "raylib.h"

typedef struct {
	float index[3][3];
} Matrix3x3;

typedef struct {
	float index[3];
} Matrix3x1;

typedef struct {
	float distance;
	Vector2 point;
} Intersection;

int e2Random(int range_start, int range_end);
float e2LimitNum(float number, float limit);
void e2DrawArrow(Vector2 v_base, Vector2 v_target, Color c);

Matrix3x1 e2VecToMatrix(Vector2 point);
Vector2 e2MatrixToVec(Matrix3x1 matrix);
Matrix3x1 e2MultMatrix(Matrix3x3 a, Matrix3x1 b);
Matrix3x1 e2RotateMatrix(Vector2 point, Vector2 center, float angel);

Vector2 e2CreateVec(float x, float y);
void e2SetV(Vector2* v, float x, float y);
void e2ScaleV(Vector2* v, float n);
Vector2 e2Scale(Vector2 v, float n);
void e2DivV(Vector2* v, float n);
Vector2 e2Add(Vector2 v, Vector2 v_add);
Vector2 e2Sub(Vector2 v, Vector2 v_sub);
void e2NormalizeV(Vector2* v);
void e2LimitV(Vector2* v, float max);
void e2SetMagV(Vector2* v, float magnitude);
float e2Dot(Vector2 v1, Vector2 v2);
float e2Cross(Vector2 v1, Vector2 v2);
Vector2 e2Perp(Vector2 v);
float e2Mag(Vector2 v);
float e2Magsq(Vector2 v);
float e2Distance(Vector2 v1, Vector2 v2);
Vector2 e2Rotate(Vector2 v, Vector2 base, float n);

Intersection e2Intersect(Vector2 start_a, Vector2 end_a, Vector2 start_b, Vector2 end_b);
float e2MinDist(Vector2 p, Vector2 start_a, Vector2 end_a);


#endif
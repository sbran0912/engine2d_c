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

int random(int range_start, int range_end);
float limitNum(float number, float limit);
void drawArrow(Vector2 v_base, Vector2 v_target, Color c);

Matrix3x1 transVecToMatrix(Vector2 point);
Vector2 transMatrixToVec(Matrix3x1 matrix);
Matrix3x1 multMatrix(Matrix3x3 a, Matrix3x1 b);
Matrix3x1 rotateMatrix(Vector2 point, Vector2 center, float angel);

Vector2 createVec(float x, float y);
void vecSet(Vector2* v, float x, float y);
void vecScale(Vector2* v, float n);
Vector2 scaleVec(Vector2 v, float n);
void vecDiv(Vector2* v, float n);
Vector2 addVec(Vector2 v, Vector2 v_add);
Vector2 subVec(Vector2 v, Vector2 v_sub);
Vector2 divVec(Vector2 v, float n);
void vecNormalize(Vector2* v);
void vecLimit(Vector2* v, float max);
void vecSetMag(Vector2* v, float magnitude);
float dotVec(Vector2 v1, Vector2 v2);
float crossVec(Vector2 v1, Vector2 v2);
Vector2 perpVec(Vector2 v);
float magVec(Vector2 v);
float magsqVec(Vector2 v);
float distanceVec(Vector2 v1, Vector2 v2);
Vector2 rotateVec(Vector2 v, Vector2 base, float n);

Intersection intersectVec(Vector2 start_a, Vector2 end_a, Vector2 start_b, Vector2 end_b);
float minDistVec(Vector2 p, Vector2 start_a, Vector2 end_a);


#endif
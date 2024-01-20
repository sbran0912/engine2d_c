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


int Random(int range_start, int range_end);
float LimitNum(float number, float limit);
void DrawArrow(Vector2 v_base, Vector2 v_target, Color c);

Matrix3x1 VecToMatrix(Vector2 point);
Vector2 MatrixToVec(Matrix3x1 matrix);
Matrix3x1 MultMatrix(Matrix3x3 a, Matrix3x1 b);
Matrix3x1 RotateMatrix(Vector2 point, Vector2 center, float angel);

Vector2 vecCreate(float x, float y);
void vSet(Vector2* v, float x, float y);
void vScale(Vector2* v, float n);
void vDiv(Vector2* v, float n);
Vector2 vecAdd(Vector2 v, Vector2 v_add);
Vector2 VecSub(Vector2 v, Vector2 v_sub);
void vNorm(Vector2* v);
void vLimit(Vector2* v, float max);
void vSetMag(Vector2* v, float magnitude);
float vecDot(Vector2 v1, Vector2 v2);
float vecCross(Vector2 v1, Vector2 v2);
Vector2 vecPerp(Vector2 v);
float vecMag(Vector2 v);
float vecMagsq(Vector2 v);
float vecDist(Vector2 v1, Vector2 v2);
Vector2 vecRotate(Vector2 v, Vector2 base, float n);

/*

*/

#endif
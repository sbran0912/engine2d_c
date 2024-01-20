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
Matrix3x1 VecToMatrix(Vector2 point);
Vector2 MatrixToVec(Matrix3x1 matrix);
Matrix3x1 MultMatrix(Matrix3x3 a, Matrix3x1 b);
Matrix3x1 RotateMatrix(Vector2 point, Vector2 center, float angel);

Vector2 createVec(float x, float y);
Vector2 copyVec(Vector2 v);
void setV(Vector2* v, float x, float y);
void multV(Vector2* v, float n);
void divV(Vector2* v, float n);
void addV(Vector2* v, Vector2 v_add);
void subV(Vector2* v, Vector2 v_sub);
void normalizeV(Vector2* v);
void limitV(Vector2* v, float max);
void setmagV(Vector2* v, float magnitude);
void rotateV(Vector2* base, float n);

float dotV(Vector2 v1, Vector2 v2);
float magV(Vector2 v);
float magsqV(Vector2 v);
/*


float cross(Vec2d v);
float mag();

float distance(Vec2d v);

Vector2 perp();
Vector2 addVec(Vec2d v1, Vec2d v2);
Vector2 subVec(Vec2d v1, Vec2d v2);
Vector2 multVec(Vec2d v, float n);
Vector2 divVec(Vec2d v, float n);
void DrawArrow(Vec2d v_base, Vec2d v_target, Color c);
*/

#endif
#include "e2d_core.h"

int Random(int range_start, int range_end) {
    struct timespec tp;
    clockid_t clk_id;
    clk_id = CLOCK_MONOTONIC;
    clock_gettime(clk_id, &tp);
    srand(tp.tv_nsec);
    return (rand() % (range_end - range_start + 1)) + range_start;
}

float LimitNum(float number, float limit) {
	int vorzeichen = (number < 0) ? -1 : 1;
	float numberMag = number * vorzeichen;
	if (numberMag > limit) numberMag = limit;

	return numberMag * vorzeichen;
}

void DrawArrow(Vector2 v_base, Vector2 v_target, Color c) {
	DrawLineEx(v_base, v_target, 4, c);
}

Matrix3x1 VecToMatrix(Vector2 point) {

	return (Matrix3x1){{point.x, point.y, 1}};
}

Vector2 MatrixToVec(Matrix3x1 matrix) {

	return (Vector2){matrix.index[0], matrix.index[1]};
}

Matrix3x1 MultMatrix(Matrix3x3 a, Matrix3x1 b) {
	Matrix3x1 result = {{0, 0, 0}};
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			result.index[i] += a.index[i][j] * b.index[j];
		}
	}
	return result;
}

Matrix3x1 RotateMatrix(Vector2 point, Vector2 center, float angel) {

	Matrix3x1 matrix_point = VecToMatrix(point);
	Matrix3x1 matrix_center = VecToMatrix(center);

	Matrix3x3 m_rotate = { {
		{cosf(angel), -sinf(angel), 0},
		{sinf(angel), cosf(angel), 0},
		{0, 0, 1}
		} };

	Matrix3x3 m_transform_center = { {
		{1, 0, -matrix_center.index[0]},
		{0, 1, -matrix_center.index[1]},
		{0, 0, 1}
	} };

	Matrix3x3 m_transform_center_back = { {
		{1, 0, matrix_center.index[0]},
		{0, 1, matrix_center.index[1]},
		{0, 0, 1}
	} };

	Matrix3x1 matrix_p_transformed = MultMatrix(m_transform_center, matrix_point);
	Matrix3x1 matrix_p_transformed_rotated = MultMatrix(m_rotate, matrix_p_transformed);

	return MultMatrix(m_transform_center_back, matrix_p_transformed_rotated);
}



Vector2 vecCreate(float x, float y) {
    return (Vector2){x, y};
}

void vSet(Vector2* v, float x, float y) {
	v->x = x;
	v->y = y;
}

void vScale(Vector2* v, float n) {
	v->x = v->x * n;
	v->y = v->y * n;
}

void vDiv(Vector2* v, float n) {
	v->x = v->x / n;
	v->y = v->y / n;	
}

Vector2 vecAdd(Vector2 v1, Vector2 v2) {
	Vector2 result = {v1.x + v2.x, v1.y + v2.y};
	return result;
}

Vector2 VecSub(Vector2 v1, Vector2 v2) {
	Vector2 result = {v1.x - v2.x, v1.y - v2.y};
	return result;
}

void vNorm(Vector2* v) {
	float len = sqrtf(v->x * v->x + v->y * v->y);
	if (len > 0) {
		v->x = v->x / len;
		v->y = v->y / len;
	}
}

void vLimit(Vector2* v, float max) {
	float len = sqrtf(v->x * v->x + v->y * v->y);
	if (len > max) {
		v->x = v->x / len * max;
		v->y = v->y / len * max;
	}
}

void vSetMag(Vector2* v, float magnitude) {
	float len = sqrtf(v->x * v->x + v->y * v->y);
	if (len > 0) {
		v->x = v->x / len * magnitude;
		v->y = v->y / len * magnitude;
	}
}

float vecDot(Vector2 v1, Vector2 v2) {
	return v1.x * v2.x + v1.y * v2.y;
}

float vecCross(Vector2 v1, Vector2 v2) {
	return v1.x * v2.y - v1.y * v2.x;
}

Vector2 vecPerp(Vector2 v) {
	return (Vector2){-v.y, v.x};
}

float vecMag(Vector2 v) {
	return sqrtf(v.x * v.x + v.y * v.y);
}

float vecMagsq(Vector2 v) {
	return v.x * v.x + v.y * v.y;
}

float vecDist(Vector2 v1, Vector2 v2) {
	Vector2 vdist = VecSub(v1, v2);
	return vecMag(vdist);
}

Vector2 vecRotate(Vector2 v, Vector2 base, float n) {
	Vector2 direction = VecSub(v, base);
	float x = direction.x * cosf(n) - direction.y * sinf(n);
	float y = direction.x * sinf(n) + direction.y * cosf(n);
	return (Vector2){x = x + base.x, y = y + base.y};
}
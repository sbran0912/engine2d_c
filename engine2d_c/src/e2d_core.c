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



Vector2 CreateVec(float x, float y) {
    return (Vector2){x, y};
}

void SetVec(Vector2* v, float x, float y) {
	v->x = x;
	v->y = y;
}

Vector2 ScaleVec(Vector2 v, float n) {
	Vector2 result = {v.x*n, v.y*n};
	return result;

}

Vector2 DivVec(Vector2 v, float n) {
	Vector2 result = {v.x/n, v.y/n};
	return result;
}

Vector2 AddVec(Vector2 v1, Vector2 v2) {
	Vector2 result = {v1.x + v2.x, v1.y + v2.y};
	return result;
}

Vector2 SubVec(Vector2 v1, Vector2 v2) {
	Vector2 result = {v1.x - v2.x, v1.y - v2.y};
	return result;
}

Vector2 NormVec(Vector2 v) {
	Vector2 result = {0.0f, 0.0f};
	float len = sqrtf(v.x * v.x + v.y * v.y);
	if (len > 0) {
		result.x = v.x / len;
		result.y = v.y / len;
	}
	return result;
}

float DotVec(Vector2 v1, Vector2 v2) {
	return v1.x * v2.x + v1.y * v2.y;
}

float MagVec(Vector2 v) {
	return sqrtf(v.x * v.x + v.y * v.y);
}

float MagsqVec(Vector2 v) {
	return v.x * v.x + v.y * v.y;
}

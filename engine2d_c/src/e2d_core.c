#include "e2d_core.h"

int e2Random(int range_start, int range_end) {
    struct timespec tp;
    clockid_t clk_id;
    clk_id = CLOCK_MONOTONIC;
    clock_gettime(clk_id, &tp);
    srand(tp.tv_nsec);
    return (rand() % (range_end - range_start + 1)) + range_start;
}

float e2LimitNum(float number, float limit) {
	int vorzeichen = (number < 0) ? -1 : 1;
	float numberMag = number * vorzeichen;
	if (numberMag > limit) numberMag = limit;

	return numberMag * vorzeichen;
}

void e2DrawArrow(Vector2 v_base, Vector2 v_target, Color c) {
	DrawLineEx(v_base, v_target, 4, c);
}

Matrix3x1 e2VecToMatrix(Vector2 point) {

	return (Matrix3x1){{point.x, point.y, 1}};
}

Vector2 e2MatrixToVec(Matrix3x1 matrix) {

	return (Vector2){matrix.index[0], matrix.index[1]};
}

Matrix3x1 e2MultMatrix(Matrix3x3 a, Matrix3x1 b) {
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

Matrix3x1 e2RotateMatrix(Vector2 point, Vector2 center, float angel) {

	Matrix3x1 matrix_point = e2VecToMatrix(point);
	Matrix3x1 matrix_center = e2VecToMatrix(center);

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

	Matrix3x1 matrix_p_transformed = e2MultMatrix(m_transform_center, matrix_point);
	Matrix3x1 matrix_p_transformed_rotated = e2MultMatrix(m_rotate, matrix_p_transformed);

	return e2MultMatrix(m_transform_center_back, matrix_p_transformed_rotated);
}



Vector2 e2CreateVec(float x, float y) {
    return (Vector2){x, y};
}

void e2SetV(Vector2* v, float x, float y) {
	v->x = x;
	v->y = y;
}

void e2ScaleV(Vector2* v, float n) {
	v->x = v->x * n;
	v->y = v->y * n;
}

Vector2 e2Scale(Vector2 v, float n) {
	return (Vector2) {v.x * n, v.y * n};
}

void e2DivV(Vector2* v, float n) {
	v->x = v->x / n;
	v->y = v->y / n;	
}

Vector2 e2Add(Vector2 v1, Vector2 v2) {
	Vector2 result = {v1.x + v2.x, v1.y + v2.y};
	return result;
}

Vector2 e2Sub(Vector2 v1, Vector2 v2) {
	Vector2 result = {v1.x - v2.x, v1.y - v2.y};
	return result;
}

void e2NormalizeV(Vector2* v) {
	float len = sqrtf(v->x * v->x + v->y * v->y);
	if (len > 0.0001) {
		v->x = v->x / len;
		v->y = v->y / len;
	}
}

void e2LimitV(Vector2* v, float max) {
	float len = sqrtf(v->x * v->x + v->y * v->y);
	if (len > max) {
		v->x = v->x / len * max;
		v->y = v->y / len * max;
	}
}

void e2SetMagV(Vector2* v, float magnitude) {
	float len = sqrtf(v->x * v->x + v->y * v->y);
	if (len > 0) {
		v->x = v->x / len * magnitude;
		v->y = v->y / len * magnitude;
	}
}

float e2Dot(Vector2 v1, Vector2 v2) {
	return v1.x * v2.x + v1.y * v2.y;
}

float e2Cross(Vector2 v1, Vector2 v2) {
	return v1.x * v2.y - v1.y * v2.x;
}

Vector2 e2Perp(Vector2 v) {
	return (Vector2){-v.y, v.x};
}

float e2Mag(Vector2 v) {
	return sqrtf(v.x * v.x + v.y * v.y);
}

float e2Magsq(Vector2 v) {
	return v.x * v.x + v.y * v.y;
}

float e2Distance(Vector2 v1, Vector2 v2) {
	Vector2 vdist = e2Sub(v1, v2);
	return e2Mag(vdist);
}

Vector2 e2Rotate(Vector2 v, Vector2 base, float n) {
	Vector2 direction = e2Sub(v, base);
	float x = direction.x * cosf(n) - direction.y * sinf(n);
	float y = direction.x * sinf(n) + direction.y * cosf(n);
	return (Vector2){x = x + base.x, y = y + base.y};
}

Intersection e2Intersect(Vector2 start_a, Vector2 end_a, Vector2 start_b, Vector2 end_b) {
	Vector2 a = e2Sub(end_a, start_a);
	Vector2 b = e2Sub(end_b, start_b);
	float cross1 = e2Cross(a, b);
	float cross2 = e2Cross(b, a);
	if (fabs(cross1 - 0.0f) > 0.01) { //Float kann man nicht direkt auf 0 testen!!!
		float s = e2Cross(e2Sub(start_b, start_a), b) / cross1;
		float u = e2Cross(e2Sub(start_a, start_b), a) / cross2;
		if (s > 0.0001 && s < 1 && u > 0.0001 && u < 1) {
			return (Intersection){s, e2Add(start_a, e2Scale(a, s))};
		}
	}
	return (Intersection){0.0f, (Vector2){0.0f, 0.0f}};
}

float e2MinDist(Vector2 p, Vector2 start_a, Vector2 end_a) {
	float dist = -1.0f;

	//Vektor start_a to end_a (line_a)
	Vector2 line_a = e2Sub(end_a, start_a);
	//Vektor imaginary line start_a to p
	Vector2 start_a_to_p = e2Sub(p, start_a);
	//Magnitude of line_a
	float magnitude = e2Mag(line_a);

	//Scalarprojecton from line (start_a to p) on line_a
	e2NormalizeV(&line_a);
	float sp = e2Dot(line_a, start_a_to_p);

	//Scalarprojection in magnitude of line_a?
	if (sp > 0.0001 && sp <= magnitude) {
		e2ScaleV(&line_a, sp);
		dist = e2Mag(e2Sub(start_a_to_p, line_a));
	}
	return dist;
}
#include "e2d_core.h"

int main() {

    Vector2 v1 = e2CreateVec((float)e2Random(10,20), (float)e2Random(100, 150));
    Vector2 v2 = v1;
        
    printf("%f %f \n", v1.x, v1.y);
    printf("%f %f \n", v2.x, v2.y);
    e2SetV(&v2, 5.0f, 5.0f);
    Matrix3x1 m1 = {.index = {10, 10, 1}};
    Matrix3x3 m2 = {.index = {{1, 2, 1}, {4, 5, 1}, {0, 0, 1}}};
    Matrix3x1 m3 = e2MultMatrix(m2, m1);
    Vector2 v = e2MatrixToVec(m3);

    float a = 10.0f;
    float b = e2LimitNum(a, -12.0f);
    printf("%f %f   ", v.x, v.y);
    printf("%f\n", b);


    return EXIT_SUCCESS;
}

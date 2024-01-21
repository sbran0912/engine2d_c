#include "e2d_core.h"

int main() {

    Vector2 v1 = vecCreate((float)random(10,20), (float)random(100, 150));
    Vector2 v2 = v1;
        
    printf("%f %f \n", v1.x, v1.y);
    printf("%f %f \n", v2.x, v2.y);
    setV(&v2, 5, 5);
    Matrix3x1 m1 = {.index = {10, 10, 1}};
    Matrix3x3 m2 = {.index = {{1, 2, 1}, {4, 5, 1}, {0, 0, 1}}};
    Matrix3x1 m3 = MultMatrix(m2, m1);
    Vector2 v = MatrixToVec(m3);

    float a = 10.0f;
    float b = limitNum(a, -12.0f);



    return EXIT_SUCCESS;
}

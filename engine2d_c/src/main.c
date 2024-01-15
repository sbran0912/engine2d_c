#include "rl2d_core.h"

int main() {

    Vector2 v1 = createVec((float)random(10,20), (float)random(100, 150));
    Vector2 v2 = copyVec(v1);
    v2.x = 99.0f;
    printf("%f %f \n", v1.x, v1.y);
    printf("%f %f \n", v2.x, v2.y);

    return EXIT_SUCCESS;
}

#include "rl2d_core.h"

int random(int range_start, int range_end) {
    struct timespec tp;
    clockid_t clk_id;
    clk_id = CLOCK_MONOTONIC;
    clock_gettime(clk_id, &tp);
    srand(tp.tv_nsec);
    return (rand() % (range_end - range_start + 1)) + range_start;
}

Vector2 createVec(float x, float y) {
    return (Vector2){x, y};
}

Vector2 copyVec(Vector2 v) {
    return v;
}
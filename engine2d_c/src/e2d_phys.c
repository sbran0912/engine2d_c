#include "e2d_phys.h"

void _drawBox(Shape* box, float thick, Color c) {
    for (int i = 0; i < 4; i++) {
        DrawLineEx(box->vertices[i], box->vertices[i+1], thick, c);
    }
    DrawCircleV(box->location, 5, c);
}

void _rotateBox(Shape* box, float angle) {
    for (int i = 0; i < 5; i++) {
        box->vertices[i] = rotateVec(box->vertices[i], box->location, angle); 
    }
}

void _updateBox(Shape* box) {
    box->velocity = addVec(box->velocity, box->accel);
    vecLimit(&box->velocity, 10);
    vecSet(&box->accel, 0, 0);

    box->angVelocity += box->angAccel;
    limitNum(box->angVelocity, 0.05f);
    box->angAccel = 0;

    box->location = addVec(box->location, box->velocity);
    box->vertices[0] = addVec(box->vertices[0], box->velocity);
    box->vertices[1] = addVec(box->vertices[1], box->velocity);
    box->vertices[2] = addVec(box->vertices[2], box->velocity);
    box->vertices[3] = addVec(box->vertices[3], box->velocity);
    box->vertices[4] = addVec(box->vertices[4], box->velocity);
    _rotateBox(box, box->angVelocity);
}

void _resetPosBox(Shape* box, Vector2 v) {
    if (box->mass != INFINITY) {
        box->location = addVec(box->location, v);
        box->vertices[0] = addVec(box->vertices[0], v);
        box->vertices[1] = addVec(box->vertices[1], v);
        box->vertices[2] = addVec(box->vertices[2], v);
        box->vertices[3] = addVec(box->vertices[3], v);
        box->vertices[4] = addVec(box->vertices[4], v);
    }
}


Shape createBox(float x, float y, float w, float h) {
    Shape result = {
        .typ = BOX,
        .marked = false,
        .location = { x + w / 2, y + h / 2 },
        .mass = (w + h) * 2,
        .inertia = w * h * w,
        .velocity = { 0, 0 },
        .angVelocity = 0,
        .accel = { 0, 0 },
        .angAccel = 0,
        .vertices[0] = { x, y },
        .vertices[1] = { x + w, y },
        .vertices[2] = {x + w, y + h},
        .vertices[3] = {x, y + h},
        .vertices[4] = { x, y },
        .funcDraw = &_drawBox,
        .funcUpdate = &_updateBox,
        .funcResetPos = &_resetPosBox
    };
    return result;
}

void Draw(Shape* shape, float thick, Color c) {
    shape->funcDraw(shape, thick, c);
}

void Update(Shape* shape) {
    shape->funcUpdate(shape);
}

void ApplyForce(Shape* shape, Vector2 force, float angForce){
    shape->accel = addVec(shape->accel, divVec(force, shape->mass));
    shape->angAccel += angForce / shape->mass;
}

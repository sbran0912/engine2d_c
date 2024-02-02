#ifndef PHYS_H
#define PHYS_H
#include "e2d_core.h"

enum figure {BOX, BALL};

typedef struct _shape {
    enum figure typ;
    Vector2 location;
    Vector2 velocity;
    float angVelocity;
    Vector2 accel;
    float angAccel;
    float mass;
    float inertia;
    bool marked;
    float radius;
    Vector2 orientation;
    Vector2 vertices[5];
    void (*funcDraw)(struct _shape*, float, Color);
    void (*funcUpdate)(struct _shape*);
    void (*funcResetPos)(struct _shape*, Vector2);
} Shape;

Shape createBox(float x, float y, float w, float h);
Shape createBall(float x, float y, float r);

void Draw(Shape* shape, float thick, Color c);
void Update(Shape* shape);
void ApplyForce(Shape* shape, Vector2 force, float angForce);
void ResetPos(Shape* shape, Vector2 v);
Vector2 checkKicking(Shape* shape);

#endif

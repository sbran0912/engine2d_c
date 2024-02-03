#include "e2d_phys.h"

void _drawBox(Shape *box, float thick, Color c) {
    for (int i = 0; i < 4; i++)
    {
        DrawLineEx(box->vertices[i], box->vertices[i + 1], thick, c);
    }
    DrawCircleV(box->location, 5, c);
}

void _rotateBox(Shape *box, float angle) {
    for (int i = 0; i < 5; i++)
    {
        box->vertices[i] = rotateVec(box->vertices[i], box->location, angle);
    }
}

void _updateBox(Shape *box) {
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

void _resetPosBox(Shape *box, Vector2 v) {
    if (box->mass != INFINITY)
    {
        box->location = addVec(box->location, v);
        box->vertices[0] = addVec(box->vertices[0], v);
        box->vertices[1] = addVec(box->vertices[1], v);
        box->vertices[2] = addVec(box->vertices[2], v);
        box->vertices[3] = addVec(box->vertices[3], v);
        box->vertices[4] = addVec(box->vertices[4], v);
    }
}

void _drawBall(Shape *ball, float thick, Color c) {
    DrawRing(ball->location, ball->radius - 3, ball->radius, 0, 360, 1, c);
    // DrawCircleV(location.pos,radius, c);
    DrawLineEx(ball->location, ball->orientation, thick, c);
}

void _rotateBall(Shape* ball, float angle) {
    ball->orientation = rotateVec(ball->orientation, ball->location, angle);
}

void _updateBall(Shape* ball) {
    ball->velocity = addVec(ball->velocity, ball->accel);
    vecLimit(&ball->velocity, 10);
    vecSet(&ball->accel, 0, 0);

    ball->angVelocity += ball->angAccel;
    limitNum(ball->angVelocity, 0.05f);
    ball->angAccel = 0;

    ball->location = addVec(ball->location, ball->velocity);
    ball->orientation = addVec(ball->orientation, ball->velocity);

    _rotateBall(ball, ball->angVelocity);
}

void _resetPosBall(Shape *ball, Vector2 v) {
    if (ball->mass != INFINITY) {
        ball->location = addVec(ball->location, v);
        ball->orientation = addVec(ball->orientation, v);
    }
}


Shape createBox(float x, float y, float w, float h) {
    Shape result = {
        .typ = BOX,
        .marked = false,
        .location = {x + w / 2, y + h / 2},
        .mass = (w + h) * 2,
        .inertia = w * h * w,
        .velocity = {0, 0},
        .angVelocity = 0,
        .accel = {0, 0},
        .angAccel = 0,
        .vertices[0] = {x, y},
        .vertices[1] = {x + w, y},
        .vertices[2] = {x + w, y + h},
        .vertices[3] = {x, y + h},
        .vertices[4] = {x, y},
        .funcDraw = &_drawBox,
        .funcUpdate = &_updateBox,
        .funcResetPos = &_resetPosBox};
    return result;
}

Shape createBall(float x, float y, float r) {
    Shape result = {
        .typ = BALL,
        .marked = false,
        .location = {x, y},
        .radius = r,
        .mass = r * 2,
        .inertia = r * r * r / 2,
        .velocity = {0, 0},
        .angVelocity = 0,
        .accel = {0, 0},
        .angAccel = 0,
        .orientation = {r + x, y},
        .funcDraw = &_drawBall,
        .funcUpdate = &_updateBall,
        .funcResetPos = &_resetPosBall};
    return result;
}

void draw(Shape *shape, float thick, Color c) {
    shape->funcDraw(shape, thick, c);
}

void update(Shape *shape) {
    shape->funcUpdate(shape);
}

void applyForce(Shape *shape, Vector2 force, float angForce) {
    shape->accel = addVec(shape->accel, divVec(force, shape->mass));
    shape->angAccel += angForce / shape->mass;
}

void resetPos(Shape *shape, Vector2 v) {
    shape->funcResetPos(shape, v);
}

Vector2 checkKicking(Shape* shape) {

    Vector2 mousePos = { (float)GetMouseX(), (float)GetMouseY() };

    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {

        if (distance(shape->location, mousePos) < 10) {
            shape->marked = true;
        }

        if (shape->marked) {
            drawArrow(shape->location, mousePos, RED);
        }
    }

    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && shape->marked) {
        shape->marked = false;
        return subVec(mousePos, shape->location);
    }

    return (Vector2){0, 0};
}

CollisionPoint detectCollisionBox(Shape boxA, Shape boxB) {
    // Geprüft wird, ob eine Ecke von boxA in die Kante von boxB schneidet
    // Zusätzlich muss die Linie von Mittelpunkt boxA und Mittelpunkt boxB durch Kante von boxB gehen
    // i ist Index von Ecke und j ist Index von Kante
    // d = Diagonale von A.Mittelpunkt zu A.vertices(i)
    // e = Kante von B(j) zu B(j+1)
    // z = Linie von A.Mittelpunkt zu B.Mittelpunkt
    // _perp = Perpendicularvektor
    // scalar_d Faktor von d für den Schnittpunkt d/e
    // scalar_z Faktor von z für den Schnittpunkt z/e
    // mtv = minimal translation vector (überlappender Teil von d zur Kante e)

    for (int i = 0; i < 4; i++) {            
        for (int j = 0; j < 4; j++) {
            // Prüfung auf intersection von Diagonale d zu Kante e
            Intersection isd = intersect(boxA.location, boxA.vertices[i], boxB.vertices[j], boxB.vertices[j + 1]);   
            if (isd.distance > 0.0f) {
                // Prüfung auf intersection Linie z zu Kante e
                Intersection isz = intersect(boxA.location, boxB.location, boxB.vertices[j], boxB.vertices[j + 1]);
                if (isz.distance > 0.0f) {
                    // Collision findet statt
                    // Objekte zurücksetzen und normal_e berechnen. Kollisionspunkt ist Ecke i von BoxA
                    Vector2 e = subVec(boxB.vertices[j + 1], boxB.vertices[j]);
                    Vector2 e_perp = {-(e.y), e.x};
                    Vector2 d = subVec(boxA.vertices[i], boxA.location);  
                    vecScale(&d, 1-isd.distance);
                    vecNormalize(&e_perp);
                    float distance = dot(e_perp, d);
                    vecScale(&e_perp, -distance); //mtv
                    resetPos(&boxA, scaleVec(e_perp, 0.5f));
                    resetPos(&boxB, scaleVec(e_perp, -0.5f));
                    vecNormalize(&e_perp); // normal_e
                    return (CollisionPoint){boxA.vertices[i], e_perp};                }
            }
        }
    }
    return (CollisionPoint) {{0.0f, 0.0f}, {0.0f, 0.0f}};
};

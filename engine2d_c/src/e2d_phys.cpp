#include "e2d_phys.h"

Vec2d Figure2d::checkKicking() {

    Vec2d mousePos{ (float)GetMouseX(), (float)GetMouseY() };

    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {

        if (location.distance(mousePos) < 10) {
            marked = true;
        }

        if (marked) {
            core::DrawArrow(location, mousePos, RED);
        }
    }

    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && marked) {
        marked = false;
        return core::subVec(mousePos, location);
    }

    return Vec2d{0, 0};
}

Ball::Ball(float x, float y, float r)
    {
        typ = 'L';
        marked = false;
        location = Vec2d{ x, y };
        radius = r;
        mass = radius * 2;
        inertia = radius * radius * radius / 2;
        velocity = Vec2d{ 0, 0 };
        angVelocity = 0;
        accel = Vec2d{ 0, 0 };
        angAccel = 0;
        orientation = Vec2d{ radius + x, 0 + y };
    }

void Ball::rotate(float angle) {
    orientation.rotate(location, angle);
}

void Ball::draw(float thick, Color c) {
    DrawRing(location.pos, radius - 3, radius, 0, 360, 1, c);
    //DrawCircleV(location.pos,radius, c);
    DrawLineEx(location.pos, orientation.pos, thick, c);
}

void Ball::update(){
    velocity.add(accel);
    velocity.limit(10);
    accel.set(0, 0);
    angVelocity += angAccel;
    angVelocity = core::limitNum(angVelocity, 0.05f);
    angAccel = 0;

    location.add(velocity);
    orientation.add(velocity);
    rotate(angVelocity);
}

void Ball::applyForce(Vec2d force, float angForce){
    accel.add(core::divVec(force, mass));
    angAccel += angForce / mass;
}

void Ball::resetPos(Vec2d v){
    location.add(v);
    orientation.add(v);

}

void Ball::test() {
    //printf("Hier ist der Ball\n");
}

Box::Box(float x, float y, float w, float h)
    {
        typ = 'X';
        marked = false;
        location = Vec2d{ x + w / 2, y + h / 2 };
        mass = (w + h) * 2;
        inertia = w * h * w;
        velocity = Vec2d{ 0, 0 };
        angVelocity = 0;
        accel = Vec2d{ 0, 0 };
        angAccel = 0;
        vertices[0] = Vec2d{ x, y };
        vertices[1] = Vec2d{ x + w, y };
        vertices[2] = Vec2d(x + w, y + h);
        vertices[3] = Vec2d{x, y + h};
        vertices[4] = Vec2d{ x, y };
    }

void Box::rotate(float angle) {
    for (int i = 0; i < 5; i++) {
        vertices[i].rotate(location, angle);
    }
}

void Box::draw(float thick, Color c) {
    for (int i = 0; i < 4; i++) {
        DrawLineEx(vertices[i].pos, vertices[i+1].pos, thick, c);
    }
    DrawCircleV(location.pos, 5, c);
}

void Box::update() {
    velocity.add(accel);
    velocity.limit(10);
    accel.set(0, 0);
    angVelocity += angAccel;
    angVelocity = core::limitNum(angVelocity, 0.05f);
    angAccel = 0;

    location.add(velocity);
    vertices[0].add(velocity);
    vertices[1].add(velocity);
    vertices[2].add(velocity);
    vertices[3].add(velocity);
    vertices[4].add(velocity);
    rotate(angVelocity);
}

void Box::applyForce(Vec2d force, float angForce){
    accel.add(core::divVec(force, mass));
    angAccel += angForce / mass;
}

void Box::resetPos(Vec2d v) {
    if (mass != INFINITY) {
        location.add(v);
        vertices[0].add(v);
        vertices[1].add(v);
        vertices[2].add(v);
        vertices[3].add(v);
        vertices[4].add(v);
    }
}

void Box::test() {
    //printf("Hier ist die Box\n");
}
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
    vecLimit(&box->velocity, 10.0f);
    vecSet(&box->accel, 0.0f, 0.0f);

    box->angVelocity += box->angAccel;
    limitNum(box->angVelocity, 0.05f);
    box->angAccel = 0.0f;

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
    vecLimit(&ball->velocity, 10.0f);
    vecSet(&ball->accel, 0.0f, 0.0f);

    ball->angVelocity += ball->angAccel;
    limitNum(ball->angVelocity, 0.05f);
    ball->angAccel = 0.0f;

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

Shadow createShadow(Shape* shape) {
    Shadow shadow;
    if (shape->typ == BALL) {
        shadow = (Shadow){
            .minX = shape->location.x - shape->radius, 
            .maxX = shape->location.x + shape->radius, 
            .minY = shape->location.y - shape->radius, 
            .maxY = shape->location.y + shape->radius
            };
    } else {
        shadow = (Shadow){
            .minX = INFINITY, 
            .maxX =-INFINITY, 
            .minY = INFINITY, 
            .maxY =-INFINITY
            };
        for (int i = 0; i < 4; i++) {
            if (shape->vertices[i].x < shadow.minX) {
                shadow.minX = shape->vertices[i].x;
            } 
            if (shape->vertices[i].y < shadow.minY) {
                shadow.minY = shape->vertices[i].y;
            } 
            if (shape->vertices[i].x > shadow.maxX) {
                shadow.maxX = shape->vertices[i].x;
            } 
            if (shape->vertices[i].y > shadow.maxY) {
                shadow.maxY = shape->vertices[i].y;
            } 
        }    
    }
    
    return shadow;
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

        if (distanceVec(shape->location, mousePos) < 10) {
            shape->marked = true;
        }

        if (shape->marked) {
            drawArrow(shape->location, mousePos, RED);
        }
    }

    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && shape->marked) {
        shape->marked = false;
        Vector2 force = subVec(mousePos, shape->location);
        vecScale(&force, 5);
        return force;
    }

    return (Vector2){0, 0};
}

CollisionPoint detectCollisionBox(Shape* boxA, Shape* boxB) {
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
            Intersection isd = intersectVec(boxA->location, boxA->vertices[i], boxB->vertices[j], boxB->vertices[j + 1]);   
            if (isd.distance > 0.0f) {
                // Prüfung auf intersection Linie z zu Kante e
                Intersection isz = intersectVec(boxA->location, boxB->location, boxB->vertices[j], boxB->vertices[j + 1]);
                if (isz.distance > 0.0f) {
                    // Collision findet statt
                    // Objekte zurücksetzen und normal_e berechnen. Kollisionspunkt ist Ecke i von BoxA
                    Vector2 e = subVec(boxB->vertices[j + 1], boxB->vertices[j]);
                    Vector2 e_perp = {-(e.y), e.x};
                    Vector2 d = subVec(boxA->vertices[i], boxA->location);  
                    vecScale(&d, 1-isd.distance);
                    vecNormalize(&e_perp);
                    float distance = dotVec(e_perp, d);
                    vecScale(&e_perp, -distance); //mtv
                    resetPos(boxA, scaleVec(e_perp, 0.5f));
                    resetPos(boxB, scaleVec(e_perp, -0.5f));
                    vecNormalize(&e_perp); // normal_e
                    return (CollisionPoint){true, boxA->vertices[i], e_perp};                }
            }
        }
    }
    return (CollisionPoint) {false, {0.0f, 0.0f}, {0.0f, 0.0f}};
};

CollisionPoint detectCollisionBall(Shape* ballA, Shape* ballB) {
    //Distanz ermitteln
    float radiusTotal = ballA->radius + ballB->radius;
    float dist = distanceVec(ballA->location, ballB->location);
    if (dist < radiusTotal) {
        //Treffer
        float space = (radiusTotal - dist);
        Vector2 collisionLine = subVec(ballA->location, ballB->location);
        vecSetMag(&collisionLine, space);
        resetPos(ballA, scaleVec( collisionLine, 0.5));
        resetPos(ballB, scaleVec( collisionLine, -0.5));
        vecNormalize(&collisionLine);
        return (CollisionPoint) {true, {0.0f, 0.0f}, collisionLine};
    }
    return (CollisionPoint) {false, {0.0f, 0.0f}, {0.0f, 0.0f}};
}

CollisionPoint detectCollisionBallBox(Shape* ball, Shape* box) {
    for (int i = 0; i < 4; i++) {
        //Kante der Box
        Vector2 e = subVec(box->vertices[i+1], box->vertices[i]);
        //Vektor von Ecke der Box zum Ball
        Vector2 VerticeToBall = subVec(ball->location, box->vertices[i]);
        //Kollision mit Ecken abfangen
        if (magVec(VerticeToBall) < ball->radius) {
            return (CollisionPoint){true, box->vertices[i], VerticeToBall};
        }
        float mag_e = magVec(e);
        vecNormalize(&e);
        //Scalarprojektion von Vektor VerticeToBall auf Kante e
        float scalar_e = dotVec(VerticeToBall, e);
        if (scalar_e > 0 && scalar_e <= mag_e) {
            //Senkrechte von Ball trifft auf Kante e der Box
            //e2 = Kante e mit der Länge von scalar_e
            Vector2 e2 = scaleVec(e, scalar_e);
            //Senkrechte von e zum Ball = VerticeToBall - e2
            Vector2 e_perp = subVec(VerticeToBall, e2);

            if (magVec(e_perp) < ball->radius) {
                //Ball berührt Box
                //Abstand wieder herstellen mit mtv (minimal translation vector)
                Vector2 mtv = e_perp;
                Vector2 p = addVec(box->vertices[i], e2);
                vecSetMag(&mtv, ball->radius - magVec(e_perp));
                //e_perp und damit mtv zeigt von Kante zu Ball
                resetPos(ball, mtv);
                //vor Berechnung muss e_perp normalisiert werden
                vecNormalize(&e_perp);
                //resolveCollisionBallBox(ball, box, p, e_perp)
                return (CollisionPoint){true, p, e_perp};
            }
        }
    }
    return (CollisionPoint) {false, {0.0f, 0.0f}, {0.0f, 0.0f}};
}

void resolveCollisionBox(Shape* boxA, Shape* boxB, Vector2 cp, Vector2 normal) {
    // rAP = Linie von A.location zu Kollisionspunkt (Ecke i von BoxA)
    Vector2 rAP = subVec(cp, boxA->location);
    // rBP = Linie von B.location zu Kollisionspunkt (ebenfalls Ecke i von BoxA)
    Vector2 rBP = subVec(cp, boxB->location);
    Vector2 rAP_perp = {-rAP.y, rAP.x};
    Vector2 rBP_perp = {-rBP.y, rBP.x};
    Vector2 VtanA = scaleVec(rAP_perp, boxA->angVelocity);
    Vector2 VtanB = scaleVec(rBP_perp, boxB->angVelocity);
    Vector2 VgesamtA = addVec(boxA->velocity, VtanA);
    Vector2 VgesamtB = addVec(boxB->velocity, VtanB);
    Vector2 velocity_AB = subVec(VgesamtA, VgesamtB);
    if (dotVec(velocity_AB, normal) < 0) { // wenn negativ, dann auf Kollisionskurs
        float e = 1.0f; //inelastischer Stoß
        float j_denominator = dotVec(scaleVec(velocity_AB, -(1+e)), normal);
        float j_divLinear = dotVec(normal, scaleVec(normal, (1/boxA->mass + 1/boxB->mass)));
        float j_divAngular = (float)pow(dotVec(rAP_perp, normal), 2) / boxA->inertia + (float)pow(dotVec(rBP_perp, normal), 2) / boxB->inertia;
        float j = j_denominator / (j_divLinear + j_divAngular);
        // Grundlage für Friction berechnen (t)
        Vector2 t = {-(normal.y), normal.x};
        float t_scalarprodukt = dotVec(velocity_AB, t);
        vecScale(&t, (t_scalarprodukt));
        vecNormalize(&t);

        //apply Force        
        Vector2 force = addVec(scaleVec(normal, (j/boxA->mass)), scaleVec(t, (0.2*-j/boxA->mass)));
        float force_ang = dotVec(rAP_perp, addVec(scaleVec(normal, j/boxA->inertia), scaleVec(t, 0.2*-j/boxA->inertia)));
        boxA->accel = addVec(boxA->accel, force);
        boxA->angAccel += force_ang;

        force = addVec(scaleVec(normal, (-j/boxB->mass)), scaleVec(t, (0.2*j/boxB->mass)));
        force_ang = dotVec(rAP_perp, addVec(scaleVec(normal, -j/boxB->inertia), scaleVec(t, 0.2*j/boxB->inertia)));
        boxB->accel = addVec(boxB->accel, force);
        boxB->angAccel += force_ang;

    }
}

void resolveCollisionBall(Shape* ballA, Shape* ballB, Vector2 normal) {
    Vector2 rA = scaleVec(normal, -ballA->radius);
    Vector2 rA_perp = {-rA.y, rA.x};
    Vector2 rB = scaleVec(normal, ballB->radius);
    Vector2 rB_perp = {-rB.y, rB.x};
    Vector2 VtanA = scaleVec(rA_perp, ballA->angVelocity);
    Vector2 VtanB = scaleVec(rB_perp, ballB->angVelocity);
    Vector2 VgesamtA = addVec(ballA->velocity, VtanA);
    Vector2 VgesamtB = addVec(ballB->velocity, VtanB);
    Vector2 velocity_AB = subVec(VgesamtA, VgesamtB);   
    
    if (dotVec(velocity_AB, normal) < 0) { // wenn negativ, dann auf Kollisionskurs
        float e = 1; //inelastischer Stoß
        float j_denominator = dotVec(scaleVec(velocity_AB, -(1+e)), normal);
        float j_divLinear = dotVec(normal, scaleVec(normal, (1/ballA->mass + 1/ballB->mass)));
        float j = j_denominator / j_divLinear;
        // Grundlage für Friction berechnen
        Vector2 t = {-normal.y, normal.x};
        float t_scalarprodukt = dotVec(velocity_AB, t);
        vecScale(&t, t_scalarprodukt);
        vecNormalize(&t);

        //apply Force
        Vector2 force = addVec(scaleVec(normal, (0.8*j/ballA->mass)), scaleVec(t, (0.2*-j/ballA->mass)));
        float force_ang = dotVec(rA_perp, scaleVec(t, 0.1*-j/ballA->inertia));
        ballA->accel = addVec(ballA->accel, force);
        ballA->angAccel += force_ang;

        force = addVec(scaleVec(normal, (0.8*-j/ballB->mass)), scaleVec(t, (0.2*j/ballB->mass)));
        force_ang = dotVec(rB_perp, scaleVec(t, 0.1*j/ballB->inertia));
        ballB->accel = addVec(ballB->accel, force);
        ballB->angAccel += force_ang;
    }
}

void resolveCollisionBallBox(Shape* ball, Shape* box, Vector2 cp, Vector2 normal) {
    Vector2 rA = scaleVec(normal, -ball->radius);
    Vector2 rA_perp = {-rA.y, rA.x};
    Vector2 rBP = subVec(cp, box->location);
    Vector2 rBP_perp = {-rBP.y, rBP.x};
    Vector2 VtanA = scaleVec(rA_perp, ball->angVelocity);
    Vector2 VgesamtA = addVec(ball->velocity, VtanA);
    Vector2 VtanB = scaleVec(rBP_perp, box->angVelocity);
    Vector2 VgesamtB = addVec(box->velocity, VtanB);
    Vector2 velocity_AB = subVec(VgesamtA, VgesamtB);

    if (dotVec(velocity_AB, normal) < 0) { // wenn negativ, dann auf Kollisionskurs

        float e = 1; //inelastischer Stoß
        float j_denominator = dotVec(scaleVec(velocity_AB, -(1+e)), normal);
        float j_divLinear = dotVec(normal, scaleVec(normal, (1/ball->mass + 1/box->mass)));
        float j_divAngular = (float)pow(dotVec(rBP_perp, normal), 2) / box->inertia; //nur für Box zu rechnen
        float j = j_denominator / (j_divLinear + j_divAngular);
        // Grundlage für Friction berechnen
        Vector2 t = {-normal.y, normal.x};
        float t_scalarprodukt = dotVec(velocity_AB, t);
        vecScale(&t, t_scalarprodukt);
        vecNormalize(&t);
        // Apply Force
        Vector2 force = addVec(scaleVec(normal, (0.8*j/ball->mass)), scaleVec(t, (0.05*-j/ball->mass)));
        float force_ang = dotVec(rA_perp, scaleVec(t, 0.05*-j/ball->inertia));
        ball->accel = addVec(ball->accel, force);
        ball->angAccel += force_ang;
           
        force = addVec(scaleVec(normal, (-j/box->mass)), scaleVec(t, (0.05*j/box->mass)));
        force_ang = dotVec(rBP_perp, addVec(scaleVec(normal, -j/box->inertia), scaleVec(t, 0.05*j/box->inertia)));
        box->accel = addVec(box->accel, force);
        box->angAccel += force_ang;
    }
}

void checkCollision(Shape* shapeA, Shape* shapeB) {
    //Shadow berechnen von Element i und Element j 
    Shadow shadowA = createShadow(shapeA);
    Shadow shadowB = createShadow(shapeB);
    //Überschneidung prüfen
    if (shadowA.maxX >= shadowB.minX && shadowA.minX <= shadowB.maxX && shadowA.maxY >= shadowB.minY && shadowA.minY <= shadowB.maxY) {  
        //dann Überschneidung
        // Testcode
        DrawLineV(shapeA->location, shapeB->location, GREEN);
        // Ende Testcodew

        if (shapeA->typ == BALL) {
            if (shapeB->typ == BALL) {
                CollisionPoint cp = detectCollisionBall(shapeA, shapeB);
                if (cp.isCollision) {
                    resolveCollisionBall(shapeA, shapeB, cp.normal);
                }
            } else {
                CollisionPoint cp = detectCollisionBallBox(shapeA, shapeB);
                if (cp.isCollision) {
                    resolveCollisionBallBox(shapeA, shapeB, cp.cp, cp.normal);
                }
            }
        }

        if (shapeA->typ == BOX) {
            if (shapeB->typ == BOX) {
                // beide Boxen müssen geprüft werden, ob sie auf
                // die jeweils andere trefen könnte
                CollisionPoint cp = detectCollisionBox(shapeA, shapeB);
                if (cp.isCollision) {
                    resolveCollisionBox(shapeA, shapeB, cp.cp, cp.normal);  
                } else {
                    CollisionPoint cp = detectCollisionBox(shapeA, shapeB);    
                    if (cp.isCollision) {
                        resolveCollisionBox(shapeA, shapeB, cp.cp, cp.normal);
                    }
                }
            } else {
                CollisionPoint cp = detectCollisionBallBox(shapeB, shapeA);
                if (cp.isCollision) {
                    resolveCollisionBallBox(shapeB, shapeA, cp.cp, cp.normal);
                }
            }            
        }
    }
}
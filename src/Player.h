#pragma once

#include <raylib.h>
#include <raymath.h>
#include <vector>

#include "Planet.h"

class Player {
private:
    float playerSpeed = 0.1f;
    float jumpStrength = 2.0f;
    Planet* planet;
    Camera2D& camera;
    Vector2 position;
    Vector2 velocity = Vector2Zero();
    bool hasJumped = false;
    bool isGrounded = false;
public:
    Player(Planet* target, Camera2D& camera, Vector2 spawnPos): planet(target), camera(camera), position(spawnPos) {};
    void draw();
    void update();
    Vector2 getPosition() { return position; };
    void updateCamera();
    void updateGravity();
    void updateMovement();
private:
    Vector2 resolveCollision(Vector2 start, Vector2 end);
};
#pragma once

#include <raylib.h>
#include <raymath.h>
#include <vector>

#include "Planet.h"

class Player {
private:
    float playerSpeed = 0.001f;
    float planetPosition = 0.0f;
    Planet* planet;
    Camera2D& camera;
public:
    Player(Planet* target, Camera2D& camera): planet(target), camera(camera) {};
    void draw();
    void update();
    Vector2 getPosition() { return planet->planetToWorldCords(planetPosition).position; }
};
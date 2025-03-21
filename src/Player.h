#pragma once

#include <raylib.h>
#include <raymath.h>
#include <vector>

#include "Planet.h"

class GravityAffected {
private:
    Planet* planet;
    PlanetSystem& ps;
    Vector2 position;
    Vector2 velocity = Vector2Zero();
    bool onGround = false;
public:
    GravityAffected(PlanetSystem& ps, Vector2 spawnPos): 
        ps(ps), position(spawnPos) {};

    // Get Set
    Vector2 getPosition() { return position; };
    void setPosition(Vector2 newPos) { position = newPos; }
    void addPosition(Vector2 change) { position = Vector2Add(position, change); };
    Vector2 getVelocity() { return velocity; };
    void setVelocity(Vector2 newVel) { velocity = newVel; }
    void addVelocity(Vector2 change) { velocity = Vector2Add(velocity, change); };
    Vector2 getPlanetPosition() { return planet->getPosition(); }
    bool isOnGround() { return onGround; };
    Vector2 getSurfaceNormal() { return planet->getSurfaceNormal(position); };
    Vector2 getSurfaceNormal(Vector2 pos) { return planet->getSurfaceNormal(pos); };
    Planet* getPlanet() { return planet; };

    void updateGravity();
    Vector2 resolveCollision(Vector2 start, Vector2 end);
    bool isColliding() { return planet->isColliding(position); };
    bool isColliding(Vector2 pos) { return planet->isColliding(pos); };
};


class Player : public GravityAffected {
private:
    // Movement values
    static constexpr float PLAYER_AIR_ACCEL = 0.01f;
    static constexpr float JUMP_STRENGTH = 2.0f;

    // Object refrences
    Camera2D& camera;
public:
    Player(PlanetSystem& ps, Camera2D& camera, Vector2 spawnPos): 
        camera(camera), GravityAffected(ps, spawnPos) {};
    void draw();
    void update();
    void updateCamera();
    void updateMovement();
    void terraform();
};
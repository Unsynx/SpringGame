#include <cmath>
#include <vector>

#include "Planet.h"
#include "Player.h"

// ----------- Helper Functions -----------

float radToDeg(float rad) {
    return fmod((rad * 180 / PI) + 360, 360);
}

// ----------- Gravity Affected -----------

void GravityAffected::updateGravity() {
    planet = ps.getNearestPlanet(position);
    
    // Apply Gravity
    velocity = Vector2Add(velocity, ps.gravityAt(position));
    
    // First, handle vertical movement (gravity and jumps)
    Vector2 newPosition = Vector2Add(position, velocity);
    
    // Check for collision from vertical movement
    onGround = false;
    if (planet->isColliding(newPosition)) {
        onGround = true;
        
        // Find closest non-colliding position
        newPosition = resolveCollision(position, newPosition);
        
        // Get surface normal at contact point
        Vector2 normal = planet->getSurfaceNormal(newPosition);
        
        // Reflect velocity along the collision normal
        velocity = Vector2Reflect(velocity, normal);
        
        // Apply friction to dampen bouncing
        velocity = Vector2Scale(velocity, 0.5);
    }
    
    // Update position after vertical movement
    position = newPosition;
}

// Binary search to resolve collisions efficiently
Vector2 GravityAffected::resolveCollision(Vector2 start, Vector2 end) {
    Vector2 mid;
    int iterations = 10; // Adjust for precision

    for (int i = 0; i < iterations; i++) {
        mid = Vector2Lerp(start, end, 0.5f);

        if (planet->isColliding(mid)) {
            end = mid;  // Move towards non-colliding position
        } else {
            start = mid; // Move towards colliding position
        }
    }

    return start; // Return the last known valid position
}


// ----------- Player -----------


void Player::updateMovement() {
    Vector2 upDirection = Vector2Normalize(Vector2Subtract(getPosition(), getPlanetPosition()));
    Vector2 rightDirection = Vector2Rotate(upDirection, PI / 2);

    // Jump, this be handled w/ the gravity calculations.
    if (IsKeyPressed(KEY_SPACE) && !hasJumped) {
        hasJumped = true;
        addVelocity(Vector2Scale(upDirection, 3));
    }

    // Movement Input
    int inputX = IsKeyDown(KEY_RIGHT) - IsKeyDown(KEY_LEFT);
    if (inputX == 0) {
        return;
    }

    // In air movement
    if (!isOnGround()) {
        Vector2 rawMovement = Vector2Scale(rightDirection, inputX);
        Vector2 newPosition = Vector2Add(getPosition(), rawMovement);
        if (isColliding(newPosition)) {
            newPosition = resolveCollision(getPosition(), newPosition);
        }
        setPosition(newPosition);
        return;
    }

    // On ground movement
    Vector2 surfaceNormal = getSurfaceNormal();
    Vector2 surfaceTangent = Vector2Rotate(surfaceNormal, PI / 2);

    Vector2 rawMovement = Vector2Scale(surfaceTangent, inputX);
    Vector2 newPosition = Vector2Add(getPosition(), rawMovement);

    if (isColliding(newPosition)) {
        // This 10 value is arbitrary
        Vector2 noCollisionSpot = Vector2Add(newPosition, Vector2Scale(surfaceNormal, 10));
        newPosition = resolveCollision(noCollisionSpot, newPosition);
    } else {
        // This -10 value is arbitrary
        Vector2 collisionSpot = Vector2Add(newPosition, Vector2Scale(surfaceNormal, -10));
        if (isColliding(collisionSpot)) {
            newPosition = resolveCollision(newPosition, collisionSpot);
        }
    }

    setPosition(newPosition);
}

void Player::updateCamera() {
    Vector2 upDirection = Vector2Normalize(Vector2Subtract(getPosition(), getPlanetPosition()));
    
    camera.target = getPosition();
    camera.rotation = -atan2(upDirection.y, upDirection.x) * 180 / PI - 90;
}

void Player::update() {
    updateGravity();
    updateMovement();
    updateCamera();
}

void Player::draw() {
    Vector2 position = getPosition();
    DrawCircle(position.x, position.y, 5, BLACK);
}
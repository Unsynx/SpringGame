#include <cmath>
#include <vector>

#include "Planet.h"
#include "Player.h"

float radToDeg(float rad) {
    return fmod((rad * 180 / PI) + 360, 360);
}

void Player::updateGravity() {
    // Todo: get gravity direction from precomputed gravity field.
    Vector2 planetPos = planet->getPosition();
    float gravity = -0.05;
    
    // Direction Vectors
    Vector2 upDirection = Vector2Normalize(Vector2Subtract(position, planetPos));
    
    // Apply Gravity
    velocity = Vector2Add(velocity, Vector2Scale(upDirection, gravity));
    
    // Jump
    if (IsKeyPressed(KEY_SPACE) && !hasJumped) {
        hasJumped = true;
        velocity = Vector2Add(velocity, Vector2Scale(upDirection, 1));
    }
    
    // First, handle vertical movement (gravity and jumps)
    Vector2 newPosition = Vector2Add(position, velocity);
    
    // Check for collision from vertical movement
    bool isOnGround = false;
    if (planet->isColliding(newPosition)) {
        hasJumped = false;
        isOnGround = true;
        
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

void Player::updateMovement() {
    Vector2 planetPos = planet->getPosition();
    Vector2 upDirection = Vector2Normalize(Vector2Subtract(position, planetPos));
    Vector2 rightDirection = Vector2Rotate(upDirection, PI / 2);

    // Movement Input
    int inputX = IsKeyDown(KEY_RIGHT) - IsKeyDown(KEY_LEFT);
    Vector2 rawMovement = Vector2Scale(rightDirection, inputX);
    
    // Now handle horizontal movement with slope handling
    if (inputX != 0) {

    }
}

void Player::updateCamera() {
    Vector2 planetPos = planet->getPosition();
    Vector2 upDirection = Vector2Normalize(Vector2Subtract(position, planetPos));
    
    camera.target = position;
    camera.rotation = -atan2(upDirection.y, upDirection.x) * 180 / PI - 90;
}

void Player::update() {
    updateGravity();
    updateMovement();
    updateCamera();
}

// Binary search to resolve collisions efficiently
Vector2 Player::resolveCollision(Vector2 start, Vector2 end) {
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

void Player::draw() {
    DrawCircle(position.x, position.y, 5, BLACK);
}
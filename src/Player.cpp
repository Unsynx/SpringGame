#include <cmath>
#include <vector>

#include "Planet.h"
#include "Player.h"


void Player::update() {
    if (IsKeyDown(KEY_RIGHT)) planetPosition -= playerSpeed;
    else if (IsKeyDown(KEY_LEFT)) planetPosition += playerSpeed;

    Orientation orientation = planet->planetToWorldCords(planetPosition);
    camera.target = orientation.position;
    camera.rotation = -atan2(orientation.centerDirection.y, orientation.centerDirection.x) * 180 / PI - 90;
}

void Player::draw() {
    Orientation orientation = planet->planetToWorldCords(planetPosition);
    DrawCircle(orientation.position.x, orientation.position.y, 5, BLACK);
}
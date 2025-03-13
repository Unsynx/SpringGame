#include <cmath>
#include <vector>

#include "Planet.h"
#include "Player.h"


void Player::update() {

    camera.target = position;
    camera.rotation = -90;
}

void Player::draw() {
    DrawCircle(position.x, position.y, 5, BLACK);
}
#include <raylib.h>
#include <raymath.h>
#include <cstdlib>
#include <cmath>

#include "Planet.h"

Planet::Planet(int size, Vector2 position): baseSize(size), position(position) {
    float circumCircle = 2 * PI * baseSize;
    nodeCount = circumCircle / NODE_PER_SURFACE_LENGTH;

    TraceLog(LOG_INFO, "nodeCount: %d", nodeCount);
}

void Planet::draw() {
    Vector2 points[nodeCount];
    points[0] = position;
    float angle = 0;
    float changeInDeg = 360 / (float)(nodeCount - 2);
    for (int i = 1; i < nodeCount; i++) {
        points[i] = (Vector2){ position.x + baseSize * cos(angle), position.y - baseSize * sin(angle) };
        angle = PI * (changeInDeg * i) / 180;
    }

    // Draw the triangle fan with the given color
    DrawTriangleFan(points, nodeCount, RED);

}
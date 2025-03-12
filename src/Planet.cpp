#include <raylib.h>
#include <raymath.h>
#include <cstdlib>
#include <cmath>
#include <vector>


#include "Planet.h"

Planet::Planet(int size, Vector2 position): baseSize(size), position(position) {
    float circumCircle = 2 * PI * baseSize;
    nodeCount = circumCircle / NODE_PER_SURFACE_LENGTH;

    nodeOffsets.resize(nodeCount);

    for (int i = 0; i < nodeCount; i++) {
        nodeOffsets[i] = rand() % 10;
    }

    TraceLog(LOG_INFO, "nodeCount: %d", nodeCount);
}

void Planet::draw() {
    Vector2 points[nodeCount];
    float angle = 0;
    float changeInDeg = 360 / (float)nodeCount;
    for (int i = 0; i < nodeCount; i++) {
        angle = PI * (changeInDeg * i) / 180;
        points[i] = (Vector2){ 
            position.x + (baseSize + nodeOffsets[i]) * cos(angle), 
            position.y - (baseSize + nodeOffsets[i]) * sin(angle) 
        };
    }

    // Draw the triangle fan with the given color
    DrawTriangleFan(points, nodeCount, RED);

}

void Planet::changeOffset(int node, int change) {
    nodeOffsets[node] = nodeOffsets[node] + change;
}


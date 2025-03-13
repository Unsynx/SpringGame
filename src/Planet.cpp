#include <raylib.h>
#include <raymath.h>
#include <cstdlib>
#include <cmath>
#include <vector>


#include "Planet.h"

Planet::Planet(int size, Vector2 position): baseSize(size), position(position) {
    float circumCircle = 2 * PI * baseSize;
    nodeCount = circumCircle / NODE_PER_SURFACE_LENGTH;

    // Surface Generation
    nodeOffsets.resize(nodeCount);
    for (int i = 0; i < nodeCount; i++) {
        nodeOffsets[i] = rand() % 10;
    }

    nodePositions.resize(nodeCount);
    updateNodePositions();

    TraceLog(LOG_INFO, "nodeCount: %d", nodeCount);
}

void Planet::updateNodePositions() {
    float angle = 0;
    float changeInDeg = 360 / (float)nodeCount;
    for (int i = 0; i < nodeCount; i++) {
        angle = PI * (changeInDeg * i) / 180;
        nodePositions[i] = (Vector2){ 
            position.x + (baseSize + nodeOffsets[i]) * cos(angle), 
            position.y - (baseSize + nodeOffsets[i]) * sin(angle) 
        };
    }
}

void Planet::draw() {
    // Draw the triangle fan with the given color
    DrawTriangleFan(nodePositions.data(), nodeCount, RED);
}

void Planet::changeOffset(int node, int change) {
    nodeOffsets[node] = nodeOffsets[node] + change;
    updateNodePositions();
}

Orientation Planet::planetToWorldCords(float planetPos) {
    planetPos = planetPos - floor(planetPos);

    int startNode = (int)floor(nodeCount * planetPos);
    int endNode = (startNode + 1) % nodeCount;

    // Compute world-space segment length
    float segmentLength = Vector2Distance(nodePositions[startNode], nodePositions[endNode]);

    // Compute how far `planetPos` is along this segment in world coordinates
    float segmentNormalizedLength = 1.0f / nodeCount;
    float distanceAlongSegment = (planetPos - (startNode * segmentNormalizedLength)) / segmentNormalizedLength * segmentLength;

    // Compute direction vector between nodes
    Vector2 direction = Vector2Normalize(Vector2Subtract(nodePositions[endNode], nodePositions[startNode]));

    // Compute world-space offset along the segment
    Vector2 offset = Vector2Scale(direction, distanceAlongSegment);

    // Return world position
    Orientation result;
    result.position = Vector2Add(nodePositions[startNode], offset);
    result.centerDirection = Vector2Normalize(Vector2Subtract(result.position, position));
    result.surfaceNormal = Vector2Rotate(direction, PI / 2);
    return result;
}
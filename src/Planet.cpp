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
    nodePositions[0] = position;
    float angle = 0;
    float changeInDeg = 360 / (float)(nodeCount - 2);
    for (int i = 1; i < nodeCount; i++) {
        nodePositions[i] = (Vector2){ 
            position.x + (baseSize + nodeOffsets[i]) * cos(angle), 
            position.y - (baseSize + nodeOffsets[i]) * sin(angle) 
        };
        TraceLog(LOG_INFO, TextFormat("%i @ angle: %f", i, angle));
        angle = PI * (changeInDeg * i) / 180;
    }

    nodePositions[nodeCount - 1] = nodePositions[1];
}

void Planet::draw() {
    // Draw the triangle fan with the given color
    DrawTriangleFan(nodePositions.data(), nodeCount, RED);

    /*
    Vector2 node = getNodePosition(0);
    DrawCircle(node.x, node.y, 5, GREEN);
    DrawText("0", node.x, node.y, 16, BLACK);

    node = getNodePosition(1);
    DrawCircle(node.x, node.y, 5, BLUE);
    DrawText("1", node.x, node.y, 16, BLACK);

    node = getNodePosition(2);
    DrawCircle(node.x, node.y, 5, YELLOW);
    DrawText("2", node.x, node.y, 16, BLACK);

    node = getNodePosition(nodeCount);
    DrawCircle(node.x, node.y, 5, BLUE);
    DrawText("nodeCount", node.x, node.y, 16, BLACK);
    */
}

void Planet::changeOffset(int node, int change) {
    nodeOffsets[node] = fmax(nodeOffsets[node] + change, CORE_SIZE - baseSize);
    updateNodePositions();
}

Orientation Planet::planetToWorldCords(float planetPos) {
    // Wrap planetPos to range [0,1)
    planetPos = fmod(planetPos, 1.0f);
    if (planetPos < 0) planetPos += 1.0f;

    // Compute valid node range [1, nodeCount - 2]
    int startNode = 1 + (int)floor((nodeCount - 2) * planetPos);
    int endNode = startNode + 1;

    // Wrap endNode if it exceeds valid range
    if (endNode >= nodeCount - 1) endNode = 1;

    // Compute world-space segment length
    float segmentLength = Vector2Distance(nodePositions[startNode], nodePositions[endNode]);

    // Compute how far `planetPos` is along this segment in world coordinates
    float segmentNormalizedLength = 1.0f / (nodeCount - 2);
    float distanceAlongSegment = (planetPos - ((startNode - 1) * segmentNormalizedLength)) / segmentNormalizedLength * segmentLength;

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

Segment Planet::getHoveredSegment(Vector2 worldMousePosition) {
    float anglePerNode = 360.0f / (nodeCount - 2);

    // Use atan2 to calculate angle correctly in all quadrants
    float angleFromCenter = atan2(position.y - worldMousePosition.y, worldMousePosition.x - position.x);
    angleFromCenter = fmod((angleFromCenter * 180 / PI) + 360, 360); // Normalize angle to [0, 360]

    Segment segment;
    segment.startNode = floor(angleFromCenter / anglePerNode) + 1;
    segment.endNode = (segment.startNode + 1) % nodeCount;
    if (segment.endNode == nodeCount - 1) {
        segment.endNode = 1;
    }
    segment.node1 = nodePositions[segment.startNode];
    segment.node2 = nodePositions[segment.endNode];
    
    return segment;
}
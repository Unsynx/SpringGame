#pragma once

#include <raylib.h>
#include <raymath.h>
#include <vector>

struct Orientation {
    Vector2 position;
    Vector2 surfaceNormal;
    Vector2 centerDirection;
};

struct Segment {
    int startNode;
    int endNode;
    Vector2 node1;
    Vector2 node2;
};

class Planet {
private:
    static const int NODE_PER_SURFACE_LENGTH = 15;
    static const int CORE_SIZE = 20;
    int nodeCount;
    std::vector<int> nodeOffsets;
    std::vector<Vector2> nodePositions;
    int baseSize;
    Vector2 position;
public:
    Planet(int size, Vector2 position);
    void updateNodePositions();
    void draw();
    void changeOffset(int node, int change);
    Orientation planetToWorldCords(float planetPos);
    Segment getHoveredSegment(Vector2 worldMousePosition);
    Vector2 getNodePosition(int node) { return nodePositions[node]; };
    Vector2 getPosition() { return position; }
    bool isColliding(Vector2 point);
    Vector2 getSurfaceNormal(Vector2 point);
};
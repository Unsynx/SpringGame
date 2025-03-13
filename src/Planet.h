#pragma once

#include <raylib.h>
#include <raymath.h>
#include <vector>

struct Orientation {
    Vector2 position;
    Vector2 surfaceNormal;
    Vector2 centerDirection;
};

class Planet {
private:
    static const int NODE_PER_SURFACE_LENGTH = 15;
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
};
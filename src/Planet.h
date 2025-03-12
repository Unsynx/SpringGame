#pragma once

#include <raylib.h>
#include <raymath.h>
#include <raymath.h>
#include <vector>

class Planet {
private:
    static const int NODE_PER_SURFACE_LENGTH = 15;
    int nodeCount;
    std::vector<int> nodeOffsets;
    int baseSize;
    Vector2 position;
public:
    Planet(int size, Vector2 position);
    void draw();
    void changeOffset(int node, int change);
    
};
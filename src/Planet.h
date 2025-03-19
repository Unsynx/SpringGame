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

struct OreDeposit {
    int centerNode;
    Vector2 centerNodeStart;
    int depth;
    int spread;
    bool mainNodeVisible = true;
    std::vector<Vector2> positions;
};

class Planet {
private:
    static const int NODE_PER_SURFACE_LENGTH = 15;
    static const int CORE_SIZE = 20;
    static const int ORE_PER_NODES = 10;
    int nodeCount;
    std::vector<int> nodeOffsets;
    std::vector<Vector2> nodePositions;
    int baseSize;
    Vector2 position;

    // Decorations
    std::vector<OreDeposit> oreDepots;
public:
    Planet() : baseSize(0), position({0, 0}) {}; // empty planet
    Planet(int size, Vector2 position);
    void updateNodePositions();
    void draw();
    void changeOffset(int node, int change);
    Orientation planetToWorldCords(float planetPos);
    Segment getHoveredSegment(Vector2 worldMousePosition);
    Vector2 getNodePosition(int node) { return nodePositions[node]; };
    Vector2 getNodePositionSafe(int node);
    Vector2 getPosition() { return position; }
    bool isColliding(Vector2 point);
    Vector2 getSurfaceNormal(Vector2 point);
    int getSize() { return baseSize; };
};

struct GravitySample {
    Vector2 gravity;
    int closestPlanet;
};

class PlanetSystem {
private:
    static const int SAMPLE_DISTANCE = 25;
    static constexpr float GRAVITY_CONSTANT = 0.00005f;
    int width = 100;
    int height = 100;
    Vector2 systemCenter;
    std::vector<Planet> planets;
    std::vector<std::vector<GravitySample>> gravityField;
public:
    PlanetSystem();
    void computeField();
    Vector2 gravityAt(Vector2 position);
    void drawField(Camera2D camera);
    void draw();
    Planet* getNearestPlanet(Vector2 position);
};
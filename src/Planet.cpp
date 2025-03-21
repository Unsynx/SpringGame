#include <raylib.h>
#include <raymath.h>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <random>


#include "Planet.h"
#include "SceneManager.h"
#include "Game.cpp"

// ----------- Helper Functions -----------

// Returns a random point inside the triangle formed by A, B, and C
Vector2 pointOnTriangle(const Vector2& pt1, const Vector2& pt2, const Vector2& pt3) {
    // this function doesn't seem to work
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dist(0.0f, 1.0f);

    // Generate two random values and sort them
    float x = dist(gen);
    float y = dist(gen);
    if (x > y) std::swap(x, y);

    float s = x;
    float t = y - x;
    float u = 1.0f - y;

    // Compute the final random point inside the triangle
    return (Vector2){
        s * pt1.x + t * pt2.x + u * pt3.x,
        s * pt1.y + t * pt2.y + u * pt3.y
    };
}

void DrawArrow(Vector2 start, Vector2 end, float arrowSize = 10.0f, float lineThickness = 2.0f, Color color = BLACK) {
    // Draw main line
    DrawLineEx(start, end, lineThickness, color);

    // Calculate direction
    Vector2 direction = Vector2Subtract(end, start);
    float length = Vector2Length(direction);
    if (length == 0) return;  // Avoid division by zero

    direction = Vector2Scale(direction, 1.0f / length); // Normalize

    // Calculate arrowhead base position (a bit before the end)
    Vector2 arrowBase = Vector2Add(end, Vector2Scale(direction, -arrowSize));

    // Perpendicular direction for arrowhead
    Vector2 perp = { -direction.y, direction.x };

    // Calculate arrowhead points
    Vector2 arrowLeft = Vector2Add(arrowBase, Vector2Scale(perp, arrowSize * 0.5f));
    Vector2 arrowRight = Vector2Subtract(arrowBase, Vector2Scale(perp, arrowSize * 0.5f));

    // Draw arrowhead
    DrawTriangle(end, arrowRight, arrowLeft, color);
}

// ----------- Planet -----------

Vector2 Planet::getNodePositionSafe(int node) {
    // Map node to the range [0, nodeCount-3]
    node = ((node % (nodeCount - 2)) + (nodeCount - 2)) % (nodeCount - 2); // Correct negative modulus
    if (node == 0) node = 1; // Ensure node is not zero

    return nodePositions[node];
}

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

    // Ore generation
    std::random_device rd;
    std::mt19937 gen(rd()); // Mersenne Twister engine
    std::uniform_int_distribution<> dis(1, nodeCount - 2); // Range from 1 to nodeCount

    int oreDepotCount = (nodeCount - 2) / ORE_PER_NODES;
    oreDepots.resize(oreDepotCount);
    for (int i = 0; i < oreDepotCount; i++) {
        OreDeposit depot;
        depot.centerNode = dis(gen);
        depot.centerNodeStart = nodePositions[depot.centerNode];
        depot.depth = 50;
        depot.spread = 3;
        depot.positions.resize(10);
        for (int i = 0; i < 10; i++) {
            Vector2 centerDirection = Vector2Normalize(Vector2Subtract(getPosition(), getNodePositionSafe(depot.centerNode)));
            depot.positions[i] = pointOnTriangle(
                Vector2Add(getNodePositionSafe(depot.centerNode), Vector2Scale(centerDirection, depot.depth)), 
                getNodePositionSafe(depot.centerNode - depot.spread),
                getNodePositionSafe(depot.centerNode + depot.spread)
            );
        }
        oreDepots[i] = depot;
    }
}

void Planet::updateNodePositions() {
    // Update surface
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

    GameScene* game = dynamic_cast<GameScene*>(SCENE_MANAGER.getCurrentScene());

    // Check ores
    for (OreDeposit& depot : oreDepots) {  // Use reference to modify elements
        for (int i = depot.positions.size() - 1; i >= 0; i--) {  // Iterate backwards
            if (depot.mainNodeVisible && depot.centerNodeStart != nodePositions[depot.centerNode]) {
                depot.mainNodeVisible = false;
                game->addPoints(5);
            }
            if (!isColliding(depot.positions[i])) {
                game->addPoints(1);
                depot.positions.erase(depot.positions.begin() + i);
            }
        }
    }
}

void Planet::draw() {
    for (OreDeposit depot : oreDepots) {
        Vector2 mainNodePos = nodePositions[depot.centerNode];
        if (depot.mainNodeVisible) DrawCircle(mainNodePos.x, mainNodePos.y, 15, YELLOW);
        for (Vector2 position: depot.positions) {
            DrawCircle(position.x, position.y, 15, BLUE);
        }
    }

    // Planet Surface
    DrawTriangleFan(nodePositions.data(), nodeCount, RED);
}

void Planet::changeOffset(int node, int change) {
    nodeOffsets[node] = fmax(nodeOffsets[node] + change, CORE_SIZE - baseSize);
    updateNodePositions(); // Shouldn't call this function everytiem something is changed, rather should be called only once at the end of all changes
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

bool Planet::isColliding(Vector2 point) {
    return CheckCollisionPointPoly(point, nodePositions.data(), nodeCount);
}

Vector2 Planet::getSurfaceNormal(Vector2 point) {
    // Find the closest segment
    Segment segment = getHoveredSegment(point);

    // Compute direction vector along the surface segment
    Vector2 direction = Vector2Normalize(Vector2Subtract(segment.node2, segment.node1));

    // Rotate 90 degrees to get the normal
    return Vector2Rotate(direction, PI / 2);
}

// ----------- Planet System -----------

PlanetSystem::PlanetSystem() {
    systemCenter = Vector2Scale((Vector2){ (float)(width * SAMPLE_DISTANCE), (float)(height * SAMPLE_DISTANCE) }, 0.5);

    planets.resize(2);
    planets[0] = Planet(50, Vector2AddValue(systemCenter, -200));
    planets[1] = Planet(150, systemCenter);

    gravityField.resize(width, std::vector<GravitySample>(height));
}

void PlanetSystem::computeField() {
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            GravitySample sample;
            sample.gravity = Vector2Zero();

            Vector2 samplePosition = (Vector2){ (float)(x * SAMPLE_DISTANCE), (float)(y * SAMPLE_DISTANCE) };
            
            float closestPlanetDistance = INFINITY;
            for (int i = 0; i < planets.size(); i++) {
                // Find closest planet
                float distance = Vector2Distance(samplePosition, planets[i].getPosition());
                float distanceFromSurface = distance - planets[i].getSize();
                if (distanceFromSurface < closestPlanetDistance) { 
                    closestPlanetDistance = distanceFromSurface;
                    sample.closestPlanet = i; 
            }

                // Find planet gravity
                float gravityMag = GRAVITY_CONSTANT * -(float)(PI * pow(planets[i].getSize(), 2)) / (distance * distance);
                Vector2 gravityAtSample = Vector2Scale(Vector2Subtract(samplePosition, planets[i].getPosition()), gravityMag);
                sample.gravity = Vector2Add(sample.gravity, gravityAtSample);
            }

            TraceLog(LOG_INFO, TextFormat("Sample: [%f, %f]", sample.gravity.x, sample.gravity.y));
            gravityField[y][x] = sample;
        }
    }

}

Vector2 PlanetSystem::gravityAt(Vector2 position) {
    // Get grid cell coordinates
    int xStart = Clamp(floor(position.x / SAMPLE_DISTANCE), 1, width - 2);
    int yStart = Clamp(floor(position.y / SAMPLE_DISTANCE), 1, height - 2);

    // Get interpolation factors
    float tx = (position.x / SAMPLE_DISTANCE) - xStart;
    float ty = (position.y / SAMPLE_DISTANCE) - yStart;

    // Get gravity samples at the four corners
    Vector2 g00 = gravityField[yStart][xStart].gravity;
    Vector2 g10 = gravityField[yStart][xStart + 1].gravity;
    Vector2 g01 = gravityField[yStart + 1][xStart].gravity;
    Vector2 g11 = gravityField[yStart + 1][xStart + 1].gravity;

    // Bilinear interpolation
    Vector2 gTop = Vector2Lerp(g00, g10, tx);  // Interpolate top row
    Vector2 gBottom = Vector2Lerp(g01, g11, tx);  // Interpolate bottom row
    Vector2 gravity = Vector2Lerp(gTop, gBottom, ty);  // Final interpolation

    return gravity;
}

void PlanetSystem::drawField(Camera2D camera) {
    // Flow field
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            Vector2 position = (Vector2){ (float)(x * SAMPLE_DISTANCE), (float)(y * SAMPLE_DISTANCE) };
            GravitySample sample = gravityField[y][x];

            if (Vector2Length(Vector2Subtract(position, camera.target)) < 200) {
                DrawArrow(position, Vector2Add(position, sample.gravity * 1000), 6, 2, GRAY);
            }
        }
    }

    // Gravity at mouse
    Vector2 position = GetScreenToWorld2D(GetMousePosition(), camera);
    DrawArrow(position, Vector2Add(position, gravityAt(position) * 1000), 6, 2, GREEN);

    // Boundary
    DrawRectangleLines(0, 0, width * SAMPLE_DISTANCE, height * SAMPLE_DISTANCE, GRAY);
}

void PlanetSystem::drawPrettyBackdrop(Camera2D camera) {
    DrawRectangleLines(0, 0, width * SAMPLE_DISTANCE, height * SAMPLE_DISTANCE, GRAY);

    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            Vector2 position = (Vector2){ (float)(x * SAMPLE_DISTANCE), (float)(y * SAMPLE_DISTANCE) };
            GravitySample sample = gravityField[y][x];

            if (Vector2Length(Vector2Subtract(position, camera.target)) < 300) {
                DrawCircle(position.x, position.y, 2, LIGHTGRAY);
            }
        }
    }
}

void PlanetSystem::draw() {
    for (Planet planet : planets) {
        planet.draw();
    }
}

Planet* PlanetSystem::getNearestPlanet(Vector2 position) {
    // Get grid cell coordinates
    int x = Clamp(round(position.x / SAMPLE_DISTANCE), 0, width - 1);
    int y = Clamp(round(position.y / SAMPLE_DISTANCE), 0, height - 1);

    return &planets[gravityField[y][x].closestPlanet];
}
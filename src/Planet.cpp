#include <raylib.h>
#include <raymath.h>
#include <cstdlib>
#include <cmath>
#include <vector>


#include "Planet.h"

// ----------- Planet -----------

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
    planets.resize(2);
    planets[0] = Planet(50, (Vector2){240, 180});
    planets[1] = Planet(200, (Vector2){0, 0});

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
                if (distance < closestPlanetDistance) { 
                    closestPlanetDistance = distance;
                    sample.closestPlanet = i; 
            }

                // Find planet gravity
                float gravityMag = -(float)(PI * pow(planets[i].getSize(), 2)) / (distance * distance);
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
    int xStart = Clamp(floor(position.x / SAMPLE_DISTANCE), 0, width - 2);
    int yStart = Clamp(floor(position.y / SAMPLE_DISTANCE), 0, height - 2);

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

void PlanetSystem::drawField(Camera2D camera) {
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            Vector2 position = (Vector2){ (float)(x * SAMPLE_DISTANCE), (float)(y * SAMPLE_DISTANCE) };
            GravitySample sample = gravityField[y][x];

            DrawArrow(position, Vector2Add(position, sample.gravity / 20), 6, 2, GREEN);
        }
    }

    Vector2 position = GetScreenToWorld2D(GetMousePosition(), camera);
    DrawArrow(position, Vector2Add(position, gravityAt(position) / 20), 6, 2, GREEN);
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
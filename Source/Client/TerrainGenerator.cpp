#include "TerrainGenerator.h"
#include "HgLogger.h"
#include <SceneManager.h>
#include <algorithm>
#include <cmath>

using namespace client;
using namespace core;
using namespace math;

void TerrainGenerator::initialize(){
    m_width = 50.0f;
    m_length = 50.0f;
    m_maxHeight = 2.0f;
    HgLogger::logDebug("TerrainGenerator initialized with width: %f, length: %f, maxHeight: %f", m_width, m_length, m_maxHeight);
    createMesh();
}

void TerrainGenerator::createMesh(){
    //Mesh creation logic would go here
    HgLogger::logDebug("Creating terrain mesh...");

    Mesh terrainMesh = Mesh();
    PointArray points;

    // Choose how many samples (vertices) per unit length.
    // Increase samplesPerUnit to make spacing smaller (more vertices).
    // Example: samplesPerUnit = 10 -> 10 samples per unit length (segments), vertices = segments + 1
    const int samplesPerUnit = 10;

    //generate points
    m_vertexCountX = std::max(2, static_cast<int>(std::ceil(m_width * samplesPerUnit)) + 1);
    m_vertexCountZ = std::max(2, static_cast<int>(std::ceil(m_length * samplesPerUnit)) + 1);

    float widthSpacing = m_width / static_cast<float>(m_vertexCountX - 1);
    float lengthSpacing = m_length / static_cast<float>(m_vertexCountZ - 1);

    // compute offsets so center of mesh sits at (0, 0) in X,Z
    const float halfWidth = m_width * 0.5f;
    const float halfLength = m_length * 0.5f;

    // Use integer loops for vertex counts and compute positions from indices.
    // This ensures we produce exactly m_vertexCountX * m_vertexCountZ vertices
    // and include the final row/column.
    for (int ix = 0; ix < m_vertexCountX; ++ix) {
        float x = ix * widthSpacing - halfWidth; // centered X
        for (int iz = 0; iz < m_vertexCountZ; ++iz) {
            float z = iz * lengthSpacing - halfLength; // centered Z
            //TODO: add height generation logic here
            float y = std::sin(x + z) / 10.0f;
            points.push_back(Point(x, y, z));
        }
    }

    HgLogger::logDebug("Generated %zu points for terrain mesh.", points.toFloatVector().size() / 3);

    //generate indices
    std::vector<unsigned int> indices;
    indices.reserve(static_cast<size_t>((m_vertexCountX - 1) * (m_vertexCountZ - 1) * 6));
    for (int ix = 0; ix < m_vertexCountX - 1; ++ix) {
        for (int iz = 0; iz < m_vertexCountZ - 1; ++iz) {

            int topLeft = (ix * m_vertexCountZ) + iz;
            int topRight = topLeft + 1;
            int bottomLeft = ((ix + 1) * m_vertexCountZ) + iz;
            int bottomRight = bottomLeft + 1;

            indices.push_back(topLeft);
            indices.push_back(bottomLeft);
            indices.push_back(topRight);
            indices.push_back(topRight);
            indices.push_back(bottomLeft);
            indices.push_back(bottomRight);
        }
    }

    HgLogger::logDebug("Generated %zu indices for terrain mesh.", indices.size());

    terrainMesh.setPoints(points, false);
    terrainMesh.setIndices(indices);

    m_parent->setDirty(true);
    m_parent->setMesh(terrainMesh);
}

void TerrainGenerator::update(){
    //Terrain generation logic would go here
}

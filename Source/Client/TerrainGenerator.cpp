#include "TerrainGenerator.h"
#include "HgLogger.h"
#include <algorithm>
#include <cmath>
#include <glm/glm.hpp>
#include <vector>
#include "Noise.h"
#include <Mesh.h>

using namespace client;
using namespace core;
using namespace math;

void TerrainGenerator::initialize(){
    m_width = 10.0f;
    m_length = 10.0f;
    m_maxHeight = 1.0f;
    m_parent->setScale(glm::vec3(10.0f, 2.0f, 10.0f));
    HgLogger::logDebug("TerrainGenerator initialized with width: %f, length: %f, maxHeight: %f", m_width, m_length, m_maxHeight);
    //TODO: random seed
    uint64_t seed = 12345;
    m_noiseGenerator0 = Noise(seed);
    m_noiseGenerator1 = Noise(seed * 2);

    createMesh();

}

void TerrainGenerator::createMesh(){
    //Mesh creation logic would go here
    HgLogger::logDebug("Creating terrain mesh...");

    Mesh terrainMesh = Mesh();
    
    createPoints(&terrainMesh);

    createIndices(&terrainMesh);
    
    createNormals(&terrainMesh);

    m_parent->setDirty(true);
    m_parent->setMesh(terrainMesh);
}

void TerrainGenerator::createPoints(core::Mesh* mesh){
    PointArray points;

    // Choose how many samples (vertices) per unit length.
    const int samplesPerUnit = 16;

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
            float y = calculateHeight(x, z);

            points.push_back(Point(x, y, z));
        }
    }
    mesh->setPoints(points, false);

    HgLogger::logDebug("Generated %zu points for terrain mesh.", points.toFloatVector().size() / 3);

}

void TerrainGenerator::createIndices(core::Mesh* mesh){
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

    // Set vertex positions on mesh
    mesh->setIndices(indices);

}

void TerrainGenerator::createNormals(core::Mesh* mesh){
    //Normal creation logic would go here.

    // --- Compute vertex normals ---
    // Convert point array to float vector for easy indexed access (x,y,z)
    std::vector<float> posFloats = mesh->getPoints().toFloatVector();
    const size_t vertexCount = posFloats.size() / 3;
    std::vector<glm::vec3> normalAcc(vertexCount, glm::vec3(0.0f));

    const std::vector<unsigned int> indices = mesh->getIndices();
    // For each triangle, compute face normal (area-weighted) and accumulate to each vertex
    for (size_t i = 0; i + 2 < indices.size(); i += 3) {
        unsigned int i0 = indices[i + 0];
        unsigned int i1 = indices[i + 1];
        unsigned int i2 = indices[i + 2];

        glm::vec3 v0(
            posFloats[i0 * 3 + 0],
            posFloats[i0 * 3 + 1],
            posFloats[i0 * 3 + 2]
        );
        glm::vec3 v1(
            posFloats[i1 * 3 + 0],
            posFloats[i1 * 3 + 1],
            posFloats[i1 * 3 + 2]
        );
        glm::vec3 v2(
            posFloats[i2 * 3 + 0],
            posFloats[i2 * 3 + 1],
            posFloats[i2 * 3 + 2]
        );

        glm::vec3 edge1 = v1 - v0;
        glm::vec3 edge2 = v2 - v0;
        glm::vec3 faceNormal = glm::cross(edge1, edge2); // area-weighted

        normalAcc[i0] += faceNormal;
        normalAcc[i1] += faceNormal;
        normalAcc[i2] += faceNormal;
    }

    // Normalize accumulated normals and create PointArray for normals
    PointArray normals;
    normals = PointArray(); // ensure default constructed
    for (size_t vi = 0; vi < vertexCount; ++vi) {
        glm::vec3 n = normalAcc[vi];
        float len = glm::length(n);
        if (len > 1e-6f) {
            n = glm::normalize(n);
        }
        else {
            // Fallback normal (up)
            n = glm::vec3(0.0f, 1.0f, 0.0f);
        }
        normals.push_back(Point(n.x, n.y, n.z));
    }

    mesh->setNormals(normals);

    HgLogger::logDebug("Generated %zu normals for terrain mesh.", vertexCount);
}


float TerrainGenerator::calculateHeight(float x, float z){
    //TODO: I'm not too sold on this height calculation method, may change later
    // I want valleys and hills, but not too extreme, and I want some good variance over distances.


    float weight0 = 1.0f;
    float weight1 = 0.5f;
    float weight2 = 0.25f;

    float weight3 = 0.5f;
    float weight4 = 0.25f;

    float noise0 = m_noiseGenerator0.generateNoise2d(x, z) * weight0;
    float noise1 = m_noiseGenerator1.generateNoise2d(x * 2.0f, z * 2.0f) * weight1;
    float noise2 = m_noiseGenerator1.generateNoise2d(4.0f * x + 4.0f, 4.0f * z + 4.0f) * weight2;

    float noise3 = Noise::simplexNoise(x, z, noise0) * weight3;
    float noise4 = Noise::simplexNoise(x * 0.5f, z * 0.5f, noise1) * weight4;

    float finalHeight = (noise0 + noise1 + noise2 + noise3 + noise4) / (weight0 + weight1 + weight2 + weight3 + weight4);

    return finalHeight * 0.1f;
}

void TerrainGenerator::update(){
    //Terrain generation logic would go here
}

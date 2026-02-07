#include "Chunk.h"
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

Chunk::Chunk(uint32_t id, glm::vec3 pos, uint64_t seed) : Entity(id){
    //need to be sure it is initialized with a position, otherwise the noise value will be wrong.
    setPosition(pos);
    m_seed = seed;
    createMesh();
    m_noiseGenerator0 = Noise(seed);
    m_noiseGenerator1 = Noise(seed * 2);
}


void Chunk::createMesh() {
    //Mesh creation logic would go here

    Mesh* terrainMesh = new Mesh();

    createPoints(terrainMesh);

    createIndices(terrainMesh);

    createNormals(terrainMesh);

    setDirty(true);
    setMesh(terrainMesh);
}

void Chunk::createPoints(core::Mesh* mesh) {
    // Choose how many samples (vertices) per unit length.
    const int samplesPerUnit = 2;

    m_vertexCountX = std::max(2, static_cast<int>(std::ceil(CHUNK_SIZE * samplesPerUnit)) + 1);
    m_vertexCountZ = std::max(2, static_cast<int>(std::ceil(CHUNK_SIZE * samplesPerUnit)) + 1);

    float widthSpacing = CHUNK_SIZE / static_cast<float>(m_vertexCountX - 1);
    float lengthSpacing = CHUNK_SIZE / static_cast<float>(m_vertexCountZ - 1);
    const float halfWidth = CHUNK_SIZE * 0.5f;
    const float halfLength = CHUNK_SIZE * 0.5f;

    // Precompute world position to avoid repeated getPosition() calls
    glm::vec3 chunkPos = getPosition();

    PointArray points;
    for (int ix = 0; ix < m_vertexCountX; ++ix) {
        float x = ix * widthSpacing - halfWidth; // centered X
        for (int iz = 0; iz < m_vertexCountZ; ++iz) {
            float z = iz * lengthSpacing - halfLength; // centered Z
            float y = calculateHeight(x, z, chunkPos); // pass chunkPos to avoid repeated getPosition()
            points.push_back(Point(x, y, z));
        }
    }
    mesh->setPoints(points, false);

}

void Chunk::createIndices(core::Mesh* mesh) {
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
    // Set vertex positions on mesh
    mesh->setIndices(indices);

}

void Chunk::createNormals(core::Mesh* mesh) {
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
    for (size_t vi = 0; vi < vertexCount; ++vi) {
        glm::vec3 n = normalAcc[vi];
        float len = glm::length(n);
        if (len > 1e-6f) {
            n = glm::normalize(n);
        } else {
            n = glm::vec3(0.0f, 1.0f, 0.0f);
        }
        normals.push_back(Point(n.x, n.y, n.z));
    }
    mesh->setNormals(normals);

}


// Overload to accept chunk position to avoid repeated getPosition() calls
float Chunk::calculateHeight(float x, float z, const glm::vec3& chunkPos) {
    float world_x = chunkPos.x + x;
    float world_z = chunkPos.z + z;

    constexpr float weight0 = 1.0f;
    constexpr float weight1 = 0.5f;
    constexpr float weight2 = 0.25f;
    constexpr float weight3 = 0.5f;
    constexpr float weight4 = 0.25f;

    float noise0 = m_noiseGenerator0.generateNoise2d(world_x, world_z) * weight0;
    float noise1 = m_noiseGenerator1.generateNoise2d(world_x * 2.0f, world_z * 2.0f) * weight1;
    float noise2 = m_noiseGenerator1.generateNoise2d(4.0f * world_x + 4.0f, 4.0f * world_z + 4.0f) * weight2;

    float noise3 = Noise::simplexNoise(world_x, world_z, noise0) * weight3;
    float noise4 = Noise::simplexNoise(world_x * 0.5f, world_z * 0.5f, noise1) * weight4;

    float finalHeight = (noise0 + noise1 + noise2 + noise3 + noise4) / (weight0 + weight1 + weight2 + weight3 + weight4);

    return finalHeight * 0.1f;
}

// Backward compatibility for existing calls
float Chunk::calculateHeight(float x, float z) {
    return calculateHeight(x, z, getPosition());
}

void Chunk::onCollision(){
    //do nothing - we don't currently care about chunk collisions.
    //HgLogger::logMsg("Chunk collision triggered!");
}

Chunk::~Chunk(){
    HgLogger::logDebug("Destroying chunk with id: %u", getId());
}
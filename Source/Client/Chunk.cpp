#include "Chunk.h"
#include "HgLogger.h"
#include <algorithm>
#include <cmath>
#include <glm/glm.hpp>
#include <vector>
#include "Math/Noise.h"
#include "Core/Mesh.h"

using namespace client;
using namespace core;
using namespace math;

Chunk::Chunk(uint32_t id, glm::vec3 pos, uint64_t seed) : Entity(id){
    //need to be sure it is initialized with a position, otherwise the noise value will be wrong.
    setPosition(pos);
    m_seed = seed;
    //TODO:probably revisit the different seeds
    m_BaseTerrainNoiseGenerator = Noise(m_seed);
    m_SecondaryTerrainNoiseGenerator = Noise(m_seed * 2);
    m_BiomeNoiseGenerator = Noise(m_seed * 3);
    createMesh();
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

    // Sample biome noise and map from [-1, 1] to [0, 1]
    float biomeValue = m_BiomeNoiseGenerator.generateNoise2d(world_x * 0.001, world_z * 0.005);
    float t = (biomeValue + 1.0f) * 0.5f; // t in [0, 1]

    // Define biome thresholds (adjust as needed)
    float plainsEnd = 0.50f;
    float hillsEnd = 0.70f;

    // Calculate heights for each biome
    float plainsHeight = calculatePlains(world_x, world_z);
    float hillsHeight = calculateHills(world_x, world_z);
    float mountainsHeight = calculateMountains(world_x, world_z);

    float height = 0.0f;
    if (t < plainsEnd) {
        // Lerp between plains and hills
        float localT = t / plainsEnd;
        height = std::lerp(plainsHeight, hillsHeight, localT);
    } else if (t < hillsEnd) {
        // Lerp between hills and mountains
        float localT = (t - plainsEnd) / (hillsEnd - plainsEnd);
        height = std::lerp(hillsHeight, mountainsHeight, localT);
    } else {
        // Use mountains
        height = mountainsHeight;
    }

    return height;
}

float Chunk::calculateHeight(float x, float z) {
    return calculateHeight(x, z, getPosition());
}

float Chunk::calculatePlains(float x, float z) {

    // Use low-frequency noise for smoothness
    float baseFreq = 0.05f; // Lower = smoother, broader hills
    float noise0 = m_BaseTerrainNoiseGenerator.generateNoise2d(x * baseFreq, z * baseFreq);

    // Optionally add a very subtle higher-frequency layer for gentle detail
    float detailFreq = 0.1f;
    float noise1 = m_SecondaryTerrainNoiseGenerator.generateNoise2d(x * detailFreq, z * detailFreq) * 0.2f;

    // Combine and normalize
    float height = (noise0 + noise1) / 1.2f; // Weighted sum

    // Scale to desired amplitude (e.g., 0.0 to 1.0, then scale down for gentle hills)
    return height * 0.15f; // 0.15f controls the max height of hills

}

float Chunk::calculateHills(float x, float z) {
    // Use low-frequency noise for smoothness
    float baseFreq = 0.05f; // Lower = smoother, broader hills
    float noise0 = m_BaseTerrainNoiseGenerator.generateNoise2d(x * baseFreq, z * baseFreq);

    // Optionally add a very subtle higher-frequency layer for gentle detail
    float detailFreq = 0.1f;
    float noise1 = m_SecondaryTerrainNoiseGenerator.generateNoise2d(x * detailFreq, z * detailFreq) * 0.2f;

    // Combine and normalize
    float height = (noise0 + noise1) / 1.2f; // Weighted sum

    // Scale to desired amplitude (e.g., 0.0 to 1.0, then scale down for gentle hills)
    return height; // 0.15f controls the max height of hills
}

float Chunk::calculateMountains(float x, float z) {
    // Use low-frequency noise for smooth, broad mountains
    float baseFreq = 0.07f; // Slightly higher than hills for more variation, but still smooth
    float noise0 = m_BaseTerrainNoiseGenerator.generateNoise2d(x * baseFreq, z * baseFreq);

    // Optional: add a subtle detail layer
    float detailFreq = 0.14f;
    float noise1 = m_SecondaryTerrainNoiseGenerator.generateNoise2d(x * detailFreq, z * detailFreq) * 0.2f;

    // Combine and normalize
    float height = (noise0 + noise1) / 1.2f;
    float finalHeight = std::pow(std::max(0.0f, height), 3.0f);
    // Scale up for tall, smooth mountains
    return height * 10.0f; // 2.5f controls the max height of mountains
}


void Chunk::onCollision(){
    //do nothing - we don't currently care about chunk collisions.
    //HgLogger::logMsg("Chunk collision triggered!");
}

Chunk::~Chunk(){
    HgLogger::logDebug("Destroying chunk with id: %u", getId());
}
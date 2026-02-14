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

float Chunk::getPointHeight(float worldX, float worldZ) {
    // Map world coordinates to chunk coordinates
    glm::vec3 chunkPos = getPosition();
    float localX = worldX - chunkPos.x;
    float localZ = worldZ - chunkPos.z;

    // Calculate the grid cell coordinates
    int gridX = static_cast<int>((localX / CHUNK_SIZE) * m_vertexCountX);
    int gridZ = static_cast<int>((localZ / CHUNK_SIZE) * m_vertexCountZ);

    // Clamp to valid range
    gridX = std::clamp(gridX, 0, m_vertexCountX - 1);
    gridZ = std::clamp(gridZ, 0, m_vertexCountZ - 1);

    return m_heightMap[gridX * m_vertexCountX + gridZ];
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

    //has to be allocated after vertex counts are determined
    m_heightMap = new float[m_vertexCountX * m_vertexCountZ];

    // Precompute world position to avoid repeated getPosition() calls
    glm::vec3 chunkPos = getPosition();

    PointArray points;
    PointArray colours;
    for (int ix = 0; ix < m_vertexCountX; ++ix) {
        float x = ix * widthSpacing - halfWidth; // centered X
        for (int iz = 0; iz < m_vertexCountZ; ++iz) {
            float z = iz * lengthSpacing - halfLength; // centered Z
            
            float world_x = chunkPos.x + x;
            float world_z = chunkPos.z + z;

            float y = calculateHeight(world_x, world_z);
            m_heightMap[ix * m_vertexCountX + iz] = y; // Store height in heightmap for potential future use
            points.push_back(Point(x, y, z));
            sBiome biome = getBiomeType(world_x, world_z);
            if (biome.type == Lake) {
                float blue = biome.transition / 2.0f; // Transition from 0.5 to 1.0 as we go from deep lake to plains
                colours.push_back(Point(blue, blue, 0.8f)); // Placeholder color (blue)
            } else {
                colours.push_back(Point(0.0f, 1.0f, 0.0f)); // Placeholder color (green)
            }

        }
    }
    mesh->setPoints(points, false);
    mesh->setColours(colours);

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

Chunk::sBiome Chunk::getBiomeType(float x, float z) {
    // Sample biome noise and map from [-1, 1] to [0, 1]
    float biomeValue = m_BiomeNoiseGenerator.generateNoise2d(x * 0.001, z * 0.005);
    float t = (biomeValue + 1.0f) * 0.5f; // t in [0, 1]

    // Define biome thresholds (adjust as needed)
    float lakeThreshold = 0.3f; // Below this is lake
    float plainsEnd = 0.50f;
    float hillsEnd = 0.70f;

    if (t < lakeThreshold) {
        float transition = t / lakeThreshold; // Transition from 0 to 1 as we go from deep lake to plains
        return { Lake,  transition * 0.95f};
    }

    if (t < plainsEnd) {
        float transition = (t - lakeThreshold) / (plainsEnd - lakeThreshold); // Transition from 0 to 1 as we go from deep lake to plains

        return { Plains, transition };
    }

    if (t < hillsEnd) {
        float transition = (t - plainsEnd) / (hillsEnd - plainsEnd); // Transition from 0 to 1 as we go from deep lake to plains

        return { Hills, transition };
    }
    float transition = (t - hillsEnd) / (1.0f - hillsEnd); // Transition from 0 to 1 as we go from deep lake to plains
    return { Mountains, transition };
}


float Chunk::calculateHeight(float x, float z) {

    // Calculate heights for each biome
    float plainsHeight = calculatePlains(x, z);
    float hillsHeight = calculateHills(x, z);
    float mountainsHeight = calculateMountains(x, z);
    float lakesHeight = calculateLakes(x, z);

    float height = 0.0f;
    Chunk::sBiome biome = getBiomeType(x, z);

    switch (biome.type) {
    case Lake:
        height = std::lerp(lakesHeight, plainsHeight, biome.transition);
        break;
    case Plains:
        height = std::lerp(plainsHeight, hillsHeight, biome.transition);
        break;
    case Hills:
        height = std::lerp(hillsHeight, mountainsHeight, biome.transition);
        break;
    case Mountains:
        height = std::lerp(mountainsHeight, hillsHeight, biome.transition);
        break;

    }

    return height;
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

float Chunk::calculateLakes(float x, float z) {

    // Use low-frequency noise for smoothness
    float baseFreq = 0.05f; // Lower = smoother, broader hills
    float noise0 = m_BaseTerrainNoiseGenerator.generateNoise2d(x * baseFreq, z * baseFreq);

    // Optionally add a very subtle higher-frequency layer for gentle detail
    float detailFreq = 0.1f;
    float noise1 = m_SecondaryTerrainNoiseGenerator.generateNoise2d(x * detailFreq, z * detailFreq) * 0.2f;

    // Combine and normalize
    float height = (noise0 + noise1) / 1.2f; // Weighted sum

    // Scale to desired amplitude (e.g., 0.0 to 1.0, then scale down for gentle hills)
    return (height * 0.15f) - 3.0f; // 0.15f controls the max height of hills
}

bool Chunk::setPointHeight(float worldX, float worldZ, float newY) {
    core::Mesh* mesh = getMesh();
    if (!mesh) {
        return false;
    }

    glm::vec3 chunkPos = getPosition();
    
    // Convert world coordinates to chunk-local coordinates
    float localX = worldX - chunkPos.x;
    float localZ = worldZ - chunkPos.z;

    // Calculate grid spacing
    float widthSpacing = CHUNK_SIZE / static_cast<float>(m_vertexCountX - 1);
    float lengthSpacing = CHUNK_SIZE / static_cast<float>(m_vertexCountZ - 1);
    const float halfWidth = CHUNK_SIZE * 0.5f;
    const float halfLength = CHUNK_SIZE * 0.5f;

    // Convert local coordinates to grid indices
    // Reverse the transformation from createPoints: x = ix * widthSpacing - halfWidth
    float ixFloat = (localX + halfWidth) / widthSpacing;
    float izFloat = (localZ + halfLength) / lengthSpacing;

    // Round to nearest vertex
    int ix = static_cast<int>(std::round(ixFloat));
    int iz = static_cast<int>(std::round(izFloat));

    // Check if indices are within bounds
    if (ix < 0 || ix >= m_vertexCountX || iz < 0 || iz >= m_vertexCountZ) {
        return false; // Point is outside chunk bounds
    }

    // Get the point array
    PointArray points = mesh->getPoints();

    // Define the radius of influence (in grid units)
    const int influenceRadius = 3; // Affects points within 3 vertices in each direction

    // Iterate through all vertices within the influence radius
    for (int dx = -influenceRadius; dx <= influenceRadius; ++dx) {
        for (int dz = -influenceRadius; dz <= influenceRadius; ++dz) {
            int targetIx = ix + dx;
            int targetIz = iz + dz;

            // Skip if out of bounds
            if (targetIx < 0 || targetIx >= m_vertexCountX || 
                targetIz < 0 || targetIz >= m_vertexCountZ) {
                continue;
            }

            // Calculate vertex index
            int vertexIndex = (targetIx * m_vertexCountZ) + targetIz;

            // Verify vertex index is valid
            if (vertexIndex < 0 || vertexIndex >= points.size()) {
                continue;
            }

            // Calculate distance from target point to current vertex
            float distance = std::sqrt(static_cast<float>(dx * dx + dz * dz));

            // Calculate influence weight using smooth falloff (inverse quadratic)
            // At distance 0, weight = 1.0; at influenceRadius, weight approaches 0
            float weight = 1.0f - (distance / static_cast<float>(influenceRadius));
            weight = std::max(0.0f, weight); // Clamp to [0, 1]
            weight = weight * weight; // Square for smoother falloff

            // Get the existing point
            Point& oldPoint = points[vertexIndex];
            float currentY = static_cast<float>(oldPoint.Y());

            // Lerp between current height and new height based on weight
            float targetY = std::lerp(currentY, newY, weight);

            // Create updated point
            Point updatedPoint(static_cast<float>(oldPoint.X()), targetY, static_cast<float>(oldPoint.Z()));
            
            // Update the point in the array
            points.setPoint(vertexIndex, updatedPoint);
        }
    }

    // Update the mesh with modified points
    mesh->setPoints(points, false);

    // Recalculate normals to reflect the height changes
    createNormals(mesh);

    // Mark chunk as dirty for re-rendering
    setDirty(true);

    return true;
}

bool Chunk::setRegionHeight(float centerWorldX, float centerWorldZ, float radius, float newY) {
    core::Mesh* mesh = getMesh();
    if (!mesh) {
        return false;
    }

    glm::vec3 chunkPos = getPosition();
    
    // Calculate grid spacing
    float widthSpacing = CHUNK_SIZE / static_cast<float>(m_vertexCountX - 1);
    float lengthSpacing = CHUNK_SIZE / static_cast<float>(m_vertexCountZ - 1);
    const float halfWidth = CHUNK_SIZE * 0.5f;
    const float halfLength = CHUNK_SIZE * 0.5f;

    // Get the point array
    PointArray points = mesh->getPoints();

    // Track if any points were modified
    bool modified = false;

    // Define falloff margin (smooth transition at edges)
    const float falloffMargin = 2.0f; // Units in world space

    // Iterate through all vertices in the chunk
    for (int ix = 0; ix < m_vertexCountX; ++ix) {
        for (int iz = 0; iz < m_vertexCountZ; ++iz) {
            // Calculate vertex index
            int vertexIndex = (ix * m_vertexCountZ) + iz;

            if (vertexIndex < 0 || vertexIndex >= points.size()) {
                continue;
            }

            // Get vertex local coordinates
            float localX = ix * widthSpacing - halfWidth;
            float localZ = iz * lengthSpacing - halfLength;

            // Convert to world coordinates
            float worldX = chunkPos.x + localX;
            float worldZ = chunkPos.z + localZ;

            // Calculate distance from center point
            float dx = worldX - centerWorldX;
            float dz = worldZ - centerWorldZ;
            float distance = std::sqrt(dx * dx + dz * dz);

            // Skip points outside the influence radius
            if (distance > radius + falloffMargin) {
                continue;
            }

            // Calculate weight based on distance
            float weight = 0.0f;
            if (distance < radius) {
                // Inside the core radius - full effect
                weight = 1.0f;
            } else {
                // In the falloff margin - smooth transition
                float falloffDist = distance - radius;
                weight = 1.0f - (falloffDist / falloffMargin);
                weight = std::max(0.0f, std::min(1.0f, weight));
                // Apply smoothstep for smoother falloff
                weight = weight * weight * (3.0f - 2.0f * weight);
            }

            // Get the existing point
            Point& oldPoint = points[vertexIndex];
            float currentY = static_cast<float>(oldPoint.Y());

            // Lerp between current height and new height based on weight
            float targetY = std::lerp(currentY, newY, weight);

            // Create updated point
            Point updatedPoint(static_cast<float>(oldPoint.X()), targetY, static_cast<float>(oldPoint.Z()));
            
            // Update the point in the array
            points.setPoint(vertexIndex, updatedPoint);
            modified = true;
        }
    }

    if (!modified) {
        return false; // No points in this chunk were affected
    }

    // Update the mesh with modified points
    mesh->setPoints(points, false);

    // Recalculate normals to reflect the height changes
    createNormals(mesh);

    // Mark chunk as dirty for re-rendering
    setDirty(true);

    return true;
}

void Chunk::onCollision(){
    //do nothing - we don't currently care about chunk collisions.
    //HgLogger::logMsg("Chunk collision triggered!");
}

Chunk::~Chunk(){
    HgLogger::logDebug("Destroying chunk with id: %u", getId());
}
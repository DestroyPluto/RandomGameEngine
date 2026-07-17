#include "TerrainGenerator.h"
#include "Chunk.h"
#include <cmath>
#include <glm/glm.hpp>
#include "SceneManager.h"
#include "HgLogger.h"

using namespace client;
using namespace core;
using namespace math;

void TerrainGenerator::initialize(){
    
    m_parent->setMesh(nullptr);
    createChunks();

}

uint32_t TerrainGenerator::generateChunkId(float x, float y) {

    //first cast to ints
    uint16_t x_int = static_cast<uint16_t>(x);
    uint16_t y_int = static_cast<uint16_t>(y);

    const uint32_t chunkIdOffset = 10000; // Arbitrary offset to avoid low IDs reserved for other entities
    uint32_t id = chunkIdOffset + (x_int << 16 | y_int);

    return id;
}

void TerrainGenerator::createChunks(){
    uint64_t seed = 12345;
    
    int numChunks = static_cast<int>(std::ceil((m_renderRadius * 2) / (Chunk::CHUNK_SIZE - 1)));

    Entity* playerEntity = SceneManager::getInstance()->getPlayerEntity();
    //if the player doesn't exist yet, then just use the origin.
    glm::vec2 playerPos(0.0f);

    if (playerEntity) {
        playerPos.x = playerEntity->getPosition().x;
        playerPos.y = playerEntity->getPosition().z;
    }

    float centerOffsetX = std::round(playerPos.x / Chunk::CHUNK_SIZE) * Chunk::CHUNK_SIZE;
    float centerOffsetZ = std::round(playerPos.y / Chunk::CHUNK_SIZE) * Chunk::CHUNK_SIZE;

    m_centerChunkCoords = glm::vec2(centerOffsetX, centerOffsetZ);

    for (int i = 0; i < numChunks; ++i) {
        for (int j = 0; j < numChunks; ++j) {
            float x = ((i - numChunks / 2) * Chunk::CHUNK_SIZE) + centerOffsetX;
            float z = ((j - numChunks / 2) * Chunk::CHUNK_SIZE) + centerOffsetZ;
            
            glm::vec3 chunkPos(x, 0.0f, z);
            glm::vec2 chunkPos2D(x, z);
            if (getDistanceBetweenTwoPoints2D(chunkPos2D, m_centerChunkCoords) > m_renderRadius)
                continue;
            //chunk id is a combination of the chunk's grid coordinates, 
            // this allows us to easily identify and manage chunks based on their position in the world.
            bool chunkExists = false;
            uint32_t chunkId = generateChunkId(x, z);
            for (Chunk* chunk : m_loadedChunks) {
                if ((const uint32_t)chunk->getId() == chunkId) {
                    //chunk already exists, just make sure it's rendered and move on.
                    chunk->setRenderMesh(true);
                    chunkExists = true;
                    break;
                }
            }

            if (!chunkExists){
                //queue the chunk to be created
                m_pendingChunks.emplace(chunkId, chunkPos, seed);
            }
        }
    }
}

void TerrainGenerator::loadPendingChunks() {

    int loadedThisFrame = 0;

    while (m_pendingChunks.size() > 0 && loadedThisFrame < m_chunksPerFrame) {
        auto [chunkId, chunkPos, seed] = m_pendingChunks.front();
        m_pendingChunks.pop();
        Chunk* newChunk = new Chunk(chunkId, chunkPos, seed, this);
        m_parent->addChild(newChunk);
        m_loadedChunks.push_back(newChunk);
        loadedThisFrame++;
    }
}

void TerrainGenerator::update(){

    loadPendingChunks();
    
    Entity* playerEntity = SceneManager::getInstance()->getPlayerEntity();
    //if the player doesn't exist yet, then just use the origin.
    glm::vec2 playerPos(0.0f);

    if (playerEntity) {
        playerPos.x = playerEntity->getPosition().x;
        playerPos.y = playerEntity->getPosition().z;
    }

    //if we move away from the center chunk by more than the chunk size, we need to create new chunks around the player.
    if (getDistanceBetweenTwoPoints2D(m_centerChunkCoords, playerPos) > (Chunk::CHUNK_SIZE/2) + 1) {
        createChunks();

        for (Chunk* chunk : m_loadedChunks) {

            glm::vec2 chunkPos(chunk->getPosition().x, chunk->getPosition().z);

            if (getDistanceBetweenTwoPoints2D(chunkPos, m_centerChunkCoords) > m_renderRadius) {
                //turn chunk off, but don't delete it, we may need it again if the player moves back.
                chunk->setRenderMesh(false);
            }

            if (getDistanceBetweenTwoPoints2D(chunkPos, m_centerChunkCoords) > m_loadRadius) {
                //if the chunk is outside the load radius, we can safely delete it.
                m_loadedChunks.erase(std::remove(m_loadedChunks.begin(), m_loadedChunks.end(), chunk), m_loadedChunks.end());
                chunk->markForDestruction();
            }
        }
    }
    //handle debug entities
    for (DebugEntity* debugEntity : m_debugEntities) {
        if (getDistanceBetweenTwoPoints2D(glm::vec2(debugEntity->getPosition().x, debugEntity->getPosition().z), playerPos) > m_renderRadius) {
            debugEntity->setRenderMesh(false);
        }
        else {
            debugEntity->setRenderMesh(true);
        }
    }

}

float TerrainGenerator::getDistanceBetweenTwoPoints2D(glm::vec2 pos1, glm::vec2 pos2){
    float dx = pos1.x - pos2.x;
    float dz = pos1.y - pos2.y;
    return std::sqrt(dx * dx + dz * dz);
}

Chunk* TerrainGenerator::getChunkAtWorldPosition(float worldX, float worldZ) {
    // Calculate which chunk contains this world position
    // Chunks are centered at multiples of CHUNK_SIZE (e.g., 0, 16, 32...)
    // A chunk centered at position X covers from (X - CHUNK_SIZE/2) to (X + CHUNK_SIZE/2)
    // To find which chunk contains a point, we need to find which multiple of CHUNK_SIZE is nearest
    
    // Add half chunk size, then floor-divide to get the chunk grid coordinate
    float halfSize = Chunk::CHUNK_SIZE * 0.5f;
    int chunkGridX = static_cast<int>(std::floor((worldX + halfSize) / Chunk::CHUNK_SIZE));
    int chunkGridZ = static_cast<int>(std::floor((worldZ + halfSize) / Chunk::CHUNK_SIZE));
    
    // Convert grid coordinates back to world position (chunk center)
    float chunkX = chunkGridX * Chunk::CHUNK_SIZE;
    float chunkZ = chunkGridZ * Chunk::CHUNK_SIZE;
    
    uint32_t chunkId = generateChunkId(chunkX, chunkZ);
    
    // Find the chunk in loaded chunks
    for (Chunk* chunk : m_loadedChunks) {
        if (chunk->getId() == chunkId) {
            return chunk;
        }
    }
    
    return nullptr;
}

void TerrainGenerator::setRegionHeight(float centerWorldX, float centerWorldZ, float radius, float newY) {
    // Add falloff margin to the influence distance
    const float falloffMargin = 2.0f;
    float maxInfluenceDistance = radius + falloffMargin;
    
    // Check all loaded chunks to see if they might contain vertices within the region
    for (Chunk* chunk : m_loadedChunks) {
        glm::vec3 chunkPos = chunk->getPosition();
        
        // Calculate the distance from the region center to the chunk center
        float dx = chunkPos.x - centerWorldX;
        float dz = chunkPos.z - centerWorldZ;
        float distanceToChunk = std::sqrt(dx * dx + dz * dz);
        
        // A chunk might contain affected vertices if the region overlaps with the chunk's bounds
        // Chunk extends CHUNK_SIZE/2 in each direction from its center
        float chunkRadius = Chunk::CHUNK_SIZE * 0.5f * 1.415f; // Multiply by sqrt(2) for diagonal
        
        if (distanceToChunk <= maxInfluenceDistance + chunkRadius) {
            // This chunk might have vertices within the region
            chunk->setRegionHeight(centerWorldX, centerWorldZ, radius, newY);
        }
    }
}

void TerrainGenerator::addRiverNode(RiverNode* node) {
    m_riverNodes.push_back(node);
    DebugEntity* debugEntity = new DebugEntity(m_nextDebugEntityId++, DebugEntityType::debug_Box, glm::vec3(node->getX(), node->getY() + 0.05f, node->getZ()), glm::vec3(0.0f, 0.0f, 1.0f));
    debugEntity->setScale(glm::vec3(0.1f));
    m_parent->addChild(debugEntity);
    m_debugEntities.push_back(debugEntity);
}

/**
* TODO: rivers
* what I'm currently thinking:
*   - have a "mouth" node, and a "tail" node
*   - start generating at the tail (should be a high altitude point), 
*     and generate a path to the mouth (should be a low altitude point)
*   - probably just greedy search
*   - river consists of multiple nodes
*   - should probably deform the terrain... but start with just getting the paths right first.
*/
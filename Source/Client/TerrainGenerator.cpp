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

    uint32_t id = 100 + (x_int << 16 | y_int);

    return id;
}

void TerrainGenerator::createChunks(){
    HgLogger::logDebug("Creating chunks around player...");
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
                HgLogger::logDebug("Chunk does not exist.");
                //queue the chunk to be created
                m_pendingChunks.emplace(chunkId, chunkPos, seed);
            }

           
        }
    }
}
void TerrainGenerator::loadPendingChunks() {

    int loadedThisFrame = 0;

    while (m_pendingChunks.size() > 0 && loadedThisFrame < m_chunksPerFrame) {
        HgLogger::logDebug("Dynamically loading chunks");

        auto [chunkId, chunkPos, seed] = m_pendingChunks.front();
        m_pendingChunks.pop();
        Chunk* newChunk = new Chunk(chunkId, chunkPos, seed);
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
   

}

float TerrainGenerator::getDistanceBetweenTwoPoints2D(glm::vec2 pos1, glm::vec2 pos2){

    float dx = pos1.x - pos2.x;
    float dz = pos1.y - pos2.y;
    return std::sqrt(dx * dx + dz * dz);
}

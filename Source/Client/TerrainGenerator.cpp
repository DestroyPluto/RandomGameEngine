#include "TerrainGenerator.h"
#include "Chunk.h"
#include <cmath> // Add this include at the top of the file
#include <glm/glm.hpp>
#include "SceneManager.h"

using namespace client;
using namespace core;
using namespace math;

void TerrainGenerator::initialize(){
    
    m_parent->setMesh(nullptr);
    createChunks();

}

void TerrainGenerator::createChunks(){
    uint64_t seed = 12345;

    float radius = 32.0f; // Example radius for chunk generation
    
    int numChunks = static_cast<int>(std::ceil((radius * 2) / (Chunk::CHUNK_SIZE - 1)));

    for (int i = 0; i < numChunks; ++i) {
        for(int j = 0; j < numChunks; ++j){
            float x = (i - numChunks / 2) * Chunk::CHUNK_SIZE;
            float z = (j - numChunks / 2) * Chunk::CHUNK_SIZE;

            
            glm::vec3 chunkPos(x, 0.0f, z);
            if (getDistanceFromPlayer(chunkPos) > radius)
                continue;

            uint32_t chunkId = static_cast<uint32_t>(i * numChunks + j);
            Chunk* chunk = new Chunk(chunkId, chunkPos, seed);
            m_parent->addChild(chunk);
        }
    }

}


void TerrainGenerator::update(){
    //Terrain generation logic would go here
}

float TerrainGenerator::getDistanceFromPlayer(glm::vec3 chunkPos) {
    Entity* playerEntity = SceneManager::getInstance()->getPlayerEntity();
    //if the player doesn't exist yet, then just use the origin.
    glm::vec3 playerPos(0.0f);
    
    if (playerEntity) {
        playerPos = playerEntity->getPosition();
    }

    float dx = chunkPos.x - playerPos.x;
    float dz = chunkPos.z - playerPos.z;
    return std::sqrt(dx * dx + dz * dz);
}

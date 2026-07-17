#pragma once
#include "Behaviour.h"
#include "glm/glm.hpp"
#include <queue>
#include <tuple>
#include "RiverNode.h"
#include "DebugEntity.h"

namespace client {
    class Chunk;

    class TerrainGenerator : public core::Behaviour{
    public:
        virtual void update() override;
        virtual void initialize() override;
        virtual std::string const getName() override {
            return "TerrainGenerator";
        }

       virtual ~TerrainGenerator() {
            
       }

     void setRegionHeight(float centerWorldX, float centerWorldZ, float radius, float newY);
     void addRiverNode(RiverNode* node);

    private:
        void createChunks();
        float getDistanceBetweenTwoPoints2D(glm::vec2 pos1, glm::vec2 pos2);
        void loadPendingChunks();
        uint32_t generateChunkId(float x, float z);
        Chunk* getChunkAtWorldPosition(float worldX, float worldZ);

        std::vector<Chunk*> m_loadedChunks;
        glm::vec2 m_centerChunkCoords;
        float m_renderRadius = 48.0f;
        float m_loadRadius = 128.0f;
        
        std::queue<std::tuple<uint32_t, glm::vec3, uint64_t>> m_pendingChunks;
        int m_chunksPerFrame = 2; // Tune this for your performance
        std::vector<RiverNode*> m_riverNodes;
        std::vector<client::DebugEntity*> m_debugEntities;
        uint32_t m_nextDebugEntityId = 123456;
    };
}
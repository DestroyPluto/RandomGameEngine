#pragma once
#include "Behaviour.h"
#include "RiverGenerator.h"
#include "Chunk.h"
#include "glm/glm.hpp"
#include <queue>
#include <tuple>

namespace client {
    class RiverGenerator; // Forward declaration

    class TerrainGenerator : public core::Behaviour{
    public:
        virtual void update() override;
        virtual void initialize() override;
        virtual std::string const getName() override {
            return "TerrainGenerator";
        }

       virtual ~TerrainGenerator() {
            delete m_riverGenerator;
       }

     void setRegionHeight(float centerWorldX, float centerWorldZ, float radius, float newY);


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
        RiverGenerator* m_riverGenerator;

        friend class RiverGenerator;
    };
}
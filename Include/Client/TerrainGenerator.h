#pragma once
#include "Behaviour.h"
#include "Chunk.h"
#include "glm/glm.hpp"
#include <queue>
#include <tuple>

namespace client {
    class TerrainGenerator : public core::Behaviour{
    public:
        virtual void update() override;
        virtual void initialize() override;
        virtual std::string const getName() override {
            return "TerrainGenerator";
        }

       virtual ~TerrainGenerator() {
          //  delete m_noiseGenerator0;
           // delete m_noiseGenerator1;
       }

    private:
        void createChunks();
        float getDistanceBetweenTwoPoints2D(glm::vec2 pos1, glm::vec2 pos2);
        void loadPendingChunks();
        uint32_t generateChunkId(float x, float z);

        std::vector<Chunk*> m_loadedChunks;
        glm::vec2 m_centerChunkCoords;
        float m_renderRadius = 64.0f;
        float m_loadRadius = 128.0f;
        
        std::queue<std::tuple<uint32_t, glm::vec3, uint64_t>> m_pendingChunks;
        int m_chunksPerFrame = 2; // Tune this for your performance
    };
}
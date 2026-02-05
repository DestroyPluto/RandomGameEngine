#pragma once
#include "Behaviour.h"


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
        float getDistanceFromPlayer(glm::vec3 chunkPos);
        
    };
}
#pragma once
#include "Core/Entity.h"
#include "Core/Mesh.h"
#include "Math/Noise.h"


namespace client {

    class Chunk : public core::Entity{
    public:
            Chunk(uint32_t id, glm::vec3 pos, uint64_t seed);
            virtual ~Chunk();
            virtual void onCollision() override;
            static constexpr int CHUNK_SIZE = 16;

            bool setPointHeight(float worldX, float worldZ, float newY);
            bool setRegionHeight(float centerWorldX, float centerWorldZ, float radius, float newY);
            float getPointHeight(float worldX, float worldZ);
    protected:

    private:
        typedef enum BiomeType {
            Lake,
            Plains,
            Hills,
            Mountains
        } eBiomeType;

        typedef struct {
            BiomeType type;
            float transition;
        } sBiome;

        void createMesh();

        void createPoints(core::Mesh* mesh);
        void createIndices(core::Mesh* mesh);
        void createNormals(core::Mesh* mesh);

        float calculateHeight(float x, float z);

        float calculatePlains(float x, float z);
        float calculateHills(float x, float z);
        float calculateMountains(float x, float z);
        float calculateLakes(float x, float z);

        sBiome getBiomeType(float x, float z);

        int m_vertexCountX;
        int m_vertexCountZ;

        math::Noise m_BaseTerrainNoiseGenerator;
        math::Noise m_SecondaryTerrainNoiseGenerator;
        math::Noise m_BiomeNoiseGenerator;

        uint64_t m_seed;

        
        float* m_heightMap;
        
    };
}
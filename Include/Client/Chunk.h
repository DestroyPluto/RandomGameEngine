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

    protected:

    private:

        void createMesh();

        void createPoints(core::Mesh* mesh);
        void createIndices(core::Mesh* mesh);
        void createNormals(core::Mesh* mesh);

        float calculateHeight(float x, float z);
        float calculateHeight(float x, float z, const glm::vec3& chunkPos);


        int m_vertexCountX = 16;
        int m_vertexCountZ = 16;

        math::Noise m_noiseGenerator0;
        math::Noise m_noiseGenerator1;
        uint64_t m_seed;

    };
}
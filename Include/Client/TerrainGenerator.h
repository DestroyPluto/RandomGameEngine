#pragma once
#include "Behaviour.h"
#include "core/Mesh.h"

namespace client {
    class TerrainGenerator : public core::Behaviour{
    public:
        virtual void update() override;
        virtual void initialize() override;
        virtual std::string const getName() override {
            return "TerrainGenerator";
        }

       

    private:
        void createMesh();
        
        void createPoints(core::Mesh* mesh);
        void createIndices(core::Mesh* mesh);
        void createNormals(core::Mesh* mesh);

        float calculateHeight(float x, float z);

        float m_width;
        float m_length;
        float m_maxHeight;
        int m_vertexCountX;
        int m_vertexCountZ;
        
    };
}
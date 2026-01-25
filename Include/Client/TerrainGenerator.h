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

        void createMesh();

    private:

        float m_width;
        float m_length;
        float m_maxHeight;
        int m_vertexCountX;
        int m_vertexCountZ;
        
    };
}
#pragma once
#include <glm/vec3.hpp>
#include "Mesh.h"

namespace core{

class Entity{

    public:
        void setMesh(Mesh mesh){m_mesh = mesh;}
        Mesh* getMesh(){return &m_mesh;}
    private:
        glm::vec3 m_position;
        glm::vec3 m_rotation;
        glm::vec3 m_scale;

        Mesh m_mesh;
};
}
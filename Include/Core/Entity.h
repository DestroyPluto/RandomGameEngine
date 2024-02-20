#pragma once
#include <glm/vec3.hpp>
#include "Mesh.h"

namespace core{

class Entity{

    public:
        Entity(uint32_t id);
        void setMesh(Mesh mesh){m_mesh = mesh;}
        Mesh* getMesh(){return &m_mesh;}
        bool isDirty(){return m_isDirty;}
        void setDirty(bool isDirty){m_isDirty = isDirty;}
        uint32_t getId(){return m_id;}
    private:
        glm::vec3 m_position;
        glm::vec3 m_rotation;
        glm::vec3 m_scale;

        Mesh m_mesh;
        uint32_t m_id;
        bool m_isDirty;
};
}
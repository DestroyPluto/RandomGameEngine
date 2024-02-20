#pragma once
#include <glm/glm.hpp>
#include <glm/vec3.hpp>

namespace core{
class Entity{
    public:
        Entity();
    private:
    //TODO: these should be vectors
        glm::vec3 m_position;
        glm::vec3 m_rotation;
        glm::vec3 m_scale;
};
}
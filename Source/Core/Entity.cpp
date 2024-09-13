#include  "Entity.h"

using namespace core;

Entity::Entity(uint32_t id){
    m_id = id;
    m_position = glm::vec3(0.0f);
    m_rotation = glm::vec3(1.0f);
    m_scale = glm::vec3(1.0f);
}
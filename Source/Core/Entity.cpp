#include  "Entity.h"
#include "Logger.h"

using namespace core;

Entity::Entity(uint32_t id){
    m_id = id;
    m_position = glm::vec3(0.0f);
    m_rotation = glm::vec3(1.0f);
    m_scale = glm::vec3(1.0f);
}

//TODO
bool Entity::intersects(Entity* other){

    return false;
}
//TODO: 3D collision
bool Entity::intersects(glm::vec3 pos){
    float left = m_position.x - (m_scale.x/2.0f);
    float right = m_position.x + (m_scale.x/2.0f);
    float top = m_position.y + (m_scale.y/2.0f);
    float bottom = m_position.y - (m_scale.y/2.0f);

    bool xIntersect = pos.x >= left && pos.x <= right;
    bool yIntersect = pos.y >= bottom && pos.y <= top;

    return xIntersect && yIntersect;
}
bool Entity::intersects(float x, float y, float z){
    glm::vec3 pos = glm::vec3(x, y, z);
    return intersects(pos);
}


void Entity::onCollision(Entity* other){
    HgLogger::logMsg("entity collision triggered!");
}

void Entity::onCollision(){
    HgLogger::logMsg("entity collision triggered!");
}
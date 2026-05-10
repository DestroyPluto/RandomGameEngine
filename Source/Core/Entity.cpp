#include  "Entity.h"
#include "HgLogger.h"
#include "Config.h"

using namespace core;

Entity::Entity(uint32_t id){
    m_id = id;
    m_position = glm::vec3(0.0f);
    m_rotation = glm::vec3(1.0f);
    m_scale = glm::vec3(1.0f);
    m_mesh = new Mesh();
}

Entity::~Entity(){
    delete m_mesh;
    for (Entity* ent : m_children) {
        delete ent;
    }

    HgLogger::logDebug("Entity destroyed");
}

//TODO
bool Entity::intersects(Entity* other){
    return false;
}
//TODO: 3D collision
//TODO handle collision with children as well
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

void Entity::onUpdate(){

}

void Entity::onClick(){

}

void Entity::addChild(Entity* child) {
    m_children.push_back(child);
    child->setParent(this);
    child->setDirty(true);
    //child transforms should be relative to the parent.
    child->setPosition(child->getPosition() + m_position);
    child->setRotation(child->getRotation() + m_rotation);
    child->setScale(child->getScale() * m_scale); 
}

void Entity::removeChild(Entity* child) {
    m_children.erase(std::remove(m_children.begin(), m_children.end(), child), m_children.end());
    child->setParent(nullptr);
    child->setDirty(true);
}

void Entity::markForDestruction(bool shouldDestroy) {
    m_shouldDestroy = shouldDestroy;
    setDirty(true);
    for (Entity* child : m_children)
    {
        child->markForDestruction(shouldDestroy);
    }
}

#include "GameObject.h"
#include "Mouse.h"
#include "Entity.h"

using namespace core;
using namespace io;

GameObject::GameObject(uint32_t id, Behaviour* behaviour) : Entity(id){
   m_behaviour = behaviour;
   if (m_behaviour) {
       m_behaviour->setParent(this);
       m_behaviour->initialize();
   }
}

GameObject::GameObject(uint32_t id, Behaviour* behaviour, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) : Entity(id) {
    setPosition(position);
    setRotation(rotation);
    setScale(scale);

    m_behaviour = behaviour;
    if (m_behaviour) {
        m_behaviour->setParent(this);
        m_behaviour->initialize();
    }
}

void GameObject::onUpdate(){
    if (m_behaviour) {
        m_behaviour->update();
    }
}

void GameObject::onClick(){
    if (m_behaviour) {
        m_behaviour->onClick();
    }
}

void GameObject::onCollision(){
    if(Mouse::getInstance()->isMouseButtonDown(Mouse::MOUSE_LEFT)){
        if(!m_clickProgress){
            m_clickProgress = true;
            onClick();
        }
    }else{
        m_clickProgress = false;
    }
}


GameObject::~GameObject(){
    if (m_behaviour) {
        delete m_behaviour;
    }
}
#include "GameObject.h"

using namespace core;

GameObject::GameObject(uint32_t id, Behaviour* behaviour) : Entity(id){
   m_behaviour = behaviour;
   m_behaviour->setParent(this);
   m_behaviour->initialize();
}

void GameObject::onUpdate(){
    m_behaviour->update();
}

GameObject::~GameObject(){
    delete m_behaviour;
}
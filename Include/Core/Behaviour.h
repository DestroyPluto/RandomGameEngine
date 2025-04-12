#pragma once
#include "Entity.h"

namespace core{
class Behaviour{
    
public:
    void setParent(Entity* parent){m_parent = parent;}
    
    virtual void update(){}
    virtual void initialize(){}
    virtual void onClick(){}
    
    virtual const std::string getName(){return "";}
    core::Entity* m_parent;
};
}
#pragma once
#include "Entity.h"

namespace core{
class Behaviour{
    
public:
    void setParent(Entity* parent){m_parent = parent;}
    
    virtual void update(){}
    virtual void initialize(){}
        
    core::Entity* m_parent;
};
}
#include "Button.h"
#include "PointArray.h"
#include "Logger.h"
#include "Mouse.h"

using namespace math;
using namespace core;
using namespace io;

Button::Button(uint32_t id) : Entity(id){
    //m_mesh = Mesh();
    setLayer(eUI);
    m_clickProgress = false;
}

void Button::onCollision(Entity* other){
    HgLogger::logMsg("button Collision Occured!");
}

void Button::onCollision(){
    if(Mouse::getInstance()->isMouseButtonDown(Mouse::MOUSE_LEFT)){
        if(!m_clickProgress){
            m_clickProgress = true;
            onClick();
        }
    }else{
        m_clickProgress = false;
    }
}

void Button::onClick(){
    HgLogger::logDebug("Button Clicked!");
    return;
}

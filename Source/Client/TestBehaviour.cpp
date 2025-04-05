#include "TestBehaviour.h"
#include "HgLogger.h"

using namespace client;
using namespace core;

void TestBehaviour::initialize(){
    int id = m_parent->getId();
    HgLogger::logDebug("Test behaviour initialize called!, for object with id: %u", id);

    m_up = true;
    m_speed = 0.000001f;

}

void TestBehaviour::update(){
    int id = m_parent->getId();
    //HgLogger::logDebug("Test behaviour update called!, for object with id: %u", id);

    glm::vec3 pos = m_parent->getPosition();

    if(pos.y >= 1.0f){
        m_up = false;
        HgLogger::logDebug("Reached the top!");

    }else if(pos.y <= -1.0f){
        m_up = true;
        HgLogger::logDebug("Reached the Bottom!");
    }

    if(m_up)
        pos.y += m_speed;
    else
        pos.y -= m_speed;
    
m_parent->setPosition(pos);
}
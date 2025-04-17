#include "Player.h"
#include "Keyboard.h"
#include "GameTime.h"

using namespace client;
using namespace io;
using namespace core;

void Player::initialize(){
    
}

void Player::update(){
    handleMovement();
}

void Player::handleMovement(){

    glm::vec3 pos = m_parent->getPosition();

    //handle movement
    if(Keyboard::getInstance()->isKeyDown(Keyboard::KEY_W)){
        pos.y += m_speed * GameTime::getDeltaTime();
    }
    
    if(Keyboard::getInstance()->isKeyDown(Keyboard::KEY_S)){
        pos.y -= m_speed * GameTime::getDeltaTime();
    }

    if(Keyboard::getInstance()->isKeyDown(Keyboard::KEY_D)){
        pos.x += m_speed * GameTime::getDeltaTime();
    }
    
    if(Keyboard::getInstance()->isKeyDown(Keyboard::KEY_A)){
        pos.x -= m_speed * GameTime::getDeltaTime();
    }

    m_parent->setPosition(pos);
}
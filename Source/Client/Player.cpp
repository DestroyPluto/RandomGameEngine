#include "Player.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "GameTime.h"
#include "SceneManager.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace client;
using namespace io;
using namespace core;

void Player::initialize(){
    //make sure the scene knows this is the player.
    SceneManager::getInstance()->setPlayerEntity(m_parent);
}

void Player::update(){
    double mouseX = 0.0;
    double mouseY = 0.0;
    Mouse::getInstance()->getScreenPos(mouseX, mouseY);
    if(!m_hasMouse){
        m_lastMouseX = mouseX;
        m_lastMouseY = mouseY;
        m_hasMouse = true;
    }

    float deltaX = static_cast<float>(mouseX - m_lastMouseX);
    float deltaY = static_cast<float>(mouseY - m_lastMouseY);
    m_lastMouseX = mouseX;
    m_lastMouseY = mouseY;

    m_yaw += deltaX * m_mouseSensitivity;
    m_pitch -= deltaY * m_mouseSensitivity;
    m_pitch = glm::clamp(m_pitch, glm::radians(-89.0f), glm::radians(89.0f));
    SceneManager::getInstance()->setCameraRotation(m_yaw, m_pitch);

    if (Keyboard::getInstance()->isKeyDown(Keyboard::KEY_LEFT_CONTROL)) {
        m_speed = 10.0f;
    } else {
        m_speed = 2.0f;
    }

    handleMovement();
}

void Player::handleMovement(){

    glm::vec3 pos = m_parent->getPosition();
    float moveAmount = m_speed * static_cast<float>(GameTime::getDeltaTime());
    glm::vec3 forward = glm::normalize(glm::vec3(sinf(m_yaw), 0.0f, -cosf(m_yaw)));
    glm::vec3 right = glm::normalize(glm::vec3(cosf(m_yaw), 0.0f, sinf(m_yaw)));

    //handle movement
    if(Keyboard::getInstance()->isKeyDown(Keyboard::KEY_W)){
        pos += forward * moveAmount;
    }
    
    if(Keyboard::getInstance()->isKeyDown(Keyboard::KEY_S)){
        pos -= forward * moveAmount;
    }

    if(Keyboard::getInstance()->isKeyDown(Keyboard::KEY_D)){
        pos += right * moveAmount;
    }
    
    if(Keyboard::getInstance()->isKeyDown(Keyboard::KEY_A)){
        pos -= right * moveAmount;
    }

    if(Keyboard::getInstance()->isKeyDown(Keyboard::KEY_SPACE)){
        pos.y += moveAmount;
    }

    if(Keyboard::getInstance()->isKeyDown(Keyboard::KEY_LEFT_SHIFT)){
        pos.y -= moveAmount;
    }

    m_parent->setPosition(pos);
    glm::vec3 camPos = SceneManager::getInstance()->getCameraPosition();
    camPos.x = pos.x;
    camPos.y = pos.y;
    camPos.z = pos.z + 2.0f;
    SceneManager::getInstance()->setCameraPosition(camPos);
}
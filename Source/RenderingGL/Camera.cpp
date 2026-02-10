#include "Camera.h"
using namespace rendering;

    glm::mat4 Camera::getPerspective(){
        float aspect = m_screenWidth/m_screenHeight;
        return glm::perspective(glm::radians(45.0f), aspect, 0.01f, 500.0f);
    }

    glm::mat4 Camera::getOrtho(){
        return glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, -100.0f, 100.0f);
    }

    glm::mat4 Camera::getView(){
        glm::mat4 view = glm::mat4(1.0f);
        view = glm::rotate(view, m_pitch, glm::vec3(1.0f, 0.0f, 0.0f));
        view = glm::rotate(view, m_yaw, glm::vec3(0.0f, 1.0f, 0.0f));
        view = glm::translate(view, m_position);
        return view;
    }

    void Camera::setPosition(glm::vec3 translation){
        m_position = translation * -1.0f;
        //m_position.z = -3.0f;
    }

    void Camera::setRotation(float yaw, float pitch){
        m_yaw = yaw;
        m_pitch = pitch;
    }

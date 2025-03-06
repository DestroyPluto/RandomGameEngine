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
        view = glm::translate(view, glm::vec3(0.0f,0.0f,-3.0f));
        return view;
    }
#pragma once
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>

namespace rendering {
    class Camera{
    public:
        float getScreenWidth(){return m_screenWidth;};
        float getScreenHeight(){return m_screenHeight;};

        void setScreenWidth(float width){m_screenWidth = width;};
        void setScreenHeight(float height){m_screenHeight = height;};

        glm::mat4 getPerspective();
        glm::mat4 getOrtho();

        glm::mat4 getView();

        void setPosition(glm::vec3 translation);
        const glm::vec3 getPosition(){return m_position;}
        void setRotation(float yaw, float pitch);
        const glm::vec2 getRotation(){return glm::vec2(m_yaw, m_pitch);}

    private:
        float m_screenWidth;
        float m_screenHeight;
        
        glm::vec3 m_position = glm::vec3(0.0f);
        float m_yaw = 0.0f;
        float m_pitch = 0.0f;

    };
}
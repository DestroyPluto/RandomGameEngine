#pragma once
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>

namespace rendering {
    class Camera{
    public:
        float getScreenWidth(){return m_screenWidth;};
        float getScreenHeight(){return m_screenWidth;};

        void setScreenWidth(float width){m_screenWidth = width;};
        void setScreenHeight(float height){m_screenHeight = height;};

        glm::mat4 getPerspective();
        glm::mat4 getOrtho();

        glm::mat4 getView();

        void setPosition(glm::vec3 translation);
        glm::vec3 getPosition(){return m_position;}
        
    private:
        float m_screenWidth;
        float m_screenHeight;
        
        glm::vec3 m_position = glm::vec3(1.0,1.0,-3.0);

    };
}
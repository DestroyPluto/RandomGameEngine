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

    private:
        float m_screenWidth;
        float m_screenHeight;
        
    };
}
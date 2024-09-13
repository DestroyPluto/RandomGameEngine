#pragma once
#include "Shader.h"
#include <glm/mat4x4.hpp>

namespace rendering{
    class BasicShader: public Shader{
        public:
        BasicShader();
        void setColour(float r, float g, float b);
        void setModelMatrix(glm::mat4 model);
        void setViewMatrix(glm::mat4 view);
        void setProjectionMatrix(glm::mat4 projection);
    };
}
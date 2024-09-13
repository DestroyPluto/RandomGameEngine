#pragma once
#include "Shader.h"

namespace rendering{
    class BasicShader: public Shader{
        public:
        BasicShader();
        void setColour(float r, float g, float b);

    };
}
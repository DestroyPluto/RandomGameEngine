#pragma once
#include "Shader.h"

namespace rendering{
    class TextShader: public Shader{
        public:
        TextShader();
        void setColour(float r, float g, float b);
    };
}
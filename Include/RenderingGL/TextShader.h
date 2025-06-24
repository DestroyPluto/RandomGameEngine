#pragma once
#include "Shader.h"
#include <memory>
#include <Config.h>

namespace rendering{
    class TextShader: public Shader{
        public:
        TextShader(std::shared_ptr<core::Config> config);
        void setColour(float r, float g, float b);
    };
}
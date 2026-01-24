#pragma once
#include "Shader.h"
#include <memory>
#include <Config.h>

namespace rendering{
    class ColourShader: public Shader{
    public:
        ColourShader(std::shared_ptr<core::Config> config);
        void setColour(float r, float g, float b);
    };
}
#include "ColourShader.h"
#include <glm/vec3.hpp>

using namespace rendering;

ColourShader::ColourShader(std::shared_ptr<core::Config> config)
: Shader("vertex.vs", "colour.fs", config)
{
    Shader::bind();
    // default colour white
    setColour(1.0f, 1.0f, 1.0f);
    Shader::unBind();
}

void ColourShader::setColour(float r, float g, float b){
    glm::vec3 rgb = glm::vec3(r, g, b);
    setUniform<glm::vec3>("inColour", rgb);
}
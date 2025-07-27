#include "TextShader.h"

using namespace rendering;

TextShader::TextShader(std::shared_ptr<core::Config> config) 
: Shader("TextShader.vs", "TextShader.fs", config){
    Shader::bind();
    //set default colour
    setColour(1.0f, 1.0f, 1.0f);
    Shader::unBind();
}

void TextShader::setColour(float r, float g, float b){
    glm::vec3 rgb = glm::vec3(r, g, b);

    setUniform<glm::vec3>("textColour", rgb);
}

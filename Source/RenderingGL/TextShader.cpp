#include "TextShader.h"

using namespace rendering;

TextShader::TextShader() 
: Shader("../Source/RenderingGL/Shaders/TextShader.vs", "../Source/RenderingGL/Shaders/TextShader.fs"){
    Shader::bind();
    //set default colour
    setColour(1.0f, 1.0f, 1.0f);
    Shader::unBind();
}

void TextShader::setColour(float r, float g, float b){
    glm::vec3 rgb = glm::vec3(r, g, b);

    setUniform<glm::vec3>("textColour", rgb);
}

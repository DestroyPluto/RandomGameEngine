#include "BasicShader.h"
#include <glm/vec3.hpp>
using namespace rendering;

BasicShader::BasicShader()
: Shader("vertex.vs", "fragment.fs")
{
    Shader::bind();
    //set default colour
    setColour(1.0f, 1.0f, 1.0f);
    Shader::unBind();
}

void BasicShader::setColour(float r, float g, float b){
    glm::vec3 rgb = glm::vec3(r, g, b);

    setUniform<glm::vec3>("inColour", rgb);
}



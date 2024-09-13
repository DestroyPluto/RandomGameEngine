#include "BasicShader.h"
#include <glm/vec3.hpp>
using namespace rendering;

BasicShader::BasicShader()
: Shader("../Source/RenderingGL/Shaders/vertex.vs", "../Source/RenderingGL/Shaders/fragment.fs")
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

void BasicShader::setProjectionMatrix(glm::mat4 proj){
    setUniform<glm::mat4>("projection", proj);
}

void BasicShader::setModelMatrix(glm::mat4 model){
    setUniform<glm::mat4>("model", model);
}

void BasicShader::setViewMatrix(glm::mat4 view){
    setUniform<glm::mat4>("view", view);
}

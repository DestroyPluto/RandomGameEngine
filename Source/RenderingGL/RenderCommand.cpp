#include "RenderCommand.h"
#include "Geometry.h"
#include "PointArray.h"
#include <glm/gtc/matrix_transform.hpp>

using namespace rendering;
using namespace core;

RenderCommand::RenderCommand(uint32_t id, Mesh* mesh, uint32_t textureId, uint32_t key){
    m_geometry = new Geometry(id); //memory owndership :/
    m_geometry->addAttribute(Attribute::aVertex, mesh->getPoints().toFloatVector());
    m_geometry->addAttribute(Attribute::aTexture, mesh->getUvs());
    m_modelMatrix = glm::mat4(1.0f); //make sure it is initialized
    m_geometry->addTexture(textureId);
    m_key = key;
}

void RenderCommand::setTextureID(uint32_t texID){
    m_geometry->addTexture(texID);
}

void RenderCommand::execute(Shader* shader){
    shader->setModelMatrix(m_modelMatrix);
    m_geometry->drawGeometry();
}

void RenderCommand::updateModelMatrix(glm::vec3 pos, glm::vec3 rot, glm::vec3 scale){
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, pos);
    //TODO: implement this properly
    //model = glm::rotate(model, rot);
    model = glm::scale(model, scale);

    m_modelMatrix = model;
}
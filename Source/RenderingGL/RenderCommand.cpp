#include "RenderCommand.h"
#include "Geometry.h"
#include "PointArray.h"

using namespace rendering;
using namespace core;

RenderCommand::RenderCommand(uint32_t id, Mesh* mesh){
    m_geometry = new Geometry(id); //memory owndership :/
    m_geometry->addAttribute(Attribute::aVertex, mesh->getPoints().toFloatVector());
}

void RenderCommand::execute(){
    m_geometry->drawGeometry();
}
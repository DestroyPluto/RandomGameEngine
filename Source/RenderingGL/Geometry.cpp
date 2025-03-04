#include "Geometry.h"
#include "glad/glad.h"

using namespace rendering;

Geometry::Geometry(uint32_t globalId){
    m_globalId = globalId;
    glGenVertexArrays(1, &m_vaoId);
}

//maybe make this a template?
void Geometry::addAttribute(Attribute attrib, std::vector<float> attribData){
    
    //could possibly change this to a switch statement?
  //  if(attrib == aVertex){
        m_vertexCount = attribData.size() / 3; //TODO magic numbers :/

        glBindVertexArray(m_vaoId);
        
        unsigned int VBO;
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, attribData.size() * sizeof(float), attribData.data(), GL_STATIC_DRAW);
    
        glVertexAttribPointer(attrib, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(attrib);
        glBindVertexArray(0); //reset state
  //  }
}

void Geometry::addAttribute(Attribute attrib, std::vector<unsigned int> attribData){
        glBindVertexArray(m_vaoId);
        unsigned int VBO;
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, attribData.size() * sizeof(unsigned int), attribData.data(), GL_STATIC_DRAW);
    
        glVertexAttribPointer(attrib, 2, GL_UNSIGNED_INT, GL_FALSE, 0, (void*)0);
        glEnableVertexAttribArray(attrib);
        glBindVertexArray(0); //reset state
}

void Geometry::drawGeometry(){
  glEnable(GL_BLEND);
  glBindTexture(GL_TEXTURE_2D, m_textureId);
  
  glBindVertexArray(m_vaoId);
  glDrawArrays(GL_TRIANGLES, 0, m_vertexCount);
  
  glBindTexture(GL_TEXTURE_2D, 0);
  glDisable(GL_BLEND);
}
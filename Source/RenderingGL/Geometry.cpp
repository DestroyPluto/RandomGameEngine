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

void Geometry::setIndices(std::vector<unsigned int> indices){
    if(indices.size() == 0)
        return;
    m_indexCount = indices.size();
    glBindVertexArray(m_vaoId);
    glGenBuffers(1, &m_eboId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_eboId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
    glBindVertexArray(0); //reset state
}

void Geometry::drawGeometry(){
  glEnable(GL_BLEND);
  if (m_textureId != 0)
    glBindTexture(GL_TEXTURE_2D, m_textureId);
  else {
      glBindTexture(GL_TEXTURE_2D, 0);
  }
  glBindVertexArray(m_vaoId);

  if(m_indexCount > 0){
        glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, 0);
  }
  else {
      glDrawArrays(GL_TRIANGLES, 0, m_vertexCount);
  }
  
  glBindTexture(GL_TEXTURE_2D, 0);
  glDisable(GL_BLEND);
}
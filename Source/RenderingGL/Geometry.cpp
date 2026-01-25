#include "Geometry.h"
#include "glad/glad.h"

using namespace rendering;

Geometry::Geometry(uint32_t globalId){
    m_globalId = globalId;
    glGenVertexArrays(1, &m_vaoId);
}

// existing convenience: default float attributes use 3 components
void Geometry::addAttribute(Attribute attrib, std::vector<float> attribData){
    addAttribute(attrib, attribData, 3);
}

// new overload that accepts component count
void Geometry::addAttribute(Attribute attrib, std::vector<float> attribData, int components){
    if(attribData.empty() || components <= 0) return;

    // Only set vertex count when supplying vertex positions
    if(attrib == aVertex){
        m_vertexCount = static_cast<uint32_t>(attribData.size()) / static_cast<uint32_t>(components);
    }

    glBindVertexArray(m_vaoId);
    
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, attribData.size() * sizeof(float), attribData.data(), GL_STATIC_DRAW);

    // components used here (3 for vertex/normals, 2 for texcoords if needed)
    glVertexAttribPointer(static_cast<GLuint>(attrib), components, GL_FLOAT, GL_FALSE, components * sizeof(float), (void*)0);
    glEnableVertexAttribArray(static_cast<GLuint>(attrib));
    glBindVertexArray(0); //reset state
}

void Geometry::addAttribute(Attribute attrib, std::vector<unsigned int> attribData){
    if(attribData.empty()) return;
    glBindVertexArray(m_vaoId);
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, attribData.size() * sizeof(unsigned int), attribData.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(static_cast<GLuint>(attrib), 2, GL_UNSIGNED_INT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(static_cast<GLuint>(attrib));
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
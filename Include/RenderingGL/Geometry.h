#pragma once
#include <cstdint>
#include <vector>

namespace rendering{
    
    typedef enum{
        aVertex = 0,
        aTexture = 1,
        aNormal = 2,
    } Attribute;

class Geometry{
   
public:
    Geometry(uint32_t globalId);
    void addAttribute(Attribute attrib, std::vector<float> attribData);
    void addAttribute(Attribute attrib, std::vector<float> attribData, int components);
    void addAttribute(Attribute attrib, std::vector<unsigned int> attribData);
    void drawGeometry();
    void addTexture(uint32_t texID){m_textureId = texID;};
    void setIndices(std::vector<unsigned int> indices);
    //needs to create a VAO/VBO... probably also needs someway to update it... 
    //and probably have the id as well for better tracing?
private:
    uint32_t m_globalId;
    uint32_t m_vaoId = 0;
    uint32_t m_vertexCount = 0;
    uint32_t m_textureId = 0;
    uint32_t m_indexCount = 0;
    uint32_t m_eboId = 0;
};
}
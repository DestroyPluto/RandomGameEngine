#pragma once
#include <cstdint>
#include <vector>

namespace rendering{
    
    typedef enum{
        aVertex = 0
    } Attribute;

class Geometry{
   
public:
    Geometry(uint32_t globalId);
    void addAttribute(Attribute attrib, std::vector<float> attribData);
    //needs to create a VAO/VBO... probably also needs someway to update it... 
    //and probably have the id as well for better tracing?
private:
    uint32_t m_globalId;
    uint32_t m_vaoId;
};
}
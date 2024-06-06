#pragma once
#include "Mesh.h"
#include <string>
namespace io {

class OBJLoader {
public:    
    static core::HgError loadFromFile(const char* filePath, core::Mesh& mesh);
 
private:
    
typedef struct Face {
    unsigned int vertexIndex;
    unsigned int uvIndex;
    unsigned int normalIndex;
} Face;

    static Face* parseFaceIndex(const std::string& face);
};
}
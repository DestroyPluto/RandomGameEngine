#pragma once
#include <cstdint>
#include <glad/glad.h>
#include <string>

namespace rendering{
class Shader {
    //TODO: not entirely sure if I like it like this...
    typedef enum uniformName{
        eModelViewMatrix = 0,
        eProjectionMatrix = 1
    } eUniformName;
public:
    uint32_t m_id;
    Shader(const char* vertexPath, const char* fragmentPath);
    void bind();

    template<typename t>
    void setUniform(eUniformName name, t data);
private:
    std::string m_locations[2]{
        "modelViewMatrx",
        "projectionMatrix"
    };
    //TODO: uniform functions
};
}
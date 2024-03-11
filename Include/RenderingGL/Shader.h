#pragma once
#include <cstdint>
#include <glad/glad.h>

namespace rendering{
class Shader {
public:
    uint32_t m_id;
    Shader(const char* vertexPath, const char* fragmentPath);
    void bind();
    //TODO: uniform functions
};
}
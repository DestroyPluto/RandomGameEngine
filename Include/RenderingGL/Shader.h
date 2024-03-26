#pragma once
#include <cstdint>
#include <glad/glad.h>
#include <string>

namespace rendering{
class Shader {
public:
    uint32_t m_id;
    Shader(const char* vertexPath, const char* fragmentPath);
    void bind();
    void unBind();

protected:
    template<typename t>
    void setUniform(std::string name, t data);

private:
};
}
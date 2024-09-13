#pragma once
#include <cstdint>
#include <glad/glad.h>
#include <string>
#include <glm/mat4x4.hpp>

namespace rendering{
class Shader {
public:
    uint32_t m_id;
    Shader(const char* vertexPath, const char* fragmentPath);
    void bind();
    void unBind();

    void setModelMatrix(glm::mat4 model);
    void setViewMatrix(glm::mat4 view);
    void setProjectionMatrix(glm::mat4 projection);

protected:
    template<typename t>
    void setUniform(std::string name, t data);

private:
};
}
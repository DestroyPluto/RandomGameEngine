#pragma once
#include <cstdint>
#include <glad/glad.h>
#include <string>
#include <glm/mat4x4.hpp>
#include <Config.h>
#include <memory>


namespace rendering{
class Shader {
public:
    uint32_t m_id;
    Shader(const char* vertexPath, const char* fragmentPath, std::shared_ptr<core::Config> config);
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
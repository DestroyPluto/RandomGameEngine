#pragma once
#include "Geometry.h"
#include <Mesh.h>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include "shader.h"

namespace rendering{
    class RenderCommand{
    public:
        RenderCommand(uint32_t globalId, core::Mesh* mesh); //since render command owns the geometry object, it should be responsible for creating it.
        void execute(Shader* shader);
        void updateModelMatrix(glm::vec3 pos, glm::vec3 rot, glm::vec3 scale);

    private:
        uint32_t m_globalId;
        Geometry* m_geometry;
        glm::mat4 m_modelMatrix;
    };
}
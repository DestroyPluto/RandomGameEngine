#pragma once
#include "Geometry.h"
#include <Mesh.h>

namespace rendering{
    class RenderCommand{
    public:
        RenderCommand(uint32_t globalId, core::Mesh* mesh); //since render command owns the geometry object, it should be responsible for creating it.
        void excecute();
    private:
        uint32_t m_globalId;
        Geometry* m_geometry;
    };
}
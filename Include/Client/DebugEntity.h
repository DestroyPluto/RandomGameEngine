#pragma once
#include "Entity.h"

namespace client {

    typedef enum eDebugEntityType {
        debug_Box,
        debug_line
    } DebugEntityType;

    #include <glm/vec3.hpp>

    class DebugEntity : public core::Entity {
    
    public:
        // Optional color parameter (RGB). Defaults to red.
        DebugEntity(uint32_t id, DebugEntityType type, glm::vec3 position, glm::vec3 color = glm::vec3(1.0f, 0.0f, 0.0f));

        virtual ~DebugEntity() {}
        virtual void onCollision() override {};
    
    private:
        DebugEntityType m_type;
    };
};
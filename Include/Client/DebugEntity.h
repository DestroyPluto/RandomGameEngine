#pragma once
#include "Entity.h"

namespace client {

    typedef enum eDebugEntityType {
        debug_Box,
        debug_line
    } DebugEntityType;

    class DebugEntity : public core::Entity {
    
    public:
        DebugEntity(uint32_t id, DebugEntityType type, glm::vec3 position);

        virtual ~DebugEntity() {}
        virtual void onCollision() override {};
    
    private:
        DebugEntityType m_type;
    };
};
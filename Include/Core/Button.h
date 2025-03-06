#pragma once
#include "Entity.h"

namespace core {
    class Button : public Entity {
        public:
            Button(uint32_t id);
            virtual void onCollision(Entity* other) override;
            virtual void onCollision() override;
            virtual void onClick();
    };
}
#pragma once
#include "Entity.h"
#include "Behaviour.h"

namespace core{
    class GameObject : public Entity{
        public:
        GameObject(uint32_t id, Behaviour* behaviour);
        virtual ~GameObject();
        virtual void onUpdate() override;
        private:
        Behaviour* m_behaviour;
    };
}
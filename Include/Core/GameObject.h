#pragma once
#include "Entity.h"
#include "Behaviour.h"

namespace core{
    class GameObject : public Entity{
        public:
            GameObject(uint32_t id, Behaviour* behaviour);
            GameObject(uint32_t id, Behaviour* behaviour, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);
            virtual ~GameObject();
            virtual void onUpdate() override;
            virtual void onClick() override;
            virtual void onCollision() override;
            Behaviour* getBehaviour(){return m_behaviour;}
            
        private:
            Behaviour* m_behaviour;
            bool m_clickProgress;

    };
}
#pragma once
#include "Behaviour.h"
namespace client{
    class TestBehaviour : public core::Behaviour{
        public:
        virtual void update() override;
        virtual void initialize() override;
        private:
        bool m_up;
        float m_speed;
    };
}
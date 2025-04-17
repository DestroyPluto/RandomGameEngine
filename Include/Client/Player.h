#pragma once
#include "Behaviour.h"

namespace client{
    class Player : public core::Behaviour{
        public:
        virtual void update() override;
        virtual void initialize() override;
        virtual std::string const getName() override {
            //TODO: must be a better way of doing this
            return "Player";
        }

        private:
        float m_speed = 2.0;
        void handleMovement();
    };
}
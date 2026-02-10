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
        float m_yaw = 0.0f;
        float m_pitch = 0.0f;
        double m_lastMouseX = 0.0;
        double m_lastMouseY = 0.0;
        bool m_hasMouse = false;
        float m_mouseSensitivity = 0.5f;
        void handleMovement();
    };
}
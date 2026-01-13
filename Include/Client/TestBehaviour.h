#pragma once
#include "Behaviour.h"
namespace client{
    class TestBehaviour : public core::Behaviour {
        public:

        virtual void update() override;
        virtual void initialize() override;
        virtual void onClick() override;
        //TODO: must be a better way of doing this
        virtual std::string const getName() override {
            //TODO: must be a better way of doing this
            return "TestBehaviour";
        }

        private:
        bool m_up;
        float m_speed;
    };
}
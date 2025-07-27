#pragma once
#include "Config.h"
#include <memory>

namespace client{
    class GameManager{
        public:
        GameManager(std::shared_ptr<core::Config> config);
        void update();
        bool shouldEnd(){return m_shouldEnd;}

        private:
            bool m_shouldEnd = false;
    };
}
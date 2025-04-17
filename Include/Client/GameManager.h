#pragma once

namespace client{
    class GameManager{
        public:
        GameManager();
        void update();
        bool shouldEnd(){return m_shouldEnd;}

        private:
            bool m_shouldEnd = false;
    };
}